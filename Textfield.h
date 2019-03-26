#pragma once
#ifndef TEXTFIELD_H
#define TEXTFIELD_H
#include "ScreenElement.h"
//класс простого текстового поля

class TextField: public ScreenElement{
public:
	TextField():ScreenElement(){};
	void FeedbackOnMouse(SDL_Event e){return;}
};

#endif