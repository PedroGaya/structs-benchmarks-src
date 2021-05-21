#include <stdio.h>
#include <conio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>

#include "lse.h"
#include "../../log/log.c/src/log.h"
#include "../../data_gen/mtwister/mtwister.h"

LSE* init_LSE(void)
{
  LSE* list = NULL;
  list = (LSE*) malloc(sizeof(LSE));

  list->size = 0;
  list->start = NULL;

  return list;
};

ptLSE* insert_ptLSE(int input, LSE* list, long* ops_counter)
{
       ptLSE* novo;
       novo = (ptLSE*) malloc(sizeof(ptLSE));

       novo->numero = input;
      
       *ops_counter += 1;
       if (input == 0) return NULL;
       
       *ops_counter += 1;
       if (list->start == NULL) {
         list->start = novo;
         novo->prox = novo;
         novo->ant = novo;
         list->size++;
         return list->start;
       }
       
       novo->prox = list->start;
       novo->ant = list->start->ant;
       list->start->ant->prox = novo;
       list->start->ant = novo;

       list->size++;

       return list->start;
}

int show_LSE(LSE* list)
{
  if (list == NULL) { printf("List is NULL\n"); return -1; };
  if (list->start == NULL) { printf("Empty list\n"); return -1; };

  ptLSE* head = list->start;

  do {
    printf("%d ", head->numero);
    head = head->prox;
  } while(head != list->start);

  printf("\n");

  return 0;
}

int destroy_LSE(LSE* list)
{
  if (list != NULL) {
    do {
      ptLSE* curr = list->start;
      list->start = list->start->prox;
      free(curr);
    } while (list->start != NULL);
    return 0;
  free(list);
  } else {
    return -1;
  }
}

int find_LSE(int input, LSE* list, long* ops_counter) {
  if (list == NULL) { printf("List is NULL\n"); return 0; };
  if (list->start == NULL) { printf("Empty list\n"); return 0; };
  
  ptLSE* head = list->start;
  long curr_ops = 3;

  while(head->numero != input) {
    head = head->prox;

    curr_ops++;
    if (head == list->start) {
      return 0;
    }

    curr_ops++;
  };

  *ops_counter += curr_ops;
  return 1;
}

void benchmark_LSE(int* data, int data_size, int is_random) {
  LSE* list = init_LSE();

  // Start by inserting into the list.

  long insert_ops = 0;
  clock_t insert_start = clock();

  printf("Inserting into list...\n");
  for (int i = 0; i < data_size; i++) {
    printf("\r%d of %d done.", i+1, data_size);
    fflush(stdout);
    insert_ptLSE(data[i], list, &insert_ops);
  };
  printf("\n");

  long insert_time = (long)(clock() - insert_start) / (CLOCKS_PER_SEC / 1000);

  log_info("INSERT LSEC|%s|%li|%lims|%liops\n", 
   (is_random ? "randomized" : "ordered"), data_size, insert_time, insert_ops);

  if (is_random) {
    FILE* insert_output = fopen("../logs/lsec_benchmarks/insert/random.txt", "a");
    fprintf(insert_output, "%d|%li|%d\n", data_size, insert_time, insert_ops);
    fclose(insert_output);
  } else {
    FILE* insert_output = fopen("../logs/lsec_benchmarks/insert/ordered.txt", "a");
    fprintf(insert_output, "%d|%li|%d\n", data_size, insert_time, insert_ops);
    fclose(insert_output);
  }

  // Now we consult according to specification.

  long consult_ops = 0;
  long consult_array[5] = {0,0,0,0,0};

  if (!is_random) {
    consult_array[0] = 1;
    consult_array[1] = data_size - 1;
    consult_array[2] = data_size / 2;
  } else {
    MTRand r = seedRand(clock());
    for (int i = 0; i < 5; i++) {
      long to_consult = (long)(genRand(&r) * data_size) + 1;
      consult_array[i] = to_consult;
    }
  }

  for (int i = 0; i < 5; i++) {
    long val = consult_array[i];

    if (val != 0) {
      clock_t consult_start = clock();
      if (!find_LSE(val, list, &consult_ops)) { log_error("Something went wrong while consulting.\n");}
      long consult_time_curr = (long)(clock() - consult_start) / (CLOCKS_PER_SEC / 1000);
      consult_array[i] = consult_time_curr;
    }
  }

  long consult_time = 0;
  if (is_random) {
    for (int i = 0; i < 5; i++) {
      consult_time += consult_array[i];
    }
    consult_time = consult_time / 5;

    log_info("CONSULT LSEC|%s|%d|%lims(mean)|%dops\n", 
      (is_random ? "randomized" : "ordered"), data_size, consult_time, consult_ops);

    FILE* consult_output = fopen("../logs/lsec_benchmarks/consult/random.txt", "a");
    fprintf(consult_output, "%d|%li|%d\n", data_size, consult_time, consult_ops);
    fclose(consult_output);
  } else {
    log_info("CONSULT LSEC|%s|%d|%lims|%lims|%lims|%liops\n", 
      (is_random ? "randomized" : "ordered"), data_size, consult_array[0], 
      consult_array[1], consult_array[2], consult_ops);

    FILE* consult_output = fopen("../logs/lsec_benchmarks/consult/ordered.txt", "a");
    fprintf(consult_output, "%d|%li|%li|%li|%d\n", data_size, 
      consult_array[0], consult_array[1], consult_array[2], consult_ops);
    fclose(consult_output);
  }

};
