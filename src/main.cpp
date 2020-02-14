#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<cstdio>
#include<utils/shader.h>
#include<utils/helper.h>
#include<vector>

class BezierCurve{
    public:
    GLFWwindow * window;
    Shader curveShader;
    unsigned int VAO,VBO;
    int res;
    int width,height;
    int seletNode = -1;
    std::vector<node> nodeList;
    BezierCurve(int w, int h, int r){
        if (init(w, h) == -1){
            printf("Initialize error");
            return;
        }
        glfwSetWindowUserPointer(window,this);
        glfwSetFramebufferSizeCallback(window, resize_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        curveShader.initShader("shader/bezier.vert","shader/bezier.frag");
        data_init(r);
    }
    node getMousePos(){
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return node(xpos/width*2-1,ypos/height*2-1);
    }
    static void resize_callback(GLFWwindow * window, int width, int height){
        glViewport(0,0,width,height);
    }
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        BezierCurve *curve = (BezierCurve *)glfwGetWindowUserPointer(window);
        node pos = curve->getMousePos();
    }
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        BezierCurve *curve = (BezierCurve *)glfwGetWindowUserPointer(window);
        node pos = curve->getMousePos();
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
            for (unsigned int i = 0;i < curve->nodeList.size();i++){
                if (pos.dist(curve->nodeList[i]) < 0.01){
                    curve->seletNode = i;
                    break;
                }
            }
        }
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            curve->seletNode = -1;
    }
    int init(int width, int height){
        this->width = width;
        this->height = height;
        //initialize GLFW
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
        //map from the range (-1 to 1) to (0, width) and (0, height)
        glViewport(0,0,width,height);
        return 0;
    }
    void data_init(int res){
        std::vector<float> idx;
        this->res = res;
        for (int i = 0; i <= res; i++)
            idx.push_back(i);
        nodeList.push_back(node(-0.5,-0.5));

        glGenVertexArrays(1,&VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(res+1), &idx[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
    }

    void loop(){
        
        while(!glfwWindowShouldClose(window)){
            //rendering
            glClearColor(255,255,255,1);
            glClear(GL_COLOR_BUFFER_BIT);
            
            curveShader.use();
            glEnable(GL_PROGRAM_POINT_SIZE);
            curveShader.setBool("drawPoints", true);
            glDrawArrays(GL_POINTS, 0, nodeList.size());

            curveShader.setBool("drawPoints", false);
            curveShader.setVec2s("nodes",nodeList);
            curveShader.setInt("num",nodeList.size());
            curveShader.setInt("res",res);
            glBindVertexArray(VAO);
            glDrawArrays(GL_LINE_STRIP,0,res);
            

            
            //swap the buffers, check and call events
            glfwSwapBuffers(window);
            glfwWaitEvents();
        }
        glfwTerminate();

    }    

};

int main(){
    BezierCurve curve = BezierCurve(800, 600, 1000);
    curve.loop();
}