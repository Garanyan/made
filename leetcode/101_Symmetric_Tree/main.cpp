#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    
    bool equal(TreeNode*l,TreeNode* r)
    {
        if(l==nullptr && r == nullptr)
        {
            return true;
        }
        
        if(l==nullptr && r != nullptr || l!=nullptr && r == nullptr)
        {
            return false;
        }
        
        return l->val == r->val && equal(l->right, r->left) &&
            equal(l->left, r->right);
    }
    bool isSymmetric(TreeNode* root) {
        if(!root)
            return true;
        
        return equal(root->left, root->right);
    }
};

int main(int argc, char **argv){

    return 0;
}