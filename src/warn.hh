// vim: noet ts=4 sw=4 sts=0
#pragma once
#include <iostream>
#include <string>
using namespace std;

namespace jopang
{
	void warn(const string& msg)
	{
		cerr << "\e[1;35mwarning: \e[1;34m" << msg << "\e[0m" << endl;
	}
};

