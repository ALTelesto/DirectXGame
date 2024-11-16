#pragma once

#include <wrl/client.h>
#include "WICTextureLoader.h"
#include "DirectXHelpers.h"
#include "Prerequisites.h"

#include "UIScreen.h"
class MenuScreen :
    public UIScreen
{
public:
    MenuScreen();

private:
    void draw() override;

    void onCreatePlaneClicked();
    void onCreateRigidBodyCubeClicked();
    void onCreateRigidBodyPlaneClicked();

    void showCreditsWindow();
    void showColorPickerWindow();

    bool isColorPickerOpen = false;
    bool isCreditsOpen = false;

    bool isMaterialInitialized = false;

    void initializeMaterial();

    MaterialPtr defaultMaterial;
    MaterialPtr defaultMaterial2;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> creditsTexture;
};

