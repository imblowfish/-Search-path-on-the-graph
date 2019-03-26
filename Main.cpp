#include "stdafx.h"
#include "Main.h"
#include "MainMenu.h"
using namespace std;

int main(int argc, char *argvp[]){
	if(!initSDL()){
		system("pause");
		return -1;
	} else cout << "initSDL() success" << endl;
	MainMenu menu("mainMenu", true);
	//menu.SetPathToDir(argvp[0]);
	menu.LinkRenderer(renderer);
	menu.InitMenu();
	menu.MainLoop();
	outSDL();
	return 0;
}

