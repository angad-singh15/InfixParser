#include "Evaluator.h"
#include "EvaluatorHelper.h"
#include "Tokenizer.h"
Tokenizer::Tokenizer(string expression) {
	string result = "";
	string temp = "";
	// remove spaces
	for (int i = 0; i < expression.length(); i++) {
		if (expression[i] != ' ') temp += expression[i];
	}
	expression = temp;
	char ch = ' ';
	string token = "";
	istringstream tokens(expression);
	vector<string> exp;
	// iterate through each character and add each token to the vector exp
	while (tokens >> ch) {
		int tokenLen = token.length();
		bool shouldStartNewToken = (EvaluatorHelper::isOperator(token) && !EvaluatorHelper::isOperator(token + ch))
			|| (EvaluatorHelper::isNumber(token) && !EvaluatorHelper::isNumber(token + ch));
		bool isNegativeNumber = (token == "-" && EvaluatorHelper::isDigit(ch));
		if (!shouldStartNewToken || isNegativeNumber) token += ch;
		else {
			istringstream tempstream(token);
			int a = 0;
			tempstream >> a;
			if (a < 0 && (exp.size() > 0 && (exp[exp.size() - 1] == ")" || EvaluatorHelper::isNumber(exp[exp.size() - 1])))) exp.push_back("+");
			if (EvaluatorHelper::isNumber(token) || EvaluatorHelper::isOperator(token) || token == "") {
				if (token != "") exp.push_back(token);
			}
			else EvaluatorHelper::throwException(token + " is an invalid character/operator");
			token = ch;
		}
	}
	// concatentate result;
	if (token != "" && (EvaluatorHelper::isNumber(token) || EvaluatorHelper::isOperator(token))) {
		istringstream tempstream(token);
		int a = 0;
		tempstream >> a;
		if (a < 0 && (exp.size() > 0 && (exp[exp.size() - 1] == ")" || EvaluatorHelper::isNumber(exp[exp.size() - 1])))) exp.push_back("+");
		if (EvaluatorHelper::isNumber(token) || EvaluatorHelper::isOperator(token) || token == "") {
			if (token != "") exp.push_back(token);
		}
		else EvaluatorHelper::throwException(token + " is an invalid character/operator");
		token = ch;
	}
	else EvaluatorHelper::throwException(token + " is an invalid character/operator");

	Tokenizer::eqnPieces = exp; // set eqnPieces to exp
	correctOps(eqnPieces);
	currentPosition = 0; // Initialize currentPosition to 0
}
string Tokenizer::next() {
	if (eqnPieces.size() == 0) // If the vector of items is empty, return nothing, because there is nothing to be returned
		return "";
	else {
		if ((currentPosition + 1) == eqnPieces.size()) // If we are at the end of the vector, return nothing
			return "";
		currentPosition++; // Increment currentPosition for the next use
		return eqnPieces[currentPosition - 1]; // Return the item in the [currentPosition - 1] spot so the correct item is returned
	}
}

void Tokenizer::correctOps(vector <string> & eqn) {
	bool keepRecurse = true; // Initialize keepRecurse so the while loop

	while (keepRecurse) {

		string prevChar = ""; // Make a string that will be the previous character

		for (int i = 0; i < eqn.size(); i++) { // Iterate through the vector
			// If we find a ++ and then a + in the vector, (or a -- followed by -), we need to swap those operators because that is incorrect
			if ((prevChar == "++" || prevChar == "--") && (eqn[i] == "+" || eqn[i] == "-")) {
				eqn[i - 1] = eqn[i];
				eqn[i] = prevChar;
				keepRecurse = false; // Set keepRecurse to false so the recursive call will happen
			}

			prevChar = eqn[i]; // Move prevChar to the current position before the next iteration starts
		}

		if (keepRecurse) // If keepRecurse is never set to false, then that means the operators are in the correct order, so return
			return;

		if (keepRecurse == false) { // If keepRecurse is set to false, we need to iterate through the vector again
			correctOps(eqn); // So do a recursive call
			return;
		}
	}
}