#include "ShaderNames.h"

const unordered_map<string, wstring>ShaderNames::PixelShaderNames =
{
	{"PixelShader",L"PixelShader.hlsl"},
	{"ShadedTextured",L"ShadedTextured.hlsl"},
	{"Test",L"Test.hlsl"}
};

const unordered_map<string, wstring>ShaderNames::VertexShaderNames =
{
	{"FSVertexShader",L"FSVertexShader.hlsl"},
	{"ShadedVertex",L"ShadedVertex.hlsl"},
	{"VertexShader",L"VertexShader.hlsl"}
};