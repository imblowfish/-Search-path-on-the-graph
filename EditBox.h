#pragma once
#ifndef EDITBOX_H
#define EDITBOX_H
#include "ScreenElement.h"

class EditBox: public ScreenElement{
private:
	const int MAX_STR_SIZE = 5;
	string defaultVal = "0";
	bool nowEdit;
public:
	EditBox():ScreenElement(){nowEdit = false;};
	void SetEdit(){nowEdit = !nowEdit;}
	bool GetEdit(){return nowEdit;}
	void FeedbackOnMouse(SDL_Event e){
		StartEditing();
	}
	void StartEditing(){
		if(!isVisible) return;
		if(nowEdit) return;
		nowEdit = true;
		SDL_Color save = color;
		color = inputColor;
		value.insert(value.end(), '|');
		InitText();
		color = save;
		SDL_StartTextInput();
	}
	void KeyboardListener(char symbol, int val = -100){
		if(!isVisible) return;
		if(!nowEdit) return;
		int pos = value.find('|');
		if(pos >= 0) value.erase(pos, 1);
		else return;
		if(val == SDLK_RETURN){
			nowEdit = false;
			if(value.empty() || value[0] == '0') value = defaultVal;
			InitText();
			SDL_StopTextInput();
			return;
		}
		if(val == SDLK_LEFT){
			if(pos > 0) pos--;
		}
		else if(val == SDLK_RIGHT){
			if(pos < value.size())pos++;
		} else if(val == SDLK_BACKSPACE){
			if(pos > 0) value.erase(--pos, 1);
		}
		else if(symbol >= '0' && symbol <= '9' && value.size() < MAX_STR_SIZE)
			value.insert(value.begin()+pos++, symbol);
		value.insert(value.begin() + pos, '|');
		SDL_Color save = color;
		color = inputColor;
		InitText();
		color = save;
	}
	void Draw() override{
		if(!isVisible) return;
		if(!font || !texture) return;
		if(!nowEdit) SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		else SDL_SetRenderDrawColor(renderer, inputColor.r, inputColor.g, inputColor.b, inputColor.a);
		SDL_RenderFillRect(renderer, &bgRect);										
		SDL_RenderCopy(renderer, texture, NULL, &textRect);							
		SDL_SetRenderDrawColor(renderer, frame.r, frame.g, frame.b, frame.a);
		SDL_RenderDrawRect(renderer, &bgRect);										
	}
};

#endif