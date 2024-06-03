#ifndef MINER_HPP
#define MINER_HPP

#include "Utils.hpp"
#include "Server.hpp"

class Miner
{
private:
    int m_BlockHeight = 0;
    int m_Timestamp = 0;  
    unsigned int m_Hash = 0;       
    unsigned int m_PrevHash = INITIAL_PREV_HASH;   
    int m_Difficulty = 0;    
    int m_Nonce = 0;          
    int m_MinerID = 0;
    bool m_wasSuggestionSent = false;
    uLong m_DifficultyLimit = 0;

    void mine();
    void getHash();
    void suggestBlock();
    void readDataFromHeadBlock();
    void printSuggestion() const;
    bool isValidHash() const;
    
public:
    void SetMinerDifficultyLimit(ulong i_difficultyLimit) {this->m_DifficultyLimit = i_difficultyLimit;}
    void SetMinerID(int i_Id) { this->m_MinerID = i_Id; }
    static void* StartMinerFlow(void* i_Miner);
};

#endif // MINER_HPP
