/* * * * * * * * * * * * * * * * * * * * * * * * *
 *												 *
 * Name:    Filesystem Hashtable (FS_HT).		 *
 * Subject: Algorithms and Principles of I.T.	 * 
 * Object:  University final project.			 *
 * Credits: Andrea Losavio.						 *
 *												 *
 *			  Politecnico di Milano				 *
 *												 *
 * * * * * * * * * * * * * * * * * * * * * * * * */


#include "main.h"

int main()
{
	Hashtable		*pHashtable = htMake(HASH_SIZE);
	DirectoryItem	*insertList = NULL;
	DirectoryItem	*deleteList = NULL;

	char szCommand[12];
	char szDirectoryUNIX[1024*16];
	char szName[1024];
	char szContent[512];

	while (1) {
		if (scanf("%11s", szCommand))
		{
			if (!strcmp(szCommand, "print")) {
				//printList(&insertList);
			}
			else if (!strcmp(szCommand, "create_dir")) {
				if (scanf("%s", szDirectoryUNIX))
					createDirectory(&pHashtable, &insertList, szDirectoryUNIX);
			}
			else if (!strcmp(szCommand, "create")) {
				if (scanf("%s", szDirectoryUNIX))
					createFile(&pHashtable, &insertList, szDirectoryUNIX);
			}
			else if (!strcmp(szCommand, "write")) {
				if (scanf("%s", szDirectoryUNIX))
					if (scanf(" %[^\n]s", szContent))
						writeFile(&pHashtable, szDirectoryUNIX, szContent);
			}
			else if (!strcmp(szCommand, "read")) {
				if (scanf("%s", szDirectoryUNIX))
					readFile(&pHashtable, szDirectoryUNIX);
			}
			else if (!strcmp(szCommand, "delete")) {
				if (scanf("%s", szDirectoryUNIX))
					deleteResource(&pHashtable, &deleteList, szDirectoryUNIX);
			}
			else if (!strcmp(szCommand, "delete_r")) {
				if (scanf("%s", szDirectoryUNIX))
					deleteAll(&pHashtable, &deleteList, szDirectoryUNIX);
			}
			else if (!strcmp(szCommand, "find")) {
				if (scanf("%s", szName))
					findElements(&insertList, &deleteList, szName);
			}
			else if (!strcmp(szCommand, "exit"))
				return (0);
			else {
				//printf("invalid option\n");
			}
		}
	}

	return (0);
}