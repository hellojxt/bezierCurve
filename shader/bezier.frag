#version 460 core
out vec4 FragColor;
in vec4 color;
in float radius;
void main(){
    if (radius > 0) {
        vec2 p = (gl_PointCoord - vec2(0.5, 0.5))*2;
        vec4 background = vec4(0);
        float t = smoothstep(radius, 1, length(p));
        FragColor = color*(1-t) + background*t;
    }
    else
        FragColor = vec4(color);
}
