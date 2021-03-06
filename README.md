# FilesystemAPI
A simple Filesystem using Hashtables. (No re-hashing)

![filesystemGIF](https://puu.sh/xD2P4/9342acb7e4.gif)

## Commands

> **create <_path>** - Create a file with the path given. - Prints "ok" if it's all right.  
> **create_dir <_path>** - Create a directory with the path given. - Prints "ok" if it's all right.  
> **delete <_path>** - Delete one resource without any children. - Prints "ok" if it's all right.  
> **delete_r <_path>** - Delete recursively the resource + his children. - Prints "ok" if it's all right.  
> **write <_path> <_content>** - Write inside a file. Prints "ok <number_of_characters>" if the file exists.  
> **read <_path>** - Read the content of a file. Prints "contenuto <_content> " if the file exists.  
> **find <_name>** - Find every folder/file with the <name> given. - Prints every <_path_find> if file/s exists/-.  
> **exit** - Exit from the program (return 0 successfully).  

## Compile

```
gcc -std=c99 -O2 main.c main.h -o filesystem  
```

## Information

* Filesystem: https://en.wikipedia.org/wiki/File_system  
* Hashtable: https://en.wikipedia.org/wiki/Hash_table  

No load and re-hashing were used in this project.  
Free()s are not implemented (very bad!).  
Delete_r is a fake one but it still works for this project :( .  
Find uses a particular method to find elements and to order lexicographically (qsort), it's not the classic recursive method.  

Tested on:
 - **Win7 Pro x64** - i7 4790k (4.0 GHz - 4core/8thread) - 16 GB DDR3 (1866 MHz) - SSD Samsung Evo 850 (250 GB)  
 - **Elementary OS x64** - i7 6500U (2.6 GHz - 2core/4thread) - 16 GB DDR3 (1866 MHz) - SSD SanDisk (240 GB)  

You can find any further information in the comments (main.c, main.h)

## Author

**Andrea Losavio** - [facebook](https://www.facebook.com/andrea.losavio.7/)  
