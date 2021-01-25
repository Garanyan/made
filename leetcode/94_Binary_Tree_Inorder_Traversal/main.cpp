//
// Created by Ovanes on 12.02.2020.
//
//

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
    void in_order(std::vector<int>& nums, TreeNode* root)
    {
        if(!root)
        {
            return;
        }
        
        in_order(nums, root->left);
        nums.emplace_back(root->val);
        in_order(nums,root->right);
    }
    
    vector<int> inorderTraversal(TreeNode* root) {
        std::vector<int> res;
        in_order(res, root);
        return res;
    }
};

int main(int argc, char **argv){

    return 0;
}