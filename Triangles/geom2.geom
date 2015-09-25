#version 420

uniform mat4 proj;

in vec3 nn [];

layout(triangles, equal_spacing) in;

void main(void)
{
    float   u = gl_TessCoord.x;
    float   v = gl_TessCoord.y;
    float   w = gl_TessCoord.z;
    
    vec4    p = u * gl_in [0].gl_Position + v * gl_in [1].gl_Position + w * gl_in [2].gl_Position;
    vec3    n = normalize ( u * nn [0] + v * nn [1] + w * nn [2] );

    vec4    p0 = p - dot ( p.xyz - gl_in [0].gl_Position.xyz, nn [0] ) * vec4 ( nn [0], 0.0 );
    vec4    p1 = p - dot ( p.xyz - gl_in [1].gl_Position.xyz, nn [1] ) * vec4 ( nn [1], 0.0 );
    vec4    p2 = p - dot ( p.xyz - gl_in [2].gl_Position.xyz, nn [2] ) * vec4 ( nn [2], 0.0 );
    
    gl_Position = proj * ( u * p0 + v * p1 + w * p2 );
}