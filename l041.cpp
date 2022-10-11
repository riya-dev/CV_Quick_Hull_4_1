// cd project4
// cd part1
// g++ -std=c++11 -o l041  -Wall l041.cpp
// ./l041

#include <iostream>
#include <fstream>
#include <iomanip>  // set precision
#include <climits>  // int_max
#include <list>     // list
#include <math.h>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>    // std::sort
#include <map>
#include <unordered_map>
#include <functional>

const int size = 400;
std::ofstream results;

// point class
// private, getters, setters, constructors
class Point {
public:
    void setX(double x);
    double getX(void);
    void setY(double y);
    double getY(void);
    bool diff(Point p2);
    bool same(Point p2);
    //int Compare(Point d);
    static bool sortFunct(Point i, Point j) { return (j.getX() > i.getX()); }
    static std::vector<Point> getSorted(std::vector<Point> p) {
        std::sort(p.begin(), p.end(), sortFunct);
        return p;
    }
    static bool sortFunctY(Point i, Point j) { return (j.getY() > i.getY()); }
    static std::vector<Point> getSortedY(std::vector<Point> p) {
        std::sort(p.begin(), p.end(), sortFunctY);
        return p;
    }
    void print();
    Point(double x, double y);  // constructor
    Point(void);
private:
    double x;
    double y;
};
Point::Point(void) {
    x = 0;
    y = 0;
}
Point::Point(double xval, double yval) {
    x = xval;
    y = yval;
}
void Point::setX(double xval) {
    x = xval;
}
double Point::getX(void) {
    return x;
}
void Point::setY(double yval) {
    y = yval;
}
double Point::getY(void) {
    return y;
}
bool Point::diff(Point p2) {
    if (getX() != p2.getX() && getY() != p2.getY())
        return true;
    return false;
}
bool Point::same(Point p2) {
    if (getX() == p2.getX() && getY() == p2.getY())
        return true;
    return false;
}
void Point::print() {
    std::cout << std::fixed;
    std::cout << std::setprecision(23) << "(" << getX() << " , " << getY() << ")";
}

class Distance {
public:
    void setdist(double d);
    double getdist(void);
    void setp1(Point p);
    Point getp1(void);
    void setp2(Point p);
    Point getp2(void);
    Distance compare(Distance d1, Distance d2);
    void print();
    Distance(double dist, Point p1, Point p2);  // constructors
    Distance(void);
private:
    double dist;
    Point p1;
    Point p2;
};
Distance::Distance(void) {
    dist = 0;
    p1 = Point();
    p2 = Point();
}
Distance::Distance(double d, Point p1val, Point p2val) {
    dist = d;
    p1 = p1val;
    p2 = p2val;
}
void Distance::setdist(double d) {
    dist = d;
}
double Distance::getdist(void) {
    return dist;
}
void Distance::setp1(Point p) {
    p1 = p;
}
Point Distance::getp1(void) {
    return p1;
}
void Distance::setp2(Point p) {
    p2 = p;
}
Point Distance::getp2(void) {
    return p2;
}
Distance Distance::compare(Distance d1, Distance d2) {
    if (d1.getdist() < d2.getdist())
        return d1;
    return d2;
}
void Distance::print() {
    std::cout << std::fixed;
    std::cout << std::setprecision(23) << dist << "\t";
    getp1().print();
    std::cout << "\t";
    getp2().print();
}

std::vector<Point> convexhull;

void illuminate(int** arr, int y, int x, int color) {
    if (color == 0)
        arr[x][y] = 0;
    else if (color == 1)
        arr[x][y] = 1;
    else if (color == 2)
        arr[x][y] = 2;
    else if (color == 3)
        arr[x][y] = 3;
}

void notoutofbounds(int** array, int x, int y, int color) {
    if (x >= 0 && x < size && y >= 0 && y < size)
        //array[x][y] = 0;
        illuminate(array, x, y, color);
}

double getdistance(double x1, double y1, double x2, double y2) {
    double t1 = pow(x1 - x2, 2);
    double t2 = pow(y1 - y2, 2);
    return (double)(sqrt(t1 + t2));
}

double getdistance(Point a, Point b) {
    return getdistance(a.getX(), a.getY(), b.getX(), b.getY());
}

