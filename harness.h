#include <stdint.h>

typedef struct Data {
  char * key;
  //real hash table would also have some value here
  struct Data * next; // for Chaining
  int pos; // for Robin Hood hashing
} Data;

int size;

// initialize and destroy the hashmap
// you need to destroy chains if you make them
Data * initialize(void);
void destroy(Data * hm);

// operations on Data
uint32_t hash(char * key);
Data * new(char * key, int pos);
int is_empty(Data data);
void harness(Data * hm, void (*functionPtr)(Data * hm, char * key), int step);
FILE * safe_open(char * fn, char * mode);
void write_csv(FILE * fp, int val);
