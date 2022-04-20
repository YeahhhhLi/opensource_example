#include <iostream>

#include "concurrentqueue.h"

int main() 
{
    moodycamel::ConcurrentQueue<int> q;
    q.enqueue(25);

    int item = 0;
    bool found = q.try_dequeue(item);
    std::cout << "deque_res[" << found << "] value[" << item << "]" << std::endl;
    return 0;    
}