void circle(int** array, int xcenter, int ycenter, int R, int color) {
    int y = R;
    int xmax = (int)(R * 0.70710678);
    int ysquare = y * y;
    int ty = (2 * y) - 1;
    int y2_new = ysquare;

    for (int x = 0; x <= xmax + 1; x++) {
        if ((ysquare - y2_new) >= ty) {
            ysquare -= ty;
            y -= 1;
            ty -= 2;
        }
        notoutofbounds(array, y + xcenter, x + ycenter, color);
        notoutofbounds(array, -y + xcenter, x + ycenter, color);
        notoutofbounds(array, y + xcenter, -x + ycenter, color);
        notoutofbounds(array, -y + xcenter, -x + ycenter, color);

        notoutofbounds(array, x + xcenter, y + ycenter, color);
        notoutofbounds(array, -x + xcenter, y + ycenter, color);
        notoutofbounds(array, x + xcenter, -y + ycenter, color);
        notoutofbounds(array, -x + xcenter, -y + ycenter, color);

        y2_new -= (2 * x) - 3;
    }
}

bool insidetriangle(double rx1, double ry1, double rx2, double ry2, double rx3, double ry3, double rx4, double ry4, double m23, double m34, double m24) {
    // triangle 234, p1 is the point i'm testing
    // 1 = true
    // 0 = false

    int count = 0;

    if (std::min(rx2, rx3) < rx1 && rx1 < std::max(rx2, rx3))
        if (rx1 < m23 * (rx1 - rx2) + ry2)
            count++;
    if (std::min(rx3, rx4) < rx1 && rx1 < std::max(rx3, rx4))
        if (m34 * (rx1 - rx3) + ry3 > rx1)
            count++;
    if (std::min(rx2, rx4) < rx1 && rx1 < std::max(rx2, rx4))
        if (m24 * (rx1 - rx4) + ry4 > rx1)
            count++;

    if (count == 3)
        return true;
    return false;
}

// driving force x -> dx < dy

int** linecase1(int x1, int x2, int y1, int y2, int** arr) { // x1 < x2, y1 < y2, dx < dy = default bresenham's
    //std::cout << x1 << y1 << x2 << y2 << "\n";

    int dx = x2 - x1; // |
    int dy = y2 - y1; // _
    int e = dx - dy;
    int i = x1;

    for (int j = y1; j <= y2; j++) {
        //arr[i][j] = 0; // illuminate
        notoutofbounds(arr, i, j, 2);
        if (e >= 0) {
            i++;
            e -= dy;
        }
        e += dx;
    }
    return arr;
}

int** linecase2(int x1, int x2, int y1, int y2, int** arr) { // x1 < x2 && y1 > y2 && dx > dy
    //std::cout << x1 << y1 << x2 << y2 << "\n";

    int dx = x1 - x2; // |
    int dy = y2 - y1; // _
    int e = dx - dy;
    int i = x1;

    for (int j = y1; j <= y2; j++) {
        //arr[i][j] = 0; // illuminate
        notoutofbounds(arr, i, j, 2);
        if (e >= 0) {
            i--;
            e -= dy;
        }
        e += dx;
    }
    return arr;
}

int** linecase3(int x1, int x2, int y1, int y2, int** arr) { // y1 = y2 && x1 < x2 && dx < dy
    for (int i = x1; i < x2; i++) {
        //arr[i][y1] = 0;
        notoutofbounds(arr, i, y1, 2);
    }
    return arr;
}

// driving force y -> dx > dy

int** linecase4(int x1, int x2, int y1, int y2, int** arr) { //x1 < x2 && y1 < y2 && dx > dy
    //std::cout << "case 4 " << x1 << y1 << " " << x2 << y2 << "\n";

    int dx = x2 - x1; // |
    int dy = y2 - y1; // _
    int e = dy - dx;
    int j = y1;

    for (int i = x1; i <= x2; i++) {
        //arr[i][j] = 0; // illuminate
        notoutofbounds(arr, i, j, 2);
        if (e >= 0) {
            j++;
            e -= dx;
        }
        e += dy;
    }
    return arr;
}

