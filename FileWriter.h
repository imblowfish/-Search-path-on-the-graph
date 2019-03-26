#pragma once
#ifndef FILEWRITER_H
#define FILEWRITER_H
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class FileWriter{
protected:
	string path;
	template <typename T>
	void WriteArr(string name, vector<T> *v){
		fstream f;
		f.open(path, ios::in | ios::out);
		if(!f.is_open()) return;
		f.seekg(0, f.end);
		string str = "";
		f << name << ":";
		for(unsigned int i = 0; i < (*v).size(); i++) str += to_string((*v)[i]) + ' ';
		str += '\n';
		f << str;
		f.close();
	}
	template <typename T>
	void WriteMatrix(string name, vector<vector<T>> *m){
		fstream f;
		f.open(path, ios::in | ios::out);
		if(!f.is_open()) return;
		f.seekg(0, f.end);
		string str = "";
		f << name << ":" << endl;
		for(unsigned int i = 0; i < (*m).size(); i++){
			for(unsigned int j = 0; j < (*m).size(); j++){
				str += to_string((*m)[i][j]) + ' ';
			}
			str += '\n';
			f << str;
			str = "";
		}
		f.close();
	}
	void WriteStr(string val){
		fstream f;
		f.open(path, ios::in | ios::out);
		if(!f.is_open()) 
			return;
		f.seekg(0, f.end);
		f << val.c_str() << endl;
		f.close();
	}
	template <typename T>
	void WriteVal(string name, T val){
		fstream f;
		f.open(path, ios::in | ios::out);
		if(!f.is_open()) return;
		f.seekg(0, f.end);
		f << name << ":" << to_string(val) << endl;
		f.close();
	}
public:
	void ResetFile(){
		ofstream f;
		f.open(path, ios::out | ios::trunc);
		f.close();
	}
	void SetPath(string path){
		this->path = path;
	}
};

#endif