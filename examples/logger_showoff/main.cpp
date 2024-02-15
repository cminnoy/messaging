/**
 * @file main.cpp
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

#include <util/logger.hpp>

#ifdef WITH_SPDLOG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
# include <util/logger_spdlog.hpp>
# include <spdlog/sinks/stdout_color_sinks.h>
# include <spdlog/sinks/systemd_sink.h>
#endif

//=============================================================================

static void init_debug_streams() __attribute__((constructor));
static void init_debug_streams() {
  using namespace util::logger;

  // Set stream options
  for (auto && stream : std::vector<costream *> {&info, &debug, &trace, &warning, &error, &stats}) {
    stream->enable(true);
    stream->show_module(true);
    stream->show_thread(true);
    stream->show_linenumber(true);
    stream->show_time(true);
  }

#if WITH_SPDLOG
  // Set SPDLOG level
  spdlog::set_level(spdlog::level::trace);
  spdlog::set_pattern("[%H:%M:%S %z] [%^thread %t%$] (%l) (%n) %! %v");
#endif
}

#undef TRACER_MODULE_NAME
TRACER_MODULE("RMS", "RMS")
#define TRACER_MODULE_NAME module_name

auto l = []() {
  TRACER_FUNC();
  std::cout << "-----------" << std::endl;
  std::cout << INSIDE_LAMBDA << std::endl;
  std::cout << LAMBDA_OUTER_FUNC_NAME() << std::endl;
  std::cout << "-----------" << std::endl;
};

auto r = LAMBDA_OUTER_FUNC_NAME();

struct Foobar {

  const std::string module_name;

  Foobar() : module_name(util::logger::register_module("Foobar", "FB")) {}

  ~Foobar() { util::logger::unregister_module(module_name); }

  void foo() {
    TRACER_COMPONENT_FUNC(module_name);
    [&] {
      TRACER_COMPONENT_FUNC(module_name);
      std::cout << "inside lambda" << std::endl;
    }();
    TRACER_RETURN();
  }

  void bar() {
    TRACER_COMPONENT_FUNC(module_name);
    TRACER_RETURN();
  }
};

int main() {
  SPDLOG_TRACE("sdf");
  TRACER_FUNC();

  auto console = spdlog::stdout_color_mt("some_unique_name");

  l();
  std::cout << r << std::endl;

  SPDLOG_INFO("warning");
  console->info("some info");

  util::loggable_component component;
  //component.info << "test" << std::endl;

  auto systemd_sink = std::make_shared<spdlog::sinks::systemd_sink_st>();
  auto systemd_logger = std::make_shared<spdlog::logger>("logger_name", systemd_sink);
  systemd_logger->info("SYSTEMD OUTPUT");

  Foobar fb;
  fb.foo();
  fb.bar();

  TRACER_RETURN(EXIT_SUCCESS);
}