int** linecase5(int x1, int x2, int y1, int y2, int** arr) {
    //std::cout << "case 5 " << x1 << y1 << " " << x2 << y2 << "\n";

    int dx = x2 - x1; // |
    int dy = y1 - y2; // _
    int e = dy - dx;
    int j = y1;

    for (int i = x1; i <= x2; i++) {
        //arr[i][j] = 0; // illuminate
        notoutofbounds(arr, i, j, 2);
        if (e >= 0) {
            j--;
            e -= dx;
        }
        e += dy;
    }
    return arr;
}

int** linecase6(int x1, int x2, int y1, int y2, int** arr) { //x1 == x2 && y1 < y2
    //std::cout << "case 6" << x1 << y1 << " " << x2 << y2 << "\n";

    for (int j = y1; j < y2; j++) {
        //arr[x1][j] = 0;
        notoutofbounds(arr, x2, j, 2);
    }
    return arr;
}

void chooselinefunction(int x1, int x2, int y1, int y2, int** array) {
    // the 6 freaking cases

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    // driving force x -> dx > dy

    // case 1
    if (x1 < x2 && y1 < y2 && dx <= dy) {
        //std::cout << "case 1a" << x1 << y1 << " " << x2 << y2 << "\n";
        array = linecase1(x1, x2, y1, y2, array);
    }
    else if (x2 < x1 && y2 < y1 && dx <= dy) {
        //std::cout << "case 1b" << x1 << y1 << " " << x2 << y2 << "\n";
        int temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
        array = linecase1(x1, x2, y1, y2, array);
    }

    // case 2
    else if (x1 > x2 && y1 < y2 && dx <= dy) {
        //std::cout << "case 2a" << x1 << y1 << " " << x2 << y2 << "\n";
        array = linecase2(x1, x2, y1, y2, array);
    }
    else if (x1 < x2 && y1 > y2 && dx <= dy) {
        //std::cout << "case 2b" << x1 << y1 << " " << x2 << y2 << "\n";
        int temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
        array = linecase2(x1, x2, y1, y2, array);
    }

    // case 3
    else if (y1 == y2 && x1 < x2) { // use higher precedence operator first
        //std::cout << "case 3a" << x1 << y1 << " " << x2 << y2 << "\n";
        array = linecase3(x1, x2, y1, y2, array);
    }
    else if (y1 == y2 && x1 > x2) {
        //std::cout << "case 3b" << x1 << y1 << " " << x2 << y2 << "\n";
        int temp = x1;
        x1 = x2;
        x2 = temp;
        array = linecase3(x1, x2, y1, y2, array);

    }

    // driving force y -> dy > dx

    // case 4
    else if (x1 < x2 && y1 < y2 && dx > dy) {
        //std::cout << "case 4a" << x1 << y1 << " " << x2 << y2 << "\n";
        array = linecase4(x1, x2, y1, y2, array);
    }
    else if (x2 < x1 && y2 < y1 && dx > dy) {
        //std::cout << "case 4b" << x1 << y1 << " " << x2 << y2 << "\n";
        int temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
        array = linecase4(x1, x2, y1, y2, array);
    }

    // case 5
    else if (x1 < x2 && y1 > y2 && dx > dy) {
        //std::cout << "case 5a" << x1 << y1 << " " << x2 << y2 << "\n";
        array = linecase5(x1, x2, y1, y2, array);
    }
    else if (x2 < x1 && y2 > y1 && dx > dy) {
        //std::cout << "case 5b" << x1 << y1 << " " << x2 << y2 << "\n";
        int temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
        array = linecase5(x1, x2, y1, y2, array);
    }

    // case 6
    else if (x1 == x2 && y1 < y2) { // use higher precedence operator first
        array = linecase6(x1, x2, y1, y2, array);
    }
    else if (x1 == x2 && y2 < y1) {
        int temp = y1;
        y1 = y2;
        y2 = temp;
        array = linecase6(x1, x2, y1, y2, array);
    }
}

double turnvalue(Point A, Point B, Point C) {
    double z = ((B.getX() - A.getX()) * (C.getY() - A.getY())) - ((B.getY() - A.getY()) * (C.getX() - A.getX()));
    return z;
}

void displayhull() {
    std::cout << "convexhull ";
    for (size_t i = 0; i < convexhull.size(); i++) {
        convexhull[i].print();
        std::cout << " ";
    }
    std::cout << "\n";
    //std::string s;
    //std::cin >> s;
}

