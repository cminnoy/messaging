/**
 * @file message_queue.hpp
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

#ifndef INCLUDE__MESSAGING__COROUTINE__MESSAGE_QUEUE_HPP
#define INCLUDE__MESSAGING__COROUTINE__MESSAGE_QUEUE_HPP

#include <util/attributes.hpp>

static_assert(__cplusplus >= CPP14_STANDARD, "Minimum required standard is C++14.");

#include <mutex>
#include <memory>
#include <type_traits>

#include <messaging/queue_base.hpp>

namespace messaging {

namespace coroutine {

class queue final : public queue_base {
public:

  using queue_base::queue_base;

  template <typename YieldType>
  NO_DISCARD pointer_type wait_and_pop(YieldType && yield) {
    pointer_type res;
    iswaiting.store(true);
    while (not q.pop(res)) yield();
    iswaiting.store(false);
    return res;
  }
};

} // namespace coroutine

} // namespace messaging

#endif /* INCLUDE__MESSAGING__COROUTINE__MESSAGE_QUEUE_HPP */
