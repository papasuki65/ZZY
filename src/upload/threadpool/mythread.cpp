#include <iostream>
#include <chrono>
#include <atomic>
#include <memory>
#include <thread>
#include <list>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <future>
using namespace std;

class ThreadPool {
public:
    //线程的状态
    enum class ThreadState {
        kInit = 0,
        kWaiting = 1,
        kRunning = 2,
        kStop = 3
    };

    //线程的种类标识
    enum class ThreadFlag{
        kInit = 0,
        kCore = 1,
        kCache = 2
    };

    struct ThreadPoolConfig //线程池配置
    {
        int core_threads;//核心线程数
        int max_threads;//最大线程数
        int max_task_size;//最多任务数
        std::chrono::seconds time_out;//超时时间
    };

    //保存线程相关配置
    struct ThreadWrapper {
        std::shared_ptr<std::thread> ptr;//thread对象
        std::atomic<int> id;//线程id
        std::atomic<ThreadFlag> flag;//线程状态
        std::atomic<ThreadState> state;//线程标识

        ThreadWrapper() {
            ptr = nullptr;
            id = 0;
            state.store(ThreadState::kInit);
        }
    };


    ThreadPool(ThreadPoolConfig config) : _config(config) {
        this->total_function_num_.store(0);
        this->waiting_thread_num_.store(0);

        this->thread_id_.store(0);
        this->is_shutdown_.store(false);
        this->is_shutdown_now_.store(false);

        if(IsvaildConfig(_config)) {
            is_available_.store(true);
        }
        else {
            is_available_.store(false);
        }
    }
    
    ~ThreadPool() {ShutDown();}

    bool IsAvailable() {return is_available_.load();}

    bool Start()
    {
        if(!IsAvailable())
        {
            return false;
        }

        int core_thread_num = _config.core_threads;
        cout << "Init thread num " << core_thread_num << endl;
        while(core_thread_num-- > 0)
        {
            AddThread(this->thread_id_++);
        }
        cout << "Init thread end" << endl;
        return true;
    }

    // 关掉线程池，内部还没有执行的任务会继续执行
    void ShutDown() {
        ShutDown(false);
        cout << "shutdown" << endl;
    }

    // 执行关掉线程池，内部还没有执行的任务直接取消，不会再执行
    void ShutDownNow() {
        ShutDown(true);
        cout << "shutdown now" << endl;
    }
    
    bool IsvaildConfig(ThreadPoolConfig config) {
        if(config.core_threads < 1 ||config.max_threads < config.core_threads || config.time_out.count() < 1) {
            return false;
        }
        return true;
    }

    void AddThread(int id) {AddThread(id, ThreadFlag::kCore);}
    void AddThread(int id, ThreadFlag thread_flag) {
        cout << "AddThread " << id << " flag " << static_cast<int>(thread_flag) << endl;
        std::shared_ptr<ThreadWrapper> thread_ptr = std::make_shared<ThreadWrapper>();
        thread_ptr->id.store(id);
        thread_ptr->flag.store(thread_flag);
        auto func = [this, thread_ptr]() {
            for(;;) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->task_mutex_);
                    if(thread_ptr->state.load() == ThreadState::kStop) {
                        break;
                    }

                    thread_ptr->state.store(ThreadState::kWaiting);
                    ++this->waiting_thread_num_;
                    bool is_timeout = false;
                    if(thread_ptr->flag.load() == ThreadFlag::kCore) {
                        this->task_cv_.wait(lock, [this, thread_ptr] {
                            return (this->is_shutdown_ || this->is_shutdown_now_ || !this->tasks_.empty() ||
                                thread_ptr->state.load() == ThreadState::kStop);
                        });
                    }
                    else {
                        this->task_cv_.wait_for(lock, this->_config.time_out, [this, thread_ptr] {
                            return (this->is_shutdown_ || this->is_shutdown_now_ || !this->tasks_.empty() ||
                                        thread_ptr->state.load() == ThreadState::kStop);
                        });
                    }
                    --this->waiting_thread_num_;

                    if(is_timeout) {
                        thread_ptr->state.store(ThreadState::kStop);
                    }

                    if (thread_ptr->state.load() == ThreadState::kStop) {
                        break;
                    }

                    if(this->is_shutdown_ && this->tasks_.empty()) {
                        break;
                    }

                    if (this->is_shutdown_now_) {
                        break;
                    }

                    thread_ptr->state.store(ThreadState::kRunning);
                    task = std::move(this->tasks_.front()); //应该对task_队列进行保护？
                    this->tasks_.pop();
                }
                task();
            }
            
        };
        thread_ptr->ptr = std::make_shared<std::thread>(std::move(func));
        if(thread_ptr->ptr->joinable()) {
            thread_ptr->ptr->detach();
        }
        this->worker_threads_.emplace_back(move(thread_ptr));
    }

    template <typename F, typename... Args>
    std::shared_ptr<std::future<std::result_of_t<F(Args...)>>> Run(F &&f, Args &&... args) {
        if(this->is_shutdown_.load() || this->is_shutdown_now_.load() || !IsAvailable()) {
            return nullptr;
        }
        if(waiting_thread_num_ == 0 && this->worker_threads_.size() < _config.max_threads) {
            AddThread(this->thread_id_++, ThreadFlag::kCache);
        }

        using return_type = std::result_of_t<F(Args...)>;
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        total_function_num_++;
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(this->task_mutex_);
            this->tasks_.emplace([task](){(*task)();});
        }
        this->task_cv_.notify_one();

        return std::make_shared<std::future<return_type>>(std::move(res));
    }

    
private:
    ThreadPoolConfig _config;

    std::atomic<bool> is_available_;
    std::atomic<bool> is_shutdown_;
    std::atomic<bool> is_shutdown_now_;

    std::atomic<int> total_function_num_; //线程池已经执行过的函数个数
    std::atomic<int> waiting_thread_num_; //空闲线程
    std::atomic<int> thread_id_;

    std::list<std::shared_ptr<ThreadWrapper>> worker_threads_;

    std::mutex task_mutex_;

    std::condition_variable task_cv_;

    std::queue<std::function<void()>> tasks_;

    void ShutDown(bool is_now) {
        if (is_available_.load()) {
            if (is_now) {
                this->is_shutdown_now_.store(true);
            } else {
                this->is_shutdown_.store(true);
            }
            this->task_cv_.notify_all();
            is_available_.store(false);
        }
    }
};

int main() {
    cout << "hello" << endl;
    ThreadPool pool{ThreadPool::ThreadPoolConfig{4, 5, 6, std::chrono::seconds(4)}};
    pool.Start();
    std::this_thread::sleep_for(std::chrono::seconds(4));
     auto getSum = [](int a,int b){
        cout << "sum : " << a+b  << endl;
        return a+b;
    };
    auto res = pool.Run(getSum, 1, 5);
    std::this_thread::sleep_for(std::chrono::seconds(4));
    return 0;
}