#ifndef PVECTOR_H
#define PVECTOR_H
#define CHAIN return *this;
// Import mathematical header files
#include <cmath>
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
// Define a macro for chained functions
#include <stdexcept>
// Macro that prints out a vector in conjunction with std::cout
#define VECT_OUT(v) "{" << v.x << ", " << v.y << ", " << v.z << "}"
#define VECT_XY(v) v.x << ", " << v.y
union PVector {
    // Define the return type of a chain method
    typedef PVector& chain_method;
    // Converts radians to degrees
    double radToDeg(double r) {
        return r * 180.0 / M_PI;
    }
    // Converts degrees to radians
    double degToRad(double r) {
        return r * M_PI / 180.0;
    }
    // Static versions of the above functions
    static double sradToDeg(double r) {
        return r * 180.0 / M_PI;
    }
    static double sdegToRad(double r) {
        return r * M_PI / 180.0;
    }
    // The propertys
    // x, y, and z are in a nameless struct so that they are bound to the array propertys
    struct
    {
        double x, y, z;
    };
    double arr[3] = {0.0, 0.0, 0.0};
    // Access the array propertys directly
    double& operator[](int i) {
        return arr[i];
    }
    // Constructors
    PVector(double ex, double ey) {
        x = ex;
        y = ey;
    }
    PVector(double ex, double ey, double ez) {
        x = ex;
        y = ey;
        z = ez;
    }
    PVector() {
        x = 0.0;
        y = 0.0;
        x = 0.0;
    }
    // Have compiler generate move and copy constructors and destruct
    PVector(PVector&& v) = default;
    PVector(const PVector& v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    PVector(std::initializer_list<double> l) {
        int i = 0;
        if (l.size() > 3) {
            std::cout << "Error: initializer list too large (>3 elements)" << std::endl;
            // throw std::range_error("Out of size PVector initializer list (> 3 elements)");
        }
        for (double s : l) {
            arr[i] = s;
            i++;
        }
    }
    ~PVector() = default;
    // Set a PVector
    chain_method set(double ex, double ey, double ez = 0.0) {
        x = ex;
        y = ey;
        z = ez;
        CHAIN
    }
    chain_method set(PVector v) {
        x = v.x;
        y = v.y;
        z = v.z;
        CHAIN
    }
    // Get a copy of PVector, kinda useless but I have kept it from the original Java source code
    PVector get() {
        PVector h = PVector(x, y, z);
        return h;
    }
    // Get 2D magnitude of a vector
    double mag() {
        return sqrt(x * x + y * y);
    }
    // Add a vectors components
    chain_method add(double tx, double ty, double tz = 0.0) {
        x += tx;
        y += ty;
        z += tz;
        CHAIN
    }
    // Add 2 vectors
    chain_method add(PVector temp) {
        x += temp.x;
        y += temp.y;
        z += temp.z;
        CHAIN
    }
    chain_method add(PVector* temp) {
        x += temp->x;
        y += temp->y;
        z += temp->z;
        CHAIN
    }
    chain_method add(double a) {
        x += a;
        y += a;
        z += a;
        CHAIN
    }

    // Subtract 2 vectors
    chain_method sub(PVector v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        CHAIN
    }
    chain_method sub(PVector* v) {
        x -= v->x;
        y -= v->y;
        z -= v->z;
        CHAIN
    }

    chain_method sub(double tx, double ty, double tz) {
        x -= x;
        y -= y;
        z -= tz;
        CHAIN
    }
    // Subtract 2 vectors
    static PVector sub(PVector t1, PVector t2) {
        PVector ret = PVector(t1.x - t2.x, t1.y - t2.y);
        return ret;
    }
    // Multiply a vector
    chain_method mult(double n) {
        x *= n;
        y *= n;
        z *= n;
        CHAIN
    }
    chain_method mult(PVector v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        CHAIN
    }
    chain_method mult(PVector* v) {
        x *= v->x;
        y *= v->y;
        z *= v->z;
        CHAIN
    }

    // Divide 2 vectors
    chain_method div(double n) {
        x /= n;
        y /= n;
        z /= n;
        CHAIN
    }
    chain_method div(PVector v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        CHAIN
    }
    chain_method div(PVector* v) {
        x /= v->x;
        y /= v->y;
        z /= v->z;
        CHAIN
    }

    // Get Euclidean distance to another vector on the 2d plane
    double dist2D(PVector v) {
        double dx = x - v.x;
        double dy = y - v.y;
        return sqrt(dx * dx + dy * dy);
    }

    double dist2D(PVector* v) {
        double dx = x - v->x;
        double dy = y - v->y;
        return sqrt(dx * dx + dy * dy);
    }
    // Get distances in different planes
    double dist2D() {
        return distXY();
    }
    double distXY() {
        return sqrt(x * x + y * y);
    }
    double distXZ() {
        return sqrt(x * x + z * z);
    }
    double distYZ() {
        return sqrt(z * z + y * y);
    }
    // Get 3d distances
    double dist3D() {
        return sqrt(distXY() * distXY() + z * z);
    }
    double dist3D(PVector v) {
        double dx = x - v.x;
        double dy = y - v.y;
        double dz = z - v.z;
        return sqrt(dx * dx + dy * dy + dz * dz);
    }
    // Normalize a 2d vector
    chain_method normalize() {
        double m = mag();
        if (m != 0 && m != 1) {
            div(m);
        }
        CHAIN
    }
    chain_method normalize3D() {
        double m = dist3D();
        if (m != 0.0 && m != 1.0) {
            div(m);
        }
        CHAIN
    }
    // Limit a vector to a certain size
    chain_method limit(double max) {
        if (mag() > max) {
            normalize();
            mult(max);
        }
        CHAIN
    }
    double dot(PVector v) {
        return x * v.x + y * v.y + z * v.z;
    }
    PVector cross(PVector v) {
        double crossX = y * v.z - v.y * z;
        double crossY = z * v.x - v.z * x;
        double crossZ = x * v.y - v.x * y;
        return PVector(crossX, crossY, crossZ);
    }
    // Get headings
    double heading() {
        double angle = atan2(-x, y);
        return -radToDeg(angle);
    }
    double headingXZ() {
        double angle = atan2(-x, z);
        return -radToDeg(angle);
    }
    double headingYZ() {
        double angle = atan2(-y, z);
        return -radToDeg(angle);
    }

    // Rotate along various planes
    chain_method rotateXY(double r) {
        double m = this->distXY();
        double sinTheta = sin(degToRad(this->heading() + r));
        double cosTheta = cos(degToRad(this->heading() + r));
        x = sinTheta;
        y = cosTheta;
        x *= m;
        y *= m;
        CHAIN
    }
    chain_method rotateXZ(double r) {
        double m = this->distXZ();
        double newR = headingXZ() + r;
        newR = degToRad(newR);
        double sinTheta = sin(newR);
        double cosTheta = cos(newR);
        this->x = cosTheta;
        this->z = sinTheta;
        x *= m;
        y *= m;
        CHAIN
    }
    chain_method rotateYZ(double r) {
        double m = this->distYZ();
        double newR = headingYZ() + r;
        newR = degToRad(newR);
        double sinTheta = sin(newR);
        double cosTheta = cos(newR);
        this->y = cosTheta;
        this->z = sinTheta;
        y *= m;
        z *= m;
        CHAIN
    }
    chain_method rotate(double r) {
        rotateXY(r);
        CHAIN
    }
    // Get angle between 2 vectors
    static double angleBetween(PVector v1, PVector v2) {
        double dot = v1.x * v2.x + v1.y * v2.y;
        double v1mag = sqrt(v1.x * v1.x + v1.y * v1.y);
        double v2mag = sqrt(v2.x * v2.x + v2.y * v2.y);
        return PVector::sradToDeg(acos(dot / (v1mag * v2mag)));
    }
    // Get the angle to another vector
    double angleTo(PVector v) {
        PVector newV = v;
        newV.sub(this);
        return newV.heading();
    }
    // Overload operators
    chain_method operator+=(double a) {
        return add(a);
    }
    chain_method operator+=(PVector v) {
        *this = *this + v;
        CHAIN
    }
    chain_method operator-=(PVector v) {
        *this = *this - v;
        CHAIN
    }
    PVector operator-(PVector v) {
        PVector ret = PVector(x - v.x, y - v.y, z - v.z);
        return ret;
    }
    PVector operator*(double dd) {
        PVector ret = PVector(x, y, z);
        ret.x *= dd;
        ret.y *= dd;
        ret.z *= dd;
        return ret;
    }
    chain_method operator*=(double d) {
        x *= d;
        y *= d;
        z *= d;
        CHAIN
    }
    PVector operator/(double dd) {
        PVector ret = PVector(x, y, z);
        ret.x /= dd;
        ret.y /= dd;
        ret.z /= dd;
        return ret;
    }
    chain_method operator/=(double d) {
        x /= d;
        y /= d;
        z /= d;
        CHAIN
    }
    chain_method operator=(PVector v) {
        x = v.x;
        y = v.y;
        z = v.z;
        CHAIN
    }
    chain_method operator=(std::initializer_list<double> l) {
        int i = 0;
        if (l.size() > 3) {
            std::cout << "Error: initializer list too large (>3 elements)" << std::endl;
            // throw std::range_error("Out of size PVector initializer list (> 3 elements)");
        }
        for (double s : l) {
            arr[i] = s;
            i++;
        }
        CHAIN
    }
    PVector operator+(PVector v) {
        PVector newV;
        newV.x = x + v.x;
        newV.y = y + v.y;
        newV.z = z + v.z;
        return newV;
    }
    bool operator==(PVector v) {
        return x == v.x && y == v.y && z == v.z;
    }
    PVector operator-() {
        return PVector(-x, -y, -z);
    }
    bool operator!=(PVector v) {
        return !operator==(v);
    }
    template <typename T, class VType = decltype(T::x)>
    operator T() {
        return {
            static_cast<VType>(x),
            static_cast<VType>(y)};
    }
};

#endif // PVECTOR_H
