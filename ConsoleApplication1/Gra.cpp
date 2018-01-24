#include "stdafx.h"
#include "Gra.h"

Gra::Gra()
{
	window = initWindow(800,600);
	renderer = initRenderer(window);
	backgroundTexture = createTexture("resources/Ulica.bmp");
	playerTexture = createTexture("resources/Gracz.bmp");
	enemyTexture = createTexture("resources/Przeciwnik.bmp" , true, 255, 255, 0);
	loseTexture = createTexture("resources/Przegrana.bmp", true, 255, 255, 255);
	fizykabg = Fizykabg({ 0.0,0.0 }, { 0.0,0.0 }, { 0.0,0.0 });
	fizykabg.velocity[1] = 0.007;
	fizykabg.acceleration[1] = 0.0004;
	screenWidth = 800;
	screenHeight = 600;
	playerX = currentLane * 160 + 20;
	playerY = 600 - 120 - 20;
	enemyWidth = 120;
	enemyHeight = 200;
	start();
}

void Gra::start() {
	StanGry Graj = StanGry::Ongoing;
	SDL_Event event;
	lastRender = SDL_GetTicks();
	currentRender = SDL_GetTicks();
	while (Graj == Ongoing) {
		currentRender = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) Graj = Lost;
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym) {
				case SDLK_RIGHT:
					if (currentLane < 4)
						currentLane++;
					break;
				case SDLK_LEFT:
					if (currentLane > 0)
						currentLane--;
					break;
				case SDLK_ESCAPE:
					Graj = End;
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
		if (szukajKolizji()) {
			Graj = Lost;
		}
		//rysowanie przeciwnikow
		drawEnemies();

		drawPoints();
		//rekalkulacja pozycji przy poruszaniu
		playerX = currentLane * 160 + 20;
		SDL_Rect playerPosition = { playerX, playerY, playerSize, playerSize};
		SDL_RenderCopy(renderer.get(), playerTexture.get(), NULL, &playerPosition);

		SDL_RenderPresent(renderer.get());
		SDL_Delay(1);
		lastRender = currentRender;
	}

	if (Graj == Lost) {
		SDL_Rect loosePosition = { 100, 75, 600, 450 };
		SDL_RenderCopy(renderer.get(), loseTexture.get(), NULL, &loosePosition);

		SDL_RenderPresent(renderer.get());

		bool beMean = true;
		while (beMean)
		{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_KEYDOWN)
				{
					beMean = false;
				}
			}
		}
	}
	
}

void Gra::addEnemies() {
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, 4); // define the range

	if (enemies.size() < 7 && lastEnemySpawn + enemyRespawnTime < currentRender) {
		int lane = distr(eng);
		Przeciwnik p = Przeciwnik({ (double)160* lane + 20, -260.0 }, {0.0, 0.002}, {0.0,0.0003}, enemyTexture.get());
		enemies.push_back(p);
		lastEnemySpawn = currentRender;
	}
}

void Gra::removeEnemies() {
	std::list<Przeciwnik>::iterator enemy = enemies.begin();
	while (enemy != enemies.end()) {
		if (enemy->position[1] > 600 + 140) {
			score++;
			enemy = enemies.erase(enemy);
		}
		else {
			enemy->move(currentRender - lastRender);
			enemy++;
		}
	}
}

void Gra::drawEnemies() {
	SDL_Rect player_position = { playerX, playerY, playerSize, playerSize };
	std::list<Przeciwnik>::iterator enemy = enemies.begin();
	while (enemy != enemies.end()) {
		player_position = { (int)enemy->position[0], (int)enemy->position[1], enemyWidth, enemyHeight };
		SDL_RenderCopy(renderer.get(), enemy->texture, NULL, &player_position);
		enemy++;
	}
}

