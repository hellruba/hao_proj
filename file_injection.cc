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
bool FileInjection::inject_data_automatically(GtkWidget* window)
{
	dialog = gtk_dialog_new_with_buttons("Text to insert", GTK_WINDOW(window),
			GTK_DIALOG_MODAL,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	buffer = gtk_text_buffer_new(NULL);
	textZone = gtk_text_view_new_with_buffer(buffer);
	scrollbar = gtk_scrolled_window_new(NULL, NULL);

	gtk_container_add(GTK_CONTAINER(scrollbar), textZone);
	auto dialogBox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	gtk_box_pack_start(GTK_BOX(dialog), scrollbar, TRUE, TRUE, 5);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_AUTOMATIC,
		GTK_POLICY_AUTOMATIC);
	bool inject = true;
	gtk_widget_show_all(dialog);
	switch(gtk_dialog_run(GTK_DIALOG(dialog)))
	{
		case GTK_RESPONSE_OK:
			inject = true;
			break;
		case GTK_RESPONSE_CANCEL:
		case GTK_RESPONSE_NONE:
		default:
			inject = false;
			break;
	}
	gtk_widget_destroy(dialog);
	if (inject)
	{

		GtkTextIter start;
		GtkTextIter end;
		gtk_text_buffer_get_start_iter(buffer, &start);
		gtk_text_buffer_get_end_iter(buffer, &end);
		auto txt = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
	
		size_t nblines = 0;
		std::fstream read_file;
		std::string line;
		read_file.open(filename, std::ios::out | std::ios::in);
		std::vector<std::string> stored_lines;
		while (std::getline(read_file, line))
		{
			nblines += 1;
			stored_lines.push_back(line);
		}
		read_file.close();
		std::string lines = std::string("The text has ").append(std::to_string(nblines)).append(" lines, every how many lines do you want to add the text?");
		dialog = gtk_dialog_new_with_buttons(lines.c_str(), GTK_WINDOW(window),
				GTK_DIALOG_MODAL,
				GTK_STOCK_OK, GTK_RESPONSE_OK,
				GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				NULL);
		entry = gtk_entry_new();
		gtk_entry_set_text(GTK_ENTRY(entry), "Enter every which number of line");
		auto dialogBox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
		gtk_box_pack_start(GTK_BOX(dialogBox), entry, TRUE, FALSE, 0);
		gtk_widget_show_all(dialog);
		std::string xLines;
		switch(gtk_dialog_run(GTK_DIALOG(dialog)))
		{
			case GTK_RESPONSE_OK:
				xLines = std::string(gtk_entry_get_text(GTK_ENTRY(entry)));
			case GTK_RESPONSE_CANCEL:
			case GTK_RESPONSE_NONE:
			default:
				break;
		}
		gtk_widget_destroy(dialog);
		size_t everyXlines = std::stoi(xLines);
		std::fstream write_file;
		std::string newline;
		write_file.open(filename, std::ios::out | std::ios::in);
		size_t i = 0;
		while (i < nblines)
		{
			if (i % everyXlines == 0)
			{
				write_file << txt << std::endl;
			}
			write_file << stored_lines.at(i) << std::endl;
			i++;
		}
		write_file.close();
	}
}
bool FileInjection::inject_data_manually(GtkWidget* window)
{
	dialog = gtk_dialog_new_with_buttons("Insert Text to add", GTK_WINDOW(window),
			GTK_DIALOG_MODAL,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	buffer = gtk_text_buffer_new(NULL);
	textZone = gtk_text_view_new_with_buffer(buffer);
	GtkTextIter end;
	gtk_text_buffer_get_end_iter(buffer, &end);
	std::fstream read_file;
	std::string line;
	read_file.open(filename, std::ios::out | std::ios::in);
	while  (std::getline(read_file, line))
	{
		gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer), &end, line.c_str(), line.size());
		gtk_text_buffer_get_end_iter(buffer, &end);
	}
	read_file.close();

	scrollbar = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(scrollbar), textZone);
	auto dialogBox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	gtk_box_pack_start(GTK_BOX(dialogBox), scrollbar, TRUE, TRUE, 5);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_AUTOMATIC,
			GTK_POLICY_AUTOMATIC);

	gtk_widget_show_all(dialog);
	bool inject = true;
	switch(gtk_dialog_run(GTK_DIALOG(dialog)))
	{
		case GTK_RESPONSE_OK:
			inject = true;
			break;
		case GTK_RESPONSE_CANCEL:
		case GTK_RESPONSE_NONE:
		default:
			inject = false;
			break;
	}
	if (inject) {
		GtkTextIter startBuffer;
		GtkTextIter endBuffer;
		gtk_text_buffer_get_start_iter(buffer, &startBuffer);
		gtk_text_buffer_get_end_iter(buffer, &endBuffer);
		auto txt = gtk_text_buffer_get_text(buffer, &startBuffer, &endBuffer, FALSE);
		std::cout << std::string(txt) << std::endl;
		std::fstream updateFile;
		updateFile.open(filename, std::ios::out | std::ios::in);
		updateFile << txt << std::endl;
		updateFile.close();
	}
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
