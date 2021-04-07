/* Ponto Venda.c
*
* Programa para o cadastro de clientes e compras de um ponto de venda.
*
* Felipe Mathias May (Ciência da Computação - II)
* Geovanna de Oliveira Névola (Ciência da Computação - II)
* Silvio Cesar Barreira Junior (Ciência da Computação - II)
* Mateus Morais (Ciência da Computação - II)
*
* Disciplina: Introdução à Computação-II
*
* 04/12/2019
*/

#include <stdio.h>
#include <stdbool.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

/*--------------------------------------------
                 opções do menu
  --------------------------------------------*/
#define SAIR                   0
#define CADASTRAR_CLIENTE      1
#define LISTAR_CLIENTES        2
#define ALTERAR_CLIENTE        3
#define EXCLUIR_CLIENTE        4
#define CADASTRAR_COMPRA       5
#define LISTAR_COMPRAS_DATA    6
#define LISTAR_COMPRAS_CLIENTE 7
#define EXCLUIR_COMPRA         8

/*-------------------------------------------
             nomes dos arquivos
  -------------------------------------------*/

/* nome do arquivo aonde os clientes são cadastrados */
#define ARQ_CLIENTE "clientes.dat"

/* nome do arquivo aonde deve ser gerada a listagem dos clientes */
#define ARQ_LISTAGEM "clientes.txt"

/* nome do arquivo aonde as compras são cadastradas */
#define ARQ_COMPRA "compras.dat"

/* nome do arquivo para a listagem de compras por data */
#define ARQ_LISTAGEM_COMPRA_DATA "compras_data.txt"

/* nome do arquivo para a listagem de compras por cliente */
#define ARQ_LISTAGEM_COMPRA_CLIENTE "compras_cliente.txt"


/*---------------------------------------------------
                estruturas de dados
  --------------------------------------------------*/
/* estrutura que representa os dados de um cliente */
typedef struct
{
    int codigo;
    char nome[51];
    char telefone[21];
    bool removido;
} tcliente;

/* estrutura que representa uma data */
typedef struct
{
    int dia;
    int mes;
    int ano;
} tdata;

/* estrutura que representa os dados de uma compra */
typedef struct
{
    int cod_compra;
    int cod_cliente;
    tdata dt_compra;
    float valor;
    bool removida;
} tcompra;

/*--------------------------------------------------
              protótipos das funções
  --------------------------------------------------*/
/* função para cadastrar um cliente no arquivo */
void cadastrar_cliente(void);

/* função para listar todos os clientes cadastrados em ordem alfabética */
void listar_clientes(void);

/* função que ordena em ordem alfábética um vetor do tipo tcliente passado como
 * 2º parâmetro de tamanho igual ao inteiro passado como 1º parâmetro da função.
 * Função auxiliar para a função listar_clientes() */
void ordenar_clientes(int, tcliente []);

/* função para alterar o nome e/ou o telefone de um cliente cadastrado */
void alterar_cliente(void);

/* função para excluir um cliente cadastrado, deste que ele não tenha compras
 * cadastradas */
void excluir_cliente(void);

/* função para cadastrar uma compra no arquivo */
void cadastrar_compra(void);

/* função para listar todas as compras realizadas em uma data */
void listar_compras_data(void);

/* função que devolve verdadeiro se as duas datas passadas
 * como argumentos são iguais, ou falso, caso contrário. Função
 * auxiliar para a função listar_compras() */
bool datas_iguais(tdata, tdata);

/* função para lista todas as compras de um cliente */
void listar_compras_cliente(void);

/* função para excluir a compra de uma cliente */
void excluir_compra(void);

