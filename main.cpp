#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Utils.hpp"

int main(int argc, char* args[]) {

	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << "SDL_INIT has failed! SDL_ERROR: " << SDL_GetError() << std::endl;
	}

	if (!(IMG_Init(IMG_INIT_PNG))) {
		std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
	}

	RenderWindow window("CozyFishFarm Editor v1.0", 1280, 720);

	std::cout << window.getRefreshRate() << std::endl;

	SDL_Texture* grassTexture = window.loadTexture("assets/images/test.png");

	//Entity entities[3] = ;

	std::vector<Entity> entities = {
		Entity(Vector2f(0, 0), grassTexture),
		Entity(Vector2f(30, 0), grassTexture),
		Entity(Vector2f(30, 30), grassTexture),
		Entity(Vector2f(60, 30), grassTexture)
	};



	// Init imgui
	window.setupImGui();



	bool gameRunning = true;

	const float timeStep = 0.01f;
	float accumulator = 0.0f;
	float currentTime = utils::hireTimeInSeconds();

	SDL_Event event;

	while (gameRunning) {
		int startTicks = SDL_GetTicks();

		float newTime = utils::hireTimeInSeconds();
		float frameTime = newTime - currentTime;

		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= timeStep) {
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT)
					gameRunning = false;
				ImGui_ImplSDL2_ProcessEvent(&event);
			}

			accumulator -= timeStep;
		}

		const float alpha = accumulator / timeStep;

		// TODO: imgui test
		window.renderTestImGuiText();

		window.clear();

		// TODO: imgui test
		window.drawImgui();

		/*for (Entity& e : entities) {
			window.render(e);
		}*/

		window.display();

		int frameTicks = SDL_GetTicks() - startTicks;
		if (frameTicks < 1000 / window.getRefreshRate())
			SDL_Delay(1000 / window.getRefreshRate() - frameTicks);
	}

	window.cleanUp();
	SDL_Quit();

	return 0;
}