#include "Quad.h"

#include <iostream>

#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "DeviceContext.h"

Quad::Quad(float x, float y, float width, float height, vec3 color, void* shader_byte_code, size_t size_shader)
{
    m_position = { x, y, 0 };
    m_size = { width, height, 0 };
    m_color = color;

    sbc = shader_byte_code;
    ss = size_shader;

}

void Quad::draw()
{
    if (!m_vb)return;
    GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
    GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertex(), 0);
    //std::cout << "draw\n";
}

void Quad::draw(VertexShader* vs, PixelShader* ps)
{
    if (!m_vb)return;
    GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(vs);
    GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(ps);

    GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
    GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertex(), 0);
}

bool Quad::init()
{

    float half_width = m_size.x / 2.0f;
    float half_height = m_size.y / 2.0f;

    vertex list[] =
    {
        {m_position.x - half_width, m_position.y - half_height, 0.0f, m_color.x, m_color.y, m_color.z},  // bl
        {m_position.x - half_width, m_position.y + half_height, 0.0f, m_color.x, m_color.y, m_color.z},  // tl
        {m_position.x + half_width, m_position.y - half_height, 0.0f, m_color.x, m_color.y, m_color.z},  // br
        {m_position.x + half_width, m_position.y + half_height, 0.0f, m_color.x, m_color.y, m_color.z},  // tr
    };
    std::cout << "creating vb\n";
    m_vb = GraphicsEngine::get()->createVertexBuffer();
    if (m_vb != nullptr)std::cout << "created vb\n";;
    std::cout << "loading vb\n";
    if (m_vb->load(list, sizeof(vertex), ARRAYSIZE(list), sbc, ss))
    {
        std::cout << "loaded vb\n";
    }
    else
    {
        if(m_vb->load(list, sizeof(vertex), ARRAYSIZE(list), sbc, ss))std::cout << "loaded vb\n";
        std::cout << "failed loading vb\n";
        return false;
    }
    return true;
}
//not done yet since its not yet needed
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
