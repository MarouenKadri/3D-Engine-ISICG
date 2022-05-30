#version 450
layout( location = 0 ) out vec4 fragColor;



in vec3 fragPos;
in vec3 normal ;
in vec3 lightPos; 

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor ;
uniform float shininess;

uniform vec3 CameraPos ; 
uniform vec3 LightColor;       
uniform bool Blinn_Phong;  
uniform bool Gauss_light ;


void main()
{
	
	vec3 normal=normalize(normal);

	vec3 lightDir  = normalize( lightPos - fragPos );
	vec3 viewDir = normalize(CameraPos - fragPos);  
	
	if (!gl_FrontFacing)
		normal = -normal;

//Diffuse Color
vec3 Diffuse = max ( dot(normal , lightDir ), 0.f ) *diffuseColor ;  

//Specular Color     
vec3 Spec;
vec3 halfwayDir=normalize(lightDir+viewDir) ;


if(Blinn_Phong)      //Blinn-Phong Model 
{   

 Spec = specularColor* pow(max(dot(normal, halfwayDir  ), 0.0),shininess);

}  
else      //Phong Model
{    

vec3 reflectDir = reflect( - lightDir,normal );
Spec =  specularColor *pow(max(dot(viewDir, reflectDir), 0), shininess);

}     
// Gaussian lighting Model   
if(Gauss_light )  
{
float angleNormalHalf=acos(dot(halfwayDir,normal));
float exponent=angleNormalHalf/shininess ;
exponent=-(exponent*exponent) ;
float gaussianTerm=exp(exponent) ;
gaussianTerm=lightDir !=0.0 ?gaussianTerm:0.0 ;
Spec=gaussianTerm*specularColor ;    
}

vec3 color =Diffuse+ambientColor+ Spec  ; 


color*=LightColor;
fragColor = vec4( color , 1);

	
}
