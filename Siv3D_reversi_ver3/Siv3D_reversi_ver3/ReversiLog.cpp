#include "ReversiLog.h"

ReversiLog::ReversiLog()
{
	data = NULL;
}

ReversiLog::~ReversiLog()
{
	if (data != NULL) {
		data->close();
		delete data;
	}
}

void ReversiLog::Delete()
{
	if (data != NULL) {
		data->close();
		delete data;
	}
}

void ReversiLog::LoadFile(std::string file)
{
	data = new std::ofstream(file);
}

void ReversiLog::ReadFile(std::string file)
{
	std::ifstream ifs(file);
	std::string str;
	while (getline(ifs, str)) {

		std::vector<std::string> s_pos = split(str, ',');

		if (s_pos.size() == 2) {
			//y,x
			std::pair<int, int> pos;
			pos.first = stoi(s_pos[0]);
			pos.second = stoi(s_pos[1]);
			input_log.push_back(pos);
		}
	}
}

void ReversiLog::WriteString(std::string str)
{
	if (data != NULL) {
		*data << str;
	}
}

std::pair<int, int> ReversiLog::get_log(int count)
{
	return input_log[count];
}

std::vector<std::string> split(const std::string &str, char sep)
{
	std::vector<std::string> v;
	std::stringstream ss(str);
	std::string buffer;
	while (std::getline(ss, buffer, sep)) {
		v.push_back(buffer);
	}
	return v;
}