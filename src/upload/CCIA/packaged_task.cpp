#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <chrono>
#include <future>

using namespace std;
future<int> result;
//普通函数
int Add(int x, int y)
{
    return x + y;
}


// void task_lambda()
// {
//     //包装可调用目标时lambda
//     packaged_task<int(int,int)> task([](int a, int b){ return a + b;});
    
//     //仿函数形式，启动任务
//     task(2, 10);
    
//     //获取共享状态中的值,直到ready才能返回结果或者异常
//     future<int> result = task.get_future();
//     cout << "task_lambda :" << result.get() << "\n";
// }

void task_thread()
{
    //包装普通函数
    std::packaged_task<int (int,int)> task(Add);
    result = task.get_future();
    //启动任务，非异步
    // task(4,8);
    // cout << "task_thread :" << result.get() << "\n";
        
    // //重置共享状态
    // task.reset();
    // result = task.get_future();

    //通过线程启动任务，异步启动
    thread td(move(task), 2, 100);
    td.join();
    //获取执行结果
    cout << "task_thread :" << result.get() << "\n";
    // cout << "task_thread :" << result.get() << "\n";

    std::future<int> answer_async = std::async(Add, 15, 8);
    cout << "answer_async :" << answer_async.get() << "\n";
}

int main(int argc, char *argv[])
{
    // task_lambda();
    task_thread();
    // cout << "main :" << result.get() << "\n";
    return 0;
}