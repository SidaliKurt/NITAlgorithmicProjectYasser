#include <stdio.h>
#include <stdlib.h>
#include "sha256.h"


typedef struct{
    char prevHash[65];
    char hash[65];
    int nonce;
    char *transactions;
} block;

typedef struct blockchain {
    block *blk;
    struct blockchain *next;
} blockchain;

void newTransaction(char *sender, char *receiver, int amount, char *transaction){
    sprintf(transaction, "%s%s%d", sender, receiver, amount);
}

void mineBlock(int difficulty, block *blk){
    char *target = (char *)malloc(difficulty + 1);
    for (int i = 0; i < difficulty; i++){
        target[i] = '0';
    }
    target[difficulty] = '\0';

    char *hash = (char *)malloc(65);
    int nonce = 0;
    while (1){
        sprintf(blk->hash, "%s%d", blk->prevHash, nonce);
        sha256((const uint8_t *)blk->hash, strlen(blk->hash), (uint8_t *)hash);
        if (strncmp(hash, target, difficulty) == 0){
            break;
        }
        nonce++;
    }
    blk->nonce = nonce;
    free(target);
    free(hash);
}

blockchain *newBlockchain(block *blk){
    blockchain *chain = (blockchain *)malloc(sizeof(blockchain));
    chain->blk = blk;
    chain->next = NULL;
    return chain;
}

void addBlock(blockchain *chain, block *blk){
    blockchain *curr = chain;
    while (curr->next != NULL){
        curr = curr->next;
    }
    curr->next = newBlockchain(blk);
}

void printBlock(block *blk){
    printf("Block: \n");
    printf("Previous Hash: %s\n", blk->prevHash);
    printf("Hash: %s\n", blk->hash);
    printf("Nonce: %d\n", blk->nonce);
    printf("Transactions: %s\n", blk->transactions);
}

void printBlockchain(blockchain *chain){
    blockchain *curr = chain;
    while (curr != NULL){
        printBlock(curr->blk);
        curr = curr->next;
    }
}