#include "stdafx.h"
#include "Game\Scene.h"

Scene* Scene::instance = nullptr;
std::atomic<int> Scene::userActionsDisabledCount = 0;

Scene::Scene() {
	userActionsDisabledCount = 0;
}

Scene* Scene::getInstance() {
	return instance;
}

void Scene::updateInstance(Scene* inst) {
	if (instance)
		delete instance;

	instance = inst;
}

void Scene::releaseInstance() {
	delete instance;
}