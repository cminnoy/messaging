/**
 * @file logger.hpp
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

#include <fstream>
#include "atm_messages.hpp"

template <typename Process>
struct logger final : Process {
  using self = logger;
  using receiver_type = typename Process::receiver_type;
  using sender_type = typename Process::sender_type;
  using dispatcher_type = typename Process::dispatcher_type;

  logger() : log("log.txt") {
    /**
     * @note This is the prefered way of registering handlers.
     *       It is cleaner and more performant.
     *       If you need to handle states then see implementation of atm machine.
	 */
    (*this)
      .handle([&](std::string const & msg) { log << msg << std::endl; })
      .handle([&](verify_pin const & msg) { log << "verifing pin of account " << msg.account << std::endl; })
      .handle([&](pin_verified const &) { log << "pin verified" << std::endl; })
      .handle([&](pin_incorrect const &) { log << "pin incorrect" << std::endl; })
      .handle([&](withdraw const & msg) { log << "request withdraw " << msg.amount << std::endl; })
      .handle([&](withdraw_ok const &) { log << "withdraw accepted" << std::endl; })
      .handle([&](withdraw_denied const &) { log << "withdraw denied" << std::endl; });
  }

  ~logger() { log.close(); }

private:

  std::ofstream log;
};
