/**
 * @file inspect.hpp
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

#pragma once

#ifndef INCLUDE__UTIL__INSPECT_HPP
#define INCLUDE__UTIL__INSPECT_HPP

#include <util/attributes.hpp>

static_assert(__cplusplus >= CPP14_STANDARD, "Minimum required standard is C++14.");

#include <memory>
#include <util/pmte.hpp>

/**
 * @file
 * 
 * Type inspect system
 */

namespace util {

namespace detail {

template <typename T, typename = void>
class inspector {

  // clang-format off
  template <typename U>
  CPP20_CONSTINIT_OR_CONSTEXPR static const bool invocable =
    (std::is_same<U, T>::value)
    ||
    (std::is_same<U, std::remove_reference_t<T>>::value)
    ||
    (
      (std::is_integral<util::remove_cvref_t<T>>::value ||
       std::is_floating_point<util::remove_cvref_t<T>>::value ||
       std::is_enum<util::remove_cvref_t<T>>::value)
      &&
      (std::is_same<U, util::remove_cvref_t<T>>::value)
    )
    ||
    (std::is_class<T>::value && std::is_convertible<T,U>::value)
    ||
    (std::is_pointer<T>::value && std::is_convertible<T,U>::value)
    ||
    (std::is_array<T>::value && std::is_convertible<T,U>::value)
    ||
    (std::is_enum<T>::value && std::is_same<U, util::remove_cvref_t<T>>::value)
    ||
    (std::is_function<T>::value)
    ;
  // clang-format on

public:

  template <typename U, typename Func>
  FORCE_INLINE constexpr auto & as(Func && f) noexcept(noexcept(f)) {
    if (invocable<U>) {
      invoke(f, value);
    }
    return *this;
  }

  template <typename U, typename Func>
  FORCE_INLINE constexpr auto & is(Func && f) noexcept(noexcept(f)) {
    if (invocable<U>) {
      f();
      invoked = true;
    }
    return *this;
  }

  template <typename Func>
  FORCE_INLINE constexpr bool or_else(Func && f) const noexcept(noexcept(f)) {
    if (!invoked) f();
    return invoked;
  }

  FORCE_INLINE bool or_throw() const {
    if (!invoked) throw std::runtime_error(std::string("inspect case not handled"));
    return invoked;
  }

  NO_DISCARD FORCE_INLINE constexpr operator bool() const noexcept { return invoked; }

  template <typename U>
  NO_DISCARD FORCE_INLINE constexpr static decltype(auto) inspect(U && u) noexcept(std::is_nothrow_move_constructible<U>::value) {
    return inspector<U>(std::forward<U>(u));
  }

private:

  template <typename Func, typename U>
  FORCE_INLINE constexpr typename std::enable_if_t<util::is_invocable<Func, U>::value> invoke(Func && f, U && u) noexcept(noexcept(f)) {
    f(std::forward<U>(u));
    invoked = true;
  }

  template <typename Func, typename U>
  FORCE_INLINE constexpr typename std::enable_if_t<!util::is_invocable<Func, U>::value> invoke(Func &&, U &&) const noexcept {}

  FORCE_INLINE constexpr inspector(T && v) noexcept(std::is_nothrow_move_constructible<T>::value) : value(std::forward<T>(v)) {}

  T && value;
  bool invoked = false;
};

template <typename T>
class inspector<std::unique_ptr<T>, typename std::enable_if<!std::is_base_of<pmte::pmte_value, T>::value>::type> {

  // clang-format off
  template <typename U>
  CPP20_CONSTINIT_OR_CONSTEXPR static const bool invocable =
    (std::is_same<U, T>::value)
    ||
    (std::is_same<U, std::remove_reference_t<T>>::value)
    ||
    (
      (std::is_integral<util::remove_cvref_t<T>>::value ||
       std::is_floating_point<util::remove_cvref_t<T>>::value ||
       std::is_enum<util::remove_cvref_t<T>>::value)
      &&
      (std::is_same<U, util::remove_cvref_t<T>>::value)
    )
    ||
    (std::is_class<T>::value && std::is_convertible<T,U>::value)
    ||
    (std::is_pointer<T>::value && std::is_convertible<T,U>::value)
    ||
    (std::is_array<T>::value && std::is_convertible<T,U>::value)
    ||
    (std::is_enum<T>::value && std::is_same<U, util::remove_cvref_t<T>>::value)
    ||
    (std::is_function<T>::value)
    ;
  // clang-format on

public:

  template <typename U, typename Func>
  FORCE_INLINE constexpr auto & as(Func && f) noexcept(noexcept(f)) {
    if (invocable<U>) {
      invoke(f, *ptr);
    }
    return *this;
  }

  template <typename U, typename Func>
  FORCE_INLINE constexpr auto & is(Func && f) noexcept(noexcept(f)) {
    if (invocable<U>) {
      f();
      invoked = true;
    }
    return *this;
  }

