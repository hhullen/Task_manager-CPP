# Task manager C++

Library to manage multithread tasks

# Class definition
```c++
namespace hhullen {

class TaskManager {
 public:
  TaskManager(size_t threads);
  TaskManager(const TaskManager &src);
  TaskManager(TaskManager &&src);
  ~TaskManager();

  template <typename Func, typename... Args>
  void AddTask(Func &&function, Args &&...args);
  void SetThreads(size_t number);
  void AwaitAll();
  void operator=(const TaskManager &src);
    ...
};

}  // namespace hhullen
```

# Installation
This is the one-header library, so `*.h` files all are needed. Try `make lib` command from src directory. It will create `task_manager` directory with necessary files. This is the directory to be copied to your project.

# Usage
- Once the `task_manager` directory copied to project, include header file `task_manager.h`.  
- Create `TaskManager` object for necessary threads amount set as constructor argument, otherwise, with empty constructor argument, the threads will be created as `thread::hardware_concurrency() / 2`. In case failure getting value from `hardware_concurrency()`, the only one thread will be created.
### Example:
```c++
#include "task_manager.h"

using hhullen::TaskManager;

void Foo(int a, double b, char c) {
    /* Some code */
}

class GooClass {
 public:
  void Boo(double a, size_t b) {
    /* Some code */
  }
};

int main() {
    // Create the object
    TaskManager manager(4);

    // add tasks
    manager.AddTask(Foo, 10, 2.718, 'e');
    manager.AddTask(Foo, -359, 3.14, 'J');
    manager.AddTask(Foo, 993, 33.175, 'V');

    // add tasks in case class method usage
    GooClass obj;
    manager.AddTask(&GooClass::Boo, obj, 1.0021, 100077);
    manager.AddTask(&GooClass::Boo, obj, 45.3405, 9877);
    manager.AddTask(&GooClass::Boo, obj, 87.0435, 9000000);

    // await all task completion
    manager.AwaitAll();

    return 0;
}
```

# Make targets
- `make lib` - create `task_manager` directory with necessary files
- `make check` - run cppcheck and linter for whole library code
- `make tests` - run tests
- `make gcov_report` - create tests coverage report in `report` dirctory
- `make clean` - all files could be created by any targets
- `make all` - run targets in order: `clean gcov_report tests check`