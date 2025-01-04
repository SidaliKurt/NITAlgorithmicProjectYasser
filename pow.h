#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sha256.h"

int difficulty = 5;
typedef struct block{
    struct block *prev;
    struct block *next;
    char *prevHash;
    char *transactions;
    time_t timestamp;
    int nonce;
    char *hash;
    int id;
} block;
block blockChain[100];
block *firstBlock;

int verifyLeadingZeros(uint8_t *digest, int leadingZeros) { 
    int bytesToCheck = leadingZeros / 2;
    int bitsToCheck = (leadingZeros % 2) * 4;
    for (int i = 0; i < bytesToCheck; i++) {
        if (digest[i] != 0x00) {
            return 0;
        }
    }
    if (bitsToCheck != 0) {
        uint8_t mask = 0xF0;
        if ((digest[bytesToCheck] & mask) != 0x00) {
            return 0;
        }
    }
    return 1;
}

void newTransaction(char *sender, char *receiver, int amount, char *transaction){
    sprintf(transaction, "%s:%s:%d,", sender, receiver, amount);
}

void setHash(block *blk, uint8_t *digest){
    blk->hash = (char *)malloc(65);
    for (int i = 0; i < 32; i++) {
        sprintf(blk->hash+i*2, "%02x", digest[i]);
    }
}

void mineBlock(block *blk){
    char *data = (char *)malloc(501);
    uint8_t digest[32];
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d::%H:%M:%S", (struct tm *)localtime(&(blk->timestamp)));
    int nonce = 0;
    while (1){
        sprintf(data, "%s%s%s%d", blk->prevHash, blk->transactions, timestamp, nonce);
        sha256((const uint8_t *)data, strlen(data), digest);
        nonce++;
        if (verifyLeadingZeros(digest, difficulty)){
            break;
        }
    }
    blk->nonce = nonce;
    setHash(blk, digest);
    free(data);
}

block *newBlock(block *chain){
    block *blk = (block *)malloc(sizeof(block));
    blk->hash = NULL;
    blk->next = NULL;
    blk->timestamp = time(NULL);
    if(chain == NULL){
        blk->prevHash = "0000000000000000000000000000000000000000000000000000000000000000";
        blk->prev = NULL;
        blk->id = 0;
        return blk;
    }
    block *curr = chain;
    while (curr->next != NULL){
        curr = curr->next;
    }
    curr->next = blk;
    blk->prev = curr;
    blk->prevHash = curr->hash;
    blk->id = curr->id + 1;
    return blk;
}



void printBlock(block *blk){
    printf("Block: id:%d\n",blk->id);
    printf("Previous Hash: %s\n", blk->prevHash);
    printf("Hash: %s\n", blk->hash);
    printf("Nonce: %d\n", blk->nonce);
    printf("Transactions: %s\n", blk->transactions);
    printf("Timestamp: %s\n", ctime(&(blk->timestamp)));

}