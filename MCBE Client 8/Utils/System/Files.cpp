#include <Utils/System/Files.h>

#include <windows.h>


std::string Files::GetRoamingPath()
{
    char* path = nullptr;
    size_t length;

    _dupenv_s(&path, &length, "appdata");

    if (path == nullptr)
        return "";

    return std::string(path) + "\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState";
};

void Files::CreateDirectoryP(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directory(path);
    }
}

void Files::DeleteFileF(const std::string& path) {
    if (std::filesystem::exists(path)) {
        std::filesystem::remove(path);
    }
}

bool Files::FileExists(std::string path) {
    std::ifstream f(path.c_str());
    return f.good();
}

void Files::InitFileSystem() {
    try {
        CreateDirectoryP(AssetsFolder);
        CreateDirectoryP(ConfigFolder);
    }
    catch (std::exception& e) {

    }
}


std::string AssetsFolder = Files::GetRoamingPath() + "\\Assets";
std::string ConfigFolder = Files::GetRoamingPath() + "\\Config";