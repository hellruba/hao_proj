#include <iostream>
#include <stdio.h>
#include <string>
#include "handler.hh"
#include <gtk/gtk.h>

void run_display(Handler* handler)
{
	handler->init_window();
	handler->init_table();
	handler->init_grid();
	handler->init_previous_button();
	handler->init_next_button();
	handler->init_menu();
	handler->run();
}
int main(int argc, char* argv[])
{
	auto handler = new Handler();

	run_display(handler);

}
