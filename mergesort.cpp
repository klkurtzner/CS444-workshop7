/*
Name: Katy Kurtzner
Email: kkurtzner@crimson.ua.edu
Course Section: Fall 2024 CS 201
Homework #: 0
*/

#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

template< class RandomIt >
constexpr void mergesort(RandomIt first, RandomIt last, RandomIt tempstart){
    auto len=std::distance(first, last);
    if (len<=1) {
        return;
    }
    auto mid=std::next(first, len/2);

    mergesort(first, mid, tempstart);
    mergesort(mid, last, tempstart);

    RandomIt tempEnd=tempstart;
    RandomIt left=first;
    RandomIt right=mid;

    while (left !=mid &&right !=last){
        if(*left<*right){
            *tempEnd++=std::move(*left++);
        }
        else{
            *tempEnd++=std::move(*right++);
        }
    }

    std::copy(left, mid, tempEnd);
    std::copy(right, last, tempEnd);
    std::copy(tempstart, tempstart+len, first);
}
