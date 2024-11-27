#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<algorithm>
#include "glut.h"

using namespace std;
double Pi = 3.1415926535;

class Point {
public:
    double x, y;

    Point() {
        x = 0;
        y = 0;
    }

    Point(double _x, double _y) :x(_x), y(_y) {}

    void Display() {
        std::cout << "(" << x << ", " << y << ")" << '\n';
    }

    Point operator=(Point pnt) {
        x = pnt.x;
        y = pnt.y;
        return *this;
    }

    friend bool operator ==(const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
    friend bool operator !=(const Point& a, const Point& b) { return !(a == b); }

    Point& operator+=(const Point& P) {
        x += P.x;
        y += P.y;
        return *this;
    }
    Point& operator-=(const Point& P) {
        x -= P.x;
        y -= P.y;
        return *this;
    }
    Point operator/(const double k)  const {
        return Point(x / k, y / k);
    }
    Point operator*(const double k) const {
        return Point(x * k, y * k);
    }
    Point operator+(const Point& P2) const {
        Point tmp = *this;
        return (tmp += P2);
    }
    Point operator-() const {
        return Point(-x, -y);
    }
    Point operator-(const Point& P2) const {
        Point tmp = *this;
        return (tmp += -P2);
    }
    Point rotate(Point P, double degree)
    {
        double deg = degree / 57.29577951308, x, y;
        x = P.x + (this->x - P.x) * cosf(deg) - (this->y - P.y) * sinf(deg);
        y = P.y + (this->x - P.x) * sinf(deg) + (this->y - P.y) * cosf(deg);
        return Point(x, y);
    }
};

class Line 
{
public:
    Point p1;
    Point p2;
    double k;
    double b;
    Line() : k(0), b(0) {}
    Line(Point P1, Point P2) : p1(P1), p2(P2), k((P2.y - P1.y) / (P2.x - P1.x)), b(-k * P1.x + P1.y) {}
    Line(double k, double b) : k(k), b(b) {}
    Line(Point P1, double coe) : p1(P1), k(coe), b(-k * P1.x + P1.y), p2(Point(P1.x + 1, k* (P1.x + 1) + b)) {}

    bool operator==(const Line another) {return (another.k == this->k && another.b == this->b); }
    bool operator!=(const Line another) {return !(*this == another);}


    double Get_coiffecient() { return k;}
    double Get_slope() {return b;}
    void Display() 
    {
        cout << " y = " << k << " * x ";
        if (b > 0) {cout << "+ " << b;}
        else {cout << b;}
        cout << '\n';
    }
};

class Shape 
{
public:
    Shape() {}
    const double EPS = 0.0001;

    virtual bool operator==(const Shape& another) const = 0;
    virtual bool operator!=(const Shape& another) const = 0;
    virtual bool isCongruentTo(const Shape& another) const = 0;
    virtual bool isSimilarTo(const Shape& another) const = 0;
    virtual bool containsPoint(const Point& point) const = 0;
    virtual void rotate(Point center, double deg) = 0;
    virtual void reflex(Point center) = 0;
    virtual void reflex(Line axis) = 0;
    virtual void scale(Point center, const double& coefficient) = 0;
    virtual double area() const = 0;
    virtual double perimetr() const = 0;
    virtual void glutDraw() = 0;
    virtual string getInfo() = 0;

    Point Scale(const Point center, const Point& P, const double& coefficient) const {return center + (P - center) * coefficient;}
    Point Reflex(const Point& P, const Point& center) {return center * 2 - P;}
    Point Reflex(const Point& P, const Line& L) {Line line = perpendicular(L, P);Point center = intersect(line, L);return Reflex(P, center);}
    Point Rotate(const Point& center, const Point& P, const double deg) 
    {double x = P.x - center.x;double y = P.y - center.y;double fi = Pi * deg / 180;return center + Point(x * cos(fi) - y * sin(fi), x * sin(fi) + y * cos(fi));}
    Line perpendicular(const Line& L, const Point& P1) {Point P2 = Point(P1.x + L.k, P1.y + (-1));return Line(P1, P2);}
    Point intersect(const Line& L1, const Line& L2) const 
    {
        double a = -L1.b * (-1) + L2.b * (-1);
        double b = -L1.k * L2.b + L2.k * L1.b;
        double det = Det(L1, L2);
        return Point(a / det, b / det);
    }

