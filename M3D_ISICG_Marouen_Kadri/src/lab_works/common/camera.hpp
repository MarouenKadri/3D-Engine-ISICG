#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "GL/gl3w.h"
#include "define.hpp"

namespace M3D_ISICG
{
	// Freefly camera.
	class Camera
	{
	  public:
		Camera() {}

		inline const Mat4f & getViewMatrix() const { return _viewMatrix; }
		inline const Mat4f & getProjectionMatrix() const { return _projectionMatrix; }

		void setPosition( const Vec3f & p_position );
		void setLookAt( const Vec3f & p_lookAt );
		void setFovy( const float p_fovy );

		void setScreenSize( const int p_width, const int p_height );

		void moveFront( const float p_delta );
		void moveRight( const float p_delta );
		void moveUp( const float p_delta );
		void rotate( const float p_yaw, const float p_pitch );
		Vec3f getPosition();
		void print() const;

	  private:
		void _computeViewMatrix();
		void _computeProjectionMatrix();
		void _updateVectors();

	  private:
		Vec3f _position		= VEC3F_ZERO;
		Vec3f _invDirection = Vec3f( 0.f, 0.f, 1.f );  // Dw dans le cours.
		Vec3f _right		= Vec3f( -1.f, 0.f, 0.f ); // Rw dans le cours.
		Vec3f _up			= Vec3f( 0.f, 1.f, 0.f );  // Uw dans le cours.
		// Angles defining the orientation in degrees
		float _yaw	 = 90.f;
		float _pitch = 0.f;

		int	  _screenWidth	= 1280;
		int	  _screenHeight = 720;
		float _aspectRatio	= float( _screenWidth ) / _screenHeight;
		float _fovy			= 60.f;
		float _zNear		= 0.1f;
		float _zFar			= 1000.f;

		Mat4f _viewMatrix		= MAT4F_ID;
		Mat4f _projectionMatrix = MAT4F_ID;
	};
} // namespace M3D_ISICG

#endif // __CAMERA_HPP__
