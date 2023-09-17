#include <util/pmte.hpp>
#include <iostream>
#include <future>
#include <type_traits>
#include <gtest/gtest.h>

TEST(PMTE, CreateAndCallPMTEHandler) {
  using namespace util::pmte;

  // build_pmte_handler
  {
    int counter_true = 0;
    int counter_false = 0;

    auto handler = build_pmte_handler<bool>([&](bool v) {
      if (v) ++counter_true;
      else
        ++counter_false;
    });
    EXPECT_EQ(handler.size(), platform_is64bit ? 56 : 28);
    EXPECT_TRUE(is_type_of<bool>(handler));

    auto v0 = build_pmte(true);
    auto v1 = build_pmte(false);
    const auto v2 = build_pmte(true);
    const auto v3 = build_pmte(false);
    auto v4 = build_pmte(5);
    const auto v5 = build_pmte(5.6f);

    handler(v0);
    handler(v1);
    handler(v2);
    handler(v3);
    handler(v4);
    handler(v5);

    EXPECT_EQ(counter_true, 2);
    EXPECT_EQ(counter_false, 2);
  }

  // construct_pmte_handler_at
  {
    int counter_true = 0;
    int counter_false = 0;

    char buffer[256];
    void * address = &buffer;

    auto handler = construct_pmte_handler_at<bool>(address, [&](bool v) {
      if (v) ++counter_true;
      else
        ++counter_false;
    });
    ASSERT_NE(handler, nullptr);
    ASSERT_EQ(handler, address);
    EXPECT_EQ(handler->size(), platform_is64bit ? 56 : 28);
    EXPECT_TRUE(is_type_of<bool>(handler));

    auto v0 = build_pmte(true);
    auto v1 = build_pmte(false);
    const auto v2 = build_pmte(true);
    const auto v3 = build_pmte(false);
    auto v4 = build_pmte(5);
    const auto v5 = build_pmte(5.6f);

    (*handler)(v0);
    (*handler)(v1);
    (*handler)(v2);
    (*handler)(v3);
    (*handler)(v4);
    (*handler)(v5);

    EXPECT_EQ(counter_true, 2);
    EXPECT_EQ(counter_false, 2);
  }

  // make_pmte_handler
  {
    int counter_true = 0;
    int counter_false = 0;

    auto handler = make_pmte_handler<bool>([&](bool v) {
      if (v) ++counter_true;
      else
        ++counter_false;
    });
    ASSERT_NE(handler, nullptr);
    EXPECT_EQ(handler->size(), platform_is64bit ? 56 : 28);
    EXPECT_TRUE(is_type_of<bool>(handler));

    auto v0 = build_pmte(true);
    auto v1 = build_pmte(false);
    const auto v2 = build_pmte(true);
    const auto v3 = build_pmte(false);
    auto v4 = build_pmte(5);
    const auto v5 = build_pmte(5.6f);

    (*handler)(v0);
    (*handler)(v1);
    (*handler)(v2);
    (*handler)(v3);
    (*handler)(v4);
    (*handler)(v5);

    EXPECT_EQ(counter_true, 2);
    EXPECT_EQ(counter_false, 2);

    delete handler;
  }

  // make_unique_pmte_handler
  {
    int counter_true = 0;
    int counter_false = 0;

    auto handler = make_unique_pmte_handler<bool>([&](bool v) {
      if (v) ++counter_true;
      else
        ++counter_false;
    });
    ASSERT_NE(handler, nullptr);
    EXPECT_EQ(handler->size(), platform_is64bit ? 56 : 28);
    EXPECT_TRUE(is_type_of<bool>(handler));

    auto v0 = build_pmte(true);
    auto v1 = build_pmte(false);
    const auto v2 = build_pmte(true);
    const auto v3 = build_pmte(false);
    auto v4 = build_pmte(5);
    const auto v5 = build_pmte(5.6f);

    (*handler)(v0);
    (*handler)(v1);
    (*handler)(v2);
    (*handler)(v3);
    (*handler)(v4);
    (*handler)(v5);

    EXPECT_EQ(counter_true, 2);
    EXPECT_EQ(counter_false, 2);
  }

  // make_shared_pmte_handler
  {
    int counter_true = 0;
    int counter_false = 0;

    auto handler = make_shared_pmte_handler<bool>([&](bool v) {
      if (v) ++counter_true;
      else
        ++counter_false;
    });
    ASSERT_NE(handler, nullptr);
    EXPECT_EQ(handler->size(), platform_is64bit ? 56 : 28);
    EXPECT_TRUE(is_type_of<bool>(handler));

    auto v0 = build_pmte(true);
    auto v1 = build_pmte(false);
    const auto v2 = build_pmte(true);
    const auto v3 = build_pmte(false);
    auto v4 = build_pmte(5);
    const auto v5 = build_pmte(5.6f);

    (*handler)(v0);
    (*handler)(v1);
    (*handler)(v2);
    (*handler)(v3);
    (*handler)(v4);
    (*handler)(v5);

    EXPECT_EQ(counter_true, 2);
    EXPECT_EQ(counter_false, 2);
  }
}