int main()
{
    int op;
    setlocale (LC_ALL, "Portuguese");

    do
    {
        /* exibe o menu de opções */
        printf("\n\n\tPONTO DE VENDA - MENU PRINCIPAL\n\n");
        printf("\t<1> Cadastrar cliente\n");
        printf("\t<2> Listar clientes\n");
        printf("\t<3> Alterar cliente\n");
        printf("\t<4> Excluir cliente\n");
        printf("\t<5> Cadastrar compra\n");
        printf("\t<6> Listar compras por data\n");
        printf("\t<7> Listar compras por cliente\n");
        printf("\t<8> Excluir compra\n");
        printf("\t<0> Sair do programa\n");
        printf("\tOpção: ");
        scanf("%d", &op);
        switch (op)
        {
        case CADASTRAR_CLIENTE:
            /* cadastra um cliente no arquivo */
            cadastrar_cliente();
            break;
        case LISTAR_CLIENTES:
            /* lista todos os clientes cadastrados */
            listar_clientes();
            break;
        case ALTERAR_CLIENTE:
            /* altera o nome de um cliente cadastrado */
            alterar_cliente();
            break;
        case EXCLUIR_CLIENTE:
            /* exclui um cliente cadastrado */
            excluir_cliente();
            break;
        case CADASTRAR_COMPRA:
            /* cadastra uma compra no arquivo */
            cadastrar_compra();
            break;
        case LISTAR_COMPRAS_DATA:
            /* lista todas as compras em uma data */
            listar_compras_data();
            break;
        case LISTAR_COMPRAS_CLIENTE:
            /* lista todas as compras de um cliente */
            listar_compras_cliente();
            break;
        case EXCLUIR_COMPRA:
            /* exclui uma comprar cadastrada */
            excluir_compra();
            break;
        case SAIR:
            break;
        default:
            printf("\n\tOpção inválida!\n");
        }
    }
    while (op != SAIR);
    return 0;
}

void cadastrar_cliente()
{
    FILE * arq;
    tcliente c;

    /* verificando abertura de arquivo */
    if ((arq = fopen(ARQ_CLIENTE, "ab")) == NULL)
    {
        fprintf(stderr, "\n\tErro: não foi possivel abrir o arquivo %s\n",ARQ_CLIENTE);
        return;
    }

    printf("\n\n\tCADASTRO DE CLIENTE\n\n");
    fseek(arq, 0, SEEK_END); // verificando o ultimo byte do arquivo
    c.codigo = ftell(arq) / sizeof(tcliente)+1; // contador de codigo do cliente
    printf ("\tConta: %07d\n", c.codigo);
    printf("\tNome do cliente: ");
    scanf(" %59[^\n]", c.nome);
    printf("\tTelefone: ");
    scanf(" %20[^\n]", c.telefone);
    c.removido = false;

    /* gravando dados em um arquivo.dat("clientes.dat") */
    fwrite(&c, sizeof(tcliente), 1, arq);
    fclose(arq);
    printf("\n\tCliente cadastrado com sucesso!\n");

}

void listar_clientes()
{
    FILE * arq;
    int num_cadastrados;

    /* verificando abertura de arquivo */
    if ((arq = fopen(ARQ_CLIENTE, "rb")) == NULL)
    {
        fprintf(stderr, "\n\tErro: não foi possivel abrir o arquivo %s\n",ARQ_CLIENTE);
        return;
    }

    fseek(arq,0,SEEK_END);
    printf("\n\n\tLISTAGEM DE CLIENTES\n");
    num_cadastrados = ftell(arq)/sizeof(tcliente); //obtendo e atribuindo o número total de clientes cadastrados

    /* criando um vetor de estruturas (tcliente), tendo como tamanho
    o número de clientes cadastrados (num_cadastrados) */
    tcliente c[num_cadastrados];
    fseek(arq,0,SEEK_SET);
    fread(&c,sizeof(tcliente),num_cadastrados,arq);
    ordenar_clientes(num_cadastrados,c);

    return;
}

void ordenar_clientes(int n, tcliente clientes[])
{
    FILE * arq;
    FILE * list;

    int op, k, i, j;

    if ((arq = fopen(ARQ_CLIENTE, "rb")) == NULL)
    {
        fprintf(stderr, "\n\tErro: não foi possivel abrir o arquivo %s\n",ARQ_CLIENTE);
        return;
    }

    printf("\n\t<0> Mostrar na tela\n");
    printf("\t<1> Gravar arquivo.txt\n");
    printf("\tOpção: ");
    scanf ("%d", &op);

    i = 0;
    j = 1;
    tcliente aux; // estrutura auxiliar

    while(j < n)
    {
        aux = clientes[j];
        i = j - 1;
        /* comparando os nomes, verificando e assim ordenando, no caso
        em ordem alfabética */
        while((i >= 0) && (strcmp(clientes[i].nome, aux.nome)) > 0)
        {
            clientes[i + 1] = clientes[i];
            i--;
        }
        clientes[i + 1] = aux;
        j++;
    }

    if(op == 0)
    {
        printf("\n\n\t\t        CLIENTES CADASTRADOS\n");
        printf("\t---------------------------------------------------\n");
        printf("\t#Código Nome                             Telefone   \n");
        printf("\t---------------------------------------------------\n");

        fseek(arq,0,SEEK_SET);
        //estrutura de repetição, para imprimindo o vetor
        for (i = 0 ; i < n ; i++)
        {
            if (clientes[i].removido == false)
            {
                printf("\t%07d %-30.30s %-20.20s\n", clientes[i].codigo, clientes[i].nome, clientes[i].telefone);
            }
        }
        printf("\t---------------------------------------------------\n");
        return;
    }
    if (op == 1)
    {
        // imprimindo no arquivo.txt (clientes.txt)
        list = fopen(ARQ_LISTAGEM,"w");
        for (i = 0 ; i < n ; i++)
        {
            fprintf (list,"%d %s %s\n", clientes[i].codigo, clientes[i].nome, clientes[i].telefone);
        }
        printf ("\n\tArquivo.txt gravado com succeso!\n");
    }

    fclose(arq);
    fclose(list);
    return;
}

