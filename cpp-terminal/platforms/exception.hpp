/*
* cpp-terminal
* C++ library for writing multi-platform terminal applications.
*
* SPDX-FileCopyrightText: 2019-2023 cpp-terminal
*
* SPDX-License-Identifier: MIT
*/

#include "cpp-terminal/exception.hpp"

#include <cstdint>

namespace Term
{

namespace Private
{

// Helper for windows errors
#if defined(_WIN32)
class WindowsError : public Term::Exception
{
public:
  WindowsError(const unsigned long& error, const std::string& context = std::string());
  virtual ~WindowsError() = default;

private:
  void build_what() override;
};
#endif

class Errno
{
public:
  Errno();
  ~Errno();
  std::int32_t error() const;
  bool         check_value() const;
  Errno&       check_if(const bool& ret);
  void         throw_exception(const std::string& str = std::string());

private:
  std::int32_t m_errno{0};
  bool         m_check_value{false};
};

class ErrnoException : public Term::Exception
{
public:
  ErrnoException(const std::int32_t& error, const std::string& context = std::string());
  ~ErrnoException() override;

private:
  void build_what() override;
};

}  // namespace Private

}  // namespace Term
