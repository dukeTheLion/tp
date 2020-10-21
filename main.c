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
void args(char* arg[]);  // Chamada (inicia o codigo de fato)
long data(char data[]);  //Chamada (transforma data em long para fazer comparações)
int numero_de_pessoas(char arquivo[]);  // Chamada (Indentifica o numero de pessoas no arquivo de entrada)
void ler_nome(FILE *arq, char nome[40]);  // Chamada (Faz a leitura de nome no arquivo de entrada)
float media(Funcionario funcionario[], int num); // Case 5-6 (Tira a media de uma lista de funcionarios)
int mais_velho(Funcionario funcionario[], int num);  // Case 3-4 (Identifica a posição do fun. mais velho da lista)
int ordem_alfb_func(const void *s1, const void *s2);  // Case 1-2 função de comparação (Comparação por nome de struct)
void ordem_alfb(Funcionario funcionario[], int num);  // Case 1-2 (Ordena a lista de fun.)
void leitura(Funcionario funcionario[], int num, char arquivo[]);  //Chamada (Seta a lista de funcionarios)
Funcionario * dep(Funcionario funcionario[], int *num, char departamento[]); // Case 2 (Gera lista de fun. por derp.)
void print_funcionarios(Funcionario funcionario[], int num, int i,char arquivo[]); // Print e output
Funcionario set_funcionario(char nome[], float salario, char admissao[], char departamento[]);  // Chamada (Inicia fun.)

int main(int argc, char* argv[]){
    if(argc == 0) printf("Nenhum argumento passado");  // Pequena verificação se algum argumento foi passado
    else args(argv);  // Inicia o programa

    return 0;
}

void args(char* arg[]){
    char qst = (char) arg[2][0];  // Salva qual dos casos foi passado no terminal

    int num = numero_de_pessoas(arg[1]);  // Lê o numero de pessoas no arquivo input
    int valor;

    Funcionario funcionario[num];
    Funcionario *temp1;

    leitura(funcionario, num, arg[1]);  // Gera a lista de funcionarios

    switch (qst){
        case '1':  // Gera arquivo output.txt (3º argumento) com os funcionarios em ordem alfabetica
            ordem_alfb(funcionario, num);
            print_funcionarios(funcionario, num, 0, arg[3]);
            break;
        case '2':  // Printa funcionarios de um determinado departamento em ordem alfabetica
            ordem_alfb(funcionario, num);
            temp1 = dep(funcionario, &num, arg[3]);
            print_funcionarios(temp1, num, 1, NULL);
            break;
        case '3':  // Printa o funcionario mais velho
            valor = mais_velho(funcionario, num);
            temp1 = &funcionario[valor];
            print_funcionarios(temp1, 1, 1, NULL);
            break;
        case '4':  // Printa o funcionario mais velho de um departamento
            temp1 = dep(funcionario, &num, arg[3]);
            valor = mais_velho(temp1, num);
            print_funcionarios(&temp1[valor-1], 1, 1, NULL);
            break;
        case '5':  // Printa a media salarial
            printf("R$%.2f\n\n", media(funcionario, num));
            break;
        case '6':  // Printa a media salarial de um departamento
            temp1 = dep(funcionario, &num, arg[3]);
            printf("R$%.2f\n\n", media(temp1, num));
            break;
        default:
            printf("Segundo argumento menor que 1 ou maior que 6\n");
            break;

    }
}

long data(char data[]){

    time_t t;

    strptime(data, "%d/%m/%Y", &tm);  // Gera uma struct tm "data" a partir de uma string

    tm.tm_isdst = -1;
    t = mktime(&tm);  // Gera a partir de tm um numero referente a data

    return t;
}

int numero_de_pessoas(char arquivo[]){
    int num;

    FILE *arq;

    arq = fopen(arquivo, "r");  // Abre o arquivo em formato leitura
    if (arq == NULL) exit(0);

    fscanf(arq, "%d", &num);  // Lê primeira linha com o numero de funcionarios
    fclose(arq);

    return num;
}

void ler_nome(FILE *arq, char nome[40]){
    int count = 0;  // Inicia contador
    char whl;

    fgetc(arq);  // Lê um caracter do arquivo

    // Loop para ler o nome, para quando encontra a quebra de linha
    while (1){
        whl = fgetc(arq);
        if (whl == '\n') break;
        nome[count] = whl;
        count++;
    }
}

float media(Funcionario funcionario[], int num){
    float media = 0; // Inicia a media

    // Loop para ler todos os salarios da lista de uncionarios e somar na media
    for (int i = 0; i < num; ++i) {
        media += funcionario[i].salario;
    }

    return media/(float)num;
}

void leitura(Funcionario funcionario[], int num, char arquivo[]){
    FILE *arq;
    arq = fopen(arquivo, "r");  // Abre arquivo em modo leitura

    if (arq == NULL) exit(0);  // Se arquivo não for encontrado, encerra o programa

    fscanf(arq, "%d", &num);

    // Ler as linhas e gera a lista de funcionarios
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

    fclose(arq);  // Fecha o arquivo
}

int mais_velho(Funcionario funcionario[], int num){
    long temp = funcionario[0].admissao_lg;  // Inicia a variavel com a primeira dada de admissão no formato long
    int valor = 1;

    // Conpara com outros da lista e retorna o valor
    for (int i = 0; i < num; ++i) {
        if(temp > funcionario[i].admissao_lg){
            temp = funcionario[i].admissao_lg;
            valor = i;
        }
    }

    return valor;
}

int ordem_alfb_func(const void *s1, const void *s2){
    // A função recebe dois ponteiros, struct[i] e struct[i+1], a apartir dai faz a comparação que retorna um int

    Funcionario *e1 = (Funcionario *)s1;  // faz o cast de void* para int*
    Funcionario *e2 = (Funcionario *)s2;

    int compNome = strcmp(e1->nome, e2->nome);  // Faz comparação dos nomes

    return compNome;
}

void ordem_alfb(Funcionario funcionario[], int num){
    qsort(funcionario, num, sizeof(Funcionario), ordem_alfb_func);  //Sorting
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

    Funcionario *temp = malloc(sizeof(Funcionario) * valor1); //Alocar memoria para nova lista de funcionarios

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
