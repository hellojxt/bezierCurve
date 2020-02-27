#ifndef BSPLINE_H
#define BSPLINE_H

#include<utils/shader.h>
#include<utils/helper.h>
#include<modes/modes.h>
#include<cstdio>
#include<vector>

class BsplineMode: public Mode{
    public:
    unsigned int road_texture;
    unsigned int VAO,VBO;
    float res;
    Shader shader;
    glm::vec2 nodeList[100][100];
    Ref nodeRefList[100][100];
    int lineLen[100];
    int editLine = -1;
    bool inEdit = false;
    BsplineMode(){
        load_texture();
        data_init(1000);
        load_shader();
        memset(lineLen,0,sizeof(lineLen));
    } 
    void checkMerge(Node *n){
        for(int i = 0;i <= editLine;i++){
            if (nodeRefList[i][0].n == n || nodeRefList[i][lineLen[i]-1].n == n){
                if (lineLen[i] == 0)
                    continue;
                for(int j = 0;j <= editLine;j++){
                    if(j != i &&(nodeRefList[j][0].n || nodeRefList[j][lineLen[j]-1].n == n)){
                        if (lineLen[j] == 0)
                            continue;
                        glm::vec2 temp1[100];
                        Ref temp2[100];
                        int len = 0;
                        if (nodeRefList[i][0].n != n){
                            for(int k = 0;k < lineLen[i];k++){
                                temp1[len] = nodeList[i][k];
                                temp2[len] = nodeRefList[i][k];
                                len++;
                            }
                        }else{
                            for(int k = lineLen[i]-1;k >= 0 ;k--){
                                temp1[len] = nodeList[i][k];
                                temp2[len] = nodeRefList[i][k];
                                len++;
                            }
                        }

                        if (nodeRefList[j][0].n == n){
                            Ref r = nodeRefList[j][0];
                            r.n->refPoint[r.refIdx].set(NULL,NULL);
                            for(int k = 1;k < lineLen[j];k++){
                                temp1[len] = nodeList[j][k];
                                temp2[len] = nodeRefList[j][k];
                                len++;
                            }
                        }else{
                            Ref r = nodeRefList[j][lineLen[j]-1];
                            r.n->refPoint[r.refIdx].set(NULL,NULL);
                            for(int k = lineLen[j]-2;k >= 0;k--){
                                temp1[len] = nodeList[j][k];
                                temp2[len] = nodeRefList[j][k];
                                len++;
                            }
                        }
                        lineLen[j] = 0;
                        lineLen[i] = len;
                        for(int k = 0;k < len;k++){
                            nodeList[i][k] = temp1[k];
                            nodeRefList[i][k] = temp2[k];
                            Ref r = nodeRefList[i][k];
                            r.n->refPoint[r.refIdx].set(&nodeList[i][k],&nodeRefList[i][k]);
                        }

                    break;
                    }
                    
                }
                break;
            }
        }
    }
    void insert(Ref r){
        if (!inEdit){
            editLine++;
            inEdit = true;
        }
        int len = lineLen[editLine];
        lineLen[editLine]++;
        nodeRefList[editLine][len] = r;
        (r.n)->refPoint[r.refIdx] = Refed(&nodeList[editLine][len],
                                            &nodeRefList[editLine][len]);
        nodeList[editLine][len] = (r.n)->pos;
    }
    void end_insert(){
        inEdit = false;
    }
    void draw(){
        shader.use();
        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_BLEND);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, road_texture);
        shader.setVec3("selectColor", 1,0,1);
        shader.setFloat("sweepWidth", 30);
        shader.setVec2("viewPort",width,height);
        for(int i = 0;i <= editLine;i++){
            if (lineLen[i] == 0)
                continue;
            //set uniforms
            shader.setInt("num",lineLen[i]);
            shader.setInt("selectIndex", -1);
            shader.setVec2s("nodes",nodeList[i],lineLen[i]);
            shader.setInt("res",res);
            
            //draw curve
            shader.setInt("drawType", 2);
            shader.setInt("level",2);
            shader.setVec3("lineColor",0,0,1);
            glDrawArrays(GL_TRIANGLE_STRIP,0,res+2);

            //draw polyline
            shader.setInt("drawType", 1);
            shader.setInt("level",1);
            shader.setVec3("lineColor",1,0,0);
            glLineWidth(2);
            glDrawArrays(GL_LINE_STRIP,0,res+1);
            //draw points
            shader.setInt("drawType", 0);
            glDrawArrays(GL_POINTS, 0, lineLen[i]);
        }
    
    }
    void load_texture(){
        int width, height, nrChannels;
        glGenTextures(1, &road_texture);
        glBindTexture(GL_TEXTURE_2D, road_texture);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        unsigned char *data = stbi_load("imgs/road_texture.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
            printf("Failed to load texture\n");
        stbi_image_free(data);
    }
    void data_init(int res){
        std::vector<float> idx;
        this->res = res;
        for (int i = 0; i <= res+10; i++)
            idx.push_back(i);
        glGenVertexArrays(1,&VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(res+10), &idx[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
    }
    void load_shader(){
        std::string vertShaderName = "shader/bSpline.vert";
        std::string fragShaderName = "shader/bSpline.frag";
        shader.initShader(vertShaderName.c_str(),fragShaderName.c_str());
    }

};

#endif