#include "ShaderLibrary.h"

ShaderLibrary* ShaderLibrary::sharedInstance = nullptr;

ShaderLibrary* ShaderLibrary::getInstance()
{
	return sharedInstance;
}

void ShaderLibrary::initialize()
{

}

void ShaderLibrary::destroy()
{
	sharedInstance->pixelShaderMap.clear();
	sharedInstance->vertexShaderMap.clear();

	delete sharedInstance;
}

ShaderLibrary::ShaderLibrary()
{
}

ShaderLibrary::~ShaderLibrary()
{
}
