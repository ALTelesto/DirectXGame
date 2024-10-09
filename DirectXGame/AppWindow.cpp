#include "AppWindow.h"

#include <iostream>
#include <Windows.h>

#include "EngineTime.h"

#include "Vector3D.h"
#include "Matrix4x4.h"

struct vec3
{
	float x, y, z;
};

struct vertex
{
	Vector3D position;
	Vector3D position1;
	Vector3D color;
	Vector3D color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;
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
	int width = windowRect.right - windowRect.left;
	int height = windowRect.bottom - windowRect.top;

	this->m_swap_chain->init(this->m_hwnd, width, height);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::getInstance()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::getInstance()->createVertexShader(shader_byte_code, size_shader);

	//challenge 1
	/*vertex list[] =
	{
		{Vector3D(-0.8f,-0.9f,0.0f),Vector3D(-0.32f,-0.11f,0.0f),Vector3D(0,0,0),Vector3D(0,1,0) },
		{Vector3D(-0.9f,0.2f,0.0f),Vector3D(-0.11f,0.78f,0.0f),Vector3D(1,1,0),Vector3D(0,1,1) },
		{Vector3D(0.2f,-0.3f,0.0f),Vector3D(0.75f,-0.73f,0.0f),Vector3D(0,0,1),Vector3D(1,0,0) },
		{Vector3D(0.1f,0.25f,0.0f),Vector3D(0.88f,0.77f,0.0f),Vector3D(1,1,1),Vector3D(0,0,1) }
	};*/

	//challenge 2
	vertex list[] =
	{
		{Vector3D( -0.8f,-0.9f,0.0f),Vector3D(-0.3f,-0.2f,0.0f),Vector3D(0,0,0),Vector3D(0,1,0)},
		{Vector3D(-0.9f,0.2f,0.0f),Vector3D(-0.2f,0.83f,0.0f),Vector3D(1,1,0),Vector3D(0,1,1) },
		{Vector3D(0.95f,-0.3f,0.0f),Vector3D(0.05f,-0.73f,0.0f),Vector3D(0,0,1),Vector3D(1,0,0) },
		{Vector3D(-0.8f,-0.9f,0.0f),Vector3D(0.85f,0.83f,0.0f),Vector3D(1,1,1),Vector3D(0,0,1) }
	};

	m_vb = GraphicsEngine::getInstance()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::getInstance()->releaseCompiledShader();

	GraphicsEngine::getInstance()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::getInstance()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::getInstance()->releaseCompiledShader();

	constant cc;
	cc.m_angle = 0;

	m_cb = GraphicsEngine::getInstance()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void AppWindow::updateQuadPosition()
{
	/*unsigned long new_time = 0;
	if (m_old_time)
		new_time = ::GetTickCount() - m_old_time;
	m_delta_time = new_time / 1000.0f;
	m_old_time = ::GetTickCount();*/

	total_time += EngineTime::getDeltaTime();

	//challenge 1 - increasing/decreasing animation
	//float speed = (2.0f * std::sin(static_cast<float>(total_time * 0.3f)));

	//challenge 2 - linear
	float speed = 0.25f;

	m_angle += 5.f * static_cast<float>(EngineTime::getDeltaTime()) * std::abs(speed);
	constant cc;
	cc.m_angle = m_angle;

	if(m_angle > 1.0f)
	{
		m_angle = 0;
	}

	cc.m_world.setTranslation(Vector3D::lerp(Vector3D(-2,-2,0),Vector3D(2,2,0),m_angle));

	//cc.m_world.setTranslation(Vector3D(0,0,0));
	cc.m_view.setIdentity();
	cc.m_projection.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left)/200.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top)/200.0f,
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
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	this->updateQuadPosition();

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertex(), 0);
	m_swap_chain->present(true);

	std::cout << "delta time: " << EngineTime::getDeltaTime() << "\n";
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::destroy();
}

void AppWindow::onCreate()
{

}