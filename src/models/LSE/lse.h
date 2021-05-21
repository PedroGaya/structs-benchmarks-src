typedef struct tipoNo ptLSE;
typedef struct LSE LSE;

struct tipoNo {
       int numero;   
       ptLSE *prox;
       ptLSE *ant;
};

struct LSE {
       int size;
       ptLSE *start;
};

LSE* init_LSE(void);
ptLSE* insert_ptLSE(int input, LSE* list, long* ops_counter);
int destroy_LSE(LSE* list);
int show_LSE(LSE* list);
int find_LSE(int input, LSE* list, long* ops_counter);

void benchmark_LSE(int* data, int data_size, int is_random);