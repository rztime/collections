//
//  main.m
//  collection_codes
//
//  Created by hejunqiu on 2017/2/24.
//  Copyright © 2017年 CHE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#include "String.hpp"
#include <iostream>
#include "TaggedPointer.h"

using namespace std;
using namespace _CC;

int main(int argc, char * argv[]) {
    @autoreleasepool {
        shared_ptr<String> str = s("2235465432");
        for (auto &s : *str) {
            printf("%c ", s);
        }
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
