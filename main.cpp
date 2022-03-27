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

	// text class xD

	Text titleText("GLOWP", 50, {0, 213, 255, 255}, "assets/CascadiaCode.ttf", winW / 2, 30, true, renderer);

	Text resText("hee", 30, {255, 255, 255, 255}, "assets/CascadiaCode.ttf", winW / 2, winH / 2 - 40, true, renderer);

	Text instText("ENTER TO START - ESCAPE TO EXIT", 
				 20, {255, 255, 255, 255}, "assets/CascadiaCode.ttf", winW / 2, winH / 2, true, renderer);
	
	Text inst2Text("press F for fullscreen", 
				 20, {255, 255, 255, 255}, "assets/CascadiaCode.ttf", winW / 2, winH / 2 - 20, true, renderer);
	
	Text fpsText("fps", 20, {255, 255, 255, 255}, "assets/CascadiaCode.ttf", 0, 0, false, renderer);
	int fpsVar = 100;

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
	int fullscreenMode = 1;

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

			if (keys[SDL_SCANCODE_RETURN])
			{
				SDL_SetWindowSize(window, winW, winH);
				SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

				menu = false;
				level3pre = true;
				plr = Player(plrT, winW / 2, winH / 2, 32, 32);
			}

			// presents everything
			SDL_RenderPresent(renderer);
		}
		if (level0pre)
		{
			speed *= windowScale;

			layer0 = Entity(layer0T, 50, 3, 0, winH - winW * 0.06f, 1280.0f, 76.8, 0, false);
			layer0.alpha = 255;

			level0pre = false;
			level0 = true;
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

			// background color
			SDL_SetRenderDrawColor(renderer, r, g, b, 0);

			// clear old stuff with color
			SDL_RenderClear(renderer);
			layer0.update();
			layer0.draw(renderer);

			plr.update(winW, winH, deltaTime, keys);
			plr.draw(plrTList, renderer);
			// get the current frame and render it with the rotation
			
			
			if (plr.x > winW + 100 * windowScale ||
				plr.x < -100 * windowScale ||
				plr.y > winH + 100 * windowScale ||
				plr.y < -100 * windowScale)
			{
				plr.speed = 0;
				layer0.downAlpha(3);
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
			plr.s_posX = winW / 2;
			plr.s_posY = winH / 2;
			plr.angle = 0;
			plr.resetSpeed();
			
			// creating foods in random spots but in schools
			for (int i = 0; i < 50; i++)
			{
				int foodg_x = rand() % winW;
				int foodg_y = rand() % winH;
				for (int i = 0; i < 10; i++)
				{
					Entity food = Entity(foodTexture, 1, 1, foodg_x + rand() % 200 - 100, foodg_y + rand() % 200 - 100, 4, 4, 0, true);
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

			plr.noExplore(winW, winH);
			plr.update(winW, winH, deltaTime, keys);

			// background color
			SDL_SetRenderDrawColor(renderer, r, g, b, 0);

			SDL_RenderClear(renderer);

			// foods iteration that checks if they collide with the center of player, and deletes them
			for (unsigned int i = 0; i < foods.size(); i++)
			{
				foods[i].upAlpha(3);
				foods[i].update();
				foods[i].draw(renderer);
				if (collideCenter(foods[i].rect, plr.rect))
				{	
					plr.scale += foods[i].scale;
					plr.s_posX -= 0.5f;
					plr.s_posY -= 0.5f;
					plr.speedX *= 0.9f;
					plr.speedY *= 0.9f;
					plr.speed *= 1.01f;
					foods.erase(foods.begin() + i);
				}
			}

			// get the current frame and render it with the rotation
			plr.draw(plrTList, renderer);

			if (plr.scale >= 40)
			{
				SDL_RenderClear(renderer);
				lvlTime += deltaTime / 100.0f;
				if (lvlTime >= 3)
				{
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
			plr.s_posX = -100;
			plr.s_posY =  -100;
			plr.speedX = 5 * windowScale;
			plr.speedY = 5 * windowScale;
			plr.angle = 120;
			plr.rotationSpeed = 10.0f;
			plr.scale = 0;
			plr.resetSpeed();

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
						crimp.speedX = (rand() % 3 - 1);
						crimp.speedY = (rand() % 3 - 1);
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

			if (crimps.size() > 100)
			{
				plr.noExplore(winW, winH);
			}
			
			plr.update(winW, winH, deltaTime, keys);

			for (unsigned int i = 0; i < crimps.size(); i++)
			{
				if (crimps[i].actVar >= crimps[i].randAct)
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
					crimps[i].downAlpha(3);

				if (crimps.size() > 100)
				{
					crimps[i].upAlpha(3);
					if (collideCenter(crimps[i].rect, plr.rect))
					{
						plr.scale += crimps[i].scale;
						plr.s_posX -= 0.5f;
						plr.s_posY -= 0.5f;
						plr.speedX *= 0.9f;
						plr.speedY *= 0.9f;
						plr.speed *= 1.001f;
						crimps.erase(crimps.begin() + i);
					}
				}
			}

			// get the current frame and render it with the rotation
			plr.draw(plrTList, renderer);

			if (crimps.size() <= 100)
			{
				plr.speedY += -0.15f;
				if (plr.x > winW + 100 * windowScale ||
				plr.x < -100 * windowScale ||
				plr.y > winH + 100 * windowScale ||
				plr.y < -100 * windowScale)
				{
					SDL_RenderClear(renderer);
					lvlTime += deltaTime / 100.0f;
					if (lvlTime >= 3)
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
			}
			// presents everything
			SDL_RenderPresent(renderer);
		}
		if (level3pre)
		{
			b = 40;
			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
			plr.s_posX = winW / 2 - plr.w;
			plr.s_posY =  winH / 2 - plr.h;
			plr.speedX = 5 * windowScale;
			plr.speedY = 5 * windowScale;
			plr.angle = 120;
			plr.rotationSpeed = 100.0f;
			plr.scale = 0;
			plr.resetSpeed();
			level3pre = false;
			level3 = true;

			// creating squids in random spots
			for (int i = 0; i < 20; i++)
			{
				Entity squid = Entity(squT0, 14, 32, rand() % winW, rand() % winH, 56, 128, 0.15f, true);
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

			colTime += deltaTime / 100.0f;

			for (unsigned int i = 0; i < squids.size(); i++)
			{
				if (collideCenter(squids[i].rect, plr.rect) && squids[i].randAlpha > 100 && colTime > 0.5)
				{
					if (plr.x - squids[i].x < 0)
						{plr.speedX = rand() % 2 * -1;}
					if (plr.x - squids[i].x > 0)
						{plr.speedX = rand() % 2;}

					plr.alpha -= 20;

					colTime = 0;
				}
				
				squids[i].currentFrame += 0.03f * deltaTime;
				squids[i].tex = squTList[ftint(squids[i].currentFrame)];
				if (squids[i].alpha < squids[i].randAlpha)
				{
					squids[i].upAlpha(3);
					for (int o = 0; o < 9; ++o)
					{
						SDL_SetTextureAlphaMod(squTList[o], squids[i].alpha);
					}
				}

				if (ftint(squids[i].currentFrame) == 2)
				{
					squids[i].speedY = -3;
				}

				if (ftint(squids[i].currentFrame) == 6)
				{
					squids[i].speedY = 0.3;
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
				plr.speedY = 0;
				for (unsigned int i = 0; i < squids.size(); i++)
				{
					squids[i].downAlpha(6);
				}
				lvlTime += deltaTime / 100.0f;
				if (lvlTime > 5)
				{
					SDL_RenderClear(renderer);
					level3 = false;
					level4pre = true;
				}
			}

			// presents everything
			SDL_RenderPresent(renderer);
		}
		if (level4pre)
		{
			b = 40;
			angler = Entity(angT0, 21, 13, winW/2, winH - 200, 160, 104, 0.4, true);
			level4pre = false;
			level4 = true;
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
					//angler.tex = angTList[0];
					plr.removeHealth(100);
					plr.s_posX += 100;
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
			angAngle -= angler.speed * 0.6f * deltaTime * windowScale;
			angAngleRadians = angAngle * M_PI / 180;
			angler.speedY = sin(angAngleRadians) * deltaTime;
			angler.speedX = cos(angAngleRadians) * deltaTime;
			SDL_RenderCopyEx(renderer, angler.tex, &angler.srcRect, &angler.rect, angAngle, NULL, SDL_FLIP_NONE);

			plr.noExplore(winW, winH);
			plr.update(winW, winH, deltaTime, keys);
			plr.draw(plrTList, renderer);

			if (plr.alpha <= 10)
			{
				level4 = false;
				level0pre = true;
				r = 0;
				g = 0;
				b = 0;
				plr.s_posX = winW/2;
				plr.s_posY = winH/2;
			}
			SDL_RenderPresent(renderer);
		}
	}
	// sdl needs a return value for some reason
	return 0;
}