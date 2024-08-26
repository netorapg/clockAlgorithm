#include <stdio.h>
#include <stdlib.h>

#define FRAME_SIZE 3 // Número de quadros na memória

// Função para inicializar o buffer de páginas e bits de referência
void initialize(int frames[], int refBits[], int size) {
    for (int i = 0; i < size; i++) {
        frames[i] = -1; // -1 indica que o quadro está vazio
        refBits[i] = 0; // Bit de referência inicializado para 0
    }
}

// Função para imprimir o estado atual dos quadros
void printFrames(int frames[], int refBits[], int size, int pointer) {
    for (int i = 0; i < size; i++) {
        if (i == pointer) {
            printf("[*%d|%d] ", frames[i], refBits[i]);
        } else {
            printf(" %d|%d ", frames[i], refBits[i]);
        }
    }
    printf("\n");
}

// Função para executar o algoritmo do relógio
void clockPageReplacement(int pages[], int numPages, int frameSize) {
    int frames[frameSize];
    int refBits[frameSize];
    int pointer = 0; // Ponteiro para o "relógio"
    int pageFaults = 0;

    initialize(frames, refBits, frameSize);

    // Imprime o estado inicial dos quadros
    printf("Estado inicial dos quadros: ");
    printFrames(frames, refBits, frameSize, pointer);
    
    for (int i = 0; i < numPages; i++) {
        int page = pages[i];
        printf("Referenciando a página %d: ", page);

        // Verifica se a página está presente nos quadros
        int found = 0;
        for (int j = 0; j < frameSize; j++) {
            if (frames[j] == page) {
                refBits[j] = 1; // Atualiza o bit de referência
                found = 1;
                break;
            }
        }

        if (!found) {
            // Página não está na memória, ocorre uma falta de página
            while (refBits[pointer] == 1) {
                // Dá uma "segunda chance" e zera o bit de referência
                refBits[pointer] = 0;
                pointer = (pointer + 1) % frameSize;
            }

            // Substitui a página
            frames[pointer] = page;
            refBits[pointer] = 1; // Define o bit de referência
            pointer = (pointer + 1) % frameSize; // Avança o ponteiro
            pageFaults++;

            printf("Falta de página. Estado atual dos quadros: ");
            printFrames(frames, refBits, frameSize, (pointer - 1 + frameSize) % frameSize);
        } else {
            printf("Página já presente. Estado atual dos quadros: ");
            printFrames(frames, refBits, frameSize, (pointer - 1 + frameSize) % frameSize);
        }
    }

    printf("Número total de faltas de página: %d\n", pageFaults);
}

int main() {
    // Exemplo de referência de páginas
    int pages[] = {1, 2, 3, 5, 4, 3, 1, 4, 2, 5, 3, 1};
    int numPages = sizeof(pages) / sizeof(pages[0]);

    printf("Simulando o algoritmo do relógio com %d quadros de memória:\n", FRAME_SIZE);
    clockPageReplacement(pages, numPages, FRAME_SIZE);

    return 0;
}
