#include <exception>
#include <memory>
#include <mutex>
#include <stack>
#include <iostream>

struct empty_stack : std::exception
{
    const char* what() const throw(); //虚函数重写
};

template<typename T>
class threadsafe_stack
{
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
    threadsafe_stack(){}
    threadsafe_stack(const threadsafe_stack& other)//不用初始化列表，保证互斥的锁定会横跨整个复制过程。
    {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }

    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(new_value));
    }

    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
        data.pop();
        return res;
    }

    void pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        value = data.top();
        data.pop();
    }

    bool empty() const{
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};
class A{
public:
    A(){std::cout << this <<"\t construct\n";}
    ~A(){std::cout << this << "\t dis construct\n";}
    A(A const& o){std::cout << this << "\t copy construct\n";}
};

int main()
{
    threadsafe_stack<A> st;
    A a;
    st.push(a);//两次拷贝构造（进入到push的入参拷贝构造一次，move不会有构造发生，之后进入到stack的allocator发生一次拷贝构造
    return 0;
}