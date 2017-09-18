#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* Defining hashTable size. `109` because it's a prime number and generates less collisions (github.com/ontech7/SieveOfEratosthenes) */
#define HASH_SIZE 109

/* Defining conditions on tree */
#define MAX_LENGTH 1024
#define MAX_DEPTH 255
#define MAX_NAME 255

#define NOT_FOUND 0

/* Declaring `type_t` struct */
typedef enum {
	TYPE_FILE,
	TYPE_DIR
} type_t;

/* Declaring and defining `tNode` struct */
typedef struct tNode{
	unsigned short		nKey;
	char			*szDirectory;
	char			*szCompleteDir;
	char			*szContent;
	type_t			tTypeInfo;
	struct tNode		*pNext;
	struct tHashtable	*pNextTable;
} Node;

/* Declaring and defining `tHashtable` struct */
typedef struct tHashtable{
	unsigned int		nLength;
	struct tNode		*pElem;
} Hashtable;

/* Declaring and defining `tDirectoryItem` struct */
typedef struct tDirectoryItem {
	char			*szDirectory;
	char			*szCompleteDir;
	unsigned short		bIsDeleteR;
	struct tDirectoryItem	*pNext;
} DirectoryItem;

/* ------------------------------------------ Prototypes ---------------------------------------------*/
int		compareStrings(const void* pString1, const void* pString2);
unsigned int	getHashKey(const unsigned int nKey);
unsigned int	char_to_ASCII(char* szWord);
unsigned int	lengthCounter(char *szWord, char sDelim);
unsigned int	listLenght(DirectoryItem **pList);
char**		splitSentence(char *szPhrase, char *szDelim);
Hashtable*	htMake(unsigned int nSize);
Node*		htNewNode(unsigned int nKey, char *szDirectory, char* szCompleteDir, type_t tTypeInfo);
Node**		htSearchAddr(Node **pElem, char *szDirectory);
void		pushElement(DirectoryItem **pHead, char *szDirectory, char *szCompleteDir, unsigned short bIsDeleteR);
void		removingElements(DirectoryItem **insertList, DirectoryItem **deleteList);
void		htInsert(Hashtable **pHashtable, unsigned int nKey, char *szDirectory, char* szCompleteDir, type_t tTypeInfo);
void		createFile(Hashtable **pHashtable, DirectoryItem **pHead, char *szDirectoryUNIX);
void		createDirectory(Hashtable **pHashtable, DirectoryItem **pHead, char *szDirectoryUNIX);
void		writeFile(Hashtable **pHashtable, char *szDirectoryUNIX, char *szContentOrig);
void		readFile(Hashtable **pHashtable, char *szDirectoryUNIX);
void		deleteResource(Hashtable **pHashtable, DirectoryItem **pHead, char *szDirectoryUNIX);
void		deleteAll(Hashtable **pHashtable, DirectoryItem **pHead, char *szDirectoryUNIX);
void		findElements(DirectoryItem **insertList, DirectoryItem **deleteList, char *szName);
/* ------------------------------------------------------------------------------------------------- */

/* Compare 2 strings. Function pointer that works as `callback` for `qsort` */
int compareStrings(const void* pString1, const void* pString2)
{
	char* szString1 = *(char**)pString1;
	char* szString2 = *(char**)pString2;

	return (strcmp(szString1, szString2));
}

/* Getting `hashKey` from module operation */
unsigned int getHashKey(const unsigned int nKey)
{
	return (nKey % HASH_SIZE);
}

/* Converting `szWord` letters to ASCII and summing them */
unsigned int char_to_ASCII(char* szWord)
{
	unsigned int i;
	unsigned int nResult = 0;

	for (i = 0; szWord[i] != '\0'; i++)
		nResult += (unsigned int)szWord[i];

	return (nResult);
}

/* Count all `szWord` delimeters (useful for arrays) */
unsigned int lengthCounter(char *szWord, char sDelim)
{
	unsigned int i;
	unsigned int nSize = 0;

	/* Counting the array size based on the delimiter */
	for (i = 0; szWord[i] != '\0'; i++)
		if (szWord[i] == sDelim)
			nSize++;

	return (nSize);
}

/* Counting all elements of a list */
unsigned int listLenght(DirectoryItem **pList)
{
	unsigned int nCount = 0;

	while ((*pList) != NULL)
	{
		pList = &(*pList)->pNext;
		nCount++;
	}

	return nCount;
}