void alterar_cliente(void)
{
    FILE * arq;
    tcliente c;
    int codigo = 0;

    if ((arq = fopen(ARQ_CLIENTE, "r+b")) == NULL)
    {
        fprintf(stderr, "\n\tErro: não foi possivel abrir o arquivo %s\n",ARQ_CLIENTE);
        return;
    }

    printf("\n\n\tALTERAR CLIENTE\n");
    fseek(arq, 0, SEEK_END);
    rewind(arq);
    printf("\tCódigo do cliente: #");
    scanf("%d", &codigo);

    /* verificando codigo do cliente, caso verdadeiro, alterando o mesmo e armazenando*/
    while(fread(&c, sizeof(tcliente), 1, arq) > 0)
    {
        if (codigo == c.codigo)
        {
            printf("\tNome do cliente: %s\n", c.nome);
            printf("\tNovo nome: ");
            scanf(" %59[^\n]", c.nome);
            printf("\tTelefone atual: %s\n", c.telefone);
            printf("\tNovo telefone: ");
            scanf(" %20[^\n]", c.telefone);
            c.removido = false;
            fseek(arq, -sizeof(tcliente), SEEK_CUR); // redirecionando o ponteiro
            fwrite(&c, sizeof(tcliente), 1, arq);
            fclose(arq);
            printf("\n\tCliente alterado com sucesso!\n");
        }
    }
    if (codigo != c.codigo)
    {
        printf ("\n\tCódigo de cliente ínvalido!\n\n");
    }
}

void excluir_cliente(void)
{
    FILE * arq;
    FILE * cmp;
    tcliente c;
    tcompra compra;
    int codigo, op;

    if ((arq = fopen(ARQ_CLIENTE, "r+b")) == NULL)
    {
        fprintf(stderr, "\n\tErro: não foi possivel abrir o arquivo %s\n",ARQ_CLIENTE);
        return;
    }

    printf("\n\n\tEXCLUIR CLIENTE\n");
    cmp = fopen(ARQ_COMPRA, "rb");
    fseek(arq, 0, SEEK_END);
    fseek(cmp, 0, SEEK_END);
    rewind(arq);
    rewind(cmp);

    printf("\tCódigo do cliente: #");
    scanf ("%d", &codigo);

    /* verificando se o cliente já efetuou uma compra */
    while(fread(&compra, sizeof(tcompra), 1, cmp)> 0)
    {
        if ((codigo == compra.cod_cliente) && (compra.removida == false))
        {
            printf ("\tCliente já efetuou uma compra!");
            fclose(cmp);
            return;
        }
    }

    /* verificando o codigo do cliente, e assim excluindo */
    while(fread(&c, sizeof(tcliente), 1, arq)> 0)
    {
        if (codigo == c.codigo)
        {
            printf("\t---------------------------------------------------\n");
            printf("\t#Código Nome                             Telefone   \n");
            printf("\t---------------------------------------------------\n");
            printf("\t%07d %-30.30s %-20.20s\n", c.codigo, c.nome, c.telefone);
            printf("\t---------------------------------------------------\n");

            printf("\n\n\tEXCLUIR CLIENTE\n");
            printf("\n\t<0> Excluir cliente\n");
            printf("\t<1> Voltar\n");
            printf("\tOpção: ");
            scanf ("%d", &op);

            if (op == 0)
            {
                c.removido = true; // removendo cliente
                fseek(arq, -sizeof(tcliente), SEEK_CUR); // redirecionando o ponteiro
                fwrite(&c, sizeof(tcliente), 1, arq);
                printf ("\n\tCliente excluído com sucesso!\n");
                fclose(arq);
                return;
            }
            else
            {
                printf ("\n\tVoltando...\n\n");
                return;
            }
        }
    }
    if (codigo != c.codigo)
    {
        printf ("\n\tCódigo do cliente ínvalido!\n");
    }

}