  template <typename Func>
  FORCE_INLINE constexpr bool or_else(Func && f) const noexcept(noexcept(f)) {
    if (!invoked) f();
    return invoked;
  }

  FORCE_INLINE bool or_throw() const {
    if (!invoked) throw std::runtime_error(std::string("inspect case not handled"));
    return invoked;
  }

  NO_DISCARD FORCE_INLINE constexpr operator bool() const noexcept { return invoked; }

  template <typename U>
  NO_DISCARD FORCE_INLINE constexpr static decltype(auto) inspect(std::unique_ptr<U> && u) noexcept {
    return inspector<std::unique_ptr<U>>(std::forward<std::unique_ptr<U>>(u));
  }

private:

  template <typename Func, typename U>
  FORCE_INLINE constexpr typename std::enable_if_t<util::is_invocable<Func, U>::value && util::is_invocable<Func, U &>::value> invoke(
    Func && f, U && u) noexcept(noexcept(f)) {
    f(std::forward<U>(u));
    invoked = true;
  }

  template <typename Func, typename U>
  FORCE_INLINE constexpr typename std::enable_if_t<!util::is_invocable<Func, U>::value && !util::is_invocable<Func, U &>::value> invoke(
    Func &&, U &&) const noexcept {}

  template <typename U>
  FORCE_INLINE constexpr inspector(std::unique_ptr<U> && p) noexcept : ptr(std::forward<std::unique_ptr<U>>(p)) {}

  std::unique_ptr<T> && ptr;
  bool invoked = false;
};

template <typename T>
class inspector<const std::unique_ptr<T>, typename std::enable_if<!std::is_base_of<pmte::pmte_value, T>::value>::type> {

  // clang-format off
  template <typename U>
  CPP20_CONSTINIT_OR_CONSTEXPR static const bool invocable =
    (std::is_same<U, T>::value)
    ||
    (std::is_same<U, std::remove_reference_t<T>>::value)
    ||
    (
      (std::is_integral<util::remove_cvref_t<T>>::value ||
       std::is_floating_point<util::remove_cvref_t<T>>::value ||
       std::is_enum<util::remove_cvref_t<T>>::value)
      &&
      (std::is_same<U, util::remove_cvref_t<T>>::value)
    )
    ||
    (std::is_class<T>::value && std::is_convertible<T,U>::value)
    ||
    (std::is_pointer<T>::value && std::is_convertible<T,U>::value)
    ||
    (std::is_array<T>::value && std::is_convertible<T,U>::value)
    ||
    (std::is_enum<T>::value && std::is_same<U, util::remove_cvref_t<T>>::value)
    ||
    (std::is_function<T>::value)
    ;
  // clang-format on

public:

  template <typename U, typename Func>
  FORCE_INLINE constexpr auto & as(Func && f) noexcept(noexcept(f)) {
    if (invocable<U>) {
      invoke(f, *ptr);
    }
    return *this;
  }

  template <typename U, typename Func>
  FORCE_INLINE constexpr auto & is(Func && f) noexcept(noexcept(f)) {
    if (invocable<U>) {
      f();
      invoked = true;
    }
    return *this;
  }

  template <typename Func>
  FORCE_INLINE constexpr bool or_else(Func && f) const noexcept(noexcept(f)) {
    if (!invoked) f();
    return invoked;
  }

  FORCE_INLINE bool or_throw() const {
    if (!invoked) throw std::runtime_error(std::string("inspect case not handled"));
    return invoked;
  }

  NO_DISCARD FORCE_INLINE constexpr operator bool() const noexcept { return invoked; }

  template <typename U>
  NO_DISCARD FORCE_INLINE static constexpr decltype(auto) inspect(std::unique_ptr<U> const & u) noexcept {
    return inspector<const std::unique_ptr<U>>(u);
  }

private:

  template <typename Func, typename U>
  FORCE_INLINE constexpr typename std::enable_if_t<util::is_invocable<Func, U>::value && util::is_invocable<Func, U &>::value> invoke(
    Func && f, U && u) noexcept(noexcept(f)) {
    f(std::forward<U>(u));
    invoked = true;
  }

  template <typename Func, typename U>
  FORCE_INLINE constexpr typename std::enable_if_t<!util::is_invocable<Func, U>::value && !util::is_invocable<Func, U &>::value> invoke(
    Func &&, U &&) const noexcept {}

  template <typename U>
  FORCE_INLINE constexpr inspector(std::unique_ptr<U> const & p) noexcept : ptr(p) {}

  std::unique_ptr<T> const & ptr;
  bool invoked = false;
};

template <typename T>
class inspector<std::shared_ptr<T>, typename std::enable_if<!std::is_base_of<pmte::pmte_value, T>::value>::type> {

