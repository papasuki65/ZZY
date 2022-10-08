/*  22. 括号生成

    数字 n 代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的并且 有效的 括号组合。
    示例 1：
        输入：n = 3
        输出：["((()))","(()())","(())()","()(())","()()()"]
    示例 2：
        输入：n = 1
        输出：["()"]
    提示：
    1 <= n <= 8
 */
#include <iostream>
#include <vector>
using namespace std;
class Solution {
public:
    int value = 0;
    int ince = 0;
    void dfs(string &str)
    {
        if(0 == value && 0 == ince)
        {
            g_res.push_back(str);
            return ;
        }
            
        if(value > 0 && ince > 0)
        {
            str += '(';
            ++value;
            --ince;
            dfs(str);
            str.pop_back();
            --value;
            ++ince;

            str += ')';
            --value;
            dfs(str);
            str.pop_back();
            ++value;
        }
        else if(value > 0 && ince == 0)
        {
            str += ')';
            --value;
            dfs(str);
            str.pop_back();
            ++value;
        }
        else if(value == 0 && ince > 0)
        {
            str += '(';
            ++value;
            --ince;
            dfs(str);
            str.pop_back();
            --value;
            ++ince;
        }
        return ;
    }
    vector<string> g_res;
    vector<string> generateParenthesis(int n) {
        string str;
        str.reserve(2*n);
        g_res.reserve(1430);
        value = 0;
        ince = n;
        dfs(str);
        return g_res;
    }
};

int main() {
    Solution so;
    auto au = so.generateParenthesis(3);
    for(auto item: au)
    cout << item<< endl;
    return 0;
}