// Functions/Includes that nearly all source files need access to.

#pragma once

#include <cstdint>
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <map>

#define FMT_HEADER_ONLY
#include <fmt/core.h>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h> // great names, microsoft
#include <tchar.h>

#elif __linux__
#include <gtk/gtk.h>
#endif