  // clang-format off
  template <typename U>
  CPP20_CONSTINIT_OR_CONSTEXPR static const bool invocable =
    (std::is_same<U, T>::value)
    ||
    (std::is_same<U, std::remove_reference_t<T>>::value)
    ||
    (
      (std::is_integral<util::remove_cvref_t<T>>::value ||
       std::is_floating_point<util::remove_cvref_t<T>>::value ||
       std::is_enum<util::remove_cvref_t<T>>::value)
      &&
      (std::is_same<U, util::remove_cvref_t<T>>::value)
    )
    ||
    (std::is_class<T>::value && std::is_convertible<T,U>::value)
    ||
    (std::is_pointer<T>::value && std::is_convertible<T,U>::value)
    ||
    (std::is_array<T>::value && std::is_convertible<T,U>::value)
    ||
    (std::is_enum<T>::value && std::is_same<U, util::remove_cvref_t<T>>::value)
    ||
    (std::is_function<T>::value)
    ;
  // clang-format on

public:

  template <typename U, typename Func>
  FORCE_INLINE auto & as(Func && f) noexcept(noexcept(f)) {
    if (invocable<U>) {
      invoke(f, *ptr);
    }
    return *this;
  }

  template <typename U, typename Func>
  FORCE_INLINE auto & is(Func && f) noexcept(noexcept(f)) {
    if (invocable<U>) {
      f();
      invoked = true;
    }
    return *this;
  }

  template <typename Func>
  FORCE_INLINE bool or_else(Func && f) const noexcept(noexcept(f)) {
    if (!invoked) f();
    return invoked;
  }

  FORCE_INLINE bool or_throw() const {
    if (!invoked) throw std::runtime_error(std::string("inspect case not handled"));
    return invoked;
  }

  NO_DISCARD FORCE_INLINE operator bool() const noexcept { return invoked; }

  template <typename U>
  NO_DISCARD FORCE_INLINE static decltype(auto) inspect(std::shared_ptr<U> && u) noexcept {
    return inspector<std::shared_ptr<U>>(std::forward<std::shared_ptr<U>>(u));
  }

private:

  template <typename Func, typename U>
  FORCE_INLINE typename std::enable_if_t<util::is_invocable<Func, U>::value && util::is_invocable<Func, U &>::value> invoke(Func && f,
    U && u) noexcept(noexcept(f)) {
    f(std::forward<U>(u));
    invoked = true;
  }

  template <typename Func, typename U>
  FORCE_INLINE typename std::enable_if_t<!util::is_invocable<Func, U>::value && !util::is_invocable<Func, U &>::value> invoke(Func &&,
    U &&) const noexcept {}

  template <typename U>
  FORCE_INLINE inspector(std::shared_ptr<U> && p) noexcept : ptr(std::forward<std::shared_ptr<U>>(p)) {}

  std::shared_ptr<T> && ptr;
  bool invoked = false;
};

template <typename T>
class inspector<const std::shared_ptr<T>, typename std::enable_if<!std::is_base_of<pmte::pmte_value, T>::value>::type> {

  // clang-format off
  template <typename U>
  CPP20_CONSTINIT_OR_CONSTEXPR static const bool invocable =
    (std::is_same<U, T>::value)
    ||
    (std::is_same<U, std::remove_reference_t<T>>::value)
    ||
    (
      (std::is_integral<util::remove_cvref_t<T>>::value ||
       std::is_floating_point<util::remove_cvref_t<T>>::value ||
       std::is_enum<util::remove_cvref_t<T>>::value)
      &&
      (std::is_same<U, util::remove_cvref_t<T>>::value)
    )
    ||
    (std::is_class<T>::value && std::is_convertible<T,U>::value)
    ||
    (std::is_pointer<T>::value && std::is_convertible<T,U>::value)
    ||
    (std::is_array<T>::value && std::is_convertible<T,U>::value)
    ||
    (std::is_enum<T>::value && std::is_same<U, util::remove_cvref_t<T>>::value)
    ||
    (std::is_function<T>::value)
    ;
  // clang-format on

public:

  template <typename U, typename Func>
  FORCE_INLINE auto & as(Func && f) noexcept(noexcept(f)) {
    if (invocable<U>) {
      invoke(f, *ptr);
    }
    return *this;
  }

  template <typename U, typename Func>
  FORCE_INLINE auto & is(Func && f) noexcept(noexcept(f)) {
    if (invocable<U>) {
      f();
      invoked = true;
    }
    return *this;
  }

  template <typename Func>
  FORCE_INLINE bool or_else(Func && f) const noexcept(noexcept(f)) {
    if (!invoked) f();
    return invoked;
  }

  FORCE_INLINE bool or_throw() const {
    if (!invoked) throw std::runtime_error(std::string("inspect case not handled"));
    return invoked;
  }

  NO_DISCARD FORCE_INLINE operator bool() const noexcept { return invoked; }

  template <typename U>
  NO_DISCARD FORCE_INLINE static decltype(auto) inspect(std::shared_ptr<U> const & u) noexcept {
    return inspector<const std::shared_ptr<U>>(u);
  }

private:

