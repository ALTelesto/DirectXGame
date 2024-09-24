#pragma once

class VertexBuffer;

struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
	vec3 color;
};

class Object
{
public:
	Object();
	void draw();
	bool init(void* shader_byte_code, size_t size_shader);
	~Object();
private:
	vertex list;
	VertexBuffer* m_vb;
};

