#pragma once

class VertexBuffer;
class VertexShader;
class PixelShader;

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
	Quad(float x, float y, float width, float height, vec3 color, void* shader_byte_code, size_t size_shader);
	void draw();
	void draw(VertexShader* vs, PixelShader* ps);
	bool init();
	void setPosition(float x, float y);
	void setSize(float width, float height);
	void setColor(vec3 color);
	~Quad();
private:
	VertexBuffer* m_vb;
	vec3 m_position;
	vec3 m_size;
	vec3 m_color;

	void* sbc;
	size_t ss;
};

