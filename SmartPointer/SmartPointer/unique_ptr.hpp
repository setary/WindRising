//
//  unique_ptr.hpp
//  SmartPointer
//
//  Created by 张淑慧 on 2022/7/11.
//

#ifndef unique_ptr_hpp
#define unique_ptr_hpp

#include <stdio.h>
#include <iostream>
#include <tuple>

using namespace std;

namespace test_mem{
template<typename _Tp> class default_delete{
public:
    static_assert(!std::is_function<_Tp>::value, "default_delete cannot be instantiated for function types");
    default_delete(){}
    void operator()(_Tp* __ptr) const noexcept{
        static_assert(!is_void<_Tp>::value, "can't delete pointer to incomplete type.");
        static_assert(sizeof(_Tp)>0, "can't delete pointer to incomplete type.");
        delete __ptr;
    }
};

template<typename _Tp, typename _Dp>
class __unique_ptr_impl{
    template<typename _Up, typename _Ep>
    struct Ptr{
        using type = _Up*;
    };
public:
    typedef _Tp* pointer;
    
    __unique_ptr_impl():_M_t(){}
    explicit __unique_ptr_impl(pointer _p):_M_t(){
        M_ptr() = _p;
    }
    __unique_ptr_impl(pointer _p, _Dp* _d):_M_t(_p, _d){}
    ~__unique_ptr_impl(){}
    
    pointer& M_ptr(){ return std::get<0>(_M_t); }
    _Dp& get_deleter(){ return std::get<1>(_M_t); }
private:
    tuple<pointer, _Dp> _M_t;
};

template<typename _Tp, typename _Dp=default_delete<_Tp>>
class unique_ptr{
private:
    typedef typename __unique_ptr_impl<_Tp, _Dp>::pointer pointer;
    typedef _Tp element_type;
    typedef _Dp deleter_type;
    __unique_ptr_impl<_Tp, _Dp> _M_t;
public:		
    unique_ptr():_M_t(){};
    unique_ptr(pointer _p):_M_t(){
        _M_t.M_ptr() = _p;
    }
    unique_ptr(_Dp _d):_M_t(){
        _M_t.get_deleter() = _d;
    }
    
    unique_ptr(pointer _p, _Dp _d):_M_t(){
        _M_t.M_ptr() = _p;
        _M_t.get_deleter() = _d;
    }
    unique_ptr(unique_ptr<_Tp, _Dp>& u) = delete;
    ~unique_ptr(){
        if(_M_t.M_ptr() != nullptr){
            _M_t.get_deleter()(_M_t.M_ptr());
            _M_t.M_ptr() = nullptr;
        }
    }
    bool operator =(unique_ptr<_Tp, _Dp>& u) = delete;
    operator bool(){
        return !(_M_t.M_ptr() == nullptr);
    }
    pointer& operator *(){
        return _M_t.M_ptr();
    }
    pointer operator ->(){
        return _M_t.M_ptr();
    }
    pointer& get(){
        return _M_t.M_ptr();
    }
    deleter_type& get_deleter(){
        return _M_t.get_deleter();
    }
    void swap(unique_ptr& u){
        pointer tmp = _M_t.M_ptr();
        _M_t.M_ptr() = u->get();
        u->get() = tmp;
    }
    pointer release(){
        pointer p = _M_t.M_ptr();
        _M_t.M_ptr() = nullptr;
        return p;
    }
    void reset(){
        _M_t.get_deleter()(_M_t.M_ptr());
        _M_t.M_ptr() = nullptr;
    }
    void reset(pointer q){
        if(q != nullptr)
            _M_t.M_ptr() = q;
        else
            _M_t.get_deleter()(_M_t.M_ptr());
    }
};

}

#endif /* unique_ptr_hpp */
