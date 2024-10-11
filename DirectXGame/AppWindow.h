#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Circle.h"
#include <vector>

class AppWindow: public Window
{
public:
	static AppWindow* getInstance();
	static void initialize();
	static void destroy();
private:
	AppWindow();
	~AppWindow();
	AppWindow(AppWindow const&) {};
	AppWindow& operator = (AppWindow const&) {};
	static AppWindow* sharedInstance;
public:
	void createGraphicsWindow();

	void updateQuadPosition();

	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
private:
	float m_delta_pos = 0;
	float m_angle = 0;
	double total_time = 0;
private:
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
	VertexShader* m_vs;
	PixelShader* m_ps;
	ConstantBuffer* m_cb;
	IndexBuffer* m_ib;

private:
	// Add a list of Circle objects
	std::vector<Circle*> m_circles;
	void createCircles();
};

