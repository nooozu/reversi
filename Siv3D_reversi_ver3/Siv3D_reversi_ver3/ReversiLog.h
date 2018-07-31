#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>      // std::ostringstream
#include <Siv3D.hpp>

class ReversiLog
{
private:
	std::ofstream *data;
	//pair (y,x)
	std::vector<std::pair<int, int>>input_log;
public:

	ReversiLog();
	~ReversiLog();

	void Delete();
	void LoadFile(std::string file);
	void ReadFile(std::string file);

	void WriteString(std::string str);

	int get_LogSize() { return input_log.size(); }
	std::pair<int, int> get_log(int count);
};

std::vector<std::string> split(const std::string &str, char sep);