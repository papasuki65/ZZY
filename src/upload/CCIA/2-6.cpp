#include <iostream>
#include <thread>

using namespace std;

class scoped_thread{
        std::thread t;
    public:
        explicit scoped_thread(std::thread t_):t(move(t_)) {
            if(!t.joinable()) {
                throw std::logic_error("No thread");
            }
        }
        ~scoped_thread()
        {
            t.join();//阻塞
        }
        scoped_thread(scoped_thread const&)=delete;
        scoped_thread& operator=(scoped_thread const&)=delete;
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
    scoped_thread t{std::thread{func(some_local_state)}};
}

int main()
{
    oops();
    return 0;
}