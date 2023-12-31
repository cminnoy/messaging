/**
 * @file logger_spdlog.hpp
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

#include <util/attributes.hpp>

static_assert(__cplusplus >= CPP11_STANDARD, "Minimum required standard is C++11.");

#ifndef INCLUDE__UTIL__LOGGER_SPDLOG_HPP_
#define INCLUDE__UTIL__LOGGER_SPDLOG_HPP_

#include <memory>

#include <spdlog/spdlog.h>

namespace util {

struct logstream {

  logstream() : _logger(spdlog::default_logger()) {}
  logstream(std::shared_ptr<spdlog::logger> logger_) : _logger(logger_) {}

private:

  std::shared_ptr<spdlog::logger> _logger;
};

struct loggable_component {

  loggable_component() = default;

  logstream info;
  logstream debug;
  logstream warning;
  logstream error;
  logstream stats;
};

} // namespace util

#endif /* INCLUDE__UTIL__LOGGER_SPDLOG_HPP_ */
