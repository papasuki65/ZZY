#include <iostream>
using namespace std;


//traits tag struct
struct input_interator_tag{};
struct output_interator_tag{};
struct forward_interator_tag : public input_interator_tag{};
struct bidirectional_interator_tag : public forward_interator_tag{};
struct random_interator_tag : public bidirectional_interator_tag{};


//work class
template<class T>
struct input_interator{
    typedef input_interator_tag category;
    int year = 3;
};

template<class T>
struct output_interator{
    typedef output_interator_tag category;
    int year = 30;
};

template<class T>
struct forward_interator{
    typedef forward_interator_tag category;
    int year = 90;
};

template<class T>
struct test_traits {
    typedef typename T::category category;
};


//增加一层traits的好处是把原生指针等没有traits tag的数据类型指定偏特化编译路径
//目前没看到有啥用
template<class T>
struct test_traits<T*> {
    typedef forward_interator_tag category;
};


//algorithm
template<class Iterator>
void advance(Iterator it)
{
    _advance(it,typename test_traits<Iterator>::category());

    //此时可以通过萃取类型指针来达到重载决议，本例中构造栈对象不多，例如通过value_type来重载决议时，
    //当value_type类型属性较为复杂时，会造成不必要的construct/deconstruct开销
     _advance(it,static_cast<typename test_traits<Iterator>::category*>(0));
}

template<class Iterator>
void _advance(Iterator it, input_interator_tag)
{
    cout << "泛化  _advance\n\n";
}

template<class Iterator>
void _advance(Iterator it, output_interator_tag)
{
    cout << "output_interator  _advance\n\n";
}

template<class Iterator>
void _advance(Iterator it, forward_interator_tag)
{
    cout << "forward_interator  _advance\n\n";
}




template<class Iterator>
void _advance(Iterator it, input_interator_tag*)
{
    cout << "泛化  _advance\n\n";
}

template<class Iterator>
void _advance(Iterator it, output_interator_tag*)
{
    cout << "output_interator  _advance\n\n";
}

template<class Iterator>
void _advance(Iterator it, forward_interator_tag*)
{
    cout << "forward_interator  _advance\n\n";
}


int main(){
    //use first
    advance(input_interator<int>());
    advance(output_interator<string>());
    advance(forward_interator<long>());

    //use second
    _advance(input_interator<int>(), input_interator<int>::category());
    _advance(output_interator<string>(), output_interator<string>::category());
    _advance(forward_interator<long>(), forward_interator<long>::category());
    return 0;
}