  template <typename Func, typename U>
  FORCE_INLINE typename std::enable_if_t<util::is_invocable<Func, U>::value && util::is_invocable<Func, U &>::value> invoke(Func && f,
    U && u) noexcept(noexcept(f)) {
    f(std::forward<U>(u));
    invoked = true;
  }

  template <typename Func, typename U>
  FORCE_INLINE typename std::enable_if_t<!util::is_invocable<Func, U>::value && !util::is_invocable<Func, U &>::value> invoke(Func &&,
    U &&) const noexcept {}

  template <typename U>
  FORCE_INLINE inspector(std::shared_ptr<U> const & p) noexcept : ptr(p) {}

  std::shared_ptr<T> const & ptr;
  bool invoked = false;
};

template <typename T>
class inspector<T, typename std::enable_if<std::is_base_of<pmte::pmte_value, T>::value>::type> {
public:

#if __cplusplus >= CPP17_STANDARD
  template <typename U, typename Func>
  auto & as(Func && f) noexcept(noexcept(f)) {
    if constexpr (std::is_pointer_v<U> && std::is_const_v<T>) {
      if (const U p = pmte::get_if<const U>(value)) {
        invoke(f, p);
      }
    } else if constexpr (std::is_pointer_v<U> && !std::is_const_v<T>) {
      if (U p = pmte::get_if<U>(value)) {
        invoke(f, p);
      }
    } else if constexpr (!std::is_pointer_v<U> && std::is_const_v<T>) {
      if (const U * const p = pmte::get_if<const U>(value)) {
        invoke(f, *p);
      }
    } else if constexpr (!std::is_pointer_v<U> && !std::is_const_v<T>) {
      if (is_const(value.type())) {
        if (const U * const p = pmte::get_if<const U>(value)) {
          invoke(f, *p);
        }
      } else {
        if (U * const p = pmte::get_if<U>(value)) {
          invoke(f, *p);
        }
      }
    }
    return *this;
  }

  template <typename U, typename Func>
  auto & is(Func && f) noexcept(noexcept(f)) {
    if constexpr (std::is_const<T>::value) {
      if (pmte::get_if<const U>(value)) {
        f();
        invoked = true;
      }
    } else {
      if (is_const(value.type())) {
        if (pmte::get_if<const U>(value)) {
          f();
          invoked = true;
        }
      } else {
        if (pmte::get_if<U>(value)) {
          f();
          invoked = true;
        }
      }
    }
    return *this;
  }
#else
  template <typename U, typename Func>
  std::enable_if_t<std::is_pointer<U>::value && std::is_const<T>::value, auto &> as(Func && f) noexcept(noexcept(f)) {
    if (const U p = pmte::get_if<const U>(value)) {
      invoke(f, p);
    }
    return *this;
  }

  template <typename U, typename Func>
  std::enable_if_t<std::is_pointer<U>::value && !std::is_const<T>::value, auto &> as(Func && f) noexcept(noexcept(f)) {
    if (U p = pmte::get_if<U>(value)) {
      invoke(f, p);
    }
    return *this;
  }

  template <typename U, typename Func>
  std::enable_if_t<!std::is_pointer<U>::value && std::is_const<T>::value, auto &> as(Func && f) noexcept(noexcept(f)) {
    if (const U * const p = pmte::get_if<const U>(value)) {
      invoke(f, *p);
    }
    return *this;
  }

  template <typename U, typename Func>
  std::enable_if_t<!std::is_pointer<U>::value && !std::is_const<T>::value, auto &> as(Func && f) noexcept(noexcept(f)) {
    if (is_const(value.type())) {
      if (const U * const p = pmte::get_if<const U>(value)) {
        invoke(f, *p);
      }
    } else {
      if (U * const p = pmte::get_if<U>(value)) {
        invoke(f, *p);
      }
    }
    return *this;
  }

  template <typename U, typename Func>
  std::enable_if_t<std::is_const<T>::value, auto &> is(Func && f) noexcept(noexcept(f)) {
    if (pmte::get_if<const U>(value)) {
      f();
      invoked = true;
    }
    return *this;
  }

  template <typename U, typename Func>
  std::enable_if_t<!std::is_const<T>::value, auto &> is(Func && f) noexcept(noexcept(f)) {
    if (is_const(value.type())) {
      if (pmte::get_if<const U>(value)) {
        f();
        invoked = true;
      }
    } else {
      if (pmte::get_if<U>(value)) {
        f();
        invoked = true;
      }
    }
    return *this;
  }
#endif

  template <typename Func>
  FORCE_INLINE bool or_else(Func && f) const noexcept(noexcept(f)) {
    if (!invoked) f();
    return invoked;
  }

  FORCE_INLINE bool or_throw() const {
    if (!invoked) throw std::runtime_error(std::string("inspect case not handled"));
    return invoked;
  }

