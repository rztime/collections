//
//  Iterator.inl
//  collection_codes
//
//  Created by hejunqiu on 2017/2/27.
//  Copyright © 2017年 CHE. All rights reserved.
//

#ifndef iterator_INL
#define iterator_INL

#include "defines.h"
#include <type_traits>
#include <stddef.h>

CC_BEGIN

template <class _Tp>
struct iterator_traits { };

template <class _Tp>
struct iterator_traits<_Tp*>
{
    typedef ptrdiff_t difference_type;
    typedef typename std::remove_const<_Tp>::type value_type;
    typedef _Tp* pointer;
    typedef _Tp& reference;
};

template <typename _Iter>
class iterator {
    _Iter current;
public:
    typedef _Iter iterator_type;
    typedef typename iterator_traits<_Iter>::difference_type difference_type;
    typedef typename iterator_traits<_Iter>::value_type      value_type;
    typedef typename iterator_traits<_Iter>::reference       reference;
    typedef typename iterator_traits<_Iter>::pointer         pointer;

    iterator() _NOEXCEPT : current() {}
    explicit iterator(iterator_type __x) _NOEXCEPT : current(__x) {}

    iterator_type base() const _NOEXCEPT { return current; }
    reference operator*() const _NOEXCEPT { return *current; }
    pointer operator->() const _NOEXCEPT { ::std::addressof(operator*()); }

    iterator& operator++() _NOEXCEPT
    {
        ++current;
        return *this;
    }

    iterator operator++(int) _NOEXCEPT
    {
        iterator_type tmp(this);
        ++(*this);
        return tmp;
    }

    iterator& operator--() _NOEXCEPT
    {
        --current;
        return *this;
    }

    iterator operator--(int) _NOEXCEPT
    {
        iterator_type tmp(this);
        --(*this);
        return tmp;
    }

    iterator operator+(difference_type n) const _NOEXCEPT
    {
        iterator __w(*this);
        __w += n;
        return __w;
    }

    iterator& operator+=(difference_type n) _NOEXCEPT
    {
        current += n;
        return *this;
    }

    reference operator[](difference_type n) const _NOEXCEPT
    { return current[n]; }

    template<typename _Iter1, typename _Iter2>
    friend bool operator==(const iterator<_Iter1> &x, const iterator<_Iter2> &y)
    { return x.base() == y.base(); }

    template<typename _Iter1, typename _Iter2>
    friend bool operator<(const iterator<_Iter1> &x, const iterator<_Iter2> &y)
    { return x.base() < y.base(); }

    template<typename _Iter1, typename _Iter2>
    friend bool operator!=(const iterator<_Iter1> &x, const iterator<_Iter2> &y)
    { return !(x == y); }

    template<typename _Iter1, typename _Iter2>
    friend bool operator>(const iterator<_Iter1> &x, const iterator<_Iter2> &y)
    { return y < x; }

    template<typename _Iter1, typename _Iter2>
    friend bool operator>=(const iterator<_Iter1> &x, const iterator<_Iter2> &y)
    { return !(x < y); }

    template<typename _Iter1, typename _Iter2>
    friend bool operator<=(const iterator<_Iter1> &x, const iterator<_Iter2> &y)
    { return !(y < x); }
};

template <typename _Iter>
class reverse_iterator {
    _Iter current;
public:
    typedef _Iter                                            iterator_type;
    typedef typename _Iter::difference_type difference_type;
    typedef typename _Iter::reference       reference;
    typedef typename _Iter::pointer         pointer;

    reverse_iterator() _NOEXCEPT : current() {}
    explicit reverse_iterator(_Iter __x) _NOEXCEPT : current(__x) {}
//    template <class _Up> reverse_iterator(const reverse_iterator<_Up>& __u)
//    : current(__u.base()) {}

    _Iter base() const _NOEXCEPT {return current;}
    reference operator*() const _NOEXCEPT {_Iter __tmp = current; return *--__tmp;}
    pointer  operator->() const _NOEXCEPT {return _VSTD::addressof(operator*());}

    reverse_iterator& operator++() _NOEXCEPT
    {--current; return *this;}

    reverse_iterator  operator++(int) _NOEXCEPT
    {reverse_iterator __tmp(*this); --current; return __tmp;}

    reverse_iterator& operator--() _NOEXCEPT
    {++current; return *this;}

    reverse_iterator  operator--(int) _NOEXCEPT
    {reverse_iterator __tmp(*this); ++current; return __tmp;}

    reverse_iterator  operator+ (difference_type __n) const _NOEXCEPT
    {return reverse_iterator(current - __n);}

    reverse_iterator& operator+=(difference_type __n) _NOEXCEPT
    {current -= __n; return *this;}

    reverse_iterator  operator- (difference_type __n) const _NOEXCEPT
    {return reverse_iterator(current + __n);}

    reverse_iterator& operator-=(difference_type __n) _NOEXCEPT
    {current += __n; return *this;}

    reference         operator[](difference_type __n) const _NOEXCEPT
    {return *(*this + __n);}
};

CC_END

#endif /* iterator_INL */
