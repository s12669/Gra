#include "stdafx.h"
#include "Gra.h"
#include <SDL.h>
#include <memory>
#include <string>
#include <random>
Gra::Gra()
{
	window = init_window(800,600);
	renderer = init_renderer(window);
	background_texture = create_texture("resources/Ulica.bmp");
	player_texture = create_texture("resources/Gracz.bmp");
	enemy_texture = create_texture("resources/Przeciwnik.bmp");
	fizykabg = Fizykabg({ 0.0,0.0 }, { 0.0,0.0 }, { 0.0,0.0 });
	fizykabg.velocity[1] = 0.007;
	fizykabg.acceleration[1] = 0.0004;
	start();
}

void Gra::start() {
	bool Graj = true;
	SDL_Event event;
	last_render = SDL_GetTicks();
	current_render = SDL_GetTicks();
	while (Graj) {
		current_render = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) Graj = false;
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym) {
				case SDLK_RIGHT:
					if (current_lane < 4)
						current_lane++;
					break;
				case SDLK_LEFT:
					if (current_lane > 0)
						current_lane--;
					break;
				case SDLK_ESCAPE:
					Graj = false;
					break;
				}
			}
		}
		SDL_RenderClear(renderer.get());
		animateBg();
		// func do robienie przeciwnikow
		addEnemies();
		// func do usuwanie przeciwnikow
		removeEnemies();
		//wykrywanie kolizji

		//rysowanie przeciwnikow
		drawEnemies();

		SDL_Rect player_position = { current_lane* 160 + 20, 600- 140, 120, 120};
		SDL_RenderCopy(renderer.get(), player_texture.get(), NULL, &player_position);

		SDL_RenderPresent(renderer.get());
		SDL_Delay(1);
		last_render = current_render;
	}
}

void Gra::addEnemies() {
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, 4); // define the range

	if (enemies.empty()) {
		int lane = distr(eng);
		Przeciwnik p = Przeciwnik({ (double)160* lane + 20, -220.0 }, {0.0, 0.002}, {0.0,0.0003}, enemy_texture.get());
		enemies.push_back(p);
	}
}

void Gra::removeEnemies() {
	std::list<Przeciwnik>::iterator enemy = enemies.begin();
	while (enemy != enemies.end()) {
		if (enemy->position[1] > 600 + 140) {
			enemy = enemies.erase(enemy);
		}
		else {
			enemy->move(current_render - last_render);
			enemy++;
		}
	}
}

void Gra::drawEnemies() {
	SDL_Rect player_position = { current_lane * 160 + 20, 600 - 140, 120, 120 };
	std::list<Przeciwnik>::iterator enemy = enemies.begin();
	while (enemy != enemies.end()) {
		player_position = { (int)enemy->position[0], (int)enemy->position[1], 120, 200 };
		SDL_RenderCopy(renderer.get(), enemy->texture, NULL, &player_position);
		enemy++;
	}
}

void Gra::animateBg() {
	int screen_width = 800, screen_height = 600;
	fizykabg.move(current_render - last_render);
	if (fizykabg.velocity[1] > 1.1) {
		fizykabg.acceleration[1] = 0;
	}
	SDL_RenderCopy(renderer.get(), background_texture.get(), NULL, NULL);
	if ((int)fizykabg.position[1] >= screen_height)
		fizykabg.position[1] = 0.0;
	SDL_Rect dst_bg = { 0, (int)fizykabg.position[1] - screen_height, screen_width, screen_height };
	SDL_RenderCopy(renderer.get(), background_texture.get(), NULL, &dst_bg);
	SDL_Rect dst_bg2 = { 0, (int)fizykabg.position[1], screen_width, screen_height };
	SDL_RenderCopy(renderer.get(), background_texture.get(), NULL, &dst_bg2);
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
