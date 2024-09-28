#include "Quad.h"

#include <iostream>

#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "DeviceContext.h"

Quad::Quad(float x, float y, float width, float height, vec3 color)
{
    m_position = { x, y, 0 };
    m_size = { width, height, 0 };
    m_color = color;

    // Calculate half width and height to offset vertices from the center
    float half_width = width / 2.0f;
    float half_height = height / 2.0f;

    // Vertices relative to the center (x, y)
    vertex list[] =
    {
        {x - half_width, y - half_height, 0.0f, color.x, color.y, color.z},  // Bottom-left
        {x - half_width, y + half_height, 0.0f, color.x, color.y, color.z},  // Top-left
        {x + half_width, y - half_height, 0.0f, color.x, color.y, color.z},  // Bottom-right
        {x + half_width, y + half_height, 0.0f, color.x, color.y, color.z},  // Top-right
    };

    m_vb = GraphicsEngine::get()->createVertexBuffer();
    m_vb->load(list, sizeof(vertex), ARRAYSIZE(list), nullptr, 0);
}

void Quad::draw()
{
    GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
    GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertex(), 0);
    //std::cout << "draw\n";
}

void Quad::draw(VertexShader* vs, PixelShader* ps)
{
    GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(vs);
    GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(ps);

    GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
    GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertex(), 0);
}

void Quad::setPosition(float x, float y)
{
}

void Quad::setSize(float width, float height)
{
}

void Quad::setColor(vec3 color)
{
}

Quad::~Quad()
{
}
