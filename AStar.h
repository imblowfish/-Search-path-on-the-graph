#pragma once
#ifndef ASTAR_H
#define ASTAR_H
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include "FileWriter.h"
using namespace std;

class AStar: public FileWriter{
private:
	const int MAX_VAL = 9999;
	string path;
	int minLength = 0;
	vector<int> closedV,		
				openV,			
				minPath,		
				g,				//path length from beg vertex
				f,				//f(x) = g(x) + h(x)
				h,				//heuristic val
				cameFrom;
	void Clear(){
		closedV.clear(); openV.clear();
		minPath.clear(); cameFrom.clear();
		g.clear(); f.clear(); h.clear();
		minLength = 0;
	}
	void Init(int size){
		for(unsigned int i = 0; i < size; i++){
			g.push_back(0); f.push_back(MAX_VAL); 
			h.push_back(0); closedV.push_back(0); 
			openV.push_back(0); cameFrom.push_back(MAX_VAL);
		}
		minLength = 0;
	}
	unsigned int MinFuncValVertex(){
		int min = MAX_VAL;
		int minIdx = MAX_VAL;
		for(unsigned int i = 0; i < openV.size(); i++){
			if(openV[i] == 1 && f[i] < min){
				min = f[i];
				minIdx = i;
			}
		}
		return minIdx;
	}
	bool EndOfAnOpenSet(){
		for(unsigned int i = 0; i < openV.size(); i++)
			if(openV[i] == 1) return false;
		return true;
	}
	void ReconstructPath(vector<vector<int>> *graph, int start, int end){
		int currentV = end;
		minLength = 0;
		vector<int> reversePath;
		while(currentV != MAX_VAL){
			reversePath.push_back(currentV);
			currentV = cameFrom[currentV];
		}
		minPath.push_back(reversePath[reversePath.size()-1]);
		for(int i = reversePath.size() - 1; i >= 1; i--){
			minPath.push_back(reversePath[i - 1]);
			int from = reversePath[i];
			int to = reversePath[i - 1];
			minLength += (*graph)[from][to];
		}
	}
public:
	AStar(){}
	~AStar(){
		this->Clear();
	}
	vector<int>* GetMinPath(){
		if(minPath.empty()) return nullptr;
		else return &minPath;
	}
	int GetMinLength(){return minLength;}
	bool FindPath(vector<vector<int>> *graph, vector<int> *evristic, unsigned int start, unsigned int end){
		if(start > (*graph).size() || end > (*graph).size()) return false;
		ResetFile();
		this->Clear();
		this->Init((*graph).size());
		WriteStr("A* algorithm");
		vector<vector<int>> weightMatrix;
		for(unsigned int i = 0; i < (*graph).size(); i++){
			vector<int> node;
			for(unsigned int j = 0; j < (*graph).size(); j++){
				if(i == j){
					node.push_back(0);
					continue;
				} else if((*graph)[i][j] == 0){
					node.push_back(MAX_VAL);
					continue;
				}
				node.push_back((*graph)[i][j]);
			}
			weightMatrix.push_back(node);
		}
		openV[start] = 1;					//note starting
		WriteStr("Add " + to_string(start) + " in open");
		g[start] = 0;						
		h[start] = (*evristic)[start];		
		f[start] = g[start] + h[start];		
		while(!EndOfAnOpenSet()){
			WriteArr("OpenSet", &openV);
			WriteArr("ClosedSet", &closedV);
			int x = this->MinFuncValVertex();
			if(x == end){
				this->ReconstructPath(graph, start, end);
				WriteArr("Min Path", &minPath);
				WriteVal("Min Length", minLength);
				return true;
			}
			openV[x] = 0;
			WriteStr("Take " + to_string(x) + " from open");
			closedV[x] = 1;
			WriteStr("Add " + to_string(x) + " in closed");
			for(unsigned int y = 0; y < weightMatrix[x].size(); y++){
				if(closedV[y] == 1) continue;
				if(weightMatrix[x][y] >= MAX_VAL) continue;
				int gNeighbourVal = g[x] + weightMatrix[x][y];
				bool gBetter = false;	
				if(openV[y] == 0){
					openV[y] = 1;
					WriteStr("Add " + to_string(y) + " in open");
					gBetter = true;
				} else{
					if(gNeighbourVal < g[y]){
						gBetter = true;
					} else{
						gBetter = false;
					}
				}
				if(gBetter){
					cameFrom[y] = x;
					g[y] = gNeighbourVal;
					h[y] = (*evristic)[y];
					f[y] = g[y] + h[y];
				}
			}
		}
		WriteStr("Failure to find the way");
		return false;
	}
};

#endif