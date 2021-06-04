#include <iostream>
#include <stdlib.h>
#include <stdbool.h>
#include <string>
#include <sstream>
#include <fstream>
#include <stdbool.h>
#include "file_update.hh"

FileUpdate::FileUpdate() {}

std::string FileUpdate::getPreviousValue() const
{
	return previous_value;
}
void FileUpdate::setPreviousValue(std::string value)
{
	previous_value = value;
}

std::string FileUpdate::getNewValue() const
{
	return new_value;
}
void FileUpdate::setNewValue(std::string value)
{
	new_value = value;
}

std::string FileUpdate::getFileName() const
{
	return filename;
}
void FileUpdate::setFileName(std::string f)
{
	filename = f;
}
bool FileUpdate::run_dialog_file(GtkWidget* window)
{
	file_dialog = gtk_file_chooser_dialog_new("Open a file", NULL,
			GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
			NULL);

	bool isValidFile = true;
	switch (gtk_dialog_run(GTK_DIALOG(file_dialog)))
	{
		case GTK_RESPONSE_ACCEPT:
			filename = std::string(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_dialog)));
			std::string::size_type index;
			index = filename.rfind('.');
			if (index != std::string::npos)
			{
				std::string extension = filename.substr(index + 1);
				if (strcmp(extension.c_str(), "tap") != 0)
				{
					isValidFile = false;
				}
			}
			break;
		case GTK_RESPONSE_CANCEL:
		default:
			break;
	}
	gtk_widget_destroy(file_dialog);
	return isValidFile;
}
bool FileUpdate::run_dialog_values(GtkWidget* window)
{
	bool validate = true;
	for (int i = 0; i < 3; i++)
	{
		values_dialog = gtk_dialog_new_with_buttons("Replace file content", GTK_WINDOW(window),
				GTK_DIALOG_MODAL,
				GTK_STOCK_OK, GTK_RESPONSE_OK,
				GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				NULL
				);
		entry = gtk_entry_new();
		if (i == 0)
		{
			gtk_entry_set_text(GTK_ENTRY(entry), "Enter value you want to replace");
		}
		if (i == 1)
		{
			gtk_entry_set_text(GTK_ENTRY(entry), "Enter new value");
		}
		if (i == 2)
		{
			gtk_entry_set_text(GTK_ENTRY(entry), "Enter end of file name text");
		}
		gtk_box_pack_start(GTK_BOX(GTK_DIALOG(values_dialog)->vbox),
				entry, TRUE, FALSE, 0);
		gtk_widget_show_all(values_dialog);
		switch (gtk_dialog_run(GTK_DIALOG(values_dialog)))
		{
			case GTK_RESPONSE_OK:
				if (i == 0)
				{
					previous_value = std::string(gtk_entry_get_text(GTK_ENTRY(entry)));
				}
				else if (i == 1)
				{
					new_value = std::string(gtk_entry_get_text(GTK_ENTRY(entry)));
				}
				else
				{
					text_filename = std::string(gtk_entry_get_text(GTK_ENTRY(entry)));
				}
				break;
			case GTK_RESPONSE_CANCEL:
			case GTK_RESPONSE_NONE:
			default:
				validate = false;
				break;
		}
		gtk_widget_destroy(values_dialog);
		if (!validate)
		{
			return validate;
		}
	}
	return validate;
}
bool contains_value(std::string line, std::string value)
{
	return line.find(value) != std::string::npos;
}

void FileUpdate::replace()
{
	std::string::size_type extension_index;
	std::string new_filename;
	extension_index = filename.rfind('.');
	if (extension_index != std::string::npos)
	{
		/* create new path file */
		new_filename = filename.substr(0, extension_index);
		new_filename.append("-");
		new_filename.append(text_filename);
		new_filename.append(".tap");
		
		std::fstream new_file;
		std::fstream old_file;
		
		new_file.open(new_filename, std::ios::out);
		if (!new_file.is_open())
		{
			std::cout << "Failed to create new file" << std::endl;
		}
		old_file.open(filename, std::ios::out | std::ios::in);
		if (!old_file.is_open())
		{
			std::cout << "Failed to open file" << std::endl;
			new_file.close();
			return;
		}
		std::string line;
		std::string newline;
		while (std::getline(old_file, line))
		{
			newline = line;
			while (contains_value(newline, previous_value))
			{
				/* replace occurence of old value by the new value */
				std::string::size_type index;
				index = newline.find(previous_value);
				newline.replace(index, previous_value.size(), new_value);
			}
			new_file << newline << std::endl;
		}
		new_file.close();
		old_file.close();
	}	
}
