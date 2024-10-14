#include "AppWindow.h"

#include <iostream>
#include <Windows.h>

#include "EngineTime.h"

#include "Vector3D.h"
#include "Matrix4x4.h"

#include <cstdlib>
#include <ctime> 

#include "Plane.h"

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
}

void AppWindow::createGraphicsWindow()
{
	GraphicsEngine::initialize();
	GraphicsEngine* graphEngine = GraphicsEngine::getInstance();

	this->m_swap_chain = GraphicsEngine::getInstance()->createSwapChain();
	RECT windowRect = this->getClientWindowRect();
	width = windowRect.right - windowRect.left;
	height = windowRect.bottom - windowRect.top;

	this->m_swap_chain->init(this->m_hwnd, width, height);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::getInstance()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::getInstance()->createVertexShader(shader_byte_code, size_shader);

	for (int i = 0; i < 1; ++i)
	{
		/*float x = -0.75f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.5f)));
		float y = -0.75f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.5f)));*/

		float x = 0;
		float y = 0;

		Cube* cubeObject = new Cube("Cube "+to_string(i + 1), shader_byte_code, size_shader);
		cubeObject->setAnimSpeed(-3.75f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (7.5f))));
		cubeObject->setPosition(Vector3D(x, y, 2.0f));
		cubeObject->setScale(Vector3D(0.15, 0.15, 0.15));
		this->gameObjectList.push_back(cubeObject);
	}

	Plane* planeObject = new Plane("Plane " + to_string(1), shader_byte_code, size_shader);
	planeObject->setPosition(Vector3D(0, 0, 2.0f));
	planeObject->setRotation(-2, -2,-2);
	planeObject->setScale(Vector3D(0.5, 0.5, 0.5));
	this->gameObjectList.push_back(planeObject);

	//vertex vertex_list[] =
	//{
	//	//front
	//	{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1,0,0),  Vector3D(0.2f,0,0) },
	//	{Vector3D(-0.5f,0.5f,-0.5f),    Vector3D(1,1,0), Vector3D(0.2f,0.2f,0) },
	//	{ Vector3D(0.5f,0.5f,-0.5f),   Vector3D(1,1,0),  Vector3D(0.2f,0.2f,0) },
	//	{ Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,0,0), Vector3D(0.2f,0,0) },
	//	//back
	//	{ Vector3D(0.5f,-0.5f,0.5f),    Vector3D(0,1,0), Vector3D(0,0.2f,0) },
	//	{ Vector3D(0.5f,0.5f,0.5f),    Vector3D(0,1,1), Vector3D(0,0.2f,0.2f) },
	//	{ Vector3D(-0.5f,0.5f,0.5f),   Vector3D(0,1,1),  Vector3D(0,0.2f,0.2f) },
	//	{ Vector3D(-0.5f,-0.5f,0.5f),     Vector3D(0,1,0), Vector3D(0,0.2f,0) }

	//};

	//m_vb = GraphicsEngine::getInstance()->createVertexBuffer();
	//UINT size_list = ARRAYSIZE(vertex_list);

	//unsigned int index_list[] =
	//{
	//	//front
	//	0,1,2,
	//	2,3,0,
	//	//back
	//	4,5,6,
	//	6,7,4,
	//	//top
	//	1,6,5,
	//	5,2,1,
	//	//bottom
	//	7,0,3,
	//	3,4,7,
	//	//right
	//	3,2,5,
	//	5,4,3,
	//	//left
	//	7,6,1,
	//	1,0,7
	//};

	//m_ib = GraphicsEngine::getInstance()->createIndexBuffer();
	//UINT size_index_list = ARRAYSIZE(index_list);

	//m_ib->load(index_list, size_index_list);

	//m_vb->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::getInstance()->releaseCompiledShader();

	GraphicsEngine::getInstance()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::getInstance()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::getInstance()->releaseCompiledShader();

	/*constant cc;
	cc.m_angle = 0;

	m_cb = GraphicsEngine::getInstance()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));*/
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
	temp.setRotationZ(m_angle);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_angle);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_angle);
	temp.setTranslation(Vector3D(0, 0, 2));
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

void AppWindow::onUpdate()
{
	Window::onUpdate();

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.5, 0.5, 1);
	RECT windowRect = this->getClientWindowRect();
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setViewportSize(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);

	width = windowRect.right - windowRect.left;
	height = windowRect.bottom - windowRect.top;

	//this->updateQuadPosition();

	/*GraphicsEngine::getInstance()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setIndexBuffer(m_ib);*/

	//GraphicsEngine::getInstance()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);

	for (AGameObject* gameObject : this->gameObjectList)
	{
		gameObject->update(EngineTime::getDeltaTime());
		gameObject->draw(width, height, this->m_vs, this->m_ps);
	}

	m_swap_chain->present(true);

	//std::cout << "delta time: " << EngineTime::getDeltaTime() << "\n";
}

void AppWindow::onDestroy()
{
	Window::onDestroy();

	for (AGameObject* gameObject : gameObjectList)
	{
		delete gameObject;
	}
	gameObjectList.clear();

	if(m_vb)m_vb->release();
	if(m_swap_chain)m_swap_chain->release();
	if(m_vs)m_vs->release();
	if(m_ps)m_ps->release();
	if(m_ib)m_ib->release();
	if(m_cb)m_cb->release();
	GraphicsEngine::destroy();
}

void AppWindow::onCreate()
{

}