    double Det(const Line& L1, const Line& L2) const {return L1.k * (-1) - (-1) * L2.k;}
    double Det(double a, double a1, double b, double b1) const {return a * b1 - b * a1;}
    double cross_product(const Point P1, const Point P2) const {return P2.y * P1.x - P2.x * P1.y;}

    double Sin(const Point P1, const Point P2) const {return (P2.y * P1.x - P2.x * P1.y) / (sqrt(P1.x * P1.x + P1.y * P1.y) * sqrt(P2.x * P2.x + P2.y * P2.y));}
    double Sin(const Line L1, const Line L2) const {return Sin(Point(-1, L1.k), Point(-1, L2.k));}
    double Cos(const Point P1, const Point P2) const {return (P1.x * P2.x + P1.y * P2.y) / (sqrt(P1.x * P1.x + P1.y * P1.y) * sqrt(P2.x * P2.x + P2.y * P2.y));}
    double Cos(const Line L1, const Line L2) const {return Cos(Point(-1, L1.k), Point(-1, L2.k));}

    double Sin2(const Line L1, const Line L2) const {return 2 * Sin(L1, L2) * Cos(L1, L2);}

    double dist(const Point& P1, const Point& P2) const {return sqrt((P1.x - P2.x) * (P1.x - P2.x) + (P1.y - P2.y) * (P1.y - P2.y));}
    bool onLine(const Line& L1, const Point& p) const 
    {
        if (p.y == L1.k * p.x + L1.b) {return true;}
        else {return false;}
    }
    int direction(const Point& a, const Point& b, const Point& c) const 
    {
        int val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
        if (val == 0) {return 0;}
        else if (val < 0) {return 2;}
        return 1;
    }
    bool isIntersect(const Line& l1, const Line& l2) const
    {
        if (l1.k == l2.k && l1.b != l2.b)return false;
        return true;
    }
    bool equal(double& a, double& b) const {return abs(a - b) < EPS;}
    ~Shape() {}
};



class Polygon : public Shape 
{
public:
    int size_;
    std::vector<Point> points;

    Polygon() {size_ = 0;};
    Polygon(const std::vector<Point>& g) {size_ = g.size();points = g;}
    Polygon& operator=(const Polygon& P) 
    {
        size_ = P.size_;
        for (int i = 0; i < size_; ++i) {points[i] = P.points[i];}
        return *this;
    }

    double perimetr() const 
    {
        double Perimeter = 0;
        for (int i = 0; i < size_; ++i) {Perimeter += dist(Prev(i), points[i]);}
        return Perimeter;
    }
    double area() const 
    {
        double s = 0;
        for (int i = 0; i < size_; ++i) {s += Det(points[i].x, points[i].y, Next(i).x, Next(i).y);}
        return 0.5 * abs(s);
    }

    Point Next(int pos) const {return points[(pos + 1) % size_];}
    Point Prev(int pos) const {return  points[(pos - 1 + size_) % size_];}
    int verticesCount() const {return size_;}
    std::vector<Point> getVertices() const {return points;}
    Point centroid() const 
    {
        double x = 0;
        double y = 0;
        for (int i = 0; i < size_; ++i) {x += points[i].x;y += points[i].y;}
        return Point(x / size_, y / size_);
    }

