//
//  main.cpp
//  ThreadPool
//
//  Created by 张淑慧 on 2022/7/10.
//

#include <iostream>
#include <vector>
#include <deque>
#include <atomic>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <utility>

using namespace std;

template<typename T> class thread_safe_queue{
private:
    deque<T> data;
    mutex mut;
    condition_variable cond;
public:
    thread_safe_queue(){}
    thread_safe_queue(thread_safe_queue& other){
        data = other.data;
        mut = other.mut;
        cond = other.cond;
    }
    ~thread_safe_queue(){
        for(unsigned i = 0; i < data.size(); i++){
            data.pop_back();
        }
    }
    void push(T& tvalue){
        lock_guard<mutex> lk(mut);
        data.push_back(tvalue);
        cond.notify_all();
    }
    void wait_and_pop(T& tvalue){
        unique_lock<mutex> lk(mut);
        cond.wait(lk, [this]{ return !data.empty();});
        tvalue = move(data.front());
        data.pop_front();
    }
    shared_ptr<T> wait_and_pop(){
        unique_lock<mutex> lk(mut);
        cond.wait(lk, [this]{return !data.empty();});
        shared_ptr<T> ret = make_shared<T>(data.front());
        data.pop_front();
        return ret;
    }
    bool try_pop(T& tvalue){
        lock_guard<mutex> lk(mut);
        if(data.empty())
            return false;
        tvalue = data.front();
        data.pop_front();
        return true;
    }
    shared_ptr<T> try_pop(){
        lock_guard<mutex> lk(mut);
        if(data.empty())
            return make_shared<T>();
        return make_shared<T>(move(data.pop_front()));
    }
};

class thread_join{
private:
    vector<thread>& threads;
public:
    explicit thread_join(vector<thread>& th):threads(th){};
    ~thread_join(){
        for(unsigned long i = 0; i < threads.size(); i++){
            if(threads[i].joinable())
                threads[i].join();
        }
    }
};

class ThreadPool{
private:
    int thread_count;
    atomic<bool> done;
    thread_safe_queue<std::function<void()>> task_queue;
    vector<thread> threads;
    thread_join joiner;

    void work_thread(){
        try{
            while(!done){
                function<void()> task;
                task_queue.wait_and_pop(task);
                task();
                /*if(task_queue.try_pop(task)){
                    task();
                }else{
                    this_thread::yield();
                }*/
            }
        }catch(...){
            cout<<"there occurs an error"<<endl;
            done = true;
        }
    }
public:
    void submit(function<void()> tvalue){
        task_queue.push(tvalue);
    }
    ThreadPool(int count=thread::hardware_concurrency()):thread_count(count),done(false),joiner(threads){
        for(unsigned i = 0; i < thread_count; i++){
            thread t(&ThreadPool::work_thread, this);
            threads.push_back(std::move(t));
        }
    }
    ~ThreadPool(){
        cout<<"descontructor"<<endl;
        done = true;
    }
};

vector<int> vec = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

void task1(){
    for(int i = 0; i < vec.size(); i++){
        vec[i] += 1;
        cout<<"task1: "<<vec[i]<<endl;
    }
}

void task2(){
    for(int i = 0; i < vec.size(); i++){
        vec[i] += 1;
        cout<<"task2: "<<vec[i]<<endl;
    }
}

void task3(){
    for(int i = 0; i < vec.size(); i++){
        vec[i] += 1;
        cout<<"task3: "<<vec[i]<<endl;
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    ThreadPool pool;
    pool.submit(task1);
    pool.submit(task2);
    pool.submit(task3);
    sleep(10000);
    std::cout << "Hello, World!\n";
    return 0;
}
