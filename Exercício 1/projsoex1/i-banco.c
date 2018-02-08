/*
// Projeto SO - exercicio 1, version 1
// Sistemas Operativos, DEI/IST/ULisboa 2016-17

//Grupo 7
//Miguel Regouga 83530
//Pedro Caldeira 83539
*/

#include "commandlinereader.h"
#include "contas.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define COMANDO_DEBITAR "debitar"
#define COMANDO_CREDITAR "creditar"
#define COMANDO_LER_SALDO "lerSaldo"
#define COMANDO_SIMULAR "simular"
#define COMANDO_SAIR "sair"
#define COMANDO_SAIR_AGORA "agora"

#define MAXARGS 3
#define BUFFER_SIZE 100
#define STATUS_INFO_LINE_SIZE 50
#define NUM_PROC 20

int proc[NUM_PROC];
int proc_count = 0;

void handler(int sig) {
	if (sig == SIGUSR1) 
	    atualiza(SIGUSR1);
}

int main (int argc, char** argv) {

    char *args[MAXARGS + 1];
    char buffer[BUFFER_SIZE];

    inicializarContas();
    printf("Bem-vinda/o ao i-banco\n\n");
    signal(SIGUSR1, handler);

    while (1) {
        int numargs;
    
        numargs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE);

        /* EOF (end of file) do stdin ou comando "sair" */
        if (numargs < 0 ||
	        (numargs > 0 && (strcmp(args[0], COMANDO_SAIR) == 0))) {

            int status, childpid;

            printf("i-banco vai terminar.\n--\n");

            if (numargs == 2 && (strcmp(args[1], COMANDO_SAIR_AGORA) == 0)) {
            	kill(0, SIGUSR1);
            	printf("Simulacao terminada por signal\n");
            }

            while (proc_count > 0) {
                
                childpid = wait(&status);

                if (WIFEXITED(status)) {
                    printf("FILHO TERMINADO (PID=%d; terminou normalmente)\n", childpid);
                }
                
                else {
                    printf("FILHO TERMINADO (PID=%d; terminou abruptamente)\n", childpid);
                }

                proc_count --;
            }

            printf("--\ni-banco terminou.\n");
            exit(EXIT_SUCCESS);
        }
    
        else if (numargs == 0)
            /* Nenhum argumento; ignora e volta a pedir */
            continue;
            
        /* Debitar */
        else if (strcmp(args[0], COMANDO_DEBITAR) == 0) {
            int idConta, valor;
            if (numargs < 3) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_DEBITAR);
	           continue;
            }

            idConta = atoi(args[1]);
            valor = atoi(args[2]);

            if (debitar (idConta, valor) < 0)
	           printf("%s(%d, %d): Erro\n\n", COMANDO_DEBITAR, idConta, valor);
            else
                printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, idConta, valor);
        }

        /* Creditar */
        else if (strcmp(args[0], COMANDO_CREDITAR) == 0) {
            int idConta, valor;
            if (numargs < 3) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_CREDITAR);
                continue;
            }

            idConta = atoi(args[1]);
            valor = atoi(args[2]);

            if (creditar (idConta, valor) < 0)
                printf("%s(%d, %d): Erro\n\n", COMANDO_CREDITAR, idConta, valor);
            else
                printf("%s(%d, %d): OK\n\n", COMANDO_CREDITAR, idConta, valor);
        }

        /* Ler Saldo */
        else if (strcmp(args[0], COMANDO_LER_SALDO) == 0) {
            int idConta, saldo;

            if (numargs < 2) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_LER_SALDO);
                continue;
            }
            idConta = atoi(args[1]);
            saldo = lerSaldo (idConta);
            if (saldo < 0)
                printf("%s(%d): Erro.\n\n", COMANDO_LER_SALDO, idConta);
            else
                printf("%s(%d): O saldo da conta é %d.\n\n", COMANDO_LER_SALDO, idConta, saldo);
        }

        /* Simular */

        else if (strcmp(args[0], COMANDO_SIMULAR) == 0) {

            int valor;
            int pid = fork();
            
            valor = atoi(args[1]);
            
            if (numargs < 2) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_SIMULAR);
                continue;
            }

            if (pid < 0) {
                perror("Erro no fork\n");
                exit(EXIT_FAILURE);
            }

            else if (pid > 0) { /* Codigo do processo pai */
                proc_count++;
            }

            else { /* Codigo do processo filho */
                simular(valor);
                exit(EXIT_SUCCESS);
            }
        }

        else {
          printf("Comando desconhecido. Tente de novo.\n");
        }
        
    } 
}

