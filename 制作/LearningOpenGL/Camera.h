#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
//カメラの動作
enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//カメラの基本変数
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

class Camera 
{
public:
	float Zoom;
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yOffset);
	//Get/Set
	void SetPosition(glm::vec3 Pos) { Position = Pos; }
	glm::vec3 GetPosition() const { return Position; }
	void SetFront(glm::vec3 front) { Front = front; }
	glm::vec3 GetFront() const { return Front; }
private:
	//カメラの属性
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	//ユーラアングル
	float Yaw;
	float Pitch;
	//オプション
	float MovementSpeed;
	float MouseSensitivity;
	
	void updateCameraVectors();
};

