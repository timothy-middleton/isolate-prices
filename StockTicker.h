#pragma once

#include <vector>

class StockTicker
{
public:
	StockTicker();
	~StockTicker();
	void updateDateCount();	// increment the count of dates the stock is present
	void addPricePair(std::string openP, std::string closeP);	// add a particular date prices
	size_t GetDateCount();	// return the count of dates
	std::string GetOpenPrice(int index);	// return a particular date open price
	std::string GetClosePrice(int index);	// return a particular date close price

private:
	size_t m_dateCount;
	std::vector<std::string> m_openPrices;	// vector to hold all the dates open prices
	std::vector<std::string> m_closePrices;	// vector to hold all the dates close prices
};

