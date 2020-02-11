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
    string defangIPaddr(string address) {
        size_t pos = address.find('.');
        while(pos != std::string::npos)
        {
            address.insert(address.begin() + pos, '[');
            address.insert(address.begin() + pos+2, ']');
            pos += 3;
            pos = address.find('.', pos);
        }
        return address;
    }
};

int main(int argc, char **argv){

    return 0;
}