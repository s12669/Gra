#pragma once
#include <string>
#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Przeciwnik.h"
#include <list>
#include "Fizykabg.h"
class Gra
{
private:
	std::shared_ptr<SDL_Window> window;
	std::shared_ptr<SDL_Renderer> renderer;
	int current_lane=2;
	int max_lanes=5;
	std::list<Przeciwnik> enemies;
	int last_render, current_render;
	Fizykabg fizykabg;
	int enemyRespawnTime = 800;
	int lastEnemySpawn = 0;
	int score=0;
	int laneWidth = 160, playerX = 0, playerY = 0, playerSize = 120;
public:
	Gra();
	~Gra();
	void start();
	std::shared_ptr<SDL_Window> init_window(int width, int height);
	std::shared_ptr<SDL_Renderer> init_renderer(std::shared_ptr<SDL_Window> window);
	std::shared_ptr<SDL_Texture> create_texture(std::string resource_path, bool apply_alpha = false, int r = 0, int g = 255, int b = 0);
	void errthrow(const std::string &e);
	std::shared_ptr<SDL_Surface> create_surface(std::string file_name);
	std::shared_ptr<SDL_Texture> background_texture;
	std::shared_ptr<SDL_Texture> player_texture;
	std::shared_ptr<SDL_Texture> enemy_texture;
	std::shared_ptr<SDL_Texture> initText(std::string text);
	std::shared_ptr<TTF_Font> initFont(std::string fontName);
	void addEnemies();
	void removeEnemies();
	void drawEnemies();
	void animateBg();
	void drawPoints();
	bool szukajKolizji();
	bool kolizja(Przeciwnik* obj1);
};

