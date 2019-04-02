/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: carlao
 * N USP 8901184
 * Created on 29 de Março de 2019, 13:03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_REGISTRO 80
#define TAMANHO_CAMPOS_VARIAVEIS 57

/*
 * 
 */
int main() {

    char TAG_CAMPO_CIDADE = '4';
    char TAG_CAMPO_ESCOLA = '5';

    //comando a ser lido
    char comando[100] = "1 csv.csv";
    //varicavel que guarda a opcao selecioanda
    int opc = 0;

    //leitura do comando a ser executado
    //fgets(comando, 100, stdin);

    //pega o tamnho do comando lido
    size_t ln = strlen(comando) - 1;

    //elimina o \n caso houve para ajuda no processamento
    if (comando[ln] == '\n') {
        comando[ln] = '\0';
    }

    //verifica o inteiro digitado no comando
    opc = atoi(strtok(comando, " "));



    if (opc == 1) {

        char * nomeArquivo = strtok(NULL, " ");

        FILE * file = fopen(nomeArquivo, "r+");



        if (file) {
            char *nomeArqWB = "wbfile.bin";

            //cria o arquivo para salvar os dados
            FILE * wbFile = fopen(nomeArqWB, "wb+");


            char buff[200];

            int vez = 1;

            while (!feof(file)) {
                // Lê uma linha (inclusive com o '\n')
                char * result = fgets(buff, 200, file); // o 'fgets' lê até 99 caracteres ou até o '\n'

                //verifica se e uma linha valida
                if (result != NULL) {
                    //printf("%s\n", result);



                    size_t ln = strlen(result) - 1;
                    //remove o enter e o carriage return
                    if (result[ln] == '\n') {
                        result[ln] = '\0';

                        if (result[ln - 1] == '\r') {
                            result[ln - 1] = '\0';
                        }
                    }

                    if (vez == 1) {
                        //recolhe os dados do cabecalho

                    } else {
                        //grava o valor de removido
                        char arr = '-';
                        fwrite(&arr, sizeof (arr), 1, wbFile);

                        //grava o encadeamento
                        int encadeamento = -1;
                        fwrite(&encadeamento, sizeof (encadeamento), 1, wbFile);

                        //começa a ler os dados
                        char * tmp = strsep(&result, ",");

                        int nroInscricao = atoi(tmp);

                        //grava no arquivo binario
                        fwrite(&nroInscricao, sizeof (nroInscricao), 1, wbFile);

                        //pega  anote
                        tmp = strsep(&result, ",");

                        double nota = -1;

                        if (strlen(tmp)) {
                            nota = strtod(tmp, NULL);
                        }
                        //grava no arquivo binario
                        fwrite(&nota, sizeof (nota), 1, wbFile);


                        //printf("%lf\n",nota);

                        tmp = strsep(&result, ",");
                        char data[10] = "\0@@@@@@@@@";

                        if (strlen(tmp)) {
                            strcpy(data, tmp);
                        }

                        fwrite(&data, sizeof (data), 1, wbFile);


                        size_t totalBytes = 27;


                        char * cidade = strsep(&result, ",");

                        int tamanhoCidade = strlen(cidade);
                        if (tamanhoCidade > 0) {

                            //salva o tamanho do campo
                            fwrite(&tamanhoCidade, sizeof (tamanhoCidade), 1, wbFile);

                            //salva a tag do campo
                            fwrite(&TAG_CAMPO_CIDADE, sizeof (char), 1, wbFile);

                            fwrite(cidade, tamanhoCidade, 1, wbFile);

                            totalBytes += 5 + tamanhoCidade;
                        }

                        /*size_t ok = sizeof(cidade);
                        ok = strlen(cidade);
                        break;*/

                        char * nomeEscola = strsep(&result, ",");

                        int tamanhoEscola = strlen(nomeEscola);

                        if (tamanhoEscola > 0) {
                            //salva o tamanho do campo
                            fwrite(&tamanhoEscola, sizeof (tamanhoEscola), 1, wbFile);

                            //salva a tag do campo
                            fwrite(&TAG_CAMPO_ESCOLA, sizeof(char), 1, wbFile);

                            fwrite(nomeEscola, tamanhoEscola, 1, wbFile);

                            totalBytes += 5 + tamanhoEscola;
                        }



                        //escreve @ nos bytes restantes


                        //int TotalBytes = 23;

                        arr = '@';
                        for (; totalBytes < TAMANHO_REGISTRO; totalBytes++) {
                            fwrite(&arr, sizeof (arr), 1, wbFile);
                        }


                        /*if(vez==269){
                            int a = 10;
                            break;
                        }*/
                    }
                }

                result = NULL;
                vez++;


            }

            printf("%p", wbFile);
            fclose(wbFile);
            fclose(file);


            //char buff2[200];
            char arr;
            int encadeamento;
            int nroInscricao;
            double nota;
            char data[10];
            char * cidade;
            char * nomeEscola;
            char tmp[TAMANHO_CAMPOS_VARIAVEIS];

            file = fopen(nomeArqWB, "rb");
            fread(&arr, sizeof (arr), 1, file);
            fread(&encadeamento, sizeof (encadeamento), 1, file);
            fread(&nroInscricao, sizeof (nroInscricao), 1, file);
            fread(&nota, sizeof (nota), 1, file);

            fread(&tmp, sizeof (data), 1, file);
            strcpy(data, tmp);

            fread(&tmp, TAMANHO_CAMPOS_VARIAVEIS, 1, file);

            cidade = strtok(tmp, "\\0");
            nomeEscola = strtok(NULL, "\\0");

            printf("%d\n%lf\n%s\n", nroInscricao, nota, data);
            //printf("%s\n",tmp);

            //size_t a = sizeof (cidade);

            fclose(file);
            printf("Acabou");
        } else {
            printf("Falha no carregamento do arquivo.");
        }

    }


    return (EXIT_SUCCESS);
}

