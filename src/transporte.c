#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

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
#define MAX_VERTICES 100
#define INFINITO INT_MAX

typedef struct {
    int **matrizAdj;
    int numVertices;
} Grafo;

typedef struct {
    int id;
    int x;
    int y;
} Vertice;

typedef struct {
    Vertice *vertices[MAX_VERTICES];
    int numVertices;
} ListaVertices;

typedef struct {
    int id;
    Vertice *localizacao;
} Taxi;

typedef struct {
    int id;
    Vertice *localizacao;
} Usuario;

typedef struct {
    Usuario *usuarios[MAX_VERTICES];
    int numUsuarios;
    Taxi *taxis[MAX_VERTICES];
    int numTaxis;
} ListaUsuarioTaxi;

Grafo *criarGrafo(int numVertices) {
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    grafo->numVertices = numVertices;
    grafo->matrizAdj = (int **)malloc(numVertices * sizeof(int *));

    for (int i = 0; i < numVertices; i++) {
        grafo->matrizAdj[i] = (int *)malloc(numVertices * sizeof(int));
        for (int j = 0; j < numVertices; j++) {
            grafo->matrizAdj[i][j] = (i == j) ? 0 : INFINITO;
        }
    }

    return grafo;
}

void adicionarAresta(Grafo *grafo, int inicio, int fim, int peso) {
    if (inicio >= 0 && inicio < grafo->numVertices && fim >= 0 && fim < grafo->numVertices) {
        grafo->matrizAdj[inicio][fim] = peso;
        grafo->matrizAdj[fim][inicio] = peso; // Grafo não direcionado
    }
}

void removerAresta(Grafo *grafo, int inicio, int fim) {
    if (inicio >= 0 && inicio < grafo->numVertices && fim >= 0 && fim < grafo->numVertices) {
        grafo->matrizAdj[inicio][fim] = INFINITO;
        grafo->matrizAdj[fim][inicio] = INFINITO; // Grafo não direcionado
    }
}

ListaUsuarioTaxi *criarListaUsuarioTaxi() {
    ListaUsuarioTaxi *lista = (ListaUsuarioTaxi *)malloc(sizeof(ListaUsuarioTaxi));
    lista->numUsuarios = 0;
    lista->numTaxis = 0;
    return lista;
}

void adicionarUsuario(ListaUsuarioTaxi *lista, int id, Vertice *localizacao) {
    if (lista->numUsuarios < MAX_VERTICES) {
        Usuario *usuario = (Usuario *)malloc(sizeof(Usuario));
        usuario->id = id;
        usuario->localizacao = localizacao;
        lista->usuarios[lista->numUsuarios++] = usuario;
    }
}

void adicionarTaxi(ListaUsuarioTaxi *lista, int id, Vertice *localizacao) {
    if (lista->numTaxis < MAX_VERTICES) {
        Taxi *taxi = (Taxi *)malloc(sizeof(Taxi));
        taxi->id = id;
        taxi->localizacao = localizacao;
        lista->taxis[lista->numTaxis++] = taxi;
    }
}

ListaVertices *criarListaVertices() {
    ListaVertices *lista = (ListaVertices *)malloc(sizeof(ListaVertices));
    lista->numVertices = 0;
    return lista;
}

void adicionarVertice(ListaVertices *lista, int id, int x, int y) {
    if (lista->numVertices < MAX_VERTICES) {
        Vertice *vertice = (Vertice *)malloc(sizeof(Vertice));
        vertice->id = id;
        vertice->x = x;
        vertice->y = y;
        lista->vertices[lista->numVertices++] = vertice;
    }
}

int calcularDistancia(Vertice *vertice1, Vertice *vertice2) {
    // Exemplo simplificado: Distância euclidiana
    return abs(vertice1->x - vertice2->x) + abs(vertice1->y - vertice2->y);
}

void chamarTaxi(ListaUsuarioTaxi *listaUsuariosTaxis, Taxi *taxi, Vertice *destino) {
    int custo = calcularDistancia(listaUsuariosTaxis->usuarios[taxi->id]->localizacao, destino) * 2; // Exemplo de cálculo de custo
    printf("Custo estimado da viagem: %d\n", custo);

    // Atualizar a localização do táxi para o destino
    listaUsuariosTaxis->usuarios[taxi->id]->localizacao = destino;
    printf("Localização atual do táxi: (%d, %d)\n", destino->x, destino->y);
}

void encontrarTaxiMaisProximo(ListaUsuarioTaxi *listaUsuariosTaxis, Vertice *localUsuario, Vertice *destino) {
    Taxi *taxiMaisProximo = NULL;
    int menorDistancia = INFINITO;

    for (int i = 0; i < listaUsuariosTaxis->numTaxis; i++) {
        Taxi *taxi = listaUsuariosTaxis->taxis[i];
        int distancia = calcularDistancia(listaUsuariosTaxis->usuarios[taxi->id]->localizacao, localUsuario);
        if (distancia < menorDistancia) {
            menorDistancia = distancia;
            taxiMaisProximo = taxi;
        }
    }

    Vertice *localizacaoTaxi = listaUsuariosTaxis->usuarios[taxiMaisProximo->id]->localizacao;
    printf("Taxi mais próximo está localizado em (%d, %d)\n", localizacaoTaxi->x, localizacaoTaxi->y);

    // Simulando chamada do táxi após encontrar o mais próximo
    chamarTaxi(listaUsuariosTaxis, taxiMaisProximo, destino);
}

