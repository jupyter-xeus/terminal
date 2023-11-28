/*
* cpp-terminal
* C++ library for writing multi-platform terminal applications.
*
* SPDX-FileCopyrightText: 2019-2023 cpp-terminal
*
* SPDX-License-Identifier: MIT
*/

#include "cpp-terminal/private/file_initializer.hpp"

#include "cpp-terminal/private/exception.hpp"
#include "cpp-terminal/private/file.hpp"

#if defined(_WIN32)
  #include "cpp-terminal/private/unicode.hpp"

  #include <io.h>
  #include <windows.h>
  #if defined(MessageBox)
    #undef MessageBox
  #endif
#else
  #include <sys/stat.h>
#endif

bool Term::Private::FileInitializer::m_consoleCreated = {false};

std::size_t Term::Private::FileInitializer::m_counter = {0};

void Term::Private::FileInitializer::attachConsole() noexcept
try
{
#if defined(_WIN32)
  // If something happen here we still don't have a console so we can only use a MessageBox to warn the users something is very bad and that they should contact us.
  Term::Private::WindowsError error{Term::Private::WindowsError().check_if(AttachConsole(ATTACH_PARENT_PROCESS) == 0)};
  bool                        need_allocation{false};
  switch(error.error())
  {
    case ERROR_SUCCESS: break;                                                                             // Has been attached
    case ERROR_ACCESS_DENIED: need_allocation = false; break;                                              // Already attached that's good !
    case ERROR_INVALID_PARAMETER: error.throw_exception("The specified process does not exist !"); break;  // Should never happen !
    case ERROR_INVALID_HANDLE: need_allocation = true; break;                                              // Need to allocate th console !
  }
  if(need_allocation)
  {
    Term::Private::WindowsError().check_if(AllocConsole() == 0).throw_exception("AllocConsole()");
    m_consoleCreated = true;
  }
#endif
}
catch(...)
{
  detachConsole();
  ExceptionHandler(ExceptionDestination::MessageBox);
}

void Term::Private::FileInitializer::detachConsole() noexcept
try
{
#if defined(_WIN32)
  if(m_consoleCreated) {Term::Private::WindowsError().check_if(0 == FreeConsole()).throw_exception("FreeConsole()");}
#endif
}
catch(...)
{
  ExceptionHandler(ExceptionDestination::MessageBox);
}

Term::Private::FileInitializer::FileInitializer() noexcept
try
{
  // MacOS was not happy wish a static mutex in the class so we create it and pass to each class;
  static std::recursive_mutex ioMutex;
  if(0 == m_counter)
  {
    attachConsole();
    openStandardStreams();
    if(new(&Term::Private::in) InputFileHandler(ioMutex) == nullptr) { throw Term::Exception("new(&Term::Private::in) InputFileHandler(ioMutex)"); }
    if(new(&Term::Private::out) OutputFileHandler(ioMutex) == nullptr) { throw Term::Exception("new(&Term::Private::out) OutputFileHandler(ioMutex)"); }
  }
  ++m_counter;
}
catch(...)
{
  ExceptionHandler(ExceptionDestination::StdErr);
}

Term::Private::FileInitializer::~FileInitializer() noexcept
try
{
  if(0 == m_counter)
  {
    (&Term::Private::in)->~InputFileHandler();    //NOSONAR(S3432)
    (&Term::Private::out)->~OutputFileHandler();  //NOSONAR(S3432)
    detachConsole();
  }
  --m_counter;
}
catch(...)
{
  ExceptionHandler(ExceptionDestination::StdErr);
}

void Term::Private::FileInitializer::openStandardStreams()
{
#if defined(_WIN32)
  FILE* fDummy{nullptr};
  if(_fileno(stdout) < 0 || _get_osfhandle(_fileno(stdout)) < 0) {Term::Private::WindowsError().check_if(_wfreopen_s(&fDummy, L"CONOUT$", L"w", stdout) != 0).throw_exception(R"(_wfreopen_s(&fDummy, L"CONOUT$", L"w", stdout))");}
  if(_fileno(stderr) < 0 || _get_osfhandle(_fileno(stderr)) < 0) {Term::Private::WindowsError().check_if(_wfreopen_s(&fDummy, L"CONOUT$", L"w", stderr) != 0).throw_exception(R"(_wfreopen_s(&fDummy, L"CONOUT$", L"w", stderr))");}
  if(_fileno(stdin) < 0 || _get_osfhandle(_fileno(stdin)) < 0) {Term::Private::WindowsError().check_if(_wfreopen_s(&fDummy, L"CONIN$", L"r", stdin) != 0).throw_exception(R"(_wfreopen_s(&fDummy, L"CONIN$", L"r", stdin))");}
  const std::size_t bestSize{BUFSIZ > 4096 ? BUFSIZ : 4096};
#else
  const struct stat stats
  {
  };
  const std::size_t bestSize{static_cast<size_t>(stats.st_blksize)};
#endif
  Term::Private::Errno().check_if(std::setvbuf(stdin, nullptr, _IOLBF, bestSize) != 0).throw_exception("std::setvbuf(stdin, nullptr, _IOLBF, bestSize)");
  Term::Private::Errno().check_if(std::setvbuf(stdout, nullptr, _IOLBF, bestSize) != 0).throw_exception("std::setvbuf(stdout, nullptr, _IOLBF, bestSize)");
  Term::Private::Errno().check_if(std::setvbuf(stderr, nullptr, _IOLBF, bestSize) != 0).throw_exception("std::setvbuf(stderr, nullptr, _IOLBF, bestSize)");
}
