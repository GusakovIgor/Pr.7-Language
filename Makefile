Run: Language
	./Parser.out  ./Progs/SquareEquation.txt

Language: Parser.cpp Input.cpp ./Logs/LangLogs.cpp
	gcc -w Parser.cpp Input.cpp ./Logs/LangLogs.cpp -lm -o Parser.out

Clear: clear
	
