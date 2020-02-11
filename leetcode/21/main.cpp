//
// Created by Ovanes on 11.02.2020.
//
//

#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>

//Definition for singly-linked list.
  struct ListNode {
      int val;
      ListNode *next;
      ListNode(int x) : val(x), next(NULL) {}
};
 
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* result = &dummy;

        while(l1 || l2)
        {
            if(l1==nullptr)
            {
                result->next = l2;
                break;
            }
            else if (l2 ==nullptr)
            {
                result->next = l1;
                break;
            }
            
            if(l1->val < l2->val)
            {
                result->next = l1;
                l1 = l1->next;
            }
            else
            {
                result->next = l2;
                l2 = l2->next;
            }
            result = result->next;
        }
        return dummy.next;
    }
};

int main(int argc, char **argv){

    return 0;
}