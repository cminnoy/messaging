/**
 * @file all.hpp
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

#include <messaging/stop_source.hpp>
#include <messaging/stop_token.hpp>
#include <messaging/close_queue.hpp>

#include <messaging/naming.hpp>
#include <messaging/queue_base.hpp>
#include <messaging/sender.hpp>
#include <messaging/async.hpp>

#include <messaging/dispatcher.hpp>
#include <messaging/template_dispatcher.hpp>

#include <messaging/receiver.hpp>
#include <messaging/pump.hpp>
#include <messaging/process.hpp>
#include <messaging/broadcasting_process.hpp>
#include <messaging/process_manager.hpp>

#include <messaging/coroutine/message_queue.hpp>
#include <messaging/coroutine/types.hpp>
