#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "data_gen/db.h"
#include "models/LSE/lse.h"
#include "models/ABP/abp.h"
#include "log/log.c/src/log.h"

int main(int argc, char *argv[]) {
    bool isRandom = false;
    bool shouldClean = false;
    int svalue = 5000;
    int opt;

    while ((opt = getopt(argc, argv, ":s:rc")) != -1) {
        switch (opt) {
        case 'r': isRandom = true; break;
        case 'c': shouldClean = true; break;
        case 's': if(atoi(optarg)) { svalue = atoi(optarg); break; }
        case ':': 
                printf("option [-s] needs a positive integer value\n"); 
                break; 
        case '?': 
                printf("unknown option: %c\n", optopt);
                break; 
        default:
            printf("Usage: %s [-s size of dataset] [-r]\n", argv[0]);
            printf("-r to randomize the dataset. Default is an ordered dataset.");
            printf("-s to define dataset size. Default is 5000, max is LONG_MAX.");
            printf("-c to clean old data points before generating new ones.");
            exit(EXIT_FAILURE);
        }
    };

    FILE* output = fopen("../logs/logs.txt", "a");
    log_add_fp(output, LOG_TRACE);


    if (shouldClean) {
        remove("../logs/abp_benchmarks/consult/ordered.txt");
        remove("../logs/abp_benchmarks/consult/random.txt");
        remove("../logs/abp_benchmarks/insert/ordered.txt");
        remove("../logs/abp_benchmarks/insert/random.txt");
        remove("../logs/lsec_benchmarks/consult/ordered.txt");
        remove("../logs/lsec_benchmarks/consult/random.txt");
        remove("../logs/lsec_benchmarks/insert/ordered.txt");
        remove("../logs/lsec_benchmarks/insert/random.txt");
    }

    int* array = generate_DB(svalue, isRandom);
    benchmark_LSE(array, svalue, isRandom);
    benchmark_ABP(array, svalue, isRandom);

    fclose(output);
    }