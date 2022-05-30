#include "lab_work_4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "utils/random.hpp"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork4::_shaderFolder = "src/lab_works/lab_work_4/shaders/";

	LabWork4::~LabWork4()
	{
		_bunny.cleanGL();
		glDeleteProgram( _program );
	}

	bool LabWork4::init()
	{
		std::cout << "Initializing lab work 4..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		glEnable( GL_DEPTH_TEST );

		if ( !_initProgram() )
			return false;

		_initCamera();


		_bunny.load( "Conference", "data/models/conference/conference.obj" );

		//_bunny.load( "Conference", "data/models/bunny/bunny.obj" );
		_bunny._transformation = glm::scale( _bunny._transformation, Vec3f( 0.03f, 0.03f, 0.03f ) );

		
		glUseProgram( _program );
		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork4::animate( const float p_deltaTime ) {}

	void LabWork4::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Clear the color buffer.
		GLuint uniformLocationCameraPos = glGetUniformLocation( _program, "CameraPos" );
		glProgramUniform3fv( _program, uniformLocationCameraPos, 1, glm::value_ptr( _camera.getPosition() ) );
		// Light Position
		glProgramUniform3fv(
			_program, glGetUniformLocation( _program, "LightPos" ), 1, glm::value_ptr( lightPosition ) );
		// Color Light
		glProgramUniform3fv(
			_program, glGetUniformLocation( _program, "LightColor" ), 1, glm::value_ptr( lightColor ) );
		
		

		_uMVPMatrixLoc	  = glGetUniformLocation( _program, "uMVPMatrix" );
		_uNormalMatrixLoc = glGetUniformLocation( _program, "uNormalMatrix" );
		_uViewMatrixLoc	  = glGetUniformLocation( _program, "uViewMatrix" );
		_uModelMatrixLoc  = glGetUniformLocation( _program, "uModelMatrix" );
		Mat4f mvp		  = _camera.getProjectionMatrix() * _camera.getViewMatrix() * _bunny._transformation;
		Mat3f NormalMatrix= Mat3f( glm::transpose( glm::inverse( _camera.getViewMatrix() * _bunny._transformation ) ) );
		glProgramUniformMatrix3fv( _program, _uNormalMatrixLoc, 1, GL_FALSE, glm::value_ptr( NormalMatrix ) );
		glProgramUniformMatrix4fv( _program, _uMVPMatrixLoc, 1, GL_FALSE, glm::value_ptr( mvp ) );
		glProgramUniformMatrix4fv( _program, _uViewMatrixLoc, 1, GL_FALSE, glm::value_ptr( _camera.getViewMatrix() ) );
		glProgramUniformMatrix4fv( _program, _uModelMatrixLoc, 1, GL_FALSE, glm::value_ptr( _bunny._transformation ) );
		glProgramUniformMatrix4fv(_program, _uProjectionMatrixLoc, 1, GL_FALSE, glm::value_ptr( _camera.getProjectionMatrix() ) );
		
		 
		_bunny.render( _program );


	}

	void LabWork4::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				break;
			case SDL_SCANCODE_SPACE: // Print camera info
				_camera.print();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
		}
	}

	void LabWork4::displayUI()
	{
		ImGui::Begin( "Settings lab work 4" );



		if ( ImGui::DragFloat3( "Light Position", &lightPosition.x, 0.05f ) )
			{

			glProgramUniform3fv(
					_program, glGetUniformLocation( _program, "LightPos" ), 1, glm::value_ptr( lightPosition ) );
		  }

		 if ( ImGui::ColorEdit3( "Light Color", &lightColor.r ) )
		{
			 glProgramUniform3fv(_program, glGetUniformLocation( _program, "LightColor" ), 1, glm::value_ptr( lightColor ) );
		}

		// Background.
		if ( ImGui::ColorEdit3( "Background", glm::value_ptr( _bgColor ) ) )
		{
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}

		// Camera.
		if ( ImGui::SliderFloat( "fovy", &_fovy, 10.f, 160.f, "%01.f" ) )
		{
			_camera.setFovy( _fovy );
		}
		if ( ImGui::SliderFloat( "Speed", &_cameraSpeed, 0.1f, 10.f, "%01.1f" ) ) {}   

		// 
		
        if ( ImGui::Checkbox( "Blinn-Phong lighting  Model", (bool *)&Blinn_Phong ) )
			glProgramUniform1i( _program, glGetUniformLocation( _program, "Blinn_Phong" ), Blinn_Phong );   
		 if ( ImGui::Checkbox( "Gaussian lighting Model", (bool *)&Gauss_light ) )
			glProgramUniform1i( _program, glGetUniformLocation( _program, "Gauss_light" ), Gauss_light );   




		

		ImGui::End();
	}

	void LabWork4::resize( const int p_width, const int p_height )
	{
		BaseLabWork::resize( p_width, p_height );
		_camera.setScreenSize( p_width, p_height );
	}

	bool LabWork4::_initProgram()
	{
		// ====================================================================
		// Shaders.
		// ====================================================================
		// Create shaders.
		const GLuint vertexShader	= glCreateShader( GL_VERTEX_SHADER );
		const GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		// Get sources from files.
		const std::string vertexShaderSrc	= readFile( _shaderFolder + "mesh.vert" );
		const std::string fragmentShaderSrc = readFile( _shaderFolder + "mesh.frag" );

		// Convert to GLchar *
		const GLchar * vSrc = vertexShaderSrc.c_str();
		const GLchar * fSrc = fragmentShaderSrc.c_str();

		// Compile vertex shader.
		glShaderSource( vertexShader, 1, &vSrc, NULL );
		glCompileShader( vertexShader );
		// Check if compilation is ok.
		GLint compiled;
		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vertexShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std::cerr << "Error compiling vertex shader: " << log << std::endl;
			return false;
		}

		// Compile vertex shader.
		glShaderSource( fragmentShader, 1, &fSrc, NULL );
		glCompileShader( fragmentShader );
		// Check if compilation is ok.
		glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compiled );
		if ( compiled != GL_TRUE )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( fragmentShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std::cerr << "Error compiling fragment shader: " << log << std::endl;
			return false;
		}
	
		// Create program.
		_program = glCreateProgram();

		// Attach shaders.
		glAttachShader( _program, vertexShader );
		glAttachShader( _program, fragmentShader );
		// Link program.
		glLinkProgram( _program );
		// Check if link is ok.
		GLint linked;
		glGetProgramiv( _program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( _program, sizeof( log ), NULL, log );
			std::cerr << "Error linking program: " << log << std::endl;
			return false;
		}



		GLuint uniformLocationCameraPos = glGetUniformLocation( _program, "CameraPos" );
		glProgramUniform3fv( _program, uniformLocationCameraPos, 1, glm::value_ptr( _camera.getPosition() ) );
		// Light Position
		glProgramUniform3fv(
			_program, glGetUniformLocation( _program, "LightPos" ), 1, glm::value_ptr( lightPosition ) );
		// Color Light
		glProgramUniform3fv(
			_program, glGetUniformLocation( _program, "LightColor" ), 1, glm::value_ptr( lightColor ) );
		glProgramUniform1i( _program, glGetUniformLocation( _program, "Blinn_Phong" ), Blinn_Phong );  

		// Shaders are now useless.
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
		// ====================================================================
		
		

		return true;
	}

	void LabWork4::_initCamera()  


	{   //=========================================
		//  camera position to render scene
		//=========================================
		_camera.setScreenSize( _windowWidth, _windowHeight );
		_camera.setPosition( Vec3f( 12, 12, 12 ) );
	    _camera.setLookAt( Vec3f( 7, 4, 10 ) );
		//=========================================
		//  camera position to render the bunny
		//=========================================
		// _camera.setPosition( Vec3f( 0, 0, 3 ) );
		//_camera.setLookAt( Vec3f( 0, 0, 1 ) );
	}
	
} // namespace M3D_ISICG
