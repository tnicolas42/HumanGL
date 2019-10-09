#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "commonInclude.hpp"
#include "Matrix.hpp"

enum class CamMovement {
	Forward,
	Backward,
	Left,
	Right,
	Up,
	Down
};

class Camera {
	public:
		Camera(mat::Vec3 pos = mat::Vec3(0.0f, 0.0f, 0.0f), mat::Vec3 up = \
		mat::Vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);
		Camera(Camera const &src);
		virtual ~Camera();

		Camera &operator=(Camera const &rhs);

		mat::Mat4 getViewMatrix() const;
		void processKeyboard(CamMovement direction, float dtTime);
		void processMouseMovement(float xOffset, float yOffset, bool \
		constrainPitch = true);
		void processMouseScroll(float yOffset);

		mat::Vec3	pos;
		mat::Vec3	front;
		mat::Vec3	up;
		mat::Vec3	right;
		mat::Vec3	worldUp;

		float		yaw;
		float		pitch;

		float		movementSpeed;
		float		mouseSensitivity;
		float		zoom;
	private:
		void updateCameraVectors();
};

#endif