/* Splitting `szPhrase` sentence in an array of strings */
char** splitSentence(char *szPhrase, char *szDelim)
{
	char		*szBuffer = strdup(szPhrase);
	char		*p = strtok(szBuffer, szDelim);
	char		**szToken;
	unsigned int	i = 0;
	unsigned int	nSizeArray = 0;

	/* Getting the size of array thanks to `lenghthCounter` func and allocating with calloc (malloc + memset) */
	nSizeArray = lengthCounter(szPhrase, szDelim[0]);
	szToken = (char**)calloc(nSizeArray, sizeof(char*));

	/* Splitting `szPhrase` sentence into an array using a `szDelim` delimiter */
	while (p != NULL) {
		szToken[i++] = p;
		p = strtok(NULL, szDelim);
	}

	return (szToken);
}

/* Creating a `Hashtable` based on the `nSize` */
Hashtable* htMake(unsigned int nSize)
{
	unsigned int	i;
	Hashtable	*htTmp;

	htTmp = (Hashtable*)malloc(nSize * sizeof(Hashtable));

	/* Setting `pElem` and `pNextTable` to NULL */
	for (i = 0; i < nSize; i++){
		htTmp[i].nLength = 0;
		htTmp[i].pElem = NULL;
	}

	return (htTmp);
}

/* Searching if a certain `pElem` is in the table and returning his address*/
Node** htSearchAddr(Node **pElem, char *szDirectory)
{
	/* Scanning `pNext` list until it's NULL */
	while ((*pElem) != NULL) {
		if (!strcmp((*pElem)->szDirectory, szDirectory))
			return pElem;
		pElem = &((*pElem)->pNext);
	}

	return NOT_FOUND;
}

/* Pushing elements to the head of the list */
void pushElement(DirectoryItem **pHead, char *szDirectory, char *szCompleteDir, unsigned short bIsDeleteR) 
{
	DirectoryItem* newElem = (DirectoryItem*)malloc(sizeof(DirectoryItem));

	newElem->szDirectory = strdup(szDirectory);
	newElem->szCompleteDir = strdup(szCompleteDir);
	newElem->bIsDeleteR = bIsDeleteR;
	newElem->pNext = *pHead;

	*pHead = newElem;
}

/* Removing items comparing 2 lists. */
void removingElements(DirectoryItem **insertList, DirectoryItem **deleteList)
{
	unsigned int nDirectoryDeleteLenght;
	unsigned int nDirectoryInsertLenght;
	unsigned int nFound = 0;

	while ((*insertList) != NULL)
	{
		/* Check if flag `bIsDeleteR` is false */
		if (!(*deleteList)->bIsDeleteR)
		{
			/* Remove `deleteList` item from `insertList` based on `szCompleteDir` */
			if (!strcmp((*insertList)->szCompleteDir, (*deleteList)->szCompleteDir))
			{
				*insertList = (*insertList)->pNext;
				break;
			}
		}
		/* Check if flag `bIsDeleteR` is true */
		else
		{
			/* Remove `deleteList` items from `insertList` based on `szCompleteDir` */
			nDirectoryDeleteLenght = strlen((*deleteList)->szCompleteDir);
			nDirectoryInsertLenght = strlen((*insertList)->szCompleteDir);
			if (!strncmp((*insertList)->szCompleteDir, (*deleteList)->szCompleteDir, nDirectoryDeleteLenght))
			{
				if (!nFound || nDirectoryInsertLenght > nDirectoryDeleteLenght)
				{
					*insertList = (*insertList)->pNext;
					if (nDirectoryDeleteLenght == nDirectoryInsertLenght)
						nFound = 1;
				}
				else if (nFound && nDirectoryDeleteLenght == nDirectoryInsertLenght)
				{
					break;
				}
				continue;
			}
		}
		insertList = &(*insertList)->pNext;
	}
}

/* Inserting a `pElem` in the table and increasing his `nLength` (check `MAX_LENGTH`) */
void htInsert(Hashtable **pHashtable, unsigned int nKey, char *szDirectory, char* szCompleteDir, type_t tTypeInfo)
{
	Node **ndHead = &(*pHashtable)[nKey].pElem;
	Node *ndTmp = (Node*)malloc(sizeof(Node));

	/* Increasing the number of resources: `nLength` */
	(*pHashtable)[nKey].nLength += 1;

	/* Setting all elements of the struct */
	ndTmp->nKey = nKey;
	ndTmp->szDirectory = strdup(szDirectory);
	ndTmp->szCompleteDir = strdup(szCompleteDir);
	ndTmp->tTypeInfo = tTypeInfo;
	ndTmp->szContent = NULL;
	ndTmp->pNext = *ndHead;
	ndTmp->pNextTable = NULL;

	*ndHead = ndTmp;
}

