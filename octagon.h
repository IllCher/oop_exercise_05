#ifndef OOP_EXERCISE_05_OCTAGON_H
#define OOP_EXERCISE_05_OCTAGON_H
#include <utility>
#include <cmath>
template<class T>
struct TOctagon{
    using type = T;
    using vertex = std::pair<T,T>;
    vertex A, B, C, D, E, F, G, H;
    TOctagon() :
        A(0,0), B(0,0), C(0,0), D(0,0), E(0,0), F(0,0), G(0,0), H(0,0)
    {}
    TOctagon(T x1, T y1, T x2, T y2, T x3, T y3, T x4, T y4, T x5, T y5, T x6, T y6, T x7, T y7, T x8, T y8) :
            A(x1, y1), B(x2, y2), C(x3, y3), D(x4, y4), E(x5, y5), F(x6, y6), G(x7, y7), H(x8, y8)
    {}
    TOctagon(vertex a, vertex b, vertex c, vertex d, vertex e, vertex f, vertex g, vertex h) : A(a), B(b), C(c), D(d), E(e), F(f), G(g), H(h)
    {}
    std::pair<double,double> center() const;
    void print() const;
    double area() const;
};


template <class T>

double TOctagon<T>::area() const{
    return fabs(((A.first * B.second) + (B.first * C.second) + (C.first * D.second) + (D.first * E.second) + (E.first * F.second) + (F.first * G.second) + (G.first * H.second) + (H.first * A.second) - (B.first * A.second) - (C.first * B.second) - (D.first * C.second) - (E.first * D.second) - (F.first * E.second) - (G.first * F.second) - (H.first * G.second) - (A.first * H.second)) * 0.5);
}

template <class T>
void TOctagon<T>::print() const{
    std::cout << A << " " << B << " " << C << " " << D << " " << E << " " << F << " " << G << " " << H << "\n";
}

template <class T>

std::pair<double, double> TOctagon<T>::center() const{
    return std::make_pair(static_cast<double>(A.first + B.first + C.first + D.first + E.first + F.first + G.first + H.first) / 8,static_cast<double>(A.second + B.second + C.second + D.second + E.second + F.second + G.second + H.second) / 8);
}
#endif //OOP_EXERCISE_05_OCTAGON_H
