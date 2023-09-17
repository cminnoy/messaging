#include <util/pmte.hpp>

#include <iostream>
#include <exception>

#include <gtest/gtest.h>

using namespace util;
using namespace util::pmte;

struct non_copy_construct_message {
  non_copy_construct_message() {}
  ~non_copy_construct_message() {}

  non_copy_construct_message(non_copy_construct_message &&) {}
  non_copy_construct_message & operator=(non_copy_construct_message &&) { return *this; }

  non_copy_construct_message(non_copy_construct_message const &) = delete;
  non_copy_construct_message & operator=(non_copy_construct_message const &) { return *this; }
};

struct nothrow_non_copy_construct_message {
  nothrow_non_copy_construct_message() noexcept(true) {}
  ~nothrow_non_copy_construct_message() noexcept(true) {}

  nothrow_non_copy_construct_message(nothrow_non_copy_construct_message &&) noexcept(true) {}
  nothrow_non_copy_construct_message & operator=(nothrow_non_copy_construct_message &&) noexcept(true) { return *this; }

  nothrow_non_copy_construct_message(nothrow_non_copy_construct_message const &) = delete;
  nothrow_non_copy_construct_message & operator=(nothrow_non_copy_construct_message const &) noexcept(true) { return *this; }
};

struct non_copy_assign_message {
  non_copy_assign_message() {}
  ~non_copy_assign_message() {}

  non_copy_assign_message(non_copy_assign_message &&) {}
  non_copy_assign_message & operator=(non_copy_assign_message &&) { return *this; }

  non_copy_assign_message(non_copy_assign_message const &) {}
  non_copy_assign_message & operator=(non_copy_assign_message const &) = delete;
};

struct nothrow_non_copy_assign_message {
  nothrow_non_copy_assign_message() noexcept(true) {}
  ~nothrow_non_copy_assign_message() noexcept(true) {}

  nothrow_non_copy_assign_message(nothrow_non_copy_assign_message &&) noexcept(true) {}
  nothrow_non_copy_assign_message & operator=(nothrow_non_copy_assign_message &&) noexcept(true) { return *this; }

  nothrow_non_copy_assign_message(nothrow_non_copy_assign_message const &) noexcept(true) {}
  nothrow_non_copy_assign_message & operator=(nothrow_non_copy_assign_message const &) = delete;
};

struct non_move_construct_message {
  non_move_construct_message() {}
  ~non_move_construct_message() {}

  non_move_construct_message(non_move_construct_message &&) = delete;
  non_move_construct_message & operator=(non_move_construct_message &&) { return *this; }

  non_move_construct_message(non_move_construct_message const &) {}
  non_move_construct_message & operator=(non_move_construct_message const &) { return *this; }
};

struct nothrow_non_move_construct_message {
  nothrow_non_move_construct_message() noexcept(true) {}
  ~nothrow_non_move_construct_message() noexcept(true) {}

  nothrow_non_move_construct_message(nothrow_non_move_construct_message &&) = delete;
  nothrow_non_move_construct_message & operator=(nothrow_non_move_construct_message &&) noexcept(true) { return *this; }

  nothrow_non_move_construct_message(nothrow_non_move_construct_message const &) noexcept(true) {}
  nothrow_non_move_construct_message & operator=(nothrow_non_move_construct_message const &) noexcept(true) { return *this; }
};

struct non_move_assign_message {
  non_move_assign_message() {}
  ~non_move_assign_message() {}

  non_move_assign_message(non_move_assign_message &&) {}
  non_move_assign_message & operator=(non_move_assign_message &&) = delete;

  non_move_assign_message(non_move_assign_message const &) {}
  non_move_assign_message & operator=(non_move_assign_message const &) { return *this; }
};

struct nothrow_non_move_assign_message {
  nothrow_non_move_assign_message() noexcept(true) {}
  ~nothrow_non_move_assign_message() noexcept(true) {}

  nothrow_non_move_assign_message(nothrow_non_move_assign_message &&) noexcept(true) {}
  nothrow_non_move_assign_message & operator=(nothrow_non_move_assign_message &&) = delete;

