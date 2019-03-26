#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H
#include "Menu.h"


//I am terribly writing the menu

class MainMenu: public Menu{
private:
	FontBuilder fBuilder;
	EditBox weightInput;
	const int SCREEN_WIDTH=900, SCREEN_HEIGHT=600;
	unsigned int nowMenuMode = MAIN_MENU;
	string nowAlgorithm = "NaN";
	int minVertices = 11, maxVertices = 15;
	const int stepSize = 2;
	vector<int> *path;
	GraphViewer gViewer;
	GraphBuilder gBuilder;
	int keys[1024];
	string pathToDir = ""; //if there are any characters other than English, 
						   //then we indicate the absolute path here, 
						   //otherwise uncomment the lines in main
	string pathToGraph = "graphs\\graph.txt";
	string pathToProtocol = "graphs\\protocol.txt";
	string pathToMatrix = "graphs\\matrix.txt";
	int startV = -1, endV = -1;
	vector<vector<int>> *graph;
	vector<int> editedEdge;
	Ant ant;
	AStar aStar;
	void ClearScreen(){
		SDL_SetRenderDrawColor(renderer, BG_COLOR, 255);
		SDL_RenderClear(renderer);
	}
	void StartInputWeight(vector<vector<int>> *g, vector<int> *e){
		weightInput.LinkRenderer(renderer);
		weightInput.SetupMe(to_string((*graph)[(*e)[0]][(*e)[1]]), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 15, {EDITBOX_COLOR},
							{EDITBOX_FRAME_COLOR}, {EDITBOX_INPUT_COLOR}, {EDITBOX_FONT_COLOR});
	}
	void EndInputWeight(SDL_Event event){
		weightInput.KeyboardListener(event.key.keysym.sym, SDLK_RETURN);
		gBuilder.SetVal(editedEdge[0], editedEdge[1], weightInput.GetIntValue());
		editedEdge.clear();
		weightInput.Clear();
	}
	void InputInWeight(SDL_Event event){
		if(keys[SDLK_LEFT]) weightInput.KeyboardListener(event.key.keysym.sym, SDLK_LEFT);
		else if(keys[SDLK_RIGHT]) weightInput.KeyboardListener(event.key.keysym.sym, SDLK_RIGHT);
		else if(keys[SDLK_RETURN]) weightInput.KeyboardListener(event.key.keysym.sym, SDLK_RETURN);
		else if(keys[SDLK_BACKSPACE]) weightInput.KeyboardListener(event.key.keysym.sym, SDLK_BACKSPACE);
		else weightInput.KeyboardListener(event.key.keysym.sym);
	}
	void InputInBox(EditBox *b, SDL_Event event){
		if(keys[SDLK_LEFT]) (*b).KeyboardListener(event.key.keysym.sym, SDLK_LEFT);
		else if(keys[SDLK_RIGHT]) (*b).KeyboardListener(event.key.keysym.sym, SDLK_RIGHT);
		else if(keys[SDLK_RETURN]) (*b).KeyboardListener(event.key.keysym.sym, SDLK_RETURN);
		else if(keys[SDLK_BACKSPACE]) (*b).KeyboardListener(event.key.keysym.sym, SDLK_BACKSPACE);
		else (*b).KeyboardListener(event.key.keysym.sym);
	}
	void ActionOnButton(string val){
		if(val == "Ant"){
			ShowAntButtons();
			gViewer.SetMode(V_MODE);
			nowAlgorithm = "ant";
		} else if(val == "A*"){
			ShowAStarButtons();
			gViewer.SetMode(V_MODE);
			nowAlgorithm = "A*";
		} else if(val == "Random Graph"){
			startV = endV = -1;
			gViewer.SetStart(-1);
			gViewer.SetEnd(-1);
			int val = rand() % (maxVertices - minVertices + 1) + minVertices;
			gBuilder.CreateRandomGraph(val);
		} else if(val == "Load Graph"){
			startV = endV = -1;
			gViewer.SetStart(-1);
			gViewer.SetEnd(-1);
			gBuilder.LoadGraphFromFile(pathToGraph);
		} else if(val == "Save Graph") gBuilder.SaveGraphInFile(pathToGraph);
		else if(val == "Edit Graph") system(("notepad.exe "+pathToDir + pathToGraph).c_str());
		else if(val == "Create New"){
			gBuilder.CreateGraph(pathToDir+pathToGraph);
			gBuilder.Recalculate();
		} else if(val == "Spray Pheromone") ant.SprayPheromone(gBuilder.GetGraph());
		else if(val == "Show Pher."){
			if(gViewer.GetMode() == P_MODE) gViewer.SetMode(V_MODE);
			else gViewer.SetMode(P_MODE);
		} else if(val == "Show Pher. Matrix"){
			vector<vector<float>> *m = ant.GetPheromones();
			if(!m) return;
			ofstream file;
			file.open(pathToMatrix, ios::out | ios::trunc);
			if(!file.is_open()) return;
			if((*m).empty()) file << "matrix empty" << endl;
			else{
				string s = "";
				for(unsigned int i = 0; i < (*m).size(); i++){
					for(unsigned int j = 0; j < (*m)[i].size(); j++){
						s += to_string((*m)[i][j]);
						s = s.substr(0, s.size() - 4); 
						s += '\t';
					}
					s += '\n';
					file << s;
					s = "";
				}
			}
			file.close();
			system(("notepad.exe " + pathToDir + pathToMatrix).c_str());
		} else if(val == "Show Heuristics"){
			if(endV < 0) return;
			if(gViewer.GetMode() == V_MODE) gViewer.SetMode(H_MODE);
			else gViewer.SetMode(V_MODE);
		} else if(val == "Start"){
			if(startV >= 0 && endV >= 0){
				if(nowAlgorithm == "A*")
					aStar.FindPath(gBuilder.GetGraph(), gBuilder.GetHeuristics(endV), startV, endV);
				ShowPath();
			}
		} else if(val == "Step" || val == "Cycle"){
			if(startV >= 0 && endV >= 0){
				if(nowAlgorithm != "ant") return;
				int iter = 0;
				int boxVal = boxes[0].GetIntValue();
				int antsNum = boxes[1].GetIntValue();
				if(val == "Cycle"){
					iter = boxVal;
					boxVal = 0;
				} else if(val == "Step"){
					if(boxVal < stepSize) iter = boxVal;
					else{
						boxVal -= stepSize;
						iter = stepSize;
					}
				}
				boxes[0].SetNewVal(to_string(boxVal));
				ant.FindPath(gBuilder.GetGraph(), iter, antsNum, startV, endV);
				ShowPath();
			}
		} else if(val == "Show Protocol") system(("notepad.exe " + pathToDir + pathToProtocol).c_str());
	}
	void ShowAntButtons(){
		path = nullptr;
		for(unsigned int i = 7; i < 14; i++) buttons[i].SetVisibility(false);
		for(unsigned int i = 7; i < 12; i++) buttons[i].SetVisibility(true);
		boxes[0].SetVisibility(true);
		boxes[1].SetVisibility(true);
		textfields[1].SetVisibility(false);
		textfields[2].SetVisibility(false);
		textfields[3].SetVisibility(true);
	}
	void ShowAStarButtons(){
		for(unsigned int i = 7; i < 14; i++) buttons[i].SetVisibility(false);
		boxes[0].SetVisibility(false);
		boxes[1].SetVisibility(false);
		for(unsigned int i = 12; i < 14; i++) buttons[i].SetVisibility(true);
		textfields[1].SetVisibility(false);
		textfields[2].SetVisibility(false);
		textfields[3].SetVisibility(false);
	}
	void ShowPath(){
		int minLength;
		if(nowAlgorithm == "NaN") return;
		if(nowAlgorithm == "ant"){
			path = ant.GetPath();
			minLength = ant.GetMinLength();
		} else if(nowAlgorithm == "A*"){
			path = aStar.GetMinPath();
			minLength = aStar.GetMinLength();
		}
		if(!path) return;
		string str="";
		for(unsigned int i = 0; i < (*path).size(); i++) str += to_string((*path)[i]) + ' ';
		textfields[1].SetNewVal("MinPath:" + str);
		textfields[1].SetVisibility(true);
		textfields[2].SetNewVal("MinLength" + to_string(minLength));
		textfields[2].SetVisibility(true);
	}
public:
	MainMenu(string name, bool active):Menu(name, active){fBuilder.TTFInit();}
	MainMenu(){}
	~MainMenu(){fBuilder.TTFQuit();}
	void SetPathToDir(string val){this->pathToDir = val;}
	void InitMenu(){
		int shift = 100;
		path = nullptr;
		srand(time(0));
		graph = nullptr;
		int val = rand() % (maxVertices - minVertices + 1) + minVertices;
		gBuilder.CreateRandomGraph(val);
		graph = gBuilder.GetGraph();
		//graph viewer init
		gViewer.Setup(SCREEN_WIDTH, SCREEN_HEIGHT);
		gViewer.LinkRenderer(renderer);
		//i know, it very bad
		Button button;
		button.LinkRenderer(renderer);
		for(unsigned int i = 0; i < 15; i++) buttons.push_back(button);
		buttons[0].SetupMe("Create New", SCREEN_WIDTH - shift, 20, 12, {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {BTN_FONT_COLOR});
		buttons[1].SetupMe("Random Graph", SCREEN_WIDTH - shift, 60, 12, {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {BTN_FONT_COLOR});
		buttons[2].SetupMe("Load Graph", SCREEN_WIDTH - shift, 100, 12, {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {BTN_FONT_COLOR});
		buttons[3].SetupMe("Save Graph", SCREEN_WIDTH - shift, 140, 12, {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {BTN_FONT_COLOR});
		buttons[4].SetupMe("Edit Graph", SCREEN_WIDTH - shift, 180, 12, {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {BTN_FONT_COLOR});
		buttons[5].SetupMe("A*", SCREEN_WIDTH - 120, 220, 12, {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {BTN_FONT_COLOR});
		buttons[6].SetupMe("Ant", SCREEN_WIDTH - 50, 220, 12, {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {BTN_FONT_COLOR});
		buttons[7].SetupMe("Spray Pheromone", SCREEN_WIDTH - shift, 260, 12, {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {BTN_FONT_COLOR});
		buttons[8].SetupMe("Show Pher.", SCREEN_WIDTH - shift, 300, 12, {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {BTN_FONT_COLOR});
		buttons[9].SetupMe("Show Pher. Matrix", SCREEN_WIDTH - shift, 340, 12, {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {BTN_FONT_COLOR});
		buttons[10].SetupMe("Step", SCREEN_WIDTH - 50, 380, 12, {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {BTN_FONT_COLOR});
		buttons[11].SetupMe("Cycle", SCREEN_WIDTH - 50, 420, 12, {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {BTN_FONT_COLOR});
		buttons[12].SetupMe("Show Heuristics", SCREEN_WIDTH - shift, 260, 12, {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {BTN_FONT_COLOR});
		buttons[13].SetupMe("Start", SCREEN_WIDTH - shift, 300, 12, {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {BTN_FONT_COLOR});
		buttons[14].SetupMe("Show Protocol", SCREEN_WIDTH - 250, 20, 12, {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {BTN_FONT_COLOR});
		for(unsigned int i = 7; i < 14; i++) buttons[i].SetVisibility(false);
		TextField field;
		field.LinkRenderer(renderer);
		for(unsigned int i=0;i<5; i++) textfields.push_back(field);
		textfields[0].SetupMe("Select Algorithm:", SCREEN_WIDTH - 220, 220, 12, {BG_COLOR}, {BG_COLOR}, {BG_COLOR}, {FONT_COLOR});
		textfields[1].SetupMe("Min Path:", SCREEN_WIDTH - shift, 500, 12, {BG_COLOR}, {BG_COLOR}, {BG_COLOR}, {FONT_COLOR});
		textfields[2].SetupMe("Min Length:", SCREEN_WIDTH - shift, 540, 12, {BG_COLOR}, {BG_COLOR}, {BG_COLOR}, {FONT_COLOR});
		textfields[3].SetupMe("Ants Num", SCREEN_WIDTH - 150, 460, 12, {BG_COLOR}, {BG_COLOR}, {BG_COLOR}, {FONT_COLOR});
		textfields[4].SetupMe("Log:", 700, 580, 12, {BG_COLOR}, {BG_COLOR}, {BG_COLOR}, {FONT_COLOR});
		for(unsigned int i = 1; i < 5; i++) textfields[i].SetVisibility(false);
		EditBox box;
		box.LinkRenderer(renderer);
		boxes.push_back(box);
		boxes.push_back(box);
		boxes[0].SetupMe("0", SCREEN_WIDTH - 160, 400, 12, {EDITBOX_COLOR}, {EDITBOX_FRAME_COLOR}, {EDITBOX_INPUT_COLOR}, {EDITBOX_FONT_COLOR});
		boxes[0].SetVisibility(false);
		boxes[1].SetupMe("0", SCREEN_WIDTH - 50, 460, 12, {EDITBOX_COLOR}, {EDITBOX_FRAME_COLOR}, {EDITBOX_INPUT_COLOR}, {EDITBOX_FONT_COLOR});
		boxes[1].SetVisibility(false);
		ant.SetPath(pathToProtocol);
		aStar.SetPath(pathToProtocol);
	}
	void MainLoop(){
		SDL_Event event;
		SDL_MouseButtonEvent bEvent;
		int mouseX, mouseY;
		gViewer.SetMode(V_MODE);
		bool running = true;
		int selectedVertex;
		while(running){
			while(SDL_PollEvent(&event)){
				switch(event.type){
					case SDL_KEYDOWN:
						keys[event.key.keysym.sym] = 1;
						if(keys[SDLK_ESCAPE]) running = false;
						if(keys[SDLK_RETURN]) 
							if(!editedEdge.empty()) EndInputWeight(event);
						//the process of entering a new rib weight
						if(!editedEdge.empty()) InputInWeight(event);
						for(unsigned int i = 0; i < boxes.size(); i++)
							if(boxes[i].GetVisibility() == true) InputInBox(&boxes[i], event);
					break;
					case SDL_KEYUP:
						keys[event.key.keysym.sym] = 0;
					break;
					case SDL_MOUSEBUTTONUP:
						SDL_GetMouseState(&mouseX, &mouseY);
						bEvent = event.button;
						selectedVertex = gViewer.ClickedVertex(mouseX, mouseY);
						if(bEvent.button == SDL_BUTTON_LEFT){
							if(selectedVertex >= 0){
								startV = selectedVertex;
								gViewer.SetStart(startV);
							}
							if(editedEdge.empty()){
								//check, if click on weight value
								editedEdge = gViewer.CheckClickOnVal(gBuilder.GetGraph(), mouseX, mouseY);
								for(unsigned int i = 0; i < boxes.size(); i++) boxes[i].KeyboardListener(event.key.keysym.sym, SDLK_RETURN);
								//if clicked, start input
								if(!editedEdge.empty()) StartInputWeight(gBuilder.GetGraph(), &editedEdge);
							}
							for(unsigned int i = 0; i < buttons.size(); i++) 
								if(buttons[i].MouseEventListener(event, mouseX, mouseY)) ActionOnButton(buttons[i].GetStrValue());
							for(unsigned int i = 0; i < boxes.size(); i++)
								if(boxes[i].MouseEventListener(event, mouseX, mouseY)) weightInput.KeyboardListener(event.key.keysym.sym, SDLK_RETURN);
						} else if(bEvent.button = SDL_BUTTON_RIGHT)
							if(selectedVertex >= 0){
								endV = selectedVertex;
								gViewer.SetEnd(endV);
							}
					break;
					case SDL_MOUSEBUTTONDOWN:
						SDL_GetMouseState(&mouseX, &mouseY);
						for(unsigned int i = 0; i < buttons.size(); i++) buttons[i].MouseEventListener(event, mouseX, mouseY);
						if(!editedEdge.empty()) weightInput.MouseEventListener(event, mouseX, mouseY);
					break;
					case SDL_QUIT:
						running = false;
						break;
				}
			}
			ClearScreen();
			if(graph){
				if(endV >= 0) gViewer.Draw(graph, gBuilder.GetHeuristics(endV), ant.GetPheromones(), path);
				else gViewer.Draw(graph, nullptr, ant.GetPheromones(), path);
			}
			if(!editedEdge.empty()) weightInput.Draw();
			for(unsigned int i = 0; i < buttons.size(); i++) buttons[i].Draw();
			for(unsigned int i = 0; i < textfields.size(); i++) textfields[i].Draw();
			for(unsigned int i = 0; i < boxes.size(); i++) boxes[i].Draw();
			SDL_RenderPresent(renderer);
		}
	}
};

#endif