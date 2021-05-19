Run: Language
	clear
	bin/Parser.out ./Progs/SquareEquation.txt

Language: Parser.cpp InputProc.cpp ./Logs/LangLogs.cpp
	clear
	g++ -w Parser.cpp InputProc.cpp ./Logs/LangLogs.cpp -lm -o bin/Parser.out