#include "filedialogue.hpp"

using std::string, std::vector;

// Opens a choose-file dialogue in the default directory
string openFileDialogue()
{
    return openFileDialogue("", {});
}
// Opens a choose-dialogue with a specific directory
string openFileDialogue(const string& start_path)
{
    return openFileDialogue(start_path, {});
}



// Words don't describe my mild to slightly-more-than-mild disdain for the WIN32 API
#ifdef _WIN32
// Opens a choose-file dialogue with a specific directory and extension(s)
string openFileDialogue(const string& start_path, const vector<string>& extentions)
{
    // My compiler cannot handle an empty vector<string>. Treat a single element
    // of "" as an empty vector to handle this.
    bool extensions_empty = extentions.empty() || (extentions.size() == 1 && extentions[0] == "");

    string path{};
    // Check for default values in start_path
    if(start_path.empty())
    {
        // I'm aware SHGetFolderPath is the *proper* way of doing this,
        // but look me dead in the eye and tell me that it's good.
        path = getenv("USERPROFILE");
        path.append("/Documents");
    } else {
        path = start_path;
    }

    // This portion was written by ChatGPT because dear lord what the fuck
    // i used the microsoft product to destroy(?) the microsoft product
    // The reference doesn't work. Hopefully this code does.
    TCHAR szFile[MAX_PATH];
    ZeroMemory(szFile, sizeof(szFile));

    TCHAR szFilter[MAX_PATH];
    ZeroMemory(szFilter, sizeof(szFilter));

    // Build the filter string from the specified extensions
    string filter_string;

    if(extensions_empty)
    {
        filter_string += "All Files (*.*)\0*.*\0"; // clang-tidy hates this
    } else {
        for(size_t i = 0; i < extentions.size(); i++)
        {
            filter_string += extentions[i] + " files (*." + extentions[i] + ") ";
        }
    }

    // Copy the filter string to a TCHAR buffer
    size_t len = filter_string.length();
    for(size_t i = 0; i <= len; i++)
    {
        szFilter[i] = filter_string[i];
    }

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrInitialDir = path.c_str();
    ofn.lpstrFilter = szFilter;
    ofn.lpstrTitle = _T("Open File");
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if(GetOpenFileName(&ofn))
    {
        return string(szFile);
    } else {
        return "";
    }
}

// NOTE: Linux implementations are currently untested
#elif __linux__

// Opens a choose-file dialogue with a specific directory and extension(s)
string openFileDialogue(const string& start_path, const vector<string>& extentions)
{
    string file_path;

    // Create a file chooser dialog
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
                                                    NULL,
                                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    "_Cancel",
                                                    GTK_RESPONSE_CANCEL,
                                                    "_Open",
                                                    GTK_RESPONSE_ACCEPT,
                                                    NULL);

    // Set the starting directory, if provided
    if (!start_path.empty())
    {
        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), start_path.c_str());
    }

    // Run the dialog and get the response
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        file_path = filename;
        g_free(filename);
    }

    // Destroy the dialog
    gtk_widget_destroy(dialog);

    return file_path;
}

#endif