#ifndef APPDATA_HPP
#define APPDATA_HPP

#include <fstream>
#include <sstream>

class AppData {
public:
    AppData();
    ~AppData();

    std::string& Resources();

    int Width();
    void Width(int);

    int Height();
    void Height(int);


private:
    void CreateSettingsFile();
    unsigned int FindWidth();
    unsigned int FindHeight();
    std::string FindResources();

    std::fstream file;
    unsigned int width;
    unsigned int height; 
    std::string resources;
};

#endif /* APPDATA_HPP */