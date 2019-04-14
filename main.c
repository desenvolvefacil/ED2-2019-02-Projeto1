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

#define TAMANHO_PAGINA 16000
#define TAMANHO_REGISTRO 80
#define NOME_ARQUIVO_WB_SAVE "arquivoTrab1si.bin"
#define REMOVIDO '*'
#define NAO_REMOVIDO '-'
#define TAG_CAMPO_CIDADE '4'
#define TAG_CAMPO_ESCOLA '5'
#define ARQUIVO_ABERTO_ESCRITA '0'
#define ARQUIVO_FECHADO_ESCRITA '1'

/*
//comando a ser lido
1 csv.csv
2 arquivoTrab1si.bin
3 arquivoTrab1si.bin nroInscricao 332
3 arquivoTrab1si.bin nota 561.3
3 arquivoTrab1si.bin data 23/01/2017
3 arquivoTrab1si.bin cidade Recife
3 arquivoTrab1si.bin cidade Joao Pessoa    
3 arquivoTrab1si.bin nomeEscola FRANCISCO RIBEIRO CARRI
3 arquivoTrab1si.bin nomeEscola FRANCISCO RIBEIRO CARRI
4 arquivoTrab1si.bin 4999
 */

/**
 *  Funcao que imprime os dados em tela
 * @param nroInscricao
 * @param nota
 * @param data
 * @param cidade
 * @param nomeEscola
 */
void imprimirTela(int nroInscricao, double nota, char * data, char * cidade, char * nomeEscola) {
    //imprime o número de inscrição
    printf("%d", nroInscricao);

    //se nota>=0 imprime
    if (nota >= 0) {
        printf(" %.1lf", nota);
    }

    //se tiver caracteres na data imprime
    if (strlen(data) > 0) {
        printf(" %s", data);
    }

    //verifica se tem cidade
    if (cidade != NULL && strlen(cidade) > 0) {
        printf(" %d %s", (int) strlen(cidade), cidade);
    }

    //verifica se tem escola
    if (nomeEscola != NULL && strlen(nomeEscola) > 0) {
        printf(" %d %s", (int) strlen(nomeEscola), nomeEscola);
    }

    printf("\n");
}

/**
 * Lê uma linha do arquivo binario e retorna os dados da mesma
 * @param fileWb
 * @param RRN
 * @param removido
 * @param nroInscricao
 * @param nota
 * @param data
 * @param cidade
 * @param nomeEscola
 * @return leu a linha
 */
int lerLinha(FILE * fileWb, long RRN, char * removido, int * nroInscricao, double * nota, char * data, char * cidade, char * nomeEscola) {

    int encadeamento;

    //char * auxTexto;
    int auxTamanho;
    char auxTagCampo;

    //posicao do proximo registro
    long pular = TAMANHO_PAGINA + RRN * TAMANHO_REGISTRO;

    //posicao atual do ponteiro no arquivo
    long posAtual = ftell(fileWb);

    //ajusta o tamanho do salto tirando o valor atual do ponteiro do registro a ser obtido
    pular -= posAtual;

    int seek = fseek(fileWb, pular, SEEK_CUR);


    //pega o atributo para verificar se o registro esta excluido logicamente
    int read = fread(removido, sizeof (char), 1, fileWb);

    //verifica se o registro não esta excluido e imprime em tela
    if (read) {
        fread(&encadeamento, sizeof (int), 1, fileWb);

        fread(nroInscricao, sizeof (int), 1, fileWb);

        fread(nota, sizeof (double), 1, fileWb);

        fread(data, 10, 1, fileWb);


        read = fread(&auxTamanho, sizeof (int), 1, fileWb);

        if (read) {
            //le a tag
            fread(&auxTagCampo, sizeof (char), 1, fileWb);

            auxTamanho--;
            
            //verifica se é uma tagValida
            if (auxTagCampo == TAG_CAMPO_CIDADE) {

                fread(cidade, auxTamanho, 1, fileWb);

                read = fread(&auxTamanho, sizeof (int), 1, fileWb);

                auxTamanho--;
                
                if (read) {
                    //le a tag
                    fread(&auxTagCampo, sizeof (char), 1, fileWb);

                    //verifica se é uma tag validae
                    if (auxTagCampo == TAG_CAMPO_ESCOLA) {
                        //nomeEscola = calloc(auxTamanho, sizeof (char));
                        fread(nomeEscola, auxTamanho, 1, fileWb);

                        int a = 10;
                    }

                }

            } else if (auxTagCampo == TAG_CAMPO_ESCOLA) {
                //nomeEscola = calloc(auxTamanho, sizeof (char));
                fread(nomeEscola, auxTamanho, 1, fileWb);
            }
        }
    }


    return * nroInscricao;
}

