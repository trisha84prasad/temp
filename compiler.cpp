#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cstdio>
namespace fs = std::filesystem;
void listDirectory(const fs::path& path) {
    std::cout << "Contents of " << path << ":\n";
    for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << (entry.is_directory() ? "[DIR] " : "[FILE] ") << entry.path().filename().string() << '\n';
    }
}
void changeDirectory(fs::path& currentPath, const std::string& newDir) {
    fs::path newPath = currentPath / newDir;
    if (fs::exists(newPath) && fs::is_directory(newPath)) {
        currentPath = fs::canonical(newPath);
        std::cout << "Changed directory to " << currentPath << '\n';
    } else {
        std::cout << "Directory does not exist: " << newDir << '\n';
    }
}
void viewFile(const fs::path& filePath) {
    if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
        std::ifstream file(filePath);
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << '\n';
        }
    } else {
        std::cout << "File does not exist: " << filePath << '\n';
    }
}
void createDirectory(const fs::path& path, const std::string& dirName) {
    fs::path newDir = path / dirName;
    if (fs::create_directory(newDir)) {
        std::cout << "Directory created: " << newDir << '\n';
    } else {
        std::cout << "Failed to create directory: " << newDir << '\n';
    }
}
void copyFile(const fs::path& source, const fs::path& destination) {
    try {
        fs::copy(source, destination);
        std::cout << "File copied from " << source << " to " << destination << '\n';
    } catch (fs::filesystem_error& e) {
        std::cout << "Error copying file: " << e.what() << '\n';
    }
}
void moveFile(const fs::path& source, const fs::path& destination) {
    try {
        fs::rename(source, destination);
        std::cout << "File moved from " << source << " to " << destination << '\n';
    } catch (fs::filesystem_error& e) {
        std::cout << "Error moving file: " << e.what() << '\n';
    }
}
void printHelp() {
    std::cout << "Commands:\n";
    std::cout << "  ls                List directory contents\n";
    std::cout << "  cd <dir>          Change directory\n";
    std::cout << "  view <file>       View file contents\n";
    std::cout << "  mkdir <dir>       Create a new directory\n";
    std::cout << "  cp <src> <dest>   Copy file\n";
    std::cout << "  mv <src> <dest>   Move file\n";
    std::cout << "  exit              Exit the file manager\n";
}

int main() {
    fs::path currentPath = fs::current_path();
    std::string command;
    std::cout << "Simple Command Line File Manager\n";
    printHelp();

    while (true) {
        std::cout << currentPath << "> ";
        std::getline(std::cin, command);
        std::istringstream iss(command);
        std::string cmd, arg1, arg2;
        iss >> cmd >> arg1 >> arg2;

        if (cmd == "ls") {
            listDirectory(currentPath);
        } else if (cmd == "cd") {
            changeDirectory(currentPath, arg1);
        } else if (cmd == "view") {
            viewFile(currentPath / arg1);
        } else if (cmd == "mkdir") {
            createDirectory(currentPath, arg1);
        } else if (cmd == "cp") {
            copyFile(currentPath / arg1, currentPath / arg2);
        } else if (cmd == "mv") {
            moveFile(currentPath / arg1, currentPath / arg2);
        } else if (cmd == "exit") {
            break;
        } else {
            printHelp();
        }
    }

    return 0;
}
