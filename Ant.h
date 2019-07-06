#pragma once
#ifndef ANT_H
#define ANT_H
#include <iostream>
#include <time.h>
#include "OneAnt.h"
#include "FileWriter.h"
using namespace std;

class Ant: public FileWriter{	//inherit from FileWriter to write the path finding protocol
private:
	//coeffs
	//if the found paths are not minimal, change the alpha or beta
	const float alpha = 0.9;			//collective intelligence coeff
	const float betta = 0.1;			//individual intelligence coeff
	//alpha + betta = 1
	const float evap = 0.01f;			//evaporation coeff 0<=evap<=1
	const float Q = 8.0f;				//const path legnth order
	const float initPher = 10.0f;		//initial pheromone level
	OneAnt ant;							//just ant
	vector<int> minPath;				//vector for minPath
	vector<vector<float>> pheromones;	//pheromone matrix
	vector<vector<float>> deltaPheromone;//delta matrix, keep's every ant's contribution
	int minLength;						
	unsigned int numOfIterations;
	//init delta
	void InitContributions(vector<vector<int>> *graph){
		for(unsigned int i = 0; i < (*graph).size(); i++){
			vector<float> vertex;
			for(unsigned int j = 0; j < (*graph).size(); j++) vertex.push_back(0);
			deltaPheromone.push_back(vertex);
		}
	}
	void ClearAll(){
		for(unsigned int i = 0; i < pheromones.size(); i++){
			pheromones[i].clear();
			if(!deltaPheromone.empty())deltaPheromone[i].clear();
		}
		pheromones.clear();
		deltaPheromone.clear();
		minPath.clear();
	}
	void ClearPheromoneMatrix(){
		for(unsigned int i = 0; i < pheromones.size(); i++) pheromones[i].clear();
		pheromones.clear();
	}
	void ClearPath(){
		minPath.clear();
		minLength = 0;
	}
	void ClearContributions(){
		for(unsigned int i = 0; i < deltaPheromone.size(); i++)
			for(unsigned int j = 0; j < deltaPheromone.size(); j++)
				deltaPheromone[i][j] = 0;
	}
	//caltulcation of ant contribution in pheromone matrix
	void ToContributePheromone(){
		for(unsigned int i = 0; i < ant.visitedV.size() - 1; i++)
			deltaPheromone[ant.visitedV[i]][ant.visitedV[i + 1]] += Q / ant.pathLength;
	}
	//pheromone matrix update
	void UpdatePheromone(){
		for(unsigned int i = 0; i < pheromones.size(); i++)
			for(unsigned int j = 0; j < pheromones.size(); j++)
					pheromones[i][j] = (1 - evap)*pheromones[i][j] + deltaPheromone[i][j];
	}
	//ïðîâåðêà íà òóïèê äëÿ ìóðàâüÿ
	bool DeadEnd(vector<int> *vertex){
		for(unsigned int i = 0; i < (*vertex).size(); i++)
			if(!ant.WereVisisted(i) && (*vertex)[i] > 0) //if not visited and adjacent to the current
				return false;
		return true;
	}
	unsigned int ChooseNextVertex(int nowVertex, vector<int> *vertex){
		float sum = 0.0f;
		for(unsigned int i = 0; i < (*vertex).size(); i++){
			if((*vertex)[i] == 0 || ant.WereVisisted(i)) continue;
			float N = 1.0f / float((*vertex)[i]);			//parameter inversely proportional to the length of the edge
			sum += pow(pheromones[nowVertex][i], alpha) * pow(N, betta);
		}
		vector<float>probabilities, idx;					
		//divide each vertex by the total amount
		for(unsigned int i = 0; i < (*vertex).size(); i++){
			if((*vertex)[i] == 0 || ant.WereVisisted(i)) continue;
			float N = 1.0f / float((*vertex)[i]);
			float val = pow(pheromones[nowVertex][i], alpha) * pow(N, betta);
			probabilities.push_back(val / sum);
			idx.push_back(i);
		}
		//in sum probabilities = 1
		//we throw a random point on a segment from 0 to 1
		float randVal = (float)(rand() % 101)/100.0f;
		//in wich interval point
		float right =0.0f;
		float left = 0.0f;
		for(unsigned int i = 0; i < probabilities.size(); i++){
			right += probabilities[i];
			if(randVal >= left && randVal < right)return idx[i];
			left = right;	
		}
		return idx[idx.size() - 1];
	}
	//finding the maximum level on pheromone matrix
	int FindMax(vector<int> *v, int idx){
		float max = 0.0f; 
		int maxIdx = -1;
		for(unsigned int i = 0; i < pheromones.size(); i++){
			if((*v)[i] == 0) continue;
			if(max < pheromones[idx][i]){
				max = pheromones[idx][i];
				maxIdx = i;
			}
		}
		return maxIdx;
	}
	void ReconstructPath(vector<vector<int>> *graph, int start, int end){
		int current = start;
		minPath.push_back(current);
		while(current != end){
			int from = current;
			int to = FindMax(&(*graph)[from], from);	
			current = to;
			minPath.push_back(to);
			minLength += (*graph)[from][to];
		}
	}
public:
	Ant(){
		minLength = 0;
		numOfIterations = 0;
	}
	~Ant(){ClearAll();}
	//init base pheromone level on graph
	void SprayPheromone(vector<vector<int>> *graph){
		if(!pheromones.empty()) ClearPheromoneMatrix();
		for(unsigned int i = 0; i < (*graph).size(); i++){
			vector<float> vertex;
			for(unsigned int j = 0; j < (*graph).size(); j++){
				if((*graph)[i][j] > 0) vertex.push_back(initPher);
				else vertex.push_back(0.0f);
			}
			pheromones.push_back(vertex);
			vertex.clear();
		}
	}
	vector<vector<float>>* GetPheromones(){
		if(pheromones.empty()) return nullptr;
		return &pheromones;
	}
	vector<int>* GetPath(){
		if(minPath.empty()) return nullptr;
		else return &minPath;
	}
	int GetMinLength(){return minLength;}
	bool FindPath(vector<vector<int>> *graph, unsigned int iterations, unsigned int antsNum, unsigned int start, unsigned int end){
		if(iterations == 0 || antsNum == 0) return false;
		if(start > (*graph).size() || end > (*graph).size()) return false;
		ResetFile();
		WriteStr("Ant algorithm");
		srand(time(NULL));
		if(pheromones.size() != (*graph).size()){ClearAll();}
		if(pheromones.empty()) SprayPheromone(graph);			//pheromone matrix init
		if(deltaPheromone.empty()) InitContributions(graph);	//delta matrix init
		ClearPath();
		WriteStr("Start");
		WriteMatrix("Pheromones matrix", &pheromones);
		for(unsigned int i = 0; i < iterations; i++){
			ClearContributions();
			for(unsigned int j = 0; j < antsNum; j++){
				bool reachedEnd = false;//if the ant has reached the final vertex, note this
				ant.ClearMemory();		
				ant.MoveTo(start);
				while(!DeadEnd(&(*graph)[ant.nowVertex])){	
					int nextVertex = ChooseNextVertex(ant.nowVertex, &(*graph)[ant.nowVertex]);
					ant.pathLength += (*graph)[ant.nowVertex][nextVertex];
					ant.MoveTo(nextVertex);	
					if(ant.nowVertex == end){
						reachedEnd = true;
						break;
					}
				}
				if(!reachedEnd) ant.isDead = true;
				//an ant is dead, if it is at dead end and the current vertex are not final
				if(!ant.isDead) ToContributePheromone(); //otherwise it contributes
				if(i <= 3) WriteArr("Path ant num."+to_string(j),&ant.visitedV);
			}
			UpdatePheromone();
			//we write in protocol first 3 iterations
			if(i <= 3){
				WriteStr("AFTER ITERATION:" + to_string(i));
				WriteMatrix("Pheromones matrix", &pheromones);
			}
		}
		ReconstructPath(graph, start, end);
		WriteArr("Min Path", &minPath);
		WriteVal("Min Length", minLength);
		return true;
	}
};

#endif
