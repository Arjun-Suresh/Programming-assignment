#include "Sequence.h"

void removeFromSet(std::vector<interval*>& intervelSet, int counter)
{
	//Cleanup while deleting an interval
	delete (intervelSet[counter]);
	intervelSet.erase(intervelSet.begin() + counter);
}

sequence::sequence()
{
	//Nothing to do here
}
void sequence::remove(long int min, long int max)
{
	//Removes [max,min] from the interval set
	if (!intervalsList.size())
		return; //Empty interval set
	long int curMin = 0, curMax = 0;
	while (curMin<intervalsList.size())
	{
		//Locate where the input min value corresponds to in the interval set
		if (min == intervalsList[curMin]->a || (min > intervalsList[curMin]->a && min < intervalsList[curMin]->b) || min == intervalsList[curMin]->b)
			break;
		if (min < intervalsList[curMin]->a)
			break;
		curMin++;
	}
	while (curMax<intervalsList.size())
	{
		//Locate where the input max value corresponds to in the interval set
		if (max == intervalsList[curMax]->a || (max > intervalsList[curMax]->a && max < intervalsList[curMax]->b) || max == intervalsList[curMax]->b)
			break;
		if (max < intervalsList[curMax]->a)
		{
			curMax--;
			break;
		}
		curMax++;
	}
	if (curMax < curMin || curMin == intervalsList.size())
		return; //Input interval is out of bounds or doesn't overlap with any interval in the set
        if (curMax == intervalsList.size())
		curMax--;
	long int counter = curMin + 1;
	while (counter < curMax)
	{
		//Delete all the intervals between the min and max intervals identified previously
		removeFromSet(intervalsList, counter);
		curMax--;
	}
	if (curMin == curMax)
	{
		//The min and max correspond to the same interval
		if (intervalsList[curMin]->a >= min && intervalsList[curMin]->b <= max)
			removeFromSet(intervalsList, curMin);
		else if (intervalsList[curMin]->a >= min)
			intervalsList[curMin]->a = max;
		else if (intervalsList[curMin]->b <= max)
			intervalsList[curMin]->b = min;
		else
		{
			//Splitting the interval [a,b] into [a,min] and [max,b] because a<min<max<b
			interval* node = new interval(max, intervalsList[curMin]->b);
			intervalsList[curMin]->b = min;
			intervalsList.insert(intervalsList.begin() + curMin + 1, node);
		}
	}
	else
	{
		if (intervalsList[curMin]->a >= min)
		{
			removeFromSet(intervalsList, curMin);
			curMax--;
		}
		else
			intervalsList[curMin]->b = min;
		if (intervalsList[curMax]->b <= max)
			removeFromSet(intervalsList, curMax);
		else
			intervalsList[curMax]->a = max;
	}
}

void sequence::add(long int min, long int max)
{
	//Adds the interval [min,max] into the set
	if (!intervalsList.size())
	{
		//Empty set. Directly create interval [a,b] and add it to the set
		interval* node = new interval(min, max);
		intervalsList.insert(intervalsList.begin(), node);
		return;
	}
	long int curMin = 0, curMax = 0;
	while (curMin<intervalsList.size())
	{
		//Locate where the input min value corresponds to in the interval set
		if (min == intervalsList[curMin]->a || (min > intervalsList[curMin]->a && min < intervalsList[curMin]->b) || min == intervalsList[curMin]->b)
			break;
		if (min < intervalsList[curMin]->a)
			break;
		curMin++;
	}
	while (curMax<intervalsList.size())
	{
		//Locate where the input max value corresponds to in the interval set
		if (max == intervalsList[curMax]->a || (max > intervalsList[curMax]->a && max < intervalsList[curMax]->b) || max == intervalsList[curMax]->b)
			break;
		if (max < intervalsList[curMax]->a)
		{
			curMax--;
			break;
		}
		curMax++;
	}
	if (curMax < curMin || curMin == intervalsList.size())
	{
		//Out of bounds input interval or non-overlapping input interval case
		interval* node = new interval(min, max);
		intervalsList.insert(intervalsList.begin() + curMin, node);
		return;
	}
	if (curMax == intervalsList.size())
		curMax--;
	if (min < intervalsList[curMin]->a)
		intervalsList[curMin]->a = min;
	if (max > intervalsList[curMax]->b)
		intervalsList[curMax]->b = max;
	long int counter = curMin + 1;
	while (counter < curMax)
	{
		//Merging intervals by deleting the intervals in between
		removeFromSet(intervalsList, counter);
		curMax--;
	}
	if (curMin != curMax)
	{
		//Combine 2 intervals containing the min and max
		intervalsList[curMin]->b = intervalsList[curMax]->b;
		removeFromSet(intervalsList, curMax);
	}
}

std::string sequence::outputFormat()
{
	//Create the string to be written into the output file
	std::string formattedString = "[";
	for (int counter = 0; counter < intervalsList.size(); counter++)
	{
		std::string temp = "[" + std::to_string(intervalsList[counter]->a) + "," + std::to_string(intervalsList[counter]->b) + "]";
		formattedString = formattedString + temp + ",";
	}
	if (formattedString[formattedString.size() - 1] == ',')
		formattedString[formattedString.size() - 1] = ']';
	else
		formattedString = formattedString + ']';
	char* result = new char[formattedString.size()+1];
	strcpy(result, formattedString.c_str());
	return std::string(result);
}
