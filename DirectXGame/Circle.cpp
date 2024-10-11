#include "Circle.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include <cmath>
#include <vector>
#include <cstdlib>
#include <iostream>

#define NUM_SEGMENTS 30

#ifndef PI
#define PI 3.14159265359f
#endif

struct vertex
{
    Vector3D position;
    Vector3D color;
};

Circle::Circle(float x, float y, float radius, Vector3D color)
    : m_position({ x, y, 0.0f }), m_color(color), m_radius(radius)
{
    m_direction = static_cast<float>(rand()) / RAND_MAX * 2.0f * PI;

    std::vector<vertex> vertices;

    vertex centerVertex;
    centerVertex.position = Vector3D(x, y, 0.0f);
    centerVertex.color = m_color;
    vertices.push_back(centerVertex);

    for (int i = 0; i <= NUM_SEGMENTS; ++i) {
        float theta = (2.0f * PI * i) / NUM_SEGMENTS;
        vertex edgeVertex;
        edgeVertex.position = Vector3D(x, y, 0) + Vector3D(radius * cos(theta), radius * sin(theta), 0.0f);
        edgeVertex.color = m_color;
        vertices.push_back(edgeVertex);

        edgeVertex;
        edgeVertex.position = Vector3D(x, y, 0) + Vector3D(0, 0, 0);
        edgeVertex.color = m_color;
        vertices.push_back(edgeVertex);
    }

    m_vb = GraphicsEngine::getInstance()->createVertexBuffer();
    m_vb->load(vertices.data(), sizeof(vertex), vertices.size(), nullptr, 0);

}

void Circle::draw(VertexShader* vs, PixelShader* ps)
{
    GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexShader(vs);
    GraphicsEngine::getInstance()->getImmediateDeviceContext()->setPixelShader(ps);

    GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

    // i dont understand why this works
    GraphicsEngine::getInstance()->getImmediateDeviceContext()->drawTriangleList(m_vb->getSizeVertex(), 0);
    GraphicsEngine::getInstance()->getImmediateDeviceContext()->drawTriangleList(m_vb->getSizeVertex()-1, 1);
    GraphicsEngine::getInstance()->getImmediateDeviceContext()->drawTriangleList(m_vb->getSizeVertex()-2, 2);
    // but i'll take it
}

//similar to constructor
void Circle::setPosition(float x, float y)
{
    m_position.m_x = x;
    m_position.m_y = y;
    std::vector<vertex> vertices;

    vertex centerVertex;
    centerVertex.position = Vector3D(m_position.m_x, m_position.m_y, 0.0f);
    centerVertex.color = m_color;
    vertices.push_back(centerVertex);

    for (int i = 0; i <= NUM_SEGMENTS; ++i) {
        float theta = (2.0f * PI * i) / NUM_SEGMENTS;
        vertex edgeVertex;
        edgeVertex.position = Vector3D(m_position.m_x, m_position.m_y, 0) + Vector3D(m_radius * cos(theta), m_radius * sin(theta), 0.0f);
        edgeVertex.color = m_color;
        vertices.push_back(edgeVertex);

        edgeVertex.position = Vector3D(m_position.m_x, m_position.m_y, 0.0f);
        edgeVertex.color = m_color;
        vertices.push_back(edgeVertex);
        
    }

    m_vb->load(vertices.data(), sizeof(vertex), vertices.size(), nullptr, 0);
}

void Circle::move(float deltaTime)
{
    float vx = cos(m_direction) * m_speed;
    float vy = sin(m_direction) * m_speed;

    m_position.m_x += vx * deltaTime;
    m_position.m_y += vy * deltaTime;

    setPosition(m_position.m_x, m_position.m_y);
}

float Circle::getRadius() const
{
    return m_radius;
}

Vector3D Circle::getPosition()
{
    return m_position;
}

Circle::~Circle()
{
    if (m_vb)
    {
        m_vb->release();
    }
}

void Circle::setSpeed(float speed)
{
    m_speed = speed;
}

float Circle::getSpeed()
{
    return m_speed;
}

void Circle::setDirection(float angle)
{
    m_direction = angle;
}

float Circle::getDirection()
{
    return m_direction;
}

void Circle::setColor(Vector3D color)
{
    m_color = color;
}
