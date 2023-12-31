/**
 * @file math.hpp
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

#ifndef INCLUDE__UTIL__MATH_HPP
#define INCLUDE__UTIL__MATH_HPP

#include <limits>
#include <cmath>

#ifdef _MSC_BUILD
#include <iso646.h>
#endif

namespace math {

template <typename T>
constexpr bool isinf(T a) {
  return (a == INFINITY || a == -INFINITY);
}

template <typename T>
constexpr bool equal(T a, T b, T tolerance) {
  return std::abs(a - b) <= std::abs(tolerance);
}

template <>
constexpr bool equal(float a, float b, float tolerance) {
  if (isinf(tolerance)) return true;
  if (std::isnan(tolerance)) return false;
  bool isnan_a = std::isnan(a);
  bool isnan_b = std::isnan(b);
  bool isinf_a = isinf(a);
  bool isinf_b = isinf(b);
  if (isnan_a and isnan_b) return true;
  if (isnan_a != isnan_b) return false;
  if (isinf_a != isinf_b) return false;
  if (isinf_a and isinf_b) {
    return std::signbit(a) == std::signbit(b);
  }
  return std::abs(a - b) <= std::abs(tolerance);
}

template <>
constexpr bool equal(double a, double b, double tolerance) {
  if (isinf(tolerance)) return true;
  if (std::isnan(tolerance)) return false;
  bool isnan_a = std::isnan(a);
  bool isnan_b = std::isnan(b);
  bool isinf_a = isinf(a);
  bool isinf_b = isinf(b);
  if (isnan_a and isnan_b) return true;
  if (isnan_a != isnan_b) return false;
  if (isinf_a != isinf_b) return false;
  if (isinf_a and isinf_b) {
    return std::signbit(a) == std::signbit(b);
  }
  return std::abs(a - b) <= std::abs(tolerance);
}

template <typename T>
constexpr bool equal(T a, T b) {
  return a == b;
}

template <>
constexpr bool equal(float a, float b) {
  return equal<float>(a, b, std::numeric_limits<float>::epsilon());
}

template <>
constexpr bool equal(double a, double b) {
  return equal<double>(a, b, std::numeric_limits<double>::epsilon());
}

template <typename T>
constexpr bool greater(T lhs, T rhs, T tolerance) {
  return (lhs > rhs) and not equal(lhs, rhs, tolerance);
}

template <typename T>
constexpr bool greater(T lhs, T rhs) {
  return lhs > rhs;
}

template <>
constexpr bool greater(float lhs, float rhs) {
  return greater(lhs, rhs, std::numeric_limits<float>::epsilon());
}

template <>
constexpr bool greater(double lhs, double rhs) {
  return greater(lhs, rhs, std::numeric_limits<double>::epsilon());
}

template <typename T>
constexpr bool greater_equal(const T & value, const T & setting, const T & absoluteTolerance) {
  return (value > setting) or equal(value, setting, absoluteTolerance);
}

template <typename T>
constexpr bool greater_equal(const T & value, const T & setting) {
  return (value > setting) or equal(value, setting);
}

template <>
constexpr bool greater_equal(const double & value, const double & setting) {
  return greater_equal(value, setting, std::numeric_limits<double>::epsilon());
}

template <>
constexpr bool greater_equal(const float & value, const float & setting) {
  return greater_equal(value, setting, std::numeric_limits<float>::epsilon());
}

template <typename T>
constexpr bool less(const T & value, const T & setting, const T & absoluteTolerance) {
  return (value < setting) && not equal(value, setting, absoluteTolerance);
}

template <typename T>
constexpr bool less(const T & value, const T & setting) {
  return (value < setting);
}

template <>
constexpr bool less(const double & value, const double & setting) {
  return less(value, setting, std::numeric_limits<double>::epsilon());
}

template <>
constexpr bool less(const float & value, const float & setting) {
  return less(value, setting, std::numeric_limits<float>::epsilon());
}

template <typename T>
constexpr bool less_equal(const T & value, const T & setting, const T & absoluteTolerance) {
  return (value < setting) || equal(value, setting, absoluteTolerance);
}

template <typename T>
constexpr bool less_equal(const T & value, const T & setting) {
  return (value < setting) || equal(value, setting);
}

template <>
constexpr bool less_equal(const double & value, const double & setting) {
  return less_equal(value, setting, std::numeric_limits<double>::epsilon());
}

template <>
constexpr bool less_equal(const float & value, const float & setting) {
  return less_equal(value, setting, std::numeric_limits<float>::epsilon());
}

/**
 * Calculate a scaled value between min and max to 0 to 1.
 *
 * @param value: the value to scale
 * @param min: the original minimum
 * @param max: the original maximum
 * @return: the scaled value between 0 and 1
 */
constexpr double scaled_value(double val, double min, double max) {
  return (val - min) / (max - min);
}

/**
 * Linear interpolate value between (x1, y1) and (x2, y2)
 *
 * @param val: x value to interpolate to
 * @param x1: first x coordinate
 * @param y1: first y coordinate
 * @param x2: second x coordinate
 * @param y2: second y coordinate
 * @return: corresponding y value
 */
template <typename T, typename U>
constexpr U lin_interpol(T val, T x1, T x2, U y1, U y2) {
  return y1 + (y2 - y1) / (x2 - x1) * (val - x1);
}

} // namespace math

#endif /* INCLUDE__UTIL__MATH_HPP */
