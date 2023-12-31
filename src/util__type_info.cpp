/**
 * @file util__type_info.cpp
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

#include <iomanip>
#include <util/type_info.hpp>

namespace util {

template struct type_info<bool>;
template struct type_info<char>;
template struct type_info<signed char>;
template struct type_info<unsigned char>;
template struct type_info<signed short int>;
template struct type_info<unsigned short int>;
template struct type_info<signed int>;
template struct type_info<unsigned int>;
template struct type_info<signed long int>;
template struct type_info<unsigned long int>;
template struct type_info<signed long long int>;
template struct type_info<unsigned long long int>;
template struct type_info<float>;
template struct type_info<double>;
template struct type_info<long double>;
template struct type_info<void *>;
template struct type_info<char *>;
template struct type_info<signed char *>;
template struct type_info<unsigned char *>;
template struct type_info<signed short int *>;
template struct type_info<unsigned short int *>;
template struct type_info<signed int *>;
template struct type_info<unsigned int *>;
template struct type_info<signed long int *>;
template struct type_info<unsigned long int *>;
template struct type_info<signed long long int *>;
template struct type_info<unsigned long long int *>;
template struct type_info<float *>;
template struct type_info<double *>;
template struct type_info<long double *>;

std::ostream & operator<<(std::ostream & out, type_base const & type) {
  out << '{';
  out << std::quoted("name") << ":\"" << get_name(type) << "\"";
  out << ',' << std::quoted("mangled_name") << ":\"" << get_type_info(type).name() << "\"";
  out << ',' << std::quoted("size") << ':' << get_size(type);
  out << ',' << std::quoted("hash_code") << ':' << get_hash_code(type);
  out << ',' << std::quoted("const_hash_code") << ':' << get_const_hash_code(type);
  out << ',' << std::quoted("is_void") << ':' << is_void(type);
  out << ',' << std::quoted("is_null_pointer") << ':' << is_null_pointer(type);
  out << ',' << std::quoted("is_integral") << ':' << is_integral(type);
  out << ',' << std::quoted("is_floating_point") << ':' << is_floating_point(type);
  out << ',' << std::quoted("is_array") << ':' << is_array(type);
  out << ',' << std::quoted("is_enum") << ':' << is_enum(type);
  out << ',' << std::quoted("is_union") << ':' << is_union(type);
  out << ',' << std::quoted("is_class") << ':' << is_class(type);
  out << ',' << std::quoted("is_function") << ':' << is_function(type);
  out << ',' << std::quoted("is_pointer") << ':' << is_pointer(type);
  out << ',' << std::quoted("is_lvalue_reference") << ':' << is_lvalue_reference(type);
  out << ',' << std::quoted("is_rvalue_reference") << ':' << is_rvalue_reference(type);
  out << ',' << std::quoted("is_member_object_pointer") << ':' << is_member_object_pointer(type);
  out << ',' << std::quoted("is_member_function_pointer") << ':' << is_member_function_pointer(type);
  out << ',' << std::quoted("is_fundamental") << ':' << is_fundamental(type);
  out << ',' << std::quoted("is_arithmetic") << ':' << is_arithmetic(type);
  out << ',' << std::quoted("is_scalar") << ':' << is_scalar(type);
  out << ',' << std::quoted("is_object") << ':' << is_object(type);
  out << ',' << std::quoted("is_compound") << ':' << is_compound(type);
  out << ',' << std::quoted("is_reference") << ':' << is_reference(type);
  out << ',' << std::quoted("is_member_pointer") << ':' << is_member_pointer(type);
  out << ',' << std::quoted("is_const") << ':' << is_const(type);
  out << ',' << std::quoted("is_volatile") << ':' << is_volatile(type);
  out << ',' << std::quoted("is_trivial") << ':' << is_trivial(type);
  out << ',' << std::quoted("is_trivially_copyable") << ':' << is_trivially_copyable(type);
  out << ',' << std::quoted("is_standard_layout") << ':' << is_standard_layout(type);
#if __cplusplus < CPP20_STANDARD
  out << ',' << std::quoted("is_pod") << ':' << is_pod(type);
#endif
#if __cplusplus < CPP17_STANDARD
  out << ',' << std::quoted("is_literal_type") << ':' << is_literal_type(type);
#endif
#if __cplusplus >= CPP17_STANDARD
  out << ',' << std::quoted("has_unique_object_representations") << ':' << has_unique_object_representations(type);
#endif
  out << ',' << std::quoted("is_empty") << ':' << is_empty(type);
  out << ',' << std::quoted("is_polymorphic") << ':' << is_polymorphic(type);
  out << ',' << std::quoted("is_abstract") << ':' << is_abstract(type);
#if __cplusplus >= CPP14_STANDARD
  out << ',' << std::quoted("is_final") << ':' << is_final(type);
#endif
#if __cplusplus >= CPP17_STANDARD
  out << ',' << std::quoted("is_aggregate") << ':' << is_aggregate(type);
#endif
  out << ',' << std::quoted("is_signed") << ':' << is_signed(type);
  out << ',' << std::quoted("is_unsigned") << ':' << is_unsigned(type);
#if __cplusplus >= CPP20_STANDARD
  out << ',' << std::quoted("is_bounded_array") << ':' << is_bounded_array(type);
  out << ',' << std::quoted("is_unbounded_array") << ':' << is_unbounded_array(type);
#endif
#if __cplusplus >= CPP23_STANDARD
  out << ',' << std::quoted("is_scoped_enum") << ':' << is_scoped_enum(type);
#endif
  out << ',' << std::quoted("is_default_constructible") << ':' << is_default_constructible(type);
  out << ',' << std::quoted("is_trivially_default_constructible") << ':' << is_trivially_default_constructible(type);
  out << ',' << std::quoted("is_nothrow_default_constructible") << ':' << is_nothrow_default_constructible(type);
  out << ',' << std::quoted("is_copy_constructible") << ':' << is_copy_constructible(type);
  out << ',' << std::quoted("is_trivially_copy_constructible") << ':' << is_trivially_copy_constructible(type);
  out << ',' << std::quoted("is_nothrow_copy_constructible") << ':' << is_nothrow_copy_constructible(type);
  out << ',' << std::quoted("is_move_constructible") << ':' << is_move_constructible(type);
  out << ',' << std::quoted("is_trivially_move_constructible") << ':' << is_trivially_move_constructible(type);
  out << ',' << std::quoted("is_nothrow_move_constructible") << ':' << is_nothrow_move_constructible(type);
  out << ',' << std::quoted("is_copy_assignable") << ':' << is_copy_assignable(type);
  out << ',' << std::quoted("is_trivially_copy_assignable") << ':' << is_trivially_copy_assignable(type);
  out << ',' << std::quoted("is_move_assignable") << ':' << is_move_assignable(type);
  out << ',' << std::quoted("is_trivially_move_assignable") << ':' << is_trivially_move_assignable(type);
  out << ',' << std::quoted("is_nothrow_move_assignable") << ':' << is_nothrow_move_assignable(type);
  out << ',' << std::quoted("is_destructible") << ':' << is_destructible(type);
  out << ',' << std::quoted("is_trivially_destructible") << ':' << is_trivially_destructible(type);
  out << ',' << std::quoted("is_nothrow_destructible") << ':' << is_nothrow_destructible(type);
  out << ',' << std::quoted("has_virtual_destructor") << ':' << has_virtual_destructor(type);
#if __cplusplus >= CPP17_STANDARD
  out << ',' << std::quoted("is_swappable") << ':' << is_swappable(type);
  out << ',' << std::quoted("is_nothrow_swappable") << ':' << is_nothrow_swappable(type);
#endif
  out << '}';
  return out;
}

} // namespace util
