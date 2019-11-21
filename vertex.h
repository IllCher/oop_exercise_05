#ifndef VERTEX_H
#define VERTEX_H
#include <iostream>
template <typename T1, typename T2>
std::istream& operator>> (std::istream& is, std::pair<T1, T2>& p) {
    is >> p.first >> p.second;
    if (is.fail()) {
        throw std::logic_error("Wrong type");
    }
    return is;
}

template <typename T1, typename T2>
std::ostream& operator<< (std::ostream& out, const std::pair<T1, T2>& p) {
    out << "(" << p.first << ", " << p.second << ") ";
    return out;
}

template<class T>
std::pair<T,T> operator+(std::pair<T,T> lhs, std::pair<T,T> rhs){
    std::pair<T,T> res;
    res.first = lhs.first + rhs.first;
    res.second = lhs.second + rhs.second;
    return res;
}

template<class T>
std::pair<T, T> operator/=(std::pair<T,T> vertex, double val) {
    vertex.first = vertex.first / val;
    vertex.second = vertex.second / val;
    return vertex;
}
#endif //VERTEX_H
