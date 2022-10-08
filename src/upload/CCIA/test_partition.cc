#include <iostream>
#include <vector>
#include <list>
#include <./bits/stl_algo.h>
using namespace std;

// void splice ( iterator position, list<T,Allocator>& x );会在position后把list&x所有的元素到剪接到要操作的list对象
// void splice ( iterator position, list<T,Allocator>& x, iterator it );只会把it的值剪接到要操作的list对象中
// void splice ( iterator position, list<T,Allocator>& x, iterator first, iterator last );把first 到 last 剪接到要操作的list对象中


template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{

    if(input.empty())
    {
        return input;
    }
    std::list<T> result;


    result.splice(result.begin(),input,input.begin());//第一个元素挪到result中
    T const& pivot=*result.begin();

    auto divide_point=std::partition(input.begin(),input.end(),
            [&](T const& t){return t<pivot;});

    std::list<T> lower_part;
    cout << "lower_part size: " << lower_part.size() << endl;
    lower_part.splice(lower_part.end(),input,input.begin(),divide_point);//将input前段小的元素移至lower_part
    
    auto new_lower(sequential_quick_sort(std::move(lower_part)));//前段继续递归排序

    auto new_higher(sequential_quick_sort(std::move(input)));//后段继续递归排序
    cout << "result size: " << result.size() << endl;
    result.splice(result.end(),new_higher);
    result.splice(result.begin(),new_lower);
    return result;
}

bool IsOdd (int i) { //用语判断奇数
    return (i%2)==1; //奇数返回true，偶数返回0
}
int main(void)
{
    std::list<int> ivec;  
    for (int i=1; i<10; ++i) 
        ivec.push_back(i); // 1 2 3 4 5 6 7 8 9 
    auto bound = partition (ivec.begin(), ivec.end(), IsOdd);   
    std::cout << "odd elements:";
    for (auto it=ivec.begin(); it!=bound; ++it)
        std::cout << ' ' << *it;       //输出1,9,3,7,5    
    std::cout << "\neven elements:";
    for (auto it=bound; it!=ivec.end(); ++it)
        std::cout << ' ' << *it;     //输出6,4,8,2
    std::cout << "\nivec size: " << ivec.size() << endl;
    for (auto it=ivec.begin(); it!=ivec.end(); ++it) std::cout<< *it << " ";std::cout << endl;
    auto res = sequential_quick_sort(ivec);
    for (auto it=res.begin(); it!=res.end(); ++it) std::cout<< *it << " ";std::cout << endl;
    for (auto it=ivec.begin(); it!=ivec.end(); ++it) std::cout<< *it << " ";std::cout << endl;
    return 0;
}