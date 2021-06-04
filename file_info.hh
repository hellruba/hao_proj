#pragma once
#include <string.h>

class File_info {

	public:
		File_info();
		~File_info() = default;
	void setFileName(std::string filename);
	std::string getFileName();

	void setName(std::string name);
	std::string getName() const;

	void setDiameter(std::string diameter);
	std::string getDiameter() const;	
	void print_informations_file();
	
	void setDate(std::string date);
	std::string getDate() const;

	void setTime(std::string time);
	std::string getTime() const;

	void setToolName(std::string tool_name);
	std::string getToolName() const;

	void setRunTime(std::string run_time);
	std::string getRunTime() const;
	private:
		std::string filename_;
		std::string name_;
		std::string diameter_;
		std::string tool_name_;
		std::string date_;
		std::string time_;
		std::string run_time_;
};