  nothrow_non_move_assign_message(nothrow_non_move_assign_message const &) noexcept(true) {}
  nothrow_non_move_assign_message & operator=(nothrow_non_move_assign_message const &) noexcept(true) { return *this; }
};

struct non_copy_message {
  non_copy_message() {}
  ~non_copy_message() {}

  non_copy_message(non_copy_message &&) {}
  non_copy_message & operator=(non_copy_message &&) { return *this; }

  non_copy_message(non_copy_message const &) = delete;
  non_copy_message & operator=(non_copy_message const &) = delete;
};

struct nothrow_non_copy_message {
  nothrow_non_copy_message() noexcept(true) {}
  ~nothrow_non_copy_message() noexcept(true) {}

  nothrow_non_copy_message(nothrow_non_copy_message &&) noexcept(true) {}
  nothrow_non_copy_message & operator=(nothrow_non_copy_message &&) noexcept(true) { return *this; }

  nothrow_non_copy_message(nothrow_non_copy_message const &) = delete;
  nothrow_non_copy_message & operator=(nothrow_non_copy_message const &) = delete;
};

struct non_move_message {
  non_move_message() {}
  ~non_move_message() {}

  non_move_message(non_move_message &&) = delete;
  non_move_message & operator=(non_move_message &&) = delete;

  non_move_message(non_move_message const &) {}
  non_move_message & operator=(non_move_message const &) { return *this; }
};

struct nothrow_non_move_message {
  nothrow_non_move_message() noexcept(true) {}
  ~nothrow_non_move_message() noexcept(true) {}

  nothrow_non_move_message(nothrow_non_move_message &&) = delete;
  nothrow_non_move_message & operator=(nothrow_non_move_message &&) = delete;

  nothrow_non_move_message(nothrow_non_move_message const &) noexcept(true) {}
  nothrow_non_move_message & operator=(nothrow_non_move_message const &) noexcept(true) { return *this; }
};

struct non_copy_non_move_message {
  non_copy_non_move_message() {}
  ~non_copy_non_move_message() {}

  non_copy_non_move_message(non_copy_non_move_message &&) = delete;
  non_copy_non_move_message & operator=(non_copy_non_move_message &&) = delete;

  non_copy_non_move_message(non_copy_non_move_message const &) = delete;
  non_copy_non_move_message & operator=(non_copy_non_move_message const &) = delete;
};

struct nothrow_non_copy_non_move_message {
  nothrow_non_copy_non_move_message() noexcept(true) {}
  ~nothrow_non_copy_non_move_message() noexcept(true) {}

  nothrow_non_copy_non_move_message(nothrow_non_copy_non_move_message &&) = delete;
  non_move_message & operator=(nothrow_non_copy_non_move_message &&) = delete;

  nothrow_non_copy_non_move_message(nothrow_non_copy_non_move_message const &) = delete;
  nothrow_non_copy_non_move_message & operator=(nothrow_non_copy_non_move_message const &) = delete;
};

struct non_default_constructor_message {
  non_default_constructor_message() = delete;
  virtual ~non_default_constructor_message() {}

  non_default_constructor_message(int) {}

  non_default_constructor_message(non_default_constructor_message &&) {}
  non_default_constructor_message & operator=(non_default_constructor_message &&) { return *this; }

  non_default_constructor_message(non_default_constructor_message const &) {}
  non_default_constructor_message & operator=(non_default_constructor_message const &) { return *this; }
};

struct nothrow_non_default_constructor_message {
  nothrow_non_default_constructor_message() = delete;
  virtual ~nothrow_non_default_constructor_message() noexcept(true) {}

  nothrow_non_default_constructor_message(int) noexcept(true) {}

  nothrow_non_default_constructor_message(nothrow_non_default_constructor_message &&) noexcept(true) {}
  nothrow_non_default_constructor_message & operator=(nothrow_non_default_constructor_message &&) noexcept(true) { return *this; }

  nothrow_non_default_constructor_message(nothrow_non_default_constructor_message const &) noexcept(true) {}
  nothrow_non_default_constructor_message & operator=(nothrow_non_default_constructor_message const &) noexcept(true) { return *this; }
};

