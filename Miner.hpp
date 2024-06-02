#ifndef MINER_HPP
#define MINER_HPP

#include "Utils.hpp"

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

    void Mine();
    
public:

    void SetMinerID(int i_Id) { this->m_MinerID = i_Id; }
    static void* StartMinerFlow(void* i_Miner);
};

#endif // MINER_HPP
