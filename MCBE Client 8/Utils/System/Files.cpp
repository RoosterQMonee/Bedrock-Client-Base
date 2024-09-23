#include <Utils/System/Files.h>

#include <windows.h>


std::string FileUtils::GetRoamingPath()
{
    char* path = nullptr;
    size_t length;

    _dupenv_s(&path, &length, "appdata");

    if (path == nullptr)
        return "";

    return std::string(path) + "\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState";
};

void FileUtils::CreateDirectoryP(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directory(path);
    }
}

void FileUtils::DeleteFileF(const std::string& path) {
    if (std::filesystem::exists(path)) {
        std::filesystem::remove(path);
    }
}

bool FileUtils::FileExists(std::string path) {
    std::ifstream f(path.c_str());
    return f.good();
}

void FileUtils::InitFileSystem() {
    try {
        CreateDirectoryP(AssetsFolder);
        CreateDirectoryP(ConfigFolder);
    }
    catch (std::exception& e) {

    }
}


std::string AssetsFolder = FileUtils::GetRoamingPath() + "\\Assets";
std::string ConfigFolder = FileUtils::GetRoamingPath() + "\\Config";