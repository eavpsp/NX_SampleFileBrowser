#include <stdint.h>
#include <switch.h>
#include <dirent.h>
#include <cstdio>
#include <string>
typedef struct {
    char dirName[500][256];
    int filetype[500][1];
} directoryData;
enum FileMode { //update to feature each func ie copy cut paste
    DEFAULT,
    MOVE,
    EDIT,
};
typedef struct {
    char fileNames[50][256];
    int fileCount;
    char pathTot;
} nxPathData;
extern nxPathData pathData;
extern FileMode currentFileMode;
extern int showMadeDiag;
extern directoryData dirData;
extern char currentDir;
extern char prevDir;
void OpenDirHan(char* path);
int checkDirExistence(const char* path);
void MkDirWithName(const char* dirName);
void get_current_directory();
void get_current_directory_minus();
FileMode CurrentFileMode();
void SetFileMode(FileMode mode);
void open_files_with_extension(const char* directory_path, const char* extension);
void delete_file(const char* path);
void cut_file(const char* old_path, const char* new_path);
void copy_file(const char* old_path, const char* new_path);
void rename_file(const char* old_path, const char* new_path);
int BootNCA(std::string path);//WIP
//Return Path & Boot NRO / NCA

//Track back button draws