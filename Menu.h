#pragma once
#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <vector>
#include "SDL.h"
#include "Button.h"
#include "EditBox.h"
#include "Textfield.h"
#include "Ant.h"
#include "AStar.h"
#include "GraphBuilder.h"
#include "GraphViewer.h"
using namespace std;

class Menu{
protected:
	enum MenuMode{MAIN_MENU};
	SDL_Renderer *renderer = nullptr;
	string menuName = "";
	vector<Button> buttons;
	vector<EditBox> boxes;
	vector<TextField> textfields;
	bool active;
public:
	Menu(){}
	Menu(string name, bool runned){
		menuName = name;
		active = runned;
	}
	~Menu(){
		buttons.clear(); boxes.clear();
		textfields.clear();
	}
	void LinkRenderer(SDL_Renderer *renderer){this->renderer = renderer;}
	virtual void InitMenu() = 0;
};

#endif