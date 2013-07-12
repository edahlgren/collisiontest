#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "harness.h"

FILE * insert_out;
FILE * member_out;

int insert_probe_count = 0;
int member_probe_count = 0;

void insert(Data * hm, char * key) {
  uint32_t h = hash(key);
  int pos = h % size;
  int step = 0;
  int probe_count = 0;

  Data * chain;
  Data * elem = new(key, pos);
  while (step < size) {
    chain = &(hm[pos]);
    if (is_empty(*chain)) {
      hm[pos] = (*elem);
      break;
    }
    step++;
    probe_count++;
    pos = (h+step) % size;
  }

  write_csv(insert_out, probe_count);
  insert_probe_count += probe_count;
  return;
}

void member(Data * hm, char * key) {
  uint32_t h = hash(key);
  int pos = h % size;
  int step = 0;
  int probe_count = 0;

  Data * chain;
  while (step < size) {
    chain = &(hm[pos]);
    if (!is_empty(*chain)) {
      if (strcmp(chain->key, key) == 0)
	break;
    }
    step++;
    probe_count++;
    pos = (h+step) % size;
  }

  // make this a helper function, use a file.tell to see if we're at the head or not
  // print accordingly
  write_csv(member_out, probe_count);
  member_probe_count += probe_count;
  return;
}

void print_hm(Data * hm) {
  int i;
  for (i=0; i<size; i++)
    if(!is_empty(hm[i]))
      printf("pos=%d intended pos=%d key=%s\n", i, hm[i].pos, hm[i].key);
  return;
}

void main(void) {
  size = 65536;

  insert_out = safe_open("insert_out", "w");
  member_out = safe_open("member_out", "w");

  Data * hm = initialize();

  /** FREE ALL OF THE KEYS **/
  harness(hm, &insert, 0);
  harness(hm, &member, 100);
  printf("insert probe_count\t%d\n", insert_probe_count);
  printf("member probe_count\t%d\n", member_probe_count);
  printf("total probe_count\t%d\n", insert_probe_count+member_probe_count);

  destroy(hm);

  fclose(insert_out);
  fclose(member_out);
}
