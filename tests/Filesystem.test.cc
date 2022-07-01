/** \file Filesystem.test.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/
#include "Filesystem.h"

#include <doctest/doctest.h>
#include <iostream>
#include <string>

TEST_CASE("Files")
{
  std::string file1 = {"c:/toto/titi/yoyo.txt"};
  CHECK_EQ(path(file1), "c:/toto/titi");
  CHECK_EQ(filename(file1), "yoyo");
  CHECK_EQ(extension(file1), "txt");
  std::string file2 = {"~/toto/yoyo.txt"};
  CHECK_EQ(path(file2), "~/toto");
  CHECK_EQ(filename(file2), "yoyo");
  CHECK_EQ(extension(file2), "txt");
  std::string file3 = {"./yoyo.txt"};
  CHECK_EQ(path(file3), ".");
  CHECK_EQ(filename(file3), "yoyo");
  CHECK_EQ(extension(file3), "txt");
  std::string file4 = {"./yoyo.toto.txt"};
  CHECK_EQ(path(file4), ".");
  CHECK_EQ(filename(file4), "yoyo.toto");
  CHECK_EQ(extension(file4), "txt");
  std::string file5 = {"./yoyo"};
  CHECK_EQ(path(file5), ".");
  CHECK_EQ(filename(file5), "yoyo");
  CHECK_EQ(extension(file5), "");
}
