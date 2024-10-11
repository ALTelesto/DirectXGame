#include "Circle.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

#define NUM_SEGMENTS 30 // Defines how smooth the circle is (more segments = smoother circle)

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
    // Randomize the initial direction (angle in radians)
    m_direction = static_cast<float>(rand()) / RAND_MAX * 2.0f * PI;

    // Generate vertices for a circle centered at (x, y) with a given radius
    vertex* vertices = new vertex[NUM_SEGMENTS + 2]; // +2 for the center point and the final point closing the loop
    vertices[0] = { Vector3D(x, y, 0.0f), Vector3D(m_color.m_x, m_color.m_y, m_color.m_z) }; // Center vertex

    for (int i = 1; i <= NUM_SEGMENTS + 1; ++i) // Correct number of vertices
    {
        float angle = (float)(i - 1) * 2.0f * PI / NUM_SEGMENTS; // Correctly distribute angles around the circle
        float vx = x + cos(angle) * m_radius;
        float vy = y + sin(angle) * m_radius;
        vertices[i] = { Vector3D(vx, vy, 0.0f), Vector3D(m_color.m_x, m_color.m_y, m_color.m_z) };
        std::cout << vx <<" " << vy << " \n";
    }

    m_vb = GraphicsEngine::getInstance()->createVertexBuffer();
    m_vb->load(vertices, sizeof(vertex), NUM_SEGMENTS + 2, nullptr, 0); // Use NUM_SEGMENTS + 2 here

    delete[] vertices;
}

void Circle::draw(VertexShader* vs, PixelShader* ps)
{
    GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexShader(vs);
    GraphicsEngine::getInstance()->getImmediateDeviceContext()->setPixelShader(ps);

    GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
    GraphicsEngine::getInstance()->getImmediateDeviceContext()->drawTriangleFan(m_vb->getSizeVertex(), 0);
}

void Circle::setPosition(float x, float y)
{
    m_position.m_x = x;
    m_position.m_y = y;
    // Update the vertex buffer to move the circle to the new position
    vertex* vertices = new vertex[NUM_SEGMENTS + 1];
    vertices[0] = { Vector3D(x, y, 0.0f), Vector3D(m_color.m_x, m_color.m_y, m_color.m_z) }; // Center vertex

    for (int i = 1; i <= NUM_SEGMENTS; ++i)
    {
        float angle = (float)(i - 1) * 2.0f * 3.14159265359f / NUM_SEGMENTS;
        float vx = x + cos(angle) * m_radius;
        float vy = y + sin(angle) * m_radius;
        vertices[i] = { Vector3D(vx, vy, 0.0f), Vector3D(m_color.m_x, m_color.m_y, m_color.m_z) };
    }

    m_vb->load(vertices, sizeof(vertex), NUM_SEGMENTS + 1, nullptr, 0);

    delete[] vertices;
}

float Circle::getRadius() const
{
    return m_radius;
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