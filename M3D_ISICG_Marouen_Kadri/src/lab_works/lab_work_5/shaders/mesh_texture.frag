#version 450
in vec3 FragPos;
in vec3 Normal ;
in vec2 textureCoords;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor ;
uniform float shininess;
uniform bool NormalMaps ;  
uniform bool Blinn_Phong;  
uniform bool Gauss_light ;


uniform vec3 CameraPos ; 

layout( location = 0 ) out vec4 fragColor;

uniform bool uHasDiffuseMap;
layout(binding = 1) uniform sampler2D uDiffuseMap;
uniform bool uHasAmbientMap;
layout(binding = 2) uniform sampler2D uAmbientMap;
uniform bool uHasSpecularMap;
layout(binding = 3) uniform sampler2D uSpecularMap;
uniform bool uHasShininessMap;
layout(binding = 4) uniform sampler2D uShininess;
uniform bool uHasNormalMap;
layout( binding = 5 ) uniform sampler2D uNormalMap;

in mat3 TBN  ;
in vec3 lightPos;
uniform vec3 LightColor;
uniform vec3 LightPos;


void main()
{
	if(uHasDiffuseMap && texture(uDiffuseMap, textureCoords).w < 0.5f)
		discard;      

		vec3 nrm = normalize(Normal);

	if(uHasNormalMap && NormalMaps)
	{
		nrm = vec3(texture(uNormalMap, textureCoords));
		nrm = normalize(nrm * 2.0 - 1.0);
		nrm = normalize(TBN * nrm);
	}   
	if (!gl_FrontFacing)
		nrm = -nrm;

		
	vec3 lightDir  = normalize( lightPos- FragPos );
	vec3 viewDir = normalize(CameraPos - FragPos); 
//==============================================================================
//       Ambient 
//==============================================================================

	vec3 ambient = ambientColor;
	if (uHasAmbientMap)
	{
	ambient = vec3(texture(uAmbientMap, textureCoords));   
	}    
//===============================================================================
//      Diffuse
//===============================================================================    


	vec3 diff =diffuseColor;
	if (uHasDiffuseMap)
	{
	 diff = vec3(texture(uDiffuseMap, textureCoords));  
	}    

	diff= max ( dot(nrm , lightDir ), 0.f ) *diff ;  
//================================================================================
//    shininess  
//================================================================================  


float Shininess =shininess;
	if (uHasShininessMap)
	{
		Shininess = texture(uShininess, textureCoords).x;
	}
	
//================================================================================
//     Speculaire 
//================================================================================
vec3 halfwayDir=normalize(lightDir+viewDir) ;

vec3 speculair = specularColor;
	if (uHasSpecularMap)
	{
	 speculair = vec3(texture(uSpecularMap, textureCoords)).xxx;
	}     

vec3 Spec ;
	
if(Blinn_Phong)      //Blinn-Phong Model 
{   

Spec = speculair* pow(max(dot(nrm, halfwayDir  ), 0.0),Shininess);

}  
else      //Phong Model
{    

vec3 reflectDir = reflect( - lightDir,nrm );
Spec =  speculair *pow(max(dot(viewDir, reflectDir), 0), Shininess);

}       
//================================================================================
//     Gaussian lighting model
//================================================================================
if(Gauss_light )  
{
float angleNormalHalf=acos(dot(halfwayDir,nrm));
float exponent=angleNormalHalf/Shininess ;
exponent=-(exponent*exponent) ;
float gaussianTerm=exp(exponent) ;
gaussianTerm=lightDir !=0.0 ?gaussianTerm:0.0 ;
Spec=gaussianTerm*speculair ;    
}

// Color composition
vec3 result =  vec3(diff)+ Spec+ambient ;
result *= LightColor;
fragColor = vec4(result, 1);
}

