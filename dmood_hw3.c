/*
Name: DINESH MOOD
BlazerId: dmood
Project #: Homework 3
To compile: gcc -o dmood_hw3 dmood_hw3.c
To run: ./dmood_hw3 <options> <filepath>
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int MaxDepth = -1, MaxLength = -1, Attr = 0;
char *Substr = "";
char *PATH = ".";
char *FILE_TYPE = "ALL";
char *F_CMD = NULL;
char *DIRECTORY_CMD = NULL;
char **DIR_F = NULL;

int search(char *path, int curDepth)
{
    DIR *dir;
    struct dirent *dirEntry;
    struct stat fileStat;

    dir = opendir(path);
    while (dirEntry = readdir(dir))
    {
        if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0)
            lstat(dirEntry->d_name, &fileStat);
        else
            continue;

        char *newPath = malloc(strlen(path) + strlen(dirEntry->d_name) + 2);
        strncpy(newPath, path, strlen(path) + 1);
        strcat(newPath, "/");
        strncat(newPath, dirEntry->d_name, strlen(dirEntry->d_name) + 1);

        if (MaxDepth == -1 || curDepth <= MaxDepth)
        {
            if (dirEntry->d_type == DT_DIR)
            {
                if (FILE_TYPE == "ALL" || FILE_TYPE == "DIR")
                {
                    int i = 0;
                    while (DIR_F[i] != NULL)
                    {
                        i++;
                    }
                    DIR_F[i] = malloc(strlen(newPath) + 1);
                    strncpy(DIR_F[i], newPath, strlen(newPath) + 1);

                    for (int i = 0; i < curDepth; i++)
                        printf("\t");

                    printf("%s\t", dirEntry->d_name);
                    if (Attr == 1)
                        printf("\t0\t");
                    else if (S_ISLNK(fileStat.st_mode))
                    {
                        char buf[1024];
                        int len = readlink(path, buf, sizeof(buf) - 1);
                        buf[len] = '\0';
                        char *name = strstr(buf, "/");
                        if (name == NULL)
                            name = path;
                        else
                            name++;
                        printf("(%s)\t", name);
                    }

                    if (Attr)
                    {
                        printf("\t0\t");
                        printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
                        printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
                        printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
                        printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
                        printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
                        printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
                        printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
                        printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
                        printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
                        printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
                        printf("\t%s", ctime(&fileStat.st_atime));
                    }
                    printf("\n");
                }
                search(newPath, curDepth + 1);
            }
            else if (strstr(dirEntry->d_name, Substr) && (MaxLength == -1 || fileStat.st_size <= MaxLength) && (FILE_TYPE == "ALL" || FILE_TYPE == "FILE"))
            {
                int i = 0;
                while (DIR_F[i] != NULL)
                {
                    i++;
                }
                DIR_F[i] = malloc(strlen(newPath) + 1);
                strncpy(DIR_F[i], newPath, strlen(newPath) + 1);

                for (int i = 0; i < curDepth; i++)
                    printf("\t");

                printf("%s\t", dirEntry->d_name);

                if (S_ISLNK(fileStat.st_mode))
                {
                    char buf[1024];
                    int len = readlink(path, buf, sizeof(buf) - 1);
                    buf[len] = '\0';
                    char *name = strstr(buf, "/");
                    if (name == NULL)
                        name = path;
                    else
                        name++;
                    printf("(%s)\t", name);
                }

                if (Attr)
                {
                    printf("\t%ld\t", fileStat.st_size);
                    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
                    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
                    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
                    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
                    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
                    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
                    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
                    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
                    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
                    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
                    printf("\t%s", ctime(&fileStat.st_atime));
                }
                printf("\n");
            }
        }
    }
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (strncmp(argv[i], "-s", 2) == 0)
        {
            MaxLength = atoi(argv[i + 1]);
            i++;
        }
        else if (strncmp(argv[i], "-f", 2) == 0)
        {
            Substr = argv[i + 1];
            MaxDepth = atoi(argv[i + 2]);
            i += 2;
        }
        else if (strncmp(argv[i], "-S", 2) == 0)
        {
            Attr = 1;
        }
        else if (strncmp(argv[i], "-t", 2) == 0)
        {
            if (strncmp(argv[i + 1], "d", 1) == 0)
                FILE_TYPE = "DIR";
            else if (strncmp(argv[i + 1], "f", 1) == 0)
                FILE_TYPE = "FILE";
            i++;
        }
        else if (strncmp(argv[i], "-e", 2) == 0)
        {
            F_CMD = argv[i + 1];
            i++;
        }
        else if (strncmp(argv[i], "-E", 2) == 0)
        {
            DIRECTORY_CMD = argv[i + 1];
            i++;
        }
        else
        {
            PATH = argv[i];
        }
    }
    DIR_F = malloc(sizeof(char *) * 100);
    for (int i = 0; i < 100; i++)
        DIR_F[i] = (void*)0;

    search(PATH, 0);

    if (DIRECTORY_CMD != NULL)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            char *args[100];
            int i = 0;
            char *token = strtok(DIRECTORY_CMD, " ");
            while (token != NULL)
            {
                args[i] = token;
                token = strtok(NULL, " ");
                i++;
            }
            args[i] = NULL;
            execvp(args[0], args);
        }
        else
        {
            wait(NULL);
        }
    }

    if (F_CMD != NULL)
    {
        int i = 0;
        while (DIR_F[i] != NULL)
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                char *args[100];
                int j = 0;
                char *token = strtok(F_CMD, " ");
                while (token != NULL)
                {
                    args[j] = token;
                    token = strtok(NULL, " ");
                    j++;
                }
                args[j] = DIR_F[i];
                args[j + 1] = NULL;
                execvp(args[0], args);
            }
            else
            {
                wait(NULL);
            }
            i++;
        }
    }
}
