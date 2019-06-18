#include "Miner.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

int Miner::getNextAvailableBlockchainID() const
{
	return miner.size();
}


Miner::Miner(std::string name)
{
	this->name = name;
}

Miner::~Miner()
{
	for (int i = 0; i < miner.size(); ++i)
	{
		if(miner[i]->flag == true)
			miner[i]->setHead(NULL);
	}
	for (int i = 0; i < miner.size(); ++i)
	{
		delete miner[i];
	}
}

void Miner::createNewBlockchain()
{
	Blockchain* newBlockchain = new Blockchain(getNextAvailableBlockchainID());
	miner.push_back(newBlockchain);
}


void Miner::mineUntil(int cycleCount)
{
	for(int i=0; i < cycleCount; i++){
		for(int j=0; j < miner.size(); j++){
			++(*miner[j]);
		}
	}
}


void Miner::demineUntil(int cycleCount)
{
	for(int i=0; i < cycleCount; i++){
		for(int j=0; j < miner.size(); j++){
			--(*miner[j]);
		}
	}
}


double Miner::getTotalValue() const
{
	double total = 0;

	for(int i=0; i < miner.size(); i++){
		if((*miner[i]).flag == false)
			total += (*miner[i]).getTotalValue();
	}
	return total;
}


long Miner::getBlockchainCount() const
{
	return miner.size();
}


Blockchain* Miner::operator[](int id) const
{
	if(id >= miner.size())
		return nullptr;
	return miner[id];
}


bool Miner::softFork(int blockchainID)
{
	if(blockchainID >= miner.size())
		return false;
	else{
		Blockchain* newBlockchain = new Blockchain(getNextAvailableBlockchainID());

		Koin *p = (*miner[blockchainID]).getHead();
		while(p->getNext() != NULL){
			p = p->getNext();
		}
		p->count++;

		newBlockchain->setHead(p);
		newBlockchain->flag = true;

		miner.push_back(newBlockchain);
		return true;
	}

}


bool Miner::hardFork(int blockchainID)
{
	if(blockchainID >= miner.size())
		return false;
	else{
		Blockchain* newBlockchain = new Blockchain(getNextAvailableBlockchainID());

		Koin *p = (*miner[blockchainID]).getHead();

		while(p->getNext() != NULL){
			p = p->getNext();
		}
		Koin* newKoin = new Koin(p->getValue());
		newKoin->setNext(NULL);
		newBlockchain->setHead(newKoin);

		miner.push_back(newBlockchain);

		return true;
	}

}

std::ostream& operator<<(std::ostream& os, const Miner& miner)
{
	os << "-- BEGIN MINER --" << "\n" << "Miner name: " << miner.name << "\n";
	os << "Blockchain count: " << miner.getBlockchainCount() << "\n" << "Total value: " <<std::fixed << std::setprecision(Utilizer::koinPrintPrecision()) << miner.getTotalValue() << "\n" << "\n";

	for (int i = 0; i < miner.miner.size(); i++)
	{
		os << (*miner[i]) << "\n";
	}

	os << "\n" << "-- END MINER --" << "\n";

}
