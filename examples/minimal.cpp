/*
* cpp-terminal
* C++ library for writing multiplatform terminal applications.
*
* SPDX-FileCopyrightText: 2019-2023 Ondřej Čertík
*
* SPDX-License-Identifier: MIT
*/

#include "cpp-terminal/terminal.hpp"

#include <iostream>

int main()
{
  std::cout << "Just including terminal.hpp activate \033[31mcolor\033[0m !" << std::endl;
  int i{0};
  std::cin >> i;
  return 0;
}
