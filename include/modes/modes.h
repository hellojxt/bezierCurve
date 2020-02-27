#ifndef MODES_H
#define MODES_H
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<cstdio>
#include<utils/shader.h>
#include<utils/helper.h>
#include<vector>
#define STB_IMAGE_IMPLEMENTATION
#include<utils/stb_image.h>

class Mode{
    public:
    int width,height;
    void resize(int w, int h){
        width = w;
        height = h;
    }
    virtual void draw() = 0;
    virtual bool insert(glm::vec2 ** p) = 0;
    virtual void end_insert()=0;
};
#endif