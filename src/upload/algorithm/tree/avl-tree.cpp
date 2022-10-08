#include <iostream>
#include <queue>
using namespace std;

template<class K, class V>
struct AVLTreepNode{
    AVLTreepNode(){}
    AVLTreepNode(const K& k, const V& v)
    :_key(k)
    ,_value(v)
    ,_pLeft(nullptr)
    ,_pRight(nullptr)
    ,_pParent(nullptr)
    ,_bf(0)
    {}

    K _key;
    V _value;
    int _bf;
    AVLTreepNode<K, V>* _pLeft;
    AVLTreepNode<K, V>* _pRight;
    AVLTreepNode<K, V>* _pParent;
};

template<class K, class V>
class AVLTree{
        typedef AVLTreepNode<K, V> Node;
        typedef Node* pNode;
    private:
        pNode _pRoot;
    public:
        AVLTree()
        :_pRoot(nullptr)
        {}

        bool Insert(const K& k, const V& v) {
            if(nullptr == _pRoot) {
                pNode temp = new Node(k, v);
                _pRoot = temp;
                return true;
            }
            pNode cur = _pRoot;
            pNode parent = nullptr;
            while(cur) {
                if(k > cur->_key) {
                    parent = cur;
                    cur = cur->_pRight;
                }
                else if(k < cur->_key) {
                    parent = cur;
                    cur = cur->_pLeft;
                }
                else
                    return false;   //节点k相等不插入返错
            }
            cur = new Node(k, v);
            if(k <  parent->_key)
                parent->_pLeft = cur;
            else
                parent->_pRight = cur;

            cur->_pParent = parent;
            while(parent) {
                if(parent->_pLeft == cur)
                    parent->_bf--;
                if(parent->_pRight == cur)
                    parent->_bf++;

                if(0 == parent->_bf)
                    break;
                else if(1 == parent->_bf || -1 == parent->_bf) {
                    cur = parent;
                    parent = parent->_pParent;
                }
                else{
                    if(-2 == parent->_bf && -1 == cur->_bf)  {  //右旋
                        RotateR(parent);
                    }
                    else if(2 == parent->_bf && 1 == cur->_bf) {    //左旋
                        RotateL(parent);
                    }
                    else if(-2 == parent->_bf && 1 == cur->_bf) {   //左右旋
                        RotateLR(parent);
                    }
                    else if(2 == parent->_bf && -1 == cur->_bf) {   //右左旋
                        RotateRL(parent);
                    }
                    else {
                        cout << "未知错误\n";
                        return false;
                    }
                    break;
                }
            }   
            return true;
        }

        void RotateR(pNode parent) {
            pNode subL = parent->_pLeft;
            pNode subLR = subL->_pRight;
            parent->_pLeft = subLR;
            if(subLR)
                subLR->_pParent = parent;

            subL->_pRight = parent;
            pNode pparent = parent->_pParent;
            parent->_pParent = subL;
            subL->_pParent = pparent;

            if(parent == _pRoot)
                _pRoot = subL;
            else {
                if(pparent->_pLeft == parent)
                    pparent->_pLeft = subL;
                else
                pparent->_pRight = subL;
            }
            parent->_bf = subL->_bf = 0;
        }


        void RotateL(pNode parent) {
            pNode subR = parent->_pRight;
            pNode subRL = subR->_pLeft;

            parent->_pRight = subRL;
            if(subRL)
                subRL->_pParent = parent;
            
            subR->_pLeft = parent;
            pNode pParent = parent->_pParent;
            parent->_pParent = subR;
            subR->_pParent = pParent;

            if(parent == _pRoot)
                _pRoot = subR;
            else {
                if(parent == pParent->_pLeft)
                    pParent->_pLeft = subR;
                else
                    pParent->_pRight = subR;
            }
            parent->_bf = subR->_bf = 0;

        }

        void RotateLR(pNode parent) {
            pNode subL = parent->_pLeft;
            pNode subLR = subL->_pRight;
            int bf = subLR->_bf;    //此时要先拿bf，左右旋时该值会修正
            RotateL(parent->_pLeft);
            RotateR(parent);

            //其实有bf为0的情况，注意！
            if(bf == 1)
                parent->_bf = 0;
            else if(bf == -1)
                parent->_bf = 1;
                
        }

        void RotateRL(pNode parent) {
            pNode subR = parent->_pRight;
            pNode subRL = subR->_pLeft;
            int bf = subRL->_bf;    //此时要先拿bf，左右旋时该值会修正
            RotateR(parent->_pRight);
            RotateL(parent);
            
            //其实有bf为0的情况，注意！
            if(bf == 1)
                parent->_bf = -1;
            else if(bf == -1)
                parent->_bf = 0;

        }
        void prin()
        {
            if(_pRoot == nullptr) return ;
            queue<pNode> qu;
            qu.push(_pRoot);
            while(!qu.empty()) {
                   pNode item = qu.front();
                   qu.pop();
                   cout << item->_key << "[" << item->_bf << "]";
                   if(item->_pLeft){
                       cout << "  has left child: " << item->_pLeft->_key << "[" << item->_pLeft->_bf << "]";
                       qu.push(item->_pLeft);
                   }    
                   if(item->_pRight){
                       cout << "  has right child: " << item->_pRight->_key << "[" << item->_pRight->_bf << "]";
                       qu.push(item->_pRight);
                   }
                   cout << endl;

            }
        }

};


int main()
{
    AVLTree<int ,int> t;
    //右单旋
    // t.Insert(18,0);
    // t.Insert(14,0);
    // t.Insert(20,0);
    // //t.Insert(15,0);
    // t.Insert(12,0);
    // t.Insert(11,0);
    // t.prin();

    //左单旋
    // t.Insert(18,0);
    // t.Insert(14,0);
    // t.Insert(20,0);
    // // t.Insert(19,0);
    // t.Insert(30,0);
    // t.Insert(40,0);
    // t.prin();

    // //左右单旋
    // t.Insert(40,0);
    // t.Insert(30,0);
    // t.Insert(50,0);
    // t.Insert(20,0);
    // t.Insert(35,0);
    // t.Insert(34,0);
    // t.prin();

    //左右单旋
    t.Insert(40,0);
    t.Insert(30,0);
    t.Insert(50,0);
    t.Insert(20,0);
    t.Insert(25,0);
    t.prin();
    return 0;
}
