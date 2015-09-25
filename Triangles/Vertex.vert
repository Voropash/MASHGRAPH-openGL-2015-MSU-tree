#version 420

uniform mat4 modelViewMatrix;

//inverse and transpose matrix for normals
uniform mat4 normalMatrix;

//projectionMatrix*modelViewMatrix
uniform mat4 modelViewProjectionMatrix;

uniform	vec3	lightPos;
uniform	vec3	eyePos;

uniform int ten;

//input vertex: position, normal, texture coordinates
in vec3 pos;
in vec3 nor;
in vec2 tex;

//output vertex to fragment shader
out VertexData
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
	vec3 l;
	vec3 v;
} VertexOut;


void main()
{
	gl_Position = modelViewProjectionMatrix*vec4(pos.x,pos.y,pos.z,1);
	
	vec3 n1 = nor;
	vec3 p = vec3( modelViewMatrix*vec4(pos.xyz,1));
	//vec3 l = vec3( modelViewMatrix*vec4(lightPos.xyz,1));
	//vec3 e = vec3( modelViewMatrix*vec4(eyePos.xyz,1));

	VertexOut.l = normalize ( vec3 ( lightPos ) - p );                    // vector to light source
    VertexOut.v = normalize ( vec3 ( eyePos )   - p );                    // vector to the eye
   
	VertexOut.position = vec3(modelViewProjectionMatrix*vec4(pos.xyz,1));
	VertexOut.normal = normalize (vec3(normalMatrix*vec4(nor.xyz,1)));
	VertexOut.texcoord = vec2(tex.xy);
	//nn = normalize (vec3(normalMatrix*vec4(nor.xyz,1)));
}
