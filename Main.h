#pragma once
#ifndef MAIN_H
#define MAIN_H

#include "MainMenu.h"

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;
int keys[1024];
SDL_Window *window = NULL;
SDL_Surface *screenSurface = NULL;
SDL_Renderer *renderer = NULL;
bool initSDL(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		cout << "Error SDLInit:" << SDL_GetError() << endl;
		return false;
	}
	window = SDL_CreateWindow("Graph", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(!window){
		cout << "Window SDL Error:" << SDL_GetError() << endl;
		SDL_Quit();
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(!renderer){
		SDL_DestroyWindow(window);
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return false;
	}
	return true;
}
void outSDL(){
	SDL_FreeSurface(screenSurface);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	cout << "Quit success" << endl;
}

#endif