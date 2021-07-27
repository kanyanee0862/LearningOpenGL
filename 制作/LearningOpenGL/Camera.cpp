#include "Camera.h"
#include <glfw3.h>
//ベクトルがあるコンストラクター
Camera::Camera(glm::vec3 position ,glm::vec3 up , float yaw, float pitch)
	:Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVITY),
	Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
}
//スカラー値のコンストラクタ
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	:Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVITY),
	Zoom(ZOOM)
{
	Position = glm::vec3(posX,posY,posZ);
	WorldUp = glm::vec3(upX,upY,upZ);
	Yaw = yaw;
	Pitch = pitch;
}
//ユーラアングルとlookAtマトリックスで計算したビューマトリックスを返す
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}
//キーボードのインプット
void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
	{
		Position += Front * velocity;
	}
	if (direction == BACKWARD)
	{
		Position -= Front * velocity;
	}
	if (direction == LEFT)
	{
		Position -= Right * velocity;
	}
	if (direction == RIGHT)
	{
		Position += Right * velocity;
	}
}
//マウスインプット
void Camera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw += xOffset;
	Pitch += yOffset;

	//90度にならないため（翻る）
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
		{
			Pitch = 89.0f;
		}
		if (Pitch < -89.0f)
		{
			Pitch = -89.0f;
		}
	}
	//前、右、上のベクトルをユーラアングルで更新する
	updateCameraVectors();
}
//マウススクロールのインプット
void Camera::ProcessMouseScroll(float yOffset)
{
	//FOVの範囲であれば
	if (Zoom >= 1.0f && Zoom <= 45.0f)
	{
		Zoom -= yOffset;
	}
	if (Zoom <= 1.0f)
	{
		Zoom = 1.0f;
	}
	if (Zoom >= 45.0f)
	{
		Zoom = 45.0f;
	}
}
//前ベクトルをユーラアングルで更新
void Camera::updateCameraVectors()
{
	//前ベクトルを計算する
	glm::vec3 front;
	front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	front.y = sin(glm::radians(Pitch));
	front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Front = glm::normalize(front);
	//右と上ベクトルを再計算する
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}
