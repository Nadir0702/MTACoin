#include "Server.hpp"
#include "Miner.hpp"
#include "Utils.hpp"

BLOCK_T g_SuggestedBlock;
BLOCK_T g_BlockChainHead;
pthread_mutex_t g_SuggestedBlockLock;
pthread_cond_t g_NewSuggestedBlock;

int main()
{
    Server server;
    Miner miners[NUM_OF_MINERS];
    pthread_t serverThreadID;
    pthread_t minerThreadsIDs[NUM_OF_MINERS];
    void* res;

    pthread_mutex_init(&g_SuggestedBlockLock, NULL);
	pthread_cond_init(&g_NewSuggestedBlock, NULL);

    server.CreateGenesisBlock();
    pthread_create(&serverThreadID, NULL, Server::StartServerFlow, &server);
    for (size_t i = 0; i < NUM_OF_MINERS; i++)
    {
        miners[i].SetMinerID(i + 1);
        pthread_create(&minerThreadsIDs[i], NULL, Miner::StartMinerFlow, &miners[i]);
    }
    
    for (size_t i = 0; i < NUM_OF_MINERS; i++)
    {
        pthread_join(minerThreadsIDs[i], &res);  
    }

    return 0;
}