    bool operator==(const Shape& another) const 
    {
        if (dynamic_cast<const Polygon*>(&another) == NULL)return false;
        const Polygon& Another = dynamic_cast<const Polygon&>(another);
        if (size_ != Another.size_ || this->points != Another.points)return false;
        else return true;
    }
    bool operator!=(const Shape& another) const {return !(*this == another);}
    bool isCongruentTo(const Shape& another) const {
        if (*this == another) {return true;}
        if (dynamic_cast<const Polygon*>(&another) == NULL) {return false;}
        const Polygon& Another = dynamic_cast<const Polygon&>(another);
        if (size_ != Another.size_) {return false;}
        if (!isSimilarTo(another)) {return false;}
        bool flag = true;
        for (int i = 0; i < size_; ++i) 
        {
            for (int j = 0; j < size_; ++j) 
            {
                if (dist(points[i], Next(i)) != dist(Another.points[i], Another.Next(i))) {flag = false;break;}
            }
            if (flag == true) {break;}
        }
        return flag;
    }
    bool isSimilarTo(const Shape& another) const 
    {
        if (*this == another) return true;
        if (dynamic_cast<const Polygon*>(&another) == NULL)return false;
        const Polygon& Another = dynamic_cast<const Polygon&>(another);
        if (size_ != Another.size_)return false;
        double k = perimetr() / Another.perimetr();
        bool check = true;
        for (int i = 0; i < size_; ++i) 
        {
            bool check = true;
            for (int j = 0; j < Another.size_; ++j) 
            {
                if (dist(points[i], Next(i)) / dist(Another.points[j], Another.Next(j)) != k) {check = false;break;
                }
            }
            if (check) { break;}
        }
        if (!check) {return false;}

        // Проверка углов
        for (int i = 0; i < size_; ++i) 
        {
            double a = abs(Sin(Line(points[i], Next(i)), Line(points[i], Prev(i))));
            check = true;
            for (int j = 0; j < Another.size_; ++j) 
            {
                double b = abs(Sin(Line(Another.points[j], Another.Next(j)), Line(Another.points[j], Another.Prev(j))));
                if (!equal(a, b)) {check = false;break;}
            }
            if (check) {break;}
        }
        return check;
    }

    bool containsPoint(const Point& point) const 
    {
        if (size_ < 3) { return false;}
        Line exline = { point, { INT16_MAX, point.y } };
        int count = 0;
        int i = 0;
        while (i != 0) {
            Line side = { points[i], Next(i) };
            if (isIntersect(side, exline)) 
            {
                if (direction(side.p1, point, side.p2) == 0)return onLine(side, point);
                count++;
            }
            i = (i + 1) % size_;
        }
        if (count % 2 == 0) {return true;}
        else {return false;}
    }
    bool isConvex() 
    {
        double check = 0;
        for (int i = 0; i < size_; ++i) 
        {
            double check_2 = cross_product(Next(i) - points[i], Prev(i) - points[i]);
            if (check * check_2 < 0) {return false;}
            check = check_2;
        }
        return true;
    }


    void rotate(Point center, double deg) 
    {
        for (int i = 0; i < size_; ++i) {points[i] = Rotate(center, points[i], deg);}
    }
    void reflex(Point center) 
    {
        for (int i = 0; i < size_; ++i) {points[i] = Reflex(points[i], center);}
    }
    void reflex(Line axis) 
    {
        for (int i = 0; i < size_; ++i) {points[i] = Reflex(points[i], axis);}
    }
    void scale(Point center, const double& coefficient) 
    {
        for (int i = 0; i < size_; ++i) {points[i] = Scale(center, points[i], coefficient);}
    }
    void glutDraw()
    {
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.5f);
        for (int i = 1; i < points.size(); i++) 
        {
            glVertex2f(points[i - 1].x / 100, points[i - 1].y / 100);
            glVertex2f(points[i].x / 100, points[i].y / 100);
        }
        glVertex2f(points[points.size() - 1].x / 100, points[points.size() - 1].y / 100);
        glVertex2f(points[0].x / 100, points[0].y / 100);
        glEnd();
    }
    string getInfo()
    {
        string str = "";
        if (this->points.size() > 0) {str += "" + to_string(points[0].x) + ", " + to_string(points[0].y) + "";}
        return str;
    }


