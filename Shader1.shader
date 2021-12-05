//vertex shader
#version 330 core

layout (location = 0) in vec3 attribPos;
void main() {
    gl_Position = vec4(attribPos, 1.0);
}

//fragment shader
#version 330 core
out vec4 FragColor;
void main(){ 
    vec3 col = vec3(0.5,0.1,0.0);
    FragColor = vec4(col, 1.0);
}