#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>


void printFileInfo(const char *filepath, const char *filename){
    char fullFilePath[1024];
    struct stat fileStat;

    snprintf(fullFilePath, sizeof(fullFilePath), "%s/%s", filepath, filename);
    
    if (stat(fullFilePath, &fileStat) < 0) {
        perror("stat");
        return;
    }

    char *fileType;
    if (S_ISREG(fileStat.st_mode)) {
        fileType = "Regular File";
    } else if (S_ISDIR(fileStat.st_mode)) {
        fileType = "Folder";
    } else if (S_ISLNK(fileStat.st_mode)) {
        fileType = "Symbolic Link";
    } else if (S_ISCHR(fileStat.st_mode)) {
        fileType = "Character Device";
    } else if (S_ISBLK(fileStat.st_mode)) {
        fileType = "Block Device";
    } else if (S_ISFIFO(fileStat.st_mode)) {
        fileType = "FIFO";
    } else if (S_ISSOCK(fileStat.st_mode)) {
        fileType = "Socket";
    } else {
        fileType = "Unknown Type";
    }

    struct passwd *pw = getpwuid(fileStat.st_uid);
    if (pw == NULL) {
        perror("getpwuid");
        return;
    }
    const char *owner = pw->pw_name;
    if( owner == NULL) {
        owner = "Unknown Owner";
    }

    const char *founder = strrchr(filepath, '/');
    if (founder == NULL) {
        founder = filepath; 
    } else {
        founder++; 
    }
    if (strcmp(founder, filename) == 0) {
        founder = "Current Directory";
    }
    else {
        founder = "Parent Directory";
    }
    
    char createdTime[20];
    struct tm *tm_info_1 = localtime(&fileStat.st_ctime);
    if (tm_info_1 == NULL) {
        perror("localtime");
        return;
    }
    strftime(createdTime, sizeof(createdTime), "%Y-%m-%d %H:%M:%S", tm_info_1);

    char modifiedTime[20];
    struct tm *tm_info_2 = localtime(&fileStat.st_mtime);
    if (tm_info_2 == NULL) {
        perror("localtime");
        return;
    }
    strftime(modifiedTime, sizeof(modifiedTime), "%Y-%m-%d %H:%M:%S", tm_info_2);

    char accessTime[20];
    struct tm *tm_info_3 = localtime(&fileStat.st_atime);
    if (tm_info_3 == NULL) {
        perror("localtime");
        return;
    }
    strftime(accessTime, sizeof(accessTime), "%Y-%m-%d %H:%M:%S", tm_info_3);


    printf("File name: %s | Type: %s | Founder: %s | Owner: %s | Size: %lld bytes | Created Time: %s | Last Modified: %s | Last Accessed: %s\n\n",
        filename, fileType, founder, owner, (long long)fileStat.st_size, createdTime, modifiedTime, accessTime);
}

int main(int argc, char *argv[]){
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *directory = argv[1];
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printFileInfo(directory, entry->d_name);
        }
    }

    closedir(dir);
    return EXIT_SUCCESS;
}