    void Display() 
    {
        for (int i = 0; i < size_; ++i) {points[i].Display();}
    }
};


class Ellipse : public Shape {
public:
    Point A, B;
    double a, b, c, d;

    Ellipse() {A = Point(0, 0);B = Point(0, 0);a = 0;b = 0;c = 0;d = 0;}
    Ellipse(const Point P1, const Point P2, double D) {A = P1;B = P2;d = D;a = D / 2;c = dist(P1, P2) / 2;b = sqrtf(abs(a * a - c * c));}
    virtual ~Ellipse() = default;
    Ellipse& operator=(const Ellipse& E) {A = E.A;B = E.B;d = E.d;return *this;}
    double perimetr() const {return (4 * Pi * a * b + (a - b) * 2) / (a + b);}
    double area() const {return Pi * d * sqrt(d * d - dist(A, B) * dist(A, B)) / 4;}

    Point center() const {return (A + B) / 2;}
    std::pair<Point, Point> focuses() const {return std::make_pair(A, B);}
    std::pair<Line, Line> directrices() 
    {
        double distance = dist(A, B);
        double D = d / (2 * eccentricity());
        double a = B.x - A.x;
        double b = B.y - A.y;
        double x1 = A.x + a * (D + distance / 2) / distance;
        double y1 = A.y + b * (D + distance / 2) / distance;
        double x2 = B.x - a * (D + distance / 2) / distance;
        double y2 = B.y - b * (D + distance / 2) / distance;
        Point p1(x1, y1);
        Point p2(x2, y2);
        x1 = x1 - b;
        x2 = x2 - b;
        y1 = y1 + a;
        y2 = y2 + a;
        Point p3(x1, y1);
        Point p4(x2, y2);
        return std::make_pair(Line(p1, p3), Line(p2, p4));
    }

    double eccentricity() {return dist(A, B) / d;}
    bool containsPoint(const Point& P) const 
    {
        if (dist(A, P) + dist(B, P) > EPS + d) {return false;}
        else {return true;}
    }
    void rotate(Point center, double deg) {A = Rotate(center, A, deg);B = Rotate(center, B, deg);}
    void reflex(Point center) {A = Reflex(A, center);B = Reflex(B, center);}
    void reflex(Line axis) {A = Reflex(A, axis);B = Reflex(B, axis);}
    void scale(Point c, const double& coe) 
    {
        this->A = Scale(c, A, coe);
        this->B = Scale(c, B, coe);
        this->a *= coe;
        this->b *= coe;
        this->c *= coe;
        this->d *= coe;

    }
    string getInfo()
    {
        string str = "";
        str += "F1(x, y) : " + to_string(this->A.x) + ", " + to_string(this->A.y) + ",  F2(x, y) : " + to_string(this->B.x) + ", " + to_string(this->B.y) + "";
        return str;
    }
    bool operator==(const Shape& another) const 
    {
        if (dynamic_cast<const Ellipse*>(&another) == NULL) { return false; }
        const Ellipse& Another = dynamic_cast<const Ellipse&>(another);
        if (((A == Another.A && B == Another.B) || (B == Another.A && A == Another.B)) && d == Another.d) {return true;}
        else {return false;}
    }
    bool operator!=(const Shape& another) const {return !(*this == another);}
    bool isCongruentTo(const Shape& another) const
    {
        if (dynamic_cast<const Ellipse*>(&another) == NULL) {return false;}
        const Ellipse& Another = dynamic_cast<const Ellipse&>(another);
        if (dist(A, B) == dist(Another.A, Another.B) && d == Another.d) {return true;}
        else {return false;}
    }
    bool isSimilarTo(const Shape& another) const 
    {
        if (*this == another) return true;
        if (dynamic_cast<const Ellipse*>(&another) == NULL)return false;
        
        

        const Ellipse& otherEllipse = static_cast<const Ellipse&>(another);

        // Проверить, что соответствующие полуоси эллипсов пропорциональны.
        double ratio1 = a / otherEllipse.a;
        double ratio2 = b / otherEllipse.b;

        if (abs(ratio1 - ratio2) > EPS) {
            return false;
        }

        // Проверить, что центры эллипсов совпадают.
        if (A != otherEllipse.A) 
        {
            return false;
        }

        return true;
    }



