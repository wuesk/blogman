#include "views.hpp"

#include <fstream>
#include <filesystem>

namespace {
    const std::filesystem::path viewsFilePath = "data/views.txt";
}

int loadViews()
{
    std::ifstream file(viewsFilePath);

    int views = 0;

    if (!file.is_open())
    {
        return views;
    }

    file >> views;

    return views;
}

bool saveViews(int views)
{
    //Create directory if doesn't exist
    std::filesystem::create_directories(viewsFilePath.parent_path());
    
    std::ofstream file(viewsFilePath);

    if (!file.is_open()) {
        return false;
    }

    file << views;

    return true;
}