#pragma once
#ifndef GRAPHVIEWER_H
#define GRAPHVIEWER_H
#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"
#include <string>
#include <vector>
#include "Colors.h"
#include "FontBuilder.h"
#include "EditBox.h"
using namespace std;

enum GraphMode{V_MODE, P_MODE, H_MODE};
//VERTEX_MODE, PHEROMONE_MATRIX_MODE, HEURISTICS_MODE

class GraphViewer{
private:
	SDL_Rect fontRect;						
	SDL_Renderer *renderer;					
	TTF_Font *vFont,						//vertex font
			 *eFont;						//enge font
	int vFontSize = 12, eFontSize = 9;
	unsigned int ellipseRadius = 5;
	float verticesRadius = 12.0;			
	unsigned int graphSize = 0;			
	unsigned int SCREEN_WIDTH = 0;
	unsigned int SCREEN_HEIGHT = 0;
	int startV = -1, endV = -1;
	int nowMode = V_MODE;
	unsigned int centerX = 0;				//local center x
	unsigned int centerY = 0;				//local center y
	int graphShift = 140;					//grap's left shift from center
	int valShift = 60;						//shift signatures weight relative to point
	FontBuilder fBuilder;
	void DrawOneVertex(string value, int xPos, int yPos, int r, int g, int b){
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		this->DrawEllipse(xPos, yPos, this->verticesRadius);
		this->TextOut(value, xPos, yPos, VERTEX_NUM_COLOR, false);
	}
	void DrawGraphVertices(unsigned int numOfVertices, vector<int> *heuristic = nullptr){
		float alpha = float((2 * M_PI) / numOfVertices);
		float dt = float(3*M_PI/2);
		int x, y = 0;
		for(unsigned int i = 0; i < numOfVertices; i++){
			x = int((this->ellipseRadius * sin(dt)) * 180 / M_PI);
			y = int((this->ellipseRadius * cos(dt)) * 180 / M_PI);
			dt -= alpha;

			SDL_Color vertexColor;
			if(startV >= 0 && i == startV) vertexColor = {VERTEX_START_COLOR};
			else if(endV >= 0 && i == endV) vertexColor = {VERTEX_END_COLOR};
			else vertexColor = {VERTEX_COLOR};
			if(nowMode == H_MODE && heuristic && endV >= 0)
				DrawOneVertex(to_string((*heuristic)[i]), centerX + x - graphShift, centerY + y, vertexColor.r, vertexColor.g, vertexColor.b);
			else DrawOneVertex(to_string(i), centerX + x - graphShift, centerY + y, vertexColor.r, vertexColor.g, vertexColor.b); //либо порядковый номер вершины
		}
	}
	void DrawEllipse(int xPos, int yPos, int radius){
		//midpoint algorithm
		int x = radius - 1;
		int y = 0;
		int dx = 1;
		int dy = 1;
		int err = dx - (radius << 1);
		while(x >= y){
			SDL_RenderDrawPoint(renderer, xPos + x, yPos + y);
			SDL_RenderDrawPoint(renderer, xPos + y, yPos + x);
			SDL_RenderDrawPoint(renderer, xPos - y, yPos + x);
			SDL_RenderDrawPoint(renderer, xPos - x, yPos + y);
			SDL_RenderDrawPoint(renderer, xPos - x, yPos - y);
			SDL_RenderDrawPoint(renderer, xPos - y, yPos - x);
			SDL_RenderDrawPoint(renderer, xPos + y, yPos - x);
			SDL_RenderDrawPoint(renderer, xPos + x, yPos - y);
			if(err <= 0){
				y++;
				err += dy;
				dy += 2;
			}
			if(err > 0){
				x--;
				dx += 2;
				err += dx - (radius << 1);
			}
		}
	}
	void DrawEdge(int v1, int v2, float val, bool matrixMode = false){
		if(v1 < 0 || v1 >= this->graphSize) return;
		if(v2 < 0 || v2 >= this->graphSize) return;
		float alpha = float(2 * M_PI / this->graphSize);
		float dt = float(3 * M_PI / 2);
		int x1, y1, x2, y2;
		x1 = int((this->ellipseRadius * sin(dt - v1 * alpha)) * 180 / M_PI);
		y1 = int((this->ellipseRadius * cos(dt - v1 * alpha)) * 180 / M_PI);
		x2 = int((this->ellipseRadius * sin(dt - v2 * alpha)) * 180 / M_PI);
 		y2 = int((this->ellipseRadius * cos(dt - v2 * alpha)) * 180 / M_PI);
		SDL_RenderDrawLine(this->renderer, centerX + x1 - this->graphShift, centerY + y1, centerX + x2 - this->graphShift, centerY + y2);
		int textX=0, textY=0;
		float fullLen = float(sqrt(pow((x2-x1),2)+pow((y2-y1),2)));
		if(fullLen != 0){
			textX = int(x1 + (x2 - x1) * (valShift / fullLen));
			textY = int(y1 + (y2 - y1) * (valShift / fullLen));
		}
		if(!matrixMode)this->TextOut(to_string((int)val), centerX + textX - graphShift, centerY + textY, FONT_COLOR, true);
		else{
			string str = to_string(val);
			str = str.substr(0, str.size() - 4);
			this->TextOut(str, centerX + textX - graphShift, centerY + textY, FONT_COLOR, true);
		}
	}
	void DrawPath(int v1, int v2){
		if(v1 < 0 || v1 >= this->graphSize) return;
		if(v2 < 0 || v2 >= this->graphSize) return;
		float alpha = float(2 * M_PI / this->graphSize);
		float dt = float(3 * M_PI / 2);
		int x1, y1, x2, y2;
		x1 = int((this->ellipseRadius * sin(dt - v1 * alpha)) * 180 / M_PI);
		y1 = int((this->ellipseRadius * cos(dt - v1 * alpha)) * 180 / M_PI);
		x2 = int((this->ellipseRadius * sin(dt - v2 * alpha)) * 180 / M_PI);
		y2 = int((this->ellipseRadius * cos(dt - v2 * alpha)) * 180 / M_PI);
		SDL_RenderDrawLine(this->renderer, centerX + x1 - this->graphShift, centerY + y1, centerX + x2 - this->graphShift, centerY + y2);
	}
	void TextOut(string value, int x, int y, Uint8 red, Uint8 green, Uint8 blue, bool hasBackground){
		SDL_Surface *surface = NULL;
		SDL_Color fontColor = {red, green, blue};
		if(hasBackground){
			SDL_Color bgColor = {BG_COLOR};
			surface = TTF_RenderText_Shaded(this->eFont, value.c_str(), fontColor, bgColor);
		} else
			surface = TTF_RenderText_Solid(this->vFont, value.c_str(), fontColor);
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
		int fontWidth = 0, fontHeight = 0;
		SDL_QueryTexture(texture, NULL, NULL, &fontWidth, &fontHeight);
		fontRect = {x, y, fontWidth, fontHeight};
		fontRect.x -= int(fontRect.w / 2);
		fontRect.y -= int(fontRect.h / 2);
		SDL_RenderCopy(renderer, texture, NULL, &fontRect);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
	}
	bool ClickedVals(int v1, int v2, int x, int y){
		if(v1 < 0 || v1 >= this->graphSize) return false;
		if(v2 < 0 || v2 >= this->graphSize) return false;
		int pixelMaxDiffX = 10, pixelMaxDiffY = 8;
		float alpha = float(2 * M_PI / this->graphSize);
		float dt = 4.71f;
		int x1, y1, x2, y2;
		x1 = int((this->ellipseRadius * sin(dt - v1 * alpha)) * 180 / M_PI);
		y1 = int((this->ellipseRadius * cos(dt - v1 * alpha)) * 180 / M_PI);
		x2 = int((this->ellipseRadius * sin(dt - v2 * alpha)) * 180 / M_PI);
		y2 = int((this->ellipseRadius * cos(dt - v2 * alpha)) * 180 / M_PI);
		float fullLen = float(sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2)));
		int textX = 0, textY = 0;
		if(fullLen != 0){
			textX = int(x1 + (x2 - x1) * (valShift / fullLen));
			textY = int(y1 + (y2 - y1) * (valShift / fullLen));
		}
		if(fabs(centerX + textX - graphShift - x) < pixelMaxDiffX && fabs(centerY + textY - y) < pixelMaxDiffY)
			return true;
		return false;
	}
