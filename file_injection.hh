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
		GtkWidget* dialog;
		GtkWidget* entry;
		GtkWidget* textZone;
		std::vector<std::pair<std::pair<std::string, std::string>, std::string>> injections;
		bool newCoords;
		std::string coordX;
		std::string coordY;
		std::string text;

};
