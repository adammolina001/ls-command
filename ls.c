//La command ls du bash renvoie les noms de tout les dossiers et fichiers
//du dossier dans lequel elle est appeller
//
//

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    DIR *dir;
    struct dirent *dir_file;
    char* dir_target = "."; //"." correspond au dossier ouvert

    dir = opendir(dir_target); //"." correspond au dossier ouvert

        printf("   name     |    taille \n");
        dir_file = readdir(dir);
        while(dir_file != NULL) {
            struct stat file_info;
            stat(dir_file->d_name, &file_info);
            if(dir_file->d_name[0] != '.') {  //Empeche l'affichage des entrée . .. ... rajouté par readdir et des fichierscommencant pas "."
                printf("%10s  |  %8lio\n", dir_file->d_name, file_info.st_size);
            }
            dir_file = readdir(dir);
        }

    closedir(dir);

    return 0;
}