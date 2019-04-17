

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <ctime>
#include <filesystem>
#include "StockTicker.h"

namespace fs = std::filesystem;
using namespace std;

int main()
{
	cout << endl;
	// check if the input directory exists
	string path = "C:\\3304 Input Files";
	if (!fs::exists(path))
	{
		cout << "Could not open directory C:\\3304 Input Files\\" << endl;
		return -1;
	}
	
	vector<string> dates;	// vector to store all the dates
	map<string, StockTicker*> stocks;	// map to store all the names with the stock details
	bool listDone = false;	// first time, it is false, the map is built,

	time_t ctime = time(0);
	clock_t sTime = clock();
	char cTimeC[100];
	ctime_s(cTimeC, 100, &ctime);
	cout << cTimeC << endl;

	// for each file in the input directory
	for (auto & p : fs::directory_iterator(path))
	{
		string filePath = p.path().string();
		string date;
		ifstream csvFile(filePath);	// open the file to read
		if (!csvFile.is_open()) continue;	// if the file could not be opened, continue to next file

		string csvline;
		// read the file line by line
		while (std::getline(csvFile, csvline))
		{
			// read the comma separated values through a string stream
			stringstream ss(csvline);
			string tickerName;
			std::getline(ss, tickerName, ',');
			StockTicker *stock = NULL;
			// if listDone is true, the first file stocks have been created,
			// now if the stock is not there in the first file, ignore it,
			// else get the pointer to the stock for updating.
			if (listDone)
			{
				// find the stock in the map
				map<string, StockTicker*>::iterator it = stocks.find(tickerName);
				// if not found, continue to next
				if (it == stocks.end()) continue;
				else
				{
					// stock is found, get the pointer
					stock = it->second;
				}
			}
			else
			{
				// this if first file being read, insert all the stocks which are there.
				stock = new StockTicker();
				stocks[tickerName] = stock;
			}
			// read all the values.
			std::getline(ss, date, ',');
			string openP, val2, val3, closeP, val5;
			std::getline(ss, openP, ',');
			std::getline(ss, val2, ',');
			std::getline(ss, val3, ',');
			std::getline(ss, closeP, ',');
			std::getline(ss, val5, ',');

			// update the stock's date count to reflect that this stock was found in this csv file.
			// also it will help after ward to determine if this stock is to be reported in output files or not.
			stock->updateDateCount();
			// update the vectors of the prices in the stock for this file.
			stock->addPricePair(openP, closeP);
		}

		csvFile.close();

		dates.push_back(date);

		listDone = true;
	}

	size_t datesCount = dates.size();
	if (datesCount == 0)
	{
		cout << "No Input Files Exist." << endl;
		return 0;
	}
	cout << "Number of files = " << datesCount << endl << endl;
	ctime = time(0);
	ctime_s(cTimeC, 100, &ctime);
	cout << cTimeC << endl;
	cout << "Elapsed Time = " << ((float)(clock() - sTime) / 1000) << endl;


#if 0
	int nC = 0;
	int pC = 0;
	for (auto & x : stocks)
	{
		if (x.second->GetDateCount() != datesCount)
		{
			cout << x.first << " not in all..." << endl;
			nC++;
		}
		else pC++;
	}

	cout << "Not all Count - " << nC << ", all count - " << pC;
#endif

	// create output directory if does not exist.
	string opath = "C:\\3304 Output Files";
	bool dirOut = fs::exists(opath);
	if (!dirOut)
	{
		dirOut = fs::create_directory(opath);
	}
	if (!dirOut)
	{
		cout << "The output directory could not be created." << endl;
		return -1;
	}

	ofstream openPricesFile("C:\\3304 Output Files\\Open Prices.csv");
	ofstream closePricesFile("C:\\3304 Output Files\\Close Prices.csv");

	if (!openPricesFile.is_open() || !closePricesFile.is_open())
	{
		cout << "The output files could not be created." << endl;
		openPricesFile.close();
		closePricesFile.close();
		return -1;
	}

	// first write the header into the files.
	openPricesFile << "Date";
	closePricesFile << "Date";
	for (auto & x : stocks)
	{
		// each stock name
		if (x.second->GetDateCount() == datesCount)
		{
			openPricesFile << "," << x.first;
			closePricesFile << "," << x.first;
		}
	}
	openPricesFile << endl;
	closePricesFile << endl;

	// now write the date and the stock's prices on that date for each date.
	for (int i = 0; i < datesCount; i++)
	{
		openPricesFile << dates[i];
		closePricesFile << dates[i];
		for (auto & x : stocks)
		{
			if (x.second->GetDateCount() == datesCount)
			{
				openPricesFile << "," << x.second->GetOpenPrice(i);
				closePricesFile << "," << x.second->GetClosePrice(i);
			}
		}
		openPricesFile << endl;
		closePricesFile << endl;
	}

	openPricesFile.close();
	closePricesFile.close();

	// free all the stocks before closing.
	for (auto &x : stocks)
	{
		delete(x.second);
		x.second = NULL;
	}
	system("pause");
	return 0;
}
