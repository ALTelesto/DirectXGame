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

    void showCreditsWindow();
    void showColorPickerWindow();

    bool isColorPickerOpen = false;
    bool isCreditsOpen = false;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> creditsTexture;
};

