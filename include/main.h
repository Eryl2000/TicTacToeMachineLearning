#ifndef MAIN_H
#define MAIN_H

#include <ctime>
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::string;

template <class T>
T clamp(const T &value, const T &min, const T &max) {
  if (value > max) {
    return max;
  } else if (value < min) {
    return min;
  } else {
    return value;
  }
}

#endif