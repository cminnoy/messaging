/**
 * @file test_pointer_cast.cpp
 * @author Chris Minnoy
 * @version 1.0
 * @date 2023
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

#include <sstream>

#include <util/attributes.hpp>

#if __cplusplus >= CPP17_STANDARD && __has_include(<memory_resource>)
#include <memory_resource>
#endif

#include <gtest/gtest.h>

#include <util/pointer_cast.hpp>

using namespace util;
using namespace util::pmte;

struct class1 {
  virtual ~class1() noexcept = default;
  explicit class1() : a(0) {}
  explicit class1(int a_) : a(a_) {}
  int a;
};

bool operator==(const class1 & lhs, const class1 & rhs) {
  return lhs.a == rhs.a;
}

struct class2 : class1 {
  explicit class2() : class1(), f(0.0f) {}
  explicit class2(int a_, float f_) : class1(a_), f(f_) {}
  float f;
};

bool operator==(const class2 & lhs, const class2 & rhs) {
  return lhs.a == rhs.a && lhs.f == rhs.f;
}

struct class3 : class2 {
  explicit class3() : class2(), d(0.0) {}
  explicit class3(int a_, float f_, double d_) : class2(a_, f_), d(d_) {}
  double d;
};

bool operator==(const class3 & lhs, const class3 & rhs) {
  return lhs.a == rhs.a && lhs.f == rhs.f && lhs.d == rhs.d;
}

struct class4 : class2 {};
struct class5 : class4 {};

//-----------------------------------------------------------------------------

TEST(PointerCast, RawStaticPointerCast) {
  {
    int o = 5;
    void * p0 = &o;
    auto r0 = util::static_pointer_cast<int>(p0);
    ASSERT_TRUE((std::is_same<decltype(r0), int *>::value));
    ASSERT_EQ(r0, &o);
    ASSERT_NE(r0, nullptr);
    ASSERT_EQ(*r0, o);
  }

  {
    double o = 5.9;
    void * p0 = &o;
    auto r0 = util::static_pointer_cast<double>(p0);
    ASSERT_TRUE((std::is_same<decltype(r0), double *>::value));
    ASSERT_EQ(r0, &o);
    ASSERT_NE(r0, nullptr);
    ASSERT_EQ(*r0, o);
  }

  {
    auto r0 = util::static_pointer_cast<int>(nullptr);
    ASSERT_TRUE((std::is_same<decltype(r0), int *>::value));
    ASSERT_EQ(r0, nullptr);
  }
}

TEST(PointerCast, RawConstPointerCast) {
  {
    int o = 5;
    int * p0 = &o;
    auto r0 = util::const_pointer_cast<int>(p0);
    ASSERT_TRUE((std::is_same<decltype(r0), int *>::value));
    ASSERT_EQ(r0, &o);
    ASSERT_NE(r0, nullptr);
    ASSERT_EQ(*r0, o);
  }

  {
    int o = 5;
    int * p0 = &o;
    auto r0 = util::const_pointer_cast<const int>(p0);
    ASSERT_TRUE((std::is_same<decltype(r0), const int *>::value));
    ASSERT_EQ(r0, &o);
    ASSERT_NE(r0, nullptr);
    ASSERT_EQ(*r0, o);
  }

  {
    int o = 5;
    int const * p0 = &o;
    auto r0 = util::const_pointer_cast<int>(p0);
    ASSERT_TRUE((std::is_same<decltype(r0), int *>::value));
    ASSERT_EQ(r0, &o);
    ASSERT_NE(r0, nullptr);
    ASSERT_EQ(*r0, o);
  }

  {
    const int o = 5;
    int const * p0 = &o;
    auto r0 = util::const_pointer_cast<int>(p0);
    ASSERT_TRUE((std::is_same<decltype(r0), int *>::value));
    ASSERT_EQ(r0, &o);
    ASSERT_NE(r0, nullptr);
    ASSERT_EQ(*r0, o);
  }

  {
    auto r0 = util::const_pointer_cast<int>(nullptr);
    ASSERT_TRUE((std::is_same<decltype(r0), int *>::value));
    ASSERT_EQ(r0, nullptr);
  }

  {
    auto r0 = util::const_pointer_cast<const int>(nullptr);
    ASSERT_TRUE((std::is_same<decltype(r0), const int *>::value));
    ASSERT_EQ(r0, nullptr);
  }
}

TEST(PointerCast, RawReinterpretPointerCast) {
  {
    int o = 5;
    int * p0 = &o;
    auto r0 = util::reinterpret_pointer_cast<float>(p0);
    ASSERT_TRUE((std::is_same<decltype(r0), float *>::value));
    auto r1 = util::reinterpret_pointer_cast<int>(r0);
    ASSERT_TRUE((std::is_same<decltype(r1), int *>::value));
    ASSERT_EQ(r1, &o);
    ASSERT_NE(r1, nullptr);
    ASSERT_EQ(*r1, o);
  }

  {
    auto r0 = util::reinterpret_pointer_cast<const int>(nullptr);
    ASSERT_TRUE((std::is_same<decltype(r0), const int *>::value));
    ASSERT_EQ(r0, nullptr);
  }
}

TEST(PointerCast, RawDynamicPointerCast) {
  {
    class2 o {5, 6.8f};
    class1 * p0 = &o;

    auto r0 = util::dynamic_pointer_cast<class2>(p0);
    ASSERT_TRUE((std::is_same<decltype(r0), class2 *>::value));
    ASSERT_EQ(r0, &o);
    ASSERT_NE(r0, nullptr);
    ASSERT_EQ(*r0, o);

    auto r1 = util::dynamic_pointer_cast<class3>(p0);
    ASSERT_TRUE((std::is_same<decltype(r1), class3 *>::value));
    ASSERT_EQ(r1, nullptr);

    auto r2 = util::dynamic_pointer_cast<class4>(p0);
    ASSERT_TRUE((std::is_same<decltype(r2), class4 *>::value));
    ASSERT_EQ(r2, nullptr);
  }

  {
    class4 o;
    class4 * p0 = &o;

    auto r0 = util::dynamic_pointer_cast<class4>(p0);
    ASSERT_TRUE((std::is_same<decltype(r0), class4 *>::value));
    ASSERT_EQ(r0, &o);
  }

  {
    auto r0 = util::dynamic_pointer_cast<class2>(nullptr);
    ASSERT_TRUE((std::is_same<decltype(r0), class2 *>::value));
    ASSERT_EQ(r0, nullptr);
  }
}

//-----------------------------------------------------------------------------

TEST(PointerCast, UniqueStaticPointerCast) {
  class2 * p = new class2;
  std::unique_ptr<class2> o(p);
  ASSERT_EQ(o.get(), p);

  // Casting to same type; short cutting
  auto r0 = util::static_pointer_cast<class2>(std::move(o));
  ASSERT_EQ(r0.get(), p);
  ASSERT_EQ(o.get(), nullptr);
  ASSERT_TRUE((std::is_same<class2, decltype(r0)::element_type>::value));

  // Upcast
  auto r1 = util::static_pointer_cast<class1>(std::move(r0));
  ASSERT_EQ(r1.get(), p);
  ASSERT_EQ(r0.get(), nullptr);
  ASSERT_TRUE((std::is_same<class1, decltype(r1)::element_type>::value));

  // Downcast
  auto r2 = util::static_pointer_cast<class2>(std::move(r1));
  ASSERT_EQ(r2.get(), p);
  ASSERT_EQ(r1.get(), nullptr);
  ASSERT_TRUE((std::is_same<class2, decltype(r2)::element_type>::value));

  // Illegal cast, undefined behaviour, but allowed with static_cast
  auto r3 = util::static_pointer_cast<class3>(std::move(r2));
  ASSERT_EQ(r3.get(), p);
  ASSERT_EQ(r2.get(), nullptr);
  ASSERT_TRUE((std::is_same<class3, decltype(r3)::element_type>::value));

  r3.release();
  delete p;
}

TEST(PointerCast, UniqueConstPointerCast) {
  class2 * p = new class2;
  std::unique_ptr<class2> o(p);
  ASSERT_EQ(o.get(), p);

  // Casting to same type; short cutting
  auto r0 = util::const_pointer_cast<class2>(std::move(o));
  ASSERT_EQ(r0.get(), p);
  ASSERT_EQ(o.get(), nullptr);
  ASSERT_TRUE((std::is_same<class2, decltype(r0)::element_type>::value));

  // To const
  auto r1 = util::const_pointer_cast<const class2>(std::move(r0));
  ASSERT_EQ(r1.get(), p);
  ASSERT_EQ(r0.get(), nullptr);
  ASSERT_TRUE((std::is_same<const class2, decltype(r1)::element_type>::value));

  // From const
  auto r2 = util::const_pointer_cast<class2>(std::move(r1));
  ASSERT_EQ(r2.get(), p);
  ASSERT_EQ(r1.get(), nullptr);
  ASSERT_TRUE((std::is_same<class2, decltype(r2)::element_type>::value));
}

TEST(PointerCast, UniqueReinterpretPointerCast) {
  int * p = new int(0xAA55AA55);
  std::unique_ptr<int> o(p);
  ASSERT_EQ(o.get(), p);

  auto r0 = util::reinterpret_pointer_cast<int>(std::move(o));
  ASSERT_EQ(r0.get(), p);
  ASSERT_EQ(o.get(), nullptr);
  ASSERT_TRUE((std::is_same<int, decltype(r0)::element_type>::value));

  auto r1 = util::reinterpret_pointer_cast<float>(std::move(r0));
  ASSERT_EQ(r1.get(), reinterpret_cast<float *>(p));
  ASSERT_EQ(r0.get(), nullptr);
  ASSERT_TRUE((std::is_same<float, decltype(r1)::element_type>::value));

  auto r2 = util::reinterpret_pointer_cast<const int>(std::move(r1));
  ASSERT_EQ(r2.get(), reinterpret_cast<const int *>(p));
  ASSERT_EQ(r1.get(), nullptr);
  ASSERT_TRUE((std::is_same<const int, decltype(r2)::element_type>::value));
}

TEST(PointerCast, UniqueDynamicPointerCast) {
  class2 * p = new class2;
  std::unique_ptr<class2> o(p);
  ASSERT_EQ(o.get(), p);

  // Short cut
  auto r0 = util::dynamic_pointer_cast<class2>(std::move(o));
  ASSERT_EQ(r0.get(), p);
  ASSERT_EQ(o.get(), nullptr);
  ASSERT_TRUE((std::is_same<class2, decltype(r0)::element_type>::value));

  // Upcast
  auto r1 = util::dynamic_pointer_cast<class1>(std::move(r0));
  ASSERT_EQ(r1.get(), p);
  ASSERT_EQ(r0.get(), nullptr);
  ASSERT_TRUE((std::is_same<class1, decltype(r1)::element_type>::value));

  // Downcast
  auto r2 = util::dynamic_pointer_cast<class2>(std::move(r1));
  ASSERT_EQ(r2.get(), p);
  ASSERT_EQ(r1.get(), nullptr);
  ASSERT_TRUE((std::is_same<class2, decltype(r2)::element_type>::value));

  // Failing downcast
  auto r3 = util::dynamic_pointer_cast<class3>(std::move(r2));
  ASSERT_EQ(r3.get(), nullptr);
  ASSERT_EQ(r2.get(), p);
  ASSERT_TRUE((std::is_same<class3, decltype(r3)::element_type>::value));
}

//-----------------------------------------------------------------------------

TEST(PointerCast, SharedStaticPointerCast) {
  class2 * p = new class2;
  std::shared_ptr<class2> o(p);
  ASSERT_EQ(o.get(), p);

  auto r0 = util::static_pointer_cast<class2>(std::move(o));
  ASSERT_EQ(r0.get(), p);
  ASSERT_EQ(o.get(), p);
  ASSERT_TRUE((std::is_same<class2, decltype(r0)::element_type>::value));

  auto r1 = util::static_pointer_cast<class1>(r0);
  ASSERT_EQ(r1.get(), p);
  ASSERT_EQ(r0.get(), p);
  ASSERT_EQ(o.get(), p);
  ASSERT_TRUE((std::is_same<class1, decltype(r1)::element_type>::value));

  auto r2 = util::static_pointer_cast<class3>(r1);
  ASSERT_EQ(r2.get(), p);
  ASSERT_EQ(r1.get(), p);
  ASSERT_EQ(r0.get(), p);
  ASSERT_EQ(o.get(), p);
  ASSERT_TRUE((std::is_same<class3, decltype(r2)::element_type>::value));

  r2.reset();
}

TEST(PointerCast, SharedConstPointerCast) {
  class2 * p = new class2;
  std::shared_ptr<class2> o(p);
  ASSERT_EQ(o.get(), p);

  // Casting to same type
  auto r0 = util::const_pointer_cast<class2>(std::move(o));
  ASSERT_EQ(r0.get(), p);
  ASSERT_EQ(o.get(), p);
  ASSERT_TRUE((std::is_same<class2, decltype(r0)::element_type>::value));

  // To const
  auto r1 = util::const_pointer_cast<const class2>(r0);
  ASSERT_EQ(r1.get(), p);
  ASSERT_EQ(r0.get(), p);
  ASSERT_TRUE((std::is_same<const class2, decltype(r1)::element_type>::value));

  // From const
  auto r2 = util::const_pointer_cast<class2>(r1);
  ASSERT_EQ(r2.get(), p);
  ASSERT_EQ(r1.get(), p);
  ASSERT_TRUE((std::is_same<class2, decltype(r2)::element_type>::value));
}

TEST(PointerCast, SharedReinterpretPointerCast) {
  int * p = new int(0xAA55AA55);
  std::shared_ptr<int> o(p);
  ASSERT_EQ(o.get(), p);

  auto r0 = util::reinterpret_pointer_cast<int>(std::move(o));
  ASSERT_EQ(r0.get(), p);
  ASSERT_EQ(o.get(), p);
  ASSERT_TRUE((std::is_same<int, decltype(r0)::element_type>::value));

  auto r1 = util::reinterpret_pointer_cast<float>(r0);
  ASSERT_EQ(r1.get(), reinterpret_cast<float *>(p));
  ASSERT_EQ(r0.get(), p);
  ASSERT_TRUE((std::is_same<float, decltype(r1)::element_type>::value));

  auto r2 = util::reinterpret_pointer_cast<const int>(r1);
  ASSERT_EQ(r2.get(), reinterpret_cast<const int *>(p));
  ASSERT_EQ(r1.get(), reinterpret_cast<float *>(p));
  ASSERT_TRUE((std::is_same<const int, decltype(r2)::element_type>::value));
}

TEST(PointerCast, SharedDynamicPointerCast) {
  class2 * p = new class2;
  std::shared_ptr<class2> o(p);
  ASSERT_EQ(o.get(), p);

  // Short cut
  auto r0 = util::dynamic_pointer_cast<class2>(std::move(o));
  ASSERT_EQ(r0.get(), p);
  ASSERT_EQ(o.get(), p);
  ASSERT_TRUE((std::is_same<class2, decltype(r0)::element_type>::value));

  // Upcast
  auto r1 = util::dynamic_pointer_cast<class1>(r0);
  ASSERT_EQ(r1.get(), p);
  ASSERT_EQ(r0.get(), p);
  ASSERT_TRUE((std::is_same<class1, decltype(r1)::element_type>::value));

  // Downcast
  auto r2 = util::dynamic_pointer_cast<class2>(r1);
  ASSERT_EQ(r2.get(), p);
  ASSERT_EQ(r1.get(), p);
  ASSERT_TRUE((std::is_same<class2, decltype(r2)::element_type>::value));

  // Failing downcast
  auto r3 = util::dynamic_pointer_cast<class3>(r2);
  ASSERT_EQ(r3.get(), nullptr);
  ASSERT_EQ(r2.get(), p);
  ASSERT_TRUE((std::is_same<class3, decltype(r3)::element_type>::value));
}

//-----------------------------------------------------------------------------

TEST(PointerCast, PMTEStaticPointerCast) {
#if 1
  // Basic type
  {
    auto p0 = make_shared_pmte(5);
    ASSERT_NE(p0.get(), nullptr);
    ASSERT_TRUE(is_type_of<int>(p0));
    ASSERT_EQ(*p0, 5);

    auto r0 = util::static_pointer_cast<int>(p0);
    ASSERT_NE(r0.get(), nullptr);
    ASSERT_EQ(*r0, 5);
    ASSERT_EQ(*p0, 5);

    auto r1 = util::static_pointer_cast<void>(p0);
    auto r2 = util::static_pointer_cast<float>(r1);
    auto r3 = util::static_pointer_cast<void>(r2);
    auto r4 = util::static_pointer_cast<int>(r3);
    ASSERT_NE(r4.get(), nullptr);
    ASSERT_EQ(*r4, 5);
    ASSERT_EQ(*r0, 5);
    ASSERT_EQ(*p0, 5);

    *r4 = 7;
    ASSERT_EQ(*r4, 7);
    ASSERT_EQ(*r0, 7);
    ASSERT_EQ(*p0, 7);
  }
#endif

#if 1
  // Pointer type
  {
    class2 c {5, 7.7f};
    auto p0 = make_shared_pmte(&c);
    ASSERT_NE(p0.get(), nullptr);
    ASSERT_EQ(*p0, &c);
    ASSERT_TRUE((std::is_same<class2 *, util::remove_cvref_t<decltype(p0->get())>>::value));

    auto r0 = util::static_pointer_cast<class2 *>(p0);
    ASSERT_NE(r0.get(), nullptr);
    ASSERT_EQ((*r0)->a, 5);
    ASSERT_EQ((*r0)->f, 7.7f);
    ASSERT_TRUE((std::is_same<class2 *, decltype(r0)::element_type>::value));

    auto r1 = util::static_pointer_cast<class1 *>(p0);
    ASSERT_NE(r1.get(), nullptr);
    ASSERT_EQ((*r1)->a, 5);
    ASSERT_TRUE((std::is_same<class1 *, decltype(r1)::element_type>::value));

    auto r2 = util::static_pointer_cast<class3 *>(p0);
    ASSERT_NE(r2.get(), nullptr);
    ASSERT_TRUE((std::is_same<class3 *, decltype(r2)::element_type>::value));
  }
#endif

#if __GNUC__ >= 8
  // Pointer type (deleter)
  {
    bool deleter_called = false;

    auto p0 = make_shared_pmte(new class2 {5, 7.7f}, [&](class2 * p) {
      deleter_called = true;
      delete p;
    });
    ASSERT_NE(p0.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2 *, util::remove_cvref_t<decltype(p0->get())>>::value));

    auto r0 = util::static_pointer_cast<class2 *>(p0);
    ASSERT_NE(r0.get(), nullptr);
    ASSERT_EQ((*r0)->a, 5);
    ASSERT_EQ((*r0)->f, 7.7f);
    ASSERT_TRUE((std::is_same<class2 *, decltype(r0)::element_type>::value));

    auto r1 = util::static_pointer_cast<class1 *>(p0);
    ASSERT_NE(r1.get(), nullptr);
    ASSERT_EQ((*r1)->a, 5);
    ASSERT_TRUE((std::is_same<class1 *, decltype(r1)::element_type>::value));

    // Undefined behaviour
    auto r2 = util::static_pointer_cast<class3 *>(p0);
    ASSERT_NE(r2.get(), nullptr);
    ASSERT_TRUE((std::is_same<class3 *, decltype(r2)::element_type>::value));

    p0.reset();
    ASSERT_FALSE(deleter_called);
    r2.reset();
    ASSERT_FALSE(deleter_called);
    r1.reset();
    ASSERT_FALSE(deleter_called);
    r0.reset();
    ASSERT_TRUE(deleter_called);
  }
#endif

#if 1
  // Pointer type (RTTI)
  {
    class2 c {5, 7.7f};
    auto p0 = make_shared_pmte(&c, type_list<class1 *, class2 *> {});
    ASSERT_NE(p0.get(), nullptr);
    ASSERT_EQ(*p0, &c);
    ASSERT_TRUE((std::is_same<class2 *, util::remove_cvref_t<decltype(p0->get())>>::value));
    ASSERT_TRUE((std::is_same<class2 *, decltype(p0)::element_type::element_type>::value));
    ASSERT_TRUE(is_type_of<class2 *>(p0));
    ASSERT_TRUE(is_castable_to<class2 *>(p0));
    ASSERT_TRUE(is_castable_to<class1 *>(p0));

    auto r0 = util::static_pointer_cast<class2 *>(p0);
    ASSERT_NE(r0.get(), nullptr);
    ASSERT_EQ((*r0)->a, 5);
    ASSERT_EQ((*r0)->f, 7.7f);
    ASSERT_TRUE((std::is_same<class2 *, decltype(r0)::element_type>::value));

    auto r1 = util::static_pointer_cast<class1 *>(p0);
    ASSERT_NE(r1.get(), nullptr);
    ASSERT_EQ((*r1)->a, 5);
    ASSERT_TRUE((std::is_same<class1 *, decltype(r1)::element_type>::value));

    auto r2 = util::static_pointer_cast<class3 *>(p0);
    ASSERT_NE(r2.get(), nullptr);
    ASSERT_TRUE((std::is_same<class3 *, decltype(r2)::element_type>::value));
  }
#endif

#if 1
  // Pointer type (deleter + RTTI)
  {
    bool deleter_called = false;

    auto p0 = make_shared_pmte(
      new class2 {5, 7.7f},
      [&](class2 * p) {
        deleter_called = true;
        delete p;
      },
      type_list<class1 *, class2 *> {});
    ASSERT_NE(p0.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2 *, util::remove_cvref_t<decltype(p0->get())>>::value));

    auto r0 = util::static_pointer_cast<class2 *>(p0);
    ASSERT_NE(r0.get(), nullptr);
    ASSERT_EQ((*r0)->a, 5);
    ASSERT_EQ((*r0)->f, 7.7f);
    ASSERT_TRUE((std::is_same<class2 *, decltype(r0)::element_type>::value));

    auto r1 = util::static_pointer_cast<class1 *>(p0);
    ASSERT_NE(r1.get(), nullptr);
    ASSERT_EQ((*r1)->a, 5);
    ASSERT_TRUE((std::is_same<class1 *, decltype(r1)::element_type>::value));

    // Undefined behaviour
    auto r2 = util::static_pointer_cast<class3 *>(p0);
    ASSERT_NE(r2.get(), nullptr);
    ASSERT_TRUE((std::is_same<class3 *, decltype(r2)::element_type>::value));

    p0.reset();
    ASSERT_FALSE(deleter_called);
    r2.reset();
    ASSERT_FALSE(deleter_called);
    r1.reset();
    ASSERT_FALSE(deleter_called);
    r0.reset();
    ASSERT_TRUE(deleter_called);
  }
#endif

#if 1
  // Class type (full)
  {
    auto p0 = make_shared_pmte(class2 {5, 7.7f});
    ASSERT_NE(p0.get(), nullptr);
    ASSERT_TRUE(is_type_of<class2>(p0));
    ASSERT_TRUE((std::is_same<class2, util::remove_cvref_t<decltype(p0->get())>>::value));

    auto r0 = util::static_pointer_cast<class2>(p0);
    ASSERT_NE(r0.get(), nullptr);
    ASSERT_EQ(r0->a, 5);
    ASSERT_EQ(r0->f, 7.7f);
    ASSERT_TRUE((std::is_same<class2, decltype(r0)::element_type>::value));

    auto r1 = util::static_pointer_cast<class1>(p0);
    ASSERT_NE(r1.get(), nullptr);
    ASSERT_EQ(r1->a, 5);
    ASSERT_TRUE((std::is_same<class1, decltype(r1)::element_type>::value));

    // Undefined behaviour
    auto r2 = util::static_pointer_cast<class3>(p0);
    ASSERT_NE(r2.get(), nullptr);
    ASSERT_TRUE((std::is_same<class3, decltype(r2)::element_type>::value));
  }
#endif

#if 1
  // Class type RTTI (full)
  {
    auto p0 = make_shared_pmte(class2 {5, 7.7f}, type_list<class2, class1> {});
    ASSERT_NE(p0.get(), nullptr);
    ASSERT_TRUE(is_type_of<class2>(p0));
    ASSERT_TRUE((std::is_same<class2, util::remove_cvref_t<decltype(p0->get())>>::value));

    auto r0 = util::static_pointer_cast<class2>(p0);
    ASSERT_NE(r0.get(), nullptr);
    ASSERT_EQ(r0->a, 5);
    ASSERT_EQ(r0->f, 7.7f);
    ASSERT_TRUE((std::is_same<class2, decltype(r0)::element_type>::value));

    auto r1 = util::static_pointer_cast<class1>(p0);
    ASSERT_NE(r1.get(), nullptr);
    ASSERT_EQ(r1->a, 5);
    ASSERT_TRUE((std::is_same<class1, decltype(r1)::element_type>::value));

    // Undefined behaviour
    auto r2 = util::static_pointer_cast<class3>(p0);
    ASSERT_NE(r2.get(), nullptr);
    ASSERT_TRUE((std::is_same<class3, decltype(r2)::element_type>::value));
  }
#endif

#if 1
  // Class type (interface)
  {
    shared_pmte_value p0 = make_shared_pmte(class2 {5, 7.7f});
    ASSERT_NE(p0.get(), nullptr);
    ASSERT_TRUE(is_type_of<class2>(p0));

    auto r0 = util::static_pointer_cast<class2>(p0);
    ASSERT_NE(r0.get(), nullptr);
    ASSERT_EQ(r0->a, 5);
    ASSERT_EQ(r0->f, 7.7f);
    ASSERT_TRUE((std::is_same<class2, decltype(r0)::element_type>::value));

    auto r1 = util::static_pointer_cast<class1>(p0);
    ASSERT_NE(r1.get(), nullptr);
    ASSERT_EQ(r1->a, 5);
    ASSERT_TRUE((std::is_same<class1, decltype(r1)::element_type>::value));

    // // Undefined behaviour
    auto r2 = util::static_pointer_cast<class3>(p0);
    ASSERT_NE(r2.get(), nullptr);
    ASSERT_TRUE((std::is_same<class3, decltype(r2)::element_type>::value));
  }
#endif

#if 1
  // Class type RTTI (interface)
  {
    shared_pmte_value p0 = make_shared_pmte(class2 {5, 7.7f}, type_list<class2, class1> {});
    ASSERT_NE(p0.get(), nullptr);
    ASSERT_TRUE(is_type_of<class2>(p0));

    auto r0 = util::static_pointer_cast<class2>(p0);
    ASSERT_NE(r0.get(), nullptr);
    ASSERT_EQ(r0->a, 5);
    ASSERT_EQ(r0->f, 7.7f);
    ASSERT_TRUE((std::is_same<class2, decltype(r0)::element_type>::value));

    auto r1 = util::static_pointer_cast<class1>(p0);
    ASSERT_NE(r1.get(), nullptr);
    ASSERT_EQ(r1->a, 5);
    ASSERT_TRUE((std::is_same<class1, decltype(r1)::element_type>::value));

    // Undefined behaviour
    auto r2 = util::static_pointer_cast<class3>(p0);
    ASSERT_NE(r2.get(), nullptr);
    ASSERT_TRUE((std::is_same<class3, decltype(r2)::element_type>::value));
  }
#endif
}

TEST(PointerCast, PMTEConstPointerCast) {}

TEST(PointerCast, PMTEReinterpretPointerCast) {}

TEST(PointerCast, PMTEDynamicPointerCast) {
  // Pointer type

  // Pointer type (deleter)

  // Pointer type (RTTI)

  // Pointer type (deleter + RTTI)

  // Class type (full)
  {
    auto p0 = make_shared_pmte(class3 {5, 7.7f, 9.9});
    ASSERT_NE(p0.get(), nullptr);
    EXPECT_TRUE(is_type_of<class3>(p0));

    auto p1 = make_shared_pmte<class4>();
    ASSERT_NE(p1.get(), nullptr);
    EXPECT_TRUE(is_type_of<class4>(p1));

    auto r0 = util::dynamic_pointer_cast<class2, class3>(p0);
    ASSERT_NE(r0.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r0)::element_type>::value));
    EXPECT_EQ(r0->a, 5);
    EXPECT_EQ(r0->f, 7.7f);

    auto r1 = util::dynamic_pointer_cast<class2, class3, class4>(p0);
    ASSERT_NE(r1.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r1)::element_type>::value));
    EXPECT_EQ(r1->a, 5);
    EXPECT_EQ(r1->f, 7.7f);

    auto r2 = util::dynamic_pointer_cast<class2, class4>(p1);
    ASSERT_NE(r2.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r2)::element_type>::value));
    EXPECT_EQ(r2->a, 0);
    EXPECT_EQ(r2->f, 0.0f);

    auto r3 = util::dynamic_pointer_cast<class2, class3, class4>(p1);
    ASSERT_NE(r3.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r3)::element_type>::value));
    ASSERT_EQ(r3->a, 0);
    ASSERT_EQ(r3->f, 0.0f);

    auto r4 = util::dynamic_pointer_cast<class4>(p0);
    ASSERT_EQ(r4.get(), nullptr);
    ASSERT_TRUE((std::is_same<class4, decltype(r4)::element_type>::value));
  }

  // Class type RTTI (full)
  {
    auto p0 = make_shared_pmte(class3 {5, 7.7f, 9.9}, type_list<class3, class2, class1> {});
    ASSERT_NE(p0.get(), nullptr);
    EXPECT_TRUE(is_type_of<class3>(p0));

    auto p1 = make_shared_pmte(class4 {}, type_list<class4, class2, class1> {});
    ASSERT_NE(p1.get(), nullptr);
    EXPECT_TRUE(is_type_of<class4>(p1));

    auto r0 = util::dynamic_pointer_cast<class2, class3>(p0);
    ASSERT_NE(r0.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r0)::element_type>::value));
    EXPECT_EQ(r0->a, 5);
    EXPECT_EQ(r0->f, 7.7f);

    auto r1 = util::dynamic_pointer_cast<class2>(p0);
    ASSERT_NE(r1.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r1)::element_type>::value));
    EXPECT_EQ(r1->a, 5);
    EXPECT_EQ(r1->f, 7.7f);

    auto r2 = util::dynamic_pointer_cast<class2, class4>(p1);
    ASSERT_NE(r2.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r2)::element_type>::value));
    EXPECT_EQ(r2->a, 0);
    EXPECT_EQ(r2->f, 0.0f);

    auto r3 = util::dynamic_pointer_cast<class2>(p1);
    ASSERT_NE(r3.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r3)::element_type>::value));
    ASSERT_EQ(r3->a, 0);
    ASSERT_EQ(r3->f, 0.0f);

    auto r4 = util::dynamic_pointer_cast<class4>(p0);
    ASSERT_EQ(r4.get(), nullptr);
    ASSERT_TRUE((std::is_same<class4, decltype(r4)::element_type>::value));
  }

  // Class type (interface)
  {
    shared_pmte_value p0 = make_shared_pmte(class3 {5, 7.7f, 9.9});
    ASSERT_NE(p0.get(), nullptr);
    EXPECT_TRUE(is_type_of<class3>(p0));

    shared_pmte_value p1 = make_shared_pmte<class4>();
    ASSERT_NE(p1.get(), nullptr);
    EXPECT_TRUE(is_type_of<class4>(p1));

    auto r0 = util::dynamic_pointer_cast<class2, class3>(p0);
    ASSERT_NE(r0.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r0)::element_type>::value));
    EXPECT_EQ(r0->a, 5);
    EXPECT_EQ(r0->f, 7.7f);

    auto r1 = util::dynamic_pointer_cast<class2, class3, class4>(p0);
    ASSERT_NE(r1.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r1)::element_type>::value));
    EXPECT_EQ(r1->a, 5);
    EXPECT_EQ(r1->f, 7.7f);

    auto r2 = util::dynamic_pointer_cast<class2, class4>(p1);
    ASSERT_NE(r2.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r2)::element_type>::value));
    EXPECT_EQ(r2->a, 0);
    EXPECT_EQ(r2->f, 0.0f);

    auto r3 = util::dynamic_pointer_cast<class2, class3, class4>(p1);
    ASSERT_NE(r3.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r3)::element_type>::value));
    ASSERT_EQ(r3->a, 0);
    ASSERT_EQ(r3->f, 0.0f);

    auto r4 = util::dynamic_pointer_cast<class4>(p0);
    ASSERT_EQ(r4.get(), nullptr);
    ASSERT_TRUE((std::is_same<class4, decltype(r4)::element_type>::value));
  }

  // Class type (interface; type list)
  {
    shared_pmte_value p0 = make_shared_pmte(class3 {5, 7.7f, 9.9});
    ASSERT_NE(p0.get(), nullptr);
    EXPECT_TRUE(is_type_of<class3>(p0));

    shared_pmte_value p1 = make_shared_pmte<class4>();
    ASSERT_NE(p1.get(), nullptr);
    EXPECT_TRUE(is_type_of<class4>(p1));

    auto r0 = util::dynamic_pointer_cast<class3>(p0, type_list<class3, class2, class1> {});
    ASSERT_NE(r0.get(), nullptr);
    ASSERT_TRUE((std::is_same<class3, decltype(r0)::element_type>::value));
    EXPECT_EQ(r0->a, 5);
    EXPECT_EQ(r0->f, 7.7f);
    EXPECT_EQ(r0->d, 9.9);

    auto r1 = util::dynamic_pointer_cast<class2>(p0, type_list<class2, class1> {});
    ASSERT_EQ(r1.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r1)::element_type>::value));

    auto r2 = util::dynamic_pointer_cast<class2>(p1, type_list<class4, class2, class1> {});
    ASSERT_NE(r2.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r2)::element_type>::value));
    EXPECT_EQ(r2->a, 0);
    EXPECT_EQ(r2->f, 0.0f);

    auto r3 = util::dynamic_pointer_cast<class5>(p1, type_list<class5, class4, class2, class1> {});
    ASSERT_EQ(r3.get(), nullptr);
    ASSERT_TRUE((std::is_same<class5, decltype(r3)::element_type>::value));

    auto r4 = util::dynamic_pointer_cast<class4>(p0);
    ASSERT_EQ(r4.get(), nullptr);
    ASSERT_TRUE((std::is_same<class4, decltype(r4)::element_type>::value));
  }

  // Class Type RTTI (interface)
  {
    shared_pmte_value p0 = make_shared_pmte(class3 {5, 7.7f, 9.9}, type_list<class3, class2, class1> {});
    ASSERT_NE(p0.get(), nullptr);
    EXPECT_TRUE(is_type_of<class3>(p0));

    shared_pmte_value p1 = make_shared_pmte(class4 {}, type_list<class4, class2, class1> {});
    ASSERT_NE(p1.get(), nullptr);
    EXPECT_TRUE(is_type_of<class4>(p1));

    auto r0 = util::dynamic_pointer_cast<class2, class3>(p0);
    ASSERT_NE(r0.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r0)::element_type>::value));
    EXPECT_EQ(r0->a, 5);
    EXPECT_EQ(r0->f, 7.7f);

    auto r1 = util::dynamic_pointer_cast<class2>(p0);
    ASSERT_NE(r1.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r1)::element_type>::value));
    EXPECT_EQ(r1->a, 5);
    EXPECT_EQ(r1->f, 7.7f);

    auto r2 = util::dynamic_pointer_cast<class2, class4>(p1);
    ASSERT_NE(r2.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r2)::element_type>::value));
    EXPECT_EQ(r2->a, 0);
    EXPECT_EQ(r2->f, 0.0f);

    auto r3 = util::dynamic_pointer_cast<class2>(p1);
    ASSERT_NE(r3.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r3)::element_type>::value));
    ASSERT_EQ(r3->a, 0);
    ASSERT_EQ(r3->f, 0.0f);

    auto r4 = util::dynamic_pointer_cast<class4>(p0);
    ASSERT_EQ(r4.get(), nullptr);
    ASSERT_TRUE((std::is_same<class4, decltype(r4)::element_type>::value));
  }

  // Class Type RTTI (interface; type list)
  {
    shared_pmte_value p0 = make_shared_pmte(class3 {5, 7.7f, 9.9}, type_list<class3, class2, class1> {});
    ASSERT_NE(p0.get(), nullptr);
    EXPECT_TRUE(is_type_of<class3>(p0));

    shared_pmte_value p1 = make_shared_pmte(class5 {}, type_list<class5, class4, class2, class1> {});
    ASSERT_NE(p1.get(), nullptr);
    EXPECT_TRUE(is_type_of<class5>(p1));

    auto r0 = util::dynamic_pointer_cast<class2>(p0, type_list<class1> {});
    ASSERT_NE(r0.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r0)::element_type>::value));
    EXPECT_EQ(r0->a, 5);
    EXPECT_EQ(r0->f, 7.7f);

    auto r1 = util::dynamic_pointer_cast<class3>(p0, type_list<class3, class2, class1> {});
    ASSERT_NE(r1.get(), nullptr);
    ASSERT_TRUE((std::is_same<class3, decltype(r1)::element_type>::value));
    EXPECT_EQ(r1->a, 5);
    EXPECT_EQ(r1->f, 7.7f);

    auto r2 = util::dynamic_pointer_cast<class4>(p1, type_list<class1> {});
    ASSERT_NE(r2.get(), nullptr);
    ASSERT_TRUE((std::is_same<class4, decltype(r2)::element_type>::value));
    EXPECT_EQ(r2->a, 0);
    EXPECT_EQ(r2->f, 0.0f);

    auto r3 = util::dynamic_pointer_cast<class2>(p1, type_list<class2, class1> {});
    ASSERT_NE(r3.get(), nullptr);
    ASSERT_TRUE((std::is_same<class2, decltype(r3)::element_type>::value));
    ASSERT_EQ(r3->a, 0);
    ASSERT_EQ(r3->f, 0.0f);

    auto r4 = util::dynamic_pointer_cast<class4>(p0, type_list<class4, class2, class1> {});
    ASSERT_EQ(r4.get(), nullptr);
    ASSERT_TRUE((std::is_same<class4, decltype(r4)::element_type>::value));
  }
}
