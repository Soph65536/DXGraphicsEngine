#pragma once

#include "Renderer.h"
#include "GameObject.h"

class FishCollection
{
public:
	static int fishCollected;
	static const int maxFish = 5;

	static void CollectFish(GameObject fishObject, Renderer renderer) {
		renderer.RemoveGameObject(&fishObject);
		fishCollected++;
	}
};

