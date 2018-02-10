CC		= g++
PROJECT		= IntervelManager
FILES		= Sequence.cpp	ParseCommand.cpp	IntervalsManager.cpp

${PROJECT}:${FILES}
	${CC} -std=c++14 -o ${PROJECT} ${FILES}
