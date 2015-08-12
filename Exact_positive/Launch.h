/*
 * Launch.h
 *
 *  Created on: Dec 3, 2014
 *      Author: lpan
 */

#ifndef LAUNCH_H_
#define LAUNCH_H_

#include <string>
#include <vector>
#include "RectObject.h"
#include <bitset>

using namespace std;

const int BITLENGTH = 1000000;
const int PIC_WIDTH = 1280;
const int PIC_HEIGHT = 720;

class Launch {
public:
	Launch();
	virtual ~Launch();

	void start(char*lbdir, char*picdir,string outdir);

	void getPointRange(vector<string> &items2, vector<RectObject> &pr);

	void getBitsets(vector<RectObject> &pr, bitset<BITLENGTH> &lbits);

	void getOneBitset(RectObject& rect, bitset<BITLENGTH>& bits);
};

#endif /* LAUNCH_H_ */
