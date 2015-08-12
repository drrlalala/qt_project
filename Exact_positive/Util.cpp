/*
 * Util.cpp
 *
 *  Created on: Nov 20, 2014
 *      Author: lpan
 */

#include "Util.h"
#include <vector>

using namespace std;

Util::Util() {
    // TODO Auto-generated constructor stub

}

Util::~Util() {
    // TODO Auto-generated destructor stub
}

string Util::trim(string s){
    if(s.empty()){
        return s;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

void Util::split(string s, string symbol, vector<string> &items){
    size_t last = 0;
    size_t index = s.find_first_of(symbol, last);
    while (index != std::string::npos){
        items.push_back(s.substr(last, index - last));
        last = index + 1;
        index = s.find_first_of(symbol,last);
    }
    if (index-last>0){
        items.push_back(s.substr(last, index - last));
    }
}


