/*
 * ReadFromFile.h
 *
 *  Created on: Dec 3, 2014
 *      Author: lpan
 */

#ifndef READFROMFILE_H_
#define READFROMFILE_H_

#include <string>
#include <vector>

using namespace std;

class ReadFromFile {
public:
	ReadFromFile();
	virtual ~ReadFromFile();

	void read(char* dir, vector<string>& lines);

};

#endif /* READFROMFILE_H_ */
