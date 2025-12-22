#include <iostream>

#include "Window.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "MaterialLit.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "FishCollection.h"
#include "Debug.h"

const float walkSpeed = 1;
const float runSpeed = 3;

//console debug stuff
void OpenConsole() {
	if (AllocConsole()) {
		//redirecting input, output and errors to allocated console
		FILE* fp = nullptr;
		freopen_s(&fp, "CONIN$", "r", stdin);
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);
		std::ios::sync_with_stdio(true);

		std::cout << "Hello Side Console! :)" << std::endl;
	}
}

//declare static vars from other classes
int FishCollection::fishCollected = 0;

//window main
int WINAPI WinMain(_In_ HINSTANCE instanceH, _In_opt_ HINSTANCE prevInstanceH, _In_ LPSTR lpCmdLine, _In_ int  nCmdShow) {
	//initialise window with error check
	Window window{ 800, 600, instanceH, nCmdShow };
	Renderer renderer{ window };

	//get models and textures
	Mesh mesh_cube{ renderer, "Assets/Models/cube.obj" };
	Mesh mesh_fish{ renderer, "Assets/Models/fish.obj" };
	Mesh mesh_bassfish{ renderer, "Assets/Models/bass.obj" };
	Mesh mesh_carpfish{ renderer, "Assets/Models/carp.obj" };
	Mesh mesh_tailorfish{ renderer, "Assets/Models/tailor.obj" };
	Mesh mesh_goldfish{ renderer, "Assets/Models/goldfish.obj" };
	Mesh mesh_grass{ renderer, "Assets/Models/grass.obj", true };
	
	Texture tex_skybox{ renderer, "Assets/Textures/Skybox/retro_skyboxes_pack/Sinister Ocean/vz_sinister_ocean_cubemap_ue.dds", false, Texture::TextureType::Cubemap };
	Texture tex_fish{ renderer, "Assets/Textures/fish.png", };
	Texture tex_bassfish{ renderer, "Assets/Textures/bass.png", };
	Texture tex_carpfish{ renderer, "Assets/Textures/carp.png" };
	Texture tex_tailorfish{ renderer, "Assets/Textures/tailor.png" };
	Texture tex_goldfish{ renderer, "Assets/Textures/goldfish.png" };
	Texture tex_yellowcoral{ renderer, "Assets/Textures/yellowcoral.png", true };
	Texture tex_pinkcoral{ renderer, "Assets/Textures/pinkcoral.png", true };
	
	//make materials
	MaterialLit mat_skybox{ "Lit Skybox", renderer, "Compiled Shaders/SkyboxVShader.cso", "Compiled Shaders/SkyboxFShader.cso", &tex_skybox };
	MaterialLit mat_yellowcoral{ "Lit Yellow Coral", renderer, "Compiled Shaders/VertexShader.cso", "Compiled Shaders/FragmentShader.cso", &tex_yellowcoral };
	MaterialLit mat_pinkcoral{ "Lit Yellow Coral", renderer, "Compiled Shaders/VertexShader.cso", "Compiled Shaders/FragmentShader.cso", &tex_pinkcoral };
	MaterialLit mat_fish{ "Lit Fish", renderer, "Compiled Shaders/VertexShader.cso", "Compiled Shaders/FragmentShader.cso", &tex_fish };

	MaterialLit mat_shinybassfish{ "Lit Basic Fish", renderer, "Compiled Shaders/ReflectiveVShader.cso", "Compiled Shaders/ReflectiveFShader.cso", &tex_bassfish };
	mat_shinybassfish.SetReflectionTexture(&tex_skybox);
	MaterialLit mat_shinycarpfish{ "Lit Carp Fish", renderer, "Compiled Shaders/ReflectiveVShader.cso", "Compiled Shaders/ReflectiveFShader.cso", &tex_carpfish };
	mat_shinycarpfish.SetReflectionTexture(&tex_skybox);
	MaterialLit mat_shinytailorfish{ "Lit Tailor Fish", renderer, "Compiled Shaders/ReflectiveVShader.cso", "Compiled Shaders/ReflectiveFShader.cso", &tex_tailorfish };
	mat_shinytailorfish.SetReflectionTexture(&tex_skybox);
	MaterialLit mat_shinygoldfish{ "Lit Basic Fish", renderer, "Compiled Shaders/ReflectiveVShader.cso", "Compiled Shaders/ReflectiveFShader.cso", &tex_goldfish };
	mat_shinygoldfish.SetReflectionTexture(&tex_skybox);

	//skybox object
	GameObject obj_skybox{ "Skybox", &mesh_cube, &mat_skybox };
	renderer.skyboxObject = &obj_skybox;

	//make gameobjects (render transparent objects last!!)
	GameObject objFish{ "Basic Fish", &mesh_fish, &mat_fish };
	GameObject objBassFish{ "Bass Fish", &mesh_bassfish, &mat_shinybassfish };
	GameObject objCarpFish{ "Carp Fish", &mesh_carpfish, &mat_shinycarpfish };
	GameObject objTailorFish{ "Tailor Fish", &mesh_tailorfish, &mat_shinytailorfish };
	GameObject objGoldFish{ "Gold Fish", &mesh_goldfish, &mat_shinygoldfish };
	GameObject objYellowCoral{ "Yellow Coral", &mesh_grass, &mat_yellowcoral };
	GameObject objPinkCoral{ "Pink Coral", &mesh_grass, &mat_pinkcoral };

	//set lighting
	renderer.directionalLights[0] = { DirectX::XMVECTOR{ 0.3f, 0.7f, 0.7f }, { 0, 0.8f, 0.75f }, true };
	renderer.directionalLights[1] = { DirectX::XMVECTOR{ 0.0f, 0.2f, 0.5f }, { 0.8f, 0.75f, 0 }, true };
	renderer.pointLights[0] = { DirectX::XMVECTOR{-4, 1, -1}, {0.85f, 0, 0.85f}, 10, true };
	renderer.pointLights[1] = { DirectX::XMVECTOR{2, -1, -1}, {0, 0.85f, 0.85f}, 20, true };

	//set camera and gameobject positions
	renderer.camera.transform.SetPosition({ 0, 0, -5 });

	renderer.RegisterGameObject(&objFish);
	objFish.transform.SetPosition({ -2, 0, 3, 1 });
	objFish.transform.SetScale({ 0.7f, 0.7f, 0.7f, 0.7f });
	objFish.transform.Rotate({ 0, 80, 0 });

	renderer.RegisterGameObject(&objBassFish);
	objBassFish.transform.SetPosition({ 3, 0, -2, 1 });
	objBassFish.transform.SetScale({ 0.5f, 0.5f, 0.5f, 0.5f });
	objBassFish.transform.Rotate({ 0, -60, 0 });

	renderer.RegisterGameObject(&objCarpFish);
	objCarpFish.transform.SetPosition({ -4, 0, 6, 1 });
	objCarpFish.transform.SetScale({3, 3, 3, 3});
	objCarpFish.transform.Rotate({ 0, 50, 0 });

	renderer.RegisterGameObject(&objTailorFish);
	objTailorFish.transform.SetPosition({ -1, 0, 0, 1 });
	objTailorFish.transform.SetScale({ 2, 2, 2, 2 });
	objTailorFish.transform.Rotate({ 0, -100, 0 });

	renderer.RegisterGameObject(&objGoldFish);
	objGoldFish.transform.SetPosition({ 2, 0, -4, 1 });
	objGoldFish.transform.SetScale({ 6, 6, 6, 6 });
	objGoldFish.transform.Rotate({ 0, -80, 0 });

	renderer.RegisterGameObject(&objYellowCoral);
	objYellowCoral.transform.SetPosition({ 2, -4, 5, 1 });
	renderer.RegisterGameObject(&objPinkCoral);
	objPinkCoral.transform.SetPosition({ -1, -4, 4, 1 });

	//game variables
	float loopBoundaries = 10;
	float deltaTime = CLOCKS_PER_SEC; 
	deltaTime /= 1000000;
	float moveSpeed = walkSpeed * deltaTime;

	//declare keyboard tracker
	DirectX::Keyboard::KeyboardStateTracker kbTracker;

	//used to hold windows event messages
	MSG msg;

	//main game loop
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg); //translate certain messages into correct format, namely key presses

			DispatchMessage(&msg); //send the message to the windowproc function

			if (msg.message == WM_QUIT) break; //break out of the loop if get quit message
		}
		else {
			//get current time
			clock_t endOfFrameTime = clock() + CLOCKS_PER_SEC;

			//get keyboard input state
			auto kbState = DirectX::Keyboard::Get().GetState();
			kbTracker.Update(kbState);

			if (kbTracker.pressed.Escape) {
				PostQuitMessage(0);
			}

			if (kbTracker.pressed.LeftShift) {
				moveSpeed = runSpeed * deltaTime;
			}

			if (kbTracker.released.LeftShift) {
				moveSpeed = walkSpeed * deltaTime;
			}

			//keyboard camera movement
			if (kbTracker.lastState.W) {
				renderer.camera.transform.Translate(DirectX::XMVectorScale(renderer.camera.transform.GetForward(), moveSpeed));
			}
			if (kbTracker.lastState.S) {
				renderer.camera.transform.Translate(DirectX::XMVectorScale(renderer.camera.transform.GetForward(), -moveSpeed));
			}
			if (kbTracker.lastState.A) {
				renderer.camera.transform.Translate(DirectX::XMVectorScale(renderer.camera.transform.GetRight(), -moveSpeed));
			}
			if (kbTracker.lastState.D) {
				renderer.camera.transform.Translate(DirectX::XMVectorScale(renderer.camera.transform.GetRight(), moveSpeed));
			}
			if (kbTracker.lastState.Q) {
				renderer.camera.transform.Translate(DirectX::XMVectorScale(renderer.camera.transform.GetUp(), moveSpeed));
			}
			if (kbTracker.lastState.E) {
				renderer.camera.transform.Translate(DirectX::XMVectorScale(renderer.camera.transform.GetUp(), -moveSpeed));
			}

			//get mouse input state
			auto msState = DirectX::Mouse::Get().GetState();

			//mouse camera rotate
			renderer.camera.transform.Rotate({ -(float)msState.y * 0.001f, (float)msState.x * 0.001f, 0 });
			if (msState.leftButton) {
				renderer.camera.transform.SetPosition({ 0, 0, -5 });
			}

			//camera collision
			if (BoxCollider::BoxCollision(renderer.camera.transform, objFish.transform)) { renderer.RemoveGameObject(&objFish); objFish.transform.SetScale({ 0, 0, 0 }); FishCollection::fishCollected++; }
			if (BoxCollider::BoxCollision(renderer.camera.transform, objBassFish.transform)) { renderer.RemoveGameObject(&objBassFish); objBassFish.transform.SetScale({ 0, 0, 0 }); FishCollection::fishCollected++;}
			if (BoxCollider::BoxCollision(renderer.camera.transform, objCarpFish.transform)) { renderer.RemoveGameObject(&objCarpFish); objCarpFish.transform.SetScale({ 0, 0, 0 }); FishCollection::fishCollected++; }
			if (BoxCollider::BoxCollision(renderer.camera.transform, objTailorFish.transform)) { renderer.RemoveGameObject(&objTailorFish); objTailorFish.transform.SetScale({ 0, 0, 0 }); FishCollection::fishCollected++; }
			if (BoxCollider::BoxCollision(renderer.camera.transform, objGoldFish.transform)) { renderer.RemoveGameObject(&objGoldFish); objGoldFish.transform.SetScale({ 0, 0, 0 }); FishCollection::fishCollected++; }

			objFish.transform.Translate(
				DirectX::XMVectorScale(objFish.transform.GetForward(), deltaTime));
			objFish.transform.Rotate({ 0, deltaTime/8, 0 });
			objBassFish.transform.Translate(
				DirectX::XMVectorScale(objBassFish.transform.GetForward(), deltaTime));
			objBassFish.transform.Rotate({ 0, deltaTime/5, cos(-deltaTime) / 1000 });
			objCarpFish.transform.Translate(
				DirectX::XMVectorScale(objCarpFish.transform.GetForward(), deltaTime));
			objCarpFish.transform.Rotate({ 0, -deltaTime/8, cos(deltaTime) / 5000 });
			objTailorFish.transform.Translate(
				DirectX::XMVectorScale(objTailorFish.transform.GetForward(), deltaTime));
			objGoldFish.transform.Translate(
				DirectX::XMVectorScale(objGoldFish.transform.GetForward(), deltaTime));
			objGoldFish.transform.Rotate({ 0, -deltaTime, sin(deltaTime) });

			objYellowCoral.transform.Rotate({ 0, -deltaTime/2, 0 });
			objPinkCoral.transform.Rotate({ 0, -deltaTime/2, 0 });

			for (GameObject* obj : renderer.gameObjects) {
				//object transform boundaries are looping at 10, 10
				float posx = DirectX::XMVectorGetX(obj->transform.GetPosition());
				float posz = DirectX::XMVectorGetZ(obj->transform.GetPosition());
				
				//check x boundary
				if (posx > loopBoundaries) { posx = -loopBoundaries; }
				else if (posx < -loopBoundaries) { posx = loopBoundaries; }

				//check z boundary
				if (posz > loopBoundaries) { posz = -loopBoundaries; }
				else if (posz < -loopBoundaries) { posz = loopBoundaries; }

				//update position
				obj->transform.SetPosition({ posx, DirectX::XMVectorGetY(obj->transform.GetPosition()), posz});

				//object collision if fish
				if (BoxCollider::BoxCollision(renderer.camera.transform, obj->transform) 
					&& obj->GetName().find("Fish")) {
					renderer.RemoveGameObject(obj); 
				}
			}

			renderer.RenderFrame();
			LOG(std::to_string(FishCollection::fishCollected));
		}
	}

	renderer.Clean();

	return 0;
}