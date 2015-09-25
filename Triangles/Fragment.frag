//shader version
#version 420


//mode of drawing
//if is true, then use Texture
//otherwise draw gradient
uniform int useTexture;

//texture object
uniform sampler2D textureSampler;

uniform	vec3 temn;

//retrieve this data form vertex shader
in VertexData
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
	vec3 l;
	vec3 v;
} VertexIn;

out vec4 fragColor;

//TODO: you should use VertexIn.normal value to evaluate Phong Lightning for this pixel
// 
		
void main()
{
	if (useTexture>0)
		//take color from texture using texture2D
		{
		vec3 t = texture(textureSampler,VertexIn.texcoord.xy).rgb;


		const vec4  stdblack = vec4 ( 0.05, 0.2, 0.1, 1.0 );
		const vec4  diffColor = vec4 ( 0.4, 0.59, 0.07, 1.0 );
		const vec4  specColor = vec4 ( 0.25, 0.3, 0.0, 1.0 );
		const float specPower = 5.0;



		vec3 n2   = normalize ( VertexIn.normal );
		vec3 l2   = normalize ( VertexIn.l );
		vec3 v2   = normalize ( VertexIn.v );
		vec3 r    = reflect ( -v2, n2 );
		vec4 diff = diffColor * max ( dot ( n2, l2 ), 0.0 );
		vec4 spec = specColor * pow ( max ( dot ( l2, r ), 0.0 ), specPower );

		fragColor =  stdblack+diff + spec;
		
		//const float a = (0.3*fragColor.x+0.59*fragColor.y+0.11*fragColor.z)/(0.3*t.x+0.59*t.y+0.11*t.z);

		fragColor = vec4(1.7*t.x*(0.3*fragColor.x+0.59*fragColor.y+0.11*fragColor.z)-temn.x+0.1,1.7*t.y*(0.3*fragColor.x+0.59*fragColor.y+0.11*fragColor.z)-temn.y+0.2,1.7*t.z*(0.3*fragColor.x+0.59*fragColor.y+0.11*fragColor.z)-temn.z+0.1,length(VertexIn.normal)*length(VertexIn.position));



	}
	else
	{
		const vec4  stdblack = vec4 ( 0.05, 0.2, 0.1, 1.0 );
		const vec4  diffColor = vec4 ( 0.4, 0.59, 0.07, 1.0 );
		const vec4  specColor = vec4 ( 0.25, 0.3, 0.0, 1.0 );
		const float specPower = 5.0;

		vec3 n2   = normalize ( VertexIn.normal );
		vec3 l2   = normalize ( VertexIn.l );
		vec3 v2   = normalize ( VertexIn.v );
		vec3 r    = reflect ( -v2, n2 );
		vec4 diff = diffColor * max ( dot ( n2, l2 ), 0.0 );
		vec4 spec = specColor * pow ( max ( dot ( l2, r ), 0.0 ), specPower );

		fragColor = stdblack + diff + spec;
	}
}
