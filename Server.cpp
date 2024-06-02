#include "Server.hpp"



Server::Server()
{
    this->difficultyLimit = pow(float(2), (float)(32 - g_BlockChainHead.difficulty));
}

void Server::CreateGenesisBlock()
{
    this->lastCheckedBlock = g_BlockChainHead = g_SuggestedBlock = { 0, 0, 0, 0, 0, 0, 0 };
}

void* Server::StartServerFlow(void* i_Server)
{
    Server* miner = static_cast<Server*>(i_Server);

    miner->ManageBlockChain();

    return i_Server;
}

void Server::ManageBlockChain()
{
    std::cout << "Server Thread is up with ID : #" << pthread_self() << "\n";

    while (true)
    {
        pthread_mutex_lock(&g_SuggestedBlockLock);
        if(noNewBlockSuggested(g_SuggestedBlock))
        {
            pthread_cond_wait(&g_NewSuggestedBlock, &g_SuggestedBlockLock);
        }

        if(verifyProofOfWork(g_SuggestedBlock))
        {
            appendNewBlock(g_SuggestedBlock);
        }

        pthread_mutex_unlock(&g_SuggestedBlockLock);
    }

}

void Server::appendNewBlock(const BLOCK_T& i_SuggestedBlock) const
{
    g_BlockChainHead = i_SuggestedBlock;
    printHead();
}

void Server::printHead() const
{
    std::cout << "server: New block added by " << g_BlockChainHead.relayed_by << ", attributes: ";
    std::cout << "height(" << g_BlockChainHead.height << ") ";
    std::cout << "timestamp(" << g_BlockChainHead.timestamp << ") ";
    std::cout << "hash(" << g_BlockChainHead.hash << ") ";
    std::cout << "prev_hash(" << g_BlockChainHead.prev_hash << ") ";
    std::cout << "difficulty(" << g_BlockChainHead.difficulty << ") ";
    std::cout << "nonce(" << g_BlockChainHead.nonce << ") \n";
}

bool Server::verifyProofOfWork(const BLOCK_T& i_SuggestedBlock)
{
    bool verified = false;
    ulong checkSum;
    unsigned int crcParams[] = {i_SuggestedBlock.height,
                                i_SuggestedBlock.timestamp,
                                i_SuggestedBlock.prev_hash,
                                i_SuggestedBlock.nonce,
                                i_SuggestedBlock.relayed_by};

    checkSum = crc32(i_SuggestedBlock.prev_hash, (const Bytef*)crcParams, sizeof(crcParams));

    if(checkSum == i_SuggestedBlock.hash)
    {
        if(i_SuggestedBlock.hash < this->difficultyLimit)
        {
            verified = true;
        }
    }

    this->lastCheckedBlock = i_SuggestedBlock;

    return verified;
}

bool Server::noNewBlockSuggested(const BLOCK_T& i_SuggestedBlock) const
{
    return isIdenticalBlocks(g_BlockChainHead, g_SuggestedBlock) ||
           isIdenticalBlocks(g_SuggestedBlock, this->lastCheckedBlock);
}

bool Server::isIdenticalBlocks(const BLOCK_T& i_First, const BLOCK_T& i_Second) const
{
    bool result = true;

    result = result && (i_First.difficulty == i_Second.difficulty);
    result = result && (i_First.hash == i_Second.hash);
    result = result && (i_First.height == i_Second.height);
    result = result && (i_First.nonce == i_Second.nonce);
    result = result && (i_First.prev_hash == i_Second.prev_hash);
    result = result && (i_First.relayed_by == i_Second.relayed_by);
    result = result && (i_First.timestamp == i_Second.timestamp);

    return result;
}

