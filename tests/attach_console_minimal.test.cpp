/*
* cpp-terminal
* C++ library for writing multiplatform terminal applications.
*
* SPDX-FileCopyrightText: 2019-2023 Ondřej Čertík
*
* SPDX-License-Identifier: MIT
*/

#include "cpp-terminal/color.hpp"
#include "cpp-terminal/terminal.hpp"

#include <iostream>

#ifdef _WIN32
  #include <windows.h>
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main()
#endif
{
  std::cout << Term::color_fg(Term::Color::Name::Red) << "Hello world !" << color_fg(Term::Color::Name::Default) << std::endl;
  return 0;
}
