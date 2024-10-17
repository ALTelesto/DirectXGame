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
#include "InputListener.h"
#include "InputSystem.h"
#include "SamplerState.h"

#include "Cube.h"
#include <vector>

class AppWindow: public Window, public InputListener
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
	Matrix4x4 getWorldCam();
	Matrix4x4 getProjection();
private:
	void update();
	void renderFullScreenQuad();

	//virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;

	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;

	virtual void onMouseMove(const Point& delta_mouse_pos) override;

	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;

	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;
private:
	long m_old_delta;
	long m_new_delta;
	float m_delta_time;

	float m_delta_pos;
	float m_delta_scale;
	float m_delta_rot;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	float m_scale_cube = 1;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	Matrix4x4 m_world_cam;
	Matrix4x4 m_proj;

private:
	vector<AGameObject*> gameObjectList;

private:
	int width;
	int height;

	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
	VertexShader* m_vs;
	PixelShader* m_ps;
	ConstantBuffer* m_cb;
	IndexBuffer* m_ib;

	//fullscreen quad
	VertexBuffer* fsquad_vb;
	IndexBuffer* fsquad_ib;

	//post-processing
	vector<ID3D11ShaderResourceView*> srvList;
	vector<PixelShader*> ppList;
	vector<ConstantBuffer*> cbList;

	SamplerState* m_ss;
};

