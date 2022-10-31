1. Run 'dmood_hw3.c' file.
   gcc -o dmood_hw3 dmood_hw3.c

2. Run 'dmood_hw3' executable file.
   ./dmood_hw3
3. Run executable file with given options:
    ./dmood_hw3 -s 1024 -e "ls -l" : List all files with size <= 1024.
        
    ./dmood_hw3 -f “<substring>” <depth> -E "tar cvf jpg.tar" : List all files that have the substring in their filename.

    ./dmood_hw3 -s 1024 -e "wc -l": List all files which have substring in their filename with size <= 1024.
	

