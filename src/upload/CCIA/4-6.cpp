#include "header.h"

int find_the_answer_to_ltuae()
{
    cout << "find_the_answer_to_ltuae thread id: " << std::this_thread::get_id() <<endl;
    return 10;
}

void do_other_stuff()
{
    cout << "do_other_stuff()..." <<endl;
}

int main()
{
    cout << "main thread id: " << std::this_thread::get_id() <<endl;
    std::future<int> the_answer = std::async(find_the_answer_to_ltuae);
    do_other_stuff();
    std::cout << "The answer is " << the_answer.get() << std::endl;
}