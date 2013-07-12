#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "harness.h"

FILE * insert_out;
FILE * member_out;

int insert_probe_count = 0;
int member_probe_count = 0;

int probe_distance(Data chain, int pos) {
  if (pos < chain.pos)
    return size+pos - chain.pos;
  else
    return pos - chain.pos;
}

void insert_poor(Data * hm, Data elem, int pos) {
  Data candidate;
  int dist, dist_candidate;
  int probe_count = 0;

  dist = probe_distance(elem,pos);

  while (dist < size) {
    candidate = hm[pos];

    if (is_empty(candidate)) {
      hm[pos] = elem;
      break;
    }

    dist_candidate = probe_distance(candidate,pos);
    if (dist > dist_candidate) {
      hm[pos] = elem;
      write_csv(insert_out, probe_count);
      probe_count = 0;
      dist = dist_candidate;
      elem = candidate;
    }

    dist++;
    probe_count++;
    pos = (pos+1) % size;
  }

  write_csv(insert_out, probe_count);
  insert_probe_count += probe_count;
  return;
}

void insert(Data * hm, char * key) {
  uint32_t h = hash(key);
  int pos = h % size;

  Data * elem = new(key, pos);
  insert_poor(hm,(*elem),pos);
  return;
}

void member(Data * hm, char * key) {
  uint32_t h = hash(key);
  int pos = h % size;
  int dist = 0;
  int probe_count = 0;

  Data candidate;
  while (dist < size) {
    candidate = hm[pos];
    if (dist > probe_distance(candidate,pos)) {
      printf("missing key=%s\n", key);
      break;
    }

    if (strcmp(candidate.key, key) == 0)
      break;

    dist++,
    probe_count++;
    pos = (pos+1) % size;
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
