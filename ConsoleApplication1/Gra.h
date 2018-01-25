#pragma once
#include <memory>
#include <string>
#include <random>
#include <SDL_ttf.h>
#include "Przeciwnik.h"
#include <list>
#include "Fizykabg.h"
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

enum StanGry {
	Ongoing,
	Lost,
	End
};
class Gra
{
private:
	std::shared_ptr<SDL_Window> window;
	std::shared_ptr<SDL_Renderer> renderer;
	int currentLane;
	int maxLanes;
	std::list<Przeciwnik> enemies;
	int lastRender, currentRender;
	Fizykabg fizykabg;
	int enemyRespawnTime;
	int lastEnemySpawn;
	int score;
	int laneWidth, playerX, playerY, playerSize;
	int enemyWidth, enemyHeight, screenWidth, screenHeight, enemyPadding, playerPaddingX;
	double speed;
	StanGry Graj;
public:
	Gra();
	~Gra();
	void start();
	std::shared_ptr<SDL_Window> initWindow(int width, int height);
	std::shared_ptr<SDL_Renderer> initRenderer(std::shared_ptr<SDL_Window> window);
	std::shared_ptr<SDL_Texture> createTexture(std::string resource_path, bool apply_alpha = false, int r = 0, int g = 255, int b = 0);
	void errthrow(const std::string &e);
	std::shared_ptr<SDL_Surface> createSurface(std::string file_name);
	std::shared_ptr<SDL_Texture> backgroundTexture;
	std::shared_ptr<SDL_Texture> playerTexture;
	std::shared_ptr<SDL_Texture> enemyTexture;
	std::shared_ptr<SDL_Texture> loseTexture;
	std::shared_ptr<SDL_Texture> initText(std::string text);
	std::shared_ptr<TTF_Font> initFont(const char *fontName, int size);
	void addEnemies();
	void removeEnemies();
	void drawEnemies();
	void animateBg();
	void drawPoints();
	bool szukajKolizji();
	bool kolizja(Przeciwnik* przeciwnik);
	void restartGame();
};

