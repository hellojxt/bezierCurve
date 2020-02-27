#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<cstdio>
#include<utils/shader.h>
#include<utils/helper.h>
#include<modes/BSpline.h>
#include<vector>


GLFWwindow * window;
std::vector<Mode*> shaders;
node nodeList[1000];
int nodeMaxIdx = -1;
int selectNodeIdx = -1;
Mode* currentShader;

glm::vec2 getMousePos(){
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return glm::vec2(xpos,ypos);
}
void resize_callback(GLFWwindow * window, int width, int height){
    glViewport(0,0,width,height);
    for(unsigned int i = 0;i < shaders.size();i++)
        shaders[i]->resize(width,height);
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    glm::vec2 pos = getMousePos();
    if (selectNodeIdx >= 0){
        nodeList[selectNodeIdx].pos = pos;
        nodeList[selectNodeIdx].update();
    }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    glm::vec2 pos = getMousePos();
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        if (selectNodeIdx >= 0){
            selectNodeIdx = -1;
            currentShader->end_insert();
        }
            
        for (int i = 0;i <= nodeMaxIdx;i++){
            if (glm::length(pos - nodeList[i].pos) < 10){
                selectNodeIdx = i;
                break;
            }
        }
        
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        if (selectNodeIdx >= 0){
            for(int i = 0;i < nodeMaxIdx;i++){
                if (glm::length(pos - nodeList[i].pos) < 10 && i != selectNodeIdx){
                    
                }
            }
        }
        selectNodeIdx = -1;
    }
        
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE){
        printf("selectNodeIdx:%d",selectNodeIdx);
        nodeMaxIdx++;
        nodeList[nodeMaxIdx].pos = pos;
        nodeList[nodeMaxIdx].addRef(currentShader);
        if (selectNodeIdx == -1){
            nodeMaxIdx++;
            nodeList[nodeMaxIdx].pos = pos;
            nodeList[nodeMaxIdx].addRef(currentShader);
        }
        selectNodeIdx = nodeMaxIdx;
        printf("to %d\n",selectNodeIdx);
    }
}
int main(){
    //initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int width = 800;
    int height = 800;
    //create window
    window = glfwCreateWindow(width, height, "bezier curve", NULL, NULL);
    if (window == NULL){
        printf("Failed to create GLFW window.");
        glfwTerminate();
        return -1;

    }
    glfwMakeContextCurrent(window);
    //initialize GLAD
    if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))){
        printf("Fail to initialize GLAD.");
        return -1;
    }
    //map from the range (-1 to 1) to (0, width) and (height, 0)
    glViewport(0,0,width,height);
    glfwSetFramebufferSizeCallback(window, resize_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);


    BsplineMode bspline = BsplineMode();
    shaders.push_back(&bspline);
    currentShader = &bspline;
    for(unsigned int i = 0;i < shaders.size();i++)
        shaders[i]->resize(width,height);
    while(!glfwWindowShouldClose(window)){
        //rendering
        glClearColor(255,255,255,1);
        glClear(GL_COLOR_BUFFER_BIT);
        for (unsigned int i = 0;i < shaders.size();i++){
            shaders[i]->draw();
        }
            
        //swap the buffers, check and call events
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }
    glfwTerminate();
        
}
    