public:
	GraphViewer(unsigned int width, unsigned int height){Setup(width, height);}
	GraphViewer(){}
	~GraphViewer(){
		fBuilder.CloseFont(vFont);
		fBuilder.CloseFont(eFont);
		renderer = nullptr;
	}
	void Setup(unsigned int width, unsigned int height){
		SCREEN_WIDTH = width;
		SCREEN_HEIGHT = height;
		centerX = int(SCREEN_WIDTH / 2);
		centerY = int(SCREEN_HEIGHT / 2);
		vFont = fBuilder.CreateFont(vFont, vFontSize);
		eFont = fBuilder.CreateFont(eFont, eFontSize);
		renderer = nullptr;
	}
	void LinkRenderer(SDL_Renderer *rend){this->renderer = rend;}
	int ClickedVertex(int x, int y){
		int pixelMaxDiff = 10;
		float alpha = float(2 * M_PI / graphSize);
		float dt = float(3 * M_PI / 2);
		int vx, vy;
		for(unsigned int i = 0; i < this->graphSize; i++){
			vx = int(this->ellipseRadius * sin(dt) * 180 / M_PI);
			vy = int(this->ellipseRadius * cos(dt) * 180 / M_PI);
			vx += centerX - graphShift;
			vy += centerY;
			if(fabs(vx-x) < pixelMaxDiff && fabs(vy - y) < pixelMaxDiff) return i;
			dt -= alpha;
		}
		return -1;
	}
	vector<int> CheckClickOnVal(vector<vector<int>> *graph, int x, int y){
		vector<int> v;
		if((*graph).empty()) return v;
		if(nowMode == P_MODE) return v;
		for(unsigned int i = 0; i < (*graph).size(); i++){
			for(unsigned int j = i; j < (*graph).size(); j++){
				if((*graph)[i][j] == 0) continue;
				if(ClickedVals(i, j, x, y)){
					v.push_back(i);
					v.push_back(j);
					return v;
				}
			}
		}
		return v;
	}
	void SetMode(int mode){nowMode = mode;}
	int GetMode(){
		return nowMode;
	}
	void SetStart(int v){
		startV = v;
	}
	void SetEnd(int v){
		endV = v;
	}
	void Draw(const vector<vector<int>> *graph, vector<int> *heuristic = nullptr, vector<vector<float>> *pMatrix = nullptr, vector<int> *foundPath=nullptr){
		if((*graph).empty()) return;
		SDL_SetRenderDrawColor(renderer, EDGE_COLOR, 255);
		this->graphSize = (*graph).size();
		for(unsigned int i = 0; i < this->graphSize; i++){
			for(unsigned int j = i; j < this->graphSize; j++){
				if(i == j) continue;
				if(nowMode == P_MODE && pMatrix){
					if(((*pMatrix)[i][j] > 0.000001f)){
						this->DrawEdge(i, j, (*pMatrix)[i][j], true);
					} else continue;
				}else if ((*graph)[i][j] != 0) this->DrawEdge(i, j, (*graph)[i][j]);
			}
		}
		if(foundPath){
			SDL_SetRenderDrawColor(renderer, PATH_COLOR, 255);
			for(unsigned int i = 0; i < foundPath->size() - 1; i++){
				this->DrawPath((*foundPath)[i], (*foundPath)[i + 1]);
			}
		}
		SDL_SetRenderDrawColor(renderer, VERTEX_COLOR, 255);
		if(nowMode == H_MODE && heuristic){
			this->DrawGraphVertices(this->graphSize, heuristic);
		}
		else this->DrawGraphVertices(this->graphSize);
	}
};

#endif