void Gra::animateBg() {
	int screen_width = 800, screen_height = 600;
	fizykabg.move(currentRender - lastRender);
	if (fizykabg.velocity[1] > 1.1) {
		fizykabg.acceleration[1] = 0;
	}
	SDL_RenderCopy(renderer.get(), backgroundTexture.get(), NULL, NULL);
	if ((int)fizykabg.position[1] >= screen_height)
		fizykabg.position[1] = 0.0;
	SDL_Rect dst_bg = { 0, (int)fizykabg.position[1] - screen_height, screen_width, screen_height };
	SDL_RenderCopy(renderer.get(), backgroundTexture.get(), NULL, &dst_bg);
	SDL_Rect dst_bg2 = { 0, (int)fizykabg.position[1], screen_width, screen_height };
	SDL_RenderCopy(renderer.get(), backgroundTexture.get(), NULL, &dst_bg2);
}

void Gra::drawPoints() {
	SDL_Rect dst_bg = { 0, 0, 100,  20};
	std::shared_ptr< SDL_Texture > napis = initText("Punkty:" + std::to_string(score));
	SDL_RenderCopy(this->renderer.get(), napis.get(), NULL, &dst_bg);
}

// tworzenie tekstu (wzorowane na http://lazyfoo.net/tutorials/SDL/16_true_type_fonts/index.php)
std::shared_ptr< SDL_Texture > Gra::initText(std::string text) {

	SDL_Color color = { 255, 0, 161 };
	auto font = initFont("fonts/uni.ttf", 20);

	SDL_Surface *surface = TTF_RenderText_Solid(font.get(), text.c_str(), color);
	SDL_Texture *t = SDL_CreateTextureFromSurface(renderer.get(), surface);

	std::shared_ptr< SDL_Texture > texture(t, [](SDL_Texture * ptr) {
		SDL_DestroyTexture(ptr);
		ptr = NULL;
	});
	SDL_FreeSurface(surface);

	return texture;
}

//// tworzenie fontu
//std::shared_ptr< TTF_Font > Gra::initFont(std::string fontName) {
//
//	TTF_Font *f = TTF_OpenFont(fontName.c_str(), 20);
//
//	std::shared_ptr< TTF_Font > font(f, [](TTF_Font * ptr) {
//		TTF_CloseFont(ptr);
//		ptr = NULL;
//	});
//
//	return font;
//}

// tworzenie fontu
std::shared_ptr< TTF_Font > Gra::initFont(const char *fontName, int size) {

	TTF_Font *f = TTF_OpenFont(fontName, size);

	std::shared_ptr< TTF_Font > font(f, [](TTF_Font * ptr) {
		TTF_CloseFont(ptr);
		ptr = NULL;
	});

	return font;
}

bool Gra::szukajKolizji() {
	bool result = false;
	std::list<Przeciwnik>::iterator enemy = enemies.begin();
	while (enemy != enemies.end()) {
		if (kolizja(&(*enemy))) {
			result = true;
		}
		enemy++;
	}
	return result;
}

bool Gra::kolizja(Przeciwnik* obj1)
{	//SDL_Rect player_position = { current_lane* 160 + 20, 600- 140, 120, 120};
	bool colision = false;
	if (obj1->position[1] <= 600 - 140 + 120 && obj1->position[1] + 200 >= 600 - 140)
		if (obj1->position[0] <= currentLane * 160 + 20 + 120 && obj1->position[0] + 120 >= currentLane * 160 + 20)
			colision = true;
	return colision;
}

std::shared_ptr<SDL_Window> Gra::initWindow(const int width = 800, const int height = 600) {
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

std::shared_ptr<SDL_Renderer> Gra::initRenderer(std::shared_ptr<SDL_Window> window) {
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

std::shared_ptr<SDL_Texture> Gra::createTexture(std::string resource_path, bool apply_alpha /*false*/, int r /*0*/, int g /*255*/, int b /*0*/)
{
	std::shared_ptr<SDL_Surface> surface = createSurface(resource_path);
	if (apply_alpha)
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

std::shared_ptr<SDL_Surface> Gra::createSurface(std::string file_name) {
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
