#include "RenderWindow.hpp"

#include <iostream>

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
	: window(NULL), renderer(NULL)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)
{
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);

	if (texture == NULL)
		std::cout << "failed to load texture. Error: " << SDL_GetError() << std::endl;

	return texture;
}

int RenderWindow::getRefreshRate()
{
	int displayIndex = SDL_GetWindowDisplayIndex(window);

	SDL_DisplayMode mode;

	SDL_GetDisplayMode(displayIndex, 0, &mode);

	return mode.refresh_rate;
}

void RenderWindow::cleanUp()
{
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyWindow(window);
}

void RenderWindow::clear()
{
	SDL_SetRenderDrawColor(renderer, 120, 180, 255, 255);
	SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_entity)
{
	SDL_Rect src;
	src.x = p_entity.getCurrentFrame().x;
	src.y = p_entity.getCurrentFrame().y;
	src.w = p_entity.getCurrentFrame().w;
	src.h = p_entity.getCurrentFrame().h;

	SDL_Rect dst;
	dst.x = p_entity.getPos().x * 4;
	dst.y = p_entity.getPos().y * 4;
	dst.w = p_entity.getCurrentFrame().w * 4;
	dst.h = p_entity.getCurrentFrame().h * 4;

	SDL_RenderCopy(renderer, p_entity.getTex(), &src, &dst);
}

void RenderWindow::display()
{
	SDL_RenderPresent(renderer);
}

void RenderWindow::setupImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);
}

void RenderWindow::renderTestImGuiText()
{
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Test");

	ImGui::Text("Hello, world");

	ImGui::End();

	ImGui::Render();
}

void RenderWindow::drawImgui()
{
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}

void RenderWindow::renderGrid(int width, int height, float cameraX, float cameraY, int zoomLevel, SDL_Texture* texture)
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			SDL_Rect dstRect = { (i * width - cameraX) * zoomLevel, (j * height - cameraY) * zoomLevel, width * zoomLevel, height * zoomLevel };
			SDL_RenderCopy(renderer, texture, NULL, &dstRect);
		}
	}
}
