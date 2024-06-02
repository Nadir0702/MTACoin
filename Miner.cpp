#include "Miner.hpp"

void* Miner::MinerFlow(void* arg)
{
    Miner* miner = static_cast<Miner*>(arg);

    miner->Mine();

    return arg;
}

void Miner::Mine()
{
    std::cout << this->m_MinerID << "\n";
}