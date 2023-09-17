/**
 * @file algorithm.hpp
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

#pragma once

#ifndef INCLUDE__UTIL__ALGORITHM_HPP
#define INCLUDE__UTIL__ALGORITHM_HPP

#include <tuple>
#include <memory>

#include <util/traits.hpp>
#include <util/attributes.hpp>

static_assert(__cplusplus >= CPP11_STANDARD, "Minimum required standard is C++11.");


#if __cplusplus >= CPP17_STANDARD

template <typename F, typename... Args>
F for_each_arg(F f, Args &&... args) {
  (f(std::forward<Args>(args)), ...);
  return f;
}

template <typename TupleT, typename Fn>
void for_each_tuple(TupleT && tp, Fn && fn) {
  std::apply([&fn](auto &&... args) { (fn(std::forward<decltype(args)>(args)), ...); }, std::forward<TupleT>(tp));
}

#else

#include <initializer_list>

namespace detail {

template <class Tuple, class F, std::size_t... I>
constexpr F for_each_impl(Tuple && t, F && f, std::index_sequence<I...>) {
  return (void)std::initializer_list<int> {(std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))), 0)...}, f;
}

} // namespace detail

template <typename F, typename... Args>
F for_each_arg(F f, Args &&... args) {
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-value"
#endif
  std::initializer_list<int> {((void)f(std::forward<Args>(args)), 0)...};
#ifdef __clang__
#pragma clang diagnostic pop
#endif
  return f;
}

template <class Tuple, class F>
constexpr F for_each(Tuple && t, F && f) {
  return detail::for_each_impl(std::forward<Tuple>(t), std::forward<F>(f),
    std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value> {});
}

#endif


#if __cplusplus >= CPP20_STANDARD && !defined(__clang__) // bug in clang
#include <cstdio>
/**
 * @brief RAII helper for FILE pointers
 * 
 * @example [[nodiscard]] file_ptr fopen("filename", "r");
 */
using file_ptr = std::unique_ptr<FILE, decltype([](FILE * f) { fclose(f); })>;
#endif


#if __cplusplus >= CPP17_STANDARD
/**
 * @brief Wrapper around visitor functions.
 *
 * @tparam ...Functions
 * 
 * @example
 *
 * std::variant<T1,T2,T3> myvariant; 
 * std::visit(
 *      overload(
 *          [](T1 const& value) -> T const& { ... },
 *          [](T2 const& value) -> T const& { ... },
 *          [](T3 const& value) -> T const& { ... }
 *      ),
 *      myvariant
 *  );
 */
template <typename... Functions>
struct overload : Functions... {
  using Functions::operator()...;
  overload(Functions... functions) : Functions(functions)... {}
};
#endif


/**
 * @brief Tuple tools.
 */
namespace util {

namespace detail {

template <std::size_t... Ns, typename... Ts>
constexpr auto tuple_tail_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t) {
  using tuple_type = std::tuple<std::remove_reference_t<decltype(std::get<Ns + 1u>(t))>...>;
  return tuple_type(std::move(std::get<Ns + 1u>(t))...);
}

#if __cplusplus >= CPP17_STANDARD
template <std::size_t I, typename Tuple, std::size_t... Is>
constexpr auto element_as_tuple(Tuple && tuple, std::index_sequence<Is...>) {
  using T = std::remove_reference_t<Tuple>;
  if constexpr (!(std::is_same_v<std::tuple_element_t<I, T>, std::tuple_element_t<Is, T>> || ...))
    return std::tuple<std::tuple_element_t<I, T>>(std::get<I>(std::forward<Tuple>(tuple)));
  else
    return std::make_tuple();
}
#else
template <std::size_t I, typename Tuple, std::size_t... Is>
using element_as_tuple_condition = typename not_same<typename std::tuple_element_t<I, std::remove_reference_t<Tuple>>>::template as<
  typename std::tuple_element_t<Is, std::remove_reference_t<Tuple>>...>;


template <std::size_t I, typename Tuple, std::size_t... Is, std::enable_if_t<detail::element_as_tuple_condition<I, Tuple, Is...>::value> * = nullptr>
constexpr auto element_as_tuple(Tuple && tuple, std::index_sequence<Is...>) {
  using T = std::remove_reference_t<Tuple>;
  return std::tuple<std::tuple_element_t<I, T>>(std::get<I>(std::forward<Tuple>(tuple)));
}

template <std::size_t I, typename Tuple, std::size_t... Is, std::enable_if_t<!detail::element_as_tuple_condition<I, Tuple, Is...>::value> * = nullptr>
constexpr auto element_as_tuple(Tuple &&, std::index_sequence<Is...>) {
  return std::make_tuple();
}
#endif

} // namespace detail


