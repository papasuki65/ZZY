#include <iostream>

using namespace std;

template <class T>
class auto_ptr{
    public:
        explicit auto_ptr(T* p = nullptr):pointer(p){}
        ~auto_ptr(){delete pointer;}

        template<class U>
        auto_ptr(auto_ptr<U>& rhs):pointer(rhs.release()){}
        
        template<class U>
        auto_ptr<T>& operator=(auto_ptr<U>& rhs){
            if(this != &rhs)
            {
                reset(rhs.release());
            }
            return *this;
        }

        T& operator *(){return *pointer;}
        T operator ->(){return pointer;}

        T* get(){
            return pointer;
        }

        //将auto_ptr内数据指针转移出去
        T* release()
        {
            T* tmp = pointer;
            pointer = nullptr;
            return tmp;
        }

        //将auto_ptr内数据指针设置为新的
        void reset(T* ptr)
        {
            if(pointer != nullptr)
            {
                delete pointer;
            }
            pointer = ptr;
        }
    private:
        T* pointer;
};
class test{
    public:
        test(int a):m_num(a){}
        int get(){return m_num;}
    private:
        int m_num;
};
int main()
{
    auto_ptr<int> aa = new int(1);
    auto_ptr<int> a(new int(1));
    auto_ptr<int> b(new int(9));
    cout<< *a.get() << endl;
    auto_ptr<int> c = a;
    cout<< *c.get() << endl;
    a = b;
    cout<< *a.get() << endl;

    return 0;
}