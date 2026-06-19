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


void ls(char* target_dir, int show_hidden, int show_long) {
    DIR *dir;
    struct dirent *dir_file;

    dir = opendir(target_dir); 

        dir_file = readdir(dir);
        printf("permissions  liens  proprietaire  groupe  taille  date  nom\n");
        while(dir_file != NULL) {
            if (!show_hidden && dir_file->d_name[0] == '.') { //Le fichier doit etre affiché
                dir_file = readdir(dir);
                continue;

            }
            if (show_long) {
                struct stat file_info;
                size_t len = strlen(dir_file->d_name) + strlen(target_dir) + 2; //+2 pour le "/" et le "\0"
                char* pathname = malloc(len);
                strcat(pathname, target_dir); strcat(pathname, "/"); strcat(pathname, dir_file->d_name); 
                if (stat(pathname, &file_info) == -1) {
                    perror("stat failed");
                }
                printf("%lu  %lu  %d  %d  %ld  %li  %s\n", file_info.st_rdev, file_info.st_nlink, file_info.st_uid, file_info.st_gid, file_info.st_size, file_info.st_mtime, dir_file->d_name);

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
        if (strcmp(argv[cmp], "-a") == 0) {
            show_hidden = 1;
        }
        if (strcmp(argv[cmp], "-l") == 0) {
            show_long = 1;
        }
        cmp++;
    }

    ls(target_dir, show_hidden, show_long);


    return 0;
}