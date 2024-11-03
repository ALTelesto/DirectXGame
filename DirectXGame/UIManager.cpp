#include "UIManager.h"

#include "DeviceContext.h"
#include "GraphicsEngine.h"
#include <exception>

UIManager* UIManager::sharedInstance = nullptr;

UIManager* UIManager::getInstance()
{
	return sharedInstance;
}

void UIManager::initialize(HWND windowHandle)
{
	if (UIManager::sharedInstance) throw std::exception("UIManager already initialized");
	sharedInstance = new UIManager(windowHandle);
}

void UIManager::destroy()
{
	delete sharedInstance;
}

void UIManager::drawAllUI()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow();

	this->drawAboutUI();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::drawAboutUI()
{
	ImGui::Begin("About", &about_active, 0);
	ImGui::SetWindowSize(ImVec2(350, 100));
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "JGRAEngine v.0.0.1");
	ImGui::Text("Developer: Joachim Gabriel R. Arguelles");
	/*ImGui::Text("Changelogs");
	ImGui::Text("v.0.0.1 - Added About window");*/
	ImGui::PushItemWidth(50);
	ImGui::ButtonEx("Close", ImVec2(70, 20), 0);
	ImGui::End();
}

UIManager::UIManager(HWND windowHandle)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplWin32_Init(windowHandle);
	ImGui_ImplDX11_Init(GraphicsEngine::getInstance()->getRenderSystem()->getDirectXDevice(), GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->getContext());
}

UIManager::~UIManager()
{
}
