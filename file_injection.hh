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

		void setCoordX(std::string coordX);
		std::string getCoordX() const;

		void setCoordY(std::string coordY);
		std::string getCoordY() const;

		void setTextZone(std::string text);
		std::string getTextZone() const;

		bool run_dialog_injection_file_select();
		bool inject_data_manually(GtkWidget* window);
		bool inject_data_automatically();
		bool manual_or_auto(GtkWidget* window);
		
	private:
		GtkWidget* dialog;
		GtkWidget* entry;
		GtkWidget* textZone;
		std::string filename;
		std::string textToInject;
		std::vector<std::pair<size_t, std::string>> injections;
		bool newCoords;
		size_t nb_lines;
		std::string coordX_;
		std::string coordY_;
		std::string text_;

};
