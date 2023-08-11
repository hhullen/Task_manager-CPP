#include <gtest/gtest.h>

#include "../task_manager.h"

using hhullen::TaskManager;

class TESTER {
 public:
  void method(int var) {
    for (int i = 0; i < var + 1000; ++i) {
    }
  }
};

TEST(Task_manager_tests, constructor) {
  EXPECT_NO_THROW(TaskManager object);
  EXPECT_NO_THROW(TaskManager object(3));
  EXPECT_NO_THROW(TaskManager object(12));
}

TEST(Task_manager_tests, set_threads) {
  TaskManager object(3);
  EXPECT_NO_THROW(object.SetThreads(8));
}

TEST(Task_manager_tests, add_task) {
  TaskManager object(8);
  TESTER test_obj;
  EXPECT_NO_THROW(object.AddTask(&TESTER::method, test_obj, 20));
  EXPECT_NO_THROW(object.AddTask(&TESTER::method, test_obj, 21));
  object.AddTask(&TESTER::method, test_obj, 22);
  object.AddTask(&TESTER::method, test_obj, 245);
  object.AddTask(&TESTER::method, test_obj, 25);
  object.AddTask(&TESTER::method, test_obj, 100);
  EXPECT_NO_THROW(object.AwaitAll());
  object.AddTask(&TESTER::method, test_obj, 22);
  object.AddTask(&TESTER::method, test_obj, 245);
  object.AddTask(&TESTER::method, test_obj, 25);
  object.AddTask(&TESTER::method, test_obj, 100);
  for (int i = 0; i < 10000; ++i) {
    object.AddTask(&TESTER::method, test_obj, i);
  }
  EXPECT_NO_THROW(object.AwaitAll());
}

TEST(Task_manager_tests, operator_eq) {
  TaskManager object(2);
  TaskManager object_02(3);
  TESTER test_obj;
  EXPECT_NO_THROW(object.AddTask(&TESTER::method, test_obj, 20));
  EXPECT_NO_THROW(object.AddTask(&TESTER::method, test_obj, 21));
  object.AddTask(&TESTER::method, test_obj, 22);
  object.AddTask(&TESTER::method, test_obj, 245);
  object.AddTask(&TESTER::method, test_obj, 25);
  EXPECT_NO_THROW(object_02 = object);
  EXPECT_NO_THROW(object = object_02);
}