/**
 * @brief Get head of tuple as tuple
 */
template <typename T, typename... Ts>
constexpr auto tuple_head(std::tuple<T, Ts...> t) {
  return detail::element_as_tuple<0>(t, std::make_index_sequence<0>());
}

/**
 * @brief Get everything after the head
 */
template <typename... Ts>
constexpr auto tuple_tail(std::tuple<Ts...> t) {
  return detail::tuple_tail_impl(std::make_index_sequence<sizeof...(Ts) - 1u>(), t);
}

/**
 * @brief Remove duplicate element types from the tuple.
 */
template <typename Tuple, std::size_t... Is>
constexpr auto make_tuple_unique(Tuple && tuple, std::index_sequence<Is...>) {
  return std::tuple_cat(detail::element_as_tuple<Is>(std::forward<Tuple>(tuple), std::make_index_sequence<Is>())...);
}

/**
 * @brief Remove duplicate element types from the provided tuples.
 */
template <typename... Tuples>
constexpr auto make_tuple_unique(Tuples &&... tuples) {
  auto all = std::tuple_cat(std::forward<Tuples>(tuples)...);
  return make_tuple_unique(std::move(all), std::make_index_sequence<std::tuple_size<decltype(all)>::value> {});
}

template <typename... Types, std::enable_if_t<(sizeof...(Types) > 2)> * = nullptr>
constexpr auto tuple_swap_first_and_second_element(std::tuple<Types...> tup) {
  auto head1 = tuple_head(tup);
  auto tail1 = tuple_tail(tup);
  auto head2 = tuple_head(tail1);
  auto tail2 = tuple_tail(tail1);
  return std::tuple_cat(std::move(head2), std::move(head1), std::move(tail2));
}

template <typename... Types, std::enable_if_t<(sizeof...(Types) == 2)> * = nullptr>
constexpr auto tuple_swap_first_and_second_element(std::tuple<Types...> tup) {
  auto head = tuple_head(tup);
  auto tail = tuple_tail(tup);
  return std::tuple_cat(std::move(tail), std::move(head));
}

template <typename... Types, std::enable_if_t<(sizeof...(Types) < 2)> * = nullptr>
constexpr auto tuple_swap_first_and_second_element(std::tuple<Types...> tup) {
  return tup;
}

/**
 * @brief Print sequence of constant integers
 */
#if __cplusplus >= CPP17_STANDARD
template <typename T, T... ints>
std::ostream & operator<<(std::ostream & out, std::integer_sequence<T, ints...>) {
  ((out << ints << ' '), ...);
  return out;
}
#endif

/**
 * Sort a tuple on:
 *  - classes before pointers
 *  - pointers before builtin types
 *  - most derived class first
 *  - non-const always before const
 * 
 * @note pointer to class is only checked one level deep
 * @warning more than one level of indirection (pointer) will result in undefined behaviour
 */
#if __cplusplus >= CPP14_STANDARD

