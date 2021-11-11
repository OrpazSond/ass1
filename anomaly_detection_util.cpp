//
// Created by orpaz on 10/17/21.fdh
//

#include <math.h>

float avg(float* x, int size){
    float mu = 0;
    for (int i = 0; i < size; ++i) {
        mu = mu + x[i];
    }
    return mu / size;
}

float var(float* x, int size){
    float sum = 0, result = 0;
    float m = pow(avg(x, size), 2);
    for (int i = 0; i < size; ++i) {
        sum = sum + pow(x[i], 2);
    }
    result = (sum / size) - m ;
    return result;
}

float cov(float* x, float* y, int size){
    float sum = 0;
    float muX = avg(x, size);
    float muY = avg(y, size);
    for (int i = 0; i < size; ++i) {
        sum = sum + ((x[i] - muX) * (y[i] - muY));
    }
    sum = sum / size;
    return sum;
}

float pearson(float* x, float* y, int size){
    float numerator = cov(x, y, size);
    float denominator = sqrt(var(x, size)) * sqrt(var(y, size));
    return numerator / denominator;
}

class Line{
public:
    float a,b;
    Line():a(0),b(0){}
    Line(float a, float b):a(a),b(b){}
    float f(float x){
        return a*x+b;
    }
};
class Point{
public:
    float x,y;
    Point(float x, float y):x(x),y(y){}
};

Line linear_reg(Point** points, int size) {
    float x[size];
    float y[size];
    for (int i = 0; i < size; ++i) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float a, b;
    a = cov(x, y, size) / var(x, size);
    float muX = avg(x, size);
    float muY = avg(y, size);
    b = muY - (a * muX);
    Line line (a, b);
    return line;
}

float dev(Point p,Line l){
    float lineY = l.f(p.x);
    float dev = lineY - p.y;
    if (dev < 0)
        return -dev;
    return dev;
}

float dev(Point p,Point** points, int size){
    Line line = linear_reg(points, size);
    return dev(p, line);
}