/**
 * Abre um arquivo e verifica sua integridade
 * @param nomeArquivo
 * @param modo
 * @return 
 */
FILE * abrirArquivoLeitura(char * nomeArquivo, char * modo){
    FILE * file = fopen(nomeArquivo,modo);
    
    if(file){
        char status = ARQUIVO_ABERTO_ESCRITA;
        
        //le o primeiro char para verificar a integridade
        fread(&status,sizeof(char),1,file);
        
        if(status==ARQUIVO_ABERTO_ESCRITA){
            fclose(file);
            file=NULL;
        }
        
    }
    
    return file;
}


/*
 * Função Principal
 */
int main() {

    //comando a ser lido
    char * comando = calloc(100, sizeof (char));
    //strcpy(comando,"3 arquivoTrab1si.bin nomeEscola FRANCISCO RIBEIRO CARRI\0");

    //varicavel que guarda a opcao selecioanda
    int opc = 0;



    //leitura do comando a ser executado
    fgets(comando, 100, stdin);

    //pega o tamnho do comando lido
    size_t ln = strlen(comando) - 1;

    //elimina o \n caso houve para ajuda no processamento
    if (comando[ln] == '\n') {
        comando[ln] = '\0';
    }


    //verifica o inteiro digitado no comando
    opc = atoi(strsep(&comando, " "));

    // exemplo de comando
    // 1 arquivo.csv 
    if (opc == 1) {

        char * nomeArquivo = strsep(&comando, "\0");

        FILE * file = fopen(nomeArquivo, "r");

        if (file) {


            //cria o arquivo para salvar os dados
            FILE * wbFile = fopen(NOME_ARQUIVO_WB_SAVE, "wb");

            char buff[200];

            int vez = 1;

            while (!feof(file)) {
                // Lê uma linha (inclusive com o '\n')
                char * result = fgets(buff, 200, file); // o 'fgets' lê até 199 caracteres ou até o '\n'

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

                        //grava o status como arquivo aberto pra escrita
                        char status = ARQUIVO_ABERTO_ESCRITA;
                        fwrite(&status, sizeof (status), 1, wbFile);

                        //grava o topo da pilha
                        int topoPilha = -1;
                        fwrite(&topoPilha, sizeof (topoPilha), 1, wbFile);

                        //grava os dados do campo 1
                        char tagCampo1 = '1';
                        fwrite(&tagCampo1, sizeof (tagCampo1), 1, wbFile);

                        char desCampo1[55] = "numero de inscricao do participante do ENEM\0@@@@@@@@@@@";
                        fwrite(&desCampo1, sizeof (desCampo1), 1, wbFile);

                        //grava os dados do campo 2
                        char tagCampo2 = '2';
                        fwrite(&tagCampo2, sizeof (tagCampo2), 1, wbFile);

                        char desCampo2[55] = "nota do participante do ENEM na prova de matematica\0@@@";
                        fwrite(&desCampo2, sizeof (desCampo2), 1, wbFile);

                        //grava os dados do campo 3
                        char tagCampo3 = '3';
                        fwrite(&tagCampo3, sizeof (tagCampo3), 1, wbFile);

                        char desCampo3[55] = "data\0@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
                        fwrite(&desCampo3, sizeof (desCampo3), 1, wbFile);


                        //grava os dados do campo 4
                        char tagCampo4 = '4';
                        fwrite(&tagCampo4, sizeof (tagCampo4), 1, wbFile);

                        char desCampo4[55] = "cidade na qual o participante do ENEM mora\0@@@@@@@@@@@@";
                        fwrite(&desCampo4, sizeof (desCampo4), 1, wbFile);


                        //grava os dados do campo 4
                        char tagCampo5 = '5';
                        fwrite(&tagCampo5, sizeof (tagCampo5), 1, wbFile);

                        char desCampo5[55] = "nome da escola de ensino medio\0@@@@@@@@@@@@@@@@@@@@@@@@";
                        fwrite(&desCampo5, sizeof (desCampo5), 1, wbFile);


                        char arroba = '@';

                        //for para completar com @ e deixar o cabeçalho em uma pagina só
                        int i;

                        for (i = 285; i < TAMANHO_PAGINA; i++) {
                            fwrite(&arroba, sizeof (arroba), 1, wbFile);
                        }
                        //break;

                    } else {
                        //grava o valor de removido
                        char removido = NAO_REMOVIDO;
                        fwrite(&removido, sizeof (removido), 1, wbFile);

                        //grava o encadeamento
                        int encadeamento = -1;
                        fwrite(&encadeamento, sizeof (encadeamento), 1, wbFile);

                        //começa a ler os dados
                        char * tmp = strsep(&result, ",");

                        int nroInscricao = atoi(tmp);

                        /*if (nroInscricao == 13185) {
                            int a = 10;
                        }*/

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

                        tmp = strsep(&result, ",");
                        char data[10] = "\0@@@@@@@@@";

                        if (strlen(tmp)) {
                            strncpy(data, tmp, sizeof (data));
                        }

                        fwrite(&data, sizeof (data), 1, wbFile);

                        size_t totalBytes = 27;

                        char * cidade = strsep(&result, ",");

                        //add 1 para o \0
                        int tamanhoCidade = strlen(cidade)+1;
                        if (tamanhoCidade > 1) {
                            //concatena com \0
                            cidade = strncat(cidade, "\0", tamanhoCidade);

                            //add o char tagCampoCidade
                            tamanhoCidade++;
                            //salva o tamanho do campo
                            fwrite(&tamanhoCidade, sizeof (tamanhoCidade), 1, wbFile);

                            //remove o char tagCampoCIdade
                            tamanhoCidade--;
                            
                            //salva a tag do campo
                            char tagCampoCidade = TAG_CAMPO_CIDADE;
                            fwrite(&tagCampoCidade, sizeof (char), 1, wbFile);

                            fwrite(cidade, tamanhoCidade, 1, wbFile);

                            //5 = int + tagCampoCIdade
                            totalBytes += 5 + tamanhoCidade;
                        }

                        char * nomeEscola = strsep(&result, ",");

                        //soma 1 do \0
                        int tamanhoEscola = strlen(nomeEscola)+1;

                        if (tamanhoEscola > 1) {

                            //concatena com \0
                            nomeEscola = strncat(nomeEscola, "\0", tamanhoEscola);

                            //soma 1 do char tagCampoEscola
                            tamanhoEscola++;
                            
                            //salva o tamanho do campo
                            fwrite(&tamanhoEscola, sizeof (tamanhoEscola), 1, wbFile);

                            //remove 1 do tagCampoEscola
                            tamanhoEscola--;
                            
                            //salva a tag do campo
                            char tagCampoEscola = TAG_CAMPO_ESCOLA;
                            fwrite(&tagCampoEscola, sizeof (char), 1, wbFile);

                            fwrite(nomeEscola, tamanhoEscola, 1, wbFile);

                            //5 = int tamanho + char tag
                            totalBytes += 5 + tamanhoEscola;
                        }

                        char arroba = '@';
                        for (; totalBytes < TAMANHO_REGISTRO; totalBytes++) {
                            fwrite(&arroba, sizeof (arroba), 1, wbFile);
                        }
                    }
                }

                result = NULL;
                vez++;



            }

            //atualiza o status pra 0
            //posiciona o cursor pro inicio do arquivo
            fseek(wbFile, 0, SEEK_SET);
            char status = ARQUIVO_FECHADO_ESCRITA;
            fwrite(&status, sizeof (char), 1, wbFile);

            //printf("%p", wbFile);
            fclose(wbFile);
            fclose(file);

            printf(NOME_ARQUIVO_WB_SAVE);

        } else {
            printf("Falha no carregamento do arquivo.");
        }
    } else if (opc == 2) {
        char * nomeArquivo = strsep(&comando, "\0");

        //exit(0);
        //char buff2[200];

        long vez = 0;

        FILE *fileWb = abrirArquivoLeitura(nomeArquivo, "rb");

        if (fileWb) {

            while (!feof(fileWb)) {
                char removido;
                //int encadeamento;
                int nroInscricao = 0;
                double nota = -1;
                char data[11] = "\0";
                //data[10] = '\0';

                char cidade[100] = "\0"; // = NULL;
                char nomeEscola[100] = "\0"; // = NULL;


                if (lerLinha(fileWb, vez, &removido, &nroInscricao, &nota, data, cidade, nomeEscola)) {

                    if (removido == NAO_REMOVIDO) {
                        imprimirTela(nroInscricao, nota, data, cidade, nomeEscola);
                    }
                }
                //exit(0);
                //printf("\n");

                vez++;
            }




            /*if (vez >= 4999) {
                int a = 10;
            }*/


            fclose(fileWb);


            //Calcula qtas paginas foram acessadas
            long totalBytes = TAMANHO_REGISTRO * (vez);

            int totalPaginasAcessadas = totalBytes / TAMANHO_PAGINA;

            int diff = totalBytes % TAMANHO_PAGINA;

            totalPaginasAcessadas += (diff > 0) ? 1 : 0;

            printf("Número de páginas de disco acessadas: %d", totalPaginasAcessadas);
        } else {
            printf("Falha no processamento do arquivo.");
        }



        //printf("Acabou");
    } else if (opc == 3) {
        //3 arquivo.bin NomeDoCampo valor
        //3 arquivo.bin nroInscricao 332
        char * nomeArquivo = strsep(&comando, " ");

        char * parametroNome = strsep(&comando, " ");

        char * parametroValor = strsep(&comando, "\0");

        //tenta pegar o resto do parametro caso haja
        //printf("%s\n",comando);
        //char * aux = NULL;


        //printf("%s",parametroValor);

        //nome dos campos para comparacao
        char NRO_INSCRICAO[] = "nroInscricao";
        char NOTA[] = "nota";
        char DATA[] = "data";
        char CIDADE[] = "cidade";
        char NOME_ESCOLA[] = "nomeEscola";


        int erro = 0;
        int imprimiu = 0;

        //verifica se é um parametro válido
        if (strcmp(parametroNome, NRO_INSCRICAO) == 0 || strcmp(parametroNome, NOTA) == 0 || strcmp(parametroNome, DATA) == 0 || strcmp(parametroNome, CIDADE) == 0 || strcmp(parametroNome, NOME_ESCOLA) == 0) {
            //printf("ok");

            long vez = 0;

            FILE *fileWb = abrirArquivoLeitura(nomeArquivo, "rb");

            if (fileWb) {

                while (!feof(fileWb)) {
                    char removido;
                    //int encadeamento;
                    int nroInscricao = 0;
                    double nota = -1;
                    char data[11] = "\0";
                    //data[10] = '\0';

                    char cidade[100] = "\0"; // = NULL;
                    char nomeEscola[100] = "\0"; // = NULL;


                    if (lerLinha(fileWb, vez, &removido, &nroInscricao, &nota, data, cidade, nomeEscola)) {

                        if (removido == NAO_REMOVIDO) {


                            //verificar se o valor corresponde ao parametro
                            if (strcmp(parametroNome, NRO_INSCRICAO) == 0) {
                                int nroAux = atoi(parametroValor);

                                if (nroInscricao == nroAux) {
                                    imprimirTela(nroInscricao, nota, data, cidade, nomeEscola);
                                    imprimiu = 1;
                                    vez += 2;
                                    break;
                                }
                            }

                            if (strcmp(parametroNome, NOTA) == 0) {
                                double notaAux = strtod(parametroValor, NULL);

                                if (notaAux >= 0 && notaAux == nota) {
                                    imprimirTela(nroInscricao, nota, data, cidade, nomeEscola);
                                    imprimiu = 1;
                                }
                            }

                            if (strcmp(parametroNome, DATA) == 0) {
                                if (strcmp(parametroValor, data) == 0) {
                                    imprimirTela(nroInscricao, nota, data, cidade, nomeEscola);
                                    imprimiu = 1;
                                }
                            }

                            if (strcmp(parametroNome, CIDADE) == 0) {
                                if (strcmp(parametroValor, cidade) == 0) {
                                    imprimirTela(nroInscricao, nota, data, cidade, nomeEscola);
                                    imprimiu = 1;
                                }
                            }

                            if (strcmp(parametroNome, NOME_ESCOLA) == 0) {
                                if (strcmp(parametroValor, nomeEscola) == 0) {
                                    imprimirTela(nroInscricao, nota, data, cidade, nomeEscola);
                                    imprimiu = 1;
                                }
                            }

                        }
                    }
                    //exit(0);
                    //printf("\n");

                    vez++;
                }


                if (imprimiu) {
                    //Calcula qtas paginas foram acessadas
                    //soma uma pagina para contar o cabecallho
                    long totalBytes = TAMANHO_REGISTRO * (vez - 1) + TAMANHO_PAGINA;

                    int totalPaginasAcessadas = totalBytes / TAMANHO_PAGINA;

                    int diff = totalBytes % TAMANHO_PAGINA;

                    totalPaginasAcessadas += (diff > 0) ? 1 : 0;

                    printf("Número de páginas de disco acessadas: %d", totalPaginasAcessadas);
                } else {
                    printf("Registro inexistente.");
                }


            } else {
                erro == 1;
            }

        } else {
            erro = 1;
        }


        if (erro) {
            printf("Falha no processamento do arquivo");
        }


        //printf("ok");
    } else if (opc == 4) {
        char * nomeArquivo = strsep(&comando, " ");

        long RRN = -1;
        RRN = atoi(strsep(&comando, "\0"));

        int erro = 0;
        //printf("%u",RRN);


        if (RRN >= 0) {

            FILE *fileWb = abrirArquivoLeitura(nomeArquivo, "rb");

            if (fileWb) {

                char removido;
                //int encadeamento;
                int nroInscricao = 0;
                double nota = -1;
                char data[11] = "\0";
                //data[10] = '\0';

                char cidade[100] = "\0"; // = NULL;
                char nomeEscola[100] = "\0"; // = NULL;

                if (lerLinha(fileWb, RRN, &removido, &nroInscricao, &nota, data, cidade, nomeEscola)) {

                    if (removido == NAO_REMOVIDO) {
                        imprimirTela(nroInscricao, nota, data, cidade, nomeEscola);
                        //2 cabcalho + seek direto pro registro
                        printf("Número de páginas de disco acessadas: 2");
                    } else {
                        printf("Registro inexistente.");
                    }
                } else {
                    printf("Registro inexistente.");
                }



            } else {
                erro = 1;
            }



        } else {
            erro = 1;
        }

        if (erro) {
            printf("Falha no processamento do arquivo");
        }


    } else {
        printf("Opcao Invalida");
    }


    return (0);
}
