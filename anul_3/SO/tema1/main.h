#ifndef MAIN_H
#define MAIN_H

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(EXIT_FAILURE);				\
		}							\
	} while (0)




typedef struct THashTable {
	char ***table;
	int *bucketsCapacity;
	int *bucketsElements;
	int nOfBuckets;
} THashTable;

void clearAll(THashTable hashTable);
void resize(THashTable *hashTable, int nOfBuckets);
void freeAll(THashTable *hashTable);



#endif
