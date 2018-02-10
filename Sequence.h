#pragma once
#include<iostream>
#include<vector>
#include <string>
class interval
{
	//Holds each interval
public:
	long int a;
	long int b;
	interval(long int aVal, long int bVal):a(aVal),b(bVal)
	{
	}
};

class sequence
{
	//Holds the list of intervals and related operations
private:
	std::vector<interval*> intervalsList;
public:
	sequence();

	void remove(long int min, long int max);

	void add(long int min, long int max);

	std::string outputFormat();
};