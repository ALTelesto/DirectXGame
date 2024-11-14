#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;
class PixelShader;
class ConstantBuffer;
class IndexBuffer;
class SamplerState;
class RenderSystem;
class GraphicsEngine;
class AGameObject;
class MeshedObject;
class Material;
class Resource;
class ResourceManager;
class Mesh;
class MeshManager;

typedef std::shared_ptr<SwapChain> SwapChainPtr;
typedef std::shared_ptr<DeviceContext> DeviceContextPtr;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef std::shared_ptr<VertexShader> VertexShaderPtr;
typedef std::shared_ptr<PixelShader> PixelShaderPtr;
typedef std::shared_ptr<ConstantBuffer> ConstantBufferPtr;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef std::shared_ptr<SamplerState> SamplerStatePtr;
typedef std::shared_ptr<AGameObject> GameObjectPtr;
typedef std::shared_ptr<Material> MaterialPtr;
typedef std::shared_ptr<Resource> ResourcePtr;
typedef std::shared_ptr<Mesh> MeshPtr;
typedef std::shared_ptr<MeshedObject> MeshedObjectPtr;