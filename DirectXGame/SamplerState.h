#pragma once
#include <d3d11.h>
#include <Windows.h>

class DeviceContext;

class SamplerState
{
	public:
	SamplerState();
	bool load();
	//void update(DeviceContext* context, void* buffer);
	bool release();
	~SamplerState();
	private:
	UINT m_size_vertex;
	UINT m_size_list;
	private:
	ID3D11SamplerState* m_sampler_state;
	private:
	friend class DeviceContext;
};
