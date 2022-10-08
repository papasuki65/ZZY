#include <iostream>
#include <mutex>    //unique_lock
#include <shared_mutex> //shared_mutex shared_lock
#include <thread>
#include<atomic>
 
std::mutex cout_mutex;
std::atomic<int> a_i;
class ThreadSaferCounter
{
private:
    mutable std::shared_mutex mutex_;
    unsigned int value_ = 0;
public:
    ThreadSaferCounter(/* args */) {};
    ~ThreadSaferCounter() {};
    
    unsigned int get() const {
        //读者, 获取共享锁, 使用shared_lock
        std::shared_lock<std::shared_mutex> lck(mutex_);//执行mutex_.lock_shared();
        return value_;  //lck 析构, 执行mutex_.unlock_shared();
    }

    unsigned int increment() {
        //写者, 获取独占锁, 使用unique_lock
        std::unique_lock<std::shared_mutex> lck(mutex_);//执行mutex_.lock();
        value_++;   //lck 析构, 执行mutex_.unlock();
        return value_;
    }

    void reset() {
        //写者, 获取独占锁, 使用unique_lock
        std::unique_lock<std::shared_mutex> lck(mutex_);//执行mutex_.lock();
        value_ = 0;   //lck 析构, 执行mutex_.unlock();
    }
};

ThreadSaferCounter g_counter;

void reader(int id){
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> ulck(cout_mutex);//cout也需要锁去保护, 否则输出乱序
        std::cout << "reader \t#" << id << "   \ttimes: " << a_i++ << " \tread  value " << g_counter.get() << "\n";
    }    
}

void writer(int id){
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> ulck(cout_mutex);//cout也需要锁去保护, 否则输出乱序
        std::cout << "writer \t#" << id <<  "  \ttimes: " << a_i++ << " \twrite value " << g_counter.increment() << "\n";
    }
}
 
 int main()
 {
    a_i = 0;
    std::thread rth[10];
    std::thread wth[10];
    for(int i=0; i<10; i++){
        rth[i] = std::thread(reader, i+1);
    }
    for(int i=0; i<10; i++){
        wth[i] = std::thread(writer, i+1);
    }

    for(int i=0; i<10; i++){
        rth[i].join();
    }
    for(int i=0; i<10; i++){
        wth[i].join();
    }
    return 0;
 }