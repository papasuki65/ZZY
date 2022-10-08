#include <memory>
#include <mutex>
#include <stack>
#include <iostream>
class some_big_object
{
public:
    some_big_object(){std::cout << this <<"\t construct\n";}
    ~some_big_object(){std::cout << this << "\t dis construct\n";}
    some_big_object(some_big_object const& o){std::cout << this << "\t copy construct\n";}
    some_big_object(some_big_object && o){std::cout << this << "\t move construct\n";}
};
class exclude
{
public:
    exclude(){std::cout << this <<"\t construct\n";}
    ~exclude(){std::cout << this << "\t dis construct\n";}
    exclude(exclude const& o){std::cout << this << "\t copy construct\n";}
    exclude(exclude && o){std::cout << this << "\t move construct\n";}
};

void swap(some_big_object& lhs, some_big_object& rhs)
{
    std::cout << "------swap------\n";
}

class X
{
private:
    some_big_object some_detail;
    std::mutex m;
public:
    X(some_big_object const& sd) : some_detail(sd){}
    friend void swap_x(X& lhs, X& rhs);
};
void swap_x(X& lhs, X& rhs)
{
    if(&lhs == &rhs)
        return ;
    //一次锁住两个或者两个以上互斥量。如果互斥量中有一个没锁住，他就在那里等着，等着所有的互斥量都锁住，他才往下走。
    //要么两个互斥量都锁住，要么两个互斥量都没锁住。如果只锁了一个，另外一个没锁成功，则它立即把已经锁住的解锁。
    std::lock(lhs.m, rhs.m);
    std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
    std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
    // std::scoped_lock guard(lhs.m, rhs.m);//--std=c++17 新特性：类模板参数推导
    swap(lhs.some_detail, rhs.some_detail);
}
std::mutex some_mutex;
std::unique_lock<std::mutex> get_lock()
{
    extern std::mutex some_mutex;
    std::unique_lock<std::mutex> lk(some_mutex);
    std::cout << "prepare_data()" << std::endl;
    return lk;
}

void process_data()
{
    std::unique_lock<std::mutex> lk(get_lock());
    std::cout << "do_something()" << std::endl;
}

int main()
{
    process_data();
    return 0;
    some_big_object s;
    X x1(s), x2(s);
    swap_x(x1, x2);
    return 0;
}