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

#include "SceneCameraHandler.h"

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
	float getWidth();
	float getHeight();
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

	SwapChainPtr m_swap_chain;
	VertexBufferPtr m_vb;
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	ConstantBufferPtr m_cb;
	IndexBufferPtr m_ib;

	//fullscreen quad
	VertexShaderPtr fsquad_vs;
	VertexBufferPtr fsquad_vb;
	IndexBufferPtr fsquad_ib;
	ConstantBufferPtr fsquad_cb;

	ID3D11RenderTargetView* rtv_first;
	ID3D11DepthStencilView* dsv_first;

	//post-processing
	vector<ID3D11RenderTargetView*> rtvList;
	vector<ID3D11ShaderResourceView*> srvList;
	int currentRenderTarget = 0;

	vector<PixelShaderPtr> ppList;

	SamplerStatePtr m_ss;

	//scene camera
	SceneCameraHandler* scene_camera_handler = nullptr;
};

