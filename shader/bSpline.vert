#version 460 core
layout (location = 0) in float idx;
uniform vec2 nodes[10];
uniform int num;
uniform int res;
uniform int level;
uniform int drawType;
uniform vec3 lineColor;
uniform int selectIndex;
uniform vec3 selectColor;
out vec4 color;
out float radius;

vec2 d[20];
float t[20];
int len;

//https://en.wikipedia.org/wiki/De_Boor%27s_algorithm
vec3 De_Boor(float x, int p){
    int k;
    for (k = p;k < len-p;k++)
        if (x >= t[k] && x <= t[k+1])
            break;
    for (int j = 0;j < p+1;j++)
        d[j] = nodes[j+k-p];
    for(int r = 1;r < p+1;r++)
        for(int j = p;j > r-1;j--){
            float alpha = (x - t[j+k-p]) / (t[j+1+k-r] - t[j+k-p]);
            d[j] = (1.0 - alpha) * d[j-1] + alpha * d[j];
    }
    float select = 0;
    if (selectIndex >= k-p && selectIndex <=k)
        select = 1;
    return vec3(d[p],select);
}

void main(){
    if (drawType == 0){
        vec2 node = nodes[int(idx)];
        gl_Position = vec4(node.x, node.y, 0, 1);
        gl_PointSize = 15;
        color = vec4(1, 0, 0, 1);
        radius = 0.7;
        return;
    }
    radius = 0;
    color = vec4(lineColor,1);
    int p = level;
    if (p >= num)
        p = num-1;
    if (p == 0){
        gl_Position = vec4(0);
        return;
    }
    for(int i = 0; i <= p;i++)
        t[i] = 0;
    for(int i = p+1;i < num;i++)
        t[i] = i - p;
    for(int i = num;i <= num+p;i++)
        t[i] = num-p;
    len = num + p+1;
    
    if (drawType == 1){
        vec3 ans = De_Boor(idx / res * (num-p),p);
        if (ans.z == 1)
            color = vec4(selectColor,1);
            
        gl_Position = vec4(ans.xy, 0, 1.0);
        return;
    }
    
    if (drawType == 2){
        float i1 = int(idx) / 2 * 2;
        float i2 = i1+2;
        if (i2 > res){
            i1 = i1 - 2;
            i2 = i1 + 2;
        }
        float dir = -1;
        if (int(idx) % 2 == 0)
            dir = 1;
        vec3 p1 = De_Boor(i1 / res * (num-p),p);
        vec3 p2 = De_Boor(i2 / res * (num-p),p);
        vec2 d = p2.xy - p1.xy;
        vec2 normal = cross(vec3(0, 0, 1),vec3(d,0)).xy;
        normal = normalize(normal)*0.1;
        gl_Position = vec4(p1.xy+dir*normal,0,1);
        return;
    }
}