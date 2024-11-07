#include "MenuScreen.h"
#include "GraphicsEngine.h"

#include "LogUtils.h"

MenuScreen::MenuScreen() : UIScreen("Menu Screen")
{
	LogUtils::logHResult(
		this,
		DirectX::CreateWICTextureFromFile(
			GraphicsEngine::getInstance()->getRenderSystem()->getDirectXDevice(),
			L"images/DLSU-LOGO.png",
			nullptr,
			creditsTexture.ReleaseAndGetAddressOf()));
}

void MenuScreen::draw()
{
	if (!LogUtils::logBool(this, ImGui::BeginMainMenuBar())) return;

	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("Open", nullptr)){}
		if (ImGui::MenuItem("Save", nullptr)){}
		if (ImGui::MenuItem("Save As", nullptr)){}
		ImGui::Separator();

		if (ImGui::MenuItem("Quit", nullptr)){}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("About"))
	{
		if (ImGui::MenuItem("Credits", nullptr, isCreditsOpen)) isCreditsOpen = !isCreditsOpen;
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Windows"))
	{
		if (ImGui::MenuItem("Color Picker", nullptr, isColorPickerOpen)) isColorPickerOpen = !isColorPickerOpen;
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	if (isCreditsOpen)
		showCreditsWindow();
	if (isColorPickerOpen)
		showColorPickerWindow();
}

void MenuScreen::showCreditsWindow()
{
	if (ImGui::Begin("Credits", &isCreditsOpen))
	{
		ImGui::SetWindowSize(ImVec2(300, 415));
		constexpr ImVec2 imageSize = { 200, 200 };
		ImGui::Image(static_cast<ImTextureID>(reinterpret_cast<intptr_t>(creditsTexture.Get())), imageSize);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "JGRAEngine v.0.0.1");

		ImGui::NewLine();
		ImGui::Text("Developer: Joachim Gabriel R. Arguelles");

		ImGui::NewLine();
		ImGui::Text("Acknowledgements:");
		ImGui::Text("Dr. Neil Del Gallego's GDENG03 Course");
		ImGui::Text("PardCode Game Engine Tutorial");
		ImGui::Text("ID121's broken hopes and dreams");
		ImGui::NewLine();

		ImGui::Text("Dear ImGui Version: %s (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);

		if (ImGui::Button("OK"))
			isCreditsOpen = false;
	}
	ImGui::End();
}

void MenuScreen::showColorPickerWindow()
{
	if (ImGui::Begin("Color Picker", &isColorPickerOpen))
	{
		static ImVec4 color(1.0f, 0.0f, 1.0f, 0.5f);
		ImGui::ColorPicker4("Color##4", reinterpret_cast<float*>(&color), 0);
	}
	ImGui::End();
}