/* Create a file inside a given directory (UNIX-like) */
void createFile(Hashtable **pHashtable, DirectoryItem **pHead, char *szDirectoryUNIX)
{
	unsigned int	i;
	unsigned int	nKey;
	unsigned int	nStringLength;
	unsigned int	nSizeArray = lengthCounter(szDirectoryUNIX, '/');
	char		**szSplitDir = splitSentence(szDirectoryUNIX, "/");
	Node		**ndElemFound;

	if (nSizeArray <= MAX_DEPTH)
	{
		for (i = 0; i < nSizeArray; i++)
		{
			nKey = getHashKey(char_to_ASCII(szSplitDir[i]));
			ndElemFound = htSearchAddr(&(*pHashtable)[nKey].pElem, szSplitDir[i]);
			nStringLength = strlen(szSplitDir[i]);

			/* Element not found. Insert the element in the hashTable */
			if (ndElemFound == NOT_FOUND &&
				(i == (nSizeArray - 1)) &&
				(*pHashtable)[nKey].nLength <= MAX_LENGTH &&
				nStringLength <= MAX_NAME)
			{
				htInsert(pHashtable, nKey, szSplitDir[i], szDirectoryUNIX, TYPE_FILE);
				pushElement(pHead, szSplitDir[i], szDirectoryUNIX, 0);
				break;
			}

			/* Element found. Go to the next hashTable if `ndElemFound` is a Directory */
			else if (ndElemFound != NOT_FOUND &&
				(*ndElemFound)->tTypeInfo == TYPE_DIR &&
				(i != (nSizeArray - 1)) &&
				(*pHashtable)[nKey].nLength <= MAX_LENGTH &&
				nStringLength <= MAX_NAME)
			{
				if ((*ndElemFound)->pNextTable == NULL)
					(*ndElemFound)->pNextTable = htMake(HASH_SIZE);
				pHashtable = &(*ndElemFound)->pNextTable;
				continue;
			}

			/* All other cases (ex.: Exceeding limits, it's a file, same element, etc.) */
			else
			{
				printf("no\n");
				return;
			}
		}
	}

	printf("ok\n");

	return;
}

/* Create a folder inside a given directory (UNIX-like) */
void createDirectory(Hashtable **pHashtable, DirectoryItem **pHead, char *szDirectoryUNIX)
{
	unsigned int	i;
	unsigned int	nKey;
	unsigned int	nStringLength;
	unsigned int	nSizeArray = lengthCounter(szDirectoryUNIX, '/');
	char		**szSplitDir = splitSentence(szDirectoryUNIX, "/");
	Node		**ndElemFound;

	if (nSizeArray <= MAX_DEPTH)
	{
		for (i = 0; i < nSizeArray; i++)
		{
			nKey = getHashKey(char_to_ASCII(szSplitDir[i]));
			ndElemFound = htSearchAddr(&(*pHashtable)[nKey].pElem, szSplitDir[i]);
			nStringLength = strlen(szSplitDir[i]);

			/* Element not found. Insert the element in the `pHashTable` */
			if ((ndElemFound) == NOT_FOUND &&
				(i == (nSizeArray - 1)) &&
				(*pHashtable)[nKey].nLength <= MAX_LENGTH &&
				(nStringLength) <= MAX_NAME)
			{
				htInsert(pHashtable, nKey, szSplitDir[i], szDirectoryUNIX, TYPE_DIR);
				pushElement(pHead, szSplitDir[i], szDirectoryUNIX, 0);
				break;
			}

			/* Element found. Go to the next hashTable if `ndElemFound` is a Directory */
			else if (ndElemFound != NOT_FOUND &&
				(*ndElemFound)->tTypeInfo == TYPE_DIR &&
				(i != (nSizeArray - 1)) &&
				(*pHashtable)[nKey].nLength <= MAX_LENGTH &&
				(nStringLength) <= MAX_NAME)
			{
				if ((*ndElemFound)->pNextTable == NULL)
					(*ndElemFound)->pNextTable = htMake(HASH_SIZE);
				(pHashtable) = &(*ndElemFound)->pNextTable;
				continue;
			}

			/* All other cases (ex.: Exceeding limits, it's a file, same element, etc.) */
			else
			{
				printf("no\n");
				return;
			}
		}
	}

	printf("ok\n");

	return;
}

