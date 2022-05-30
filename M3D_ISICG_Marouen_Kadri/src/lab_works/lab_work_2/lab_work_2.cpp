#include "imgui.h"
#include "lab_work_2.hpp"
#include "utils/read_file.hpp"
#include <iostream>
#include "glm/gtc/type_ptr.hpp"

namespace M3D_ISICG
{
	const std::string LabWork2::_shaderFolder = "src/lab_works/lab_work_2/shaders/";

	LabWork2::~LabWork2() 
	{   
		glDeleteBuffers( 1, &vbo_position ); 
		glDeleteBuffers( 1, &vbo_color );
	    glDeleteBuffers( 1, &vao );    
		glDeleteShader( vertex );
		glDeleteShader( frag );
	}

	bool LabWork2::init()
	{
	//________________________________________________________________________________________________
	  
		Color_Vertices =  {
		Vec3f( 1.0f, 0.0f, 0.0f ),
		Vec3f( 0.0f, 1.0f, 0.0f ), 
		Vec3f( 0.0f, 0.0f, 1.0f ), 
		Vec3f( 1.0f, 0.0f, 1.0f )
	};  
		Position_Vertices = { Vec3f( -0.5f, 0.5f, 0.0f ),
							  Vec3f( 0.5f, 0.5f, 0.0f ),
							  Vec3f( 0.5, -0.5f, 0.0f ),
							  Vec3f( -0.5f, -0.5f, 0.0f ) };
		indices= { 0, 1, 3, 1, 2, 3 };  

	//________________________________________________________________________________________________
		
		std::cout << "Initializing lab work 2..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		const std::string vertexShaderStr	= readFile( _shaderFolder + "lw2.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw2.frag" );
		vertex = glCreateShader( GL_VERTEX_SHADER );
		frag   = glCreateShader( GL_FRAGMENT_SHADER );
		const char * Vshader = vertexShaderStr.c_str();
		const char * Fshader = fragmentShaderStr.c_str();
		glShaderSource( vertex, 1, &Vshader, NULL );
		glShaderSource( frag, 1, &Fshader, NULL );
		glCompileShader( vertex );
		glGetShaderiv( vertex, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vertex, sizeof( log ), NULL, log );

			std ::cerr << " Error compiling vertex shader : " << log << std ::endl;
			return false;
		}
		glCompileShader( frag );
		glGetShaderiv( vertex, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vertex, sizeof( log ), NULL, log );
			glDeleteShader( vertex );
			glDeleteShader( frag );
			std ::cerr << " Error compiling vertex shader : " << log << std ::endl;
			return false;
		}
	    shaderProgram = glCreateProgram();
		glAttachShader( shaderProgram, vertex );
		glAttachShader( shaderProgram, frag );
		glLinkProgram( shaderProgram );
		glGetProgramiv( shaderProgram, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( shaderProgram, sizeof( log ), NULL, log );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return false;
		}  
//_________________________________________________________________________________________________
//      Initialisation des Buffers : vao & vbo_position & vbo_color
//____________________vbo Position___________________________________________________________________________
		glCreateBuffers( 1, &vbo_position ); 
		glNamedBufferData( vbo_position, sizeof(Vec3f)*Position_Vertices.size(),Position_Vertices.data(), GL_STATIC_DRAW );
//___________________ebo__________________________________________________________________________
		glCreateBuffers( 1, &ebo );
		glNamedBufferData( ebo, sizeof( unsigned int)*6, indices.data(), GL_STATIC_DRAW );
//____________________vbo color____________________________________________________________________
		glCreateBuffers( 1, &vbo_color );
		glNamedBufferData( vbo_color, sizeof(Vec3f)*Color_Vertices.size(),Color_Vertices.data(), GL_STATIC_DRAW );
//_______________VAO_______________________________________________________________________________
		glCreateVertexArrays( 1, &vao );
		glEnableVertexArrayAttrib( vao, 0 );
		glEnableVertexArrayAttrib( vao, 1 ); 
		glVertexArrayAttribFormat( vao, 0, 3, GL_FLOAT, GL_FALSE, 0 );  
//_____________vao & vbo_position__________________________________________________________________
		glVertexArrayVertexBuffer( vao, 0,vbo_position, 0, 3 * sizeof( float ) ); 
	   	glVertexArrayAttribBinding( vao, 0, 0 );
		glVertexArrayElementBuffer( vao, ebo ); 
//_____________vao & vbo_color____________________________________________________________________
		glVertexArrayVertexBuffer( vao, 1,vbo_color, 0, 3 * sizeof( float ) );
		glVertexArrayAttribBinding( vao, 1, 1 );
		glUseProgram( shaderProgram );  
//_________________________________________________________________________________________________
		
		BrightnessUniformLocation = glGetUniformLocation( shaderProgram, "uBrightness" );
		glProgramUniform1f( shaderProgram, BrightnessUniformLocation, Brightness );
		
//__________________________________________________________________________________________________
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork2::animate( const float p_deltaTime ) 
	{     

	      _time += p_deltaTime;     
		  TranslationXUniformLocation = glGetUniformLocation( shaderProgram, "TranslationX" );
		  const float t = glm::sin( _time ); 
		  glProgramUniform1f( shaderProgram, TranslationXUniformLocation, t );
		    
	}

	void LabWork2::render()
	{
		
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // bind the vao
			glBindVertexArray( vao);
			glDrawElements( GL_TRIANGLES,indices.size(), GL_UNSIGNED_INT, 0);  
			glBindVertexArray( 0 ); // unbind the VAO 

		
	}

	void LabWork2::handleEvents( const SDL_Event & p_event )
	{  

	}

	void LabWork2::displayUI()
	{
		ImGui::Begin( "Settings lab work 2" );  
		if ( ImGui::SliderFloat( "Brightness", &Brightness, 0.f, 2.f ) )
		{
			BrightnessUniformLocation = glGetUniformLocation( shaderProgram, "uBrightness" );
			glProgramUniform1f( shaderProgram, BrightnessUniformLocation, Brightness );

		}   
	  if ( ImGui::ColorEdit3( "Backround", glm::value_ptr( _bgColor ) ) )
		{
			glClearColor(_bgColor.x,_bgColor.y,_bgColor.z,_bgColor.w) ;
		} 
	
	std::cout << "The Brightness Color " << Brightness << std::endl;
		ImGui::End();  
			
	}   
	

} // namespace M3D_ISICG
