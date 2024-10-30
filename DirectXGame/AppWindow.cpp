#include "AppWindow.h"

#include <iostream>
#include <Windows.h>

#include "EngineTime.h"

#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4x4.h"

#include <cstdlib>
#include <ctime> 

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
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
}

void AppWindow::destroy()
{
	if (sharedInstance != NULL)
	{
		delete sharedInstance;
	}
}

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
	SceneCameraHandler::destroy();
	UIManager::destroy();
}

void AppWindow::createGraphicsWindow()
{
	InputSystem::getInstance()->addListener(this);
	InputSystem::getInstance()->showCursor(false);

	EngineTime::initialize();

	UIManager::initialize(this->m_hwnd);

	SceneCameraHandler::initialize();
	this->scene_camera_handler = SceneCameraHandler::getInstance();

	RECT windowRect = this->getClientWindowRect();
	width = windowRect.right - windowRect.left;
	height = windowRect.bottom - windowRect.top;
	this->m_swap_chain = GraphicsEngine::getInstance()->getRenderSystem()->createSwapChain(this->m_hwnd, width, height);
	

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::getInstance()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::getInstance()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);

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

	UINT size_list = ARRAYSIZE(vertex_list);

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = sizeof(vertex) * size_list;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	m_vb = GraphicsEngine::getInstance()->getRenderSystem()->createVertexBuffer(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader, buff_desc);

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

	UINT size_index_list = ARRAYSIZE(index_list);
	m_ib = GraphicsEngine::getInstance()->getRenderSystem()->createIndexBuffer(index_list, size_index_list);

	GraphicsEngine::getInstance()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::getInstance()->getRenderSystem()->compileVertexShader(L"FSVertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	fsquad_vs = GraphicsEngine::getInstance()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);

	fsquad_vertex fsquad_list[] =
	{
		{Vector3D(-1.0f,-1.0f,0),Vector2D(0.0f,1.0f)},
		{Vector3D(-1.0f,1.0f,0),Vector2D(0.0f,0.0f)},
		{Vector3D(1.0f,1.0f,0),Vector2D(1.0f,0.0f)},
		{Vector3D(1.0f,-1.0f,0),Vector2D(1.0f,1.0f)}
	};

	
	UINT fsquad_size_list = ARRAYSIZE(fsquad_list);

	buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DYNAMIC;
	buff_desc.ByteWidth = sizeof(fsquad_vertex) * fsquad_size_list;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buff_desc.MiscFlags = 0;

	fsquad_vb = GraphicsEngine::getInstance()->getRenderSystem()->createVertexBuffer(fsquad_list, sizeof(fsquad_vertex), fsquad_size_list, shader_byte_code, size_shader, buff_desc);

	unsigned int fsquad_index_list[] =
	{
		0, 1, 2,
		2, 3, 0
	};
	UINT fsquad_size_index_list = ARRAYSIZE(fsquad_index_list);

	fsquad_ib = GraphicsEngine::getInstance()->getRenderSystem()->createIndexBuffer(fsquad_index_list, fsquad_size_index_list);

	GraphicsEngine::getInstance()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::getInstance()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::getInstance()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::getInstance()->getRenderSystem()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::getInstance()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

	m_ss = GraphicsEngine::getInstance()->getRenderSystem()->createSamplerState();

	//create sets of two SRV/RTV for ping pong rendering
	for (int i = 0; i < 2; i++) {
		ID3D11RenderTargetView* rtv;
		ID3D11ShaderResourceView* srv;
		GraphicsEngine::getInstance()->getRenderSystem()->createRenderTexture(&srv, &rtv);
		rtvList.push_back(rtv);
		srvList.push_back(srv);
	}

	//chromatic abberation
	{
		GraphicsEngine::getInstance()->getRenderSystem()->compilePixelShader(L"Chromaberration.hlsl", "psmain", &shader_byte_code, &size_shader);
		ppList.push_back(GraphicsEngine::getInstance()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader));
		GraphicsEngine::getInstance()->getRenderSystem()->releaseCompiledShader();
	}
	//vignette
	{
		GraphicsEngine::getInstance()->getRenderSystem()->compilePixelShader(L"Vignette.hlsl", "psmain", &shader_byte_code, &size_shader);
		ppList.push_back(GraphicsEngine::getInstance()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader));
		GraphicsEngine::getInstance()->getRenderSystem()->releaseCompiledShader();
	}

	//set first pass rtv and dsv
	{
		rtv_first = rtvList[0];
		GraphicsEngine::getInstance()->getRenderSystem()->createDepthStencilView(&this->dsv_first);
	}

	fsquad_cb = GraphicsEngine::getInstance()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
}

