#include "MenuScreen.h"

#include "BaseComponentSystem.h"
#include "GraphicsEngine.h"

#include "LogUtils.h"

#include "RigidBodyCube.h"
#include "RigidBodyPlane.h"
#include "PhysicsComponent.h"

#include "GameObjectManager.h"
#include "MaterialResource.h"
#include "Plane.h"
#include "ShaderLibrary.h"
#include "ShaderNames.h"

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

	if (ImGui::BeginMenu("Game Object")) {
		if (ImGui::MenuItem("Create Plane"))
		{
			onCreatePlaneClicked();
		}
		if (ImGui::MenuItem("Create Rigid Body Cube"))
		{
			onCreateRigidBodyCubeClicked();
		}
		if (ImGui::MenuItem("Create Rigid Body Plane"))
		{
			onCreateRigidBodyPlaneClicked();
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Windows"))
	{
		if (ImGui::MenuItem("Color Picker", nullptr, isColorPickerOpen)) isColorPickerOpen = !isColorPickerOpen;
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("About"))
	{
		if (ImGui::MenuItem("Credits", nullptr, isCreditsOpen)) isCreditsOpen = !isCreditsOpen;
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	if (isCreditsOpen)
		showCreditsWindow();
	if (isColorPickerOpen)
		showColorPickerWindow();
}

void MenuScreen::onCreatePlaneClicked()
{
	if (!isMaterialInitialized) initializeMaterial();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	ShaderLibrary::getInstance()->requestVertexShaderData(L"ShadedVertex.hlsl", &shader_byte_code, &size_shader);
	GameObjectPtr plane = std::make_shared<Plane>("Plane", shader_byte_code, size_shader);
	plane->setMaterial(defaultMaterial);
	plane->setScale(5, 5, 0.1);
	plane->setRotation(90, 0, 0);

	GameObjectManager::getInstance()->addObject(plane);
}

void MenuScreen::onCreateRigidBodyCubeClicked()
{
	if (!isMaterialInitialized) initializeMaterial();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	ShaderLibrary::getInstance()->requestVertexShaderData(L"ShadedVertex.hlsl", &shader_byte_code, &size_shader);

	GameObjectPtr rigidBodyCube = std::make_shared<RigidBodyCube>("RigidBodyCube", shader_byte_code, size_shader);
	rigidBodyCube->setMaterial(defaultMaterial2);

	rigidBodyCube->setPosition(0, 5, 0);
	rigidBodyCube->setScale(0.1, 0.1, 0.1);

	PhysicsComponent* physicsComponent = new PhysicsComponent(name + "Physics component", rigidBodyCube.get());
	physicsComponent->attachOwner(rigidBodyCube.get());
	if(physicsComponent->getRigidBody() != nullptr)
	{
		Quaternion quat;
		quat.setToIdentity();
		physicsComponent->getRigidBody()->setTransform(Transform(Vector3(0, 5, 0), quat));
	}
	BaseComponentSystem::getInstance()->getPhysicsSystem()->registerComponent(physicsComponent);

	GameObjectManager::getInstance()->addObject(rigidBodyCube);
}

void MenuScreen::onCreateRigidBodyPlaneClicked()
{
	if (!isMaterialInitialized) initializeMaterial();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	ShaderLibrary::getInstance()->requestVertexShaderData(L"ShadedVertex.hlsl", &shader_byte_code, &size_shader);

	GameObjectPtr rigidBodyPlane = std::make_shared<RigidBodyPlane>("RigidBodyPlane", shader_byte_code, size_shader);
	rigidBodyPlane->setMaterial(defaultMaterial);

	rigidBodyPlane->setScale(10, 10, 0.1);
	rigidBodyPlane->setRotation(90, 0, 0);

	PhysicsComponent* physicsComponent = new PhysicsComponent(name + "Physics component", rigidBodyPlane.get());
	physicsComponent->attachOwner(rigidBodyPlane.get());
	physicsComponent->getRigidBody()->setType(BodyType::KINEMATIC);
	BaseComponentSystem::getInstance()->getPhysicsSystem()->registerComponent(physicsComponent);
	

	GameObjectManager::getInstance()->addObject(rigidBodyPlane);
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

void MenuScreen::initializeMaterial()
{
	PixelShaderPtr ps = ShaderLibrary::getInstance()->getPixelShader(ShaderNames::PixelShaderNames.find("ShadedTextured")->first);
	VertexShaderPtr vs = ShaderLibrary::getInstance()->getVertexShader(ShaderNames::VertexShaderNames.find("VertexShader")->first);

	defaultMaterial = std::make_shared<MaterialResource>(ps, vs);
	defaultMaterial->samplerState = GraphicsEngine::getInstance()->getRenderSystem()->createSamplerState();
	defaultMaterial->smoothness = 1;

	defaultMaterial2 = std::make_shared<MaterialResource>(ps, vs);
	defaultMaterial2->color = Vector4D(1, 0, 0, 1);
	defaultMaterial2->samplerState = GraphicsEngine::getInstance()->getRenderSystem()->createSamplerState();

	this->isMaterialInitialized = true;
}
