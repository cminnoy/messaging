/**
 * @file stop_token.hpp
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

#ifndef INCLUDE__MESSAGING__STOP_TOKEN_HPP
#define INCLUDE__MESSAGING__STOP_TOKEN_HPP

#include <util/attributes.hpp>
#include <atomic>

namespace messaging {

class stop_token;
class stop_source;

class stop_state final {
  friend class messaging::stop_token;
  friend class messaging::stop_source;

  std::atomic<bool> should_stop {false};
};

class stop_token final {
public:
  stop_token() noexcept : state(nullptr) {}
  stop_token(stop_token const &) noexcept = default;
  stop_token(stop_token &&) noexcept = default;
  stop_token & operator=(stop_token const &) noexcept = default;
  stop_token & operator=(stop_token &&) noexcept = default;
  ~stop_token() noexcept = default;

  void swap(stop_token & other) noexcept {
    stop_state * tmp = state;
    state = other.state;
    other.state = tmp;
  }

  NO_DISCARD bool stop_requested() const noexcept { return state ? state->should_stop.load() : false; }

  NO_DISCARD bool stop_possible() const noexcept { return state; }

private:

  friend class stop_source;
  stop_token(stop_state * state_) : state(state_) {}
  stop_state * state;
};

} // namespace messaging

#endif /* INCLUDE__MESSAGING__STOP_TOKEN_HPP */
