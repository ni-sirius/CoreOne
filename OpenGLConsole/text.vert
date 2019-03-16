#version 450

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 ProjectionMat;

void main()
{
    gl_Position = ProjectionMat * vec4(vertex.xy, 1.0, 1.0);
    TexCoords = vertex.zw;
} 