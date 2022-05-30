
#ifndef __LAB_WORK_2_HPP__
#define __LAB_WORK_2_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork2 : public BaseLabWork
	{
	  public:
		LabWork2() : BaseLabWork() {}
		~LabWork2();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override; 
	  
		
         
	  private:
		GLuint		  vertex, frag;
		GLint		  shaderProgram;   
		GLint		  compiled;
		GLint		  linked;

		
		unsigned int   vao, ebo;
		unsigned int   vbo_position ,vbo_color;  
		float		   _time = 0;  
		float		   Brightness=1;
	    //_________Uniforms value___________________________
		GLint		   TranslationXUniformLocation=GL_INVALID_INDEX; 
		GLint		   BrightnessUniformLocation=GL_INVALID_INDEX;   
		std::vector<Vec3f> Position_Vertices;
		std::vector<Vec3f> Color_Vertices;		 
		std::vector<unsigned int> indices ;
		


		
		 // Background color 
		// ================  
		Vec4f _bgColor = Vec4f( 1.0f, 0.8f, 0.8f, 1.f );

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_2_HPP__
