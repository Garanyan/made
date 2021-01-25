#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>

class Solution {
public:
    
    void in_order(std::stack<int>& s, TreeNode* root, int k)
    {
        if(root==nullptr)
        {
            return;
        }
        
        in_order(s, root->left, k);
        
        if(s.size()==k)
            return;
        
        s.push(root->val);
        
        in_order(s, root->right, k);
    }
    
    int kthSmallest(TreeNode* root, int k) {
        std::stack<int> s;
        in_order(s, root, k);
        return s.top();
    }
};

int main(int argc, char **argv){

    return 0;
}