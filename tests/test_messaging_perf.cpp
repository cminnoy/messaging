/**
 * @file test_perf.hpp
 * @author Chris Minnoy
 * @version 1.0
 * @date 2022
 * 
 * @copyright Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

//#define WITH_VISITOR_STATS

#include <messaging/process_manager.hpp>

#include <chrono>
#include <limits>
#include <vector>
#include <random>
#include <variant>

#include <util/logger.hpp>
#include <util/pmte.hpp>
#include <util/algorithm.hpp>
#include <gtest/gtest.h>

struct some_message {
  int a;
};

class base {
public:
  virtual ~base() noexcept {}
  base() : a(0) {}
  base(int a_) : a(a_) {}
  int a;
};

class derived : public base {
public:
  derived() : base(), b(0) {}
  derived(int a_, int b_) : base(a_), b(b_) {}
  int b;
};

enum class some_enum { high, normal, low };

struct class1 {
  virtual ~class1() noexcept {}
};
struct class2 : public class1 {};
struct class3 : public class2 {};

using counter_type = unsigned long long int;

#if defined(WITH_GCOV) // to reduce testing time; performance under debug isn't relevant anyhow
constexpr counter_type LOOP_COUNT = 1'000'000ULL;
constexpr counter_type PRODUCER_MAX_COUNT = 4ULL;
#else
constexpr counter_type LOOP_COUNT = 5'000'000ULL;
constexpr counter_type PRODUCER_MAX_COUNT = 10ULL;
#endif

class consumer_type : public messaging::multithread::std_thread_types::process_type {
public:

  using self = consumer_type;
  using base = typename messaging::multithread::std_thread_types::process_type;
  using receiver_type = typename base::receiver_type;
  using sender_type = typename base::sender_type;
  using dispatcher_type = typename base::dispatcher_type;

  struct setup {
    counter_type max;
  };

private:

  counter_type max_count = std::numeric_limits<counter_type>::max();
  counter_type class_type_count = 0ULL;
  counter_type int_type_count = 0ULL;
  counter_type enum_type_count = 0ULL;

  void dispatch(dispatcher_type & dispatcher) override {
    (dispatcher)
      .template handle<setup>([&](setup & msg) { max_count = msg.max; })
      .template handle<some_message>([&](some_message & msg) {
        ASSERT_EQ(msg.a, 17);
        class_type_count++;
      })
      .template handle<int>([&](int msg) {
        ASSERT_EQ(msg, 5);
        int_type_count++;
      })
      .template handle<some_enum>([&](some_enum & msg) {
        ASSERT_EQ(msg, some_enum::high);
        enum_type_count++;
      });
    if (max_count == int_type_count && max_count == enum_type_count && max_count == class_type_count) base::done();
  }
};

class consumer_type2 : public messaging::multithread::std_thread_types::process_type {
public:

  using self = consumer_type;
  using base = typename messaging::multithread::std_thread_types::process_type;
  using receiver_type = typename base::receiver_type;
  using sender_type = typename base::sender_type;
  using dispatcher_type = typename base::dispatcher_type;

  struct setup {
    counter_type max;
  };

  consumer_type2() {
    (*this)
      .handle<setup>([&](setup & msg) { max_count = msg.max; })
      .handle([&](some_message & msg) {
        ASSERT_EQ(msg.a, 17);
        class_type_count++;
      })
      .handle([&](int msg) {
        ASSERT_EQ(msg, 5);
        int_type_count++;
      })
      .handle([&](some_enum & msg) {
        ASSERT_EQ(msg, some_enum::high);
        enum_type_count++;
      });
  }

private:

  counter_type max_count = std::numeric_limits<counter_type>::max();
  counter_type class_type_count = 0ULL;
  counter_type int_type_count = 0ULL;
  counter_type enum_type_count = 0ULL;

  void dispatch(dispatcher_type &) override {
    if (max_count == int_type_count && max_count == enum_type_count && max_count == class_type_count) base::done();
  }
};

class producer_type final : public messaging::multithread::std_thread_types::process_type {
  using self = producer_type;
  using base = typename messaging::multithread::std_thread_types::process_type;
  using receiver_type = typename base::receiver_type;
  using sender_type = typename base::sender_type;
  using dispatcher_type = typename base::dispatcher_type;

  void dispatch(dispatcher_type & dispatcher) override {
    dispatcher.template handle<self::exec>([&](self::exec & cmd) { cmd(); });
  }

public:

  struct exec {

    sender_type destination;

    exec(sender_type destination_) : destination(destination_) {}

    void operator()() {
      for (counter_type i = 0; i < LOOP_COUNT; ++i) {
        destination.send(some_message {17});
        destination.send(5);
        destination.send(some_enum::high);
      }
    }
  };

  using base::base;
};

TEST(PerformanceTest, CreateWrappedMessageVsCreatePMTE) {
  using namespace messaging;
  using namespace util;
  using namespace util::pmte;
  using namespace std::chrono_literals;
  using namespace util::logger;

  // PMTE int
  {
    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    for (counter_type i = 0; i < LOOP_COUNT; ++i) {
      auto p = util::pmte::make_unique_pmte(5);
      ASSERT_NE(p.get(), nullptr);
    }
    std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
    const auto diff_time = end_time - start_time;
    const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
    info << "It took " << milliseconds << "ms to create/destroy " << LOOP_COUNT << " instances using make_unique_pmte<int>." << std::endl;
  }

  // PMTE class
  {
    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    for (counter_type i = 0; i < LOOP_COUNT; ++i) {
      auto p = make_unique_pmte(derived {5, 10});
      ASSERT_NE(p.get(), nullptr);
    }
    std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
    const auto diff_time = end_time - start_time;
    const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
    info << "It took " << milliseconds << "ms to create/destroy " << LOOP_COUNT << " instances using make_unique_pmte(derived)." << std::endl;
  }

  // PMTE class + ref
  {
    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    for (counter_type i = 0; i < LOOP_COUNT; ++i) {
      auto p = make_unique_pmte<base>(derived {5, 10});
      ASSERT_NE(p.get(), nullptr);
    }
    std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
    const auto diff_time = end_time - start_time;
    const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
    info << "It took " << milliseconds << "ms to create/destroy " << LOOP_COUNT << " instances using make_unique_pmte<base>(derived)."
         << std::endl;
  }

  // PMTE class + type_list
  {
    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    for (counter_type i = 0; i < LOOP_COUNT; ++i) {
      auto p = make_unique_pmte(derived {5, 10}, type_list<base, derived> {});
      ASSERT_NE(p.get(), nullptr);
    }
    std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
    const auto diff_time = end_time - start_time;
    const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
    info << "It took " << milliseconds << "ms to create/destroy " << LOOP_COUNT << " instances using make_unique_pmte(derived, type_list)."
         << std::endl;
  }

#if __cplusplus >= CPP17_STANDARD && __has_include(<memory_resource>)
  // Unsynchronized resource with plain int
  {
    std::pmr::unsynchronized_pool_resource resource;
    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    for (counter_type i = 0; i < LOOP_COUNT; ++i) {
      int * p = (int *)resource.allocate(sizeof(int));
      ASSERT_NE(p, nullptr);
      *p = 5;
      resource.deallocate(p, sizeof(int));
    }
    std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
    const auto diff_time = end_time - start_time;
    const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
    info << "It took " << milliseconds << "ms to create/destroy " << LOOP_COUNT << " instances of int with synchronized allocator." << std::endl;
  }

  // Unsynchronized resource with PMTE int
  {
    std::pmr::unsynchronized_pool_resource resource;
    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    for (counter_type i = 0; i < LOOP_COUNT; ++i) {
      auto p = util::pmte::make_unique_pmte(resource, 5);
      ASSERT_NE(p, nullptr);
    }
    std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
    const auto diff_time = end_time - start_time;
    const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
    info << "It took " << milliseconds << "ms to create/destroy " << LOOP_COUNT
         << " instances using make_unique_pmte<int> with synchronized allocator." << std::endl;
  }

  // Monotonic resource with plain int
  {
    std::pmr::monotonic_buffer_resource resource;
    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    for (counter_type i = 0; i < LOOP_COUNT; ++i) {
      int * p = (int *)resource.allocate(sizeof(int));
      ASSERT_NE(p, nullptr);
      *p = 5;
      resource.deallocate(p, sizeof(int));
    }
    std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
    const auto diff_time = end_time - start_time;
    const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
    info << "It took " << milliseconds << "ms to create/destroy " << LOOP_COUNT << " instances of int with monotonic allocator." << std::endl;
  }

  // Monotonic resource with PMTE int
  {
    std::pmr::monotonic_buffer_resource resource;
    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    for (counter_type i = 0; i < LOOP_COUNT; ++i) {
      auto p = util::pmte::make_unique_pmte(resource, 5);
      ASSERT_NE(p, nullptr);
    }
    std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
    const auto diff_time = end_time - start_time;
    const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
    info << "It took " << milliseconds << "ms to create/destroy " << LOOP_COUNT
         << " instances using make_unique_pmte<int> with monotonic allocator." << std::endl;
  }
#endif
}

TEST(PerformanceTest, BasicTypeDetection) {
  using namespace messaging;
  using namespace util::pmte;
  using namespace std::chrono_literals;
  using namespace util::logger;

  // dynamic_cast
  {
    // bool, int, char, float, class1, class2, class3
    std::vector<pmte_value *> v;
    v.push_back(make_pmte<bool>(true));
    v.push_back(make_pmte(8));
    v.push_back(make_pmte('b'));
    v.push_back(make_pmte(9.7f));
    v.push_back(make_pmte(class1 {}));
    v.push_back(make_pmte(class2 {}));
    v.push_back(make_pmte(class3 {}));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, (int)v.size() - 1);

    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    for (counter_type i = 0; i < LOOP_COUNT; ++i) {
      int index = distribution(gen);
      pmte_value * o = v[index];
      bool is_bool = dynamic_cast<util::pmte::detail::value_store<bool> *>(o);
      bool is_int = dynamic_cast<util::pmte::detail::value_store<int> *>(o);
      bool is_char = dynamic_cast<util::pmte::detail::value_store<char> *>(o);
      bool is_float = dynamic_cast<util::pmte::detail::value_store<float> *>(o);
      bool is_class1 = dynamic_cast<util::pmte::detail::value_store<class1> *>(o);
      bool is_class2 = dynamic_cast<util::pmte::detail::value_store<class2> *>(o);
      bool is_class3 = dynamic_cast<util::pmte::detail::value_store<class3> *>(o);
      switch (index) {
      case 0: {
        EXPECT_TRUE(is_bool);
        EXPECT_FALSE(is_int);
        EXPECT_FALSE(is_char);
        EXPECT_FALSE(is_float);
        EXPECT_FALSE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 1: {
        EXPECT_FALSE(is_bool);
        EXPECT_TRUE(is_int);
        EXPECT_FALSE(is_char);
        EXPECT_FALSE(is_float);
        EXPECT_FALSE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 2: {
        EXPECT_FALSE(is_bool);
        EXPECT_FALSE(is_int);
        EXPECT_TRUE(is_char);
        EXPECT_FALSE(is_float);
        EXPECT_FALSE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 3: {
        EXPECT_FALSE(is_bool);
        EXPECT_FALSE(is_int);
        EXPECT_FALSE(is_char);
        EXPECT_TRUE(is_float);
        EXPECT_FALSE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 4: {
        EXPECT_FALSE(is_bool);
        EXPECT_FALSE(is_int);
        EXPECT_FALSE(is_char);
        EXPECT_FALSE(is_float);
        EXPECT_TRUE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 5: {
        EXPECT_FALSE(is_bool);
        EXPECT_FALSE(is_int);
        EXPECT_FALSE(is_char);
        EXPECT_FALSE(is_float);
        EXPECT_FALSE(is_class1);
        EXPECT_TRUE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 6: {
        EXPECT_FALSE(is_bool);
        EXPECT_FALSE(is_int);
        EXPECT_FALSE(is_char);
        EXPECT_FALSE(is_float);
        EXPECT_FALSE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_TRUE(is_class3);
        break;
      }
      default: ASSERT_TRUE(false);
      }
    }
    std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
    const auto diff_time = end_time - start_time;
    const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
    info << "It took " << milliseconds << "ms to check " << (LOOP_COUNT * v.size()) << " instances using dynamic_cast." << std::endl;
  }

  // type_info
  {
    // bool, int, char, float, class1, class2, class3
    std::vector<pmte_value *> v;
    v.push_back(make_pmte<bool>(true));
    v.push_back(make_pmte(8));
    v.push_back(make_pmte('b'));
    v.push_back(make_pmte(9.7f));
    v.push_back(make_pmte(class1 {}));
    v.push_back(make_pmte(class2 {}));
    v.push_back(make_pmte(class3 {}));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, (int)v.size() - 1);

    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    for (counter_type i = 0; i < LOOP_COUNT; ++i) {
      int index = distribution(gen);
      pmte_value * o = v[index];
      bool is_bool = is_type_of<bool>(o);
      bool is_int = is_type_of<int>(o);
      bool is_char = is_type_of<char>(o);
      bool is_float = is_type_of<float>(o);
      bool is_class1 = is_type_of<class1>(o);
      bool is_class2 = is_type_of<class2>(o);
      bool is_class3 = is_type_of<class3>(o);
      switch (index) {
      case 0: {
        EXPECT_TRUE(is_bool);
        EXPECT_FALSE(is_int);
        EXPECT_FALSE(is_char);
        EXPECT_FALSE(is_float);
        EXPECT_FALSE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 1: {
        EXPECT_FALSE(is_bool);
        EXPECT_TRUE(is_int);
        EXPECT_FALSE(is_char);
        EXPECT_FALSE(is_float);
        EXPECT_FALSE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 2: {
        EXPECT_FALSE(is_bool);
        EXPECT_FALSE(is_int);
        EXPECT_TRUE(is_char);
        EXPECT_FALSE(is_float);
        EXPECT_FALSE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 3: {
        EXPECT_FALSE(is_bool);
        EXPECT_FALSE(is_int);
        EXPECT_FALSE(is_char);
        EXPECT_TRUE(is_float);
        EXPECT_FALSE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 4: {
        EXPECT_FALSE(is_bool);
        EXPECT_FALSE(is_int);
        EXPECT_FALSE(is_char);
        EXPECT_FALSE(is_float);
        EXPECT_TRUE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 5: {
        EXPECT_FALSE(is_bool);
        EXPECT_FALSE(is_int);
        EXPECT_FALSE(is_char);
        EXPECT_FALSE(is_float);
        EXPECT_FALSE(is_class1);
        EXPECT_TRUE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 6: {
        EXPECT_FALSE(is_bool);
        EXPECT_FALSE(is_int);
        EXPECT_FALSE(is_char);
        EXPECT_FALSE(is_float);
        EXPECT_FALSE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_TRUE(is_class3);
        break;
      }
      default: ASSERT_TRUE(false);
      }
    }
    std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
    const auto diff_time = end_time - start_time;
    const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
    info << "It took " << milliseconds << "ms to check " << (LOOP_COUNT * v.size()) << " instances using is_type_of." << std::endl;
  }
}

TEST(PerformanceTest, PolymorphicTypeDetection) {
  using namespace messaging;
  using namespace util;
  using namespace util::pmte;
  using namespace std::chrono_literals;
  using namespace util::logger;

  // dynamic_cast
  {
    // class1, class2, class3
    std::vector<pmte_value *> v;
    v.push_back(make_pmte(true));
    v.push_back(make_pmte(class1 {}));
    v.push_back(make_pmte(class2 {}));
    v.push_back(make_pmte(class3 {}));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, (int)v.size() - 1);

    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    for (counter_type i = 0; i < LOOP_COUNT; ++i) {
      int index = distribution(gen);
      pmte_value * o = v[index];
      auto c1 = dynamic_cast<util::pmte::detail::value_store<class1> *>(o);
      auto c2 = dynamic_cast<util::pmte::detail::value_store<class2> *>(o);
      auto c3 = dynamic_cast<util::pmte::detail::value_store<class3> *>(o);
      bool is_class1 = c1 ?
        true :
        (c2 ? (dynamic_cast<class1 *>(&c2->get()) != nullptr) : (c3 ? (dynamic_cast<class1 *>(&c3->get()) != nullptr) : false));
      bool is_class2 = c2 ?
        true :
        (c1 ? (dynamic_cast<class2 *>(&c1->get()) != nullptr) : (c3 ? (dynamic_cast<class2 *>(&c3->get()) != nullptr) : false));
      bool is_class3 = c3 ?
        true :
        (c1 ? (dynamic_cast<class3 *>(&c1->get()) != nullptr) : (c2 ? (dynamic_cast<class3 *>(&c2->get()) != nullptr) : false));
      switch (index) {
      case 0: {
        EXPECT_FALSE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 1: {
        EXPECT_TRUE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 2: {
        EXPECT_TRUE(is_class1);
        EXPECT_TRUE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 3: {
        EXPECT_TRUE(is_class1);
        EXPECT_TRUE(is_class2);
        EXPECT_TRUE(is_class3);
        break;
      }
      default: ASSERT_TRUE(false);
      }
    }
    std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
    const auto diff_time = end_time - start_time;
    const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
    info << "It took " << milliseconds << "ms to polymorphicaly check " << LOOP_COUNT << " instances using dynamic_cast." << std::endl;
  }

  // is_castable_to
  {
    // class1, class2, class3
    std::vector<pmte_value *> v;
    v.push_back(make_pmte(true));
    v.push_back(make_pmte(class1 {}));
    v.push_back(make_pmte(class2 {}, type_list<class1> {}));
    v.push_back(make_pmte(class3 {}, type_list<class2, class1> {}));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, (int)v.size() - 1);

    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    for (counter_type i = 0; i < LOOP_COUNT; ++i) {
      int index = distribution(gen);
      pmte_value * o = v[index];
      bool is_class1 = is_castable_to<class1>(o);
      bool is_class2 = is_castable_to<class2>(o);
      bool is_class3 = is_castable_to<class3>(o);
      switch (index) {
      case 0: {
        EXPECT_FALSE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 1: {
        EXPECT_TRUE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 2: {
        EXPECT_TRUE(is_class1);
        EXPECT_TRUE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 3: {
        EXPECT_TRUE(is_class1);
        EXPECT_TRUE(is_class2);
        EXPECT_TRUE(is_class3);
        break;
      }
      default: ASSERT_TRUE(false);
      }
    }
    std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
    const auto diff_time = end_time - start_time;
    const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
    info << "It took " << milliseconds << "ms to polymorphicaly check " << LOOP_COUNT << " instances using is_castable_to." << std::endl;
  }

  // get_if
  {
    // class1, class2, class3
    std::vector<pmte_value *> v;
    v.push_back(make_pmte(true));
    v.push_back(make_pmte(class1 {}));
    v.push_back(make_pmte(class2 {}, type_list<class1> {}));
    v.push_back(make_pmte(class3 {}, type_list<class2, class1> {}));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, (int)v.size() - 1);

    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    for (counter_type i = 0; i < LOOP_COUNT; ++i) {
      int index = distribution(gen);
      pmte_value * o = v[index];
      class1 * c1 = get_if<class1>(o);
      class2 * c2 = get_if<class2>(o);
      class3 * c3 = get_if<class3>(o);
      bool is_class1 = c1;
      bool is_class2 = c2;
      bool is_class3 = c3;
      switch (index) {
      case 0: {
        EXPECT_FALSE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 1: {
        EXPECT_TRUE(is_class1);
        EXPECT_FALSE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 2: {
        EXPECT_TRUE(is_class1);
        EXPECT_TRUE(is_class2);
        EXPECT_FALSE(is_class3);
        break;
      }
      case 3: {
        EXPECT_TRUE(is_class1);
        EXPECT_TRUE(is_class2);
        EXPECT_TRUE(is_class3);
        break;
      }
      default: ASSERT_TRUE(false);
      }
    }
    std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
    const auto diff_time = end_time - start_time;
    const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
    info << "It took " << milliseconds << "ms to polymorphicaly check " << LOOP_COUNT << " instances using get_if." << std::endl;
  }
}

TEST(PerformanceTest, VisitorNegativeCache) {
  using namespace util::pmte;
  using namespace std::chrono_literals;
  using namespace util::logger;

  // bool, int, char, float, some_message, base, derived
  std::vector<pmte_value *> v;
  v.push_back(make_pmte<const bool>(true));
  v.push_back(make_pmte(8));
  v.push_back(make_pmte('b'));
  v.push_back(make_pmte(9.7f));
  v.push_back(make_pmte(some_message {8}));
  v.push_back(make_pmte(base {10}));
  v.push_back(make_pmte(derived {11, 12}));

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution(0, (int)v.size() - 1);

  pmte_visitor visitor;
  visitor.add([&](double) { ASSERT_FALSE(true); });

  std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
  for (counter_type i = 0; i < LOOP_COUNT; ++i) {
    int index = distribution(gen);
    pmte_value * o = v[index];
    visitor(o);
  }
  std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
  const auto diff_time = end_time - start_time;
  const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
  info << "It took " << milliseconds << "ms to handle " << LOOP_COUNT << " negative visits." << std::endl;

#ifdef WITH_VISITOR_STATS
  stats << "Lookups: " << visitor.lookups << std::endl;
  stats << "Hash cache hits: " << visitor.hash_cache_hits << std::endl;
  stats << "Binary search hits: " << visitor.binary_search_hits << std::endl;
  stats << "Binary search misses: " << visitor.binary_search_misses << std::endl;
#endif
}

TEST(PerformanceTest, VisitorPositiveCache) {
  using namespace util::pmte;
  using namespace std::chrono_literals;
  using namespace util::logger;

  // bool, int, char, float, class1, class2, class3
  std::vector<pmte_value *> v;
  v.push_back(make_pmte<const bool>(true));
  v.push_back(make_pmte(8));
  v.push_back(make_pmte('b'));
  v.push_back(make_pmte(9.7f));
  v.push_back(make_pmte(some_message {8}));
  v.push_back(make_pmte(base {10}));
  v.push_back(make_pmte(derived {11, 12}));

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution(0, (int)v.size() - 1);

  pmte_visitor visitor;
  visitor.add([](bool v) { ASSERT_EQ(v, true); });
  visitor.add([](int v) { ASSERT_EQ(v, 8); });
  visitor.add([](char v) { ASSERT_EQ(v, 'b'); });
  visitor.add([](float v) { ASSERT_EQ(v, 9.7f); });
  visitor.add([](some_message & v) { ASSERT_EQ(v.a, 8); });
  visitor.add([](base & v) { ASSERT_EQ(v.a, 10); });
  visitor.add([](derived & v) {
    ASSERT_EQ(v.a, 11);
    ASSERT_EQ(v.b, 12);
  });

#ifdef WITH_VISITOR_STATS
  debug << "Hashes of types:" << std::endl;
  for (const auto ptr : v) {
    auto type = ptr->type();
    debug << get_name(type) << ": 0x" << std::hex << get_hash_code(type) << std::dec << std::endl;
  }
#endif

  std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
  for (counter_type i = 0; i < LOOP_COUNT; ++i) {
    int index = distribution(gen);
    pmte_value * o = v[index];
    visitor(o);
  }
  std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
  const auto diff_time = end_time - start_time;
  const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
  info << "It took " << milliseconds << "ms to handle " << LOOP_COUNT << " positive visits." << std::endl;

#ifdef WITH_VISITOR_STATS
  stats << "Lookups: " << visitor.lookups << std::endl;
  stats << "Hash cache hits: " << visitor.hash_cache_hits << std::endl;
  stats << "Binary search hits: " << visitor.binary_search_hits << std::endl;
  stats << "Binary search misses: " << visitor.binary_search_misses << std::endl;
#endif
}

#if __cplusplus >= CPP17_STANDARD
TEST(PerformanceTest, StdVisit) {
  using namespace util::pmte;
  using namespace std::chrono_literals;
  using namespace util::logger;

  // bool, int, char, float, some_message, base, derived
  using my_variant_type = std::variant<bool, int, char, float, some_message, base, derived>;

  std::vector<my_variant_type> v;
  v.push_back(true);
  v.push_back(8);
  v.push_back('b');
  v.push_back(9.7f);
  v.push_back(some_message {8});
  v.push_back(base {10});
  v.push_back(derived {11, 12});

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution distribution(0, (int)v.size() - 1);

  // clang-format off
  auto visitor = overload(
    [](bool           v) { ASSERT_EQ(v, true); },
    [](int            v) { ASSERT_EQ(v, 8); },
    [](char           v) { ASSERT_EQ(v, 'b'); },
    [](float          v) { ASSERT_EQ(v, 9.7f); },
    [](some_message & v) { ASSERT_EQ(v.a, 8); },
    [](base         & v) { ASSERT_EQ(v.a, 10 ); },
    [](derived      & v) { ASSERT_EQ(v.a, 11); ASSERT_EQ(v.b, 12); }
    );
  // clang-format on

  std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
  for (counter_type i = 0; i < LOOP_COUNT; ++i) {
    int index = distribution(gen);
    my_variant_type & o = v[index];
    std::visit(visitor, o);
  }
  std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
  const auto diff_time = end_time - start_time;
  const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
  info << "It took " << milliseconds << "ms to handle " << LOOP_COUNT << " positive visits using std::visit with a std::variant." << std::endl;
}
#endif

TEST(PerformanceTest, MultiProducerConsumerType1) {
  using multithread_types = messaging::multithread::std_thread_types;
  using namespace std::chrono_literals;
  using namespace util::logger;

  static_assert(PRODUCER_MAX_COUNT >= 1, "Minimal one producer required!");

  // single/multiple producer testing
  for (counter_type producer_count = 1ULL; producer_count <= PRODUCER_MAX_COUNT; ++producer_count) {
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    std::chrono::time_point<std::chrono::steady_clock> end_time;
    messaging::process_manager<multithread_types> manager;
    std::vector<std::string> producers;

    using consumer_type_t = consumer_type;

    auto consumer = manager.add<multithread_types>("consumer", std::make_shared<consumer_type_t>());
    (*consumer).send(consumer_type_t::setup {LOOP_COUNT * producer_count}).on_finish([&]() noexcept {
      end_time = std::chrono::steady_clock::now();
    });

    counter_type producers_active = 0;

    for (counter_type loop = 0; loop < producer_count; ++loop) {
      std::string name = "producer" + std::to_string(loop);
      auto producer = manager.add<multithread_types>(name, std::make_shared<producer_type>(false));
      producer->on_start([&]() noexcept { ++producers_active; });
      producer->on_finish([&]() noexcept { --producers_active; });
      producers.push_back(name);
    }

    start_time = std::chrono::steady_clock::now();

    // start producers
    for (auto && name : producers) {
      manager.start(name);
      auto producer = manager.get_process<multithread_types>(name);
      producer->send(producer_type::exec(consumer->get_sender<consumer_type_t::sender_type>()));
      producer->send(messaging::close_queue {});
    }

    // block until all done
    manager.run_every(100ms);

    const auto diff_time = end_time - start_time;
    const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
    info << "With " << producer_count << " producer(s) able to transmit " << ((LOOP_COUNT * 3) / milliseconds)
         << " messages per millisecond/producer." << std::endl;

#ifdef WITH_VISITOR_STATS
    stats << "Lookups: " << consumer->lookups << std::endl;
    stats << "Hash cache hits: " << consumer->hash_cache_hits << std::endl;
    stats << "Binary search hits: " << consumer->binary_search_hits << std::endl;
    stats << "Binary search misses: " << consumer->binary_search_misses << std::endl;
#endif
  }
}

TEST(PerformanceTest, MultiProducerConsumerType2) {
  using multithread_types = messaging::multithread::std_thread_types;
  using namespace std::chrono_literals;
  using namespace util::logger;

  static_assert(PRODUCER_MAX_COUNT >= 1, "Minimal one producer required!");

  // single/multiple producer testing
  for (counter_type producer_count = 1ULL; producer_count <= PRODUCER_MAX_COUNT; ++producer_count) {
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    std::chrono::time_point<std::chrono::steady_clock> end_time;
    messaging::process_manager<multithread_types> manager;
    std::vector<std::string> producers;

    using consumer_type_t = consumer_type2;

    auto consumer = manager.add<multithread_types>("consumer", std::make_shared<consumer_type_t>());
    (*consumer).send(consumer_type_t::setup {LOOP_COUNT * producer_count}).on_finish([&]() noexcept {
      end_time = std::chrono::steady_clock::now();
    });

    counter_type producers_active = 0;

    for (counter_type loop = 0; loop < producer_count; ++loop) {
      std::string name = "producer" + std::to_string(loop);
      auto producer = manager.add<multithread_types>(name, std::make_shared<producer_type>(false));
      producer->on_start([&]() noexcept { ++producers_active; });
      producer->on_finish([&]() noexcept { --producers_active; });
      producers.push_back(name);
    }

    start_time = std::chrono::steady_clock::now();

    // start producers
    for (auto && name : producers) {
      manager.start(name);
      auto producer = manager.get_process<multithread_types>(name);
      producer->send(producer_type::exec(consumer->get_sender<consumer_type_t::sender_type>()));
      producer->send(messaging::close_queue {});
    }

    // block until all done
    manager.run_every(100ms);

    const auto diff_time = end_time - start_time;
    const double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff_time).count();
    info << "With " << producer_count << " producer(s) able to transmit " << ((LOOP_COUNT * 3) / milliseconds)
         << " messages per millisecond/producer." << std::endl;

#ifdef WITH_VISITOR_STATS
    stats << "Lookups: " << consumer->lookups << std::endl;
    stats << "Hash cache hits: " << consumer->hash_cache_hits << std::endl;
    stats << "Binary search hits: " << consumer->binary_search_hits << std::endl;
    stats << "Binary search misses: " << consumer->binary_search_misses << std::endl;
#endif
  }
}
