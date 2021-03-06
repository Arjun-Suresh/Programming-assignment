#include <iostream>
#include <fstream>
#include "Sequence.h"
#include "ParseCommand.h"


char* getNewBlock(int maxCount, int count, char* line)
{
	//In case while reading the line from file, if the buffer is exhausted, this functions reallocates a larger memory
	char* newBlock = new char[maxCount * 2];
	for (int i = 0; i < count; i++)
		newBlock[i] = line[i];
	delete[] line;
	return newBlock;
}

std::string readLineFromFile(std::ifstream* input)
{
	//Read single line from input file
	char* line = new char[100];
	int maxCount = 100;
	int count = 0;
	char c;
	input->get(c);
	while (c != '\n' && !(input->eof()))
	{
		line[count++] = c;
		input->get(c);
		if (count == maxCount - 5)
		{
			//Buffer is nearly exhausted (Very rare scenario)
			line = getNewBlock(maxCount, count, line);
			maxCount = maxCount * 2;
		}
	}
	line[count++] = '\n';
	line[count] = '\0';
	return std::string(line);
}

bool checkIfCommandToBeRead(std::ifstream* input)
{
	//Check if the file has reached eof or more lines are to be read
	if (input->eof())
		return false;
	return true;
}

bool parseAndUpdateInterval(std::string line, sequence* intervals)
{
	//Parse the command to get the category - add or remove, and obtain the max and min values
	int index = 0;
	long int min, max;
	if (matchString(line, index, "add,"))
	{
		//It is an 'add' command
		if (!getNumbers(line, index, min, max))
			return false; //Probably because the command wasn't in the right format or 2 numbers weren't given
		if (max <= min)
			return false;
		intervals->add(min, max);
	}
	else if (matchString(line, index, "remove,"))
	{
		//It is a 'remove' command
		if (!getNumbers(line, index, min, max))
			return false; //Probably because the command wasn't in the right format or 2 numbers weren't given
		if (max <= min)
			return false;
		intervals->remove(min, max);
	}
	else
		return false;
	return true;
}

int main(int argc, char* argv[])
{
	int option;
	bool readFlag = false;
	std::ifstream fPtr, *input;
	std::string filePath;
	if (argc < 3)
	{
		std::cout << "Insufficient arguments passed to the program\n";
		return 0;
	}
	filePath = argv[1]; //Input file name
	fPtr.open(filePath.c_str(), std::ifstream::in);
	if (!(fPtr.good()))
	{
		std::cout << "Input File doesn't exist\n";
		return 0;
	}
	input = &fPtr;

	std::string outputFilepath = argv[2]; //Output file name
	//Need to delete an existing output file if it exists and create a new one in its place
	std::remove(outputFilepath.c_str());
	std::ifstream testExecute(outputFilepath.c_str());
	if (testExecute.good())
	{
		//Error if the output file is open in any application which is not allowing this application to delete the file
		std::cout << "Please try closing the output file and running the application\n";
		return 0;
	}
	std::ofstream fWriteExecute(outputFilepath.c_str());
	sequence* intervals = new sequence();
	while (checkIfCommandToBeRead(input))
	{
		//There is a command to read
		std::string line = readLineFromFile(input);
		if (line[0] == '\n')
			continue; //A new line character was provided accidently
		bool result = parseAndUpdateInterval(line, intervals);
		if (!result)
			std::cout << "The statement is invalid.\n";
		else
			fWriteExecute << intervals->outputFormat() << std::endl; //Output the formatted string containing all the intervals
	}
	fWriteExecute.close(); //Save output file
	std::cout << "Results are now available. Enter any character to exit....\n";
	char chRead = getchar();
}
