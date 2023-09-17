/**
 * @file template_dispatcher.hpp
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

#ifndef INCLUDE__MESSAGING__TEMPLATE_DISPATCHER_HPP
#define INCLUDE__MESSAGING__TEMPLATE_DISPATCHER_HPP

#include <util/attributes.hpp>

static_assert(__cplusplus >= CPP14_STANDARD, "Minimum required standard is C++14.");

#include <memory>

#if __cplusplus >= CPP20_STANDARD
#include <concepts>
#endif

#include <messaging/dispatcher_base.hpp>
#include <messaging/dispatcher.hpp>

namespace messaging {

template <typename PreviousDispatcher, typename Msg, typename Func, typename QueueType, typename YieldType>
#if __cplusplus >= CPP20_STANDARD
requires std::invocable<Func, Msg> || std::invocable<Func, Msg &>
#endif
class template_dispatcher final : public dispatcher_base {
  QueueType * q;
  PreviousDispatcher * prev;
  Func f;
  YieldType & _yield;
  bool chained;

  template_dispatcher(template_dispatcher const &) = delete;
  template_dispatcher & operator=(template_dispatcher const &) = delete;

  template <typename Dispatcher, typename OtherMsg, typename OtherFunc, typename OtherQueueType, typename OtherYieldType>
#if __cplusplus >= CPP20_STANDARD
  requires std::invocable<OtherFunc, OtherMsg> || std::invocable<OtherFunc, OtherMsg &>
#endif
  friend class template_dispatcher;

  template <typename OtherQueueType, typename OtherYieldType>
  friend class dispatcher;

  void wait_and_dispatch() {
    for (;;) {
      if (dispatch(q->wait_and_pop(_yield))) break;
    }
  }

  NO_DISCARD HOT_FUNC bool dispatch(typename QueueType::pointer_type msg) override {
    if (auto m = util::pmte::get_if<Msg>(msg)) {
      f(*m);
      return true;
    } else {
      return prev->dispatch(std::move(msg));
    }
  }

public:

  FORCE_INLINE template_dispatcher(template_dispatcher && other)
  : q(other.q), prev(other.prev), f(std::forward<Func>(other.f)), _yield(other._yield), chained(other.chained) {
    other.chained = true;
  }

  FORCE_INLINE template_dispatcher(QueueType * q_, PreviousDispatcher * prev_, Func && f_, YieldType & yield_)
  : q(q_), prev(prev_), f(std::forward<Func>(f_)), _yield(yield_), chained(false) {
    prev_->chained = true;
  }

  template <typename OtherMsg, typename OtherFunc>
  FORCE_INLINE template_dispatcher<template_dispatcher, OtherMsg, OtherFunc, QueueType, YieldType> handle(OtherFunc && of) {
    static_assert(!std::is_reference<OtherMsg>::value, "References are not allowed!");
    static_assert(util::is_invocable<OtherFunc, OtherMsg &>::value, "OtherFunc must accept type OtherMsg as its single argument!");
    return template_dispatcher<template_dispatcher, OtherMsg, OtherFunc, QueueType, YieldType>(q, this, std::forward<OtherFunc>(of), _yield);
  }

  template <typename OtherFunc, typename OtherMsg = std::remove_reference_t<typename function_info<std::remove_reference_t<OtherFunc>>::first_arg>>
  FORCE_INLINE template_dispatcher<template_dispatcher, OtherMsg, OtherFunc, QueueType, YieldType> handle(OtherFunc && of) {
    using first_arg = typename function_info<typename std::remove_reference_t<OtherFunc>>::first_arg;
    using return_type = typename function_info<typename std::remove_reference_t<OtherFunc>>::return_type;
    using arg_types = typename function_info<typename std::remove_reference_t<OtherFunc>>::arg_types;
    static_assert(std::tuple_size<arg_types>::value == 1, "Handlers must accept exactly one argument!");
    static_assert(std::is_same<return_type, void>::value, "Return type of handler must be void!");
    using T = std::remove_reference_t<first_arg>;
    handle<T>(std::move(of));
  }

  FORCE_INLINE ~template_dispatcher() noexcept(false) {
    if (!chained) {
      wait_and_dispatch();
    }
  }
};

} // namespace messaging

#endif /* INCLUDE__MESSAGING__TEMPLATE_DISPATCHER_HPP */
