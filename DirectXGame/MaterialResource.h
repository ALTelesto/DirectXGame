#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include "Prerequisites.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"

class MaterialResource
{
	typedef Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TexturePtr;

public:
	MaterialResource(PixelShaderPtr pixelShader, VertexShaderPtr vertexShader);

	void setPixelShader(PixelShaderPtr pixelShader);
	void setPixelShader(const std::string& pixelShaderName);

	void setVertexShader(VertexShaderPtr vertexShader);
	void setVertexShader(const std::string& vertexShaderName);

	PixelShaderPtr pixelShader = nullptr;
	VertexShaderPtr vertexShader = nullptr;

	Vector4D color = { 1, 1, 1, 1 };

	TexturePtr albedoTexture = nullptr;
	TexturePtr metallicTexture = nullptr;
	TexturePtr smoothnessTexture = nullptr;
	TexturePtr normalTexture = nullptr;

	SamplerStatePtr samplerState = nullptr;

	float metallic = 0;
	float smoothness = 0;
	float flatness = 0;

	Vector2D tiling = 0;
	Vector2D offset = 0;
};

