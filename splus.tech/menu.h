#pragma once
#include <iostream>
#include <iomanip>

//display main menu
static void man_menu_view(std::ostream& os, std::string_view name) {
	os << "\n***|";
	os << "http serv";
	os << "\n***|";
	os << "Help menu";
	os << "\n***|http serv(1)";
	os << "\n***|SYNOPSIS:";
	os << "\n***|		" << name << " ";
	os << "\n***|		" << name << " [option] [data]...";
	os << "\n***|DESCRIPTION:";
	os << "\n***|		" << name << "                          (server 0.0.0.0 80)";
	os << "\n***|		" << name << " /s [bind-address] [port] (server start)";
	os << "\n***|		" << name << " /?                       (help menu)";
	os << "\n***|		" << name << " /man                     (man menu)";
	os << "\n...by Dies_Irae\n";
}

static void help_menu_view(std::ostream& os, std::string_view name) {
	os << "	Usage: " << name << " /s ... IP ... PORT\n\n";
	os << "	OPTIONS: \n";
	os << "		s - Start server\n";
	os << "		m - man menu\n";
	os << "		? - help menu\n\n";
	os << "	EXAMPLES: \n";
	os << "		" << name << "\n";
	os << "		" << name << " /s 0.0.0.0 8080\n";
	os << "		" << name << " /s 192.168.1.1 80\n\n";
	os << "		" << name << " /h\n";
	os << "		" << name << " /?\n";
}
