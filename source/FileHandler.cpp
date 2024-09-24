#include <FileHandler.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

FileMode CurrentFileMode()
{
    return currentFileMode;
}

void SetFileMode(FileMode mode){
    currentFileMode = mode;
}

//func to format path string for full path return
void get_current_directory() {
    //get path file count and start from beging loop to format string and increase counter
    //ex count = 5 start at 0 path = path + name[0] + / -> path(name/)+name[1] + / ->name/name/
    for(int i = 0; i < pathData.fileCount ; i++)
    {
       if(i == 0){
           sprintf(&pathData.pathTot, "");

        }
        else{
            sprintf(&pathData.pathTot, "%s%s/", &pathData.pathTot, pathData.fileNames[i]);//first file at index 1[1st file name] ---0 should be "/"

        }
    }
   
   
}
void get_current_directory_minus() {
    //get path file count and start from beging loop to format string and increase counter
    //ex count = 5 start at 0 path = path + name[0] + / -> path(name/)+name[1] + / ->name/name/
    for(int i = 0; i < pathData.fileCount -1 ; i++)
    {
       if(i == 0){
           sprintf(&pathData.pathTot, "");

        }
        else{
            sprintf(&pathData.pathTot, "%s%s/", &pathData.pathTot, pathData.fileNames[i]);//first file at index 1[1st file name] ---0 should be "/"

        }
    }
   
   
}

int checkDirExistence(const char* path) {
    struct stat st = {0};

    if (stat(path, &st) == -1) {
        return 0;
    }
    return 1;
}

void MkDirWithName(const char* dirName){
      if (mkdir(dirName, 0777) != 0) 
      {
       //load pop up
       
            return;
        }
}

void rename_file(const char* old_path, const char* new_path) {
    if (rename(old_path, new_path) != 0) {
        
    }
}

void copy_file(const char* old_path, const char* new_path) {
    struct stat file_stat;
    if (stat(old_path, &file_stat) != 0) {
        
    }
    FILE* input_file = fopen(old_path, "rb");
    if (input_file == NULL) {
        
    }
    FILE* output_file = fopen(new_path, "wb");
    if (output_file == NULL) {
        
    }
    char buffer[BUFSIZ];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFSIZ, input_file)) > 0) {
        fwrite(buffer, 1, bytes_read, output_file);
    }
    fclose(input_file);
    fclose(output_file);
    chmod(new_path, file_stat.st_mode);
}

void cut_file(const char* old_path, const char* new_path) {
    rename_file(old_path, new_path);
    if (remove(old_path) != 0) {
        
    }
}

void delete_file(const char* path) {
    if (remove(path) != 0) {
       
    }
}

void open_files_with_extension(const char* directory_path, const char* extension) {
    DIR* directory = opendir(directory_path);
    if (directory == NULL) {
      
    }
    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_type == DT_REG && strstr(entry->d_name, extension) != NULL) {
            char file_path[PATH_MAX];
            snprintf(file_path, PATH_MAX, "%s/%s", directory_path, entry->d_name);
          //  printf("Opening file: %s\n", file_path);
            // do something with the file, like opening it with a text editor
        }
    }
    closedir(directory);
}

int BootNCA(const char* nca_path){  
    FsFile nca_file;
    FsFileSystem sdmc_fs;
    Result result = fsOpenSdCardFileSystem(&sdmc_fs);
    if (R_FAILED(result)) {
        printf("Failed to open NCA file: 0x%x\n", result);
        return -1;
    }
    result = fsFsOpenFile(&sdmc_fs, nca_path, 1, &nca_file);
    if (R_FAILED(result)) {
        printf("Failed to open NCA file: 0x%x\n", result);
        return -1;
    }

    // get the size of the NCA file
    s64 nca_size;
    result = fsFileGetSize(&nca_file, &nca_size);
    if (R_FAILED(result)) {
        printf("Failed to get NCA file size: 0x%x\n", result);
        fsFileClose(&nca_file);
        return -1;
    }

    // allocate memory for the NCA file contents
    void* nca_buffer = malloc(nca_size);
    if (nca_buffer == NULL) {
        printf("Failed to allocate memory for NCA buffer\n");
        fsFileClose(&nca_file);
        return -1;
    }

    // read the NCA file into memory
    u64 bytes_read;
    result = fsFileRead(&nca_file, 0, nca_buffer, nca_size, 0, &bytes_read);
    if (R_FAILED(result)) {
        printf("Failed to read NCA file: 0x%x\n", result);
        free(nca_buffer);
        fsFileClose(&nca_file);
        return -1;
    }

    // close the NCA file
    fsFileClose(&nca_file);

    // cast the NCA buffer to a function pointer and call it
    void (*nca_function)(void) = (void (*)(void))nca_buffer;
    nca_function();

    // release the memory
    free(nca_buffer);

    fsdevUnmountDevice("sdmc");

    return 0;
}