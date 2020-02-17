#version 460 core
layout (location = 0) in float idx;
uniform vec2 nodes[10];
uniform int num;
uniform int res;
uniform int level;
uniform bool drawPoints;
uniform vec3 lineColor;
uniform int selectIndex;
uniform vec3 selectColor;
out vec4 color;
out float radius;
void main(){
    if (drawPoints){
        vec2 node = nodes[int(idx)];
        gl_Position = vec4(node.x, node.y, 0, 1);
        gl_PointSize = 15;
        color = vec4(1, 0, 0, 1);
        radius = 0.7;
        return;
    }
    float t = idx / res;
    vec2 list1[10];
    vec2 list2[10];
    if (level == 1){
        vec2 ans = nodes[int(t*num)];
        gl_Position = vec4(ans.x, ans.y, 0, 1.0);
    }else{
        for (int i = 0;i < num;i++)
            list1[i] = nodes[i];
        for (int i = num;i > 1;i--){
            for (int j = 0;j < i-1;j++)
                list2[j] = list1[j]*(1-t) +list1[j+1]*t;
            for (int j = 0;j < i-1;j++)
                list1[j] = list2[j]; 
        }
        //int x = int(t*2);
        vec2 ans = list1[0];
        gl_Position = vec4(ans.x, ans.y, 0, 1.0);
    }
    
    if (selectIndex >= 0)
        color = vec4(selectColor,1);
    else
        color = vec4(lineColor,1);
    radius = 0;
}