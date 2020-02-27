#version 460 core
#define MAXNUM 100
layout (location = 0) in float idx;
uniform vec2 nodes[MAXNUM];
uniform int num;
uniform int res;
uniform int level;
uniform int drawType;
uniform vec3 lineColor;
uniform int selectIndex;
uniform vec3 selectColor;
uniform float sweepWidth;
uniform vec2 viewPort;
out vec2 lineCenter;
out float smoothLine;
out vec4 color;
out float radius;
out float lineWidth;
out float idxf;
vec2 d[MAXNUM];
float t[MAXNUM];
int len;
int num_plus = num + 2;
vec2 get_node(int idx){
    if (idx > num)
        return nodes[num-1];
    else if (idx == 0)
        return nodes[0];
    else
        return nodes[idx - 1];
}
//https://en.wikipedia.org/wiki/De_Boor%27s_algorithm
vec3 De_Boor(float x, int p){
    int k;
    for (k = p;k < len-p;k++)
        if (x >= t[k] && x <= t[k+1])
            break;
    for (int j = 0;j < p+1;j++)
        d[j] = get_node(j+k-p);
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
vec2 pix2coord(vec2 node){
    vec2 p = (node.xy/viewPort)*2;
    return vec2(p.x-1, 1-p.y);
}
void main(){
    smoothLine = 0;
    if (drawType == 0){
        vec2 node = nodes[int(idx)];
        gl_Position = vec4(pix2coord(node), 0, 1);
        gl_PointSize = 15;
        color = vec4(1, 0, 0, 1);
        radius = 0.7;
        return;
    }
    radius = 0;
    color = vec4(lineColor,1);
    int p = level;
    if (p == 0){
        gl_Position = vec4(0);
        return;
    }
    for(int i = 0; i <= p;i++)
        t[i] = 0;
    for(int i = p+1;i < num_plus;i++)
        t[i] = i - p;
    for(int i = num_plus;i <= num_plus+p;i++)
        t[i] = num_plus-p;
    len = num_plus + p+1;
    
    if (drawType == 1){
        vec3 ans = De_Boor(idx / res * (num_plus-p),p);
        if (ans.z == 1)
            color = vec4(selectColor,1);
        gl_Position = vec4(pix2coord(ans.xy), 0, 1.0);
        return;
    }

    if (drawType == 2){
        float i1 = int(idx) / 2 * 2;
        float i2 = i1+2;
        float dir = -1;
        if (int(idx) % 2 == 0)
            dir = 1;
        if (i2 > res){
            i2 = i1 - 2;
            dir = -dir;
        }
        vec3 p1 = De_Boor(i1 / res * (num_plus-p),p);
        vec3 p2 = De_Boor(i2 / res * (num_plus-p),p);
        vec2 d = p2.xy - p1.xy;
        vec2 normal = cross(vec3(0, 0, 1),vec3(d,0)).xy;
        normal = normalize(normal)*sweepWidth;
        gl_Position = vec4(pix2coord(p1.xy+dir*normal),0,1);
        smoothLine = 1;
        lineCenter = vec2(p1.x, viewPort.y-p1.y);
        lineWidth = sweepWidth;
        idxf = i1 / res * (num_plus-p);
        return;
    }
}