#ifndef HELPER
#define HELPER
#include<cstdio>
#include<cmath>
struct node
{
    float x,y;
    node(float x_, float y_){
        x = x_;
        y = y_;
    }
    void print(){
        printf("x:%f, y:%f\n",x,y);
    }
    double dist(node n){
        return sqrt((n.x-x)*(n.x-x)+(n.y-y)*(n.y-y));
    }
};
#endif
