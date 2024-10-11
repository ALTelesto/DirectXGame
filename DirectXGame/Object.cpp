#include "Object.h"
#include "GraphicsEngine.h"
#include "VertexBuffer.h"

Object::Object()
{
	
}

void Object::draw()
{
}

bool Object::init(void* shader_byte_code, size_t size_shader)
{
	m_vb = GraphicsEngine::get()->createVertexBuffer();
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
}