namespace detail {

template <typename... Types, std::enable_if_t<sizeof...(Types) <= 1> * = nullptr>
constexpr auto make_tuple_sorted_imp(std::tuple<Types...> tup) {
  return tup;
}

#if __cplusplus >= CPP17_STANDARD

template <typename... Types, std::enable_if_t<1 < sizeof...(Types)> * = nullptr>
constexpr auto make_tuple_sorted_imp(std::tuple<Types...> tup) {
  constexpr bool is_same = std::is_same_v<std::decay_t<std::tuple_element_t<0, decltype(tup)>>, std::decay_t<std::tuple_element_t<1, decltype(tup)>>>;
  constexpr bool sub_is_same = std::is_same_v<std::decay_t<std::remove_pointer_t<std::tuple_element_t<0, decltype(tup)>>>,
    std::decay_t<std::remove_pointer_t<std::tuple_element_t<1, decltype(tup)>>>>;

  constexpr bool first_is_const = std::is_const_v<std::remove_pointer_t<std::tuple_element_t<0, decltype(tup)>>>;
  constexpr bool second_is_const = std::is_const_v<std::remove_pointer_t<std::tuple_element_t<1, decltype(tup)>>>;
  constexpr bool const_ok = is_same ? (not first_is_const and second_is_const) : true;
  constexpr bool sub_const_ok = sub_is_same ? (not first_is_const and second_is_const) : true;

  constexpr bool first_is_class = std::is_class_v<std::tuple_element_t<0, decltype(tup)>>;
  constexpr bool second_is_class = std::is_class_v<std::tuple_element_t<1, decltype(tup)>>;

  constexpr bool first_is_indirect_class = std::is_pointer_v<std::tuple_element_t<0, decltype(tup)>> ?
    std::is_class_v<std::remove_pointer_t<std::tuple_element_t<0, decltype(tup)>>> :
    false;

  constexpr bool second_is_indirect_class = std::is_pointer_v<std::tuple_element_t<1, decltype(tup)>> ?
    std::is_class_v<std::remove_pointer_t<std::tuple_element_t<1, decltype(tup)>>> :
    false;

  constexpr bool first_is_pointer = std::is_pointer_v<std::tuple_element_t<0, decltype(tup)>>;
  constexpr bool second_is_pointer = std::is_pointer_v<std::tuple_element_t<1, decltype(tup)>>;

  constexpr bool base_ok = std::is_base_of_v<std::remove_pointer_t<std::tuple_element_t<1, decltype(tup)>>,
    std::remove_pointer_t<std::tuple_element_t<0, decltype(tup)>>>;

  constexpr bool must_swap = (is_same && !const_ok) || (sub_is_same && !sub_const_ok) || (first_is_class && second_is_class && !base_ok) ||
    (first_is_indirect_class && second_is_indirect_class && !base_ok) ||
    (!first_is_indirect_class && !first_is_class && second_is_indirect_class) || (!first_is_class && second_is_class) ||
    (!first_is_pointer && !first_is_class && second_is_pointer);

  if constexpr (must_swap) {
    auto new_tup = util::tuple_swap_first_and_second_element(tup);
    auto head = util::tuple_head(new_tup);
    auto tail = util::tuple_tail(new_tup);
    auto sorted_tail = make_tuple_sorted_imp(tail);
    return std::tuple_cat(head, sorted_tail);
  } else {
    auto head = util::tuple_head(tup);
    auto tail = util::tuple_tail(tup);
    auto sorted_tail = make_tuple_sorted_imp(tail);
    return std::tuple_cat(head, sorted_tail);
  }
}

#else

template <typename... Types, std::enable_if_t<1 < sizeof...(Types)> * = nullptr>
constexpr auto make_tuple_sorted_imp(std::tuple<Types...>);

template <typename... Types>
constexpr auto make_tuple_sorted_subimp(std::tuple<Types...> tup, std::false_type) {
  auto head = util::tuple_head(tup);
  auto tail = util::tuple_tail(tup);
  auto sorted_tail = make_tuple_sorted_imp(tail);
  return std::tuple_cat(head, sorted_tail);
}

template <typename... Types>
constexpr auto make_tuple_sorted_subimp(std::tuple<Types...> tup, std::true_type) {
  auto new_tup = util::tuple_swap_first_and_second_element(tup);
  auto head = util::tuple_head(new_tup);
  auto tail = util::tuple_tail(new_tup);
  auto sorted_tail = make_tuple_sorted_imp(tail);
  return std::tuple_cat(head, sorted_tail);
}

template <typename... Types, std::enable_if_t<1 < sizeof...(Types)> *>
constexpr auto make_tuple_sorted_imp(std::tuple<Types...> tup) {
  constexpr bool
    is_same = std::is_same<std::decay_t<std::tuple_element_t<0, decltype(tup)>>, std::decay_t<std::tuple_element_t<1, decltype(tup)>>>::value;
  constexpr bool sub_is_same = std::is_same<std::decay_t<std::remove_pointer_t<std::tuple_element_t<0, decltype(tup)>>>,
    std::decay_t<std::remove_pointer_t<std::tuple_element_t<1, decltype(tup)>>>>::value;

  constexpr bool first_is_const = std::is_const<std::remove_pointer_t<std::tuple_element_t<0, decltype(tup)>>>::value;
  constexpr bool second_is_const = std::is_const<std::remove_pointer_t<std::tuple_element_t<1, decltype(tup)>>>::value;
  constexpr bool const_ok = is_same ? (not first_is_const and second_is_const) : true;
  constexpr bool sub_const_ok = sub_is_same ? (not first_is_const and second_is_const) : true;

  constexpr bool first_is_class = std::is_class<std::tuple_element_t<0, decltype(tup)>>::value;
  constexpr bool second_is_class = std::is_class<std::tuple_element_t<1, decltype(tup)>>::value;

  constexpr bool first_is_indirect_class = std::is_pointer<std::tuple_element_t<0, decltype(tup)>>::value ?
    std::is_class<std::remove_pointer_t<std::tuple_element_t<0, decltype(tup)>>>::value :
    false;

  constexpr bool second_is_indirect_class = std::is_pointer<std::tuple_element_t<1, decltype(tup)>>::value ?
    std::is_class<std::remove_pointer_t<std::tuple_element_t<1, decltype(tup)>>>::value :
    false;

  constexpr bool first_is_pointer = std::is_pointer<std::tuple_element_t<0, decltype(tup)>>::value;
  constexpr bool second_is_pointer = std::is_pointer<std::tuple_element_t<1, decltype(tup)>>::value;

  constexpr bool base_ok = std::is_base_of<std::remove_pointer_t<std::tuple_element_t<1, decltype(tup)>>,
    std::remove_pointer_t<std::tuple_element_t<0, decltype(tup)>>>::value;

  constexpr bool must_swap = (is_same && !const_ok) || (sub_is_same && !sub_const_ok) || (first_is_class && second_is_class && !base_ok) ||
    (first_is_indirect_class && second_is_indirect_class && !base_ok) ||
    (!first_is_indirect_class && !first_is_class && second_is_indirect_class) || (!first_is_class && second_is_class) ||
    (!first_is_pointer && !first_is_class && second_is_pointer);

  using T = typename std::conditional<must_swap, std::true_type, std::false_type>::type;
  return make_tuple_sorted_subimp(tup, T {});
}
#endif

} // namespace detail

