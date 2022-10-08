#include "header.h"

std::mutex m;
std::deque<std::packaged_task<void()>> tasks;
// void get_and_process_gui_message();
void gui_thread()
{
    while(true)
    {
        // get_and_process_gui_message();
        std::packaged_task<void()> task;
        {
            std::lock_guard<std::mutex> lk(m);
            if(tasks.empty())
                continue;
            task = std::move(tasks.front());
            tasks.pop_front();
        }
        task();
    }
}


template<typename Func>
std::future<void> post_task_for_gui_thread(Func f)
{
    std::packaged_task<void()> task(f);
    std::future<void> res = task.get_future();
    std::lock_guard<std::mutex> lk(m);
    tasks.push_back(std::move(task));
    return res;
}
void show()
{
    cout << "show()  helloworld" << endl;
}


int main()
{
    std::thread gui_bg_thread(gui_thread);
    std::future<void> res = post_task_for_gui_thread(show);
    res.get();
    gui_bg_thread.join();
    return 0;
}