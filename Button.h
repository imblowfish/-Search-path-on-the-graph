#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#include "ScreenElement.h"

class Button: public ScreenElement{
public:
	Button():ScreenElement(){};
	void FeedbackOnMouse(SDL_Event e){
		if(!isVisible) return;
		if(e.button.button != SDL_BUTTON_LEFT) return;
		if(e.type == SDL_MOUSEBUTTONDOWN){
			SDL_Color save = color;
			color = inputColor;
			InitText();
			isSelected = true;
			color = save;
		} else{
			InitText();
			isSelected = false;
		}
	}
	void Draw() override{
		if(!isVisible) return;
		if(!font || !texture) return;
		if(!isSelected) SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		else SDL_SetRenderDrawColor(renderer, inputColor.r, inputColor.g, inputColor.b, inputColor.a);
		SDL_RenderFillRect(renderer, &bgRect);									
		SDL_RenderCopy(renderer, texture, NULL, &textRect);							
		SDL_SetRenderDrawColor(renderer, frame.r, frame.g, frame.b, frame.a);
		SDL_RenderDrawRect(renderer, &bgRect);									
	}
};
#endif