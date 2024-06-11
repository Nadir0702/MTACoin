#include "Server.hpp"

bool Server::s_ReadyToAppend = false;

void Server::CreateGenesisBlock(int i_Difficulty)
{
    int         	height = 0;
    int         	timestamp = 0;
    unsigned int    hash = 0;
    unsigned int    prev_hash = 0;
    int        	    difficulty = i_Difficulty;
    int         	nonce = 0;
    int         	relayed_by = 0;

    this->m_LastCheckedBlock = g_BlockChainHead = g_SuggestedBlock = { height, timestamp, hash, prev_hash,
                                                                        difficulty, nonce, relayed_by };
}

void* Server::StartServerFlow(void* i_Server)
{
    Server* miner = static_cast<Server*>(i_Server);

    miner->ManageBlockChain();

    return i_Server;
}

void Server::ManageBlockChain()
{
    while (true)
    {
        pthread_mutex_lock(&g_SuggestedBlockLock);
        if(noNewBlockSuggested(g_SuggestedBlock))
        {
            pthread_cond_wait(&g_NewSuggestedBlock, &g_SuggestedBlockLock);
        }

        try
        {
            verifyProofOfWork(g_SuggestedBlock);
            appendNewBlock(g_SuggestedBlock);
        }
        catch(const Exception& error)
        {
            error.Print();
        }
   
        g_BlockAlreadySuggested = false;
        pthread_mutex_unlock(&g_SuggestedBlockLock);
        pthread_cond_broadcast(&g_NewSuggestedBlock);
    }

}

void Server::appendNewBlock(const BLOCK_T& i_SuggestedBlock) const
{
    pthread_mutex_lock(&g_HeadLock);
    s_ReadyToAppend = true;
    g_BlockChainHead = i_SuggestedBlock;
    s_ReadyToAppend = false;
    pthread_mutex_unlock(&g_HeadLock);
    pthread_cond_broadcast(&g_ServerWriting);

    printHead();
}

void Server::printHead() const
{
    std::cout << "server: New block added by " << g_BlockChainHead.relayed_by << ", attributes: ";
    std::cout << "height(" << g_BlockChainHead.height << ") ";
    std::cout << "timestamp(" << g_BlockChainHead.timestamp << ") ";
    std::cout << "hash(0x" << std::hex << g_BlockChainHead.hash << ") ";
    std::cout << "prev_hash(0x" << g_BlockChainHead.prev_hash << std::dec << ") ";
    std::cout << "difficulty(" << g_BlockChainHead.difficulty << ") ";
    std::cout << "nonce(" << g_BlockChainHead.nonce << ") \n";
}

void Server::verifyProofOfWork(const BLOCK_T& i_SuggestedBlock)
{
    bool verified;
    ulong checkSum;
    unsigned int crcParams[] = {i_SuggestedBlock.height,
                                i_SuggestedBlock.timestamp,
                                i_SuggestedBlock.prev_hash,
                                i_SuggestedBlock.nonce,
                                i_SuggestedBlock.relayed_by};

    this->m_LastCheckedBlock = i_SuggestedBlock;
    checkSum = crc32(i_SuggestedBlock.prev_hash, (const Bytef*)crcParams, sizeof(crcParams));
    verified = checkCRC(i_SuggestedBlock, checkSum);
}

bool Server::checkCRC(const BLOCK_T& i_SuggestedBlock, ulong checkSum) const
{
    bool verified = false;

    if(i_SuggestedBlock.prev_hash == g_BlockChainHead.hash)
    {
        if(i_SuggestedBlock.height - 1 == g_BlockChainHead.height)
        {
            if(checkSum == i_SuggestedBlock.hash)
            {
                if(i_SuggestedBlock.hash < this->m_DifficultyLimit)
                {
                    verified = true;
                }
                else
                {
                    std::string meassage = ", not eneogh leading 0's";
                    throw Exception("difficulty", meassage, i_SuggestedBlock);      
                }
            }
            else
            {
                std::string meassage = ", received 0x" + Exception::toHexString(i_SuggestedBlock.hash) + 
                                       " but calculated 0x" + Exception::toHexString(checkSum);
                throw Exception("hash", meassage, i_SuggestedBlock);
            }
        }
        else
        {
            std::string meassage = ", received " + std::to_string(i_SuggestedBlock.height) + 
                                       " but expected " + std::to_string(g_BlockChainHead.height);
                throw Exception("height", meassage, i_SuggestedBlock);
        }     
    }
    else
    {
        std::string meassage = ", received 0x" + Exception::toHexString(i_SuggestedBlock.prev_hash) + 
                               " but expected 0x" + Exception::toHexString(g_BlockChainHead.hash) ;
        throw Exception("prev_hash", meassage, i_SuggestedBlock);
    }    

    return verified;
}

bool Server::noNewBlockSuggested(const BLOCK_T& i_SuggestedBlock) const
{
    return isIdenticalBlocks(g_BlockChainHead, g_SuggestedBlock) ||
           isIdenticalBlocks(g_SuggestedBlock, this->m_LastCheckedBlock);
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