  NO_DISCARD FORCE_INLINE operator bool() const noexcept { return invoked; }

  template <typename U>
  NO_DISCARD FORCE_INLINE static decltype(auto) inspect(U && u) noexcept(std::is_nothrow_move_constructible<U>::value) {
    return inspector<U>(std::forward<U>(u));
  }

private:

  template <typename Func, typename U>
  FORCE_INLINE typename std::enable_if_t<util::is_invocable<Func, U>::value && util::is_invocable<Func, U &>::value> invoke(Func && f,
    U && u) noexcept(noexcept(f)) {
    f(std::forward<U>(u));
    invoked = true;
  }

  template <typename Func, typename U>
  FORCE_INLINE typename std::enable_if_t<!util::is_invocable<Func, U>::value && !util::is_invocable<Func, U &>::value> invoke(Func &&,
    U &&) const noexcept {}

  FORCE_INLINE inspector(T && v) noexcept(std::is_nothrow_move_constructible<T>::value) : value(std::forward<T>(v)) {}

  T && value;
  bool invoked = false;
};

template <>
class inspector<pmte::pmte_value *> {
public:

  template <typename U, typename Func>
  auto & as(Func && f) noexcept(noexcept(f)) {
    if constexpr (std::is_pointer_v<U>) {
      if (U p = pmte::get_if<U>(ptr)) {
        invoke(f, p);
      }
    }
    if constexpr (!std::is_pointer_v<U>) {
      if (is_const(ptr->type())) {
        if (const U * const p = pmte::get_if<const U>(ptr)) {
          invoke(f, *p);
        }
      } else {
        if (U * const p = pmte::get_if<U>(ptr)) {
          invoke(f, *p);
        }
      }
    }
    return *this;
  }

  template <typename U, typename Func>
  auto & is(Func && f) noexcept(noexcept(f)) {
    if (is_const(ptr->type())) {
      if (pmte::get_if<const U>(ptr)) {
        f();
        invoked = true;
      }
    } else {
      if (pmte::get_if<U>(ptr)) {
        f();
        invoked = true;
      }
    }
    return *this;
  }

  template <typename Func>
  FORCE_INLINE bool or_else(Func && f) const noexcept(noexcept(f)) {
    if (!invoked) f();
    return invoked;
  }

  FORCE_INLINE bool or_throw() const {
    if (!invoked) throw std::runtime_error(std::string("inspect case not handled"));
    return invoked;
  }

  NO_DISCARD FORCE_INLINE operator bool() const noexcept { return invoked; }

  NO_DISCARD FORCE_INLINE static decltype(auto) inspect(pmte::pmte_value * const u) noexcept { return inspector<pmte::pmte_value *>(u); }

private:

  template <typename Func, typename U>
  FORCE_INLINE typename std::enable_if_t<util::is_invocable<Func, U>::value && util::is_invocable<Func, U &>::value> invoke(Func && f,
    U && u) noexcept(noexcept(f)) {
    f(std::forward<U>(u));
    invoked = true;
  }

  template <typename Func, typename U>
  FORCE_INLINE typename std::enable_if_t<!util::is_invocable<Func, U>::value && !util::is_invocable<Func, U &>::value> invoke(Func &&,
    U &&) const noexcept {}

  FORCE_INLINE explicit inspector(pmte::pmte_value * const p) noexcept : ptr(p) {}

  pmte::pmte_value * ptr;
  bool invoked = false;
};

template <>
class inspector<pmte::pmte_value const *> {
public:

  template <typename U, typename Func>
  auto & as(Func && f) noexcept(noexcept(f)) {
    if constexpr (std::is_pointer_v<U>) {
      if (const U p = pmte::get_if<const U>(ptr)) {
        invoke(f, p);
      }
    } else if constexpr (!std::is_pointer_v<U>) {
      if (const U * const p = pmte::get_if<const U>(ptr)) {
        invoke(f, *p);
      }
    }
    return *this;
  }

  template <typename U, typename Func>
  auto & is(Func && f) noexcept(noexcept(f)) {
    if (pmte::get_if<const U>(ptr)) {
      f();
      invoked = true;
    }
    return *this;
  }

  template <typename Func>
  FORCE_INLINE bool or_else(Func && f) const noexcept(noexcept(f)) {
    if (!invoked) f();
    return invoked;
  }

  FORCE_INLINE bool or_throw() const {
    if (!invoked) throw std::runtime_error(std::string("inspect case not handled"));
    return invoked;
  }

  NO_DISCARD FORCE_INLINE operator bool() const noexcept { return invoked; }

  NO_DISCARD FORCE_INLINE static decltype(auto) inspect(pmte::pmte_value const * const u) noexcept {
    return inspector<const pmte::pmte_value *>(u);
  }

private:

