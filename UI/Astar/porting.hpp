/*
 * For porting the code using STL to different libraries and frameworks.
 */ 
#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <QString>
#include <QtDebug>
#define OUTPUT qInfo()
#define endline Qt::endl

namespace porting
{
    template<typename T> using vector = std::vector<T>;
    template<typename T> using less   = struct std::less<T>;
    template<typename T> using hash   = struct std::hash<T>;
    template<typename T, typename U> using unordered_map  = std::unordered_map<T,U>;
    template<typename T, typename U> using priority_queue = std::priority_queue<T,U>;
    using string = QString;
}
