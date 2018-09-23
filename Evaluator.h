#include <string>
#include <iostream>
#include <map>
using namespace std;
#pragma once

class Evaluator {
private:
	int evalPostBool(string equation);
	int evalPostMath(string equation);
	string toPostMath(string equation);
	string toPostBool(string equation);
public:
	Evaluator();
	int eval(string equation);
};
