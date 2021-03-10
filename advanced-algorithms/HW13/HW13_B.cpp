//
// Created by Ovanes Garanyan on 6/6/20.
//
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <cassert>
#include <limits>
#include <algorithm>
#include <cfloat>
#include <set>

const double EPS = 1e-10;

struct Point{
    double x;
    double y;

    Point(double x_, double y_) : x(x_), y(y_) {};

    bool operator==(const Point &other) {
        return std::abs(this->x - other.x) < EPS && std::abs(this->y - other.y) < EPS;
    }


};

std::ostream &operator<<(std::ostream &out, const Point &p) {
    return out << std::setprecision(8) << p.x << ' ' << p.y;
}


struct Vector{
    double x;
    double y;

    Vector(Point from, Point to) {
        x = to.x - from.x;
        y = to.y - from.y;
    }

    double lenght() const {
        return std::sqrt(x * x + y * y);
    }

    void normalize() {
        long double z = std::sqrt(lenght());
        if (std::abs(z) > EPS){
            x /= z;
            y /= z;
        }
    }

};

double dot_product(Vector a, Vector b) {
    return a.x * b.x + a.y * b.y;
}

double cross_product(Vector a, Vector b) {
    return a.x * b.y - a.y * b.x;
}

double cross_product(Point a, Point b) {
    Point origin(0, 0);
    Vector A(origin, a);
    Vector B(origin, b);
    return cross_product(A, B);
}

double det(double a, double b, double c, double d) {
    return a * d - b * c;
}

struct Line{
    double a;
    double b;
    double c;

    Line(double a_, double b_, double c_) : a(a_), b(b_), c(c_) {
        //normalize();
    }

    Line(Point p, Point q) {
        a = p.y - q.y;
        b = q.x - p.x;
        c = -a * p.x - b * p.y;
        //normalize();
    }

    void normalize() {
        double z = std::sqrt(a * a + b * b);
        if (std::abs(z) > EPS){
            a /= z;
            b /= z;
            c /= z;
        }

    }

    bool inPosSide(const Point &p) {
        auto val = p.x * a + p.y * b + c;
        return val >= -EPS;
    }

    Point intersect(Line &other, bool &status) {
        double zn = det(a, b, other.a, other.b);
        if (std::abs(zn) < EPS){
            status = false;
            return Point(0, 0);
        }
        double x = -det(c, b, other.c, other.b) / zn;
        double y = -det(a, c, other.a, other.c) / zn;
        status = true;
        return Point(x, y);
    }


    Point intersect2(Line &other, bool &status) {
        double zn = det(a, b, other.a, other.b);
        if (std::abs(zn) < EPS){
            status = false;
            return Point(0, 0);
        }
        double x = 0, y = 0;
        if (std::abs(other.b) < EPS){
            x = other.c / other.a;
        }
        else if (std::abs(b) < EPS){
            x = c / a;
        }
        else{
            x = -det(c, b, other.c, other.b) / zn;
        }

        if (std::abs(other.a) < EPS){
            y = other.c / other.b;
        }
        else if (std::abs(a) < EPS){
            y = c / b;
        }
        else{
            y = -det(c, b, other.c, other.b) / zn;
        }


        status = true;
        return Point(x, y);

    }
};

class Poligon{
public:
    Poligon();

    void CutByLine(Line line);

    double Area();

    double Area2();

private:
    std::vector<Point> points_;
};

Poligon::Poligon() {
    int max = 1000000000;
    int min = -max;
    Point a(min, min);
    points_.push_back(a);

    Point e(0, min);
    points_.push_back(e);

    Point b(max, min);
    points_.push_back(b);

    Point f(max, 0);
    points_.push_back(e);

    Point c(max, max);
    points_.push_back(c);

    Point d(min, max);
    points_.push_back(d);

}

void Poligon::CutByLine(Line line) {
    std::vector<Point> points;
    for (size_t i = 0; i < points_.size(); ++i){
        const auto &p = points_[i];
        if (line.inPosSide(p)){
            points.push_back(p);
        }

        if (i + 1 < points_.size()){
            if ((line.inPosSide(p) && !line.inPosSide(points_[i + 1])) ||
                (!line.inPosSide(p) && line.inPosSide(points_[i + 1]))){
                // добавить пересечение
                auto a = p.y - points_[i + 1].y;
                auto b = points_[i + 1].x - p.x;
                Line seg(a, b, -a * p.x - b * p.y);
                bool status = false;
                Point intersection = line.intersect(seg, status);
                if (status){
                    points.push_back(intersection);
                }
            }
        }
        else{
            if ((line.inPosSide(p) && !line.inPosSide(points_[0])) ||
                (!line.inPosSide(p) && line.inPosSide(points_[0]))){
                // добавить пересечение
                auto a = p.y - points_[0].y;
                auto b = points_[0].x - p.x;
                Line seg(a, b, -a * p.x - b * p.y);
                bool status = false;
                Point intersection = line.intersect(seg, status);
                if (status){
                    points.push_back(intersection);
                }
            }
        }
    }

    points_ = points;

}

double Poligon::Area() {
    double area = 0;
    Point origin(0, 0);
    for (size_t i = 0; i < points_.size(); ++i){
        if (i + 1 < points_.size()){
            Vector A(origin, points_[i]);
            Vector B(origin, points_[i + 1]);
            area += cross_product(A, B) / 2;
        }
        else{
            Vector A(origin, points_[i]);
            Vector B(origin, points_[0]);
            area += cross_product(A, B) / 2;
        }
    }
    return std::abs(area);
}

double Poligon::Area2() {
    double res = 0;
    for (unsigned i = 0; i < points_.size(); i++){
        Point p1 = i ? points_[i - 1] : points_.back();
        Point p2 = points_[i];
        res += (p1.x - p2.x) * (p1.y + p2.y);
    }
    return std::abs(res) / 2;
}

int ccw(Vector A, Vector B) {
    long double cp = cross_product(A, B);
    if (cp > EPS){
        return 1;
    }

    if (cp + EPS < 0){
        return -1;
    }

    return 0;
}

bool cmp(const Line &a, const Line &b) {
    Point origin(0, 0);
    Point A(a.a, a.b);
    Vector OA(origin, A);

    Point B(b.a, b.b);
    Vector OB(origin, B);
    return ccw(OA, OB);
}

int main() {
    size_t num;
    std::cin >> num;
    Poligon pol;
    std::vector<Line> line_set;
    for (size_t i = 0; i < num; ++i){
        int a, b, c;
        std::cin >> a >> b >> c;
        Line l(a, b, c);
        line_set.push_back(l);

        if (std::abs(b) < EPS && std::abs(a) > EPS && c < 0){
            std::cout << std::fixed << std::showpoint << std::setprecision(8) << 0 << std::endl;
            return 0;
        }
    }

    std::sort(line_set.begin(), line_set.end(), cmp);

    for (auto &l: line_set){
        pol.CutByLine(l);
    }

    std::cout << std::fixed << std::showpoint << std::setprecision(8) << pol.Area2() << std::endl;
    return 0;
}
