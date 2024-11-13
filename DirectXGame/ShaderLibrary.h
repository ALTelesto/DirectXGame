#pragma once
#include "Prerequisites.h"

struct ShaderStruct
{
	void* shader_byte_code;
	size_t size_shader;
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

class ShaderLibrary
{
public:
	ShaderLibrary* getInstance();
	static void initialize();
	static void destroy();
private:
	ShaderLibrary();
	~ShaderLibrary();
	ShaderLibrary(ShaderLibrary const&) {};
	ShaderLibrary& operator=(ShaderLibrary const&) {};
	static ShaderLibrary* sharedInstance;

	PixelShaderHashMap pixelShaderMap;
	VertexShaderHashMap vertexShaderMap;
};

