#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"
#include "Exception.hpp"

class Server
{
private:
    uLong m_DifficultyLimit = 0;
    BLOCK_T m_LastCheckedBlock;
    
    void printHead() const;
    void appendNewBlock(const BLOCK_T& i_SuggestedBlock) const;
    void verifyProofOfWork(const BLOCK_T& i_SuggestedBlock);
    bool noNewBlockSuggested(const BLOCK_T& i_SuggestedBlock) const;
    bool isIdenticalBlocks(const BLOCK_T& i_First, const BLOCK_T& i_Second) const;
    bool checkCRC(const BLOCK_T& i_SuggestedBlock, ulong checkSum) const;
   
public:
    static bool s_ReadyToAppend;

    void SetServerDifficultyLimit(ulong i_DifficultyLimit) {this->m_DifficultyLimit = i_DifficultyLimit;}
    void CreateGenesisBlock(int i_Difficulty);
    void ManageBlockChain();
    static void* StartServerFlow(void* i_Server);

};

#endif // SERVER_HPP
