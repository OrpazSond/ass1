//
// Created by yarintz33 on 12/9/21.
//
#include <math.h>
#include "minCircle.h"

// Defining infinity
const float INF = 91000000;

// Function to return the euclidean distance
// between two points
float dist(const Point& a, const Point& b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Function to check whether a point lies inside
// or on the boundaries of the circle
bool is_inside(Circle& c, Point& p)
{
    return dist(c.center, p) <= c.radius;
}


// The following two functions are the functions used
// To find the equation of the circle when three
// points are given.

// Helper method to get a circle defined by 3 points
Point get_circle_center(float bx, float by,
                        float cx, float cy)
{
    float B = bx * bx + by * by;
    float C = cx * cx + cy * cy;
    float D = bx * cy - by * cx;
    return Point((cy * B - by * C) / (2 * D),
                 (bx * C - cx * B) / (2 * D) );
}

// Function to return a unique circle that intersects
// three points
Circle circle_from(Point& A, Point& B,
                    Point& C)
{
    Point I = get_circle_center(B.x - A.x, B.y - A.y,
                                C.x - A.x, C.y - A.y);
    I.x += A.x;
    I.y += A.y;
    return Circle( I, dist(I, A)) ;
}

// Function to return the smallest circle
// that intersects 2 points
Circle circle_from(Point& A, Point& B)
{
    // Set the center to be the midpoint of A and B
    Point C ( (A.x + B.x) / 2.0, (A.y + B.y) / 2.0);

    // Set the radius to be half the distance AB

    return Circle(C, dist(A, B) / 2.0); //C, dist(A, B) / 2.0 };
}

// Function to check whether a circle encloses the given points
bool is_valid_circle(Circle& c,  Point** points, size_t t)
{

    // Iterating through all the points to check
    // whether the points lie inside the circle or not
    for (int i = 0; i < t; i++)
        if (!is_inside(c, *points[i]))
            return false;
    return true;
}


Circle findMinCircle(Point** points,size_t size){

    // To find the number of points
    int n = size;

    if (n == 0)
        return Circle( Point(0,0), 0 );
    if (n == 1)
        return Circle( *points[0], 0 );

    // Set initial MEC to have infinity radius
    float infimum = INF;
    Circle mec(Point(0,0), infimum);

    // Go over all pair of points
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {

            // Get the smallest circle that
            // intersects P[i] and P[j]
            Circle tmp = circle_from(*points[i], *points[j]); // points[i], points[j]

            // Update MEC if tmp encloses all points
            // and has a smaller radius
            if (tmp.radius < mec.radius && is_valid_circle(tmp, points, size))
                mec = tmp;
        }
    }

    // Go over all triples of points
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = j + 1; k < n; k++) {

                // Get the circle that intersects P[i], P[j], P[k]
                Circle tmp = circle_from(*points[i], *points[j], *points[k]);

                // Update MEC if tmp encloses all points
                // and has smaller radius
                if (tmp.radius < mec.radius && is_valid_circle(tmp, points,size))
                    mec = tmp;
            }
        }
    }
    return mec;

}