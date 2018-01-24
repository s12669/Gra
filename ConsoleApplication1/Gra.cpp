#include "stdafx.h"
#include "Gra.h"
#include <SDL.h>
#include <memory>
#include <string>

Gra::Gra()
{
	window = init_window(800,600);
	renderer = init_renderer(window);
	background_texture = create_texture("resources/Ulica.bmp");
	start();
}

void Gra::start() {
	bool Graj = true;
	SDL_Event event;
	while (Graj) {
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) Graj = false;
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym) {
				case SDLK_RIGHT:
				case SDLK_LEFT:
					break;
				case SDLK_ESCAPE:
					Graj = false;
					break;
				}
			}
		}
		SDL_RenderClear(renderer.get());
		SDL_RenderCopy(renderer.get(), background_texture.get(), NULL, NULL);



		SDL_RenderPresent(renderer.get());
		SDL_Delay(16);
	}
}

std::shared_ptr<SDL_Window> Gra::init_window(const int width = 800, const int height = 600) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) errthrow("SDL_Init");

	SDL_Window *win = SDL_CreateWindow("Giera",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_SHOWN);
	if (win == nullptr) errthrow("SDL_CreateWindow");
	std::shared_ptr<SDL_Window> window(win, [](SDL_Window * ptr) {
		SDL_DestroyWindow(ptr);
	});
	return window;
}

std::shared_ptr<SDL_Renderer> Gra::init_renderer(std::shared_ptr<SDL_Window> window) {
	SDL_Renderer *ren = SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) errthrow("SDL_CreateRenderer");
	std::shared_ptr<SDL_Renderer> renderer(ren, [](SDL_Renderer * ptr) {
		SDL_DestroyRenderer(ptr);
	});
	return renderer;
}

void Gra::errthrow(const std::string &e) {
	std::string errstr = e + " : " + SDL_GetError();
	SDL_Quit();
	throw std::runtime_error(errstr);
};

std::shared_ptr<SDL_Texture> Gra::create_texture(std::string resource_path, bool apply_alhpa /*false*/, int r /*0*/, int g /*255*/, int b /*0*/)
{
	std::shared_ptr<SDL_Surface> surface = create_surface(resource_path);
	if (apply_alhpa)
	{
		Uint32 pixel = SDL_MapRGB(surface->format, r, g, b);
		SDL_SetColorKey(surface.get(), SDL_TRUE, pixel);
	}
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer.get(), surface.get());
	if (tex == nullptr) errthrow("SDL_CreateTexture");
	std::shared_ptr<SDL_Texture> texture(tex, [](SDL_Texture *tex) {
		SDL_DestroyTexture(tex);
	});
	return texture;
}

std::shared_ptr<SDL_Surface> Gra::create_surface(std::string file_name) {
	SDL_Surface *surf = SDL_LoadBMP(file_name.data());
	if (surf == nullptr) errthrow("SDL_CreateSurface");
	std::shared_ptr<SDL_Surface> surface(surf, [](SDL_Surface *surf) {
		SDL_FreeSurface(surf);
	});
	return surface;
}

Gra::~Gra()
{
}
