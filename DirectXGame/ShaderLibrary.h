#pragma once

#include "Prerequisites.h"
#include "ShaderLibraryDefines.h"

class ShaderLibrary
{
public:
	static ShaderLibrary* getInstance();
	static void initialize();
	static void destroy();

	PixelShaderPtr getPixelShader(const std::string& pixelShaderName);
	VertexShaderPtr getVertexShader(const std::string& vertexShaderName);
	void requestVertexShaderData(std::wstring vertexShaderName, void** shaderByteCode, size_t* sizeShader);

private:
	ShaderLibrary();
	~ShaderLibrary();
	ShaderLibrary(ShaderLibrary const&) {};
	ShaderLibrary& operator=(ShaderLibrary const&) {};
	static ShaderLibrary* sharedInstance;

	PixelShaderHashMap pixelShaderMap;
	VertexShaderHashMap vertexShaderMap;
};

