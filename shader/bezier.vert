#version 460 core
layout (location = 0) in float t;
uniform vec2 nodes[10];
uniform int num;

void main(){
    vec2 list1[10];
    vec2 list2[10];
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