template <typename... Types, std::enable_if_t<sizeof...(Types) <= 1> * = nullptr>
constexpr auto make_tuple_sorted(std::tuple<Types...> tup) {
  return tup;
}

template <std::size_t I = 0, typename... Types, std::enable_if_t<sizeof...(Types) >= 2 && I == sizeof...(Types) - 1> * = nullptr>
constexpr auto make_tuple_sorted(std::tuple<Types...> tup) {
  return tup;
}

template <std::size_t I = 0, typename... Types,
  std::enable_if_t<sizeof...(Types) >= 2 && I<sizeof...(Types) - 1> * = nullptr> constexpr auto make_tuple_sorted(std::tuple<Types...> tup) {
  return make_tuple_sorted<I + 1>(detail::make_tuple_sorted_imp(tup));
}
#endif


//@{
namespace hash {

// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function

// FNV-1a 32bit hashing algorithm.
inline constexpr std::uint32_t fnv1a_32(char const * const key, const std::size_t count) noexcept {
  return count ? (fnv1a_32(key, count - 1) ^ key[count - 1]) * 16777619u : 2166136261u;
}

// FNV-1a 64bit hashing algorithm.
inline constexpr std::uint64_t fnv1a_64(char const * const key, const std::size_t count) noexcept {
  std::uint64_t hash = 0xcbf2'9ce4'8422'2325UL;
  const std::uint64_t prime = 0x0000'0100'0000'01b3UL;

  for (std::size_t i = 0; i < count; ++i) {
    std::uint8_t value = key[i];
    hash = hash ^ value;
    hash *= prime;
  }
  return hash;
}

} // namespace hash

template <typename T>
constexpr std::uint32_t hash_fnv1a_32(T const & s) noexcept {
  return util::hash::fnv1a_32(s.data(), s.size());
}

template <typename T>
constexpr std::uint64_t hash_fnv1a_64(T const & s) noexcept {
  return util::hash::fnv1a_64(s.data(), s.size());
}

//@}

#if __cplusplus >= CPP20_STANDARD
/**
 * @brief Call expression N times in an unrolled manner (not in a loop).
 * 
 * @tparam N Number of times to unroll.
 */
template <std::size_t N>
constexpr auto unroll = [](auto expr) {
  [expr]<auto... Is>(std::index_sequence<Is...>) {
    ((expr(), void(Is)), ...);
  }(std::make_index_sequence<N> {});
};
#endif

/**
 * @brief Convert a variadic pack to a single type.
 * 
 * @tparam Ts 
 */
template <typename... Ts>
struct type_sequence {};

/**
 * @brief Get head of variadic pack.
 */
//@{
template <typename... Ts>
struct head;

template <typename T, typename... Ts>
struct head<type_sequence<T, Ts...>> {
  using type = T;
};

template <typename T>
using head_t = typename head<T>::type;
//@}

/**
 * @brief Get remaining tail of variadic pack.
 */
//@{
template <typename... Ts>
struct tail;

template <typename T, typename... Ts>
struct tail<type_sequence<T, Ts...>> {
  using type = type_sequence<Ts...>;
};

template <typename T>
using tail_t = typename tail<T>::type;
//@}

} // namespace util

#endif /* INCLUDE__UTIL__ALGORITHM_HPP */
