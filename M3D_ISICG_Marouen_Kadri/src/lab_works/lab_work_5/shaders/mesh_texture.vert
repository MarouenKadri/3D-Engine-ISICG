
#version 450
layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexNormal;
layout( location = 2 ) in vec2 aVertexTexCoords;
layout( location = 3 ) in vec3 aVertexTangent;
layout( location = 4 ) in vec3 aVertexBitagent;


uniform mat4 uMVPMatrix; // Projection * View * Model
uniform mat4 uModelMatrix;
uniform mat3 uNormalMatrix;
uniform mat4 uViewMatrix;

uniform vec3 LightPos;

out vec3 Normal ; 
out vec3 FragPos;
out vec2 textureCoords;
out mat3 TBN;  
out vec3 lightPos;

void main()
{    

    Normal =  mat3(uNormalMatrix) * aVertexNormal ;  
	FragPos = vec3(  uViewMatrix*uModelMatrix * vec4(aVertexPosition, 1.f) );
	lightPos = vec3(  uViewMatrix*vec4(LightPos, 1.f) );

	
	vec3 T = normalize(vec3(uViewMatrix*uModelMatrix * vec4(aVertexTangent, 0.f)));
	vec3 N = normalize(vec3(uViewMatrix*uModelMatrix * vec4(aVertexNormal, 0.f)));
	vec3 B = normalize(vec3(uViewMatrix*uModelMatrix * vec4(aVertexBitagent, 0.f)));	

	TBN = mat3(T, B, N);   

	

	textureCoords = aVertexTexCoords;  
	gl_Position = uMVPMatrix * vec4( aVertexPosition, 1.f );
}
