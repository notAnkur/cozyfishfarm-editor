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

	// Todo: test grid
	const int GRID_WIDTH = 10;
	const int GRID_HEIGHT = 10;
	float cameraX = 0;
	float cameraY = 0;
	float ZOOM_LEVEL = 1;
	float cameraPanSpeed = 0.1f;
	int currentActiveMW = 1; // 1(zoom), 2(cameraX), 3(cameraY)
	int grid[GRID_WIDTH][GRID_HEIGHT];

	// TODO: pan mouse
	bool isRightButtonDown = false;
	int xMousePosInitial, yMousePosInitial = 0;
	int xMousePosCurr, yMousePosCurr = 0;



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
				else if (event.type == SDL_MOUSEWHEEL) {
					if (event.wheel.y > 0) {
						if (currentActiveMW == 1) {
							ZOOM_LEVEL += 1;
						}
						else if (currentActiveMW == 2) {
							//cameraX += 1;
						}
						else if (currentActiveMW == 3) {
							//cameraY += 1;
						}
					}
					else {
						if (currentActiveMW == 1) {
							ZOOM_LEVEL -= 1;
							if (ZOOM_LEVEL <= 0)
								ZOOM_LEVEL = 1;
						}
						else if (currentActiveMW == 2) {
							//cameraX -= 1;
						}
						else if (currentActiveMW == 3) {
							//cameraY -= 1;
						}
					}
				}
				else if (event.type == SDL_KEYDOWN) {
					if (event.key.keysym.scancode == SDL_SCANCODE_1) {
						std::cout << "One" << std::endl;
						currentActiveMW = 1;
					}
					else if (event.key.keysym.scancode == SDL_SCANCODE_2) {
						currentActiveMW = 2;
					}
					else if (event.key.keysym.scancode == SDL_SCANCODE_3) {
						currentActiveMW = 3;
					}
				}
				else if (event.type == SDL_MOUSEBUTTONDOWN) {
					if (event.button.button == SDL_BUTTON_RIGHT) {
						std::cout << "right down" << std::endl;
						if (!isRightButtonDown) {
							SDL_GetMouseState(&xMousePosInitial, &yMousePosInitial);
							isRightButtonDown = true;
						}
					}
				}
				else if (event.type == SDL_MOUSEBUTTONUP) {
					if (event.button.button == SDL_BUTTON_RIGHT) {
						std::cout << "right up" << std::endl;
						isRightButtonDown = false;
					}
				}
				ImGui_ImplSDL2_ProcessEvent(&event);
			}

			accumulator -= timeStep;
		}

		// TODO: right mouse hold drag
		if (isRightButtonDown) {
			SDL_GetMouseState(&xMousePosCurr, &yMousePosCurr);
			if (std::abs(xMousePosCurr - xMousePosInitial) >= 0.5f) {
				cameraX += (xMousePosCurr - xMousePosInitial) * cameraPanSpeed;
			}

			if (std::abs(yMousePosCurr - yMousePosInitial) >= 0.5f) {
				cameraY += (yMousePosCurr - yMousePosInitial) * cameraPanSpeed;
			}

			std::cout << "Current pos delta: " << (xMousePosCurr - xMousePosInitial) << " :: " << (yMousePosCurr - yMousePosInitial) << std::endl;
			// set the current position as initial position for next frame
			xMousePosInitial = xMousePosCurr;
			yMousePosInitial = yMousePosCurr;

			std::cout << "Cameraxy pos: " << cameraX << " :: " << cameraY << std::endl;
		}

		const float alpha = accumulator / timeStep;

		// TODO: imgui test
		window.renderTestImGuiText(&ZOOM_LEVEL, &cameraPanSpeed, &cameraX, &cameraY);

		window.clear();

		// TODO: grid render
		window.renderGrid(GRID_WIDTH, GRID_HEIGHT, cameraX, cameraY, ZOOM_LEVEL, grassTexture);

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