/* Write inside a resource file located inside `pHashtable` */
void writeFile(Hashtable **pHashtable, char *szDirectoryUNIX, char *szContentOrig)
{
	char		**szSplitDir = splitSentence(szDirectoryUNIX, "/");
	char		*szContent = splitSentence(szContentOrig, "\"")[0];
	unsigned int	nSizeArray = lengthCounter(szDirectoryUNIX, '/');
	unsigned int	nContentLenght = strlen(szContent);
	unsigned int	nKey;
	unsigned int	i;
	Node			**ndElemFound;

	if (nSizeArray <= MAX_DEPTH)
	{
		for (i = 0; i < nSizeArray; i++)
		{
			nKey = getHashKey(char_to_ASCII(szSplitDir[i]));
			ndElemFound = htSearchAddr(&(*pHashtable)[nKey].pElem, szSplitDir[i]);

			/* Element not found. Break cycle. */
			if ((ndElemFound) == NOT_FOUND)
				break;

			/* Element found. If it's a folder, enter in. */
			else if ((ndElemFound) != NOT_FOUND &&
				(*ndElemFound)->tTypeInfo == TYPE_DIR)
			{
				if ((*ndElemFound)->pNextTable == NULL && (i != (nSizeArray - 1)))
					break;
				(pHashtable) = &(*ndElemFound)->pNextTable;
			}

			/* Element found. If it's a file, write in it. */
			else if ((ndElemFound) != NOT_FOUND &&
				(*ndElemFound)->tTypeInfo == TYPE_FILE)
			{
				(*ndElemFound)->szContent = strdup(szContent);
				printf("ok %i\n", nContentLenght);
				return;
			}
		}
	}

	printf("no\n");

	return;
}

/* Read a resource file located inside `pHashtable` */
void readFile(Hashtable **pHashtable, char *szDirectoryUNIX)
{
	char		**szSplitDir = splitSentence(szDirectoryUNIX, "/");
	unsigned int	nSizeArray = lengthCounter(szDirectoryUNIX, '/');
	unsigned int	nKey;
	unsigned int	i;
	Node		**ndElemFound;

	if (nSizeArray <= MAX_DEPTH)
	{
		for (i = 0; i < nSizeArray; i++)
		{
			nKey = getHashKey(char_to_ASCII(szSplitDir[i]));
			ndElemFound = htSearchAddr(&(*pHashtable)[nKey].pElem, szSplitDir[i]);

			/* Element not found. Break cycle. */
			if ((ndElemFound) == NOT_FOUND)
				break;

			/* Element found. If it's a folder, enter in. */
			else if ((ndElemFound) != NOT_FOUND &&
				(*ndElemFound)->tTypeInfo == TYPE_DIR)
			{
				if ((*ndElemFound)->pNextTable == NULL && (i != (nSizeArray - 1)))
					break;
				(pHashtable) = &(*ndElemFound)->pNextTable;
				continue;
			}

			/* Element found. If it's a file, write in it. */
			else if ((ndElemFound) != NOT_FOUND &&
				(*ndElemFound)->tTypeInfo == TYPE_FILE)
			{
				printf("contenuto %s\n", (*ndElemFound)->szContent ? (*ndElemFound)->szContent : "");
				return;
			}
		}
	}

	printf("no\n");

	return;
}

/* Delete a resource inside the `pHashtable` */
void deleteResource(Hashtable **pHashtable, DirectoryItem **pHead, char *szDirectoryUNIX)
{
	char		**szSplitDir = splitSentence(szDirectoryUNIX, "/");
	unsigned int	nSizeArray = lengthCounter(szDirectoryUNIX, '/');
	unsigned int	nKey;
	unsigned int	i;
	Node		**ndElemFound;

	if (nSizeArray <= MAX_DEPTH)
	{
		for (i = 0; i < nSizeArray; i++)
		{
			nKey = getHashKey(char_to_ASCII(szSplitDir[i]));
			ndElemFound = htSearchAddr(&(*pHashtable)[nKey].pElem, szSplitDir[i]);

			/* Element not found. Break the FOR cycle. */
			if ((ndElemFound) == NOT_FOUND)
				break;

			/* Element found. If it's a folder, enter in. */
			else if ((ndElemFound) != NOT_FOUND &&
				(*ndElemFound)->tTypeInfo == TYPE_DIR &&
				(*ndElemFound)->pNextTable != NULL &&
				(i != (nSizeArray - 1)))
			{
				(pHashtable) = &(*ndElemFound)->pNextTable;
				continue;
			}

			/* Element found. If it's leaf, delete it. */
			else if ((ndElemFound) != NOT_FOUND &&
				(i == (nSizeArray - 1)))
			{
				if ((*ndElemFound)->pNextTable == NULL && !strcmp((*ndElemFound)->szCompleteDir, szDirectoryUNIX))
				{
					(*ndElemFound) = (*ndElemFound)->pNext;
					(*pHashtable)[nKey].nLength -= 1;
					printf("ok\n");
					pushElement(pHead, szSplitDir[i], szDirectoryUNIX, 0);
					return;
				}
				else
				{
					break;
				}
			}
		}
	}

	printf("no\n");

	return;
}

