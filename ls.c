//ls renvoie le nom des fichiers trié apr ordre alphabétique 
//ls -a montre aussi les fichiers cachés
//ls -l donne plsu d'info
//ls /dossier renvoie les fichiers contenuent dans dossier

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


void ls(char* target_dir, int show_hidden, int show_long) {
    DIR *dir;
    struct dirent *dir_file;

    dir = opendir(target_dir); 

        printf("   name     |    taille \n");
        dir_file = readdir(dir);
        while(dir_file != NULL) {
            struct stat file_info;
            stat(dir_file->d_name, &file_info);

            if (show_hidden) {
                printf("%10s  |  %8lio\n", dir_file->d_name, file_info.st_size);
            } else {
                if(dir_file->d_name[0] != '.') {  //Empeche l'affichage des entrée . .. ... rajouté par readdir et des fichierscommencant pas "."
                    printf("%10s  |  %8lio\n", dir_file->d_name, file_info.st_size);
                }
            }
            
            dir_file = readdir(dir);
        }

    closedir(dir);

}


int main(int argc, char* argv[]) {
    int show_hidden = 0;
    int show_long = 0;
    char* target_dir = ".";
    int cmp = 1;

    if (argc > 1 && argv[1][0] != '-') {
        target_dir = argv[1];
        cmp++;
    }

    while(cmp <= argc) {    //<= car on initialise cmp a 1
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