void cadastrar_compra(void)
{
    FILE * arq;
    FILE * cmp;
    tcompra compra;
    tcliente c;

    printf("\n\n\tCADASTRAR COMPRA\n");

    cmp = fopen(ARQ_CLIENTE, "rb");
    if ((arq = fopen(ARQ_COMPRA, "ab")) == NULL)
    {
        fprintf(stderr, "\n\tErro: não foi possivel abrir o arquivo %s\n",ARQ_COMPRA);
        return;
    }

    fseek(arq, 0, SEEK_END);
    fseek(cmp, 0, SEEK_END);
    rewind(cmp);
    compra.cod_compra = ftell(arq) / sizeof(tcompra)+1; // contador do codigo da compra

    printf("\n\tCódigo da compra: %06d", compra.cod_compra);
    printf("\n\tCódigo do cliente: ");
    scanf ("%d", &compra.cod_cliente);

    /* percorrendo a estrutura e verificando codigo */
    while(fread(&c, sizeof(tcliente), 1, cmp) > 0)
    {
        if(compra.cod_cliente == c.codigo)
        {
            printf("\tNome do cliente: %s\n", c.nome);
            fclose(cmp);
        }
    }

    if(compra.cod_cliente != c.codigo)
    {
        printf("\n\tError: Código de cliente inválido\n");
        return;
    }

    /* obtendo a data */
    printf("\n\tData da compra: ");
    printf("\n\tDia: ");
    scanf ("%d", &compra.dt_compra.dia);
    printf("\tMês: ");
    scanf ("%d", &compra.dt_compra.mes);
    printf("\tAno: ");
    scanf ("%d", &compra.dt_compra.ano);
    printf ("\n\tValor da compra: R$ ");
    scanf ("%f", &compra.valor);
    compra.removida = false;

    /*gravando em um arquivo.dat (compras.dat) */
    fwrite(&compra, sizeof(tcompra), 1, arq);
    fclose(arq);

    printf("\n\tCompra cadastrada com sucesso!\n");

    return;
}

