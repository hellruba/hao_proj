#pragma once
#include <vector>
#include "file_info.hh"
#include "file_update.hh"
#include <gtk/gtk.h>
class Handler {

	public:
		Handler();
		~Handler() = default;
		void init_window(const char* dir_name);
		void init_grid();
		void populate_grid();
		void init_table();
		void init_next_button();
		void init_previous_button();
		void init_button_replace();
		void init_button_injection();
		void run();
		void grid_previous();
		void grid_next();
		static void call_grid_previous(GtkWidget *widget, gpointer data);
		static void call_grid_next(GtkWidget* widget, gpointer data);
		void replace(GtkWidget* widget);
		void inject(GtkWidget* widget);
		void read_directory(const char* path);
		File_info* process_file_content(char* filename);

		void setBeginIndex(int index);
		int getBeginIndex();

		void setEndIndex(int index);
		int getEndIndex();

		void setPath(std::string path);
		std::string file_name(std::string filename);
		int getSizeInformations();

		enum {
			FILE_NUMBER,
			FILE_NAME,
			TOOL_NAME,
			FILE_TIME,
			RUN_TIME,
			NB_COLUMNS
		};

	private:
		GtkWidget* window;
		GtkWidget* Table;
		GtkWidget* button_next;
		GtkWidget* button_previous;
		GtkWidget* button_replace;
		GtkWidget* button_injection;
		GtkWidget* view_informations;
		GtkWidget* notifications;
		GtkWidget* textZone;

		GtkTreeViewColumn* column;
		GtkListStore* grid_informations;
		
		FileUpdate* fileUpdate;
		int begin_index;
		int end_index;
		std::string path_;
		std::vector<File_info*> informations;

};
