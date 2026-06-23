#include <stdio.h>
#include <sys/types.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>


void ls_long(struct stat* file_info, char* file_name) {
    //Building the date
    char file_time[20]; //9 for the longest month september + "  " + day + " " + hour + "\0"
    strftime(file_time, 20, "%B  %d %R", localtime(&file_info->st_mtime));   
    
    //Permissions
    char perms[11]; //1 directory + 3 owner + 3 group + 3 others + "\0"
    perms[0] = S_ISDIR(file_info->st_mode) ? 'd' : '-';
    perms[1] = S_IRUSR & file_info->st_mode ? 'r' : '-';
    perms[2] = S_IWUSR & file_info->st_mode ? 'w' : '-';
    perms[3] = S_IXUSR & file_info->st_mode ? 'x' : '-';
    perms[4] = S_IRGRP & file_info->st_mode ? 'r' : '-';
    perms[5] = S_IWGRP & file_info->st_mode ? 'w' : '-';
    perms[6] = S_IXGRP & file_info->st_mode ? 'x' : '-';
    perms[7] = S_IROTH & file_info->st_mode ? 'r' : '-';
    perms[8] = S_IWOTH & file_info->st_mode ? 'w' : '-';
    perms[9] = S_IXOTH & file_info->st_mode ? 'x' : '-';
    perms[10] = '\0';

    printf("%s  %lu  %s  %s  %8ld  %20s  %s\n", perms, file_info->st_nlink, getpwuid(file_info->st_uid)->pw_name, getgrgid(file_info->st_gid)->gr_name, file_info->st_size, file_time, file_name);
}

int compare(const void* a, const void* b) {
    char* str_a = *(char**) a;
    char* str_b = *(char**) b;

    return strcmp(str_a, str_b);
}

void ls(char* target_dir, int show_hidden, int show_long) {
    DIR *dir;
    struct dirent *dir_file;

    //Building the filename array
    int capacity = 10;
    int nb_of_files = 0;
    char** tab_filenames = malloc(capacity * sizeof(char*));

    dir = opendir(target_dir);

    if (!dir) {
        free(tab_filenames);
        perror("opendir failed");

        return;
    }

        dir_file = readdir(dir);        
        while(dir_file != NULL) {
            
            if (!show_hidden && dir_file->d_name[0] == '.') { //File should  not appear
                dir_file = readdir(dir);
                continue;
            }

            if (nb_of_files == capacity) {
                capacity *= 2;

                char** tmp = realloc(tab_filenames, capacity * sizeof(char*));
                if (!tmp) {
                    for (int i = 0; i < nb_of_files; i++) {
                        free(tab_filenames[i]);
                    }
                    free(tab_filenames);
                    perror("realloc failed");
                    return;
                }
                tab_filenames = tmp;
            }
            tab_filenames[nb_of_files] = strdup(dir_file->d_name);
            nb_of_files++;            

            dir_file = readdir(dir);
        }    
    closedir(dir);

    qsort (tab_filenames, nb_of_files, sizeof(char*), compare);

    //Output
    if (show_long) printf("permissions  liens  proprietaire  groupe  taille  date  nom\n");
    for (int i = 0; i < nb_of_files; i++) {
        if (show_long) {
            struct stat file_info;
            size_t len = strlen(tab_filenames[i]) + strlen(target_dir) + 2; //+2 for  "/" and "\0"
            char pathname[len];
            snprintf(pathname, len, "%s/%s", target_dir, tab_filenames[i]); //Build pathname
            if (stat(pathname, &file_info) == -1) {
                perror("stat failed");
            }
            ls_long(&file_info, tab_filenames[i]);
            
        } else {
            printf("%s  ", tab_filenames[i]);
        }
        free(tab_filenames[i]); //We have to free each filenames because of strdup
    }
    if (!show_long) printf("\n");
    free(tab_filenames);
}


int main(int argc, char** argv) {   //argc = number of options | argv = list of options
    int show_hidden = 0;
    int show_long = 0;
    char* target_dir = ".";
    int cmp = 1;    //options start at argv[1] because argv[0] == ls

    if (argc > 1 && argv[1][0] != '-') {
        target_dir = argv[1];
        cmp++;
    }

    while(cmp < argc) {
        if (strcmp(argv[cmp], "-a") == 0) show_hidden = 1;
        if (strcmp(argv[cmp], "-l") == 0) show_long = 1;
        if (strcmp(argv[cmp], "-la") == 0 || strcmp(argv[cmp], "-al") == 0) {
            show_hidden = 1; 
            show_long = 1;
        }
        cmp++;
    }

    ls(target_dir, show_hidden, show_long);


    return 0;
}