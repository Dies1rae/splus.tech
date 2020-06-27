#pragma once
#include <iostream>
#include <windows.h>
#include <iomanip>
using namespace std;

//func declar
void help_menu_view();
void hidecursor();
void UNhidecursor();
void man_menu_view();
//display main menu
void man_menu_view() {
	hidecursor();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << endl << "***|";
	SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | 5);
	cout << "http serv";
	SetConsoleTextAttribute(hConsole, 7);
	cout << endl << "***|";
	SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | 5);
	cout << "Help menu";
	SetConsoleTextAttribute(hConsole, 7);
	SetConsoleTextAttribute(hConsole, 8);
	cout << endl << "***|" << "http serv(1)";
	SetConsoleTextAttribute(hConsole, 7);
	SetConsoleTextAttribute(hConsole, 8);
	cout << endl << "***|" << "SYNOPSIS:";
	SetConsoleTextAttribute(hConsole, 7);
	cout << endl << "***|" << "		cplus.tech.exe [OPTION] ... [DATA]";
	cout << endl << "***|" << "		cplus.tech.exe [OPTION] ... [DATA] ... [DATA]";
	SetConsoleTextAttribute(hConsole, 8);
	cout << endl << "***|" << "DESCRIPTION:";
	SetConsoleTextAttribute(hConsole, 7);
	cout << endl << "***|" << "		./cplus.tech.exe /s(server start) ... IPADDRESS FOR SRV ... PORT";
	cout << endl << "***|" << "		./cplus.tech.exe /?(help menu)";
	cout << endl << "***|" << "		./cplus.tech.exe /man(man menu)";
	SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | 9);
	cout << endl << "...by Dies_Irae" << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	SetConsoleTextAttribute(hConsole, 7);
	UNhidecursor();
}

void help_menu_view() {
	cout << "	Usage: cplus.tech.exe /s ... IP ... PORT" << endl << endl;
	cout << "	OPTIONS: " << endl;
	cout << "		s - Start server" << endl;
	cout << "		m - man menu" << endl;
	cout << "		? - help menu" << endl << endl;
	cout << "	EXAMPLES: " << endl;
	cout << "		cplus.tech.exe /s 0.0.0.0 8080" << endl;
	cout << "		cplus.tech.exe /s 192.168.1.1 80" << endl << endl;
	cout << "		cplus.tech.exe /h" << endl;
	cout << "		cplus.tech.exe /?" << endl;
}

void hidecursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}
void UNhidecursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.bVisible = TRUE;
	SetConsoleCursorInfo(consoleHandle, &info);
}