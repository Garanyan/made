#include <vector>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <random>
#include <iomanip>
#include <iostream>

struct Point{

    double x;

    double y;

    Point subtract(const Point &p) const;

    double distance(const Point &p) const;

    double cross(const Point &p) const;
};


class Circle{

public:
    static const Circle INVALID;

    Point c;

    double r;

    bool contains(const Point &p) const;

private:
    static const double MULTIPLICATIVE_EPSILON;

};

const Circle Circle::INVALID{Point{0, 0}, -1};

const double Circle::MULTIPLICATIVE_EPSILON = 1 + 1e-14;

Point Point::subtract(const Point &p) const {
    return Point{x - p.x, y - p.y};
}


double Point::distance(const Point &p) const {
    return std::hypot(x - p.x, y - p.y);
}

double Point::cross(const Point &p) const {
    return x * p.y - y * p.x;
}

bool Circle::contains(const Point &p) const {
    return c.distance(p) <= r * MULTIPLICATIVE_EPSILON;
}


static Circle smallestEnclosingCircleOnePoint(const std::vector<Point> &points, size_t end, const Point &p);

static Circle
smallestEnclosingCircleTwoPoints(const std::vector<Point> &points, size_t end, const Point &p, const Point &q);

static std::default_random_engine randGen((std::random_device()) ());


// Initially: No boundary points known
Circle smallestEnclosingCircle(const std::vector<Point> &points) {
    std::vector<Point> shuffled = points;
    std::shuffle(shuffled.begin(), shuffled.end(), randGen);

    Circle c = Circle::INVALID;
    for (size_t i = 0; i < shuffled.size(); i++){
        const Point &p = shuffled.at(i);
        if (c.r < 0 || !c.contains(p))
            c = smallestEnclosingCircleOnePoint(shuffled, i + 1, p);
    }
    return c;
}

Circle makeCircleOnDiameter(const Point &a, const Point &b) {
    Point c{(a.x + b.x) / 2, (a.y + b.y) / 2};
    return Circle{c, std::max(c.distance(a), c.distance(b))};
}

// One boundary point known
static Circle smallestEnclosingCircleOnePoint(const std::vector<Point> &points, size_t end, const Point &p) {
    Circle c{p, 0};
    for (size_t i = 0; i < end; i++){
        const Point &q = points.at(i);
        if (!c.contains(q)){
            if (c.r == 0){
                c = makeCircleOnDiameter(p, q);
            }
            else{
                c = smallestEnclosingCircleTwoPoints(points, i + 1, p, q);
            }
        }
    }
    return c;
}


Circle makeCircumcircle(const Point &a, const Point &b, const Point &c) {
    double ox = (std::min(std::min(a.x, b.x), c.x) + std::max(std::max(a.x, b.x), c.x)) / 2;
    double oy = (std::min(std::min(a.y, b.y), c.y) + std::max(std::max(a.y, b.y), c.y)) / 2;
    double ax = a.x - ox, ay = a.y - oy;
    double bx = b.x - ox, by = b.y - oy;
    double cx = c.x - ox, cy = c.y - oy;
    double d = (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by)) * 2;
    if (d == 0){
        return Circle::INVALID;
    }
    double x =
            ((ax * ax + ay * ay) * (by - cy) + (bx * bx + by * by) * (cy - ay) + (cx * cx + cy * cy) * (ay - by)) / d;
    double y =
            ((ax * ax + ay * ay) * (cx - bx) + (bx * bx + by * by) * (ax - cx) + (cx * cx + cy * cy) * (bx - ax)) / d;
    Point p{ox + x, oy + y};
    double r = std::max(std::max(p.distance(a), p.distance(b)), p.distance(c));
    return Circle{p, r};
}


static Circle
smallestEnclosingCircleTwoPoints(const std::vector<Point> &points, size_t end, const Point &p, const Point &q) {
    Circle circ_ = makeCircleOnDiameter(p, q);
    Circle left = Circle::INVALID;
    Circle right = Circle::INVALID;

    Point pq = q.subtract(p);
    for (size_t i = 0; i < end; i++){
        const Point &r = points.at(i);
        if (circ_.contains(r)){
            continue;
        }

        double cross = pq.cross(r.subtract(p));
        Circle c = makeCircumcircle(p, q, r);
        if (c.r < 0){
            continue;
        }
        else if (cross > 0 && (left.r < 0 || pq.cross(c.c.subtract(p)) > pq.cross(left.c.subtract(p)))){
            left = c;
        }
        else if (cross < 0 && (right.r < 0 || pq.cross(c.c.subtract(p)) < pq.cross(right.c.subtract(p)))){
            right = c;
        }
    }

    if (left.r < 0 && right.r < 0){
        return circ_;
    }
    else if (left.r < 0){
        return right;
    }
    else if (right.r < 0){
        return left;
    }
    else{
        if (left.r <= right.r){
            return left;
        }

        return right;
    }
}


int main() {
    size_t points_num;
    std::cin >> points_num;
    std::vector<Point> points;
    double x, y;
    for (size_t i = 0; i < points_num; ++i){

        std::cin >> x >> y;
        points.push_back({x, y});
    }

    Circle enclosingCircle = smallestEnclosingCircle(points);

    std::cout << std::fixed << std::showpoint << std::setprecision(8) << enclosingCircle.c.x
              << " " << enclosingCircle.c.y << "\n" << enclosingCircle.r
              << std::endl;
    return 0;
}
