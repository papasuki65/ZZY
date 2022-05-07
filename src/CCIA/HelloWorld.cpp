#include <iostream>
#include <thread>

/*
    detach的意义在于线程结束时资源的回收方式不一样了。如果子线程不detach则主线程可以通过join()函数来等待子线程结束并回收资源，但如果子线程detach了，则主线程则不能通过join()函数进行等待和回收资源，此时子线程的资源回收将由系统进行。
    如果进程的main函数返回或者任何线程调用了 exit， 整个进程就终止，其中包括它的任何线程。
 
    std::thread在以下几种情况下是不可join的：（在调用join之前判断joinable，只有在线程对象是可join的情况下再进行join或者detach操作。）
        由std::thread默认构造的std::thread对象，也就是没有指定线程函数的std::thread对象是不可join的；
        该std::thread对象被std::move操作
        该std::thread对象已经执行过std::thread::join()方法或者std::thread::detach()方法，此时std::thread对象是不可join的；

    不要忘记对有关联(已经为该线程指定了线程函数)的std::thread对象调用join或者detach方法
    在一个有关联的线程函数的std::thread对象如果没有调用join或者detach方法会在std::thread对象析构的时候中断程序。如果程序发生异常，也必须在异常处理中调用std::thread对象的join或者detach方法。
 */
void hello()
{
    std::cout << "Hello Concurrnet World\n";
}

int main()
{
    std::thread t(hello);
    t.join();
    return 0;
}