TEST(PMTE, PackagedTask) {
  using namespace util::pmte;

  // build_pmte_handler
  {
    int counter = 0;

    auto handler = build_pmte_handler<std::packaged_task<int()>>([&](std::packaged_task<int()> &) { ++counter; });
    EXPECT_EQ(handler.size(), platform_is64bit ? 48 : 24);
    EXPECT_TRUE(is_type_of<std::packaged_task<int()>>(handler));

    auto v0 = build_pmte(std::packaged_task<int()>([]() noexcept -> int { return 0; }));
    auto v1 = build_pmte(std::packaged_task<float()>([]() noexcept -> float { return 0.0f; }));
    const auto v2 = build_pmte(true);
    auto v3 = build_pmte(std::packaged_task<int()>([]() noexcept -> int { return 0; }));
    auto v4 = build_pmte(5);
    const auto v5 = build_pmte(5.6f);

    handler(v0);
    handler(v1);
    handler(v2);
    handler(v3);
    handler(v4);
    handler(v5);

    EXPECT_EQ(counter, 2);
  }

  // make_pmte_handler
  {
    int counter = 0;

    auto handler = make_pmte_handler<std::packaged_task<int()>>([&](std::packaged_task<int()> &) { ++counter; });
    ASSERT_NE(handler, nullptr);
    EXPECT_EQ(handler->size(), platform_is64bit ? 48 : 24);
    EXPECT_TRUE(is_type_of<std::packaged_task<int()>>(handler));

    auto v0 = build_pmte(std::packaged_task<int()>([]() noexcept -> int { return 0; }));
    auto v1 = build_pmte(std::packaged_task<float()>([]() noexcept -> float { return 0.0f; }));
    const auto v2 = build_pmte(true);
    auto v3 = build_pmte(std::packaged_task<int()>([]() noexcept -> int { return 0; }));
    auto v4 = build_pmte(5);
    const auto v5 = build_pmte(5.6f);

    (*handler)(v0);
    (*handler)(v1);
    (*handler)(v2);
    (*handler)(v3);
    (*handler)(v4);
    (*handler)(v5);

    EXPECT_EQ(counter, 2);

    delete handler;
  }
}

TEST(PMTE, AbstractInterface) {
  using namespace util;
  using namespace util::pmte;

  int foobar_called_n_times = 0;

  struct interface {
    virtual ~interface() noexcept = default;
    virtual void foobar() = 0;
  };

  struct concrete : interface {
    int & counter;
    concrete(int & counter_) : counter(counter_) {}
    void foobar() override { ++counter; }
  };

  auto handler = make_pmte_handler<interface>([](interface & o) { o.foobar(); });

  auto o0 = build_pmte<interface>(concrete(foobar_called_n_times));
  auto o1 = build_pmte<interface>(concrete(foobar_called_n_times), type_list<interface, concrete> {});
  (*handler)(o0);
  (*handler)(o1);

  EXPECT_EQ(foobar_called_n_times, 2);

  delete handler;
}
