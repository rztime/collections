//
//  main.cpp
//  C++
//
//  Created by hejunqiu on 2017/2/27.
//  Copyright © 2017年 CHE. All rights reserved.
//

#include <iostream>
#include "String.hpp"

using namespace _cc;
using namespace std;

int main(int argc, const char * argv[]) {
    auto str = s("123534fefer");
    cout << str->length();
    std::cout << "Hello, World!\n";
    return 0;
}
