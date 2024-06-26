#ifndef UTILS_HPP
#define UTILS_HPP

#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <zlib.h>
#include <math.h>
#include <ctime>
#include <string>
#include <sstream>

#define NUM_OF_MINERS 4
#define INITIAL_PREV_HASH 17
#define SIZE_OF_CRC_RESULT 32
#define DUMMY_MINER_ID 0

typedef struct {
    int         	height;        // Incrementeal ID of the block in the chain
    int         	timestamp;     // Time of the mine in seconds since epoch
    unsigned int    hash;          // Current block hash value
    unsigned int    prev_hash;     // Hash value of the previous block
    int        	    difficulty;    // Amount of preceding zeros in the hash
    int         	nonce;         // Incremental integer to change the hash value
    int         	relayed_by;    // Miner ID
} BLOCK_T;

extern BLOCK_T g_SuggestedBlock;
extern BLOCK_T g_BlockChainHead; 
extern pthread_mutex_t g_SuggestedBlockLock;
extern pthread_mutex_t g_HeadLock;
extern pthread_cond_t g_ServerWriting;
extern pthread_cond_t g_NewSuggestedBlock;
extern bool g_BlockAlreadySuggested;

#endif // UTILS_HPP
