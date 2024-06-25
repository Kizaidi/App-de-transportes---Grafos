#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include "Estrutura.h"
#include "backoffice.h"
// #include "aplicacaoM.h"

// Variáveis globais para armazenar estado de autenticação
int autenticadoAdm = 0;        // 0 para não autenticado, 1 para autenticado
int autenticadoUtilizador = 0; // 0 para não autenticado, 1 para autenticado

// ==================================== GESTAO DE UTILIZADORES ===============================
#define MAX_USUARIOS 100 // Número máximo de usuários que podem ser cadastrados

typedef struct
{
    char nomeUsuario[50];
    char senha[50];
    char tipo; // 'A' para administrador, 'U' para usuário comum
} UsuarioAPP;

UsuarioAPP usuarios[MAX_USUARIOS]; // Array para armazenar os usuários
int numUsuarios = 0;               // Contador de usuários cadastrado
// ============================================ END ===========================================

// ================================================ APLICACAO =================================

#define MAX_CLIENTES 100 // Número máximo de clientes que podem ser cadastrados

typedef struct
{
    char nome[50];
    int localizacao; // Localização do cliente (vértice do grafo)
    char telefone[20];
} Cliente;

Cliente clientes[MAX_CLIENTES]; // Array para armazenar os clientes
int numClientes = 0;            // Contador de clientes cadastrados

// ==================================================== END ==================================

void imprimirMenu()
{
    printf("1. Gestao de usuarios\n");
    printf("2. Backoffice (necessita de autenticacao do adm)\n");
    printf("3. Aplicacao (necessita de autenticacao do usuario)\n");
    printf("4. Sair\n");
    printf("Escolha uma opcao: ");
}

// Função para cadastrar um novo usuário
void cadastrarUsuario()
{
    if (numUsuarios >= MAX_USUARIOS)
    {
        printf("Não é possível cadastrar mais usuários. Limite excedido.\n");
        return;
    }

    printf("Digite o nome de usuário: ");
    scanf(" %[^\n]s", usuarios[numUsuarios].nomeUsuario);

    printf("Digite a senha: ");
    scanf(" %[^\n]s", usuarios[numUsuarios].senha);

    printf("Digite o tipo de usuário (A para administrador, U para usuário comum): ");
    scanf(" %c", &usuarios[numUsuarios].tipo);

    // Verifica se o tipo inserido é válido ('A' ou 'U')
    if (usuarios[numUsuarios].tipo != 'A' && usuarios[numUsuarios].tipo != 'U')
    {
        printf("Tipo de usuário inválido. Use 'A' para administrador ou 'U' para usuário comum.\n");
        return;
    }

    // Abrir o arquivo para escrita em modo append ("a")
    FILE *arquivoUsuarios = fopen("usuarios.txt", "a");
    if (arquivoUsuarios == NULL)
    {
        printf("Erro ao abrir o arquivo de usuários.\n");
        return;
    }

    // Escrever o novo usuário no arquivo
    fprintf(arquivoUsuarios, "%s %s %c\n", usuarios[numUsuarios].nomeUsuario, usuarios[numUsuarios].senha, usuarios[numUsuarios].tipo);

    // Fechar o arquivo
    fclose(arquivoUsuarios);

    numUsuarios++;
    printf("Usuário cadastrado com sucesso.\n");
}

