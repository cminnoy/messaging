/**
 * @file util__simulated_clock.cpp
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

#include <util/simulated_clock.hpp>
#include <iostream>

namespace {

util::simulated_clock::time_point current_time;

} // namespace

namespace util {

simulated_clock::time_point simulated_clock::now() noexcept {
  return current_time;
}

void simulated_clock::reset_to_epoch() noexcept {
  current_time = time_point();
}

void simulated_clock::advance(duration d) noexcept {
  current_time += d;
}

double simulated_clock::to_double(time_point tp) noexcept {
  return std::chrono::duration<double>(tp.time_since_epoch()).count();
}

simulated_clock::time_point simulated_clock::from_double(double d) noexcept {
  return simulated_clock::time_point {std::chrono::duration_cast<simulated_clock::duration>(std::chrono::duration<double>(d))};
}

} // namespace util
