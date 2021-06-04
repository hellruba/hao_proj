#pragma once
#include <gtk/gtk.h>
#include <vector>
#include <string>
#include <stdbool.h>
#include <utility>

class FileInjection
{
	public:
		FileInjection();
		~FileInjection() = default;
	private:
		std::vector<std::pair<std::pair<std::string, std::string>, std::string>> injections;
		bool

};