// Função para remover um usuário pelo nome de usuário
void removerUsuario()
{
    if (numUsuarios == 0)
    {
        printf("Nao ha usuarios para remover.\n");
        return;
    }

    char nomeUsuarioRemover[50];
    printf("Digite o nome de usuario a remover: ");
    scanf(" %[^\n]s", nomeUsuarioRemover);

    // Abrir o arquivo para leitura e um arquivo temporário para escrita
    FILE *arquivoUsuarios = fopen("usuarios.txt", "r");
    if (arquivoUsuarios == NULL)
    {
        printf("Erro ao abrir o arquivo de usuarios.\n");
        return;
    }

    FILE *temporario = fopen("temp.txt", "w");
    if (temporario == NULL)
    {
        printf("Erro ao abrir o arquivo temporario.\n");
        fclose(arquivoUsuarios);
        return;
    }

    bool encontrado = false;
    char linha[150]; // Tamanho arbitrário para a linha do arquivo
    while (fgets(linha, sizeof(linha), arquivoUsuarios) != NULL)
    {
        char nomeArquivo[50], senhaArquivo[50], tipoArquivo;
        sscanf(linha, "%s %s %c", nomeArquivo, senhaArquivo, &tipoArquivo);

        // Comparar o nome de usuário para encontrar o usuário a ser removido
        if (strcmp(nomeUsuarioRemover, nomeArquivo) != 0)
        {
            fprintf(temporario, "%s %s %c\n", nomeArquivo, senhaArquivo, tipoArquivo);
        }
        else
        {
            encontrado = true;
        }
    }

    fclose(arquivoUsuarios);
    fclose(temporario);

    // Remover o arquivo original e renomear o arquivo temporário para o original
    remove("usuarios.txt");
    rename("temp.txt", "usuarios.txt");

    if (encontrado)
    {
        // Atualizar o array de usuários removendo o último elemento
        numUsuarios--;
        printf("Usuario removido com sucesso.\n");
    }
    else
    {
        printf("Usuario nao encontrado.\n");
    }
}

// Função para imprimir a lista de usuários cadastrados
void imprimirListaUsuarios()
{
    FILE *arquivoUsuarios = fopen("usuarios.txt", "r");
    if (arquivoUsuarios == NULL)
    {
        printf("Erro ao abrir o arquivo de usuarios.\n");
        return;
    }

    printf("### Lista de Usuarios ###\n");
    char linha[150]; // Tamanho arbitrário para a linha do arquivo
    while (fgets(linha, sizeof(linha), arquivoUsuarios) != NULL)
    {
        char nomeArquivo[50], senhaArquivo[50], tipoArquivo;
        sscanf(linha, "%s %s %c", nomeArquivo, senhaArquivo, &tipoArquivo);
        printf("Nome de Usuario: %s, Senha: %s, Tipo: %c\n", nomeArquivo, senhaArquivo, tipoArquivo);
    }

    fclose(arquivoUsuarios);
}

// Função para carregar os usuários a partir de um arquivo
void carregarUsuarios()
{
    FILE *arquivoUsuarios = fopen("usuarios.txt", "r");
    if (arquivoUsuarios == NULL)
    {
        printf("Erro ao abrir o arquivo de usuarios.\n");
        return;
    }

    while (fscanf(arquivoUsuarios, "%s %s %c\n", usuarios[numUsuarios].nomeUsuario, usuarios[numUsuarios].senha, &usuarios[numUsuarios].tipo) != EOF)
    {
        numUsuarios++;
        if (numUsuarios >= MAX_USUARIOS)
        {
            printf("Limite de usuarios excedido. Alguns usuarios nao foram carregados.\n");
            break;
        }
    }

    fclose(arquivoUsuarios);
}

// Função para autenticar o administrador
bool autenticarAdm()
{
    char nomeUsuario[50];
    char senha[50];

    printf("Digite o nome de usuario: ");
    scanf(" %[^\n]s", nomeUsuario);

    printf("Digite a senha: ");
    scanf(" %[^\n]s", senha);

    for (int i = 0; i < numUsuarios; i++)
    {
        if (usuarios[i].tipo == 'A' && strcmp(usuarios[i].nomeUsuario, nomeUsuario) == 0 && strcmp(usuarios[i].senha, senha) == 0)
        {
            return true; // Administrador autenticado com sucesso
        }
    }
    return false; // Administrador não encontrado ou senha incorreta
}

// Função para autenticar o usuário comum
bool autenticarUsuario()
{

    char nomeUsuario[50];
    char senha[50];

    printf("Digite o nome de usuario: ");
    scanf(" %[^\n]s", nomeUsuario);

    printf("Digite a senha: ");
    scanf(" %[^\n]s", senha);
    for (int i = 0; i < numUsuarios; i++)
    {
        if (usuarios[i].tipo == 'U' && strcmp(usuarios[i].nomeUsuario, nomeUsuario) == 0 && strcmp(usuarios[i].senha, senha) == 0)
        {
            return true; // Usuário autenticado com sucesso
        }
    }
    return false; // Usuário não encontrado ou senha incorreta
}

