//
// Created by Michał Kawa on 20/03/2024.
//
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#define DIR_PATH "./"

int main(void){
    DIR* FD;
    struct dirent* fd;
    struct stat buf;

    if ((FD = opendir(DIR_PATH)) == NULL){
        fprintf(stderr, "Error: Cannot open directory: %s\n", DIR_PATH);
        return 1;
    }

    long long total_size = 0;
    while ((fd = readdir(FD))){
        stat(fd->d_name, &buf);
        if(S_ISDIR(buf.st_mode))
            continue;
        printf("%lld %s\n", (buf.st_size), fd->d_name);
        total_size += buf.st_size;
    }
    printf("total size: %lld\n", total_size);

    if (closedir(FD) == -1){
        fprintf(stderr, "Error: Cannot close directory: %s\n", DIR_PATH);
    }
    return 0;
}