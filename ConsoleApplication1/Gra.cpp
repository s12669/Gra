//#include "stdafx.h"
#include "Gra.h"

Gra::Gra()
{
	//inicjalizacja okna o podanych wymiarach i rendera
	window = initWindow(800,600);
	renderer = initRenderer(window);
	//³adowanie tekstur
	backgroundTexture = createTexture("resources/Ulica.bmp");
	playerTexture = createTexture("resources/Gracz.bmp", true);
	enemyTexture = createTexture("resources/Przeciwnik2.bmp" , true, 255, 255, 255);
	loseTexture = createTexture("resources/Przegrana2.bmp", true, 255, 255, 255);
	//fizyka t³a (pozycja, prêdkoœæ, przyspieszenie)
	fizykabg = Fizykabg({ 0.0,0.0 }, { 0.0,0.002 }, { 0.0,0.00005 });
	screenWidth = 800;
	screenHeight = 600;
	laneWidth = 160;
	maxLanes = 5;
	currentLane = 2;
	playerSize = 105;
	enemyPadding = 20;
	//pozycja gracza
	playerX = currentLane * laneWidth + 28;
	playerY = screenHeight - playerSize - 33;
	score = 0;
	enemyWidth = 120;
	enemyHeight = 60;
	enemyRespawnTime = 1500;
	lastEnemySpawn = 0;
	speed = 0.005;
	TTF_Init();
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
					if (currentLane < maxLanes - 1)
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
		//animacja t³a
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
		//wypisywanie punktów
		drawPoints();
		//rekalkulacja pozycji przy poruszaniu
		playerX = currentLane * laneWidth + 28;
		//render gracza
		SDL_Rect playerPosition = { playerX, playerY, playerSize, playerSize};
		SDL_RenderCopy(renderer.get(), playerTexture.get(), NULL, &playerPosition);
		//wyœwietlanie wyrenderowanego obrazu
		SDL_RenderPresent(renderer.get());
		SDL_Delay(10);
		lastRender = currentRender;
	}

	if (Graj == Lost) {
		SDL_Rect losePosition = { 100, 75, 600, 450 };
		SDL_RenderCopy(renderer.get(), loseTexture.get(), NULL, &losePosition);
		drawPoints();
		SDL_RenderPresent(renderer.get());

		bool keyPressed = false;
		while (!keyPressed)
		{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_KEYDOWN)
				{
					keyPressed = true;
				}
			}
		}
	}
	
}

void Gra::addEnemies() {
	//losowe generowanie liczb - rozwi¹zanie ze stacka (komenty te¿) - https://stackoverflow.com/questions/41489979/random-integers-from-a-function-always-return-the-same-number-why
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, 4); // define the range

	if (enemies.size() < 7 && lastEnemySpawn + enemyRespawnTime < currentRender) {
		int lane = distr(eng);
		//tworzenie przeciwnika, ({pozycja},{prêdkoœæ},{przyspieszenie}, tekstura)
		Przeciwnik p = Przeciwnik({ (double)laneWidth* lane + enemyPadding, (double)-enemyHeight }, {0.0, speed}, {0.0,0.00015}, enemyTexture.get());
		enemies.push_back(p);
		lastEnemySpawn = currentRender;
		speed += 0.015;
	}
}

