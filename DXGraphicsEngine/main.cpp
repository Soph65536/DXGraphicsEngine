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

const float walkSpeed = 0.001f;
const float runSpeed = 0.003f;

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
	Mesh mesh_carpfish{ renderer, "Assets/Models/carp.obj" };
	Mesh mesh_tailorfish{ renderer, "Assets/Models/tailor.obj" };
	//Mesh mesh_jellyfish{ renderer, "Assets/Models/jellyfish.obj" };
	//Mesh mesh_mantaray{ renderer, "Assets/Models/mantaray.obj" };
	//Mesh mesh_squid{ renderer, "Assets/Models/squid.obj" };
	//Mesh mesh_whale{ renderer, "Assets/Models/whale.obj" };
	Mesh mesh_grass{ renderer, "Assets/Models/grass.obj", true };
	
	Texture tex_skybox{ renderer, "Assets/Textures/Skybox/skybox02.dds", false, Texture::TextureType::Cubemap };
	Texture tex_carpfish{ renderer, "Assets/Textures/carp.png" };
	Texture tex_tailorfish{ renderer, "Assets/Textures/tailor.png" };
	//Texture tex_jellyfish{ renderer, "Assets/Textures/jellyfish.png" };
	//Texture tex_mantaray{ renderer, "Assets/Textures/mantaray.png" };
	//Texture tex_squid{ renderer, "Assets/Textures/squid.png" };
	//Texture tex_whale{ renderer, "Assets/Textures/whale.png" };
	Texture tex_flower{ renderer, "Assets/Textures/flower.png", true };
	
	//make materials
	MaterialLit mat_skybox{ "Lit Skybox", renderer, "Compiled Shaders/SkyboxVShader.cso", "Compiled Shaders/SkyboxFShader.cso", &tex_skybox };
	MaterialLit mat_flower{ "Lit Flower", renderer, "Compiled Shaders/VertexShader.cso", "Compiled Shaders/FragmentShader.cso", &tex_flower };
	MaterialLit mat_carpfish{ "Lit Angle Fish", renderer, "Compiled Shaders/VertexShader.cso", "Compiled Shaders/FragmentShader.cso", &tex_carpfish };

	MaterialLit mat_shinytailorfish{ "Lit Basic Fish", renderer, "Compiled Shaders/ReflectiveVShader.cso", "Compiled Shaders/ReflectiveFShader.cso", &tex_tailorfish };
	mat_shinytailorfish.SetReflectionTexture(&tex_skybox);
	//MaterialLit mat_shinyjellyfish{ "Lit Basic Fish", renderer, "Compiled Shaders/ReflectiveVShader.cso", "Compiled Shaders/ReflectiveFShader.cso", &tex_jellyfish };
	//mat_shinyjellyfish.SetReflectionTexture(&tex_skybox);
	//MaterialLit mat_shinymantaray{ "Lit Basic Fish", renderer, "Compiled Shaders/ReflectiveVShader.cso", "Compiled Shaders/ReflectiveFShader.cso", &tex_mantaray };
	//mat_shinymantaray.SetReflectionTexture(&tex_skybox);
	//MaterialLit mat_shinysquid{ "Lit Basic Fish", renderer, "Compiled Shaders/ReflectiveVShader.cso", "Compiled Shaders/ReflectiveFShader.cso", &tex_squid };
	//mat_shinysquid.SetReflectionTexture(&tex_skybox);
	//MaterialLit mat_shinywhale{ "Lit Basic Fish", renderer, "Compiled Shaders/ReflectiveVShader.cso", "Compiled Shaders/ReflectiveFShader.cso", &tex_whale };
	//mat_shinywhale.SetReflectionTexture(&tex_skybox);

	//skybox object
	GameObject obj_skybox{ "Skybox", &mesh_cube, &mat_skybox };
	renderer.skyboxObject = &obj_skybox;

	//make gameobjects (render transparent objects last!!)
	GameObject objCarpFish{ "AngleFish", &mesh_carpfish, &mat_carpfish };
	GameObject objTailorFish{ "BasicFish", &mesh_tailorfish, &mat_shinytailorfish };
	//GameObject objJellyFish{ "BasicFish", &mesh_jellyfish, &mat_shinyjellyfish };
	//GameObject objMantaRay{ "BasicFish", &mesh_mantaray, &mat_shinymantaray };
	//GameObject objSquid{ "BasicFish", &mesh_squid, &mat_shinysquid };
	//GameObject objWhale{ "BasicFish", &mesh_whale, &mat_shinywhale };
	GameObject objFlower{ "Flower", &mesh_grass, &mat_flower };

	//set lighting
	renderer.directionalLights[0] = { DirectX::XMVECTOR{ 0.3f, 0.7f, 0.7f }, { 0, 0.8f, 0.75f }, true };
	renderer.directionalLights[1] = { DirectX::XMVECTOR{ 0.0f, 0.2f, 0.5f }, { 0.8f, 0.75f, 0 }, true };
	renderer.pointLights[0] = { DirectX::XMVECTOR{-4, 1, -1}, {0.85f, 0, 0.85f}, 10, true };
	renderer.pointLights[1] = { DirectX::XMVECTOR{2, -1, -1}, {0, 0.85f, 0.85f}, 20, true };

	//set camera and gameobject positions
	renderer.camera.transform.SetPosition({ 0, 0, -5 });

	renderer.RegisterGameObject(&objCarpFish);
	objCarpFish.transform.SetPosition({ -3, 0, 0, 1 });
	objCarpFish.transform.SetScale({3, 3, 3, 3});

	renderer.RegisterGameObject(&objTailorFish);
	objTailorFish.transform.SetPosition({ 0, 0, 3, 1 });
	objTailorFish.transform.SetScale({ 2, 2, 2, 2 });

	//renderer.RegisterGameObject(&objJellyFish);
	//objJellyFish.transform.SetPosition({ -3, 0, 3, 1 });

	//renderer.RegisterGameObject(&objMantaRay);
	//objMantaRay.transform.SetPosition({ 3, 0, 3, 1 });

	//renderer.RegisterGameObject(&objSquid);
	//objSquid.transform.SetPosition({ -3, 0, 6, 1 });

	//renderer.RegisterGameObject(&objWhale);
	//objWhale.transform.SetPosition({ 3, 0, 6, 1 });

	renderer.RegisterGameObject(&objFlower);
	objFlower.transform.SetPosition({ 0, 0, 7, 1 });

	//game variables
	float deltaTime = 0.001f; //calculate delta time, i will do properly later
	float moveSpeed = walkSpeed;

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
			//get keyboard input state
			auto kbState = DirectX::Keyboard::Get().GetState();
			kbTracker.Update(kbState);

			if (kbTracker.pressed.Escape) {
				PostQuitMessage(0);
			}

			if (kbTracker.pressed.LeftShift) {
				moveSpeed = runSpeed;
			}

			if (kbTracker.released.LeftShift) {
				moveSpeed = walkSpeed;
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
			if (BoxCollider::BoxCollision(renderer.camera.transform, objCarpFish.transform)) { FishCollection::CollectFish(objCarpFish, renderer); }
			if (BoxCollider::BoxCollision(renderer.camera.transform, objFlower.transform)) { FishCollection::CollectFish(objFlower, renderer); }

			objTailorFish.transform.Rotate({ 0, -deltaTime * 0.5f, 0 });
			objCarpFish.transform.Rotate({ 0, -deltaTime, 0 });
			objFlower.transform.Rotate({ 0, -deltaTime, 0 });

			renderer.RenderFrame();
		}
	}

	renderer.Clean();

	return 0;
}