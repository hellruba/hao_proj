#include <iostream>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include "handler.hh"


Handler::Handler(): begin_index(0), end_index(10) { 
	fileUpdate = new FileUpdate();
}

void Handler::quit_window()
{
	notifications = gtk_message_dialog_new(GTK_WINDOW(window),
			GTK_DIALOG_MODAL,
			GTK_MESSAGE_QUESTION,
			GTK_BUTTONS_YES_NO,
			"Are you sure u want to exit?");
	switch(gtk_dialog_run(GTK_DIALOG(notifications)))
	{
		case GTK_RESPONSE_YES:
			gtk_widget_destroy(window);
			gtk_main_quit();
			break;
		case GTK_RESPONSE_NO:
			gtk_widget_destroy(notifications);
			break;
	}

}
void Handler::reset_grid()
{
	gtk_list_store_clear(grid_informations);
	informations.clear();
	begin_index = 0;
	end_index = 10;
}
void Handler::open_file()
{
	file_dialog = gtk_file_chooser_dialog_new("Open a dir", NULL,
			GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
			NULL);
	bool dir_selected = true;
	switch (gtk_dialog_run(GTK_DIALOG(file_dialog)))
	{
		case GTK_RESPONSE_ACCEPT:
			dir_name_ = std::string(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_dialog)));
			path_ = dir_name_;
			break;
		case GTK_RESPONSE_CANCEL:
		default:
			dir_selected = false;
			break;
	}
	gtk_widget_destroy(file_dialog);
	if (dir_selected)
	{
		notifications = gtk_message_dialog_new(GTK_WINDOW(window),
				GTK_DIALOG_MODAL,
				GTK_MESSAGE_QUESTION,
				GTK_BUTTONS_YES_NO,
				"Do you want to work on sub folders as well?");
		switch(gtk_dialog_run(GTK_DIALOG(notifications)))
		{
			case GTK_RESPONSE_YES:
				work_on_sub_folder = true;

				gtk_widget_destroy(notifications);
				break;
			case GTK_RESPONSE_NO:
				work_on_sub_folder = false;

				gtk_widget_destroy(notifications);
				break;
		}
		reset_grid();
		read_directory(dir_name_.c_str());
		populate_grid();
	}
}
void call_quit_window(GtkWidget* widget, gpointer data)
{
	auto h = (Handler*) data;
	h->quit_window();
}
void call_open(GtkWidget* widget, gpointer data)
{
	auto h = (Handler*) data;
	h->open_file();
}
void Handler::setDirName(std::string dir_name)
{
	dir_name_ = dir_name;
}
std::string Handler::getDirName() const
{
	return dir_name_;
}
void call_injection(GtkWidget* widget, gpointer data)
{
	auto h = (Handler*) data;
	h->inject(widget);
}
void call_replace(GtkWidget* widget, gpointer data)
{
	auto h = (Handler*) data;
	h->replace(widget);
}
void Handler::init_edit_menu()
{
	EditMenu = gtk_image_menu_item_new_with_label("replace");
	g_signal_connect(G_OBJECT(EditMenu), "activate",
			G_CALLBACK(call_replace), this);
	gtk_menu_shell_append(GTK_MENU_SHELL(Menu), EditMenu);

	EditMenu = gtk_image_menu_item_new_with_label("insert lines");
	g_signal_connect(G_OBJECT(EditMenu), "activate",
			G_CALLBACK(call_injection), this);
	gtk_menu_shell_append(GTK_MENU_SHELL(Menu), EditMenu);

	EditMenu = gtk_image_menu_item_new_with_label("Edit");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(EditMenu), Menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar), EditMenu);
}
void Handler::init_file_menu()
{

	FileMenu = gtk_image_menu_item_new_with_label("open");
	g_signal_connect(G_OBJECT(FileMenu), "activate",
			G_CALLBACK(call_open), this);
	gtk_menu_shell_append(GTK_MENU_SHELL(Menu), FileMenu);

	FileMenu = gtk_image_menu_item_new_with_label("quit");
	g_signal_connect(G_OBJECT(FileMenu), "activate",
			G_CALLBACK(call_quit_window), this);
	gtk_menu_shell_append(GTK_MENU_SHELL(Menu), FileMenu);


	FileMenu = gtk_image_menu_item_new_with_label("File");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(FileMenu), Menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar), FileMenu);

}
void Handler::init_menu()
{
	MenuBox = gtk_vbox_new(FALSE, 0);
	MenuBar = gtk_menu_bar_new();
	Menu = gtk_menu_new();
	init_file_menu();
	Menu = gtk_menu_new();
	init_edit_menu();
	gtk_box_pack_start(GTK_BOX(MenuBox), MenuBar, FALSE, FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(Table), MenuBox, 0, 3, 0, 2);
}
void Handler::grid_previous()
{
	if (begin_index != 0)
	{	
		gtk_list_store_clear(grid_informations);
		end_index = end_index - 10;
		begin_index = begin_index - 10;
		if (begin_index < 0)
		{
			begin_index = 0;
		}
		populate_grid();
	}
}
void Handler::grid_next()
{
	if (end_index < informations.size()) {
		gtk_list_store_clear(grid_informations);
		end_index = end_index + 10;
		begin_index = begin_index + 10;
		if (end_index == informations.size())
		{
			end_index = informations.size();
		}
		populate_grid();
	}
}
void Handler::call_grid_previous(GtkWidget* widget, gpointer data)
{
	auto h = (Handler*) data;
	h->grid_previous();
}
void Handler::call_grid_next(GtkWidget* widget, gpointer data)
{
	auto h = (Handler*) data;
	h->grid_next();
}
void Handler::replace(GtkWidget* widget)
{
	bool fileValid = fileUpdate->run_dialog_file(widget);
	if (!fileValid)
	{
		notifications = gtk_message_dialog_new(GTK_WINDOW(window),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_ERROR,
				GTK_BUTTONS_CLOSE,
				"File extension must be a .tap file");
		gtk_dialog_run(GTK_DIALOG(notifications));
		gtk_widget_destroy(notifications);	
	}
	else
	{
		bool replace = fileUpdate->run_dialog_values(window);
		if (replace)
		{
			fileUpdate->replace();
		}
	}
}

