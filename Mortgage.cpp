// Author: Michael Tarasov
// CIS 3100 BMWA Fall 2022
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
using namespace std;

double getLoanAmount()
{
	double input;
	do
	{
		cout << "Enter the loan amount (0-9999999) ";
		cin >> input;
	} while (input < 0 || input > 9999999);
	return input;
}
double getInterestRate()
{
	double input;
	do
	{
		cout << "Enter the anual interest rate (0-30) ";
		cin >> input;
	} while (input < 0 || input > 30);
	return input/100;
}
int getYears()
{
	int input;
	do
	{
		cout << "Enter the loan term in years (1-99) ";
		cin >> input;
	} while (input < 1 || input > 99);
	return input;
}
float getAdditionalPayment()
{
	float input;
	do
	{
		cout << "Enter additional principle paid each month (0-9999999) ";
		cin >> input;
	} while (input < 0 || input > 9999999);
	return input;
}

string toCurreny(arg value) {
	
}

void writeIntro(ofstream &outfile, double loan_amount, float interest_rate) {
	outfile << "\tMORTGAGE AMORTIZATION TABLE\n";
	outfile << "\n";
	outfile << "Amount:\t\t\t$" << loan_amount << "\n";
	outfile << "Interest Rate:\t\t" << interest_rate * 100 << "%\n";
}
int main()
{
	double loan_amount = getLoanAmount();
	double interest_rate = getInterestRate();
	int years = getYears();
	float additional_payment = getAdditionalPayment();
	string file_name;
	cout << "Send the mortgage amortization table to a file (enter file name): ";
	cin >> file_name;
	
	double monthlyPayment = (loan_amount * interest_rate/12)/(1-1/pow(1+interest_rate/12, years*12));
	// Round the monthly payment up to the nearest cent
	monthlyPayment = ceil(monthlyPayment * 100)/100;
	


	ofstream outfile(file_name);

	writeIntro(outfile, loan_amount, interest_rate);

	outfile.close();
	return 0;
}
