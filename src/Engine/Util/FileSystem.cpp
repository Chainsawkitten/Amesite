#include "FileSystem.hpp"

#include <cstdlib>
#include <locale>
#include <sys/types.h>
#include <sys/stat.h>
#if defined(_WIN32) || defined(WIN32)
#include <direct.h>
#endif

namespace FileSystem {
#if defined(_WIN32) || defined(WIN32)
    // Windows
    const char DELIMITER = '\\';
#else
    // MacOS and Linux
    const char DELIMITER = '/';
#endif
    
    bool FileExists(const char* filename) {
#if defined(_WIN32) || defined(WIN32)
        // Windows
        struct _stat buf;
        int result = _stat(filename, &buf);
#else
        // MacOS and Linux
        struct stat buf;
        int result = stat(Name.c_str(), &buf);
#endif
        return result == 0;
    }
    
    std::string GetFileExtension(const std::string& filename) {
        for (std::string::size_type i=filename.length()-1; i>=0; i--) {
            if (filename[i] == '.') {
                std::string extension = filename.substr(i+1);
                
                // Convert to lower case.
                std::locale loc;
                for (std::string::size_type e=0; e<extension.length(); e++)
                    extension[e] = std::tolower(extension[e], loc);
                
                return extension;
            }
            
            if (filename[i] == '/' || filename[i] == DELIMITER)
                return "";
        }
        
        return "";
    }
    
    void CreateDirectory(const char* filename) {
#if defined(_WIN32) || defined(WIN32)
        // Windows
        _mkdir(filename);
#else
        // MacOS and Linux
        mkdir(filename, ACCESSPERMS);
#endif
    }
    
    std::string SavePath(const char* appName) {
        std::string path;
        
#if defined(_WIN32) || defined(WIN32)
        // Windows
        path = getenv("APPDATA");
        path += DELIMITER;
#elif __APPLE__
        // MacOS
        path = "~/Library/Application Support/";
#elif __linux__
        // Linux
        path = "~/.local/";
#endif
        
        path += appName;
        
        CreateDirectory(path.c_str());
        
        return path;
    }
    
    std::string SavePath(const char* appName, const char* filename) {
        std::string path = SavePath(appName);
        path += DELIMITER;
        path += filename;
        
        return path;
    }
}
