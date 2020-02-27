#ifndef HELPER
#define HELPER
#include<cstdio>
#include<cmath>
#include<modes/modes.h>
struct node
{
    glm::vec2 pos;
    glm::vec2 * refPoint[100];
    int refNum;
    node(){
        refNum = 0;
    }
    void addRef(Mode * shader){
        shader->insert(&refPoint[refNum]);
        *refPoint[refNum] = pos;
        refNum++;
    }
    void update(){
        for (int i = 0;i < refNum;i++)
            if (refPoint[i] != NULL)
                *refPoint[i] = pos;
    }
    void print(){
        printf("x:%f, y:%f\n",pos.x,pos.y);
    }
    double dist(node n){
        return sqrt((n.pos.x-pos.x)*(n.pos.x-pos.x)+
                (n.pos.y-pos.y)*(n.pos.y-pos.y));
    }
};
#endif
