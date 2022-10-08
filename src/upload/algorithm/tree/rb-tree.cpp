/* 
红黑树性质：
根节点是黑色的
每个叶子节点都是黑色的空节点（NIL），也就是说，叶子节点不存储数据（图中将黑色的、空的叶子节点都省略掉了）
任何相邻的节点都不能同时为红色，也就是说，红色节点是被黑色隔开的
每个节点，从该节点到达其可达叶子节点的所有路径，都包含相同数目的黑色几点
 */
#include <iostream>
#include <set>
#include <map>
using namespace std;

int main()
{
    map<int ,int>m;
    //int a = m[44];
    cout << m.size();
    return 0;
    set<int> s;
    s.insert(123);
    auto au = s.begin();
    au++;
    cout << *au << endl;
    return 0;
}