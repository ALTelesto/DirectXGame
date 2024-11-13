#include "ShaderLibrary.h"

#include "GraphicsEngine.h"
#include "LogUtils.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "ShaderNames.h"

ShaderLibrary* ShaderLibrary::sharedInstance = nullptr;

ShaderLibrary* ShaderLibrary::getInstance()
{
	return sharedInstance;
}

void ShaderLibrary::initialize()
{
	if (ShaderLibrary::sharedInstance) throw std::exception("ShaderLibrary already initialized");
	sharedInstance = new ShaderLibrary();
}

void ShaderLibrary::destroy()
{
	sharedInstance->pixelShaderMap.clear();
	sharedInstance->vertexShaderMap.clear();

	delete sharedInstance;
}

PixelShaderPtr ShaderLibrary::getPixelShader(const std::string& pixelShaderName)
{
	return pixelShaderMap[pixelShaderName].pixelShader;
}

VertexShaderPtr ShaderLibrary::getVertexShader(const std::string& vertexShaderName)
{
	return vertexShaderMap[vertexShaderName].vertexShader;
}

void ShaderLibrary::requestVertexShaderData(std::wstring vertexShaderName, void** shaderByteCode,
	size_t* sizeShader)
{
	GraphicsEngine::getInstance()->getRenderSystem()->compileVertexShader(vertexShaderName.data(), "vsmain", shaderByteCode, sizeShader);
	GraphicsEngine::getInstance()->getRenderSystem()->createVertexShader(shaderByteCode, *sizeShader);
}

ShaderLibrary::ShaderLibrary()
{
	LogUtils::log(this, "Shader compile start");
	for (const auto& pixelShaderName : ShaderNames::PixelShaderNames)
	{
		LogUtils::log(this, pixelShaderName.first);

		PSStruct psStruct;
		ShaderStruct shaderData;

		const wchar_t* fileName = pixelShaderName.second.data();

		GraphicsEngine::getInstance()->getRenderSystem()->compilePixelShader(fileName, "psmain", &shaderData.shaderByteCode, &shaderData.sizeShader);
		psStruct.pixelShader = GraphicsEngine::getInstance()->getRenderSystem()->createPixelShader(shaderData.shaderByteCode, shaderData.sizeShader);
		psStruct.shaderData = shaderData;

		if (!psStruct.pixelShader) LogUtils::error(this, "Error creating pixel shader");

		auto result = pixelShaderMap.insert(std::make_pair(pixelShaderName.first,psStruct));

		GraphicsEngine::getInstance()->getRenderSystem()->releaseCompiledShader();
	}
	for (const auto& vertexShaderName : ShaderNames::VertexShaderNames)
	{
		LogUtils::log(this, vertexShaderName.first);

		VSStruct vsStruct;
		ShaderStruct shaderData;

		const wchar_t* fileName = vertexShaderName.second.data();

		GraphicsEngine::getInstance()->getRenderSystem()->compileVertexShader(fileName, "vsmain", &shaderData.shaderByteCode, &shaderData.sizeShader);
		vsStruct.vertexShader = GraphicsEngine::getInstance()->getRenderSystem()->createVertexShader(shaderData.shaderByteCode, shaderData.sizeShader);
		vsStruct.shaderData = shaderData;

		if (!vsStruct.vertexShader) LogUtils::error(this, "Error creating vertex shader");

		auto result =  vertexShaderMap.insert(std::make_pair(vertexShaderName.first, vsStruct));

		GraphicsEngine::getInstance()->getRenderSystem()->releaseCompiledShader();
	}
}

ShaderLibrary::~ShaderLibrary()
{
}
