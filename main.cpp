#include "Server.hpp"
#include "Miner.hpp"
#include "Utils.hpp"

BLOCK_T g_SuggestedBlock;
BLOCK_T g_BlockChainHead;

int main()
{
    Server server;
    Miner miners[4];
    pthread_t minerThreadsIDs[4];
    void* res;

    server.CreateGenesisBlock();
    server.ManageBlockChain();
    for (size_t i = 0; i < 4; i++)
    {
        miners[i].SetMinerID(i + 1);
        pthread_create(&minerThreadsIDs[i], NULL, Miner::MinerFlow, &miners[i]);
    }
    

    
    for (size_t i = 0; i < 4; i++)
    {
        pthread_join(minerThreadsIDs[i], &res);  
    }

    return 0;
}
