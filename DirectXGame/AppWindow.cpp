#include "AppWindow.h"

#include <iostream>
#include <Windows.h>

#include "EngineTime.h"

#include "Vector3D.h"
#include "Matrix4x4.h"

#include <cstdlib>

#ifndef PI
#define PI 3.14159265359f
#endif

struct vec3
{
	float x, y, z;
};

struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_angle;
};

AppWindow* AppWindow::sharedInstance = nullptr;

AppWindow* AppWindow::getInstance()
{
	return sharedInstance;
}

void AppWindow::initialize()
{
	sharedInstance = new AppWindow();
	//sharedInstance->onCreate();
}

void AppWindow::destroy()
{
	if (sharedInstance != NULL)
	{
		sharedInstance->release();
		delete sharedInstance;
	}
}

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
	// Clean up the list of circles
	for (Circle* circle : m_circles)
	{
		delete circle;
	}
	m_circles.clear();
}

void AppWindow::createGraphicsWindow()
{
	GraphicsEngine::initialize();
	GraphicsEngine* graphEngine = GraphicsEngine::getInstance();

	this->m_swap_chain = GraphicsEngine::getInstance()->createSwapChain();
	RECT windowRect = this->getClientWindowRect();
	int pixelWidth = windowRect.right - windowRect.left;
	int pixelHeight = windowRect.bottom - windowRect.top;

	width = (pixelWidth / 300.0f);
	height = (pixelHeight / 300.0f);

	this->m_swap_chain->init(this->m_hwnd, pixelWidth, pixelHeight);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::getInstance()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::getInstance()->createVertexShader(shader_byte_code, size_shader);

	vertex vertex_list[] =
	{
		//front
		{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1,0,0),  Vector3D(0.2f,0,0) },
		{Vector3D(-0.5f,0.5f,-0.5f),    Vector3D(1,1,0), Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,0.5f,-0.5f),   Vector3D(1,1,0),  Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,0,0), Vector3D(0.2f,0,0) },
		//back
		{ Vector3D(0.5f,-0.5f,0.5f),    Vector3D(0,1,0), Vector3D(0,0.2f,0) },
		{ Vector3D(0.5f,0.5f,0.5f),    Vector3D(0,1,1), Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,0.5f,0.5f),   Vector3D(0,1,1),  Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,-0.5f,0.5f),     Vector3D(0,1,0), Vector3D(0,0.2f,0) }

	};

	m_vb = GraphicsEngine::getInstance()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
	{
		//front
		0,1,2,
		2,3,0,
		//back
		4,5,6,
		6,7,4,
		//top
		1,6,5,
		5,2,1,
		//bottom
		7,0,3,
		3,4,7,
		//right
		3,2,5,
		5,4,3,
		//left
		7,6,1,
		1,0,7
	};

	m_ib = GraphicsEngine::getInstance()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	m_ib->load(index_list, size_index_list);

	m_vb->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::getInstance()->releaseCompiledShader();

	GraphicsEngine::getInstance()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::getInstance()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::getInstance()->releaseCompiledShader();

	constant cc;
	cc.m_angle = 0;

	m_cb = GraphicsEngine::getInstance()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));

	// Initialize the circle list
	//createCircles();
}

void AppWindow::updateQuadPosition()
{
	total_time += EngineTime::getDeltaTime();

	float speed = 0.25f;

	m_angle += 5.f * static_cast<float>(EngineTime::getDeltaTime()) * std::abs(speed);
	constant cc;
	cc.m_angle = m_angle;

	/*if(m_angle > 1.0f)
	{
		m_angle = 0;
	}*/

	Matrix4x4 temp;

	cc.m_world.setScale(Vector3D(0.7, 0.7, 0.7));

	temp.setIdentity();
	//temp.setRotationZ(m_angle);
	cc.m_world *= temp;

	temp.setIdentity();
	//temp.setRotationY(m_angle);
	cc.m_world *= temp;

	temp.setIdentity();
	//temp.setRotationX(m_angle);
	temp.setTranslation(Vector3D(0, 0, 1));
	cc.m_world *= temp;

	cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
		-4.0f,
		4.0f
	);

	m_cb->update(GraphicsEngine::getInstance()->getImmediateDeviceContext(), &cc);
}

bool isKeyPressed(int key) {
	return GetAsyncKeyState(key) & 0x8000;
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0, 0, 1);
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	this->updateQuadPosition();

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setIndexBuffer(m_ib);
	

	for (Circle* circle : m_circles)
	{
		float circleX = circle->getPosition().m_x;
		float circleY = circle->getPosition().m_y;
		float radius = circle->getRadius();

		bool bounce = false;
		// incredibly scuffed collision check made by you-know-who + fined-tuned by me
		if (circleX - radius * 3.35 <= -width / 2 || circleX + radius * 3.35 >= width / 2)
		{
			float direction = circle->getDirection();
			circle->setDirection(PI - direction);
			bounce = true;
		}
		if (circleY - radius * 2.7 <= -height / 2 || circleY + radius * 2.7 >= height / 2)
		{
			float direction = circle->getDirection();
			circle->setDirection(-direction);
			bounce = true;
		}
		if(bounce)
		{
			Vector3D color = Vector3D(static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX);
			circle->setColor(color);
		}

		circle->move(EngineTime::getDeltaTime());
		circle->draw(m_vs, m_ps);
	}

	handleInput();

	m_swap_chain->present(true);

	//std::cout << "delta time: " << EngineTime::getDeltaTime() << "\n";
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	m_ib->release();
	m_cb->release();
	GraphicsEngine::destroy();
}

void AppWindow::onCreate()
{

}

void AppWindow::createCircles()
{
	// Create a few circles with random positions and colors
	int num_circles = 1;  // You can change the number of circles here
	for (int i = 0; i < num_circles; ++i)
	{
		float x = 0.f;
		float y = 0.f;
		float radius = 0.2f;  // Random radius
		Vector3D color = Vector3D( static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX );

		Circle* circle = new Circle(x, y, radius, color);

		float randomSpeed = 0.1f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (0.7f - 0.1f)));
		circle->setSpeed(randomSpeed);

		m_circles.push_back(circle);
	}
}

void AppWindow::handleInput()
{
	static bool spacePressed = false;
	static bool backspacePressed = false;
	static bool deletePressed = false;

	// Space -> Spawn 1 circle
	if (isKeyPressed(VK_SPACE) && !spacePressed) {
		createCircles();
		spacePressed = true;
	}
	if (!isKeyPressed(VK_SPACE)) spacePressed = false;

	// Backspace -> Delete the most recent circle
	if (isKeyPressed(VK_BACK) && !backspacePressed) {
		if (!m_circles.empty()) {
			delete m_circles.back();
			m_circles.pop_back();
		}
		backspacePressed = true;
	}
	if (!isKeyPressed(VK_BACK)) backspacePressed = false;

	// Delete -> Delete all circles
	if (isKeyPressed(VK_DELETE) && !deletePressed) {
		for (Circle* circle : m_circles) {
			delete circle;
		}
		m_circles.clear();
		deletePressed = true;
	}
	if (!isKeyPressed(VK_DELETE)) deletePressed = false;
}