#include <iostream>

using namespace std;

/*
* 每个新设计ID迭代器都可以继承自std::iterator<std::forward_iterator_tag, T>
* 这里因为迭代器没有实现在容器内部，所以继承在了元素项上
*/
template <typename T>
class ListItem : public std::iterator<std::forward_iterator_tag, T>{
    public:
    ListItem(T value): _value(value){}
        T value(){return _value;}
        ListItem *next() const{
            return _next;
        }
        
        ListItem *_next;
    private:
        T _value;
};


template <class Item>
struct ListIter{
    Item* ptr;
    ListIter(Item* it = nullptr): ptr(it){}
    typename Item::value_type operator *(){return ptr->value();}
    Item* operator ->(){return ptr;}
    ListIter operator ++(){ptr = ptr->next();return *this;}
    ListIter operator ++(int){ListIter temp = *this;++*this;return temp;}
    bool operator ==(const ListIter& rhs)const{
        return this->ptr == rhs.ptr;
    }
    bool operator !=(const ListIter& rhs) const{

        return this->ptr != rhs.ptr;
    }
};

template <typename T>
class List{
    public:
        List(){
            _front = nullptr;
            _end = nullptr;
            _size = 0;
        }
        void insert_front(T value){
            _size++;
             auto t = new ListItem<T>(value);
            if(nullptr == _front)
            {
                _front = t;
                _front->_next = nullptr;
                _end = t;
                _end->_next = nullptr;
                return ;
            }
            auto temp = _front;
            _front = t;
            _front->_next = temp;
        }
        void insert_end(T value){
            _size++;
            auto t = new ListItem<T>(value);
            if(nullptr == _end)
            {
                _front = t;
                _front->_next = nullptr;
                _end =t;
                _end->_next = nullptr;
                return ;
            }
            auto temp = _end;
            _end = t;
            temp->_next = _end;

        }
        void display(){
            auto p = _front;
            while(nullptr != p)
            {
                cout << p->value() <<endl;
                p = p->_next;
            }
        }

        void clear(){
            auto p = _front;
            _front = nullptr;
            while(nullptr != p)
            {
                auto  temp = p->_next;
                delete p;
                p = temp;
            }
            _size = 0;
            if(_front != nullptr )
                cout << "false 1\n\n";
            if(_end == nullptr)
                cout << "false 2\n\n";
        }

        ListIter<ListItem<T>> front(){
            return ListIter<ListItem<T>>(_front);
        }

         ListIter<ListItem<T>> end(){
            return ListIter<ListItem<T>>(_end);
        }
        
    private:
       ListItem<T>* _end;
       ListItem<T>* _front;
       long _size; 
};

template<class ListIter, class T>
ListIter find(ListIter first, ListIter end, const T value)
{
    while(first != end && *first != value){
        first++;
    }
    return first;
}

int main()
{
    List<int> li;
    for(int i = 0;i < 5;i++)
    {
        li.insert_front(i);
        li.insert_end(i + 2);
    }
    li.display();
    cout <<"---------\n\n";
    ListIter<ListItem<int>> iter(li.front());
    do{
        cout << *iter << endl;
    }while(iter++ != li.end());

    auto it = find(li.front(), ++li.end(), 6);
    cout <<"----------\n\n";
    cout << *it << endl;
    return 0;
}