/* Delete a resource and all the children inside the `pHashtable` */
void deleteAll(Hashtable **pHashtable, DirectoryItem **pHead, char *szDirectoryUNIX)
{
	char		**szSplitDir = splitSentence(szDirectoryUNIX, "/");
	unsigned int	nSizeArray = lengthCounter(szDirectoryUNIX, '/');
	unsigned int	nKey;
	unsigned int	i;
	Node		**ndElemFound;

	if (nSizeArray <= MAX_DEPTH)
	{
		for (i = 0; i < nSizeArray; i++)
		{
			nKey = getHashKey(char_to_ASCII(szSplitDir[i]));
			ndElemFound = htSearchAddr(&(*pHashtable)[nKey].pElem, szSplitDir[i]);

			/* Element not found. Break the FOR cycle. */
			if ((ndElemFound) == NOT_FOUND)
				break;

			/* Element found. If it's a folder, enter in. */
			else if ((ndElemFound) != NOT_FOUND &&
				(*ndElemFound)->tTypeInfo == TYPE_DIR &&
				(*ndElemFound)->pNextTable != NULL)
			{
				(pHashtable) = &(*ndElemFound)->pNextTable;
			}

			/* Element found. Delete it. */
			if ((ndElemFound) != NOT_FOUND &&
				(i == (nSizeArray - 1)))
			{
				*ndElemFound = NULL;
				printf("ok\n");
				pushElement(pHead, szSplitDir[i], szDirectoryUNIX, 1);
				return;
			}
		}
	}

	printf("no\n");

	return;
}

/* Find all resources (files/folders) named `szName` */
void findElements(DirectoryItem **insertList, DirectoryItem **deleteList, char *szName)
{
	char		**szDirectories;
	unsigned int	nResLength = strlen(szName);
	unsigned int	nSizeDeleteList = listLenght(deleteList);
	unsigned int	nSizeFinalList;
	unsigned int	nCount = 0;
	unsigned int	i;

	if (nResLength <= MAX_NAME)
	{
		/* Check if `insertList` is empty */
		if (*insertList == NULL)
		{
			printf("no\n");
			return;
		}

		/* Check if `deletetList` is NOT empty */
		if (*deleteList != NULL)
		{
			for (i = 0; i < nSizeDeleteList; i++)
			{
				removingElements(insertList, deleteList);
				*deleteList = (*deleteList)->pNext;
			}
		}

		/* Obtaining final size of the list and allocating it */
		nSizeFinalList = listLenght(insertList);
		szDirectories = (char**)malloc(nSizeFinalList * sizeof(char*));

		/* Searching elements */
		for (i = 0; i < nSizeFinalList; i++){
			if (!strcmp((*insertList)->szDirectory, szName))
			{
				szDirectories[nCount] = strdup((*insertList)->szCompleteDir);
				nCount++;
			}
			insertList = &(*insertList)->pNext;
		}

		/* Check if `nCount` is zero */
		if (nCount == 0)
		{
			printf("no\n");
			return;
		}

		/* Quick sort. Passing `(char**)` array of strings and `compareStrings` callback func */
		qsort(szDirectories, nCount, sizeof(char*), compareStrings);

		/* Printing directories ordered lexicographically */
		for (i = 0; i < (nCount); i++)
		{
			printf("ok %s\n", szDirectories[i]);
			free(szDirectories[i]);
		}

		return;
	}

	printf("no\n");

	return;
}

/* FOR DEBUG: Printing elements of a list */
//void printList(DirectoryItem **list)
//{
//	while ((*list) != NULL)
//	{
//		printf("%s | ", (*list)->szCompleteDir);
//		list = &(*list)->pNext;
//	}
//	printf("\n");
//}
