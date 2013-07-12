#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "harness.h"

FILE * insert_out;
FILE * member_out;

int insert_probe_count = 0;
int member_probe_count = 0;

void insert(Data * hm, char * key) {
  int pos = hash(key) % size;
  int probe_count = 0;

  Data * elem = new(key, pos);
  Data * chain = &(hm[pos]);
  if (is_empty(*chain)) {
    hm[pos] = (*elem);
    write_csv(insert_out, probe_count);
    return;
  }
  else
    probe_count += 1;

  while (chain->next != NULL) {
    chain = chain->next;
    probe_count++;
  }
  chain->next = elem;

  write_csv(insert_out, probe_count);
  insert_probe_count += probe_count;
  return;
}

void member(Data * hm, char * key) {
  int pos = hash(key) % size;
  int probe_count = 0;

  Data * chain = &(hm[pos]);
  while (chain) {
    if (!is_empty((*chain))) {
      if (strcmp(chain->key, key) == 0) {
	break;
      }
    }
    chain = chain->next;
    probe_count++;
  }

  write_csv(member_out, probe_count);
  member_probe_count += probe_count;
  return;
}

void print_hm(Data * hm) {
  int i;
  for (i=0; i<size; i++) {
    Data * ptr = &(hm[i]);
    while (ptr) {
      if (!is_empty((*ptr)))
	printf("pos=%d key=%s\n", ptr->pos, ptr->key);
      ptr = ptr->next;
    }
  }
  return;
}

void main(void) {
  size = 65536;

  insert_out = safe_open("insert_out", "w");
  member_out = safe_open("member_out", "w");

  Data * hm = initialize();

  harness(hm, &insert, 0);
  harness(hm, &member, 100);
  printf("insert probe_count\t%d\n", insert_probe_count);
  printf("member probe_count\t%d\n", member_probe_count);
  printf("total probe_count\t%d\n", insert_probe_count+member_probe_count);

  destroy(hm);

  fclose(insert_out);
  fclose(member_out);
}
