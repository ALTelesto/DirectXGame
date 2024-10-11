#pragma once
#include "Vector3D.h"

class VertexBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;

class Circle
{
public:
    Circle(float x, float y, float radius, Vector3D color);
    ~Circle();

    void draw(VertexShader* vs, PixelShader* ps);

    // Setters
    void setPosition(float x, float y);

    // Getters
    float getRadius() const;

    // New methods
    void setSpeed(float speed);
    float getSpeed();
    void setDirection(float angle);
    float getDirection();

private:
    VertexBuffer* m_vb;
    IndexBuffer* m_ib;

    Vector3D m_position;
    Vector3D m_color;
    float m_radius;

    // Variables for movement
    float m_speed;
    float m_direction;  // Angle in radians
};