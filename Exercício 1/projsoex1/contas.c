#include "contas.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
 
#define atrasar() sleep(ATRASO)

int contasSaldos[NUM_CONTAS];
int cont_ano = 0, flag = 0;

int contaExiste(int idConta) {
  return (idConta > 0 && idConta <= NUM_CONTAS);
}

void inicializarContas() {
  int i;
  for (i=0; i<NUM_CONTAS; i++)
    contasSaldos[i] = 0;
}

int debitar(int idConta, int valor) {
  atrasar();
  if (!contaExiste(idConta))
    return -1;
  if (contasSaldos[idConta - 1] < valor)
    return -1;
  atrasar();
  contasSaldos[idConta - 1] -= valor;
  return 0;
}

int creditar(int idConta, int valor) {
  atrasar();
  if (!contaExiste(idConta))
    return -1;
  contasSaldos[idConta - 1] += valor;
  return 0;
}

int lerSaldo(int idConta) {
  atrasar();
  if (!contaExiste(idConta))
    return -1;
  return contasSaldos[idConta - 1];
}


void simular(int numAnos) {

  int i, saldoAnterior, saldoLido;
  int vetor[NUM_CONTAS];

  while (cont_ano <= numAnos) {

    printf("\nSIMULACAO: Ano %d\n=================\n", cont_ano);
      for (i = 1; i <= NUM_CONTAS; i++) {

        saldoLido = lerSaldo(i);

        if (cont_ano == 0) {
          printf("Conta %d, Saldo %d\n", i, saldoLido);
          vetor[i-1] = saldoLido;
        } 

        else if (saldoLido > 0) {
          saldoAnterior = saldoNovo(vetor[i-1]);
          printf("Conta %d, Saldo %d\n", i, saldoAnterior);
          vetor[i-1] = saldoAnterior;
        }

        else
          printf("Conta %d, Saldo %d\n", i, saldoLido);
      }
    
    printf("\n");

    if (flag == 0)
      cont_ano += 1;
      
    else 
      cont_ano = numAnos+1;
    
  }

}


/*----------------------------------------------------------------------------*/
/*---------------------------- FUNCOES AUXILIARES ----------------------------*/
/*----------------------------------------------------------------------------*/

int saldoNovo (int saldoAnterior) {
  return max(saldoAnterior * (1 + TAXAJURO) - CUSTOMANUTENCAO, 0);
}

void atualiza (int sig) {
    flag = 1;
}

int max(int valor1, int valor2) {
    if (valor1 >= valor2) return valor1;
    else
        return valor2;
}