  template <typename Func, typename U>
  FORCE_INLINE typename std::enable_if_t<util::is_invocable<Func, U>::value && util::is_invocable<Func, U &>::value> invoke(Func && f,
    U && u) noexcept(noexcept(f)) {
    f(std::forward<U>(u));
    invoked = true;
  }

  template <typename Func, typename U>
  FORCE_INLINE typename std::enable_if_t<!util::is_invocable<Func, U>::value && !util::is_invocable<Func, U &>::value> invoke(Func &&,
    U &&) const noexcept {}

  FORCE_INLINE explicit inspector(pmte::pmte_value const * const p) noexcept : ptr(p) {}

  pmte::pmte_value const * ptr;
  bool invoked = false;
};

template <typename T>
class inspector<std::unique_ptr<T>, typename std::enable_if<std::is_base_of<pmte::pmte_value, T>::value>::type> {
public:

  template <typename U, typename Func>
  auto & as(Func && f) noexcept(noexcept(f)) {
    if constexpr (std::is_pointer<U>::value) {
      if constexpr (std::is_const<T>::value) {
        if (const U p = pmte::get_if<const U>(ptr)) {
          invoke(f, p);
        }
      } else {
        if (U p = pmte::get_if<U>(ptr)) {
          invoke(f, p);
        }
      }
    } else {
      if constexpr (std::is_const<T>::value) {
        if (const U * const p = pmte::get_if<const U>(ptr)) {
          invoke(f, *p);
        }
      } else {
        if (is_const(ptr->type())) {
          if (const U * const p = pmte::get_if<const U>(ptr)) {
            invoke(f, *p);
          }
        } else {
          if (U * const p = pmte::get_if<U>(ptr)) {
            invoke(f, *p);
          }
        }
      }
    }
    return *this;
  }

  template <typename U, typename Func>
  auto & is(Func && f) noexcept(noexcept(f)) {
    if constexpr (std::is_const<T>::value) {
      if (pmte::get_if<const U>(ptr)) {
        f();
        invoked = true;
      }
    } else {
      if (is_const(ptr->type())) {
        if (pmte::get_if<const U>(ptr)) {
          f();
          invoked = true;
        }
      } else {
        if (pmte::get_if<U>(ptr)) {
          f();
          invoked = true;
        }
      }
    }
    return *this;
  }

  template <typename Func>
  NO_DISCARD FORCE_INLINE bool or_else(Func && f) const noexcept(noexcept(f)) {
    if (!invoked) f();
    return invoked;
  }

  FORCE_INLINE bool or_throw() const {
    if (!invoked) throw std::runtime_error(std::string("inspect case not handled"));
    return invoked;
  }

  NO_DISCARD FORCE_INLINE operator bool() const noexcept { return invoked; }

  template <typename U>
  NO_DISCARD FORCE_INLINE static decltype(auto) inspect(std::unique_ptr<U> && u) noexcept {
    return inspector<std::unique_ptr<U>>(std::forward<std::unique_ptr<U>>(u));
  }

private:

  template <typename Func, typename U>
  NO_DISCARD FORCE_INLINE typename std::enable_if_t<util::is_invocable<Func, U>::value && util::is_invocable<Func, U &>::value> invoke(
    Func && f, U && u) noexcept(noexcept(f)) {
    f(std::forward<U>(u));
    invoked = true;
  }

  template <typename Func, typename U>
  NO_DISCARD FORCE_INLINE typename std::enable_if_t<!util::is_invocable<Func, U>::value && !util::is_invocable<Func, U &>::value> invoke(
    Func &&, U &&) const noexcept {}

  template <typename U>
  FORCE_INLINE explicit inspector(std::unique_ptr<U> && p) noexcept : ptr(std::forward<std::unique_ptr<U>>(p)) {}

  std::unique_ptr<T> && ptr;
  bool invoked = false;
};

template <typename T>
class inspector<const std::unique_ptr<T>, typename std::enable_if<std::is_base_of<pmte::pmte_value, T>::value>::type> {
public:

  template <typename U, typename Func>
  auto & as(Func && f) noexcept(noexcept(f)) {
    if constexpr (std::is_pointer<U>::value) {
      if (const U p = pmte::get_if<const U>(ptr)) {
        invoke(f, p);
      }
    } else {
      if (const U * const p = pmte::get_if<const U>(ptr)) {
        invoke(f, *p);
      }
    }
    return *this;
  }

  template <typename U, typename Func>
  auto & is(Func && f) noexcept(noexcept(f)) {
    if constexpr (std::is_const<T>::value) {
      if (pmte::get_if<const U>(ptr)) {
        f();
        invoked = true;
      }
    } else {
      if (is_const(ptr->type())) {
        if (pmte::get_if<const U>(ptr)) {
          f();
          invoked = true;
        }
      } else {
        if (pmte::get_if<U>(ptr)) {
          f();
          invoked = true;
        }
      }
    }
    return *this;
  }

  template <typename Func>
  NO_DISCARD FORCE_INLINE bool or_else(Func && f) const noexcept(noexcept(f)) {
    if (!invoked) f();
    return invoked;
  }

