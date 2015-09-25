#version 420


in  vec3 n  [];
out vec3 nn [];

layout(vertices = 3) out;

void main(void)
{
    gl_TessLevelInner [0] = 1;
    gl_TessLevelInner [1] = 1;
    gl_TessLevelOuter [0] = 2;
    gl_TessLevelOuter [1] = 2;
    gl_TessLevelOuter [2] = 2;
    gl_TessLevelOuter [3] = 2;

    gl_out [gl_InvocationID].gl_Position = gl_in [gl_InvocationID].gl_Position;
    //nn     [gl_InvocationID]             = n     [gl_InvocationID];
}