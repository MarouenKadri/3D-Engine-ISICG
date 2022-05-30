#ifndef __LAB_WORK_3_HPP__
#define __LAB_WORK_3_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "common/camera.hpp"
#include "define.hpp"
#include <map>
#include <vector>

namespace M3D_ISICG
{
	class LabWork3 : public BaseLabWork
	{
		struct Mesh
		{
			~Mesh()
			{
				if ( _vao != GL_INVALID_INDEX )
				{
					glDisableVertexArrayAttrib( _vao, 0 );
					glDisableVertexArrayAttrib( _vao, 1 );
					glDeleteVertexArrays( 1, &_vao );
				}
				if ( _vboPositions != GL_INVALID_INDEX )
					glDeleteBuffers( 1, &_vboPositions );
				if ( _vboColors != GL_INVALID_INDEX )
					glDeleteBuffers( 1, &_vboColors );
				if ( _ebo != GL_INVALID_INDEX )
					glDeleteBuffers( 1, &_ebo );
			}
			// ================ Geometric data.
			std::vector<Vec3f>		  _vertices;
			std::vector<Vec3f>		  _vertexColors;
			std::vector<unsigned int> _indices;
			Mat4f					  _transformation = MAT4F_ID;
			// ================

			// ================ GL data.
			GLuint _vao = GL_INVALID_INDEX; // Vertex Array Object
			GLuint _ebo = GL_INVALID_INDEX; // Element Buffer Object
			// Vertex Buffer Objects.
			GLuint _vboPositions = GL_INVALID_INDEX;
			GLuint _vboColors	 = GL_INVALID_INDEX;
			
		};

	  public:
		LabWork3() : BaseLabWork() {}
		~LabWork3();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

		void resize( const int p_width, const int p_height ) override;

	  private:
		bool _initProgram();
		void _initCamera();
		void _initBuffers();
		void _updateViewMatrix();
		void _updateProjectionMatrix();

		// Create a mesh representing a unit cube centerd in (0,0,0)
		Mesh _createCube();

	  private:
		// ================ Scene data.
		Mesh   _cube;
		Mesh   _cube2;
		Camera _camera;
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		// Camera
		float _cameraSpeed		 = 0.1f;
		float _cameraSensitivity = 0.1f;
		float _fovy				 = 60.f;
		// ================
		bool Rotation_cube1 = true; 
		bool Cam_Rotation	= false;


		// ================ GL program data.
		GLuint _program = GL_INVALID_INDEX;

		GLint _uModelMatrixLoc		= GL_INVALID_INDEX;
		GLint _uViewMatrixLoc		= GL_INVALID_INDEX;
		GLint _uProjectionMatrixLoc = GL_INVALID_INDEX;
		// ================
		float _time = 0;

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_3_HPP__
