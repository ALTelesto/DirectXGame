#pragma once

#include <wrl/client.h>
#include "WICTextureLoader.h"
#include "DirectXHelpers.h"
#include "Prerequisites.h"

#include "UIScreen.h"
#include "Vector3D.h"

class MenuScreen :
    public UIScreen
{
public:
    MenuScreen();

private:
    void draw() override;

    void onCreateCubeClicked();
    void onCreatePlaneClicked();
    void onCreateRigidBodyCubeClicked();
    void onCreateRigidBodyPlaneClicked();

    void showCreditsWindow();
    void showColorPickerWindow();
    void showObjectList();
    void showInspector();

    bool isColorPickerOpen = false;
    bool isCreditsOpen = false;

    bool isObjectListOpen = false;
    bool isInspectorOpen = false;

    GameObjectList gameObjectList;
    GameObjectHashMap gameObjectMap;
    GameObjectPtr selectedGameObject;

    Vector3D position, rotation, scale;

    bool isMaterialInitialized = false;

    void initializeMaterial();

    MaterialPtr defaultMaterial;
    MaterialPtr defaultMaterial2;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> creditsTexture;
};

