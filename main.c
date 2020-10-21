#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct{
    char nome[40];
    float salario;
    long admissao_lg;
    char admissao_str[11];
    char departamento[40];

} Funcionario;

struct tm tm;
void args(char* arg[]);
long data(char data[]);
int numero_de_pessoas();
void ler_nome(FILE *arq, char nome[40]);
float media(Funcionario funcionario[], int num);
int mais_velho(Funcionario funcionario[], int num);
int ordem_alfb_func(const void *s1, const void *s2);
void ordem_alfb(Funcionario funcionario[], int num);
void leitura(Funcionario funcionario[], int num, char arquivo[]);
Funcionario * dep(Funcionario funcionario[], int *num, char departamento[]);
void print_funcionarios(Funcionario funcionario[], int num, int i,char arquivo[]);
Funcionario set_funcionario(char nome[], float salario, char admissao[], char departamento[]);

int main(int argc, char* argv[]){
    if(argc == 0) printf("Nenhum argumento passado");
    else args(argv);

    return 0;
}

void args(char* arg[]){
    char qst = (char) arg[2][0];
    char *departamento =  arg[3];

    int num = numero_de_pessoas();
    int valor;

    Funcionario funcionario[num];
    Funcionario *temp1;

    leitura(funcionario, num, arg[1]);

    switch (qst){
        case '1':
            ordem_alfb(funcionario, num);
            print_funcionarios(funcionario, num, 0, arg[3]);
            break;
        case '2':
            ordem_alfb(funcionario, num);
            temp1 = dep(funcionario, &num, departamento);
            print_funcionarios(temp1, num, 1, NULL);
            break;
        case '3':
            valor = mais_velho(funcionario, num);
            temp1 = &funcionario[valor];
            print_funcionarios(temp1, 1, 1, NULL);
            break;
        case '4':
            temp1 = dep(funcionario, &num, departamento);
            valor = mais_velho(temp1, num);
            print_funcionarios(&temp1[valor-1], 1, 1, NULL);
            break;
        case '5':
            printf("R$%.2f\n", media(funcionario, num));
            break;
        case '6':
            temp1 = dep(funcionario, &num, departamento);
            printf("R$%.2f\n", media(temp1, num));
            break;
        default:
            break;

    }
}

long data(char data[]){

    time_t t;

    strptime(data, "%d/%m/%Y", &tm);

    tm.tm_isdst = -1;
    t = mktime(&tm);

    return t;
}

int numero_de_pessoas(){
    int num;

    FILE *arq;

    arq = fopen("input.txt", "r");
    if (arq == NULL) exit(0);

    fscanf(arq, "%d", &num);
    fclose(arq);

    return num;
}

void ler_nome(FILE *arq, char nome[40]){
    int count = 0;
    char whl;

    fgetc(arq);

    while (1){
        whl = fgetc(arq);
        if (whl == '\n') break;
        nome[count] = whl;
        count++;
    }
}

float media(Funcionario funcionario[], int num){
    float media = 0;

    for (int i = 0; i < num; ++i) {
        media += funcionario[i].salario;
    }

    return media/(float)num;
}

void leitura(Funcionario funcionario[], int num, char arquivo[]){
    FILE *arq;
    arq = fopen(arquivo, "r");

    if (arq == NULL) exit(0);

    fscanf(arq, "%d", &num);

    for (int i = 0; i < num; ++i) {
        char nome[40] = {'\0'};
        float salario = 0;
        char data[40] = {'\0'};
        char setor[40] = {'\0'};

        ler_nome(arq, nome);
        fscanf(arq, "%f", &salario);
        fscanf(arq, "%s", data);
        fscanf(arq, "%s", setor);

        funcionario[i] = set_funcionario(nome, salario, data, setor);
    }

    fclose(arq);
}

int mais_velho(Funcionario funcionario[], int num){
    long temp = funcionario[0].admissao_lg;
    int valor = 1;

    for (int i = 0; i < num; ++i) {
        if(temp > funcionario[i].admissao_lg){
            temp = funcionario[i].admissao_lg;
            valor = i;
        }
    }

    return valor;
}

int ordem_alfb_func(const void *s1, const void *s2){
    Funcionario *e1 = (Funcionario *)s1; // faz o cast de void* para int*
    Funcionario *e2 = (Funcionario *)s2;

    int compNome = strcmp(e1->nome, e2->nome);

    return compNome;
}

void ordem_alfb(Funcionario funcionario[], int num){
    qsort(funcionario, num, sizeof(Funcionario), ordem_alfb_func);
}

void print_funcionarios(Funcionario funcionario[], int num, int i, char arquivo[]){

    if(i == 0){
        FILE *arq;
        arq = fopen(arquivo, "w");

        for (int j = 0; j < num; ++j) {
            char temp [200];
            sprintf(temp, "Nome: %s\n"
                          "Salario: R$%.2f\n"
                           "Admissão: %s\n"
                           "Departamento: %s\n\n",
                           funcionario[j].nome,
                           funcionario[j].salario,
                           funcionario[j].admissao_str,
                           funcionario[j].departamento);

            fprintf(arq,"%s",  temp);
        }

        fclose(arq);
    } else{
        for (int j = 0; j < num; ++j) {
            printf("Nome: %s\n"
                   "Salario: R$%.2f\n"
                   "Admissão: %s\n"
                   "Departamento: %s\n\n",
                   funcionario[j].nome,
                   funcionario[j].salario,
                   funcionario[j].admissao_str,
                   funcionario[j].departamento);
        }
    }
}

Funcionario * dep(Funcionario funcionario[], int *num, char departamento[]){
    int valor1, valor2  = 0;

    for (int i = 0; i < *num; ++i) {
        valor1 += strcmp(funcionario[i].departamento, departamento) == 0 ? 1 : 0;
    }

    Funcionario *temp = malloc(sizeof(Funcionario) * valor1); //Alocar memoria

    for (int i = 0; i < *num; ++i) {
        if (strcmp(funcionario[i].departamento, departamento) == 0){
            temp[valor2] = funcionario[i];
            valor2++;
        }
    }

    *num = valor2;

    return temp;
}

Funcionario set_funcionario(char nome[], float salario, char admissao[], char departamento[]){
    Funcionario var;

    strcpy(var.nome, nome);
    var.salario = salario;
    var.admissao_lg = data(admissao);
    strcpy(var.admissao_str, admissao);
    strcpy(var.departamento, departamento);

    return var;
}
