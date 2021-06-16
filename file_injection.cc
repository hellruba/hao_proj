#include <iostream>
#include <stdlib.h>
#include <stdbool.h>
#include <string>
#include <sstream>
#include <fstream>
#include "file_injection.hh"

FileInjection::FileInjection() {}

void FileInjection::setCoordX(std::string coordX)
{
	coordX_ = coordX;
}
std::string FileInjection::getCoordX() const
{
	return coordX_;
}

void FileInjection::setCoordY(std::string coordY)
{
	coordY_ = coordY;
}
std::string FileInjection::getCoordY() const
{
	return coordY_;
}

void FileInjection::setTextZone(std::string text)
{
	text_ = text;
}
std::string FileInjection::getTextZone() const
{
	return text_;
}
bool FileInjection::run_dialog_injection_file_select()
{
	bool fileSelected = true;
  	dialog = gtk_file_chooser_dialog_new("Open a file", NULL,
	GTK_FILE_CHOOSER_ACTION_OPEN,
	GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
	NULL);
	switch (gtk_dialog_run(GTK_DIALOG(dialog)))
	{
		case GTK_RESPONSE_ACCEPT:
			filename = std::string(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
			break;
		case GTK_RESPONSE_CANCEL:
		default:
			fileSelected = false;
			break;
	}
	gtk_widget_destroy(dialog);
	return fileSelected;
}
bool FileInjection::inject_data_automatically()
{

}
bool FileInjection::inject_data_manually(GtkWidget* window)
{
	dialog = gtk_dialog_new_with_buttons("Insert Text to add", GTK_WINDOW(window),
			GTK_DIALOG_MODAL,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	textZone = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), textZone, TRUE, FALSE, 0);
	gtk_widget_show_all(dialog);
	switch(gtk_dialog_run(GTK_DIALOG(dialog)))
	{
		case GTK_RESPONSE_OK:
		case GTK_RESPONSE_CANCEL:
		case GTK_RESPONSE_NONE:
		default:
			break;
	}
	GtkTextIter start;
	GtkTextIter end;
	auto buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textZone));
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);
	auto t = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
	std::cout << t << std::endl;
	std::fstream file;
	file.open("ttt", std::ios::out);
	file << t;
	file.close();
	gtk_widget_destroy(dialog);
	return true;
}
bool FileInjection::manual_or_auto(GtkWidget* window)
{
	bool isManual = true;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window),
			GTK_DIALOG_MODAL,
			GTK_MESSAGE_QUESTION,
			GTK_BUTTONS_YES_NO,
			"Do you want to inject line automatically every X lines?");
	switch(gtk_dialog_run(GTK_DIALOG(dialog)))
	{
		case GTK_RESPONSE_YES:
			isManual = false;
			break;
		case GTK_RESPONSE_NO:
			isManual = true;
			break;
	}
	gtk_widget_destroy(dialog);
	return isManual;
}
