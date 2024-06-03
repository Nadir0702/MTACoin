#include "Server.hpp"
#include "Miner.hpp"
#include "Utils.hpp"

BLOCK_T g_SuggestedBlock;
BLOCK_T g_BlockChainHead;
pthread_mutex_t g_SuggestedBlockLock;
pthread_mutex_t g_HeadLock;
pthread_cond_t g_ServerWriting;
pthread_cond_t g_NewSuggestedBlock;
bool g_BlockAlreadySuggested = false;

int main(int argc, char* argv[])
{
    Server server;
    Miner dummyMiner;
    Miner miners[NUM_OF_MINERS];
    pthread_t serverThreadID;
    pthread_t dummyMinerID;
    pthread_t minerThreadsIDs[NUM_OF_MINERS];
    int difficulty = 31; // atoi(argv[1]);
    ulong difficultyLimit = pow(float(2), (float)(SIZE_OF_CRC_RESULT - difficulty));

    pthread_mutex_init(&g_HeadLock, NULL);
    pthread_mutex_init(&g_SuggestedBlockLock, NULL);
	pthread_cond_init(&g_NewSuggestedBlock, NULL);
    pthread_cond_init(&g_ServerWriting, NULL);

    server.CreateGenesisBlock(difficulty);
    server.SetServerDifficultyLimit(difficultyLimit);
    pthread_create(&serverThreadID, NULL, Server::StartServerFlow, &server);
    for (size_t i = 1; i <= NUM_OF_MINERS; i++)
    {
        miners[i].SetMinerID(i);
        miners[i].SetMinerDifficultyLimit(difficultyLimit);
        pthread_create(&minerThreadsIDs[i], NULL, Miner::StartMinerFlow, &miners[i]);
    }

    dummyMiner.SetMinerID(DUMMY_MINER_ID);
    dummyMiner.SetMinerDifficultyLimit(difficultyLimit);
    pthread_create(&dummyMinerID, NULL, Miner::StartMinerFlow, &dummyMiner);
    
    for (size_t i = 0; i < NUM_OF_MINERS; i++)
    {
        pthread_join(minerThreadsIDs[i], NULL);  
    }
    
    pthread_join(serverThreadID, NULL);

    return 0;
}
