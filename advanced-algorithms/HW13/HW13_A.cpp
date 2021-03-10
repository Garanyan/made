//
// Created by Ovanes Garanyan on 6/4/20.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

const long double EPS = 1e-10;

struct Point{
    long double x;
    long double y;

    Point(long double x_, long double y_) : x(x_), y(y_) {};

    bool operator==(const Point &other) {
        return std::abs(this->x - other.x) < EPS && std::abs(this->y - other.y) < EPS;
    }


};

std::ostream &operator<<(std::ostream &out, const Point &p) {
    return out << std::setprecision(8) << p.x << ' ' << p.y;
}

struct Vector{
    long double x;
    long double y;

    Vector(long double x_, long double y_) : x(x_), y(y_) {}

    Vector(Point from, Point to) {
        x = to.x - from.x;
        y = to.y - from.y;
    }

    long double lenght() const {
        return std::sqrt(x * x + y * y);
    }

    long double sq_lenght() const {
        return x * x + y * y;
    }

    void normalize() {
        long double z = lenght();
        if (std::abs(z) > EPS){
            x /= z;
            y /= z;
        }
    }

    Vector operator*(long double val) {
        return {x * val, y * val};
    }

};

Point operator+(const Point &p, const Vector &v) {
    return {p.x + v.x, p.y + v.y};
}

struct Segment{
    Point A;
    Point B;

    Segment(Point a, Point b) : A(a), B(b) {};
};

long double dot_product(Vector a, Vector b) {
    return a.x * b.x + a.y * b.y;
}

long double cross_product(Vector a, Vector b) {
    return a.x * b.y - a.y * b.x;
}

