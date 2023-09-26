#pragma once

struct tagOFNA;
//this class helps you to open filedialogs for windows to save and open files
class FileDialog
{
public:
    //default conctructor
    FileDialog();
    //add a filter to the FileDialog
    //name is the name of the category in your file explorer
    //the filter is the files this categoy supports example "*.png;*.txt . . .
    void addFilter(const std::string& name, const std::string& filter);
    //waits for the user to select a filepath. The must not exist 
    //if the process wasnt successful return an empty string
    std::string saveDialog();
    //waits for the user to select a filepath. The must exist 
    //if the process wasnt successful return an empty string
    std::string openDialog();
    //waits for the user to select a folder and click open and returns the path where it is located at 
    std::string openFolderDialog();

private:
    //used to store the filter for the categories
    std::string m_filter;

private:
    //opens the filedialog. You can specify if the selected filePath must exist or not
    std::string open(bool filePathMustExist, const char* previewText = "\0");
    //this function returns the file extension for the current filter selected
    std::string getSelectedExtension(tagOFNA* dialog);
};

