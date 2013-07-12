#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "harness.h"

FILE * insert_out;
FILE * member_out;

int insert_probe_count = 0;
int member_probe_count = 0;

uint32_t hash2(uint32_t k) {
  //uint32_t prime = 3779; // center heptagonal prime under size=3967
  uint32_t prime = 57793; // center heptagonal prime under size=65213
  return (prime - (k % prime));
}

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
    pos = (h + step*hash2(h)) % size;
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
    if(!is_empty(*chain) && (strcmp(chain->key, key) == 0))
      break;
    step++;
    probe_count++;
    pos = (h + step*hash2(h)) % size;
  }

  write_csv(member_out, probe_count);
  member_probe_count += probe_count;
  return;
}

void print_hm(Data * hm) {
  int i;
  for (i=0; i<size; i++)
    if (!is_empty(hm[i]))
      printf("pos=%d intended pos=%d key=%s\n", i, hm[i].pos, hm[i].key);
  return;
}

void main(void) {
  //size =  3967; // carol prime under 4096
  size = 65213; // centered heptagonal prime under 65536

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
