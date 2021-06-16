#pragma once
#include <vector>
#include "file_info.hh"
#include "file_update.hh"
#include "file_injection.hh"
#include <stdbool.h>
#include <gtk/gtk.h>

class Handler {

	public:
		Handler();
		~Handler() = default;
		void init_window();
		void init_grid();
		void populate_grid();
		void init_table();
		void init_next_button();
		void init_previous_button();
		void init_file_menu();
		void init_edit_menu();
		void init_menu();
		void run();
		void reset_grid();
		void grid_previous();
		void grid_next();
		static void call_grid_previous(GtkWidget *widget, gpointer data);
		static void call_grid_next(GtkWidget* widget, gpointer data);
		void quit_window();
		void replace(GtkWidget* widget);
		void inject(GtkWidget* widget);
		void open_file();
		void read_directory(const char* path);
		File_info* process_file_content(char* filename);

		void setBeginIndex(int index);
		int getBeginIndex();

		void setEndIndex(int index);
		int getEndIndex();


		void setPath(std::string path);
		std::string file_name(std::string filename);
		
		int getSizeInformations();

		std::string getDirName() const;
		void setDirName(std::string dir_name);
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
		/* gtk widget related to grid */
		GtkWidget* Table;
		GtkWidget* button_next;
		GtkWidget* button_previous;
		GtkWidget* view_informations;
		/* gtk widget related to menu */
		GtkWidget* MenuBox;
		GtkWidget* MenuBar;
		GtkWidget* Menu;

		GtkWidget* FileMenu;
		GtkWidget* EditMenu;

		GtkWidget* notifications;
		GtkWidget* file_dialog;
		GtkWidget* textZone;

		GtkTreeViewColumn* column;
		GtkListStore* grid_informations;
		
		FileUpdate* fileUpdate;
		FileInjection* fileInjection;
		int begin_index;
		int end_index;
		bool work_on_sub_folder = false;
		std::string path_;
		std::string dir_name_;
		std::vector<File_info*> informations;

};
