//
// Created by Ovanes on 11.02.2020.
//
//

#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>


 //* Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
    int maxDepth(TreeNode* root) {
        if(!root)
            return 0;
        
        int leftTreeSize = maxDepth(root->left);
        int rightTreeSize = maxDepth(root->right);
        return std::max(leftTreeSize, rightTreeSize) + 1;
    }
};

int main(int argc, char **argv){

    return 0;
}