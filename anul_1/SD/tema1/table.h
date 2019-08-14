/* SPORICI Dan - 315CB */

// structura de tip (key, value)
typedef struct TbucketCell
{
    char * key;
    char * value;
} TbucketCell;

// structura din care se compune un bucket
typedef struct Tbucket
{
    struct Tbucket * addr;
    void * info;
} Tbucket;

typedef unsigned int HASH;

// prototypes
Tbucket ** createTable(size_t);
inline HASH computeHash(char *, size_t);
char * getValue(FILE *, Tbucket **, size_t, char *, int);
int addToTable(Tbucket **, size_t, char *, char *);
void disposeAll(Tbucket **, size_t);
int removeFromTable(Tbucket **, size_t, char *);
void dumpTable(FILE *, Tbucket **, size_t, int);
int findKey(FILE *, Tbucket **, size_t, char *);

// pt listele generice
void disposeElement(Tbucket *);
char * keyOf(Tbucket *);
char * valueOf(Tbucket *);
void * createInfoCell(char *, char *);
