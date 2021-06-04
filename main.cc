#include <iostream>
#include <stdio.h>
#include <string>
#include "handler.hh"
#include <gtk/gtk.h>

void run_display(Handler* handler, const char* dir_name)
{
	handler->init_window(dir_name);
	handler->init_table();
	handler->init_grid();
	handler->init_previous_button();
	handler->init_next_button();
	handler->init_button_replace();
	handler->init_button_injection();
	handler->run();
}
int main(int argc, char* argv[])
{
	if (argc >= 2) {
		auto handler = new Handler();

		for (int i = 1; i < argc; i++)
		{
			std::string dir_name = argv[i];
			handler->setPath(dir_name); 
			if (dir_name.find('/') != std::string::npos) {
				int index_ = dir_name.find('/');
				dir_name[index_] = '\0';
			}
			handler->read_directory(dir_name.c_str());
			run_display(handler, dir_name.c_str());
		}
	}

}
