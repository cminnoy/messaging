/**
 * @file util__allocator.cpp
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

#include <util/attributes.hpp>

#if __has_include(<memory_resource>)

#if __cplusplus >= CPP17_STANDARD

#include <util/allocator.hpp>
#include <cstring>
#include <string>
#include <atomic>

#ifdef WITH_TRACY
#include <Tracy.hpp>
#endif

namespace {

MAYBE_UNUSED std::atomic<int> instance_count;

} // namespace

#ifdef WITH_TRACY
util::memory_resource_tracy::memory_resource_tracy(std::pmr::memory_resource * alloc, char const * const name) noexcept : allocator(alloc) {
  //! @note Tracy requires that literals remain present during all times.
  //!       Therefor this copy is leaky intentionaly.
  std::string s("pool");
  s += '<';
  s += std::to_string(instance_count++);
  s += ">::";
  s += name;
  this->name = (char *)operator new(s.size() + 1);
  std::strcpy(this->name, s.c_str());
}

void * util::memory_resource_tracy::do_allocate(std::size_t bytes, std::size_t alignment) {
  auto ptr = allocator->allocate(bytes, alignment);
  TracyAllocN(ptr, bytes, name);
  return ptr;
}

void util::memory_resource_tracy::do_deallocate(void * p, std::size_t bytes, std::size_t alignment) {
  TracyFreeN(p, name);
  return allocator->deallocate(p, bytes, alignment);
}
#endif

void * util::memory_resource_counter::do_allocate(std::size_t bytes, std::size_t alignment) {
  ++m_allocations;
  m_allocated_memory += bytes;
  return allocator->allocate(bytes, alignment);
}

void util::memory_resource_counter::do_deallocate(void * p, std::size_t bytes, std::size_t alignment) {
  ++m_deallocations;
  m_allocated_memory -= bytes;
  return allocator->deallocate(p, bytes, alignment);
}

std::ostream & util::operator<<(std::ostream & out, const memory_resource_counter & o) {
  out << ANSI_TOK "{" ANSI_LBL "\"allocations\"" ANSI_TOK ":" ANSI_NRM << o.m_allocations
      << ANSI_TOK "," ANSI_LBL "\"deallocations\"" ANSI_TOK ":" ANSI_NRM << o.m_deallocations
      << ANSI_TOK "," ANSI_LBL "\"memory_allocated\"" ANSI_TOK ":" ANSI_NRM << o.m_allocated_memory << ANSI_TOK "}" ANSI_NRM;
  return out;
}

#endif // C++17

#endif // memory resource
