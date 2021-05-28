#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <dirent.h>
#include <string.h>
#include "handler.hh"

File_info::File_info() {}

void File_info::setDiameter(std::string diameter)
{
	diameter_ = diameter;
}
std::string File_info::getDiameter() const
{
	return diameter_;
}
void File_info::setToolName(std::string tool_name)
{
	tool_name_ = tool_name;
}
std::string File_info::getToolName() const {
	return tool_name_;
}
void File_info::setDate(std::string date)
{
	date_ = date;
}
std::string File_info::getDate() const 
{
	return date_;
}
void File_info::setTime(std::string time)
{
	time_ = time;
}
std::string File_info::getTime() const
{
	return time_;
}
void File_info::setName(std::string name)
{
	name_ = name;
}
std::string File_info::getName() const
{
	return name_;
}
void File_info::setFileName(std::string filename)
{
	filename_ = filename;
}
std::string File_info::getFileName()
{
	return filename_;
}
std::string retrieve_info(std::string line, std::string field, int format)
{
	std::string token;
	std::string previous = "";
	std::istringstream iss(line);
	while (iss >> token)
	{

		if (strcmp(previous.c_str(), field.c_str()) == 0)
		{
			if (format == 1) {
				previous = token;
				iss >> token;
				if (strcmp(previous.c_str(), ":") == 0) {
					return token;
				}
			}
			if (format == 2)
			{
				if (strcmp(previous.c_str(), field.c_str()) == 0)
				{
					return token;
				}
			}
		}
		previous = token;
	}
	return "";
}
std::string remove_end_parenthesis(std::string token)
{
	std::string newString = token;
	auto find_parenthesis = token.find(')');
	if (find_parenthesis == std::string::npos)
	{
		return token;
	}
	newString[find_parenthesis] = '\0';
	return newString;
}
void File_info::print_informations_file()
{
	std::cout << "File: " << filename_ << std::endl;
	std::cout << "DATE : " << date_ << std::endl;
	std::cout << "PART NAME : " << name_ << std::endl;
	std::cout << "TOOL NAME : " << tool_name_ << std::endl;
	std::cout << "TOOL DIA : " << diameter_ << std::endl;
	std::cout << "TOOL TIME : " << time_ << std::endl;
	std::cout << "" << std::endl;
}
bool find_name_format(std::string line) {
	return line.find("PART") != std::string::npos && line.find("NAME") != std::string::npos;
}
bool find_tool_name(std::string line)
{
	return line.find("TOOL") != std::string::npos && line.find("NAME") != std::string::npos;
}
int find_time_format(std::string line)
{
	if (line.find("TIME:") != std::string::npos)
	{
		return 2;
	}
	if (line.find("TIME") != std::string::npos)
	{
		return 1;
	}
	return 0;
}
int find_diameter_format(std::string line) {

	if (line.find("TOOL") != std::string::npos && line.find("DIA.:") != std::string::npos)
	{
		return 2;
	}
	if (line.find("TOOL") != std::string::npos && line.find("DIA") != std::string::npos)
	{
		return 1;
	}
	return 0;
}
bool find_date_format(std::string line)
{
	return line.find("DATE:") != std::string::npos;
}
std::string retrieve_time_format_two(std::string content)
{
	std::string token = "TIME:";
	int pos = content.find("TIME:");
	std::string result = content.substr(pos + token.length()); 
	return result;
}
File_info*  process_file_content(char* filename)
{
	auto info = new File_info();
	std::fstream file;
	info->setFileName(filename);
	file.open(filename, std::ios::out | std::ios::in);
	if (!file.is_open())
	{
		return NULL;
	}
	std::string line;
	while (std::getline(file, line))
	{
		if (line.find('(') != std::string::npos)
		{
			if (find_name_format(line)) {
				std::string name = retrieve_info(line, "NAME", 1);
				name = remove_end_parenthesis(name);
				info->setName(name);
			}
			int found_dia = find_diameter_format(line);
			if (found_dia != 0)
			{
				std::string diameter = retrieve_info(line, found_dia == 1 ? "DIA" : "DIA.:", found_dia);
				diameter = remove_end_parenthesis(diameter);
				info->setDiameter(diameter);
			}
			if (find_date_format(line))
			{
				std::string date = retrieve_info(line, "DATE:", 2);
				info->setDate(date);
			}
			if (find_tool_name(line))
			{
				std::string tool_name = retrieve_info(line, "NAME", 1);
				info->setToolName(tool_name);
			}
			int found_time = find_time_format(line);
			if (found_time != 0)
			{
				std::string time = retrieve_info(line, found_time == 1 ? "TIME" : "TIME:", found_time);
				info->setTime(time);
			}
		}
	}
	file.close();
	return info;

}
void read_directory_files(const char* path)
{
	DIR* directory = opendir(path);
	if (directory == NULL)
	{
		return;
	}
	struct dirent* dir;
	while ((dir = readdir(directory)) != NULL)
	{
		if (dir->d_type != DT_DIR)
		{
			char d_path[400];
			char* buffer = dir->d_name;
			sprintf(d_path, "%s/%s", path, buffer);
			auto info = process_file_content(d_path);
			info->print_informations_file();
		}
		else if (dir->d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
			char d_path[400];
			sprintf(d_path, "%s/%s", path, dir->d_name);
			read_directory_files(d_path);
		}
	}
	closedir(directory);

}
int main(int argc, char* argv[])
{
	if (argc >= 2) {

		for (int i = 1; i < argc; i++)
		{
			std::string dir_name = argv[i];
			if (dir_name.find('/') != std::string::npos) {
				int index_ = dir_name.find('/');
				dir_name[index_] = '\0';
			}
			read_directory_files(dir_name.c_str());
		}
	}
}
