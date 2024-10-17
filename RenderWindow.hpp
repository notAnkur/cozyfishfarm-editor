#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include "Entity.hpp"

class RenderWindow {
public:
	RenderWindow(const char* p_title, int p_w, int p_h);
	SDL_Texture* loadTexture(const char* p_filePath);

	int getRefreshRate();

	void cleanUp();
	void clear();
	void render(Entity& p_entity);
	void display();
	// temp
	void setupImGui();
	void renderTestImGuiText();
	void drawImgui();
	void renderGrid(int width, int height, int cameraX, int cameraY, int zoomLevel, SDL_Texture* texture);
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};