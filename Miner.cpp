#include "Miner.hpp"

void* Miner::StartMinerFlow(void* i_Miner)
{
    Miner* miner = static_cast<Miner*>(i_Miner);

    miner->Mine();

    return i_Miner;
}

void Miner::Mine()
{    
    std::cout << "Thread #" << pthread_self() << " is miner #" << this->m_MinerID << "\n";
}