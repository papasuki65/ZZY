#include <iostream>
#include <thread>
#include <unistd.h>
#include <queue>
#include <condition_variable>

using namespace std;

std::mutex mut;
std::queue<int> data_queue;
std::condition_variable data_cond;

void data_preparation_thread()
{
    while(true)
    {
        int const data = random() % 10000;
        {
            std::lock_guard<std::mutex> lk(mut);
            data_queue.push(data);
            cout << "put data_queue tail:\t" << data << endl;
        }
        data_cond.notify_one();
    }
}

void data_processing_thread()
{
    while(true)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[]{return !data_queue.empty();});
        int data = data_queue.front();
        data_queue.pop();
        lk.unlock();
        cout << "get data_queue front:\t" << data << endl;
    }   
}

int main()
{
    std::thread th1(data_preparation_thread);
    std::thread th2(data_processing_thread);
    th1.join();
    th2.join();
    return 0;
}