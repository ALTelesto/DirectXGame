#pragma once

#include "Prerequisites.h"

struct ShaderStruct
{
	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;
};

struct PSStruct
{
	ShaderStruct shaderData;
	PixelShaderPtr pixelShader;
};

struct VSStruct
{
	ShaderStruct shaderData;
	VertexShaderPtr vertexShader;
};

typedef std::unordered_map<std::string, PSStruct> PixelShaderHashMap;
typedef std::unordered_map<std::string, VSStruct> VertexShaderHashMap;