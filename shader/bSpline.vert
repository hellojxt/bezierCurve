#version 460 core
layout (location = 0) in float idx;
uniform vec2 nodes[10];
uniform int num;
uniform int res;
uniform bool drawPoints;
out vec3 color;
out float radius;
void main(){
    if (drawPoints){
        vec2 node = nodes[int(idx)];
        gl_Position = vec4(node.x, node.y, 0, 1);
        gl_PointSize = 15;
        color = vec3(1, 0, 0);
        radius = 0.7;
        return;
    }
    //https://en.wikipedia.org/wiki/De_Boor%27s_algorithm
    vec2 d[20];
    float t[20];
    int p = 2;
    for(int i = 0; i <= p;i++)
        t[i] = 0;
    for(int i = p+1;i < num;i++)
        t[i] = i - p;
    for(int i = num;i <= num+p;i++)
        t[i] = num-p;
    int len = num + p+1;
    float x = idx / res * (num-p);
    int k;
    for (k = 0;k < len-1;k++)
        if (x >= t[k] && x < t[k+1])
            break;
    for (int j = 0;j < p+1;j++)
        d[j] = nodes[j+k-p];
    for(int r = 1;r < p+1;r++)
        for(int j = p;j > r-1;j--){
            float alpha = (x - t[j+k-p]) / (t[j+1+k-r] - t[j+k-p]);
            d[j] = (1.0 - alpha) * d[j-1] + alpha * d[j];
        }
    gl_Position = vec4(d[p], 0, 1.0);
    color = vec3(0, 0, 1);
    radius = 0;
}