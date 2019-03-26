#pragma once
#ifndef SCREEN_ELEMENT_H
#define SCREEN_ELEMENT_H
#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "FontBuilder.h"
#include "Colors.h"
using namespace std;

class ScreenElement{
protected:
	int maxPixelDiff = 5;		//maximum difference in the pixels for determining the mouse click
	int fontSize = 0;
	FontBuilder fBuilder;		
	SDL_Renderer *renderer;		
	SDL_Texture *texture;		
	string value;				
	int xPos, yPos;				
	SDL_Color color,			
			  frame,			
			  inputColor,		
			  fontColor;		
	SDL_Rect bgRect, textRect;	
	TTF_Font *font;				
	bool isSelected, isVisible;	
	void InitText(){
		if(texture){
			SDL_DestroyTexture(texture);
			texture = NULL;
		}
		SDL_Surface *surface = NULL;
		surface = TTF_RenderText_Shaded(font, value.c_str(), fontColor, color);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		int fontWidth = 0, fontHeight = 0;
		SDL_QueryTexture(texture, NULL, NULL, &fontWidth, &fontHeight);
		textRect = {xPos, yPos, fontWidth, fontHeight};
		textRect.x -= int(textRect.w / 2);
		textRect.y -= int(textRect.h / 2);
		if(!texture) return;
		bgRect.x = textRect.x - 20;
		bgRect.y = textRect.y - 5;
		bgRect.w = textRect.w + 40;
		bgRect.h = textRect.h + 10;
		SDL_FreeSurface(surface);
	}
public:
	ScreenElement(){
		value = "";
		xPos = yPos = 0;
		color = frame = inputColor = fontColor = {BG_COLOR};
		font = nullptr;
		texture = nullptr;
		isVisible = false;
		isSelected = false;
	}
	~ScreenElement(){
		Clear();
	}
	void Clear(){
		if(font) fBuilder.CloseFont(font);
		font = nullptr;
		SDL_DestroyTexture(texture);
	}
	void SetupMe(string val, int x, int y, unsigned int fontSize, SDL_Color color = {BG_COLOR}, SDL_Color frameColor = {BG_COLOR},
			SDL_Color inputColor = {BG_COLOR}, SDL_Color fontColor = {FONT_COLOR}){
		this->Clear();
		value = val;
		xPos = x; yPos = y;
		this->color = color; this->frame = frameColor;
		this->inputColor = inputColor; this->fontColor = fontColor;
		if(font) fBuilder.CloseFont(font);
		font = fBuilder.CreateFont(font, fontSize);
		isSelected = false;
		isVisible = true;
		this->fontSize = fontSize;
		InitText();
	}
	void LinkRenderer(SDL_Renderer *renderer){this->renderer = renderer;}
	void SetVisibility(bool val){isVisible = val;}
	bool GetVisibility(){return isVisible;}
	bool MouseEventListener(SDL_Event e, int x, int y){
		if(!isVisible) return false;
		int left = bgRect.x,
			right = bgRect.x + bgRect.w,
			top = bgRect.y,
			bottom = bgRect.y + bgRect.h;
		if(x < left || x > right || y < top || y > bottom) return false;
		FeedbackOnMouse(e);
		return true;
	}
	int GetIntValue(){
		return atoi(value.c_str());
	}
	void SetNewVal(string val){
		value = val;
		InitText();
	}
	void SetNewValAndPos(string val, int x, int y){
		this->value = val;
		this->xPos = x;
		this->yPos = y;
	}
	string GetStrValue(){
		return value;
	}
	virtual void FeedbackOnMouse(SDL_Event e) = 0;
	virtual void Draw(){
		if(!isVisible) return;
		if(!font || !texture) return;
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, &bgRect);										
		SDL_RenderCopy(renderer, texture, NULL, &textRect);							
		SDL_SetRenderDrawColor(renderer, frame.r, frame.g, frame.b, frame.a);
		SDL_RenderDrawRect(renderer, &bgRect);										
	}
};

#endif