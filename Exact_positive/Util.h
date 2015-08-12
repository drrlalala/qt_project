/*
 * Util.h
 *
 *  Created on: Nov 20, 2014
 *      Author: lpan
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Util {
public:
	Util();
	virtual ~Util();
	string trim(string s);
	void split(string s, string symbol, vector<string> &items);

};

#endif /* UTIL_H_ */