void imprimirCaminho(int caminho[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", caminho[i]);
    }
    printf("\n");
}

void imprimirTodosCaminhosAux(Grafo *grafo, int u, int d, bool visitado[], int caminho[], int *index) {
    visitado[u] = true;
    caminho[(*index)++] = u;

    if (u == d) {
        imprimirCaminho(caminho, *index);
    } else {
        for (int i = 0; i < grafo->numVertices; i++) {
            if (!visitado[i] && grafo->matrizAdj[u][i] != INFINITO) {
                imprimirTodosCaminhosAux(grafo, i, d, visitado, caminho, index);
            }
        }
    }

    (*index)--;
    visitado[u] = false;
}

void imprimirTodosCaminhos(Grafo *grafo, int s, int d) {
    bool *visitado = (bool *)malloc(grafo->numVertices * sizeof(bool));
    int *caminho = (int *)malloc(grafo->numVertices * sizeof(int));
    int index = 0;

    for (int i = 0; i < grafo->numVertices; i++) {
        visitado[i] = false;
    }

    imprimirTodosCaminhosAux(grafo, s, d, visitado, caminho, &index);

    free(visitado);
    free(caminho);
}

int minDistancia(int dist[], bool sptSet[], int V) {
    int min = INFINITO, min_index;

    for (int v = 0; v < V; v++) {
        if (sptSet[v] == false && dist[v] <= min) {
            min = dist[v], min_index = v;
        }
    }

    return min_index;
}

void dijkstra(Grafo *grafo, int src, int dest) {
    int V = grafo->numVertices;
    int dist[V];
    bool sptSet[V];
    int parent[V];

    for (int i = 0; i < V; i++) {
        parent[src] = -1;
        dist[i] = INFINITO;
        sptSet[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistancia(dist, sptSet, V);
        sptSet[u] = true;

        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && grafo->matrizAdj[u][v] && dist[u] != INFINITO && dist[u] + grafo->matrizAdj[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + grafo->matrizAdj[u][v];
            }
        }
    }

    printf("Melhor caminho de %d até %d com custo %d: ", src, dest, dist[dest]);
    int crawl = dest;
    while (parent[crawl] != -1) {
        printf("%d <- ", crawl);
        crawl = parent[crawl];
    }
    printf("%d\n", src);
}

void carregarGrafoDeArquivo(Grafo *grafo, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    int V, E;
    fscanf(arquivo, "%d %d", &V, &E);
    for (int i = 0; i < E; i++) {
        int inicio, fim, peso;
        fscanf(arquivo, "%d %d %d", &inicio, &fim, &peso);
        adicionarAresta(grafo, inicio, fim, peso);
    }
    fclose(arquivo);
}

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

void backofficeMenu()
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
        // Implementar funcionalidade de adicionar nova rua
        printf("Funcionalidade de adicionar nova rua ainda nao implementada.\n");
        break;
    case 2:
        // Implementar funcionalidade de remover rua
        printf("Funcionalidade de remover rua ainda nao implementada.\n");
        break;
    case 3:
        // Implementar funcionalidade de adicionar buracos
        printf("Funcionalidade de adicionar buracos ainda nao implementada.\n");
        break;
    case 4:
        // Implementar funcionalidade de remover buracos
        printf("Funcionalidade de remover buracos ainda nao implementada.\n");
        break;
    case 5:
        // Implementar funcionalidade de imprimir localidade
        printf("Funcionalidade de imprimir localidade ainda nao implementada.\n");
        break;
    case 6:
        // Implementar funcionalidade de imprimir todos os caminhos sem buracos
        printf("Funcionalidade de imprimir todos os caminhos sem buracos ainda nao implementada.\n");
        break;
    case 7:
        // Implementar funcionalidade de cadastrar taxistas
        printf("Funcionalidade de cadastrar taxistas ainda nao implementada.\n");
        break;
    case 8:
        // Imprimir menu principal
        printf("Voltando ao menu principal...\n");
        break;
    default:
        printf("Opcao invalida.\n");
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

void aplicacaoMenu()
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
        // Funcionalidade de cadastrar cliente
        printf("\n \t\t\t Cadastrar cliente\n");
        cadastrarCliente();
        break;
    case 2:
        // Implementar funcionalidade de chamar taxi
        printf("Funcionalidade de chamar taxi ainda nao implementada.\n");
        break;
    case 3:
        // Implementar funcionalidade de imprimir todos os caminhos entre dois pontos
        printf("Funcionalidade de imprimir todos os caminhos entre dois pontos ainda nao implementada.\n");
        break;
    case 4:
        // Implementar funcionalidade de imprimir melhor caminho entre dois pontos
        printf("Funcionalidade de imprimir melhor caminho entre dois pontos ainda nao implementada.\n");
        break;
    case 5:
        // Imprimir menu principal
        printf("Voltando ao menu principal...\n");
        break;
    default:
        printf("Opcao invalida.\n");
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
                backofficeMenu();
            }
            break;
        case 3:
            printf("\n\n");
            if (autenticarUsuario())
            {
                aplicacaoMenu();
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
