#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <dirent.h>
#include <string.h>
#include "file_info.hh"

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
void File_info::setRunTime(std::string run_time)
{
	run_time_ = run_time;
}
std::string File_info::getRunTime() const 
{
	return run_time_;
}
