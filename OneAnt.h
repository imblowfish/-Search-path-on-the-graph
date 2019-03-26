#pragma once
#ifndef ONE_ANT_H
#define ONE_ANT_H
#include <vector>

class OneAnt{
public:
	vector<int> visitedV;			
	unsigned int nowVertex = 0;		
	int pathLength = 0;				
	bool isDead = false;			
	OneAnt(unsigned int v){this->nowVertex = v;}
	OneAnt(){}
	~OneAnt(){this->ClearMemory();}
	void MoveTo(unsigned int v){
		nowVertex = v;
		visitedV.push_back(nowVertex);
	}
	bool WereVisisted(unsigned int v){
		for(unsigned int i = 0; i < visitedV.size(); i++)
			if(visitedV[i] == v){
				return true;
			}
		return false;
	}
	void ClearMemory(){
		visitedV.clear(); 
		pathLength = 0;
		isDead = false; 
		nowVertex = 0;
	}
};


#endif 