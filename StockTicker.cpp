#include "StockTicker.h"



StockTicker::StockTicker()
{
	m_dateCount = 0;
}


StockTicker::~StockTicker()
{
}

void StockTicker::updateDateCount()
{
	m_dateCount++;
}

void StockTicker::addPricePair(std::string openP, std::string closeP)
{
	m_openPrices.push_back(openP);
	m_closePrices.push_back(closeP);
}

size_t StockTicker::GetDateCount()
{
	return m_dateCount;
}

std::string StockTicker::GetOpenPrice(int index)
{
	return m_openPrices[index];
}

std::string StockTicker::GetClosePrice(int index)
{
	return m_closePrices[index];
}
