#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include "Dijkstra.h"
using namespace std;


class GraphBuilder{
private:
	unsigned int maxWeight = 50;					//max weight for rand
	vector<vector<int>> graph;						
	vector<vector<int>> heuristics;					
	//to find the heuristics we use dykstra, 
	//since the graph is not tied to the domain and it is not possible to find the correct value 
	//of heuristics heuristics correctness condition heuristics < minimum path from a finite vertex
	Dijkstra dijkstra;
	void LogError(string error){
		cerr << error << endl;
		getchar();
	}
	bool ParseLineToVertex(string line){
		vector<int> vertex;
		string num = "";
		unsigned int i = 0;
		do{
			if((line[i] < 48 || line[i] > 57) && line[i]!=0 && line[i]!=' '){
				cout << (int)line[i] << endl;
				vertex.clear();
				LogError("Wrong symbol in graph:" + line[i]);
				return false;
			}
			if(line[i] == ' ' || i == line.size()){
				vertex.push_back(atoi(num.c_str()));
				num = "";
			}
			num += line[i];
			i++;
		} while(i <= line.size());
		graph.push_back(vertex);
		return true;
	}
	//validation of data in a file with a graph
	bool AllRightWithGraphValues(){
		if(graph.empty()){
			LogError("Graph is Empty");
			return false;
		}
		for(unsigned int i = 0; i < graph.size(); i++)
			if(graph.size() != graph[i].size()){
				cout << "graph size = " << graph.size() << endl;
				cout << "graph[i].size = " << graph[i].size() << endl;
				LogError("Row size != Column size");
				return false;
			}
		for(unsigned int i = 0; i < graph.size(); i++){
			for(unsigned int j = i; j < graph.size(); j++){
				if(i == j)
					if(graph[i][j] != 0){
						LogError("Vertex are connected with itself. Vertex ["+i+']'+'['+j+']');
						return false;
					}
					else continue;
				if(graph[i][j] != graph[j][i]) return false;
			}
		}
		return true;
	}
	void CalculateHeuristics(){
		heuristics.clear();
		for(unsigned int i = 0; i < graph.size(); i++){
			vector<int> *vertex;
			dijkstra.FindPath(this->graph, i);
			vertex = dijkstra.GetMinWeights();
			heuristics.push_back(*vertex);
		}
	}
	void Clear(){
		for(unsigned int i = 0; i < graph.size(); i++){
			if(!heuristics.empty())heuristics[i].clear();
			graph[i].clear();
		}
		graph.clear();
		heuristics.clear();
	}
public:
	GraphBuilder(unsigned int size){CreateRandomGraph(size);}
	GraphBuilder(){}
	~GraphBuilder(){Clear();}
	void CreateGraph(string path){
		fstream file;
		file.open(path.c_str(), ios::out | ios::trunc);
		if(!file.is_open()) return;
		file.close();
		system(("notepad.exe "+path).c_str());
	}
	void LoadGraphFromFile(string path){
		Clear();
		string line;
		ifstream file;
		file.open(path.c_str());
		if(!file.is_open()) return;
		while(getline(file, line)){
			if(this->ParseLineToVertex(line) == false){
				file.close();
				return;
			}
			line = "";
		}
		if(AllRightWithGraphValues()) CalculateHeuristics();
		file.close();
	}
	void SetVal(int i, int j, int val){
		this->graph[i][j] = graph[j][i] = val;
		CalculateHeuristics();
	}
	void SaveGraphInFile(string path){
		if(graph.empty()) return;
		ofstream file;
		file.open(path, ios::out | ios::trunc);
		string line="";
		for(unsigned int i = 0; i < graph.size(); i++){
			for(unsigned int j = 0; j < graph.size(); j++){
				line += to_string(graph[i][j])+' ';
			}
			line = line.substr(0, line.size() - 1);
			line += '\n';
			file << line;
			line = "";
		}
		file.close();
	}
	void Recalculate(){
		if(AllRightWithGraphValues()) CalculateHeuristics();
	}
	void CreateRandomGraph(unsigned int size){
		for(unsigned int i = 0; i < graph.size(); i++) graph[i].clear();
		graph.clear();
		heuristics.clear();
		for(unsigned int i = 0; i < size; i++){
			vector<int> vertex;
			for(unsigned int j = 0; j < size; j++) 
				vertex.push_back(0);
			graph.push_back(vertex);
		}
		for(unsigned int i = 0; i < size; i++){
			for(unsigned int j = i; j < size; j++){
				if(i == j) continue;
				int val = rand() % maxWeight;
				if(rand() % 101 > 30) graph[i][j] = graph[j][i] = val;
				else graph[i][j] = graph[j][i] = 0;
			}
		}
		CalculateHeuristics();
	}
	vector<vector<int>>* GetGraph(){
		if(!graph.empty()) return &graph;
		else return NULL;
	}
	vector<int>* GetHeuristics(unsigned int endIdx){
		if(endIdx >= heuristics.size()) return NULL;
		return &heuristics[endIdx];
	}
	string GraphString(){
		if(graph.empty()) return "";
		string str = "N| ";
		string spaces = "\n---";
		for(unsigned int i = 0; i < graph.size(); i++){
			str += to_string(i) + '\t';
			spaces += "-------";
		}
		spaces += '\n';
		str += spaces;
		for(unsigned int i = 0; i < graph.size(); i++){
			str += to_string(i) + '|' + ' ';
			for(unsigned int j = 0; j < graph.size(); j++) str += to_string(graph[i][j]) + '\t';
			str += '\n';
		}	
		return str;
	}
};

#endif