#include "AppWindow.h"

#include <iostream>

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	/*void* shader_byte_code = nullptr;
	size_t size_shader = 0;*/

	
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	
	quads.push_back(new Quad(-0.6f, -0.6f, 0.5f, 0.4f, { 1.0f, 0.0f, 0.0f }, shader_byte_code, size_shader)); // Red quad
	quads.push_back(new Quad(0.0f, 0.0f, 0.5f, 0.4f, { 0.0f, 1.0f, 0.0f }, shader_byte_code, size_shader));  // Green quad
	quads.push_back(new Quad(0.6f, 0.6f, 0.5f, 0.4f, { 0.0f, 0.0f, 1.0f }, shader_byte_code, size_shader));

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	for (Quad* quad : quads)
	{
		quad->init();
	}
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0, 0, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	/*Quad* test = new Quad(-0.5f, -0.5f, 0.2f, 0.3f, { 1.0f, 0.0f, 0.0f }, shader_byte_code, size_shader);
	test->draw(m_vs, m_ps);*/

	for (Quad* quad : quads)
	{
		quad->draw(m_vs,m_ps);
	}

	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();

	for (Quad* quad : quads)
	{
		delete quad;
	}
	quads.clear();

	//m_vb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}
