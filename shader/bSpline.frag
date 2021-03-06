#version 460 core
out vec4 FragColor;
in vec4 color;
in float radius;
in float smoothLine;
in vec2 lineCenter;
in float lineWidth;
in float idxf;
uniform sampler2D sweepTex;
void main(){
    if (radius > 0) {
        vec2 p = (gl_PointCoord - vec2(0.5, 0.5))*2;
        vec4 background = vec4(1,1,1,0);
        float t = smoothstep(radius, 1, length(p));
        FragColor = color*(1-t) + background*t;
    }
    else{
        if (smoothLine == 0)
            FragColor = vec4(color);
        else{
            vec2 p = gl_FragCoord.xy;
            p = p - lineCenter;
            float normal = length(p)/lineWidth;
            vec4 background = vec4(1,1,1,0);
            vec4 texColor = texture(sweepTex,vec2(idxf*3, 0.5-normal/2));
            float t = smoothstep(0.9, 1, normal);
            FragColor = texColor*(1-t) + background*t;
        }
    }
        
}
