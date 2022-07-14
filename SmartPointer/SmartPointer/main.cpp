//
//  main.cpp
//  SmartPointer
//
//  Created by 张淑慧 on 2022/7/11.
//

#include <iostream>
#include <string>
#include "unique_ptr.hpp"

struct deleter{
    void operator()(string* s){
        cout<<"deleter: "<<s<<endl;
        delete s;
    }
};


int main(int argc, const char * argv[]) {
    // insert code here...
    string *s1 = new string("1234");
    cout<<"s1: "<<s1<<endl;
    deleter d;
    test_mem::unique_ptr<string, deleter> u1(s1, d);
    cout<<"s1 get: "<<u1.get()<<endl;
    cout<<"s1 release: "<<u1.release()<<endl;
    string* s2 = new string("5678");
    cout<<"s2: "<<s2<<endl;
    u1.reset(s2);
    cout<<"s2 reset get: "<<u1.get()<<endl;
    
    std::cout << "Hello, World!\n";
    return 0;
}
