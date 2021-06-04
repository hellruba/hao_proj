#pragma once
#include <gtk/gtk.h>
#include <string>
class FileUpdate {

	public:
		FileUpdate();
		~FileUpdate() = default;


		std::string getPreviousValue() const;
		void setPreviousValue(std::string value);

		std::string getNewValue() const;
		void setNewValue(std::string value);


		std::string getFileName() const;
		void setFileName(std::string f);

		void replace();
		bool run_dialog_file(GtkWidget* window);
		bool run_dialog_values(GtkWidget* window);
	private:
		std::string filename;
		std::string previous_value;
		std::string new_value;
		std::string text_filename;
		GtkWidget* file_dialog;
		GtkWidget* values_dialog;
		GtkWidget* entry;
};
