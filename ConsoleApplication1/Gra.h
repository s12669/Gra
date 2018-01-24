#pragma once
#include <string>
#include <memory>
#include <SDL.h>
class Gra
{
private:
	std::shared_ptr<SDL_Window> window;
	std::shared_ptr<SDL_Renderer> renderer;
public:
	Gra();
	~Gra();
	void start();
	std::shared_ptr<SDL_Window> init_window(int width, int height);
	std::shared_ptr<SDL_Renderer> init_renderer(std::shared_ptr<SDL_Window> window);
	std::shared_ptr<SDL_Texture> create_texture(std::string resource_path, bool apply_alhpa = false, int r = 0, int g = 255, int b = 0);
	void errthrow(const std::string &e);
	std::shared_ptr<SDL_Surface> create_surface(std::string file_name);
	std::shared_ptr<SDL_Texture> background_texture;

};

