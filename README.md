# FilesystemAPI
A simple Filesystem using Hashtables. (No re-hashing)

![filesystemGIF](https://puu.sh/xD2P4/9342acb7e4.gif)

## Commands

> **create <path>** - Create a file with the path given. - Prints "ok" if it's all right.  
> **create_dir <path>** - Create a directory with the path given. - Prints "ok" if it's all right.  
> **delete <path>** - Delete one resource without any children. - Prints "ok" if it's all right.  
> **delete_r <path>** - Delete recursively the resource + his children. - Prints "ok" if it's all right.  
> **write <path>** <content> - Write inside a file. Prints "ok <number_of_characters>" if the file exists.  
> **read <path>** - Read the content of a file. Prints "contenuto <text>" if the file exists.  
> **find <name>** - Find every folder/file with the <name> given. - Prints every <path_find> if file/s exists/-.  
> **exit** - Exit from the program (return 0 successfully).  

## Compile

```
gcc -std=c99 -O2 main.c main.h -o filesystem  
```

## Author

**Andrea Losavio** - [facebook](https://www.facebook.com/andrea.losavio.7/)  

## Information

* Filesystem: https://en.wikipedia.org/wiki/File_system  
* Hashtable: https://en.wikipedia.org/wiki/Hash_table  

No load and re-hashing was used in this project.  
Free()`s are not implemented (very bad!)  
Delete_r is a fake one but it still works for this project : :( :  
Find uses a particular method to find elements and to order lexicographically (qsort)  

You can find any further information in the comments (main.c, main.h)
