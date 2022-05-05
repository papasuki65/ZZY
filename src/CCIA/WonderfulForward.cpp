#include <iostream>

using namespace std;

template <typename T> struct Identity {

    typedef T type;

};

template <typename T> struct Identity<T&> {

    typedef T type;

};

template <typename T> struct Identity<T&&> {

    typedef T type;

};
/* 
 *  正常来说此版本已满足完美转发要求，因为转发的话，t已经是一个具名的变量，不可能再是右值，
 *  下面增加其右值引用版本是对用户使用Forward<T>(move(t))情况的补充。
 */
template <typename T> T&& Forward(typename Identity<T>::type& t) {

    return static_cast<T&&>(t);

}

template <typename T> T&& Forward(typename Identity<T>::type&& t) {

    return static_cast<T&&>(t);

}


template<typename T>
void inner(T&&) {
	if(is_same<int, T>::value)
		cout << "is int"<< endl;
    else if(is_same<int&, T>::value)
		cout << "is int&" << endl;
	else if(is_same<int&&, T>::value)
		cout << "is int&&" << endl;
	else if(is_same<const int&, T>::value)
		cout << "is const int&" << endl;
}


template <typename T> void outer(T&& t) {

    inner(Forward<T>(t));

}

int main() {

    int a = 1;

    const int b = 2;

    cout << "Directly calling inner()." << endl;

    inner(a);

    inner(b);

    inner(3);

    cout << endl << "Calling outer()." << endl;

    outer(a);

    outer(b);

    outer(3);

	return 0;

}