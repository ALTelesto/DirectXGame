#pragma once
#include "AGameObject.h"
class MeshedObject :
    public AGameObject
{
public:
    MeshedObject(string name, wstring meshName);
    MeshedObject(string name, MeshPtr mesh);

    void update(float deltaTime) override;
    void draw(const RECT clientWindow) override;
    
private:
    MeshPtr mesh;

    bool hasSetBaseScale = false;
    float baseScale = 0.2;
};