void gestaoUsuariosMenu()
{
    printf("\n \t\t\t\t\t\t\t Menu de Gestao de Usuarios \n");
    printf("1. Cadastrar novo usuario\n");
    printf("2. Remover usuario\n");
    printf("3. Imprimir lista de usuarios\n");
    printf("4. Voltar ao menu principal\n");
    printf("Escolha uma opcao: ");

    int opcao;
    scanf("%d", &opcao);
    switch (opcao)
    {
    case 1:
        // Funcionalidade de cadastrar novo usuário
        printf("\n \t\t\t Cadastrar novo usuario \n");
        cadastrarUsuario();
        break;
    case 2:
        // Funcionalidade de remover usuário
        printf("\n \t\t\t Remover usuario \n");
        removerUsuario();
        break;
    case 3:
        // Funcionalidade de imprimir lista de usuários
        printf("\n \t\t\t Imprimir lista de usuarios \n");
        imprimirListaUsuarios();
        break;
    case 4:
        // Voltar ao menu principal
        printf("Voltando ao menu principal...\n");
        break;
    default:
        printf("Opcao invalida.\n");
    }
}

void backofficeMenu(Grafo *grafo, ListaUsuarioTaxi *listaUsuariosTaxis, ListaVertices *listaVertices)
{
    while (1)
    {
        printf("\n \t\t\t\t\t\t\t  Menu de Backoffice \n");
        printf("1. Adicionar nova rua\n");
        printf("2. Remover rua\n");
        printf("3. Adicionar buracos\n");
        printf("4. Remover buracos\n");
        printf("5. Imprimir localidade\n");
        printf("6. Imprimir todos os caminhos sem buracos\n");
        printf("7. Cadastrar taxistas\n");
        printf("8. Imprimir menu principal\n");
        printf("Escolha uma opcao: ");

        int opcao;
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            adicionarNovaRua(grafo);
            break;
        case 2:
            removerRua(grafo);
            break;
        case 3:
            adicionarBuracos(grafo);
            break;
        case 4:
            removerBuracos(grafo, /*pesoOriginal*/ 10); // Exemplo de peso original
            break;
        case 5:
            imprimirLocalidade(grafo);
            break;
        case 6:
            imprimirTodosCaminhosSemBuracos(grafo, /*start*/ 0, /*dest*/ 4); // Exemplo de início e destino
            break;
        case 7:
            cadastrarTaxistas(listaUsuariosTaxis, listaVertices);
            break;
        case 8:
            printf("Voltando ao menu principal...\n");
            return;
        default:
            printf("Opcao invalida.\n");
        }
    }
}

