#pragma once

#include <string>

namespace md
{
#if defined(_WIN32) || defined(_WIN64)
// Windows is always little endian
constexpr const bool is_little_endian = true;
#elif defined(__APPLE__)
#include <machine/endian.h>
#if __DARWIN_BYTE_ORDER == __DARWIN_LITTLE_ENDIAN
constexpr const bool is_little_endian = true;
#else
constexpr const bool is_little_endian = false;
#endif
#elif defined(__linux__)
#include <endian.h>
#if __BYTE_ORDER == __LITTLE_ENDIAN
constexpr const bool is_little_endian = true;
#else
constexpr const bool is_little_endian = false;
#endif
#else
#error "Unknown platform"
#endif

}
