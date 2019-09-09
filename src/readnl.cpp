/*
 License: GPL
 (C) 2010 by Eugene Kropotkin, e.kropotkin@gmail.com
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

string to_string(long long param) {
	string str = "";
	for (str = ""; param; param /= 10)
		str += (char) ('0' + param % 10);
	reverse(str.begin(), str.end());
	return str;
}

int main(int argc, char** argv) 
{
	if( (argc<2) )
	{
	    printf("---------------------------------------------------\nNo arguments! Need 1 arg = file name to analyze.\nNew lines will be at stdout, log will be at stderr\n---------------------------------------------------\n");
	    return 0;
	}

	//init
	long linecount = 0;
	long filelastfp = 0;
	long fileseek = 0;
	//printf("int %i\n",sizeof(int));
	//printf("long %i\n",sizeof(long));
	//return 0;

	std::string str, cfn;

	fprintf(stderr, "input data: %s\n", argv[1]);

	std::ifstream file, cfi;
	std::ofstream cfo;

	//config
	cfn = argv[1];
	cfn += ".fpos";
	fprintf(stderr, "Config file is: %s\n", cfn.c_str());
	cfi.open(cfn.c_str(), std::ifstream::binary);
	if (cfi) {
		while (std::getline(cfi, str)) {
			if (str.length() < 1) {
				fileseek = 0;
			} else {
				fileseek = atoll(str.c_str());
			};
			fprintf(stderr, "cfi: filepos=%ld\n", fileseek);
		}
	} else {
		fprintf(stderr, "Can't read config file. It's ok for first run\n");
	}

	//data
	file.open(argv[1], std::ifstream::binary);
	if (file) {
		file.seekg(0, file.end);
		filelastfp = file.tellg();
		file.seekg(0, file.beg);
	} else {
		fprintf(stderr, "ERR:Fatal! Can't open source file!\n");
		filelastfp = 0;
	}
	fprintf(stderr, "cfi: filesize= %ld\n", filelastfp);

	if (filelastfp == fileseek) {
		fprintf(stderr, "Waring! Zero data: seek=%ld last=%ld\n", filelastfp,
				fileseek);
		return 0;
	};

	if (filelastfp >= fileseek) {
		fprintf(stderr, "Current length of file: %ld\n", filelastfp);
		file.seekg(fileseek, file.beg);
	} else {
		fprintf(stderr, "Possible file truncated: seek=%ld last=%ld\n",
				filelastfp, fileseek);
		file.seekg(0, file.beg);
	}

	if ((filelastfp < 1) || (fileseek < 0)) {
		fprintf(stderr, "stange file metrics\n");
		return 0;
	}

	while (std::getline(file, str)) {
		printf("%s\n", str.c_str());
		linecount++;
	}
	file.close();

	file.open(argv[1], std::ifstream::binary);
	file.seekg(0, file.end);
	filelastfp = file.tellg();
	file.close();

	//final logging to stderr
	fprintf(stderr, "Config file is: [%s]\n", cfn.c_str());
	cfo.open(cfn.c_str(), std::ifstream::binary | std::ifstream::trunc);
	if (cfo) {
		str = to_string(filelastfp);
		cfo.write(str.c_str(), str.length());
		fprintf(stderr, "cfi: %ld [%s]\n", filelastfp, cfn.c_str());
	} else {
		fprintf(stderr, "Can't write config file....\n");
	}

	fprintf(stderr, "Total lines in file: %ld\n", linecount);
	fprintf(stderr, "Current position in file: %ld\n", filelastfp);
	return 0;
}