void listar_compras_data(void)
{
    FILE * arq;
    FILE * cmp;
    FILE * listc;
    tcliente c;
    tcompra compra;
    tdata data1;
    tdata data2;
    int op, vr;
    float total_compra =0;

    arq = fopen(ARQ_CLIENTE, "rb");
    if ((cmp = fopen(ARQ_COMPRA, "rb")) == NULL)
    {
        fprintf(stderr, "\n\tErro: não foi possivel abrir o arquivo %s\n",ARQ_COMPRA);
        return;
    }

    printf("\n\n\tLISTAGEM DE COMPRA\n");
    fseek(arq, 0, SEEK_END);
    fseek(cmp, 0, SEEK_END);
    rewind(arq);
    rewind(cmp);

    /* obtendo data */
    printf("\n\tData da compra: ");
    printf("\n\tDia: ");
    scanf ("%d", &data1.dia);
    printf("\tMês: ");
    scanf ("%d", &data1.mes);
    printf("\tAno: ");
    scanf ("%d", &data1.ano);

    printf("\n\n\tLISTAGEM DE COMPRA\n");
    printf("\t<0> Mostrar compras\n");
    printf("\t<1> Gravar arquivo.txt\n");
    printf("\tOpção: ");
    scanf ("%d", &op);

    if (op == 0)
    {
        /* percorrendo as estruturas tcompra e tcliente, e mostrando as compras efetuadas*/
        while (fread(&compra, sizeof(tcompra), 1, cmp) > 0)
        {
            data2.dia = compra.dt_compra.dia;
            data2.mes = compra.dt_compra.mes;
            data2.ano = compra.dt_compra.ano;

            if ((datas_iguais(data1,data2) == true) && (compra.removida == false))
            {
                while (fread(&c, sizeof(tcliente), 1, arq) > 0)
                {
                    if (c.codigo == compra.cod_cliente)
                    {
                        printf("\n\t--------------------------------------------------------------\n");
                        printf("\t#Compra #Código Nome                          Valor R$        \n");
                        printf("\t--------------------------------------------------------------\n");
                        printf("\t%07d %06d %-30.30s %.2f\n", compra.cod_compra, compra.cod_cliente, c.nome, compra.valor);
                        printf("\t--------------------------------------------------------------\n");
                        total_compra += compra.valor;
                        vr = 1;
                        fseek(arq, 0, SEEK_END);
                        rewind(arq);
                        break;

                    }
                }
            }
        }
        printf ("\n\tValor total das compras: %.2f", total_compra);
    }
    if (vr == 0)
    {
        printf ("\n\tNão há registro de compra na data informada!\n\n");
        fclose(cmp);
        fclose(arq);
        return;
    }

    if (op == 1)
    {
        /* gravando os dados em um arquivo.txt(compra_data.txt) */
        listc = fopen(ARQ_LISTAGEM_COMPRA_DATA,"w");
        while (fread(&compra, sizeof(tcompra), 1, cmp) > 0)
        {
            data2.dia = compra.dt_compra.dia;
            data2.mes = compra.dt_compra.mes;
            data2.ano = compra.dt_compra.ano;

            if ((datas_iguais(data1,data2) == true) && (compra.removida == false))
            {
                while (fread(&c, sizeof(tcliente), 1, arq) > 0)
                {
                    if (c.codigo == compra.cod_cliente)
                    {
                        fprintf(listc, "\nData: %d/%d/%d\n", compra.dt_compra.dia, compra.dt_compra.mes, compra.dt_compra.ano);
                        fprintf(listc, "#Compra #Código Nome                          Valor R$        \n");
                        fprintf(listc, "%07d %06d %-30.30s %.2f\n", compra.cod_compra, compra.cod_cliente, c.nome, compra.valor);
                        total_compra += compra.valor;
                        vr = 1;
                        fseek(arq, 0, SEEK_END);
                        rewind(arq);
                        break;
                    }
                }
            }
        }
        printf ("\n\tArquivo.txt gravado com sucesso!\n");
        fclose(arq);
        fclose(cmp);
        fclose(listc);
    }
}

bool datas_iguais(tdata n1, tdata n2)
{
    FILE * arq;
    FILE * cmp;
    tcliente c;
    tcompra compra;

    arq = fopen(ARQ_CLIENTE, "rb");
    cmp = fopen(ARQ_COMPRA, "rb");

    fseek(arq, 0, SEEK_END);
    fseek(cmp, 0, SEEK_END);
    rewind(arq);
    rewind(cmp);

    /* verificando a data fornecida */
    if ((n1.dia == n2.dia) && (n1.dia == n2.dia) && (n1.dia == n2.dia))
    {
        return true;
    }
    else
    {
        return false;
    }

}

