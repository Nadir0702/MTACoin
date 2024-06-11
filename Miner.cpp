#include "Miner.hpp"

void* Miner::StartMinerFlow(void* i_Miner)
{
    Miner* miner = static_cast<Miner*>(i_Miner);

    if(miner->m_MinerID == DUMMY_MINER_ID)
    {
        miner->dummyMinerFlow();
    }
    else
    {
        miner->mine();
    }

    return i_Miner;
}

void Miner::dummyMinerFlow()
{
    this->m_Hash = this->m_DifficultyLimit;

    while (true)
    {
        this->m_BlockHeight++;
        applyForSuggestion();
        sleep(1);
    }
    
}

void Miner::mine()
{    
    while (true)
    {
        pthread_mutex_lock(&g_HeadLock);
        if(Server::s_ReadyToAppend == true)
        {
            pthread_cond_wait(&g_ServerWriting, &g_HeadLock);
        }

        readDataFromHeadBlock();
        pthread_mutex_unlock(&g_HeadLock);

        getHash();
        if(isValidHash())
        {
            applyForSuggestion();
        }  
    }
}

void Miner::applyForSuggestion()
{
    this->m_wasSuggestionSent = false;

    while(this->m_wasSuggestionSent == false)
    {
        pthread_mutex_lock(&g_SuggestedBlockLock);
        if(g_BlockAlreadySuggested == true)
        {
            pthread_cond_wait(&g_NewSuggestedBlock, &g_SuggestedBlockLock);
        }

        if(g_BlockAlreadySuggested == false)
        {
            suggestBlock(); 
        }

        pthread_mutex_unlock(&g_SuggestedBlockLock);
        pthread_cond_broadcast(&g_NewSuggestedBlock);
    }
}

void Miner::suggestBlock()
{
    g_SuggestedBlock.difficulty = this->m_Difficulty;
    g_SuggestedBlock.hash = this->m_Hash;
    g_SuggestedBlock.height = this->m_BlockHeight;
    g_SuggestedBlock.nonce = this->m_Nonce;
    g_SuggestedBlock.prev_hash = this->m_PrevHash;
    g_SuggestedBlock.relayed_by = this->m_MinerID;
    g_SuggestedBlock.timestamp = this->m_Timestamp;

    printSuggestion();

    this->m_wasSuggestionSent = true;
    g_BlockAlreadySuggested = true;
}

void Miner::printSuggestion() const
{
    std::cout << "Miner #" << this->m_MinerID;
    std::cout << ": mined a new block #" << this->m_BlockHeight;
    std::cout << " with the hash 0x" <<  std::hex << this->m_Hash << std::dec << "\n";
}

bool Miner::isValidHash() const
{
    bool verified = false;

    if(this->m_Hash < this->m_DifficultyLimit)
    {
        verified = true;
    }

    return verified;
}

void Miner::getHash()
{
    this->m_Nonce++;
    this->m_Timestamp = std::time(nullptr);
    unsigned int crcParams[] = {this->m_BlockHeight,
                                this->m_Timestamp,
                                this->m_PrevHash,
                                this->m_Nonce,
                                this->m_MinerID};

    this->m_Hash = crc32(this->m_PrevHash, (const Bytef*)crcParams, sizeof(crcParams));
}

void Miner::readDataFromHeadBlock()
{  
    if(this->m_PrevHash != g_BlockChainHead.hash)
    {
        this->m_Nonce = 0;
    }

    this->m_BlockHeight = g_BlockChainHead.height + 1;
    this->m_Difficulty = g_BlockChainHead.difficulty;
    this->m_PrevHash = g_BlockChainHead.hash;  

}
