#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

#include "Entity.hpp"
#include "Player.hpp"
#include "Text.hpp"

#define Log(x) std::cout << x << std::endl;

void ifquit(bool &level, bool &gameRunning, SDL_Event &event, SDL_Window* window)
{
	if (event.type == SDL_QUIT)
	{
		gameRunning = false;
		level = false;
		// idk, kills the game
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	if (event.key.keysym.sym == SDLK_ESCAPE)
	{
		gameRunning = false;
		level = false;
		// idk, kills the game
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}

bool collideCenter(SDL_Rect obj, SDL_Rect cobj)
{
	if (cobj.x + cobj.w/2 > obj.x - cobj.w/4 &&
		cobj.y + cobj.h/2 > obj.y - cobj.w/4 &&
		cobj.x + cobj.w/2 < obj.x + obj.w + cobj.w/4 &&
		cobj.y + cobj.h/2 < obj.y + obj.h + cobj.w/4)
	{
		return true;
	}
	else
		return false;
}

int main(int argc, char* args[])
{
	bool gameRunning = true;
	SDL_Event event;

	// screensize
	int winW = 1280;
	int winH = 720;

	// deltaTime, used to make some things not go too fast because you tried to pay to win (ran the program fast)
	Uint64 deltaNow = SDL_GetPerformanceCounter();
	Uint64 deltaLast = 0;
	double deltaTime = 0;

	// init SDL
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	SDL_Window* window = SDL_CreateWindow("Glowp", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winW, winH, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Surface *icon = IMG_Load("assets/icon.png");
	SDL_SetWindowIcon(window, icon);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_RenderSetLogicalSize(renderer, winW, winH);

	Text titleText("GLOWP", 100, {0, 213, 255, 255}, "assets/CascadiaCode.ttf", winW / 2, 30, true, renderer);

	Text resText("hee", 30, {255, 255, 255, 255}, "assets/CascadiaCode.ttf", winW / 2, winH / 2 - 40, true, renderer);

	Text instText("ENTER TO START - ESCAPE TO EXIT", 
				 20, {255, 255, 255, 255}, "assets/CascadiaCode.ttf", winW / 2, winH / 2, true, renderer);
	
	Text inst2Text("press F for fullscreen", 
				 20, {255, 255, 255, 255}, "assets/CascadiaCode.ttf", winW / 2, winH / 2 - 20, true, renderer);
	
	Text fpsText("fps", 20, {255, 255, 255, 255}, "assets/CascadiaCode.ttf", 0, 0, false, renderer);
	int fpsVar = 100;

	double gameTime = 0;
	double score = 0;
	double highscore = gameTime;
	Text highScoreText("BEST TIME: " + std::to_string(ftint(float(highscore))),
		40, { 255, 255, 255, 255 }, "assets/CascadiaCode.ttf", winW / 2, 200, true, renderer);

	// playertex
	SDL_Texture* plrT0 = IMG_LoadTexture(renderer, "assets/player0.png");
	SDL_Texture* plrT1 = IMG_LoadTexture(renderer, "assets/player1.png");
	SDL_Texture* plrT2 = IMG_LoadTexture(renderer, "assets/player2.png");
	SDL_Texture* plrT3 = IMG_LoadTexture(renderer, "assets/player3.png");
	SDL_Texture* plrT4 = IMG_LoadTexture(renderer, "assets/player4.png");
	SDL_Texture* plrT5 = IMG_LoadTexture(renderer, "assets/player5.png");
	SDL_Texture* plrT6 = IMG_LoadTexture(renderer, "assets/player6.png");
	SDL_Texture* plrT7 = IMG_LoadTexture(renderer, "assets/player7.png");
	SDL_Texture* plrT8 = IMG_LoadTexture(renderer, "assets/player8.png");

	SDL_Texture* plrTList [9] = {plrT0, plrT1, plrT2, plrT3, plrT4, plrT5, plrT6, plrT7, plrT8};
	SDL_Texture* plrT = plrTList[0];

	Player plr;

	bool plrCol = true;
	float colTime = 0;

	SDL_Texture* layer0T = IMG_LoadTexture(renderer, "assets/layer0.png");
	Entity layer0;

	SDL_Texture* foodTexture = IMG_LoadTexture(renderer, "assets/food.png");
	// vector is resizeable, which is handy if I want more, but not necessary
	std::vector<Entity> foods;

	SDL_Texture* crimpTexture = IMG_LoadTexture(renderer, "assets/crimp.png");
	std::vector<Entity> crimps;

	SDL_Texture* squT0 = IMG_LoadTexture(renderer, "assets/squid0.png");
	SDL_Texture* squT1 = IMG_LoadTexture(renderer, "assets/squid1.png");
	SDL_Texture* squT2 = IMG_LoadTexture(renderer, "assets/squid2.png");
	SDL_Texture* squT3 = IMG_LoadTexture(renderer, "assets/squid3.png");
	SDL_Texture* squT4 = IMG_LoadTexture(renderer, "assets/squid4.png");
	SDL_Texture* squT5 = IMG_LoadTexture(renderer, "assets/squid5.png");
	SDL_Texture* squT6 = IMG_LoadTexture(renderer, "assets/squid6.png");
	SDL_Texture* squT7 = IMG_LoadTexture(renderer, "assets/squid7.png");
	SDL_Texture* squTList [9] = {squT0, squT1, squT2, squT3, squT4, squT5, squT6, squT7};
	SDL_Texture* squT = squTList[0];
	std::vector<Entity> squids;


	SDL_Texture* angT0 = IMG_LoadTexture(renderer, "assets/angler/angler00.png");
	SDL_Texture* angT1 = IMG_LoadTexture(renderer, "assets/angler/angler01.png");
	SDL_Texture* angT2 = IMG_LoadTexture(renderer, "assets/angler/angler02.png");
	SDL_Texture* angT3 = IMG_LoadTexture(renderer, "assets/angler/angler03.png");
	SDL_Texture* angT4 = IMG_LoadTexture(renderer, "assets/angler/angler04.png");
	SDL_Texture* angT5 = IMG_LoadTexture(renderer, "assets/angler/angler05.png");
	SDL_Texture* angT6 = IMG_LoadTexture(renderer, "assets/angler/angler06.png");
	SDL_Texture* angT7 = IMG_LoadTexture(renderer, "assets/angler/angler07.png");
	SDL_Texture* angT8 = IMG_LoadTexture(renderer, "assets/angler/angler08.png");
	SDL_Texture* angT9 = IMG_LoadTexture(renderer, "assets/angler/angler09.png");
	SDL_Texture* angTList [10] = {angT0, angT1, angT2, angT3, angT4, angT5, angT6, angT7, angT8, angT9};

	SDL_Texture* angEat0 = IMG_LoadTexture(renderer, "assets/angler/angEat0.png");
	SDL_Texture* angEat1 = IMG_LoadTexture(renderer, "assets/angler/angEat1.png");
	SDL_Texture* angEat2 = IMG_LoadTexture(renderer, "assets/angler/angEat2.png");
	SDL_Texture* angEat3 = IMG_LoadTexture(renderer, "assets/angler/angEat3.png");
	SDL_Texture* angEatList [5] = {angEat0, angEat1, angEat2, angEat3, angT0};

	Entity angler;
	float angAngle = 0;
	float angAngleRadians;

	float speed = 0.15f;

	// keyboard state variable
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	float windowScale = 0;
	bool menu = true;
	bool setResolution = false;
	int setResIndex = 2;
	int fullscreenMode = -1;

	int r = 0;
	int g = 0;
	float b = 0;

	bool level0 = false;
	bool level0pre = false;
	float lvlTime = 0;
	float randTime = rand() % 6 + 2.5f;

	bool level1 = false;
	bool level1pre = false;

	bool level2 = false;
	bool level2pre = false;

	bool level3 = false;
	bool level3pre = false;

	bool level4 = false;
	bool level4pre = false;

	bool level5 = false;
	bool level5pre = false;
	std::vector<Player> glowps;

	bool win = false;


	// game loop
	while (gameRunning)
	{
		while (menu)
		{
			deltaLast = deltaNow;
			deltaNow = SDL_GetPerformanceCounter();
			deltaTime = (double)(deltaNow - deltaLast) * 100 / (double)SDL_GetPerformanceFrequency();
			while (SDL_PollEvent(&event))
			{
				ifquit(menu, gameRunning, event, window);
				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_f:
						fullscreenMode *= -1;
						break;
					}
				}
			}
			if (fullscreenMode == 1)
			{
				inst2Text.text = "F FOR FULLSCREEN";
				SDL_SetWindowFullscreen(window, SDL_WINDOW_SHOWN);
				SDL_RenderSetLogicalSize(renderer, winW, winH);
			}
			if (fullscreenMode == -1)
			{
				inst2Text.text = "FULLSCREEN ON";
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				SDL_RenderSetLogicalSize(renderer, winW, winH);
			}
			
			SDL_SetRenderDrawColor(renderer, r, b, g, 255);
			SDL_RenderClear(renderer);
			titleText.render();
			instText.render();
			inst2Text.update();
			inst2Text.render();
			if (fpsVar % 100 == 0)
			{
				fpsText.text = std::to_string(int(1.0f / (deltaTime / 100.0f)));
				fpsText.update();
				fpsVar = 1;
				fpsText.textRect.x = 0;
			}
			else
				fpsVar += 1;
			fpsText.render();

			if (win)
			{
				if (ftint(score) > ftint(highscore))
				{
					highscore = score - 1;
					highScoreText.text = "NEW HIGHSCORE: " + std::to_string(float(highscore) + 1.0f) + " seconds";
					if (keys[SDL_SCANCODE_RETURN])
						highscore += 1;
				}
				if (ftint(score) <= ftint(highscore))
				{
					highScoreText.text = "HIGHSCORE: " + std::to_string(highscore) + " seconds";
				}

				highScoreText.update();
				highScoreText.render();
			}

			if (keys[SDL_SCANCODE_RETURN])
			{
				SDL_SetWindowSize(window, winW, winH);
				SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
				win = false;
				menu = false;
				level5pre = true;
				gameTime = 0;
				
				plr = Player(plrT, winW / 2, winH / 2, 32, 32);
			}

			// presents everything
			SDL_RenderPresent(renderer);
		}
		if (level0pre)
		{
			layer0 = Entity(layer0T, 50, 3, 0, winH - winW * 0.06f, 1280.0f, 76.8, 0, false);
			layer0.alpha = 0;
			layer0.randAlpha = 255;
			plr.bool1 = true;
			level0pre = false;
			level0 = true;
			plr.alpha = 0;
			plr.realAlpha = 255;
		}
		while (level0)
		{
			deltaLast = deltaNow;
			deltaNow = SDL_GetPerformanceCounter();
			deltaTime = (double)(deltaNow - deltaLast) * 100 / (double)SDL_GetPerformanceFrequency();
			while (SDL_PollEvent(&event))
			{
				ifquit(level0, gameRunning, event, window);
			}

			gameTime += deltaTime / 100.0;

			// background color
			SDL_SetRenderDrawColor(renderer, r, g, b, 0);
			// clear old stuff with color
			SDL_RenderClear(renderer);

			layer0.upAlpha(3 * deltaTime);

			layer0.update();
			layer0.draw(renderer);

			plr.upAlpha(1 * deltaTime);
			plr.update(winW, winH, deltaTime, keys);
			plr.draw(plrTList, renderer);
			// get the current frame and render it with the rotation
			
			
			if (plr.x > winW + 100 * windowScale ||
				plr.x < -100 * windowScale ||
				plr.y > winH + 100 * windowScale ||
				plr.y < -100 * windowScale)
			{
				plr.speed = 0;
				layer0.downAlpha(4 * deltaTime);
				layer0.update();
				layer0.draw(renderer);

				lvlTime += deltaTime / 100.0f;
				if (lvlTime >=  5)
				{
					lvlTime = 0;
					level0 = false;
					level1pre = true;
				}
			}
			// presents everything
			SDL_RenderPresent(renderer);
		}
		if (level1pre)
		{
			plr.s_x = winW / 2;
			plr.s_y = winH / 2;
			plr.angle = 0;
			plr.resetSpeed();
			plr.alpha = 0;
			plr.bool1 = true;
			
			// creating foods in random spots but in schools
			for (int i = 0; i < 50; i++)
			{
				int foodg_x = rand() % winW;
				int foodg_y = rand() % winH;
				for (int i = 0; i < 10; i++)
				{
					Entity food = Entity(foodTexture, 1, 1, foodg_x + rand() % 100 - 50, foodg_y + rand() % 100 - 50, 4, 4, 0, true);
					food.scale = food.randAlpha / 200.0f;
					foods.push_back(food);
				}
			}
			level1pre = false;
			level1 = true;
		}
		while (level1)
		{
			deltaLast = deltaNow;
			deltaNow = SDL_GetPerformanceCounter();
			deltaTime = (double)(deltaNow - deltaLast) * 100 / (double)SDL_GetPerformanceFrequency();
			while (SDL_PollEvent(&event))
			{
				ifquit(level1, gameRunning, event, window);
			}

			plr.upAlpha(1 * deltaTime);

			gameTime += deltaTime / 100.0;

			plr.noExplore(winW, winH);
			plr.update(winW, winH, deltaTime, keys);

			// background color
			SDL_SetRenderDrawColor(renderer, r, g, b, 0);

			SDL_RenderClear(renderer);

			// foods iteration that checks if they collide with the center of player, and deletes them
			for (unsigned int i = 0; i < foods.size(); i++)
			{
				foods[i].upAlpha(3 * deltaTime);
				foods[i].update();
				foods[i].draw(renderer);
				if (collideCenter(foods[i].rect, plr.rect))
				{	
					plr.scale += 0.5f;
					plr.s_x -= 0.1f / 2.0f;
					plr.s_y -= 0.1f / 2.0f;
					plr.speedX *= 0.9f;
					plr.speedY *= 0.9f;
					foods.erase(foods.begin() + i);
				}
				if (plr.scale >= 20)
				{
					foods[i].downAlpha(4 * deltaTime);
				}
			}

			// get the current frame and render it with the rotation
			plr.draw(plrTList, renderer);

			if (plr.scale >= 20)
			{
				plr.downAlpha(1.2 * deltaTime);
				lvlTime += deltaTime / 100.0f;
				if (lvlTime >= 3)
				{
					SDL_RenderClear(renderer);
					b += deltaTime / 10.0f;
					if (b >= 20)
					{
						b = 20;
						lvlTime = 0;
						level1 = false;
						level2pre = true;
					}
					SDL_SetRenderDrawColor(renderer, r, g, b, 255);
					SDL_RenderClear(renderer);

				}
			}
			// presents everything
			SDL_RenderPresent(renderer);
		}
		if (level2pre)
		{
			b = 20;
			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
			plr.s_x = -100;
			plr.s_y =  -100;
			plr.speedX = 5;
			plr.speedY = 5;
			plr.angle = 120;
			plr.rotationSpeed = 10.0f;
			plr.scale = 0;
			plr.resetSpeed();
			plr.alpha = 0;
			plr.bool1 = true;

			// creating crimps in random spots but in schools
			for (int i = 0; i < 20; i++)
			{
				int foodg_x = rand() % winW;
				int foodg_y = rand() % winH;
				for (int i = 0; i < 10; i++)
				{
					Entity crimp = Entity(crimpTexture, 1, 1, foodg_x + rand() % 50 - 25, foodg_y + rand() % 50 - 25, 4, 4, 0.5, true);
					crimp.scale = crimp.randAlpha / 200.0f;
					crimp.schX = foodg_x;
					crimp.schY = foodg_y;
					if (200 > crimp.randAct)
					{
						crimp.speedX = rand() % 3 - 1;
						crimp.speedY = rand() % 3 - 1;
					}
					crimps.push_back(crimp);
				}
			}
			level2pre = false;
			level2 = true;
		}
		while (level2)
		{
			deltaLast = deltaNow;
			deltaNow = SDL_GetPerformanceCounter();
			deltaTime = (double)(deltaNow - deltaLast) * 100 / (double)SDL_GetPerformanceFrequency();
			SDL_RenderClear(renderer);
			while (SDL_PollEvent(&event))
			{
				ifquit(level2, gameRunning, event, window);
			}

			plr.upAlpha(1 * deltaTime);

			gameTime += deltaTime / 100.0;

			if (crimps.size() > 100)
			{
				plr.noExplore(winW, winH);
			}
			
			plr.update(winW, winH, deltaTime, keys);

			for (unsigned int i = 0; i < crimps.size(); i++)
			{
				if (crimps[i].actVar >= crimps[i].randAct && crimps.size() > 100)
				{
					if (crimps[i].schX - crimps[i].x < 0)
						crimps[i].speedX = rand() % 2 * -1;
					if (crimps[i].schX - crimps[i].x > 0)
						crimps[i].speedX = rand() % 2;
					if (crimps[i].schY - crimps[i].y < 0)
						crimps[i].speedY = rand() % 2 * -1;
					if (crimps[i].schY - crimps[i].y > 0)
						crimps[i].speedY = rand() % 2;
					crimps[i].actVar = 0;
				}
				crimps[i].actVar += 1 * deltaTime;

				crimps[i].update();
				crimps[i].moveUpdate(deltaTime, plr.damp);
				crimps[i].draw(renderer);

				if (crimps.size() <= 100)
				{
					crimps[i].downAlpha(1 * deltaTime);
					crimps[i].speedY += crimps[i].randAlpha / 10000 * deltaTime;
				}

				if (crimps.size() > 100)
				{
					crimps[i].upAlpha(1 * deltaTime);
					if (collideCenter(crimps[i].rect, plr.rect))
					{
						plr.scale += 1.0f;
						plr.s_x -= 0.5f;
						plr.s_y -= 0.5f;
						plr.speedX *= 0.9f;
						plr.speedY *= 0.9f;
						crimps.erase(crimps.begin() + i);
					}
				}
			}

			// get the current frame and render it with the rotation
			plr.draw(plrTList, renderer);

			if (crimps.size() <= 100)
			{
				plr.speedY -= 0.1 * deltaTime;
				plr.downAlpha(2 * deltaTime);
				lvlTime += deltaTime / 100.0f;
				if (lvlTime >= 5)
				{
					b += deltaTime / 10.0f;
					if (b >= 40)
					{
						b = 40;
						lvlTime = 0;
						level2 = false;
						level3pre = true;
					}
					SDL_SetRenderDrawColor(renderer, r, g, b, 255);
					SDL_RenderClear(renderer);						
				}
			}
			// presents everything
			SDL_RenderPresent(renderer);
		}
		if (level3pre)
		{
			b = 40;
			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
			plr.s_x = winW / 2 - plr.w;
			plr.s_y =  winH + plr.h; 
			plr.resetSpeed();
			plr.speedX = rand() % 5 + (-2);
			plr.speedY = -5;
			plr.angle = 120;
			plr.rotationSpeed = 100.0f;
			plr.scale = 0;
			plr.alpha = 0;
			plr.bool1 = true;
			
			level3pre = false;
			level3 = true;

			// creating squids in random spots
			for (int i = 0; i < 20; i++)
			{
				Entity squid = Entity(squT0, 14, 32, rand() % winW, rand() % (winH + 200), 56, 128, 0.15f, true);
				squid.scale = squid.randAlpha / 200.0f;
				squid.speed *= squid.scale;
				squid.currentFrame = rand() % 8;
				squids.push_back(squid);
			}
			struct less_than
			{
				inline bool operator() (const Entity& struct1, const Entity& struct2)
				{
					return (struct1.randAlpha < struct2.randAlpha);
				}
			};

			std::sort(squids.begin(), squids.end(), less_than());
		}
		while (level3)
		{
			deltaLast = deltaNow;
			deltaNow = SDL_GetPerformanceCounter();
			deltaTime = (double)(deltaNow - deltaLast) * 100 / (double)SDL_GetPerformanceFrequency();
			SDL_RenderClear(renderer);
			while (SDL_PollEvent(&event))
			{
				ifquit(level3, gameRunning, event, window);
			}

			plr.upAlpha(1 * deltaTime);

			gameTime += deltaTime / 100.0;

			colTime += deltaTime / 100.0f;

			for (unsigned int i = 0; i < squids.size(); i++)
			{
				if (collideCenter(squids[i].rect, plr.rect) && squids[i].randAlpha > 200 && colTime > 0.2)
				{
					if (plr.x - squids[i].x < 0)
						{plr.speedX = rand() % 2 * -1;}
					if (plr.x - squids[i].x > 0)
						{plr.speedX = rand() % 2;}

					plr.removeHealth(20);

					colTime = 0;
				}
				
				squids[i].currentFrame += 0.03f * deltaTime;
				squids[i].tex = squTList[ftint(squids[i].currentFrame)];
				squids[i].upAlpha(1 * deltaTime);

				if (ftint(squids[i].currentFrame) == 2)
				{
					squids[i].speedY = -3;
				}

				if (squids[i].currentFrame > 7)
				{
					squids[i].currentFrame = 0;
				}

				if (squids[i].y < 0 - squids[i].h - 100)
				{
					squids[i].y = winH + rand() % 100;
					squids[i].x = rand() % winW;
				}
				
				squids[i].update();
				squids[i].moveUpdate(deltaTime, plr.damp);
				squids[i].draw(renderer);
			}

			plr.update(winW, winH, deltaTime, keys);

			plr.draw(plrTList, renderer);
			
			if (plr.y < -100)
			{
				plr.s_y = -5000;
				for (unsigned int i = 0; i < squids.size(); i++)
				{
					squids[i].downAlpha(2 * deltaTime);			
				}

				lvlTime += deltaTime / 100.0f;
				if (lvlTime > 5)
				{
					SDL_RenderClear(renderer);
					level3 = false;
					level4pre = true;
					plr.s_x = winW / 2;
					plr.s_y = winH / 2;
				}
			}

			if (plr.death)
			{
				plr.death = false;
				level3 = false;
				level0pre = true;
				r = 0;
				g = 0;
				b = 0;
				plr.s_x = winW / 2 - plr.w/2;
				plr.s_y = winH / 2 - plr.h/2;
				plr.alpha = 0;
				plr.bool1 = true;
				plr.realAlpha = 255;
			}

			// presents everything
			SDL_RenderPresent(renderer);
		}
		if (level4pre)
		{
			b = 40;
			plr.alpha = 0;
			angler = Entity(angT0, 21, 13, winW/2, winH - 200, 160, 104, 0.4, true);
			angler.randAlpha = 180;
			level4pre = false;
			level4 = true;
			plr.bool1 = true;
			lvlTime = 0;
		}
		while (level4)
		{
			deltaLast = deltaNow;
			deltaNow = SDL_GetPerformanceCounter();
			deltaTime = (double)(deltaNow - deltaLast) * 100 / (double)SDL_GetPerformanceFrequency();
			while (SDL_PollEvent(&event))
			{
				ifquit(level4, gameRunning, event, window);
			}

			gameTime += deltaTime / 100.0;

			// background color
			SDL_SetRenderDrawColor(renderer, r, g, b, 255);

			// clear old stuff with color
			SDL_RenderClear(renderer);

			if (collideCenter(angler.rect, plr.rect))
			{
				if (lvlTime == 0)
				{
					angler.currentFrame = 0;
					lvlTime += 1;
				}
					
				if (angler.currentFrame > 3)
				{
					plr.removeHealth(100);
					lvlTime = 0;
				}
				else
				{
					angler.currentFrame += 0.04f * deltaTime;
					angler.tex = angEatList[ftint(angler.currentFrame)];
				}
			}
			else
			{
				angler.currentFrame += 0.05f * deltaTime;
				angler.tex = angTList[ftint(angler.currentFrame)];
				if (angler.currentFrame > 9)
				{
					angler.currentFrame = 0;
				}
			}
			
			angler.moveUpdate(deltaTime, plr.damp);
			angAngle -= angler.speed * 1.0f * deltaTime;
			angAngleRadians = angAngle * M_PI / 180;
			angler.speedY = sin(angAngleRadians) * 3.0f;
			angler.speedX = cos(angAngleRadians) * 3.0f;
			angler.upAlpha(3 * deltaTime);
			SDL_RenderCopyEx(renderer, angler.tex, &angler.srcRect, &angler.rect, angAngle, NULL, SDL_FLIP_NONE);

			if (plr.alpha < plr.realAlpha && plr.bool1 == true)
			{
				plr.alpha += 1 * deltaTime;
				if (plr.alpha >= plr.realAlpha){
					plr.alpha = plr.realAlpha;
					plr.bool1 = false;
				}
			}
			if (plr.alpha > 80)
				plr.noExplore(winW, winH);
			
			plr.update(winW, winH, deltaTime, keys);
			plr.draw(plrTList, renderer);

			if (plr.death)
			{
				plr.death = false;
				level4 = false;
				level0pre = true;
				r = 0;
				g = 0;
				b = 0;
				plr.s_x = winW/2 - plr.w/2;
				plr.s_y = winH/2 - plr.w/2;
				plr.alpha = 0;
				plr.bool1 = true;
				plr.realAlpha = 255;
			}

			if (plr.x > winW + 100 || plr.x < 0 - 100 || plr.y > winH + 100 || plr.y < 0 - 100) {
				plr.s_x = -5000;
				b += 1.0f * deltaTime;
				g += 0.5f * deltaTime;
				lvlTime += 5.0f * (deltaTime / 100.0);
				if (b > 160 && g >= 60 && lvlTime > 20)
				{
					lvlTime = 0;
					level4 = false;
					level5pre = true;
				}
				
			}
			SDL_RenderPresent(renderer);
		}

		while (level5pre)
		{
			b = 160;
			g = 60;
			plr.alpha = 0;
			plr.s_x = winW / 2 - plr.w / 2;
			plr.s_y = winH / 2 - plr.h / 2;
			level5pre = false;
			level5 = true;
			for (int i = 0; i < 50; ++i)
			{
				int randSize = rand() % 64 + 8;
				
				glowps.emplace_back(plr.tex, rand() % winW, rand() % winH, randSize, randSize);
			}
		}

		while (level5)
		{
			deltaLast = deltaNow;
			deltaNow = SDL_GetPerformanceCounter();
			deltaTime = (double)(deltaNow - deltaLast) * 100 / (double)SDL_GetPerformanceFrequency();
			while (SDL_PollEvent(&event))
			{
				ifquit(level4, gameRunning, event, window);
			}

			gameTime += deltaTime / 100.0;

			// background color
			SDL_SetRenderDrawColor(renderer, r, g, b, 255);

			// clear old stuff with color
			SDL_RenderClear(renderer);

			for (int i = 0; i < glowps.size(); ++i)
			{
				glowps[i].update(winW, winH, deltaTime, keys);
				if (glowps[i].x < 0 - glowps[i].w - 100) {
					glowps[i].s_x = winW + glowps[i].w + rand() % 50 + 100;
					glowps[i].s_y = rand() % winH;
				}
				if (glowps[i].y < 0 - glowps[i].h - 100) {
					glowps[i].s_y = winW + glowps[i].w + rand() % 50 + 100;
					glowps[i].s_x = rand() % winW;
				}
				if (glowps[i].x > winW + glowps[i].w + 100) {
					glowps[i].s_x = winW - glowps[i].w - (rand() % 50 + 100);
					glowps[i].s_y = rand() % winH;
				}
				if (glowps[i].y > winH + glowps[i].h + 100) {
					glowps[i].s_y = 0 - glowps[i].w - (rand() % 50 + 100);
					glowps[i].s_x = rand() % winW;
				}
				glowps[i].draw(plrTList, renderer);
			}

			if (plr.alpha < plr.realAlpha && plr.bool1 == true)
			{
				plr.alpha += 1 * deltaTime;
				if (plr.alpha >= plr.realAlpha) {
					plr.alpha = plr.realAlpha;
					plr.bool1 = false;
				}
			}

			plr.update(winW, winH, deltaTime, keys);
			plr.draw(plrTList, renderer);

			SDL_RenderPresent(renderer);
		}

		if (win)
		{
			r = 0;
			g = 0;
			b = 0;
			menu = true;
			score = gameTime;

			foods.clear();
			crimps.clear();
			squids.clear();
		}
	}
	// sdl needs a return value for some reason
	return 0;
}