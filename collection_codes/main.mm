//
//  main.m
//  collection_codes
//
//  Created by hejunqiu on 2017/2/24.
//  Copyright © 2017年 CHE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#include <memory>

using namespace std;

class Test : public shared_ptr<Test>, public enable_shared_from_this<Test>
{
public:
    Test()
    :shared_ptr<Test>(this)
    {}
    void print() { printf("11111\n"); }
protected:
    void* operator new(size_t s) CA_NORETURN
    {
        throw "";
    }
};

int main(int argc, char * argv[]) {
    @autoreleasepool {
        {
            Test t;
            t.print();
            {
                Test t1(t);
                t1.print();
            }
        }
        printf("3\n");
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
