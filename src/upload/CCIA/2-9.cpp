#include <iostream>
#include <thread>
#include <numeric>
#include <vector>
#include <string>
using namespace std;

template <typename Iterator, typename T>
struct accumulate_block
{
    void operator ()(Iterator first, Iterator last, T& result)
    {
        result = std::accumulate(first, last, result);
    }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last);
    if(!length)
        return init;
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread; //向上取整
    unsigned long const hardware_threads = std::thread::hardware_concurrency(); //获取硬件支持真正可并发的线程数量
    unsigned long const num_threads = 4;//std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads); //确定并行的线程数量
    unsigned long const block_size = length / num_threads;//每个线程处理的块任务大小
    std::vector<T> results(num_threads);//存放结果集
    std::vector<std::thread> threads(num_threads - 1);//线程池
    Iterator block_start = first;
    for(unsigned long i = 0; i < (num_threads - 1); ++i)    //处理前 n - 1个块
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));
        // threads[i] = std::thread(std::accumulate<Iterator, T>, block_start, block_end, std::ref(results[i]));//accumulate有连两个版本，显式模板参指定调用哪个版本
        cout << "thread id: " << threads[i].get_id() <<endl;
        block_start = block_end;
    }
    accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);   //处理最后一个块

    for(auto& entry: threads)
        entry.join();
    return std::accumulate(results.begin(), results.end(), init);
}
    int main()
    {
        cout << "main thread id: " << std::this_thread::get_id() <<endl;
        std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int sum = parallel_accumulate(v.begin(), v.end(), 0);//T accumulate( InputIt first, InputIt last, T init ); init + first + .. + last
        std::cout << "sum: " <<sum << endl;

        auto dash_fold = [](string a, int b) {
            return std::move(a) + '-' + std::to_string(b);
        };
        string rs = std::accumulate(v.begin() + 1, v.end(), std::to_string(v[0]), dash_fold);//T accumulate( InputIt first, InputIt last, T init, BinaryOperation op ); op(op(init, first), first + 1)  ... last
        cout << "rs: " << rs << endl;
        return 0;
    }
