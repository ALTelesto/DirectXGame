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

class Quad
{
public:
	Quad(vec3 pos, vec3 size, vec3 col);
	void draw();
	bool init(void* shader_byte_code, size_t size_shader);
	~Quad();
private:
	vertex list;
	VertexBuffer* m_vb;
};