    void glutDraw()
    {
        double pointCount = 50;
        float step = double(2 * Pi / pointCount);

        double deg;
        if (A != B)
            deg = atan((B.y - A.y) / (B.x - A.x));
        else
            deg = 0;
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.5f);
        double dx = a / 100 * cosf(-step);
        double dy = b / 100 * sinf(-step);
        Point Cen = (A + B) / 200;
        double xb, yb, x, y;
        for (double angle = 0; angle < double(2 * Pi); angle += step)
        {
            xb = dx + Cen.x;
            yb = dy + Cen.y;
            x = Cen.x + (xb - Cen.x) * cosf(deg) - (yb - Cen.y) * sinf(deg);
            y = Cen.y + (xb - Cen.x) * sinf(deg) + (yb - Cen.y) * cosf(deg);
            glVertex2f(x, y);
            dx = a / 100 * cosf(angle);
            dy = b / 100 * sinf(angle);
            xb = dx + Cen.x;
            yb = dy + Cen.y;
            x = Cen.x + (xb - Cen.x) * cosf(deg) - (yb - Cen.y) * sinf(deg);
            y = Cen.y + (xb - Cen.x) * sinf(deg) + (yb - Cen.y) * cosf(deg);
            glVertex2f(x, y);
        }
        glEnd();
    }
};


class Circle : public Ellipse 
{
public:
    Circle() {}
    Circle(Point center, double rad) {A = center;B = center;a = rad;b = rad;d = 2 * rad;c = 0;}
    ~Circle() {}

    string getInfo() {string str = "";str += "Центр (x ,y) : " + to_string(this->A.x) + ", " + to_string(this->A.y) + ",   Радиус: " + to_string(this->radius());return str;}
    double radius() const {return a;}
    bool operator==(const Circle& other) const {
        return A == other.A && radius() == other.radius();
    }

    bool isSimilarTo(const Shape& another) const 
    {
        
        if (dynamic_cast<const Circle*>(&another) == NULL)return false;

        const Circle& otherCircle = static_cast<const Circle&>(another);

        
        if (radius() != otherCircle.radius()) {
            return true;
        }

        return false;
    }
    
};


class Rectangle :public Polygon
{
public:
    Rectangle() {}
    Rectangle(Point& P1, Point& P3, double k) 
    {
        if (k < 1) {k = 1 / k;}
        Point center = (P1 + P3) / 2;
        Point P2 = Rotate(center, P1, 2 * atan(1 / k) * 57.29578);
        Point P4 = Reflex(P2, center);
        points.push_back(P1);
        points.push_back(P2);
        points.push_back(P3);
        points.push_back(P4);
        size_ = 4;
    }
    Point center() {return (points[0] + points[2]) / 2;}
    std::pair<Line, Line>diagonals() {return { Line(points[0], points[2]), Line(points[1], points[3]) };}

    ~Rectangle() = default;
};



class Square : public Rectangle 
{
public:
    Square() {}
    Square(Point& P1, Point& P3) 
    {
        double tg = 2;
        Point center = (P1 + P3) / 2;
        Point P2 = Rotate(center, P1, 90);
        Point P4 = Reflex(P2, center);
        points.push_back(P1);
        points.push_back(P2);
        points.push_back(P3);
        points.push_back(P4);
        size_ = 4;
    }
    Circle circumscribedCircle() 
    {
        Point p = Point((points[0] + points[2]) / 2);
        return Circle(p, dist(p, points[0]));
    }
    Circle inscribedCircle() 
    {
        Point p = Point((points[0] + points[2]) / 2);
        return Circle(p, dist(points[0], points[1]) / 2);
    }

