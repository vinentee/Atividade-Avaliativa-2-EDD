#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo a estrutura para um nó da lista
typedef struct Node {
    char artist[100];
    char song[100];
    struct Node* next;
    struct Node* prev;
} Node;

// Função para inserir um nó na lista circular duplamente encadeada
Node* insertNode(Node* tail, char* artist, char* song) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Erro ao alocar memória para um novo nó.\n");
        exit(1);
    }
    strcpy(newNode->artist, artist);
    strcpy(newNode->song, song);

    if (tail == NULL) {
        newNode->next = newNode;
        newNode->prev = newNode;
        tail = newNode;
    } else {
        newNode->next = tail->next;
        newNode->prev = tail;
        tail->next->prev = newNode;
        tail->next = newNode;
        tail = newNode;
    }
    return tail;
}

// Função para exibir a lista pela ordem de cadastro
void displayPlaylist(Node* tail) {
    if (tail == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    Node* current = tail->next;
    do {
        printf("Artista: %s, Música: %s\n", current->artist, current->song);
        current = current->next;
    } while (current != tail->next);
}

// Função para exibir a lista ordenada pelo nome das músicas
void displaySortedPlaylist(Node* tail) {
    if (tail == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    Node* current = tail->next;
    int count = 0;
    while (count < 2) {
        current = current->next;
        count++;
    }
    // Inserção de ordenação por nome da música
    for (Node* i = tail->next; i != tail; i = i->next) {
        for (Node* j = i->next; j != tail->next; j = j->next) {
            if (strcmp(i->song, j->song) > 0) {
                // Swap
                char tempArtist[100];
                char tempSong[100];
                strcpy(tempArtist, i->artist);
                strcpy(tempSong, i->song);
                strcpy(i->artist, j->artist);
                strcpy(i->song, j->song);
                strcpy(j->artist, tempArtist);
                strcpy(j->song, tempSong);
            }
        }
    }
    // Exibição ordenada
    do {
        printf("Artista: %s, Música: %s\n", current->artist, current->song);
        current = current->next;
    } while (current != tail->next);
}

// Função para avançar para a próxima música
Node* nextSong(Node* tail) {
    if (tail == NULL) {
        printf("A playlist está vazia.\n");
        return tail;
    }
    return tail->next;
}

// Função para retornar à música anterior
Node* prevSong(Node* tail) {
    if (tail == NULL) {
        printf("A playlist está vazia.\n");
        return tail;
    }
    return tail;
}

// Função para inserir uma nova música
Node* insertNewSong(Node* tail, char* artist, char* song) {
    FILE* file = fopen("musicas.txt", "a");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo para escrita.\n");
        exit(1);
    }
    fprintf(file, "%s;%s\n", artist, song);
    fclose(file);
    return insertNode(tail, artist, song);
}

// Função para remover uma música
Node* removeSong(Node* tail, char* artist, char* song) {
    if (tail == NULL) {
        printf("A playlist está vazia.\n");
        return tail;
    }

    Node* current = tail->next;
    do {
        if (strcmp(current->artist, artist) == 0 && strcmp(current->song, song) == 0) {
            if (current == tail) {
                tail = tail->prev;
            }
            current->prev->next = current->next;
            current->next->prev = current->prev;
            free(current);
            // Atualizar o arquivo musicas.txt
            FILE* file = fopen("musicas.txt", "w");
            if (file == NULL) {
                fprintf(stderr, "Erro ao abrir o arquivo para escrita.\n");
                exit(1);
            }
            Node* writeCurrent = tail->next;
            do {
                fprintf(file, "%s;%s\n", writeCurrent->artist, writeCurrent->song);
                writeCurrent = writeCurrent->next;
            } while (writeCurrent != tail->next);
            fclose(file);
            return tail;
        }
        current = current->next;
    } while (current != tail->next);
    printf("Música não encontrada na playlist.\n");
    return tail;
}

// Função para buscar uma música
void searchSong(Node* tail, char* song) {
    if (tail == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    Node* current = tail->next;
    int found = 0;
    do {
        if (strcmp(current->song, song) == 0) {
            printf("Música encontrada: Artista: %s, Música: %s\n", current->artist, current->song);
            found = 1;
        }
        current = current->next;
    } while (current != tail->next);
    if (!found) {
        printf("Música não encontrada na playlist.\n");
    }
}

// Função para liberar a memória da lista
void freePlaylist(Node* tail) {
    if (tail == NULL) {
        return;
    }
    Node* current = tail->next;
    do {
        Node* temp = current;
        current = current->next;
        free(temp);
    } while (current != tail->next);
}

int main() {
    Node* tail = NULL;

    // Abrir o arquivo de entrada
    FILE* file = fopen("musicas.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    // Ler os registros do arquivo e criar a lista
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char artist[100], song[100];
        sscanf(line, "%[^;];%[^\n]", artist, song);
        tail = insertNode(tail, artist, song);
    }

    fclose(file);

    int choice;
    char artist[100], song[100];

    do {
        printf("\nMenu:\n");
        printf("1. Exibir playlist pela ordem de cadastro\n");
        printf("2. Exibir playlist ordenadas pelo nome das músicas\n");
        printf("3. Inserir nova música\n");
        printf("4. Remover uma música\n");
        printf("5. Buscar uma música\n");
        printf("6. Avançar para a próxima música\n");
        printf("7. Retornar à música anterior\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayPlaylist(tail);
                break;
            case 2:
                displaySortedPlaylist(tail);
                break;
            case 3:
                printf("Digite o nome do artista: ");
                scanf("%s", artist);
                printf("Digite o nome da música: ");
                scanf("%s", song);
                tail = insertNewSong(tail, artist, song);
                break;
            case 4:
                printf("Digite o nome do artista: ");
                scanf("%s", artist);
                printf("Digite o nome da música: ");
                scanf("%s", song);
                tail = removeSong(tail, artist, song);
                break;
            case 5:
                printf("Digite o nome da música que deseja buscar: ");
                scanf("%s", song);
                searchSong(tail, song);
                break;
            case 6:
                tail = nextSong(tail);
                printf("Música atual: Artista: %s, Música: %s\n", tail->artist, tail->song);
                break;
            case 7:
                tail = prevSong(tail);
                printf("Música atual: Artista: %s, Música: %s\n", tail->artist, tail->song);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (choice != 0);

    // Liberar a memória da lista antes de encerrar o programa
    freePlaylist(tail);

    return 0;
}