void FindHull(std::vector<Point>& v, Point& A, Point& B) {
    if (v.size() == 0)
        return;
    double m = (double)(B.getY() - A.getY()) / (B.getX() - A.getX());
    double perpm = -(1 / m);

    double x, y;
    Point intersection;
    Point C;
    Distance distance(0, A, B);

    for (Point temp : v) {
        //x = ((A.getX() * m) - A.getY() + (temp.getX() * perpm) + temp.getY()) / (m - perpm); INCORRECT
        x = (temp.getY() - (perpm * temp.getX()) - A.getY() + (m * A.getX())) / (m - perpm);
        //y = (m * x) - (m * A.getX()) + A.getY(); CORRECT
        y = (perpm * x) - (perpm * temp.getX()) + temp.getY();
        intersection = Point(x, y);
        if (distance.getdist() < getdistance(intersection, temp)) {
            distance = Distance(getdistance(intersection, temp), intersection, temp);
            C = temp;
        }
    }

    for (size_t i = 0; i < convexhull.size() - 1; i++) {
        //if (convexhull[i].getX() == A.getX() && convexhull[i].getY() == A.getY() && convexhull[i + 1].getX() == B.getX() && convexhull[i+1].getY() == B.getY())
        if (convexhull[i].same(A) && convexhull[i + 1].same(B)) {
            convexhull.insert(convexhull.begin() + i + 1, C);
            //displayhull();
        }
    }

    std::vector<Point> S1;
    std::vector<Point> S2;

    for (Point s : v) {
        double z = turnvalue(A, C, s);
        if (z < 0)
            S1.push_back(s);
    }

    for (Point s : v) {
        double z = turnvalue(C, B, s);
        if (z < 0)
            S2.push_back(s);
    }

    FindHull(S1, A, C);
    FindHull(S2, C, B);
}

void part1() {
    std::ofstream points;
    points.open("points.txt");
    std::vector <Point> pointlist;

    // make array
    int** array;
    array = new int* [size];

    // fill with 1s
    for (int row = 0; row < size; ++row)
        array[row] = new int[size];
    for (int row = 0; row < size; ++row)
        for (int col = 0; col < size; ++col)
            array[row][col] = 1;

    srand(time(NULL));
    for (int i = 0; i <= 59; i++) {
        double x = double(rand()) / RAND_MAX;
        double y = double(rand()) / RAND_MAX;
        Point p(x, y);
        pointlist.push_back(p);
        circle(array, (int)(x * size), (int)(y * size), 3, 0);
        points << std::fixed;
        points << std::setprecision(23) << x << "  " << y << "\n";
    }
    points.close();

    // make ppm
    std::ofstream ppm;
    ppm.open("quickhull.ppm");
    ppm << "P3 " << size << " " << size << " 255\n";

    pointlist = Point::getSorted(pointlist);
    Point A = pointlist[0];
    //A.print();
    Point B = pointlist[pointlist.size() - 1];
    //B.print();
    convexhull.push_back(A);
    convexhull.push_back(B);
    convexhull.push_back(A);
    //displayhull();

    std::vector<Point> S1; //right of the line AB
    std::vector<Point> S2; // right of the line BA

    for (Point C : pointlist) {
        double z = turnvalue(A, B, C);
        if (z < 0)
            S1.push_back(C);
        else if(z > 0)
            S2.push_back(C);
    }

    FindHull(S1, A, B); // right
    //displayhull();
    FindHull(S2, B, A); // left
    //displayhull();

    //std::cout << "\n" << convexhull.size() << "\n";

    for (size_t i = 0; i < convexhull.size() - 1; i++) {
        //convexhull[i].print();
        //std::cout << "\n";
        chooselinefunction((int)(convexhull[i].getX() * size), (int)(convexhull[i + 1].getX() * size), (int)(convexhull[i].getY() * size), (int)(convexhull[i + 1].getY() * size), array);
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (array[i][j] == 0)
                ppm << array[i][j] << " " << array[i][j] << " " << array[i][j] << ' ';
            else if (array[i][j] == 2)
                ppm << 255 << " " << 0 << " " << 0 << " ";
            else
                ppm << 255 << " " << 255 << " " << 255 << ' ';
        }
        ppm << std::endl;
    }
    ppm << "\n";

    ppm.close();
}

int main() {
    part1();
}