// Função para cadastrar um novo cliente, incluindo localização
void cadastrarCliente()
{
    if (numClientes >= MAX_CLIENTES)
    {
        printf("Nao e possivel cadastrar mais clientes. Limite excedido.\n");
        return;
    }

    printf("Digite o nome do cliente: ");
    scanf(" %[^\n]s", clientes[numClientes].nome);

    printf("Digite o telefone do cliente: ");
    scanf(" %[^\n]s", clientes[numClientes].telefone);

    printf("Digite a localizacao do cliente (numero do vertice do grafo): ");
    scanf(" %d", &clientes[numClientes].localizacao);

    // Abrir o arquivo para escrita em modo append ("a")
    FILE *arquivoClientes = fopen("clientes.txt", "a");
    if (arquivoClientes == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    // Escrever o novo cliente no arquivo
    fprintf(arquivoClientes, "%s;%d;%s\n", clientes[numClientes].nome, clientes[numClientes].localizacao, clientes[numClientes].telefone);

    // Fechar o arquivo
    fclose(arquivoClientes);

    numClientes++;
    printf("Cliente cadastrado com sucesso.\n");
}

void aplicacaoMenu(ListaUsuarioTaxi *listaUsuariosTaxis, ListaVertices *listaVertices, Grafo *grafo)
{
    while (1)
    {
        printf("\n \t\t\t\t\t\t\t Menu da Aplicacao \n");
        printf("1. Cadastrar cliente\n");
        printf("2. Chamar taxi\n");
        printf("3. Imprimir todos os caminhos entre dois pontos\n");
        printf("4. Imprimir melhor caminho entre dois pontos\n");
        printf("5. Imprimir menu principal\n");
        printf("Escolha uma opcao: ");

        int opcao;
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("\n \t\t\t Cadastrar cliente\n");
            cadastrarCliente(listaUsuariosTaxis, listaVertices);
            break;
        case 2:
            Vertice localUsuario;
            printf("Informe a localizacao do usuario (x y): ");
            scanf("%d %d", &localUsuario.x, &localUsuario.y);

            Vertice destino;
            printf("Informe o destino desejado (x y): ");
            scanf("%d %d", &destino.x, &destino.y);

            encontrarTaxiMaisProximo(listaUsuariosTaxis, &localUsuario, &destino);

            int idTaxi;
            printf("Informe o ID do táxi para chamar: ");
            scanf("%d", &idTaxi);

            // Encontrar o táxi na lista
            Taxi *taxiChamado = NULL;
            for (int i = 0; i < listaUsuariosTaxis->numTaxis; i++)
            {
                if (listaUsuariosTaxis->taxis[i]->id == idTaxi)
                {
                    taxiChamado = listaUsuariosTaxis->taxis[i];
                    break;
                }
            }

            if (taxiChamado != NULL)
            {
                chamarTaxi(listaUsuariosTaxis, taxiChamado, &destino);
            }
            else
            {
                printf("Táxi com ID %d não encontrado na lista.\n", idTaxi);
            }

            // chamarTaxi(listaUsuariosTaxis, listaVertices, &destino);
            break;
        case 3:
            printf("\n \t\t\t Imprimir todos os caminhos entre dois pontos\n");
            if (grafo->numVertices == 0)
            {
                printf("Grafo vazio. Nenhum caminho disponivel.\n");
            }
            else
            {
                int inicio, fim;
                printf("Digite o ID do ponto de inicio: ");
                scanf("%d", &inicio);
                printf("Digite o ID do ponto de destino: ");
                scanf("%d", &fim);

                if (inicio < 0 || inicio >= grafo->numVertices || fim < 0 || fim >= grafo->numVertices)
                {
                    printf("Vertices de inicio ou destino invalidos.\n");
                }
                else
                {
                    imprimirTodosCaminhos(grafo, inicio, fim);
                }
            }
            break;
        case 4:
            printf("\n \t\t\t Imprimir melhor caminho entre dois pontos\n");
            if (grafo->numVertices == 0)
            {
                printf("Grafo vazio. Nenhum caminho disponivel.\n");
            }
            else
            {
                int inicio, fim;
                printf("Digite o ID do ponto de inicio: ");
                scanf("%d", &inicio);
                printf("Digite o ID do ponto de destino: ");
                scanf("%d", &fim);

                if (inicio < 0 || inicio >= grafo->numVertices || fim < 0 || fim >= grafo->numVertices)
                {
                    printf("Vertices de inicio ou destino invalidos.\n");
                }
                else
                {
                    imprimirTodosCaminhos(grafo, inicio, fim);
                }
            }
            break;
        case 5:
            printf("Voltando ao menu principal...\n");
            return;
        default:
            printf("Opcao invalida.\n");
        }
    }
}

int main()
{
    carregarUsuarios();
    Grafo *grafo = criarGrafo(MAX_VERTICES);
    ListaVertices *listaVertices = criarListaVertices();
    ListaUsuarioTaxi *listaUsuarioTaxi = criarListaUsuarioTaxi();

    carregarGrafoDeArquivo(grafo, "dados.txt");

    int opcao;
    do
    {
        imprimirMenu();
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:

            gestaoUsuariosMenu();
            break;
        case 2:
            printf("\n");
            if (autenticarAdm())
            {
                backofficeMenu(grafo, listaUsuarioTaxi, listaVertices);
            }
            break;
        case 3:
            printf("\n\n");
            if (autenticarUsuario())
            {
                aplicacaoMenu(listaUsuarioTaxi, listaVertices, grafo);
            }
            break;
        case 4:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida.\n");
        }
    } while (opcao != 4);

    return 0;
}
