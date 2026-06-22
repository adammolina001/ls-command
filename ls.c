//ls renvoie le nom des fichiers trié apr ordre alphabétique 
//ls -a montre aussi les fichiers cachés
//ls -l donne plus d'infos : permissions  liens  proprietaire  groupe  taille  date  nom
//ls /dossier renvoie les fichiers contenus dans dossier

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>



void set_str_perms(struct stat file_info, char* perms) {

    perms[0] = S_ISDIR(file_info.st_mode) ? 'd' : '-';
    perms[1] = S_IRUSR & file_info.st_mode ? 'r' : '-';
    perms[2] = S_IWUSR & file_info.st_mode ? 'w' : '-';
    perms[3] = S_IXUSR & file_info.st_mode ? 'x' : '-';
    perms[4] = S_IRGRP & file_info.st_mode ? 'r' : '-';
    perms[5] = S_IWGRP & file_info.st_mode ? 'w' : '-';
    perms[6] = S_IXGRP & file_info.st_mode ? 'x' : '-';
    perms[7] = S_IROTH & file_info.st_mode ? 'r' : '-';
    perms[8] = S_IWOTH & file_info.st_mode ? 'w' : '-';
    perms[9] = S_IXOTH & file_info.st_mode ? 'x' : '-';
    perms[10] = '\0';
}

void ls(char* target_dir, int show_hidden, int show_long) {
    DIR *dir;
    struct dirent *dir_file;

    dir = opendir(target_dir); 

        dir_file = readdir(dir);
        
        if (show_long) printf("permissions  liens  proprietaire  groupe  taille  date  nom\n");

        while(dir_file != NULL) {
            if (!show_hidden && dir_file->d_name[0] == '.') { //Le fichier doit etre affiché
                dir_file = readdir(dir);
                continue;
            }
            if (show_long) {
                struct stat file_info;
                
                size_t len = strlen(dir_file->d_name) + strlen(target_dir) + 2; //+2 pour le "/" et le "\0"
                char pathname[len];
                snprintf(pathname, len, "%s/%s", target_dir, dir_file->d_name); //Construit le pathname
                
                if (stat(pathname, &file_info) == -1) {
                    perror("stat failed");
                }

                //Construction de la date 
                char file_time[20]; //9 = september + "  " + day + " " + hour + \0
                strftime(file_time, 20, "%B  %d %R", localtime(&file_info.st_mtime));   
                
                //permissions
                char perms[11]; //1directory 3owner 3group 3others
                set_str_perms(file_info, perms);

                printf("%s  %lu  %s  %s  %8ld  %20s  %s\n", perms, file_info.st_nlink, getpwuid(file_info.st_uid)->pw_name, getgrgid(file_info.st_gid)->gr_name, file_info.st_size, file_time, dir_file->d_name);

            } else {
                printf("%s  \n", dir_file->d_name);
            }

            dir_file = readdir(dir);
        }

    closedir(dir);

}


int main(int argc, char** argv) {
    int show_hidden = 0;
    int show_long = 0;
    char* target_dir = ".";
    int cmp = 1;

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