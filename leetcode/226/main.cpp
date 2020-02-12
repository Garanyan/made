//
// Created by Ovanes on 12.02.2020.
//
//

#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>


  //Definition for a binary tree node.
struct TreeNode {
      int val;
      TreeNode *left;
      TreeNode *right;
      TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
 
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if(!root)
            return nullptr;
        
        TreeNode* tmp = root->left;
        TreeNode* tmpRight = root->right;
        root->left = invertTree(tmpRight);
        root->right = invertTree(tmp);
        
        return root;
    }
};

int main(int argc, char **argv){

    return 0;
}