struct throwing_destructor_message {
  throwing_destructor_message() noexcept {}
  ~throwing_destructor_message() noexcept(false) { throw std::runtime_error("msg"); }

  throwing_destructor_message(throwing_destructor_message &&) noexcept {}
  throwing_destructor_message & operator=(throwing_destructor_message &&) noexcept(false) { return *this; }

  throwing_destructor_message(throwing_destructor_message const &) noexcept {}
  throwing_destructor_message & operator=(throwing_destructor_message const &) noexcept(false) { return *this; }
};

template <typename O>
struct BaseClass {
  O o;
};

template <typename O>
struct DerivedClass : BaseClass<O> {};

template <typename O>
struct SuperDerivedClass : DerivedClass<O> {};

TEST(PMTE, TraitsBuildPmteDefault) {
  // float
  {
    using T = float;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const float
  {
    using T = const float;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // float*
  {
    using T = float *;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const float*
  {
    using T = const float *;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // std::unique_ptr<int>
  {
    using T = std::unique_ptr<int>;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

#if __cplusplus >= CPP17_STANDARD
  // const std::unique_ptr<int>
  {
    using T = const std::unique_ptr<int>;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }
#endif

  // non_copy_construct_message
  {
    using T = non_copy_construct_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

#if __cplusplus >= CPP17_STANDARD
  // const non_copy_construct_message
  {
    using T = const non_copy_construct_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }
#endif

  // nothrow_non_copy_construct_message
  {
    using T = nothrow_non_copy_construct_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

#if __cplusplus >= CPP17_STANDARD
  // const nothrow_non_copy_construct_message
  {
    using T = const nothrow_non_copy_construct_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }
#endif

  // non_copy_assign_message
  {
    using T = non_copy_assign_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const non_copy_assign_message
  {
    using T = const non_copy_assign_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // nothrow_non_copy_assign_message
  {
    using T = nothrow_non_copy_assign_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const nothrow_non_copy_assign_message
  {
    using T = const nothrow_non_copy_assign_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // non_move_construct_message
  {
    using T = non_move_construct_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const non_move_construct_message
  {
    using T = const non_move_construct_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // nothrow_non_move_construct_message
  {
    using T = nothrow_non_move_construct_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const nothrow_non_move_construct_message
  {
    using T = const nothrow_non_move_construct_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // non_move_assign_message
  {
    using T = non_move_assign_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // const non_move_assign_message
  {
    using T = const non_move_assign_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // nothrow_non_move_assign_message
  {
    using T = nothrow_non_move_assign_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // const nothrow_non_move_assign_message
  {
    using T = const nothrow_non_move_assign_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // non_copy_message
  {
    using T = non_copy_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

#if __cplusplus >= CPP17_STANDARD
  // const non_copy_message
  {
    using T = const non_copy_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }
#endif

  // nothrow_non_copy_message
  {
    using T = nothrow_non_copy_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const nothrow_non_copy_message
  {
    using T = nothrow_non_copy_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // non_move_message
  {
    using T = non_move_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // const non_move_message
  {
    using T = const non_move_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // nothrow_non_move_message
  {
    using T = nothrow_non_move_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // const nothrow_non_move_message
  {
    using T = const nothrow_non_move_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

#if __cplusplus >= CPP17_STANDARD
  // non_copy_non_move_message
  {
    using T = non_copy_non_move_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }
#endif

#if __cplusplus >= CPP17_STANDARD
  // const non_copy_non_move_message
  {
    using T = const non_copy_non_move_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }
#endif

#if __cplusplus >= CPP17_STANDARD
  // nothrow_non_copy_non_move_message
  {
    using T = nothrow_non_copy_non_move_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }
#endif

#if __cplusplus >= CPP17_STANDARD
  // const nothrow_non_copy_non_move_message
  {
    using T = const nothrow_non_copy_non_move_message;
    using object_type = decltype(build_pmte<T>());

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }
#endif
}

TEST(PMTE, TraitsBuildPmteValue) {
  // float
  {
    using T = float;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const float
  {
    using T = const float;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // float*
  {
    using T = float *;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const float*
  {
    using T = const float *;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // std::unique_ptr<int>
  {
    using T = std::unique_ptr<int>;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const std::unique_ptr<int>

  // non_copy_construct_message
  {
    using T = non_copy_construct_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const non_copy_construct_message

  // nothrow_non_copy_construct_message
  {
    using T = nothrow_non_copy_construct_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const nothrow_non_copy_construct_message
  {
    using T = nothrow_non_copy_construct_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // non_copy_assign_message
  {
    using T = non_copy_assign_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const non_copy_assign_message
  {
    using T = const non_copy_assign_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // nothrow_non_copy_assign_message
  {
    using T = nothrow_non_copy_assign_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const nothrow_non_copy_assign_message
  {
    using T = const nothrow_non_copy_assign_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // non_move_construct_message
  {
    using T = non_move_construct_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const non_move_construct_message
  {
    using T = const non_move_construct_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // nothrow_non_move_construct_message
  {
    using T = nothrow_non_move_construct_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const nothrow_non_move_construct_message
  {
    using T = const nothrow_non_move_construct_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // non_move_assign_message
  {
    using T = non_move_assign_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // const non_move_assign_message
  {
    using T = const non_move_assign_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // nothrow_non_move_assign_message
  {
    using T = nothrow_non_move_assign_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // const nothrow_non_move_assign_message
  {
    using T = const nothrow_non_move_assign_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // non_copy_message
  {
    using T = non_copy_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const non_copy_message

  // nothrow_non_copy_message
  {
    using T = nothrow_non_copy_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const nothrow_non_copy_message

  // non_move_message
  {
    using T = non_move_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // const non_move_message
  {
    using T = const non_move_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // nothrow_non_move_message
  {
    using T = nothrow_non_move_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_FALSE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // const nothrow_non_move_message
  {
    using T = const nothrow_non_move_message;
    using object_type = decltype(build_pmte(T {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // non_default_constructor_message
  {
    using T = non_default_constructor_message;
    using object_type = decltype(build_pmte(T {5}));

    EXPECT_FALSE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // const non_default_constructor_message
  {
    using T = const non_default_constructor_message;
    using object_type = decltype(build_pmte(T {5}));

    EXPECT_FALSE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // nothrow_non_default_constructor_message
  {
    using T = nothrow_non_default_constructor_message;
    using object_type = decltype(build_pmte(T {5}));

    EXPECT_FALSE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // const nothrow_non_default_constructor_message
  {
    using T = const nothrow_non_default_constructor_message;
    using object_type = decltype(build_pmte(T {5}));

    EXPECT_FALSE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }
}

TEST(PMTE, TraitsBuildPmteRefType) {
  // float
  {
    using T1 = float;
    using T2 = int;
    using object_type = decltype(build_pmte<T1>(T2 {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const float
  {
    using T1 = const float;
    using T2 = int;
    using object_type = decltype(build_pmte<T1>(T2 {5}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // void*
  {
    using T1 = void *;
    using T2 = float *;
    using object_type = decltype(build_pmte<T1>(T2 {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const void*
  {
    using T1 = const void *;
    using T2 = float *;
    using object_type = decltype(build_pmte<T1>(T2 {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // float*
  {
    using T1 = const float *;
    using T2 = float *;
    using object_type = decltype(build_pmte<T1>(T2 {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const float*
  {
    using T1 = const float *;
    using T2 = const float *;
    using object_type = decltype(build_pmte<T1>(T2 {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // std::unique_ptr<int>
  {
    using T1 = std::unique_ptr<int>;
    using T2 = std::unique_ptr<int>;
    using object_type = decltype(build_pmte<T1>(T2 {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_FALSE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }
}

TEST(PMTE, TraitsBuildPmteRTTI) {
  {
    using T0 = int;
    using T1 = BaseClass<T0>;
    using T2 = DerivedClass<T0>;
    using object_type = decltype(build_pmte<T1>(T2 {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  {
    using T0 = int;
    using T1 = const BaseClass<T0>;
    using T2 = const DerivedClass<T0>;
    using object_type = decltype(build_pmte<T1>(T2 {}));

    EXPECT_FALSE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  {
    using T0 = int;
    using T1 = BaseClass<T0>;
    using T2 = SuperDerivedClass<T0>;
    using T3 = DerivedClass<T0>;
    using object_type = decltype(build_pmte<T1>(T2 {}, type_list<T3> {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  {
    using T0 = int;
    using T1 = const BaseClass<T0>;
    using T2 = const SuperDerivedClass<T0>;
    using T3 = const DerivedClass<T0>;
    using object_type = decltype(build_pmte<T1>(T2 {}, type_list<T3> {}));

    EXPECT_FALSE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // non_copy_construct_message
  {
    using T0 = non_copy_construct_message;
    using T1 = BaseClass<T0>;
    using T2 = SuperDerivedClass<T0>;
    using T3 = DerivedClass<T0>;
    using object_type = decltype(build_pmte<T1>(T2 {}, type_list<T3> {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_FALSE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif
  }

  // non_copy_assign_message
  {
    using T0 = non_copy_assign_message;
    using T1 = BaseClass<T0>;
    using T2 = SuperDerivedClass<T0>;
    using T3 = DerivedClass<T0>;
    using object_type = decltype(build_pmte<T1>(T2 {}, type_list<T3> {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const non_copy_assign_message
  {
    using T0 = const non_copy_assign_message;
    using T1 = const BaseClass<T0>;
    using T2 = const SuperDerivedClass<T0>;
    using T3 = const DerivedClass<T0>;
    using object_type = decltype(build_pmte<T1>(T2 {}, type_list<T3> {}));

#if defined(__clang__)
    EXPECT_FALSE(std::is_default_constructible<object_type>::value);
#else
    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
#endif
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // non_copy_assign_message
  {
    using T0 = non_copy_assign_message;
    //using T1 = BaseClass<T0>;
    using T2 = SuperDerivedClass<T0>;
    using T3 = DerivedClass<T0>;
    using object_type = decltype(build_pmte(T2 {}, type_list<T3> {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const non_copy_assign_message
  {
    using T0 = non_copy_assign_message;
    //using T1 = const BaseClass<T0>;
    using T2 = const SuperDerivedClass<T0>;
    using T3 = const DerivedClass<T0>;
    using object_type = decltype(build_pmte(T2 {}, type_list<T3> {}));

#if defined(__clang__)
    EXPECT_FALSE(std::is_default_constructible<object_type>::value);
#else
    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
#endif
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // non_move_construct_message
  {
    using T0 = non_move_construct_message;
    using T1 = BaseClass<T0>;
    using T2 = SuperDerivedClass<T0>;
    using T3 = DerivedClass<T0>;
    using object_type = decltype(build_pmte<T1>(T2 {}, type_list<T3> {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const non_move_construct_message
  {
    using T0 = non_move_construct_message;
    using T1 = const BaseClass<T0>;
    using T2 = const SuperDerivedClass<T0>;
    using T3 = const DerivedClass<T0>;
    using object_type = decltype(build_pmte<T1>(T2 {}, type_list<T3> {}));

#if defined(__clang__)
    EXPECT_FALSE(std::is_default_constructible<object_type>::value);
#else
    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
#endif
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // non_move_construct_message
  {
    using T0 = non_move_construct_message;
    using T1 = BaseClass<T0>;
    using T2 = SuperDerivedClass<T0>;
    using T3 = DerivedClass<T0>;
    using object_type = decltype(build_pmte(T2 {}, type_list<T1, T3> {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // const non_move_construct_message
  {
    using T0 = const non_move_construct_message;
    using T1 = BaseClass<T0>;
    using T2 = SuperDerivedClass<T0>;
    using T3 = DerivedClass<T0>;
    using object_type = decltype(build_pmte(T2 {}, type_list<T1, T3> {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_FALSE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_FALSE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_FALSE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_FALSE(object_type::is_copy_assignable);
    EXPECT_FALSE(object_type::is_move_assignable);
  }

  // non_move_assign_message
  {
    using T0 = non_move_assign_message;
    using T1 = BaseClass<T0>;
    using T2 = SuperDerivedClass<T0>;
    using T3 = DerivedClass<T0>;
    using object_type = decltype(build_pmte<T1>(T2 {}, type_list<T3> {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  // non_move_message
  {
    using T0 = non_move_message;
    using T1 = BaseClass<T0>;
    using T2 = SuperDerivedClass<T0>;
    using T3 = DerivedClass<T0>;
    using object_type = decltype(build_pmte<T1>(T2 {}, type_list<T3> {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

#if __cplusplus >= CPP17_STANDARD
    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_FALSE(std::is_nothrow_swappable<object_type>::value);
#endif

    EXPECT_FALSE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }
}

#if __cplusplus >= CPP20_STANDARD && !defined(__clang__) // bug in clang
TEST(PMTE, TraitsBuildPmteDeleter) {
  {
    //using T1 = int;
    using T2 = int;
    using object_type = decltype(build_pmte((T2 *)0, [](T2 *) {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);

    EXPECT_TRUE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  {
    using T1 = nothrow_non_copy_message;
    using T2 = nothrow_non_copy_message;
    using object_type = decltype(build_pmte<T1 *>((T2 *)0, [](T2 *) {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);

    EXPECT_TRUE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  {
    using T1 = BaseClass<float>;
    using T2 = DerivedClass<float>;
    using object_type = decltype(build_pmte<T1 *>((T2 *)0, [](T1 *) {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);

    EXPECT_TRUE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  {
    using T1 = BaseClass<const float>;
    using T2 = DerivedClass<const float>;
    using object_type = decltype(build_pmte<T1 *>((T2 *)0, [](T1 *) {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);

    EXPECT_TRUE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }
}
#endif

#if __cplusplus >= CPP20_STANDARD && !defined(__clang__) // bug in clang
TEST(PMTE, TraitsBuildPmteDeleterRtti) {
  {
    //using T1 = int;
    using T2 = int;
    using T3 = void;
    using object_type = decltype(build_pmte((T2 *)0, [](T2 *) {}, type_list<T3 *> {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);

    EXPECT_TRUE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  {
    using T1 = nothrow_non_copy_message;
    using T2 = nothrow_non_copy_message;
    using T3 = const nothrow_non_copy_message;
    using object_type = decltype(build_pmte<T1 *>((T2 *)0, [](T2 *) {}, type_list<T3 *, void *> {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);

    EXPECT_TRUE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }

  {
    using T1 = BaseClass<int>;
    using T2 = SuperDerivedClass<int>;
    using T3 = DerivedClass<int>;
    using object_type = decltype(build_pmte<T1 *>((T2 *)0, [](T1 *) {}, type_list<T3 *, void *> {}));

    EXPECT_TRUE(std::is_default_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_default_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_constructible<object_type>::value);

    EXPECT_TRUE(std::is_copy_constructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_constructible<object_type>::value);

    EXPECT_TRUE(std::is_move_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<object_type>::value);

    EXPECT_TRUE(std::is_copy_assignable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_copy_assignable<object_type>::value);

    EXPECT_TRUE(std::is_destructible<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_destructible<object_type>::value);

    EXPECT_TRUE(std::has_virtual_destructor<object_type>::value);

    EXPECT_TRUE(std::is_swappable<object_type>::value);
    EXPECT_TRUE(std::is_nothrow_swappable<object_type>::value);

    EXPECT_TRUE(object_type::has_deleter);
    EXPECT_TRUE(object_type::is_copy_constructible);
    EXPECT_TRUE(object_type::is_move_constructible);
    EXPECT_TRUE(object_type::is_copy_assignable);
    EXPECT_TRUE(object_type::is_move_assignable);
  }
}
#endif

// std::cout << type_name<T>() << std::endl;
// std::cout << type_operations<T>() << '\n' << std::endl;
// std::cout << type_operations<util::pmte::detail::value_store<T> >() << '\n' << std::endl;
// std::cout << type_operations<object_type>() << '\n' << std::endl;
