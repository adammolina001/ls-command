## ls-command

My version of the "ls" terminal's command before starting the shell.


## Compilation
```
  gcc my_ls.c -o my_ls
  ./my_ls
```


## Features
  -  ```./my_ls```  show files
  -  ```./my_ls -a```  show files and hidden files (starting with ".")
  -  ```./my_ls -l```  show files names and status
  -  ```./my_ls -la``` or ```./my_ls -al``` to see both


## Example : ./my_ls -l
```
  permissions  links  owner  group       size           date          name
  -rw-rw-r--     1     adam  adam           402o    June  24 10:11  README.md
  -rwxrwxr-x     1     adam  adam         16912o    June  24 10:20  my_ls
  -rw-rw-r--     1     adam  adam          4397o    June  24 10:20  my_ls.c
```
