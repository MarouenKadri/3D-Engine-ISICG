#include "lab_work_1.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork1::_shaderFolder = "src/lab_works/lab_work_1/shaders/";

	LabWork1::~LabWork1() {
		glDeleteShader( vertex );
		glDeleteShader( frag );
		glDeleteBuffers( 1, &VBO );
		glDeleteBuffers( 1, &VAO );  
	}
	
	bool LabWork1::init()
	{     	
        vertices = { Vec3f( -0.5f, 0.5f,0 ), Vec3f( 0.5f, 0.5f,0 ), Vec3f( 0.5f, -0.5f,0 ) }; 
		std::cout << "Initializing lab work 1..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).  
		const std::string vertexShaderStr = readFile( _shaderFolder + "lw1.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw1.frag" );    
	   vertex=glCreateShader( GL_VERTEX_SHADER ); 
	   frag= glCreateShader(GL_FRAGMENT_SHADER );   
	  const char* Vshader = vertexShaderStr.c_str();
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
	  glAttachShader( shaderProgram,frag);
	  glLinkProgram( shaderProgram );
	  glGetProgramiv( shaderProgram, GL_LINK_STATUS, &linked );
	  if ( !linked )
	  {
		  GLchar log[ 1024 ];
		  glGetProgramInfoLog(shaderProgram, sizeof( log ), NULL, log );
		  std ::cerr << " Error linking program : " << log << std ::endl;
		  return false;
	  }
	  glGenVertexArrays( 1, &VAO );
	  glGenBuffers( 1, &VBO );
	  glBindVertexArray( VAO );
	  glBindBuffer( GL_ARRAY_BUFFER, VBO );
	  glBufferData( GL_ARRAY_BUFFER, sizeof(Vec3f)*vertices.size(), vertices.data(), GL_STATIC_DRAW );
	  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void *)0 );
	  glEnableVertexAttribArray( 0 );
	  glBindBuffer( GL_ARRAY_BUFFER, 0 );
	  glBindVertexArray( 0 );
	
	 glUseProgram( shaderProgram );
		
	 	std::cout << "hey From labwork 1" << std::endl;
	glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
	std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork1::animate( const float p_deltaTime ) {}

	void LabWork1::render() {
     
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glBindVertexArray( VAO);              // binding of the VAO 

	glDrawArrays( GL_TRIANGLES, 0, 3 );
	glBindVertexArray( 0 );          // unbind the VAO 

	
	}

	void LabWork1::handleEvents( const SDL_Event & p_event ) {}

	void LabWork1::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

} // namespace M3D_ISICG
