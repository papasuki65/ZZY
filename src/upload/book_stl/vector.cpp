#include "../alloc/jjalloc.h"
using namespace std;

template<class T, class Alloc = JJ::allocator<T>>
class vector {
    typedef T 			value_type;
    typedef T* 			pointer;
    typedef T* 			iterator;
    typedef T& 			reference;
    typedef size_t		size_type;
    typedef ptrdiff_t	difference_type;

protected:
    Alloc data_allocator;

    iterator start;
    iterator finish;
    iterator end_of_storage;
    iterator copy_backward(iterator first, iterator finish, iterator new_finish){
        iterator cur = finish;
        iterator new_cur = new_finish;
        for(;cur != first; --cur,--new_cur)
        {
            data_allocator.construct(new_cur - 1, *(cur - 1));
        }
        return new_finish - (finish - first);
    }
    iterator uninitialized_copy(iterator first, iterator finish, iterator new_start)
    {
        iterator cur = first;
        iterator new_cur = new_start;
        for(;cur != finish; ++cur,++new_cur)
        {
            data_allocator.construct(new_cur, *cur);
        }
        return new_start + (finish - first);
    }
    void destroy(iterator first, iterator finish){
        for(;first != finish;++first)
            first->~value_type();
    }

    void destroy(iterator iter){
            (--iter)->~value_type();
    }
    
    void deallocate() {
        if(start)
            data_allocator.deallocate(start, end_of_storage - start);
    }
    

    
    public:
   
        iterator begin() {return start;}
        iterator end() {return finish;}
        size_type size()  {return size_type(end() - begin());}
        size_type capacity() {
            return size_type(end_of_storage - begin());
        }
        bool empty() const {return begin() == end();}
        reference operator[](size_type n){return *(begin() + n);}
        vector():start(0), finish(0), end_of_storage(0){}
        vector(size_type n, const T& value){fill_initialize(n, value);}
        vector(int n, const T& value){fill_initialize(n, value);}
        vector(long n, const T& value){fill_initialize(n, value);}
        explicit vector(size_type n) {fill_initialize(n,T());}

        ~vector(){
            destroy(start, finish);
            deallocate();
        }
        void insert_aux(iterator position, const T& x){
            if(finish != end_of_storage) {
                data_allocator.construct(finish, *(finish - 1));
                ++finish;
                T x_copy = x;
                copy_backward(position, finish - 2,finish - 1);
                *position = x_copy;
            }
            else{
                const size_type old_size = size();
                const size_type len = old_size != 0 ? 2 * old_size : 1;

                iterator new_start  =data_allocator.allocate(len);
                iterator new_finish = new_start;
                {
                    new_finish = uninitialized_copy(start, position, new_start);
                    data_allocator.construct(new_finish, x);
                    ++new_finish;
                    new_finish = uninitialized_copy(position, finish, new_finish);
                }

                destroy(begin(), end());
                deallocate();

                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + len;
            }
        }

        void insert(iterator position, size_type n, const T& x) {
            if(n != 0)
            {
                if(size_type(end_of_storage - finish) >= n) {
                    iterator old_finish = finish;
                    finish = uninitialized_copy(position, finish, position + n);
                    fill(position, position + n, x);
                }
                else {
                    const size_type old_size = size();
                    const size_type len = old_size !=old_size + (n > old_size ? n : old_size);
                    iterator new_start  =data_allocator.allocate(len);
                    iterator new_finish = new_start;
                    {
                        new_finish = uninitialized_copy(start, position, new_start);
                        fill(new_finish, new_finish + n, x);
                        new_finish += n;
                        new_finish = uninitialized_copy(position, finish, new_finish);
                    }

                    destroy(begin(), end());
                    deallocate();

                    start = new_start;
                    finish = new_finish;
                    end_of_storage = new_start + len;
                }
            }
        }

        reference front(){return *begin();}
        reference back(){return *(end() - 1);}
        void push_back(const T& x) {
            if(finish != end_of_storage) {
                data_allocator.construct(finish, x);
                ++finish;
            }
            else
                insert_aux(end(), x);
        }

        void pop_back() {
            --finish;
            destroy(finish);
        }

        iterator erase(iterator position) {
            if(position + 1 != end())
                copy(position + 1, finish, position);
            --finish;
            destroy(finish);
            return position;
        }

        iterator erase(iterator first,iterator last) {
            iterator i = copy(last, finish, first);
            destroy(i, finish);
            finish = finish - (last - first);
            return first;
        }


        void resize(size_type new_size, const T& x) {
            if(new_size < size())
                erase(begin() + new_size, end());
            else
                insert(end(), new_size - size(), x);
        }

        void resize(size_type new_size) {
            resize(new_size, T());
        }

        void clear(){
            erase(begin(), end());
        }

        void fill_initialize(size_type n, const T& value) {
            start = allocate_and_fill(n, value);
            finish = start + n;
            end_of_storage = finish;
        }



    protected:
        iterator allocate_and_fill(size_type n, const T& x) {
            iterator result = data_allocator.allocate(n);
            uninitialized_fill_n(result, n ,x);
            return result;
        }

};


int main()
{
    vector<int> ve;
    ve.push_back(1);
    ve.push_back(2);
    ve.push_back(3);
    ve.push_back(4);
    ve.push_back(5);
    ve.push_back(6);
    ve.capacity();
    for(auto i: ve)
        cout << "value: " << i << endl;
    return 0;
}