int ccw(Point a, Point b, Point c) {
    Vector A(a, c);
    Vector B(a, b);

    long double cp = cross_product(A, B);
    if (cp > 0){
        return 1;
    }

    if (cp < 0){
        return -1;
    }

    return 0;
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

bool isCollinear(Vector A, Vector B) {
    return std::abs(cross_product(A, B)) < EPS;
}

bool isPointInSegment(Segment t, Point p) {
    Vector AB(t.A, t.B);
    Vector AP(t.A, p);
    if (!isCollinear(AB, AP)){
        return false;
    }

    Vector BP(t.B, p);
    Vector BA(t.B, t.A);
    return dot_product(AP, AB) >= EPS && dot_product(BP, BA) >= EPS;
}

struct Circle{
    long double radius;
    Point center;


    Circle(Point p, long double r) : center(p), radius(r) {};

    Circle(long double x, long double y, long double r) : center(x, y), radius(r) {};
};

struct Line{
    long double a;
    long double b;
    long double c;

    Line(long double a_, long double b_, long double c_) : a(a_), b(b_), c(c_) {

    }

    Line(Point p, Point q) {
        a = p.y - q.y;
        b = q.x - p.x;
        c = -a * p.x - b * p.y;
        normalize();
    }

    void normalize() {
        long double z = std::sqrt(a * a + b * b);
        if (std::abs(z) > EPS){
            a /= z;
            b /= z;
            c /= z;
        }
    }
};

std::vector<Point> CrossLineAndOriginCircle(Circle origin_circle, Line line) {
    long double denominator = (line.a * line.a + line.b * line.b);
    long double x0 = -line.a * line.c / denominator, y0 = -line.b * line.c / denominator;
    std::vector<Point> result;
    if (line.c * line.c > origin_circle.radius * origin_circle.radius * denominator + EPS){
        return result;
    }
    else if (abs(line.c * line.c - origin_circle.radius * origin_circle.radius * denominator) < EPS){
        Point p(x0, y0);
        result.emplace_back(p);
    }
    else{
        long double d = origin_circle.radius * origin_circle.radius - line.c * line.c / denominator;
        long double mult = std::sqrt(d / denominator);
        long double ax, ay, bx, by;
        ax = x0 + line.b * mult;
        bx = x0 - line.b * mult;
        ay = y0 - line.a * mult;
        by = y0 + line.a * mult;
        Point s(ax, ay);
        Point t(bx, by);
        result.emplace_back(s);
        result.emplace_back(t);
    }

    return result;
}

std::vector<Point> CrossLineAndCircle(Circle c, Line l) {
    Point origin(0, 0);
    Circle origin_circle = {origin, c.radius};
    Line line = {l.a, l.b, l.c + l.a * c.center.x + l.b * c.center.y};
    auto result = CrossLineAndOriginCircle(origin_circle, line);

    for (auto &p: result){
        p.x += c.center.x;
        p.y += c.center.y;
    }
    return result;
}

void printAns(Point &intersection_1, Point &intersection_2, Circle &a) {
    intersection_1.x += a.center.x;
    intersection_1.y += a.center.y;

    intersection_2.x += a.center.x;
    intersection_2.y += a.center.y;

    Point H((intersection_1.x + intersection_2.x) * 0.5, (intersection_1.y + intersection_2.y) * 0.5);

    std::cout << H.x << " " << H.y << std::endl;
    Vector OH(a.center, H);
    Vector HP(H, intersection_1);
    std::cout << OH.lenght() << " " << HP.lenght() << std::endl;
    std::cout << intersection_1 << std::endl;
    std::cout << intersection_2 << std::endl;
}

#define det(a, b, c, d)  (a*d-b*c)

void CircleCrossCircle(Circle a, Circle b) {
    std::cout << std::fixed << std::showpoint;
    if (a.center == b.center){
        if (std::abs(a.radius) < EPS && std::abs(b.radius) < EPS){
            std::cout << 1 << std::endl;
            std::cout << a.center << std::endl;
        }
        else if (std::abs(a.radius - b.radius) < EPS){
            std::cout << 3 << std::endl;
        }
        else{
            std::cout << 0 << std::endl;
        }
        return;
    }
    Vector O1O2(a.center, b.center);
    if (O1O2.sq_lenght() > (a.radius + b.radius) * (a.radius + b.radius)
        || O1O2.sq_lenght() < (a.radius - b.radius) * (a.radius - b.radius)){
        std::cout << 0 << std::endl;
        return;
    }

    if (std::abs(O1O2.sq_lenght() - (a.radius + b.radius) * (a.radius + b.radius)) < EPS){
        std::cout << 1 << std::endl;
        O1O2.normalize();
        Point one = a.center + O1O2 * a.radius;
        std::cout << one << std::endl;
        return;
    }

    if (std::abs(O1O2.sq_lenght() - (a.radius - b.radius) * (a.radius - b.radius)) < EPS){
        std::cout << 1 << std::endl;
        O1O2.normalize();
        if (a.radius < b.radius){
            // a in b
            Vector O2O1 = O1O2 * (-1);
            Point ans = a.center + O2O1 * a.radius;
            std::cout << ans << std::endl;
            return;
        }

        // b in a
        Point one = b.center + O1O2 * b.radius;
        std::cout << one << std::endl;
        return;
    }

    std::cout << 2 << std::endl;
    // дальше 2 точки пересечения
    Line l = {
            -2 * O1O2.x,
            -2 * O1O2.y,
            O1O2.x * O1O2.x + O1O2.y * O1O2.y - b.radius * b.radius + a.radius * a.radius
    };

    if (std::abs(l.a) < EPS){
        double y = -l.c / l.b;
        Point intersection_1(std::sqrt(a.radius * a.radius - y * y), y);
        Point intersection_2(-std::sqrt(a.radius * a.radius - y * y), y);
        printAns(intersection_1, intersection_2, a);
        return;;

    }

    if (std::abs(l.b) < EPS){
        double x = -l.c / l.a;
        Point intersection_1(x, std::sqrt(a.radius * a.radius - x * x));
        Point intersection_2(x, -std::sqrt(a.radius * a.radius - x * x));
        printAns(intersection_1, intersection_2, a);
        return;
    }


    {
        long double A, B, C;
        A = l.a * l.a + l.b * l.b;
        B = 2 * l.a * l.c;
        C = l.c * l.c - a.radius * a.radius * l.b * l.b;
        long double D = B * B - 4 * A * C;
        double x1 = (-B + std::sqrt(D)) / (2 * A);
        double x2 = (-B - std::sqrt(D)) / (2 * A);
        Point intersection_1(x1, (-l.a * x1 - l.c) / l.b);
        Point intersection_2(x2, (-l.a * x2 - l.c) / l.b);
        printAns(intersection_1, intersection_2, a);

    }
}

int main() {
    size_t test_num;
    std::cin >> test_num;
    for (size_t i = 0; i < test_num; ++i){
        int x, y, r;
        std::cin >> x >> y >> r;
        Circle one(x, y, r);
        std::cin >> x >> y >> r;
        Circle two(x, y, r);

        CircleCrossCircle(one, two);
    }
    return 0;
}
