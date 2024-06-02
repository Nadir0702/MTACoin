#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"

class Server
{
private:
    uLong difficultyLimit;
    BLOCK_T lastCheckedBlock;
    
    void printHead() const;
    void appendNewBlock(const BLOCK_T& i_SuggestedBlock) const;
    bool noNewBlockSuggested(const BLOCK_T& i_SuggestedBlock) const;
    bool verifyProofOfWork(const BLOCK_T& i_SuggestedBlock);
    bool isIdenticalBlocks(const BLOCK_T& i_First, const BLOCK_T& i_Second) const;
   
public:
    Server();
    static void* StartServerFlow(void* i_Server);
    void CreateGenesisBlock();
    void ManageBlockChain();

};

#endif // SERVER_HPP
