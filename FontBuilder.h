#pragma once
#ifndef FONTBUILDER_H
#define FONTBUILDER_H
#include <iostream>
#include "SDL_ttf.h"
using namespace std;

class FontBuilder{
public:
	~FontBuilder(){}
	FontBuilder(){
		if(!TTF_WasInit()){
			if(!TTFInit()) getchar();
		}
	}
	bool TTFInit(){
		if(TTF_Init() < 0) return false;
		return true;
	}
	void TTFQuit(){
		TTF_Quit();
	}
	void CloseFont(TTF_Font *font){
		if(!font) return;
		TTF_CloseFont(font);
		font = nullptr;
	}
	TTF_Font* CreateFont(TTF_Font* font, unsigned int size, string path = "fonts\\OpenSans-Regular.ttf"){
		if(font) CloseFont(font);
		font = TTF_OpenFont(path.c_str(), size);
		if(!font) return false;
		return font;
	}
};

#endif