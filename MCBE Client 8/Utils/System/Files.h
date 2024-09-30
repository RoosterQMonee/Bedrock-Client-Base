#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <chrono>


extern std::string AssetsFolder;
extern std::string ConfigFolder;


class Files {
public:
	static std::string GetRoamingPath();
	static void InitFileSystem();
    static void CreateDirectoryP(const std::string& path);
    static void DeleteFileF(const std::string& path);
    static bool FileExists(std::string path);
};