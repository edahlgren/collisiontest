#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "harness.h"
#include "lookup3.h"

int size = 4096;

uint32_t hash(char * key) {
  size_t len = strlen(key);
  return hashlittle(key, len, 0x106fc397);
}

Data * new(char * key, int pos) {
  Data * n = malloc(sizeof(Data));
  n->key = key;
  n->pos = pos;
  n->next = NULL;
  return n;
}

static const Data EmptyStruct;

Data empty(void) {
  Data empty = EmptyStruct;
  empty.pos = -1;
  return empty;
}

int is_empty(Data data) {
  if (data.pos < 0)
    return 1;
  return 0;
}

Data * initialize(void) {
  Data * hm = malloc(size*sizeof(Data));
  if (hm == NULL) {
    fprintf(stderr, "Oh no your array couldn't be malloc'd\n");
    exit(1);
  }

  int i;
  for (i=0; i<size; i++)
    hm[i] = empty();

  int check = check_initialized(hm);
  if (check < 0) {
    fprintf(stderr, "Oh no your array wasn't initialized properly\n");
    exit(1);
  }
  return hm;
}

int check_initialized(Data * hm) {
  int i;
  for (i=0; i<size; i++) {
    if (!is_empty(hm[i]))
      return -1;
  }
  return 0;
}

void destroy(Data * hm) {
  free(hm);
}

void harness(Data * hm, void (*functionPtr)(Data * hm, char * key), int step) {
  int i, index;
  for (i=0; i<size; i++) {
    index = (i+step) % size;
    char * k = malloc(10);
    snprintf(k, 10, "%d", index);
    (*functionPtr)(hm,k);
  }
  return;
}

FILE * safe_open(char * fn, char * mode) {
  FILE * out = fopen(fn,mode);
  if (out == NULL) {
    fprintf(stderr, "Can't open file %s\n", fn);
    exit(1);
  }
  return out;
}

void write_csv(FILE * fp, int val) {
  long pos;
  char * format;

  if (fp == NULL) {
    printf("write_int_csv: file pointer is NULL\n");
    return;
  }

  if (ftell(fp) == 0)
    format = "%d";
  else
    format = ",%d";

  fprintf(fp, format, val);
  return;
}
