#ifndef OPENMC_ERROR_H
#define OPENMC_ERROR_H

#include <cstring>
#include <sstream>
#include <string>

#include <fmt/format.h>

#include "openmc/capi.h"
#include "openmc/settings.h"

#if defined(__GNUC__) || defined(__clang__)
#define UNREACHABLE() __builtin_unreachable()
#else
#define UNREACHABLE() (void)0
#endif

namespace openmc {

inline void set_errmsg(const char* message)
{
  std::strcpy(openmc_err_msg, message);
}

inline void set_errmsg(const std::string& message)
{
  std::strcpy(openmc_err_msg, message.c_str());
}

inline void set_errmsg(const std::stringstream& message)
{
  std::strcpy(openmc_err_msg, message.str().c_str());
}

[[noreturn]] void fatal_error(const std::string& message, int err = -1);

[[noreturn]] inline void fatal_error(const std::stringstream& message)
{
  fatal_error(message.str());
}

[[noreturn]] inline void fatal_error(const char* message)
{
  fatal_error(std::string {message, std::strlen(message)});
}

void warning(const std::string& message);

inline void warning(const std::stringstream& message)
{
  warning(message.str());
}

void write_message(const std::string& message, int level = 0);

inline void write_message(const std::stringstream& message, int level)
{
  write_message(message.str(), level);
}

template<typename... Params>
void write_message(
  int level, const std::string& message, const Params&... fmt_args)
{
  if (settings::verbosity >= level) {
    write_message(fmt::format(message, fmt_args...));
  }
}

template<typename... Params>
void write_message(const std::string& message, const Params&... fmt_args)
{
  write_message(fmt::format(message, fmt_args...));
}

#ifdef OPENMC_MPI
extern "C" void abort_mpi(int code);
#endif

} // namespace openmc
#endif // OPENMC_ERROR_H