    bool isSimilarTo(const Shape& another) const
    {
        if (*this == another) return true;
        if (dynamic_cast<const Polygon*>(&another) == NULL)return false;
        const Square& Another = dynamic_cast<const Square&>(another);
        if (size_ != Another.size_)return false;

        const Square& otherSquare = static_cast<const Square&>(another);

        
        double diagonal1 = dist(points[0], points[2]);
        double diagonal2 = dist(otherSquare.points[0], otherSquare.points[2]);

        if (otherSquare == *this)  {
            return false;
        }

        return true;
    }
    ~Square() {}
};



class Triangle : public Polygon 
{
public:
    Triangle(std::vector<Point>& g) 
    {
        this->size_ = g.size();
        this->points = g;
    }
    Circle circumscribedCircle()
    {
        Point A = points[0]; Point B = points[1]; Point C = points[2];
        Line AB(A, B); Line BC(B, C);  Line CA(C, A);
        Point Circumcenter((A.x * Sin2(CA, AB) + B.x * Sin2(AB, BC) + C.x * Sin2(CA, BC) / (Sin2(CA, AB) + Sin2(AB, BC) +
            Sin2(CA, BC))), ((A.y * Sin2(CA, AB)) + B.y * Sin2(AB, BC) + C.y * Sin2(CA, BC)) / (Sin2(CA, AB) + Sin2(AB, BC) + Sin2(CA, BC)));
        return Circle(Circumcenter, dist(Circumcenter, A));
    }
    Circle inscribedCircle() 
    {
        double rad = (2 * area()) / (dist(points[0], points[1]) + dist(points[1], points[2]) + dist(points[2], points[0]));
        Point Center = orthocenter();
        return Circle(Center, rad);
    }
    Point orthocenter()
    {
        Line FirstPerpendicular = perpendicular(Line(points[1], points[2]), points[0]);
        Line SecondPerpendicular = perpendicular(Line(points[2], points[0]), points[1]);
        return intersect(FirstPerpendicular, SecondPerpendicular);
    }
    Line EulerLine() {return Line(orthocenter(), centroid());}
    Circle ninePointsCircle() 
    {
        Point EulerCenter((orthocenter() + circumscribedCircle().focuses().first) / 2);
        double EulerRadius = circumscribedCircle().radius() / 2;
        return Circle(EulerCenter, EulerRadius);
    }


    bool isSimilarTo(const Shape& another) const 
    {
        if (*this == another) return true;
        if (dynamic_cast<const Triangle*>(&another) == NULL)
            return false;
        const Polygon& Another = dynamic_cast<const Triangle&>(another);
        if (size_ != Another.size_)
            return false;

        const Triangle& otherTriangle = static_cast<const Triangle&>(another);

        // Проверить, что соответствующие стороны треугольников пропорциональны.
        double side1Ratio = dist(points[0], points[1]) / otherTriangle.dist(otherTriangle.points[0], otherTriangle.points[1]);
        double side2Ratio = dist(points[1], points[2]) / otherTriangle.dist(otherTriangle.points[1], otherTriangle.points[2]);
        double side3Ratio = dist(points[2], points[0]) / otherTriangle.dist(otherTriangle.points[2], otherTriangle.points[0]);

        if (abs(side1Ratio - side2Ratio) > EPS || abs(side2Ratio - side3Ratio) > EPS || abs(side3Ratio - side1Ratio) > EPS) {
            return false;
        }

        // Проверить, что соответствующие углы треугольников равны.
        double angle1 = acos(Shape::Cos(Line(points[0], points[1]), Line(points[0], points[2])));
        double angle2 = acos(Shape::Cos(Line(otherTriangle.points[0], otherTriangle.points[1]), Line(otherTriangle.points[0], otherTriangle.points[2])));


        if (abs(angle1 - angle2) > EPS) 
        {
            return false;
        }

        return true;
    }
};



