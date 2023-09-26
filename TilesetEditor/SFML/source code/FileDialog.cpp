#include "stdafx.h"
#include "FileDialog.h"
#include <Windows.h>

FileDialog::FileDialog()
{

}

void FileDialog::addFilter(const std::string& name, const std::string& filter)
{
    m_filter += name + '\0';
    m_filter += filter + '\0';
}

std::string FileDialog::saveDialog()
{
    return open(false);
}

std::string FileDialog::openDialog()
{
    return open(true);
}

std::string FileDialog::openFolderDialog()
{
    addFilter("Folders", ".");
    std::string FolderPath = open(false, "Select a folder");

    if (FolderPath.empty()) return FolderPath;

    size_t index = FolderPath.find_last_of('\\'); //remove the file name if a file was selected so that we are left with the folder path
    return FolderPath.substr(0, index + 1);
}

std::string FileDialog::open(bool filePathMustExist, const char* previewText)
{
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = { 0 };
    strcpy_s(fileName, previewText);

    DWORD Flags = OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;

    if (filePathMustExist) Flags |= OFN_FILEMUSTEXIST;

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = m_filter.c_str();
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = Flags;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        std::string filePath = fileName;
        std::string extension = getSelectedExtension(&ofn);

        if (extension.length() > 0 && filePath.find('.') == std::string::npos) //if there is no extension at end and the current filter extension
        {
            filePath += extension;
        }
        return filePath;
    }

    return std::string();
}

std::string FileDialog::getSelectedExtension(tagOFNA* dialog)
{
    size_t currentFilterIndex = 0;
    const char* current = dialog->lpstrFilter;
    size_t count = 0;
    //iterate over the lpstrFilter string until the end
    while (*current != '\0')
    {
        count++;

        //check if the count is even because every even string is the extension and the string before it is its category name
        if (count % 2 == 0)
        {
            currentFilterIndex++; // Increment the filter index.

            //if the index of the filter matches the filter index break means that we have found the right string with the extention
            if (currentFilterIndex == dialog->nFilterIndex)
            {
                break;
            }
        }

        //move the current pointer to the next filter
        current += strlen(current) + 1;
    }

    //create an std::string named 'result' from the current position of 'current'.
    std::string result(current);

    //find the last semicolon (';') in the result string.
    size_t lastIndex = result.find(';');

    //if no semicolon is found, set lastIndex to the length of 'result'.
    if (lastIndex == std::string::npos)
    {
        lastIndex = result.length();
    }

    //return a substring of result starting from the second character index 1
    //and ending at lastIndex - 1
    //this should extract the file extension.
    return result.substr(1, lastIndex - 1);
}

