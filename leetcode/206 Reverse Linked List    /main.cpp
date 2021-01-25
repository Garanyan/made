//
// Created by Ovanes on 11.02.2020.
//
//

#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>

class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* current = head;
        while(current)
        {
            ListNode* tmp = current->next;
            current->next = prev;
            prev = current;
            current = tmp;
        }
        
        return prev;
    }
};

int main(int argc, char **argv){

    return 0;
}