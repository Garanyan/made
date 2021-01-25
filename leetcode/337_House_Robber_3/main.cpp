//
// Created by Ovanes on 11.02.2020.
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
    
    void post_order(int& id, std::map<TreeNode*,int>& ids, TreeNode* root,
                   std::vector<int>& rob, std::vector<int>& no_rob)
    {
        if(root == nullptr)
            return;
        
        ids[root] = id++;
        
        post_order(id, ids, root->left, rob, no_rob);
        post_order(id, ids, root->right, rob, no_rob);
        
        rob.resize(id);
        no_rob.resize(id);
        
        int left_id = ids[root->left];
        int right_id = ids[root->right];
        rob[ids[root]] = root->val + no_rob[left_id] + no_rob[ids[root->right]];
        no_rob[ids[root]] = std::max(rob[left_id], no_rob[left_id]) + std::max(rob[right_id], no_rob[right_id]);
    }
    int rob(TreeNode* root) {
        std::vector<int> rob={0};
        std::vector<int> no_rob={0};
        std::map<TreeNode*,int> ids;
        int id = 1;
        ids[nullptr] = 0;
        post_order(id, ids, root, rob, no_rob);
        
        
        return std::max(rob[ids[root]], no_rob[ids[root]]);
    }
};

int main(int argc, char **argv){

    return 0;
}