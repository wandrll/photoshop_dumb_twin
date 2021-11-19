#ifndef VECTOR_HEADER
#define VECTOR_HEADER
#include <cmath>
#include <cstdio>

class Vector{
    public:

    double x;
    double y;

    Vector (double x, double y){
        this->x = x;
        this->y = y;
    }

    Vector (){
        this->x = 0;
        this->y = 0;
    }

    double length(){
        return sqrt(x*x + y*y);
    }

    void normalize(){
        double len = length();
        x = x / len;
        y = y / len;
    }

    void orthogonalize(){
        double tmp = x;
        x = -y;
        y = tmp;
    }

    Vector operator+ (const Vector& a) const{
        return {this->x + a.x, this->y + a.y};
    }

    Vector operator- (const Vector& a) const{
        return {this->x - a.x, this->y - a.y};
    }

    double operator* (const Vector& a){
        return this->x * a.x + this->y * a.y;
    }

    Vector operator* (const double coef) const{
        return {this->x * coef, this->y * coef};
    }


    void operator*= (const double coef){
        this->x *= coef;
        this->y *= coef;
        // return {this->x * coef, this->y * coef};
    }

    void operator+= (const Vector& a){
        this->x += a.x;
        this->y += a.y;
        
    }

    void rotate(double angle){
        double cos_ = cos(angle);
        double sin_ = sin(angle);

        double old_x = x;
        double old_y = y;

        x =  cos_ * old_x + sin_ * old_y;
        y = -sin_ * old_x + cos_ * old_y;
    }

    void print(const char* line){
        printf("%s %lg %lg\n", line, this->x, this->y);
    }

    private:
    



};




#endif