#include "Camera.h"

#include "AppWindow.h"
#include "EngineTime.h"
#include "InputSystem.h"
#include "Settings.h"

Camera::Camera(string name): AGameObject(name)
{
	InputSystem::getInstance()->addListener(this);
}

Camera::~Camera()
{
}

void Camera::update(float deltaTime)
{
	Matrix4x4 worldCam; worldCam.setIdentity();
	Matrix4x4 temp;

	Vector3D localRot = this->getLocalRotation();

	temp.setIdentity();
	temp.setRotationX(localRot.m_x);
	worldCam *= temp;

	temp.setIdentity();
	temp.setRotationY(localRot.m_y);
	worldCam *= temp;

	/*temp.setIdentity();
	temp.setTranslation(this->getLocalPosition());
	worldCam.setTranslation(this->getLocalPosition());*/

	Vector3D new_pos = getLocalPosition() + worldCam.getZDirection() * (forward * 0.1f);
	new_pos = new_pos + worldCam.getXDirection() * (rightward * 0.1f);
	this->localPosition = new_pos;
	worldCam.setTranslation(new_pos);

	worldCam.inverse();
	this->localMatrix = worldCam;
}

Matrix4x4 Camera::getViewMatrix()
{
	return this->localMatrix;
}

void Camera::draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader)
{
}

void Camera::onKeyDown(int key)
{
	if (key == 'W')
	{
		forward = 1.0f;
	}
	else if (key == 'S')
	{
		forward = -1.0f;
	}
	else if (key == 'A')
	{
		rightward = -1.0f;
	}
	else if (key == 'D')
	{
		rightward = 1.0f;
	}
}

void Camera::onKeyUp(int key)
{
	forward = 0;
	rightward = 0;
}

void Camera::onMouseMove(const Point& delta_mouse_pos)
{
	rot_x += (delta_mouse_pos.m_y - (AppWindow::getInstance()->getHeight() / 2.0f)) * EngineTime::getDeltaTime() * 0.1f;
	rot_y += (delta_mouse_pos.m_x - (AppWindow::getInstance()->getWidth() / 2.0f)) * EngineTime::getDeltaTime() * 0.1f;

	std::cout << rot_x << " " << rot_y << "\n";

	this->localRotation.m_x = this->rot_x;
	this->localRotation.m_y = this->rot_y;

	//this->updateViewMatrix();
}

void Camera::onLeftMouseDown(const Point& mouse_pos)
{
}

void Camera::onLeftMouseUp(const Point& mouse_pos)
{
}

void Camera::onRightMouseDown(const Point& mouse_pos)
{
}

void Camera::onRightMouseUp(const Point& mouse_pos)
{
}

void Camera::onFocus()
{
	InputSystem::getInstance()->addListener(this);
}

void Camera::onKillFocus()
{
	InputSystem::getInstance()->removeListener(this);
}
