#include "ParseCommand.h"

bool matchString(std::string line, int& index, char* str)
{
	//Match if the given line contains string 'str' starting with location 'index'
	int i = index;
	char val[100];
	std::locale loc;
	while (i < index + strlen(str) && i < line.size())
	{
		val[i - index] = std::tolower(line[i], loc);
		i++;
	}
	val[i - index] = '\0';
	if (!strcmp(val, str))
	{
		if ((index + strlen(str)) != line.size() && isalpha(line[index + strlen(str)]))
			return false; //The string is 'str' followed by another alphabet character. Shouldn't proceed
		index += strlen(str); //Update index to the location following the matched string if successful
		return true;
	}
	return false;
}

bool getDigit(std::string line, int& index, long int& intVal)
{
	//Scan if the location starting from index in string 'line' contains digits. If so, return the value as long int.
	bool negativeInteger = false, found = false;
	char val[10];
	int count = 0, counter = index;
	if (line[counter] == '-')
		negativeInteger = true; //Negative number is also handled
	while (counter != line.size() && !isspace(line[counter]) && isdigit(line[counter]))
	{
		val[count] = line[counter];
		count++;
		counter++;
		if (!found)
			found = true;
	}
	if (found)
	{
		val[count] = '\0';
		intVal = atol(val);
		if (negativeInteger)
			intVal = intVal*(-1);
		index = counter; //Update the index to location following the digit
		return true;
	}
	return false;
}

bool getNumbers(std::string line, int& index, long int& min, long int& max)
{
	//Get the max and min values starting at location index in input line
	int counter = index;
	while (counter != line.size() && isspace(line[counter]))
		counter++; //Discard spaces
	
	bool foundInteger1 = getDigit(line, counter, min); //Get the first integer

	while (counter != line.size() && isspace(line[counter]))
		counter++; //Discard spaces
	
	if (line[counter++] != ',')
		return false; //Need to have ',' between the numbers
	
	while (counter != line.size() && isspace(line[counter]))
		counter++;
	
	bool foundInteger2 = getDigit(line, counter, max);

	bool finalSizeCheck = false;
	while (counter != line.size() && line[counter] != '\n' && isspace(line[counter]))
		counter++;
	if (counter == line.size() || line[counter] == '\n')
		finalSizeCheck = true; //No character apart from newline following the parsed items

	if (foundInteger1 && foundInteger2 && finalSizeCheck)
		return true;
	return false;
}