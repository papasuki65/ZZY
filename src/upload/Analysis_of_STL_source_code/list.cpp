#include <iostream>

using namespace std;

/*
* 每个新设计的迭代器都可以继承自std::iterator<std::forward_iterator_tag, T>
*/

/*
*普通类继承时，子类可以直接使用父类定义的typedef类型
*如果是模板类继承时，这样的类型是dependent type，这里Base<T>被用作基类，于是这种基类叫做dependent base class。
*按照C++标准规定，除非显式用基类名::成员名的语法，否则不会查找dependent base class的成员。
*/

template <typename T>
class List {
    public:
        struct ListItem {
                ListItem *_prve;
                ListItem *_next;
                T _value;
        };

        struct ListIter :public std::iterator<std::forward_iterator_tag, T>{
            typedef typename std::iterator<std::forward_iterator_tag, T>::value_type value_type;
            ListItem* ptr;
            ListIter(ListItem* it = nullptr): ptr(it){}
            value_type operator *(){return ptr->_value;} //这里为什么不能用value_type?
            ListItem* operator ->(){return ptr;}
            ListIter operator ++(){ptr = ptr->_next;return *this;}
            ListIter operator ++(int){ListIter temp = *this;++*this;return temp;}
            bool operator ==(const ListIter& rhs)const{
                return this->ptr == rhs.ptr;
            }
            bool operator !=(const ListIter& rhs) const{

                return this->ptr != rhs.ptr;
            }
        };

        List(){
            _M_Item = new ListItem();
            _M_Item->_next = _M_Item->_prve = _M_Item;
            _size = 0;
        }
        ListIter insert(ListIter position, const T& value)
        {
            ListItem* x = new ListItem();
            x->_value = value;
            x->_prve = position.ptr->_prve;
            x->_next = position.ptr;
            position.ptr->_prve->_next = x;
            position.ptr->_prve = x;
            _size++;
            return x;

        }
        ListIter push_front(T value){
            
            return insert(begin(), value);
        }
        ListIter push_back(T value){
            return insert(end(), value);
        }

        void clear(){
            auto first = begin();
            while(first != end()){
                auto temp = first->_next;
                delete first.ptr;
                first = temp;
            }
            _M_Item->_next = _M_Item->_prve = &_M_Item;
            _size = 0;
        }

        ListIter begin(){
            return _M_Item->_next;
        }

        ListIter end(){
            return _M_Item;
        }

        long size(){
            return _size;
        }

        bool empty() {
            return begin() == end();
        }
    

    private:
        long _size; 
        ListItem* _M_Item;
    public:

        //将first-last插入到position前（move语义）
        void transfer(ListIter position, ListIter first, ListIter last) {   
            if(position != last) {
                last.ptr->_prve->_next = position.ptr;
                first.ptr->_prve->_next = last.ptr;
                position.ptr->_prve->_next = first.ptr;
                
                auto temp = position.ptr->_prve;
                position.ptr->_prve = last.ptr->_prve;
                last.ptr->_prve = first.ptr->_prve;
                first.ptr->_prve = temp;
            }
        }
    
        //将x合并到this（move语义）
        void merge(List x) {
            ListIter m_first = begin();
            ListIter m_end = end();
            ListIter e_first = x.begin();
            ListIter e_end = x.end();
            while(m_first != m_end && e_first != e_end)
            {
                if(*m_first > *e_first)
                {
                    auto temp = e_first;
                    transfer(m_first, e_first, ++temp);
                    e_first = temp;
                }
                else
                    m_first++;
            }
            if(e_first != e_end)
                transfer(m_end, e_first, e_end);
        }

        void swap(List &x){
            auto temp = _M_Item;
            _M_Item = x._M_Item;
            x._M_Item = temp;
        }

        void sort() {
            if(size() == 0 || size() == 1)
                return ;
            List carry;
            List counter[64];   //counter[i]内个list中最多能放2^i个元素
            int fill = 0;       //指向counter顶部层数，类似end()
            while(!empty())
            {
                int i = 0;
                carry.transfer(carry.begin(), begin(), ++begin());  //将第一个元素分离给carry
                while(i < fill && !counter[i].empty()) {
                    
                    // counter[i].merge(carry);
                    // carry.swap(counter[i++]);
                    carry.merge(counter[i++]);  //从底到顶，层数据量不会0时，依次归并
                }
                carry.swap(counter[i]); //交给空数据层或者fill最高层
                if(i == fill)  ++fill;
                
            }
            for(int i = 1;i < fill; ++i)
                counter[i].merge(counter[i - 1]);   //合并每层数据集
            swap(counter[fill - 1]);    //返回归并后顶层数据集为sort排序结果
        }

        void display(){
            for(auto iter = begin();iter != end(); ++iter) {
                cout << "item: " << *iter <<endl;
            }
        }
        

        
};
template<class ListIter, class T>
ListIter find(ListIter first, ListIter end, const T value)
{
    while(first != end && *first != value){
        first++;
    }
    return (first == end) ? first : NULL;
}

int main()
{
 
    List<int> li;
    li.push_back(34);
    li.push_back(6);
    li.push_back(54);
    li.push_back(23);
    li.push_back(18);

    List<int> st;
    st.push_back(7);
    st.push_back(13);
    st.push_back(4);
    st.push_back(44);
    st.push_back(3);
    
    li.sort();
    li.display();

    st.sort();
    st.display();

    
    return 0;
}