  FORCE_INLINE bool or_throw() const {
    if (!invoked) throw std::runtime_error(std::string("inspect case not handled"));
    return invoked;
  }

  NO_DISCARD FORCE_INLINE operator bool() const noexcept { return invoked; }

  template <typename U>
  NO_DISCARD FORCE_INLINE static decltype(auto) inspect(std::unique_ptr<U> const & u) noexcept {
    return inspector<const std::unique_ptr<U>>(u);
  }

private:

  template <typename Func, typename U>
  NO_DISCARD FORCE_INLINE typename std::enable_if_t<util::is_invocable<Func, U>::value && util::is_invocable<Func, U &>::value> invoke(
    Func && f, U && u) noexcept(noexcept(f)) {
    f(std::forward<U>(u));
    invoked = true;
  }

  template <typename Func, typename U>
  NO_DISCARD FORCE_INLINE typename std::enable_if_t<!util::is_invocable<Func, U>::value && !util::is_invocable<Func, U &>::value> invoke(
    Func &&, U &&) const noexcept {}

  template <typename U>
  FORCE_INLINE explicit inspector(std::unique_ptr<U> && p) noexcept : ptr(std::forward<std::unique_ptr<U>>(p)) {}

  std::unique_ptr<T> const & ptr;
  bool invoked = false;
};

template <typename T>
class inspector<std::shared_ptr<T>, typename std::enable_if<std::is_base_of<pmte::pmte_value, T>::value>::type> {
public:

  template <typename U, typename Func>
  auto & as(Func && f) noexcept(noexcept(f)) {
    if constexpr (std::is_pointer<U>::value) {
      if constexpr (std::is_const<T>::value) {
        if (const U p = pmte::get_if<const U>(ptr)) {
          invoke_for_pointer(f, p);
        }
      } else {
        if (U p = pmte::get_if<U>(ptr)) {
          invoke_for_pointer(f, p);
        }
      }
    } else {
      if constexpr (std::is_const<T>::value) {
        if (const U * const p = pmte::get_if<const U>(ptr)) {
          invoke(f, *p);
        }
      } else {
        if (is_const(ptr->type())) {
          if (const U * const p = pmte::get_if<const U>(ptr)) {
            invoke(f, *p);
          }
        } else {
          if (U * const p = pmte::get_if<U>(ptr)) {
            invoke(f, *p);
          }
        }
      }
    }
    return *this;
  }

  template <typename U, typename Func>
  auto & is(Func && f) noexcept(noexcept(f)) {
    if constexpr (std::is_const<T>::value) {
      if (pmte::get_if<const U>(ptr)) {
        f();
        invoked = true;
      }
    } else {
      if (is_const(ptr->type())) {
        if (pmte::get_if<const U>(ptr)) {
          f();
          invoked = true;
        }
      } else {
        if (pmte::get_if<U>(ptr)) {
          f();
          invoked = true;
        }
      }
    }
    return *this;
  }

  template <typename Func>
  NO_DISCARD FORCE_INLINE bool or_else(Func && f) const noexcept(noexcept(f)) {
    if (!invoked) f();
    return invoked;
  }

  NO_DISCARD FORCE_INLINE bool or_throw() const {
    if (!invoked) throw std::runtime_error(std::string("inspect case not handled"));
    return invoked;
  }

  NO_DISCARD FORCE_INLINE operator bool() const noexcept { return invoked; }

  template <typename U>
  NO_DISCARD FORCE_INLINE static decltype(auto) inspect(std::shared_ptr<U> && u) noexcept {
    return inspector<std::shared_ptr<U>>(std::forward<std::shared_ptr<U>>(u));
  }

private:

  template <typename Func, typename U>
  NO_DISCARD FORCE_INLINE typename std::enable_if_t<util::is_invocable<Func, U>::value && util::is_invocable<Func, U &>::value> invoke(
    Func && f, U && u) noexcept(noexcept(f)) {
    f(std::forward<U>(u));
    invoked = true;
  }

  template <typename Func, typename U>
  NO_DISCARD FORCE_INLINE typename std::enable_if_t<!util::is_invocable<Func, U>::value && !util::is_invocable<Func, U &>::value> invoke(
    Func &&, U &&) const noexcept {}

  template <typename U>
  FORCE_INLINE explicit inspector(std::shared_ptr<U> && p) noexcept : ptr(std::forward<std::shared_ptr<U>>(p)) {}

  std::shared_ptr<T> && ptr;
  bool invoked = false;
};

template <typename T>
class inspector<const std::shared_ptr<T>, typename std::enable_if<std::is_base_of<pmte::pmte_value, T>::value>::type> {
public:

