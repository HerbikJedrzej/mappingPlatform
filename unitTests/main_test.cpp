#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>
#include <logSystem.hh>

#define TESTS 0

int main(int argc, char **argv){
  srand( time( NULL ) );
  testing::InitGoogleTest(&argc, argv);
	Log log(Log::LogLevel::Debug, ".", "unitTest.log");
  return RUN_ALL_TESTS();
}