void listar_compras_cliente(void)
{
    FILE * arq;
    FILE * cmp;
    FILE * listcl;
    tcliente c;
    tcompra compra;
    int codigo, op;

    arq = fopen(ARQ_CLIENTE, "rb");
    if ((cmp = fopen(ARQ_COMPRA, "rb")) == NULL)
    {
        fprintf(stderr, "\n\tErro: não foi possivel abrir o arquivo %s\n",ARQ_COMPRA);
        return;
    }

    printf("\n\n\tLISTAGEM DE COMPRA\n");
    fseek(arq, 0, SEEK_END);
    fseek(cmp, 0, SEEK_END);
    rewind(arq);
    rewind(cmp);

    printf("\n\tCodigo do Cliente: ");
    scanf ("%d", &codigo);

    printf("\n\tLISTAGEM DE COMPRA\n");
    printf("\t<0> Mostrar compras\n");
    printf("\t<1> Gravar arquivo.txt\n");
    printf("\tOpção: ");
    scanf ("%d", &op);

    if (op == 0)
    {
        /* percorendo a estrutura tcliente, comparando codigo obtido, e fornecer os dados */
        while (fread(&c, sizeof(tcliente), 1, arq) > 0)
        {
            if (codigo == c.codigo)
            {
                printf("\n\tNome\n");
                printf("\t--------------------------------------------------------------\n");
                printf("\t%-30.30s\n",c.nome);
                break;
            }
        }

        /* percorendo estrutura tcompra, comparando codigo, e fornecer as compras */
        while (fread(&compra, sizeof(tcompra), 1, cmp) > 0)
        {
            if (codigo == compra.cod_cliente && compra.removida == false)
            {
                printf("\t--------------------------------------------------------------\n");
                printf("\t#Compra #Data                                    Valor R$      \n");
                printf("\t--------------------------------------------------------------\n");
                printf("\t%07d %d/%d/%d \t\t\t\t %.2f\n", compra.cod_compra,compra.dt_compra.dia,compra.dt_compra.mes,compra.dt_compra.ano, compra.valor);
                printf("\t--------------------------------------------------------------\n");

            }
        }
    }
    if (op == 1)
    {
        /* gravando em um arquivo.txt(compras_cliente.txt) */
        listcl = fopen(ARQ_LISTAGEM_COMPRA_CLIENTE,"w");
        /* percorendo arquivo e armazenando dados */
        while (fread(&c, sizeof(tcliente), 1, arq) > 0)
        {
            if (codigo == c.codigo)
            {
                fprintf(listcl,"\nNome\n");
                fprintf(listcl,"--------------------------------------------------------------\n");
                fprintf(listcl,"%-30.30s\n\n",c.nome);
                break;
            }
        }

        while (fread(&compra, sizeof(tcompra), 1, cmp) > 0)
        {
            if (codigo == compra.cod_cliente && compra.removida == false)
            {
                fprintf(listcl,"#Compra #Data                                    Valor R$      \n");
                fprintf(listcl,"--------------------------------------------------------------\n");
                fprintf(listcl,"%07d %d/%d/%d \t\t\t\t %.2f\n", compra.cod_compra,compra.dt_compra.dia,compra.dt_compra.mes,compra.dt_compra.ano, compra.valor);
            }
        }
        printf ("\n\tArquivo.txt gravado com sucesso!\n");
        fclose(arq);
        fclose(cmp);
        fclose(listcl);
        return;
    }

}

void excluir_compra(void)
{
    FILE * arq;
    FILE * cmp;
    tcliente c;
    tcompra compra;
    int codigo, op,vr = 0;

    arq = fopen(ARQ_CLIENTE, "rb");
    if ((cmp = fopen(ARQ_COMPRA, "r+b")) == NULL)
    {
        fprintf(stderr, "\n\tErro: não foi possivel abrir o arquivo %s\n",ARQ_COMPRA);
        return;
    }

    printf("\n\n\tEXCLUIR COMPRA\n");
    fseek(arq, 0, SEEK_END);
    fseek(cmp, 0, SEEK_END);
    rewind(arq);
    rewind(cmp);

    printf("\n\tCodigo da compra: ");
    scanf ("%d", &codigo);

    /* percorendo o arquivo, verificando codigos, e validando remoção */
    while (fread(&compra, sizeof(tcompra), 1, cmp) > 0)
    {
        if (codigo == compra.cod_compra && compra.removida == false)
        {
            while (fread(&c, sizeof(tcliente), 1, arq) > 0)
            {
                if(compra.cod_cliente == c.codigo)
                {
                    printf("\n\t------------------------------------------------------------\n");
                    printf("\t#Compra #Código Nome       #Data                    Valor R$   \n");
                    printf("\t--------------------------------------------------------------\n");
                    printf("\t%07d %06d %-20.20s %d/%d/%d\t %.2f\n", compra.cod_compra, compra.cod_cliente, c.nome,compra.dt_compra.dia,compra.dt_compra.mes,compra.dt_compra.ano, compra.valor);
                    printf("\t--------------------------------------------------------------\n");


                    //validação
                    printf("\n\n\tEXCLUIR COMPRA\n");
                    printf("\t<0> Excluir compra\n");
                    printf("\t<1> Voltar\n");
                    printf("\tOpção: ");
                    scanf ("%d", &op);
                    fflush(stdin);
                    if (op == 0)
                    {

                        compra.removida = true;
                        fseek(cmp, (codigo-1)*sizeof(tcompra), SEEK_SET);
                        fwrite(&compra,sizeof(tcompra),1,cmp);
                        printf("\n\tCompra excluída\n");
                        fclose(arq);
                        fclose(cmp);
                        break;
                        return;
                    }
                }
            }
        }
    }

}
