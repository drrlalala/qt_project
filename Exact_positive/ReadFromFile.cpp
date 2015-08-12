
/*
 * ReadFromFile.cpp
 *
 *  Created on: Dec 3, 2014
 *      Author: lpan
 */

#include "ReadFromFile.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;



ReadFromFile::ReadFromFile() {
    // TODO Auto-generated constructor stub

}

ReadFromFile::~ReadFromFile() {
    // TODO Auto-generated destructor stub
}


void ReadFromFile::read(char*dir, vector<string> & lines){
    ifstream infile;
    infile.open(dir);
    if(!infile){
        cerr << "error: unable to open input file." << dir << endl;
    }
    string s;
    while(getline(infile, s)){
        lines.push_back(s);
    }
}

