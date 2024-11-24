#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUTOS 100
#define MAX_CLIENTES 100
#define ESTOQUE_BAIXO 3 // Quantidade para considerar estoque baixo

typedef struct {
    int id;
    char nome[50];
    char categoria[20]; // jogos, consoles, artigos, etc.
    float preco;
    int quantidade;
} Produto;

typedef struct {
    int id;
    char nome[50];
    char email[50];
} Cliente;

typedef struct {
    int produtoId;
    int quantidade;
    float valorTotal;
} Venda;

Produto produtos[MAX_PRODUTOS];
Cliente clientes[MAX_CLIENTES];
Venda vendas[MAX_PRODUTOS];
int totalProdutos = 0;
int totalClientes = 0;
int totalVendas = 0;

// Funções auxiliares
int buscarProdutoPorId(int id) {
    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].id == id) return i;
    }
    return -1;
}

// Funções de cadastro e listagem
void cadastrarProduto() {
    if (totalProdutos >= MAX_PRODUTOS) {
        printf("Limite de produtos atingido.\n");
        return;
    }

    Produto p;
    p.id = totalProdutos + 1;

    printf("Nome do produto: ");
    scanf(" %[^\n]", p.nome);
    printf("Categoria (jogos, consoles, artigos): ");
    scanf(" %[^\n]", p.categoria);
    printf("Preço: R$ ");
    scanf("%f", &p.preco);
    printf("Quantidade em estoque: ");
    scanf("%d", &p.quantidade);

    produtos[totalProdutos++] = p;
    printf("Produto cadastrado com sucesso!\n");
}

void listarProdutos() {
    printf("\n--- Lista de Produtos ---\n");
    for (int i = 0; i < totalProdutos; i++) {
        printf("ID: %d | Nome: %s | Categoria: %s | Preco: R$ %.2f | Estoque: %d\n",
               produtos[i].id, produtos[i].nome, produtos[i].categoria, produtos[i].preco, produtos[i].quantidade);
    }
    printf("\n");
}

// Função de alerta de estoque baixo
void alertarEstoqueBaixo() {
    int alertaEncontrado = 0;
    printf("\n--- Produtos com Estoque Baixo ---\n");
    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].quantidade < ESTOQUE_BAIXO) {
            printf("ID: %d | Nome: %s | Estoque Atual: %d\n",
                   produtos[i].id, produtos[i].nome, produtos[i].quantidade);
            alertaEncontrado = 1;
        }
    }
    if (!alertaEncontrado) {
        printf("Nenhum produto com estoque baixo.\n");
    }
    printf("\n");
}

// Funções de vendas
void realizarVenda() {
    int id, quantidade, indice;
    printf("\n--- Realizar Venda ---\n");
    printf("ID do produto: ");
    scanf("%d", &id);

    indice = buscarProdutoPorId(id);
    if (indice == -1) {
        printf("Produto nao encontrado.\n");
        return;
    }

    printf("Quantidade: ");
    scanf("%d", &quantidade);

    if (produtos[indice].quantidade < quantidade) {
        printf("Estoque insuficiente.\n");
        return;
    }

    produtos[indice].quantidade -= quantidade;

    Venda v;
    v.produtoId = id;
    v.quantidade = quantidade;
    v.valorTotal = produtos[indice].preco * quantidade;
    vendas[totalVendas++] = v;

    printf("Venda realizada com sucesso! Valor total: R$ %.2f\n", v.valorTotal);
}

void gerarRelatorio() {
    printf("\n--- Relatorio de Vendas ---\n");
    float totalVendasRelatorio = 0;

    for (int i = 0; i < totalVendas; i++) {
        int indice = buscarProdutoPorId(vendas[i].produtoId);
        printf("Produto: %s | Quantidade: %d | Valor Total: R$ %.2f\n",
               produtos[indice].nome, vendas[i].quantidade, vendas[i].valorTotal);
        totalVendasRelatorio += vendas[i].valorTotal;
    }

    printf("Total Geral de Vendas: R$ %.2f\n", totalVendasRelatorio);
}

// Funções de arquivos
void salvarDados() {
    FILE *arquivo = fopen("dados.txt", "w");
    if (!arquivo) {
        printf("Erro ao salvar dados.\n");
        return;
    }

    fprintf(arquivo, "%d\n", totalProdutos);
    for (int i = 0; i < totalProdutos; i++) {
        fprintf(arquivo, "%d;%s;%s;%.2f;%d\n", produtos[i].id, produtos[i].nome, produtos[i].categoria,
                produtos[i].preco, produtos[i].quantidade);
    }

    fclose(arquivo);
    printf("Dados salvos com sucesso.\n");
}

void carregarDados() {
    FILE *arquivo = fopen("dados.txt", "r");
    if (!arquivo) {
        printf("Nenhum dado encontrado para carregar.\n");
        return;
    }

    fscanf(arquivo, "%d\n", &totalProdutos);
    for (int i = 0; i < totalProdutos; i++) {
        fscanf(arquivo, "%d;%[^;];%[^;];%f;%d\n", &produtos[i].id, produtos[i].nome, produtos[i].categoria,
               &produtos[i].preco, &produtos[i].quantidade);
    }

    fclose(arquivo);
    printf("Dados carregados com sucesso.\n");
}

void gerarArquivoExcel() {
    FILE *arquivo = fopen("estoque.csv", "w");
    if (!arquivo) {
        printf("Erro ao gerar arquivo Excel.\n");
        return;
    }

    fprintf(arquivo, "ID;Nome;Categoria;Preço;Quantidade\n");
    for (int i = 0; i < totalProdutos; i++) {
        fprintf(arquivo, "%d;%s;%s;%.2f;%d\n", produtos[i].id, produtos[i].nome, produtos[i].categoria,
                produtos[i].preco, produtos[i].quantidade);
    }

    fclose(arquivo);
    printf("Arquivo Excel gerado com sucesso: estoque.csv\n");
}

// Função de menu
void menu() {
    int opcao;
    do {
        printf("\n--- Sistema de Loja de Video-Games ---\n");
        printf("1. Cadastrar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Realizar Venda\n");
        printf("4. Gerar Relatorio de Vendas\n");
        printf("5. Alertar Estoque Baixo\n");
        printf("6. Salvar Dados\n");
        printf("7. Carregar Dados\n");
        printf("8. Gerar Arquivo Excel\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarProduto();
                break;
            case 2:
                listarProdutos();
                break;
            case 3:
                realizarVenda();
                break;
            case 4:
                gerarRelatorio();
                break;
            case 5:
                alertarEstoqueBaixo();
                break;
            case 6:
                salvarDados();
                break;
            case 7:
                carregarDados();
                break;
            case 8:
                gerarArquivoExcel();
                break;
            case 0:
                printf("Encerrando o sistema...\n");
                break;
            default:
                printf("Opção invalida.\n");
        }
    } while (opcao != 0);
}

int main() {
    carregarDados(); // Carrega dados ao iniciar o sistema
    menu();
    salvarDados(); // Salva dados ao sair do sistema
    return 0;
}
