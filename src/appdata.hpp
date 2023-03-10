#ifndef APPDATA_HPP
#define APPDATA_HPP

#include <fstream>
#include <sstream>

class AppData {
public:
    AppData();
    ~AppData();

    const char* Resources();

    int Width();
    void Width(int);

    int Height();
    void Height(int);
private:
    void CreateSettingsFile();
    unsigned int FindWidth();
    unsigned int FindHeight();
    const char* FindResources();

    std::fstream file;
    unsigned int width;
    unsigned int height; 
    const char* resources;
};

#endif /* APPDATA_HPP */