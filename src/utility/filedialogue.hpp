// Contains relatively easy to use methods to call for a file dialogue, and return
// a file path as a string.

#include "../core.hpp"

// Opens a choose-file dialogue in the default directory
std::string openFileDialogue();
// Opens a choose-dialogue with a specific directory
std::string openFileDialogue(const std::string& start_path);
// Opens a choose-file dialogue with a specific directory and extension(s)
std::string openFileDialogue(const std::string& start_path,
                             const std::string& filter_name,
                             const std::vector<std::string>& extentions);