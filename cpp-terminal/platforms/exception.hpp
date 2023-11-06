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
#include <string>

namespace Term
{

namespace Private
{

#if defined(_WIN32)
class WindowsError
{
public:
  WindowsError(const WindowsError&)                     = default;
  WindowsError(WindowsError&&)                          = default;
  WindowsError() noexcept                               = default;
  virtual ~WindowsError() noexcept                      = default;
  WindowsError& operator=(WindowsError&&) noexcept      = default;
  WindowsError& operator=(const WindowsError&) noexcept = default;
  std::int64_t  error() const noexcept;
  bool          check_value() const noexcept;
  WindowsError& check_if(const bool& ret) noexcept;
  void          throw_exception(const std::string& str = std::string()) const;

private:
  std::int64_t m_error{0};
  bool         m_check_value{false};
};

class WindowsException : public Term::Exception
{
public:
  WindowsException(const std::int64_t& error, const std::string& context = std::string());
  ~WindowsException() override = default;

private:
  void build_what() const noexcept final;
};
#endif

class Errno
{
public:
  Errno(const Errno&) noexcept = default;
  Errno(Errno&&) noexcept      = default;
  Errno() noexcept;
  virtual ~Errno() noexcept;
  Errno&       operator=(Errno&&) noexcept      = default;
  Errno&       operator=(const Errno&) noexcept = default;
  std::int64_t error() const noexcept;
  bool         check_value() const noexcept;
  Errno&       check_if(const bool& ret) noexcept;
  void         throw_exception(const std::string& str = std::string()) const;

private:
  std::int64_t m_errno{0};
  bool         m_check_value{false};
};

class ErrnoException : public Term::Exception
{
public:
  ErrnoException(const ErrnoException&) = default;
  ErrnoException(ErrnoException&&)      = default;
  explicit ErrnoException(const std::int64_t& error, const std::string& context = std::string());
  ~ErrnoException() override                                = default;
  ErrnoException& operator=(ErrnoException&&) noexcept      = default;
  ErrnoException& operator=(const ErrnoException&) noexcept = default;

private:
  void build_what() const noexcept final;
};

}  // namespace Private

}  // namespace Term
