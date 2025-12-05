Huffman Coding
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char ch;
    int freq;
    struct Node *left, *right;
} Node;

typedef struct MinHeap {
    int size;
    Node* array[256];
} MinHeap;

Node* createNode(char ch, int freq) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->ch = ch;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

MinHeap* createMinHeap() {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->size = 0;
    return heap;
}

void insertHeap(MinHeap* heap, Node* node) {
    int i = heap->size++;
    while (i && node->freq < heap->array[(i - 1) / 2]->freq) {
        heap->array[i] = heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->array[i] = node;
}

Node* extractMin(MinHeap* heap) {
    Node *temp = heap->array[0];
    Node *lastNode = heap->array[--heap->size];

    int i = 0, smallest = 1;

    while (smallest < heap->size) {
        if (smallest + 1 < heap->size &&
            heap->array[smallest + 1]->freq < heap->array[smallest]->freq)
            smallest++;

        if (lastNode->freq <= heap->array[smallest]->freq)
            break;

        heap->array[i] = heap->array[smallest];
        i = smallest;
        smallest = 2 * i + 1;
    }

    heap->array[i] = lastNode;
    return temp;
}

Node* buildHuffmanTree(int freq[]) {
    MinHeap* heap = createMinHeap();

    for (int i = 0; i < 256; i++) {
        if (freq[i])
            insertHeap(heap, createNode(i, freq[i]));
    }

    while (heap->size > 1) {
        Node *left = extractMin(heap);
        Node *right = extractMin(heap);

        Node *merged = createNode('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;

        insertHeap(heap, merged);
    }

    return extractMin(heap);
}

void printCodes(Node* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    if (!root->left && !root->right) {
        printf("'%c' (%d) : ", root->ch, root->freq);
        for (int i = 0; i < top; i++)
            printf("%d", arr[i]);
        printf("\n");
    }
}

int main() {
    char text[1000];
    int freq[256] = {0};

    printf("Enter text to compress: ");
    fgets(text, sizeof(text), stdin);

    for (int i = 0; text[i] != '\0'; i++) {
        freq[(unsigned char)text[i]]++;
    }

    Node* root = buildHuffmanTree(freq);

    printf("\nHuffman Codes:\n");
    printf("--------------------------\n");

    int arr[256], top = 0;
    printCodes(root, arr, top);

    return 0;
}
