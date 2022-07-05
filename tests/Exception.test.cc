/** \file Exception.test.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/
#include "Exception.h"

#include <doctest/doctest.h>
#include <iostream>
#include <string>

TEST_CASE("Exception(5,\"MyException\")")
{
  Exception exception(5, "MyException");
  CHECK_EQ(exception.error(), 5);
  CHECK_EQ(exception.message(), "MyException");
  std::string what = exception.what();
  CHECK_EQ(what, "Error 5 : MyException");
}

TEST_CASE("Exception(\"MyException\")")
{
  Exception exception("MyException");
  CHECK_EQ(exception.error(), 0);
  CHECK_EQ(exception.message(), "MyException");
  std::string what = exception.what();
  CHECK_EQ(what, "MyException");
}
