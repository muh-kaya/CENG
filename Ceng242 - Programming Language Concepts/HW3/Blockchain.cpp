#include "Blockchain.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Blockchain::Blockchain(int id) : id(id)
{
	head = NULL;
	flag = false;
}

Blockchain::Blockchain(int id, Koin *head) : id(id)
{
	this->head = head;
	this->flag = false;
}

Blockchain::Blockchain(const Blockchain& rhs) : id(rhs.id)
{	
	Koin* hd = new Koin(rhs.getHead()->getValue());
	hd->setNext(rhs.getHead()->getNext());
	this->setHead(hd);

	this->flag = false;
}

Blockchain& Blockchain::operator=(Blockchain&& rhs) noexcept
{
	Koin* p = rhs.getHead();
	clear();	

	if(p != NULL){
		Koin* newKoin = new Koin(p->getValue());
		newKoin->setNext(NULL);
		this->setHead(newKoin);

		Koin* q = this->getHead();
		p = p->getNext();

		while(p != NULL){
			Koin* newKoin2 = new Koin(p->getValue());
			newKoin2->setNext(NULL);
			q->setNext(newKoin2);

			p = p->getNext();
			q = q->getNext();
		}
	}
	rhs.clear();
	return *this;
}


Blockchain::~Blockchain()
{
	clear();
}

int Blockchain::getID() const
{
	return id;
}

Koin* Blockchain::getHead() const
{
	return head;
}

double Blockchain::getTotalValue() const
{
	double total = 0;

	Koin* p = getHead();

	while(p != NULL){
		total += p->getValue();
		p = p->getNext();
	}
	return total;
}

long Blockchain::getChainLength() const
{
	long len = 0;

	Koin* p = getHead();

	while(p != NULL){
		len += 1;
		p = p->getNext();
	}
	return len;
}

void Blockchain::operator++()
{
	Koin* newKoin = new Koin(Utilizer::fetchRandomValue());

	Koin* p = this->getHead();

	if(p == NULL){
		this->setHead(newKoin);
		newKoin->setNext(NULL);
	}
	else{
		while(p->getNext() != NULL)
			p = p->getNext();

		p->setNext(newKoin);
		newKoin->setNext(NULL);
	}
}

void Blockchain::operator--()
{
	if(this->flag == false){
		Koin* p = this->getHead();

		if(p == NULL){
			return;
		}
		else if(p->getNext() == NULL){
			if(p->count == 1){
				delete p;
				this->setHead(NULL);
			}
			return;
		}
		else{
			while(p->getNext()->getNext() != NULL)
				p = p->getNext();
			
			if(p->getNext()->count == 1){
				Koin* q = p->getNext();
				p->setNext(NULL);
				q->setNext(NULL);
				delete q;
			}
			return;
		}
	}
}

Blockchain& Blockchain::operator*=(int multiplier)
{
	Koin* p = getHead();

	while(p != NULL){
		p->setValue(multiplier*p->getValue());
		p = p->getNext();
	}
	return *this;
}

Blockchain& Blockchain::operator/=(int divisor)
{
	Koin* p = getHead();

	while(p != NULL){
		p->setValue(p->getValue()/divisor);
		p = p->getNext();
	}
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Blockchain& blockchain)
{
	const Koin* p = blockchain.getHead();

	if(p == NULL){
		os << "Block " << blockchain.getID() << ": Empty.";
	}
	else{
		os << "Block " << blockchain.getID() << ": ";
	
		while(p != NULL){
			os << std::fixed << std::setprecision(Utilizer::koinPrintPrecision()) << p->getValue() << "--";
			p = p->getNext();
		}
		os << "|(" << std::fixed << std::setprecision(Utilizer::koinPrintPrecision()) << blockchain.getTotalValue() << ")";
	}
	return os;

}

void Blockchain::setHead(Koin* head)
{
	this->head = head;
}

void Blockchain::clear()
{
	Koin* p = getHead();

	if(p == NULL)
		return;
	else{
		while(p->getNext() != NULL){
			Koin* q = p->getNext();
			p->setNext(q->getNext());
			q->setNext(NULL);

			delete q;
		}
	}
	delete p;

	head = NULL;
}