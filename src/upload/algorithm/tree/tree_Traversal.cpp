#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
using namespace std;

struct TreeNode{
    int val;
    TreeNode* left;
    TreeNode* right;
};

TreeNode* CreateTree()
{
    TreeNode* node3 = new TreeNode();
    node3->val = 3;
    node3->left = nullptr;
    node3->right = nullptr;

    TreeNode* node1 = new TreeNode();
    node1->val = 1;
    node1->left = nullptr;
    node1->right = nullptr;


    TreeNode* node7 = new TreeNode();
    node7->val = 7;
    node7->left = nullptr;
    node7->right = nullptr;


    TreeNode* node9 = new TreeNode();
    node9->val = 9;
    node9->left = nullptr;
    node9->right = nullptr;


    TreeNode* node4 = new TreeNode();
    node4->val = 4;
    node4->left = node3;
    node4->right = nullptr;

    TreeNode* node2 = new TreeNode();
    node2->val = 2;
    node2->left = node1;
    node2->right = node4;

    TreeNode* node8 = new TreeNode();
    node8->val = 8;
    node8->left = node7;
    node8->right = node9;

    TreeNode* node5 = new TreeNode();
    node5->val = 5;
    node5->left = node2;
    node5->right = node8;

    return node5;
}
/* 
                5
        2               8
1               4   7       9
            3
*/


//先序遍历 根左右
vector<int> preorderTraversal(TreeNode* root) 
    {
        stack<TreeNode*> st;
        vector<int> res;
        if(root==nullptr)
        {
            return res;
        }
        st.push(root);
        while(!st.empty())
        {
            TreeNode* node=st.top();
            st.pop();
            res.push_back(node->val);   //打印根节点
            if(node->right)
            {
                st.push(node->right);   //先压入右节点
            }
            if(node->left)
            {
                st.push(node->left);    //再压入左节点
            }
        }
        return res;
}

//先序遍历 左根右
vector<int> inorderTraversal(TreeNode* root) 
{
    vector<int> res;
    stack<TreeNode*> st;
    if(root==nullptr)
    {
        return res;
    }
    TreeNode* cur=root;
    while(cur!=nullptr||!st.empty())    //此处注意循环条件
    {
        if(cur!=nullptr)    //遍历到最左子树
        {
            st.push(cur);
            cur=cur->left;
        }
        else                //换到右节点，继续遍历到最左子树
        {
            cur=st.top();
            res.push_back(st.top()->val);
            st.pop();
            cur=cur->right;
        }
    }
    return res;

}

//后序遍历 左右根 = reverse(根右左)
vector<int> postorderTraversal(TreeNode* root) 
{
    vector<int> res;
    stack<TreeNode*> st;
    TreeNode* node;
    if(root==nullptr)
    {
        return res;
    }
    st.push(root);
    while(!st.empty())
    {
        node=st.top();
        st.pop();
        res.push_back(node->val);
        if(node->left)
        {
            st.push(node->left);
        }
        if(node->right)
        {
            st.push(node->right);
        }
    }
    std::reverse(res.begin(),res.end());
    return res;
}

//统一迭代
vector<int> unityTraversal(int flag, TreeNode* root) 
{
    vector<int> res;
    stack<TreeNode*> st;
    if(root!=nullptr)
    {
        st.push(root);
    }
    while(!st.empty())
    {
        TreeNode* node=st.top();
        if(node!=nullptr)
        {
            switch (flag)
            {
            case 1:
                st.pop();
                if(node->right)
                {
                    st.push(node->right);
                }
                if(node->left)
                {
                    st.push(node->left);
                }
                st.push(node);
                st.push(nullptr);  //根节点被访问过，但是还没有处理，加入空节点作为标记
                break;
            case 2:
                st.pop();
                if(node->right)
                {
                    st.push(node->right);
                }
                st.push(node);
                st.push(nullptr);  //根节点被访问过，但是还没有处理，加入空节点作为标记
                if(node->left)
                {
                    st.push(node->left);
                }
                break;
            case 3:
                st.pop();
                st.push(node);
                st.push(nullptr);  //根节点被访问过，但是还没有处理，加入空节点作为标记
                if(node->right)
                {
                    st.push(node->right);
                }
                if(node->left)
                {
                    st.push(node->left);
                }
                break;
            
            default:
                cout << "flag error\n";
                exit(-1);
            }
            
        }
        else//只有当遇到空节点是，才将下一节点放进res
        {
            st.pop();//弹掉空节点
            res.push_back(st.top()->val);
            st.pop();//用过后把它弹掉
        }
    }
    return res;
}


int main()
{
    TreeNode* root = CreateTree();
    auto ve = unityTraversal(2, root);
    for(auto it: ve)
    cout << it << "  ";
    return 0;
}