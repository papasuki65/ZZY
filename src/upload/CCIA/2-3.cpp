#include <iostream>
#include <thread>

using namespace std;

class thread_guard{
        std::thread &t;
    public:
        explicit thread_guard(std::thread &t_):t(t_){}
        ~thread_guard()
        {
            if(t.joinable())//判断thread是否已经被detach/join
            {
                t.join();//阻塞
            }
        }
        thread_guard(thread_guard const&)=delete;
        thread_guard& operator=(thread_guard const&)=delete;
};

void do_something(int& i)
{
    cout << i << endl;
}
struct func{
    int& i;
    func(int& i_):i(i_){}
    void operator()(){
        for(unsigned j = 0;j < 1000000;++j)
        {
            do_something(i);
        }
    }
};

void oops()
{
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);
    thread_guard g(t);
}

int main()
{
    oops();
    return 0;
}