Matrix4x4 AppWindow::getWorldCam()
{
	return this->m_world_cam;
}

Matrix4x4 AppWindow::getProjection()
{
	return this->m_proj;
}

float AppWindow::getWidth()
{
	return this->width;
}

float AppWindow::getHeight()
{
	return this->height;
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

	m_cb->update(GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::renderFullScreenQuad()
{
	GraphicsEngine::getInstance()->getRenderSystem()->DisableDepthTest();
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(fsquad_vb);
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(fsquad_ib);
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(fsquad_ib->getSizeIndexList(), 0,0);
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::getInstance()->update();

	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.5, 0.5, 1);

	for(ID3D11RenderTargetView* rtv : this->rtvList)
	{
		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(rtv, this->dsv_first,
			0, 0.5, 0.5, 1);
	}

	GraphicsEngine::getInstance()->getRenderSystem()->EnableDepthTest();

	RECT windowRect = this->getClientWindowRect();
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);

	width = windowRect.right - windowRect.left;
	height = windowRect.bottom - windowRect.top;

	this->scene_camera_handler->update();
	this->update();

	//GraphicsEngine::getInstance()->setToRenderTexture();
	//GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setRenderTargets(m_swap_chain->getRenderTargetView(), m_swap_chain->getDepthStencilView());
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setRenderTargets(this->rtv_first, this->dsv_first);

	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);
	
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);

	for (AGameObject* gameObject : this->gameObjectList)
	{
		gameObject->update(EngineTime::getDeltaTime());
		gameObject->draw(width, height, this->m_vs, this->m_ps);
	}

	// Loop over the post-processing passes
	int read = 0;  // Reading from first render texture initially
	int write = 1; // Writing to second render texture
	
	//post-processing stage ----------------------------------------------------------------------

	ID3D11RenderTargetView* NULL_RT = nullptr;
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setRenderTargets(NULL_RT, nullptr);
	GraphicsEngine::getInstance()->getRenderSystem()->DisableDepthTest();
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setSamplerState(m_ss);

	//chromatic abberation
	{
		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setRenderTargets(rtvList[write], nullptr);

		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(fsquad_vs);
		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(ppList[0]);

		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setShaderResources(0, 1, &srvList[read]);

		renderFullScreenQuad();

		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setRenderTargets(NULL_RT, nullptr);
		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->unbindShaderResources();
	}

	std::swap(read, write);

	//vignette
	{
		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setRenderTargets(m_swap_chain->getRenderTargetView(), m_swap_chain->getDepthStencilView());
	
		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(fsquad_vs);
		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(ppList[1]);

		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setShaderResources(0, 1, &srvList[read]);

		constant_vignette cc;
		cc.vignetteRadius = 0.9f;
		cc.vignetteStrength = 0.5f;

		fsquad_cb->load(&cc, sizeof(constant));

		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(fsquad_vs, fsquad_cb);
		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(ppList[0],fsquad_cb);
		fsquad_cb->update(GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext(),&cc);

		renderFullScreenQuad();

		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setRenderTargets(NULL_RT, nullptr);
		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->unbindShaderResources();
	}

	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setRenderTargets(m_swap_chain->getRenderTargetView(), m_swap_chain->getDepthStencilView());

	UIManager::getInstance()->drawAllUI();

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

	SceneCameraHandler::destroy();

	this->destroy();
}

void AppWindow::onFocus()
{
	InputSystem::getInstance()->addListener(this);
	std::cout << "scene on focus call\n";
	if(scene_camera_handler != nullptr) scene_camera_handler->onFocus();
	InputSystem::getInstance()->showCursor(false);
}

void AppWindow::onKillFocus()
{
	InputSystem::getInstance()->removeListener(this);
	std::cout << "scene on kill focus call\n";
	if (scene_camera_handler != nullptr) scene_camera_handler->onKillFocus();
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

	this->scene_camera_handler->setCamRotation(m_rot_x, m_rot_y);

	std::cout << "rotation values: " << m_rot_x << " " << m_rot_y << "\n";

	//std::cout << height << " app\n";

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

