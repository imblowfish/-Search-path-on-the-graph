#pragma once
#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include <iostream>
#include <vector>
using namespace std;

class Dijkstra{
private:
	const int MAX_VAL = 9999;
	vector<int> minWeights;				
public:
	Dijkstra(){}
	~Dijkstra(){minWeights.clear();}
	void FindPath(vector<vector<int>> &graph, unsigned int idx){
		minWeights.clear();					
		vector<int> visitedNodes;							
		vector<vector<int>> labelMatrix;					
		int tmp, minIdx, minVal;
		for(unsigned int i = 0; i < graph.size(); i++){
			vector<int> node;
			for(unsigned int j = 0; j < graph.size(); j++){
				if(i == j){
					node.push_back(0);
					continue;
				} else if(graph[i][j] == 0){
					node.push_back(MAX_VAL);
					continue;
				}
				node.push_back(graph[i][j]);
			}
			labelMatrix.push_back(node);
			visitedNodes.push_back(0);
			minWeights.push_back(MAX_VAL);
		}
		minWeights[idx] = 0;
		do{
			minVal = minIdx = MAX_VAL;
			for(unsigned int i = 0; i < graph.size(); i++){
				if(visitedNodes[i] == 0 && (minWeights[i] < minVal)){
					minVal = minWeights[i];
					minIdx = i;
				}
			}
			if(minIdx != MAX_VAL){
				for(unsigned int i = 0; i < graph.size(); i++){
					if(graph[minIdx][i] > 0){
						tmp = minVal + graph[minIdx][i];
						if(tmp < minWeights[i]) minWeights[i] = tmp;
					}
				}
				visitedNodes[minIdx] = 1;
			}
		} while(minIdx < MAX_VAL);
	}
	vector<int>* GetMinWeights(){
		if(minWeights.empty()) return NULL;
		return &minWeights;
	}
};

#endif