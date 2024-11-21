#include "MenuScreen.h"

#include "ActionHistory.h"
#include "BaseComponentSystem.h"
#include "Cube.h"
#include "EditorAction.h"
#include "EngineBackend.h"
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

#ifndef M_PI
#define M_PI 3.14159265358979323846 // Define pi if not defined
#endif

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
		if (ImGui::MenuItem("Create Cube") && EngineBackend::getInstance()->getMode() == EngineBackend::EDITOR)
		{
			onCreateCubeClicked();
		}
		if (ImGui::MenuItem("Create Plane") && EngineBackend::getInstance()->getMode() == EngineBackend::EDITOR)
		{
			onCreatePlaneClicked();
		}
		if (ImGui::MenuItem("Create Rigid Body Cube") && EngineBackend::getInstance()->getMode() == EngineBackend::EDITOR)
		{
			onCreateRigidBodyCubeClicked();
		}
		if (ImGui::MenuItem("Create Rigid Body Plane") && EngineBackend::getInstance()->getMode() == EngineBackend::EDITOR)
		{
			onCreateRigidBodyPlaneClicked();
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Windows"))
	{
		if (ImGui::MenuItem("Playback", nullptr, isPlaybackOpen)) isPlaybackOpen = !isPlaybackOpen;
		if (ImGui::MenuItem("Action History", nullptr, isActionHistoryOpen)) isActionHistoryOpen = !isActionHistoryOpen;
		if (ImGui::MenuItem("Object List", nullptr, isObjectListOpen)) isObjectListOpen = !isObjectListOpen;
		if (ImGui::MenuItem("Inspector", nullptr, isInspectorOpen)) isInspectorOpen = !isInspectorOpen;
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
	if (isObjectListOpen)
		showObjectList();
	if (isInspectorOpen)
		showInspector();
	if (isPlaybackOpen)
		showPlayback();
	if (isActionHistoryOpen)
		showActionHistory();
}

void MenuScreen::onCreateCubeClicked()
{
	if (!isMaterialInitialized) initializeMaterial();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	ShaderLibrary::getInstance()->requestVertexShaderData(L"ShadedVertex.hlsl", &shader_byte_code, &size_shader);
	GameObjectPtr cube = std::make_shared<Cube>("Cube", shader_byte_code, size_shader);
	cube->setMaterial(defaultMaterial);
	cube->setScale(0.2, 0.2, 0.2);
	cube->setRotation(0, 0, 0);
	static_cast<Cube*>(cube.get())->setAnimSpeed(0);

	GameObjectManager::getInstance()->addObject(cube);
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
	rigidBodyCube->setRotation(90, 90, 90);
	rigidBodyCube->setPosition(0, 5, 0);
	rigidBodyCube->setScale(0.1, 0.1, 0.1);

	PhysicsComponent* physicsComponent = new PhysicsComponent(name + "Physics component", rigidBodyCube.get());
	physicsComponent->attachOwner(rigidBodyCube.get());
	/*if(physicsComponent->getRigidBody() != nullptr)
	{
		Quaternion quat;
		quat.setToIdentity();
		physicsComponent->getRigidBody()->setTransform(Transform(Vector3(0, 5, 0), quat));
	}*/
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
	physicsComponent->getRigidBody()->setType(BodyType::STATIC);
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

void MenuScreen::showObjectList()
{
	gameObjectMap = GameObjectManager::getInstance()->getObjectHashMap();
	if (ImGui::Begin("Object List", &isObjectListOpen))
	{
		for(std::pair<std::string, GameObjectPtr> pair : gameObjectMap)
		{
			if(ImGui::Button(pair.first.data()))
			{
				selectedGameObject = pair.second;
				position = selectedGameObject->getLocalPosition();
				rotation = selectedGameObject->getLocalRotation();
				scale = selectedGameObject->getLocalScale();

				pos[0] = position.x; pos[1] = position.y; pos[2] = position.z;
				rot[0] = rotation.x; rot[1] = rotation.y; rot[2] = rotation.z;
				sca[0] = scale.x; sca[1] = scale.y; sca[2] = scale.z;

				const float degreesPerRadian = 180.0 / M_PI;
				for (int i = 0; i < 3; ++i) {
					rot[i] *= degreesPerRadian;
				}
			}
		}
	}
	ImGui::End();
}
void MenuScreen::showInspector()
{
	if (ImGui::Begin("Inspector", &isObjectListOpen))
	{
		if(selectedGameObject != nullptr)
		{
			ImGui::Text(selectedGameObject->getName().data());
			if (ImGui::InputFloat3("Position", pos)) this->onTransformUpdate();
			if (ImGui::InputFloat3("Rotation", rot)) this->onTransformUpdate();
			if (ImGui::InputFloat3("Scale", sca)) this->onTransformUpdate();
		}
	}
	ImGui::End();
}

void MenuScreen::onTransformUpdate()
{
	if (selectedGameObject == nullptr) return;
	ActionHistory::getInstance()->recordAction(this->selectedGameObject);

	position.x = pos[0]; position.y = pos[1]; position.z = pos[2];
	rotation.x = rot[0]; rotation.y = rot[1]; rotation.z = rot[2];
	scale.x = sca[0]; scale.y = sca[1]; scale.z = sca[2];

	selectedGameObject->setPosition(position);
	selectedGameObject->setRotation(rotation);
	selectedGameObject->setScale(scale);
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

void MenuScreen::showPlayback()
{
	EngineBackend* backend = EngineBackend::getInstance();

	if (ImGui::Begin("Playback", &isPlaybackOpen))
	{
		ImGui::SetWindowSize(ImVec2(205, 75));
		ImGui::SameLine();

		if(backend->getMode() == EngineBackend::EDITOR)
		{
			if (ImGui::Button("Play")) backend->setMode(EngineBackend::PLAY);
		}

		else if (backend->getMode() != EngineBackend::EDITOR)
		{
			if (ImGui::Button("Stop")) backend->setMode(EngineBackend::EDITOR);
		}

		ImGui::SameLine();

		if (backend->getMode() == EngineBackend::PLAY)
		{
			if (ImGui::Button("Pause")) backend->setMode(EngineBackend::PAUSED);
		}
		else if (backend->getMode() == EngineBackend::PAUSED)
		{
			if (ImGui::Button("Resume")) backend->setMode(EngineBackend::PLAY);
		}

		ImGui::SameLine();
		if (backend->getMode() == EngineBackend::PAUSED && ImGui::Button("Frame Step")) backend->startFrameStep();
	}
	ImGui::End();
}

void MenuScreen::showActionHistory()
{
	if (ImGui::Begin("Actions", &isColorPickerOpen))
	{
		EditorAction* action = nullptr;
		if (ImGui::Button("Undo") && action == nullptr)
		{
			action = ActionHistory::getInstance()->undoAction();
		}
		if (ImGui::Button("Redo") && action == nullptr)
		{
			action = ActionHistory::getInstance()->redoAction();
		}
		if (action != nullptr)
		{
			GameObjectPtr gameObject = GameObjectManager::getInstance()->findObjectByName(action->getOwnerName());
			if (gameObject != nullptr) gameObject->setState(action);
		}
	}
	ImGui::End();
}