  template <typename U, typename Func>
  auto & as(Func && f) noexcept(noexcept(f)) {
    if constexpr (std::is_pointer<U>::value) {
      if (const U p = pmte::get_if<const U>(ptr)) {
        invoke_for_pointer(f, p);
      }
    } else {
      if (const U * const p = pmte::get_if<const U>(ptr)) {
        invoke(f, *p);
      }
    }
    return *this;
  }

  template <typename U, typename Func>
  auto & is(Func && f) noexcept(noexcept(f)) {
    if (pmte::get_if<const U>(ptr)) {
      f();
      invoked = true;
    }
    return *this;
  }

  template <typename Func>
  NO_DISCARD FORCE_INLINE bool or_else(Func && f) const noexcept(noexcept(f)) {
    if (!invoked) f();
    return invoked;
  }

  NO_DISCARD FORCE_INLINE bool or_throw() const {
    if (!invoked) throw std::runtime_error(std::string("inspect case not handled"));
    return invoked;
  }

  NO_DISCARD FORCE_INLINE operator bool() const noexcept { return invoked; }

  template <typename U>
  NO_DISCARD FORCE_INLINE static decltype(auto) inspect(std::shared_ptr<U> const & u) noexcept {
    return inspector<const std::shared_ptr<U>>(u);
  }

private:

  template <typename Func, typename U>
  NO_DISCARD FORCE_INLINE typename std::enable_if_t<util::is_invocable<Func, U>::value && util::is_invocable<Func, U &>::value> invoke(
    Func && f, U && u) noexcept(noexcept(f)) {
    f(std::forward<U>(u));
    invoked = true;
  }

  template <typename Func, typename U>
  NO_DISCARD FORCE_INLINE typename std::enable_if_t<!util::is_invocable<Func, U>::value && !util::is_invocable<Func, U &>::value> invoke(
    Func &&, U &&) const noexcept {}

  template <typename U>
  FORCE_INLINE explicit inspector(std::shared_ptr<U> const & p) noexcept : ptr(p) {}

  std::shared_ptr<T> const & ptr;
  bool invoked = false;
};

} // namespace detail


template <typename U>
constexpr decltype(auto) inspect(U && value,
  std::enable_if_t<!std::is_pointer<std::remove_reference_t<U>>::value ||
    (std::is_pointer<std::remove_reference_t<U>>::value && !std::is_convertible<std::remove_reference_t<U>, pmte::pmte_value const *>::value)> * =
    nullptr) {
  return detail::inspector<U>::inspect(std::forward<U>(value));
}

template <typename U>
constexpr decltype(auto) inspect(U & value,
  std::enable_if_t<!std::is_pointer<std::remove_reference_t<U>>::value ||
    (std::is_pointer<std::remove_reference_t<U>>::value && !std::is_convertible<std::remove_reference_t<U>, pmte::pmte_value const *>::value)> * =
    nullptr) {
  return detail::inspector<U>::inspect(std::forward<U>(value));
}

template <typename U>
constexpr decltype(auto) inspect(U const & value,
  std::enable_if_t<!std::is_pointer<std::remove_reference_t<U>>::value ||
    (std::is_pointer<std::remove_reference_t<U>>::value && !std::is_convertible<std::remove_reference_t<U>, pmte::pmte_value const *>::value)> * =
    nullptr) {
  return detail::inspector<const U>::inspect(std::forward<const U>(value));
}


CONST_FUNC decltype(auto) inspect(pmte::pmte_value * const ptr) {
  return detail::inspector<pmte::pmte_value *>::inspect(ptr);
}

CONST_FUNC decltype(auto) inspect(pmte::pmte_value const * const ptr) {
  return detail::inspector<pmte::pmte_value const *>::inspect(ptr);
}


template <typename U>
constexpr decltype(auto) inspect(std::unique_ptr<U> && value) {
  return detail::inspector<std::unique_ptr<U>>::inspect(std::forward<std::unique_ptr<U>>(value));
}

template <typename U>
constexpr decltype(auto) inspect(std::unique_ptr<U> & value) {
  return detail::inspector<std::unique_ptr<U>>::inspect(std::forward<std::unique_ptr<U>>(value));
}

template <typename U>
constexpr decltype(auto) inspect(std::unique_ptr<U> const & value) {
  return std::add_const_t<detail::inspector<const std::unique_ptr<U>>>::inspect(value);
}


template <typename U>
decltype(auto) inspect(std::shared_ptr<U> && value) {
  return detail::inspector<std::shared_ptr<U>>::inspect(std::forward<std::shared_ptr<U>>(value));
}

template <typename U>
decltype(auto) inspect(std::shared_ptr<U> & value) {
  return detail::inspector<std::shared_ptr<U>>::inspect(std::forward<std::shared_ptr<U>>(value));
}

template <typename U>
decltype(auto) inspect(std::shared_ptr<U> const & value) {
  return detail::inspector<std::shared_ptr<U>>::inspect(std::shared_ptr<U>(value));
}

} // namespace util

#endif /* INCLUDE__UTIL__INSPECT_HPP */
