#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "lib/hello-time.h"
#include "main/hello-greet.h"

#include "concurrentqueue.h"

int main(int argc, char** argv) {

  std::string who = "world";
  if (argc > 1) {
    who = argv[1];
  }
  std::cout << get_greet(who) << std::endl;
  print_localtime();

  moodycamel::ConcurrentQueue<int> q;
    q.enqueue(25);

    int item = 0;
    bool found = q.try_dequeue(item);
    std::cout << "deque_res[" << found << "] value[" << item << "]" << std::endl;
  return EXIT_SUCCESS;
}