void Gra::removeEnemies() {
	std::list<Przeciwnik>::iterator enemy = enemies.begin();
	while (enemy != enemies.end()) {
		//je¿eli przeciwnik jest poni¿ej dolnej linii ekranu to usuwa go i dodaje punkt
		if (enemy->position[1] > screenWidth + enemyWidth) {
			score++;
			enemyRespawnTime -= 25;
			if (enemyRespawnTime < 0) enemyRespawnTime = 0;
			enemy = enemies.erase(enemy);
		}
		else {
			//poruszanie siê przeciwnika je¿eli nadal znajduje siê na ekranie
			enemy->move(currentRender - lastRender);
			enemy++;
		}
	}
}
//rysowanie przeciwników
void Gra::drawEnemies() {
	SDL_Rect playerPosition = { playerX, playerY, playerSize, playerSize };
	std::list<Przeciwnik>::iterator enemy = enemies.begin();
	while (enemy != enemies.end()) {
		playerPosition = { (int)enemy->position[0], (int)enemy->position[1], enemyWidth, enemyHeight };
		SDL_RenderCopy(renderer.get(), enemy->texture, NULL, &playerPosition);
		enemy++;
	}
}
//animacja t³a
void Gra::animateBg() {
	int screen_width = 800, screen_height = 600;
	fizykabg.move(currentRender - lastRender);
	// wyzerowanie przyspieszenia dla pewnej wartoœci prêdkoœci => ustawia to max prêdkoœæ
	if (fizykabg.velocity[1] > 1.1) {
		fizykabg.acceleration[1] = 0;
	}
	if ((int)fizykabg.position[1] >= screen_height)
		fizykabg.position[1] = 0.0;
	//renderowanie dwóch te³ tak, ¿eby wygl¹da³o, ¿e siê rusza
	SDL_Rect dst_bg = { 0, (int)fizykabg.position[1] - screen_height, screen_width, screen_height };
	SDL_RenderCopy(renderer.get(), backgroundTexture.get(), NULL, &dst_bg);
	SDL_Rect dst_bg2 = { 0, (int)fizykabg.position[1], screen_width, screen_height };
	SDL_RenderCopy(renderer.get(), backgroundTexture.get(), NULL, &dst_bg2);
}

void Gra::drawPoints() {
	//(wspó³rzêdne,szerokoœæ,wysokoœæ)
	SDL_Rect dst_bg = {0, 0, 100, 20 };
	std::shared_ptr< SDL_Texture > napis = initText("Punkty:" + std::to_string(score));
	SDL_RenderCopy(this->renderer.get(), napis.get(), NULL, &dst_bg);
}

// tworzenie tekstu (wzorowane na http://lazyfoo.net/tutorials/SDL/16_true_type_fonts/index.php)
std::shared_ptr< SDL_Texture > Gra::initText(std::string text) {

	SDL_Color color = { 255, 255, 255 };
	auto font = initFont("fonts/czcionka.ttf", 20);

	SDL_Surface *surface = TTF_RenderText_Solid(font.get(), text.c_str(), color);
	SDL_Texture *t = SDL_CreateTextureFromSurface(renderer.get(), surface);

	std::shared_ptr< SDL_Texture > texture(t, [](SDL_Texture * ptr) {
		SDL_DestroyTexture(ptr);
		ptr = NULL;
	});
	SDL_FreeSurface(surface);

	return texture;
}

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

bool Gra::kolizja(Przeciwnik* przeciwnik)
{
	bool colision = false;
	// sprawdzanie czy obiekty na siebie nie nachodz¹
	if (przeciwnik->position[1] <= screenHeight - 140 + 120 && przeciwnik->position[1] + enemyHeight >= screenHeight - 140)
		if (przeciwnik->position[0] <= currentLane * laneWidth + enemyPadding + enemyWidth && przeciwnik->position[0] + enemyWidth >= currentLane * laneWidth + enemyPadding)
			colision = true;
	return colision;
}
//inicjalizacja okna gry
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
//inicjalizacja renderera
std::shared_ptr<SDL_Renderer> Gra::initRenderer(std::shared_ptr<SDL_Window> window) {
	SDL_Renderer *ren = SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) errthrow("SDL_CreateRenderer");
	std::shared_ptr<SDL_Renderer> renderer(ren, [](SDL_Renderer * ptr) {
		SDL_DestroyRenderer(ptr);
	});
	return renderer;
}
//wyrzucanie b³êdów
void Gra::errthrow(const std::string &e) {
	std::string errstr = e + " : " + SDL_GetError();
	SDL_Quit();
	throw std::runtime_error(errstr);
};
//tworzenie tekstur
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
//tworzenie t³a
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
	TTF_Quit();
}