void Handler::inject(GtkWidget* widget)
{

}

void Handler::init_previous_button()
{
	button_previous = gtk_button_new_with_label("previous");
	g_signal_connect(G_OBJECT(button_previous), "clicked", G_CALLBACK(call_grid_previous), this);
	gtk_table_attach_defaults(GTK_TABLE(Table), button_previous, 2,3,6,7);
}
void Handler::init_next_button()
{
	button_next = gtk_button_new_with_label("next");
	g_signal_connect(G_OBJECT(button_next), "clicked", G_CALLBACK(call_grid_next),
			this);
	gtk_table_attach_defaults(GTK_TABLE(Table), button_next, 7,8, 6,7);
}
void Handler::init_window()
{
	gtk_init(0, NULL);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 5);
	gtk_window_set_default_size(GTK_WINDOW(window), 1400, 1400);
	gtk_window_set_title(GTK_WINDOW(window), "FILE INFORMATIONS");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	gtk_window_maximize(GTK_WINDOW(window));
	gtk_window_set_gravity(GTK_WINDOW(window), GDK_GRAVITY_CENTER);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
}
void Handler::init_table()
{
	Table = gtk_table_new(10,10, TRUE);
	gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(Table));
}
std::string Handler::file_name(std::string filename)
{
	std::string res= filename.substr(path_.length() + 1, filename.length());
	return res;
}
void Handler::populate_grid()
{
	for (size_t i = 0 + begin_index; i < (end_index < informations.size() ? end_index : informations.size()); i++)
	{
		std::string name = file_name(informations[i]->getFileName());
		gtk_list_store_insert_with_values(grid_informations, NULL, -1,
				FILE_NUMBER, i + 1, FILE_NAME, name.c_str() , TOOL_NAME, informations[i]->getToolName().c_str(),  FILE_TIME,
				informations[i]->getTime().c_str(), RUN_TIME, informations[i]->getRunTime().c_str(), -1);
	}

}
void Handler::init_grid()
{
	grid_informations = gtk_list_store_new(NB_COLUMNS, 
			G_TYPE_UINT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING); 


	/* create the grid view using the model as reference */
	view_informations = gtk_tree_view_new_with_model(GTK_TREE_MODEL(grid_informations));
	g_object_unref(grid_informations);

	/* init the grid column file number */
	column = gtk_tree_view_column_new_with_attributes("File number", gtk_cell_renderer_text_new(),
			"text", FILE_NUMBER, NULL);
	gtk_tree_view_column_set_expand(column, TRUE);

	/* add the column to the grid view */
	gtk_tree_view_append_column(GTK_TREE_VIEW(view_informations), column);

	column = gtk_tree_view_column_new_with_attributes("NAME", gtk_cell_renderer_text_new(),
			"text", FILE_NAME, NULL);

	gtk_tree_view_column_set_expand(column, TRUE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view_informations), column);

	column = gtk_tree_view_column_new_with_attributes("TOOL NAME", gtk_cell_renderer_text_new(),
			"text", TOOL_NAME, NULL);

	gtk_tree_view_column_set_expand(column, TRUE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view_informations), column);

	column = gtk_tree_view_column_new_with_attributes("TIME", gtk_cell_renderer_text_new(),
			"text", FILE_TIME, NULL);
	gtk_tree_view_column_set_expand(column, TRUE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view_informations), column);


	column = gtk_tree_view_column_new_with_attributes("RUN TIME", gtk_cell_renderer_text_new(),
			"text", RUN_TIME, NULL);
	gtk_tree_view_column_set_expand(column, TRUE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view_informations), column);
	gtk_widget_set_size_request(view_informations, 1000, 600);

	/* add the grid to the major window display */
	gtk_table_attach_defaults(GTK_TABLE(Table), view_informations, 2,8, 2,5);
}
void Handler::run()
{
	gtk_widget_show_all(window);
	gtk_main();
}
void Handler::setBeginIndex(int index)
{
	begin_index = index;
}
int Handler::getBeginIndex()
{
	return begin_index;
}
void Handler::setEndIndex(int index)
{
	end_index = index;
}
int Handler::getEndIndex()
{
	return end_index;
}
int Handler::getSizeInformations()
{
	return informations.size();
}
std::string retrieve_run_time(std::string line, std::string field)
{
	std::string token;
	std::string result;
	std::string previous = "";
	std::istringstream iss(line);
	while (iss >> token)
	{
		if(strcmp(previous.c_str(), field.c_str()) == 0)
		{
			previous = token;
			iss >> token;
			if (strcmp(previous.c_str(), ":") == 0)
			{
				while (strcmp(token.c_str(), ")") != 0)
				{
					result.append(token);
					iss >> token;
				}
			}
			return result;
		}
		previous = token;
	}
	return "";
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
	std::cout << "RUN TIME : " << run_time_ << std::endl;
	std::cout << "" << std::endl;
}
bool find_name_format(std::string line) {
	return line.find("PART") != std::string::npos && line.find("NAME") != std::string::npos;
}
bool find_tool_name(std::string line)
{
	return line.find("TOOL") != std::string::npos && line.find("NAME") != std::string::npos;
}
bool find_tool_run_time(std::string line)
{
	return line.find("GC") != std::string::npos;
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

File_info* Handler::process_file_content(char* filename)
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
			if (find_tool_run_time(line))
			{
				std::string run_time = retrieve_run_time(line, "GC");
				info->setRunTime(run_time);
			}
		}
	}
	file.close();
	return info;


}
void Handler::setPath(std::string path)
{
	path_ = path;
}
void Handler::read_directory(const char* path)
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
			sprintf(d_path, "%s/%s",path, buffer);
			std::string::size_type index;
			std::string filename(d_path);
			index = filename.rfind('.');
			if (index != std::string::npos)
			{
				std::string extension = filename.substr(index + 1);
				if (strcmp(extension.c_str(), "tap") == 0)
				{
					auto info = process_file_content(d_path);
					informations.push_back(info);
				}
			}
		}
		else if (dir->d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 && work_on_sub_folder) {
			char d_path[400];	
			sprintf(d_path, "%s/%s", path, dir->d_name);
			read_directory(d_path);
		}
	}
	closedir(directory);
}
