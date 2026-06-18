//La command ls du bash renvoie les noms de tout les dossiers et fichiers
//du dossier dans lequel elle est appeller
//
//

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main() {
    DIR *dir;
    struct dirent *dirp;

    dir = opendir("..");

    dirp = readdir(dir);
    printf("%s\n", dirp->d_name);

    closedir(dir);

    return 0;
}