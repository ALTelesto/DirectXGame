#include "AppWindow.h"

#include <iostream>
#include <Windows.h>

#include "EngineTime.h"

#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4x4.h"

#include <cstdlib>
#include <ctime> 

#include "Plane.h"

struct vec2
{
	float x, y;
};

struct fsquad_vertex
{
	Vector3D position;
	Vector2D texcoord;
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
	unsigned int m_time;
};

struct constant_distortion
{
	float distortionStrength;
};

struct constant_vignette
{
	float vignetteRadius;
	float vignetteStrength;
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

	InputSystem::getInstance()->addListener(this);
	InputSystem::getInstance()->showCursor(false);

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
		cubeObject->setPosition(Vector3D(x, y, 2));
		cubeObject->setScale(Vector3D(0.15, 0.15, 0.15));
		this->gameObjectList.push_back(cubeObject);
	}

	Plane* planeObject = new Plane("Plane " + to_string(1), shader_byte_code, size_shader);
	planeObject->setPosition(Vector3D(0, 0, 2));
	planeObject->setRotation(-2, -2,-2);
	planeObject->setScale(Vector3D(0.5, 0.5, 0.5));
	this->gameObjectList.push_back(planeObject);

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


	GraphicsEngine::getInstance()->compileVertexShader(L"CAVertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	fsquad_vs = GraphicsEngine::getInstance()->createVertexShader(shader_byte_code, size_shader);


	fsquad_vertex fsquad_list[] =
	{
		{Vector3D(-1.0f,-1.0f,0.0f),Vector2D(0.0f,1.0f)},
		{Vector3D(-1.0f,1.0f,0.0f),Vector2D(0.0f,0.0f)},
		{Vector3D(1.0f,-1.0f,0.0f),Vector2D(1.0f,1.0f)},
		{Vector3D(1.0f,1.0f,0.0f),Vector2D(1.0f,0.0f)},
	};

	fsquad_vb = GraphicsEngine::getInstance()->createVertexBuffer();
	UINT fsquad_size_list = ARRAYSIZE(fsquad_list);

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DYNAMIC;
	buff_desc.ByteWidth = sizeof(fsquad_vertex) * fsquad_size_list;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buff_desc.MiscFlags = 0;

	fsquad_vb->load(fsquad_list, sizeof(fsquad_vertex), fsquad_size_list, shader_byte_code, size_shader, buff_desc);

	unsigned int fsquad_index_list[] =
	{
		0, 1, 2,
		2, 1, 3
	};
	UINT fsquad_size_index_list = ARRAYSIZE(index_list);

	fsquad_ib = GraphicsEngine::getInstance()->createIndexBuffer();
	fsquad_ib->load(fsquad_list, fsquad_size_index_list);

	GraphicsEngine::getInstance()->releaseCompiledShader();

	GraphicsEngine::getInstance()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::getInstance()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::getInstance()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::getInstance()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));

	m_ss = GraphicsEngine::getInstance()->createSamplerState();
	m_ss->load();

	ConstantBuffer* cb;

	//we have two post-processing shaders, so create two SRVs
	srvList.push_back(GraphicsEngine::getInstance()->createShaderResourceView());

	GraphicsEngine::getInstance()->compilePixelShader(L"Test.hlsl", "psmain", &shader_byte_code, &size_shader);
	ppList.push_back(GraphicsEngine::getInstance()->createPixelShader(shader_byte_code, size_shader));

	constant_vignette cc2;
	cc2.vignetteRadius = 0.5f;
	cc2.vignetteStrength = 1.0f;

	fsquad_cb = GraphicsEngine::getInstance()->createConstantBuffer();
	fsquad_cb->load(&cc2, sizeof(constant));
	

	//srvList.push_back(GraphicsEngine::getInstance()->createShaderResourceView());
}

Matrix4x4 AppWindow::getWorldCam()
{
	return this->m_world_cam;
}

Matrix4x4 AppWindow::getProjection()
{
	return this->m_proj;
}

void AppWindow::update()
{
	constant cc;
	cc.m_time = ::GetTickCount();

	Matrix4x4 temp;

	cc.m_world.setIdentity();

	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.1f);
	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.1f);
	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;

	world_cam.inverse();

	cc.m_view = world_cam;

	/*cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left)/300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top)/300.0f,
		-4.0f,
		4.0f
	);*/

	cc.m_proj.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

	this->m_proj = cc.m_proj;

	m_cb->update(GraphicsEngine::getInstance()->getImmediateDeviceContext(), &cc);
}

void AppWindow::renderFullScreenQuad()
{
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setDepthStencilState(nullptr);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexBuffer(fsquad_vb);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setIndexBuffer(fsquad_ib);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->drawIndexedTriangleList(fsquad_ib->getSizeIndexList(), 0,0);
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::getInstance()->update();
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 
		0, 0.5, 0.5, 1);
	RECT windowRect = this->getClientWindowRect();
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setViewportSize(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);

	width = windowRect.right - windowRect.left;
	height = windowRect.bottom - windowRect.top;

	this->update();

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setSamplerState(m_ss);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);

	for (AGameObject* gameObject : this->gameObjectList)
	{
		gameObject->update(EngineTime::getDeltaTime());
		gameObject->draw(width, height, this->m_vs, this->m_ps);
	}

	//post processing stage

	GraphicsEngine::getInstance()->setToRenderTexture(); //set render target to the render texture target for post processing

	constant_vignette cc;
	cc.vignetteRadius = 0.5f;
	cc.vignetteStrength = 0.5f;

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexShader(fsquad_vs);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setPixelShader(ppList[0]);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setConstantBuffer(fsquad_vs, fsquad_cb);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setConstantBuffer(ppList[0],fsquad_cb);
	fsquad_cb->update(GraphicsEngine::getInstance()->getImmediateDeviceContext(),&cc);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setShaderResources(0, 1, &srvList[0]);
	renderFullScreenQuad();

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setRenderTargets(m_swap_chain->getRenderTargetView(), nullptr);

	m_swap_chain->present(true);

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

void AppWindow::onFocus()
{
	InputSystem::getInstance()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::getInstance()->removeListener(this);
	InputSystem::getInstance()->showCursor(true);
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		m_rightward = 1.0f;
	}
}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	//std::cout <<"mouse_pos values: "<< mouse_pos.m_x << " " << mouse_pos.m_y << "\n";

	m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * EngineTime::getDeltaTime() * 0.1f;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * EngineTime::getDeltaTime() * 0.1f;

	//std::cout << "rotation values: " << m_rot_x << " " << m_rot_y << "\n";

	InputSystem::getInstance()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	std::cout << "lmb \n";
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	std::cout << "rmb \n";
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
}

