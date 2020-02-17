#version 460 core
out vec4 FragColor;
in vec3 color;
in float radius;
void main(){
    if (radius > 0) {
        vec2 p = (gl_PointCoord - vec2(0.5, 0.5))*2;
        vec3 background = vec3(1);
        float t = smoothstep(radius, 1, length(p));
        FragColor = vec4(color*(1-t) + background*t, 1);
    }
    else
        FragColor = vec4(color,1);
}
