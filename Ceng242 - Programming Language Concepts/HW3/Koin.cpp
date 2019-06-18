#include "Koin.h"
#include <iostream>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Koin::Koin(double value)
{
	this->value = value;
	this->next = NULL;
	this->count = 1;
}

Koin::Koin(const Koin& rhs)
{
	*this = rhs;
}

Koin& Koin::operator=(const Koin& rhs)
{
	this->next = rhs.getNext();
	this->value = rhs.getValue();
	this->count = 1;

	return *this;
}

Koin::~Koin()
{

}

double Koin::getValue() const
{
	return value;
}

Koin* Koin::getNext() const
{
	return next;
}

void Koin::setNext(Koin *next)
{
	this->next = next;
}

bool Koin::operator==(const Koin& rhs) const
{
	if((this->getValue()-rhs.getValue()) < Utilizer::doubleSensitivity() && (rhs.getValue()-this->getValue()) < Utilizer::doubleSensitivity()){
		if(this->getNext()==rhs.getNext())
        	return true;
        else
        	return false;
    }
    else
        return false;
}

bool Koin::operator!=(const Koin& rhs) const
{
	if((this->getValue()-rhs.getValue()) < Utilizer::doubleSensitivity() && (rhs.getValue()-this->getValue()) < Utilizer::doubleSensitivity()){
		if(this->getNext()==rhs.getNext())
        	return false;
        else
        	return true;
    }
    else
        return true;
}

Koin& Koin::operator*=(int multiplier)
{
	this->value = multiplier*this->value;

	return *this;
}

Koin& Koin::operator/=(int divisor)
{
	this->value = (this->value/divisor);
	
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Koin& koin)
{
	const Koin* p = &koin;
	while(p != NULL){
		os << std::fixed << std::setprecision(Utilizer::koinPrintPrecision()) << p->value << "--";
		p = p->getNext();
	}
	os << "|";
	return os;
}

void Koin::setValue(double value)
{
	this->value = value;
}