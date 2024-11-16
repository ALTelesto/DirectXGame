#include "MaterialResource.h"

#include "GraphicsEngine.h"
#include "LogUtils.h"

MaterialResource::MaterialResource(PixelShaderPtr pixelShader, VertexShaderPtr vertexShader) :
pixelShader(pixelShader), vertexShader(vertexShader)
{
}

void MaterialResource::setPixelShader(PixelShaderPtr pixelShader)
{
	this->pixelShader = pixelShader;
}

void MaterialResource::setVertexShader(VertexShaderPtr vertexShader)
{
	this->vertexShader = vertexShader;
}

void MaterialResource::setPixelShader(const std::string& pixelShaderName)
{
	void* shaderByteCode = nullptr;
	size_t byteCodeSize = 0;

	std::string append = pixelShaderName + ".hlsl";
	const std::wstring widestr = std::wstring(append.begin(), append.end());

	LogUtils::log(this, "Pixel shader compilation");
	GraphicsEngine::getInstance()->getRenderSystem()->compilePixelShader(widestr.c_str(), "main", &shaderByteCode, &byteCodeSize);
	pixelShader = GraphicsEngine::getInstance()->getRenderSystem()->createPixelShader(shaderByteCode, byteCodeSize);
	GraphicsEngine::getInstance()->getRenderSystem()->releaseCompiledShader();
}

void MaterialResource::setVertexShader(const std::string& vertexShaderName)
{
	void* shaderByteCode = nullptr;
	size_t byteCodeSize = 0;

	std::string append = vertexShaderName + ".hlsl";
	const std::wstring widestr = std::wstring(append.begin(), append.end());

	LogUtils::log(this, "Pixel shader compilation");
	GraphicsEngine::getInstance()->getRenderSystem()->compilePixelShader(widestr.c_str(), "main", &shaderByteCode, &byteCodeSize);
	vertexShader = GraphicsEngine::getInstance()->getRenderSystem()->createVertexShader(shaderByteCode, byteCodeSize);
	GraphicsEngine::getInstance()->getRenderSystem()->releaseCompiledShader();
}
