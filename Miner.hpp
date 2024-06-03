#ifndef MINER_HPP
#define MINER_HPP

#include "Utils.hpp"
#include "Server.hpp"

class Miner
{
private:
    int m_BlockHeight;
    int m_Timestamp;  
    unsigned int m_Hash;       
    unsigned int m_PrevHash;   
    int m_Difficulty;    
    int m_Nonce;          
    int m_MinerID;
    bool m_wasSuggestionSent;
    uLong m_DifficultyLimit;

    void mine();
    void getHash();
    void suggestBlock();
    void readDataFromHeadBlock();
    bool isValidHash() const;
    
public:
    void SetMinerDifficultyLimit(ulong i_difficultyLimit) {this->m_DifficultyLimit = i_difficultyLimit;}
    void SetMinerID(int i_Id) { this->m_MinerID = i_Id; }
    static void* StartMinerFlow(void* i_Miner);
};

#endif // MINER_HPP
