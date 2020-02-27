#ifndef HELPER
#define HELPER
struct Node;
struct Ref;
struct Refed;
#include<cstdio>
#include<cmath>
#include<modes/modes.h>

struct Ref{
    Node* n = NULL;
    int refIdx = 0;
    Ref(Node* n_, int r){
        n = n_;
        refIdx = r;
    }
    Ref(){
    }
    void set(Node* n_, int r){
        n = n_;
        refIdx = r;
    }
};

struct Refed{
    glm::vec2 * pos = NULL;
    Ref * r = NULL;
    Refed(glm::vec2 * p, Ref * r_){
        pos = p;
        r = r_;
    }
    Refed(){
    }
    void set(glm::vec2 * p, Ref * r_){
        pos = p;
        r = r_;
    }
};
struct Node
{
    glm::vec2 pos;
    Refed refPoint[100];
    int refNum;
    Node(){
        refNum = 0;
    }
    void addRef(Mode * shader){
        shader->insert(Ref(this,refNum));
        refNum++;
    }
    void update(){
        for (int i = 0;i < refNum;i++)
            if (refPoint[i].pos != NULL)
                *refPoint[i].pos = pos;
    }
    void print(){
        printf("x:%f, y:%f\n",pos.x,pos.y);
    }
    double dist(Node n){
        return sqrt((n.pos.x-pos.x)*(n.pos.x-pos.x)+
                (n.pos.y-pos.y)*(n.pos.y-pos.y));
    }
};
#endif
