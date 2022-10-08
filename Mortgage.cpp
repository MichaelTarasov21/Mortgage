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
	} while (input <= 0 || input > 9999999);
	return input;
}
double getInterestRate()
{
	double input;
	do
	{
		cout << "Enter the anual interest rate (0-30) ";
		cin >> input;
	} while (input <= 0 || input > 30);
	return input;
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
double getAdditionalPayment()
{
	double input;
	do
	{
		cout << "Enter additional principle paid each month (0-9999999) ";
		cin >> input;
	} while (input < 0 || input > 9999999);
	return input;
}

string formatCurrency(double value)
{
	// Takes a double as an input. Returns the value formatted as currency.
	string result = "";
	int intval = round(value * 100);
	if (intval % 100 <= 9)
	{
		// If the number after the decimal point is a single digit, append a 0 before the digit.
		result = result + to_string(intval / 100) + ".0" + to_string(intval % 100);
	}
	else
	{
		result = result + to_string(intval / 100) + "." + to_string(intval % 100);
	}
	return result;
}

string formatPercent(double value)
{
	// Used to output the interest rate to the third decimal place
	int intval = value * 1000;
	string result;
	result = to_string(intval / 1000);
	result = result + ".";
	if (intval % 1000 >= 100)
	{
		result = result + to_string(intval % 1000);
	}
	else if (intval % 1000 >= 10)
	{
		result = result + "0" + to_string(intval % 1000);
	}
	else
	{
		result = result + "00" + to_string(intval % 1000);
	}
	result = result + "%";
	return result;
}

void writeIntro(ofstream &outfile, double loan_amount, double interest_rate, int years, double monthly_payment, double additional_payment)
{
	outfile << "\t MORTGAGE AMORTIZATION TABLE\n";
	outfile << "\n";
	outfile << "Amount:\t\t\t$" << formatCurrency(loan_amount) << "\n";
	outfile << "Interest Rate:\t\t" << formatPercent(interest_rate) << "\n";
	outfile << "Term(Years):\t\t" << years << "\n";
	outfile << "Monthly Payment:\t$" << formatCurrency(monthly_payment) << "\n";
	outfile << "Additonal Principal:\t$" << formatCurrency(additional_payment) << "\n";
	outfile << "Actual Payment:\t\t$" << formatCurrency(monthly_payment + additional_payment) << "\n";
	outfile << "\n";
}

void writeColumns(ofstream &outfile)
{
	outfile << "\n";
	outfile << "\t\tPrincipal\t\tInterest\t\tBalance\n";
}
void writeRow(ofstream &outfile, double principle, double interest, double remaining)
{
	static int payment = 0;
	payment++;

	string row;
	row = formatCurrency(remaining);
	int maxlength = 10;
	while (row.length() < maxlength)
	{
		row = " " + row;
	}
	row = "\t" + row;
	if (payment == 1)
	{
		row = "$" + row;
		maxlength = maxlength + 1;
	}
	row = "\t" + row;

	row = formatCurrency(interest) + row;
	maxlength = maxlength + 10;
	while (row.length() < maxlength)
	{
		row = " " + row;
	}
	row = "\t" + row;
	if (payment == 1)
	{
		row = "$" + row;
		maxlength = maxlength + 1;
	}
	row = "\t" + row;

	row = formatCurrency(principle) + row;
	maxlength = maxlength + 11;
	while (row.length() < maxlength)
	{
		row = " " + row;
	}
	row = "\t" + row;
	if (payment == 1)
	{
		row = "$" + row;
		maxlength = maxlength + 1;
	}
	row = "\t" + row;

	row = to_string(payment) + row + "\n";

	outfile << row;
}

void makePayment(ofstream &outfile, double &loan_amount, double interest_rate, double monthly_payment, double additional_payment)
{
	double interest = loan_amount * interest_rate / (100 * 12);
	double principle_payment = monthly_payment + additional_payment - interest;
	if (principle_payment > loan_amount)
	{
		principle_payment = loan_amount;
	}
	loan_amount = loan_amount - principle_payment;
	writeRow(outfile, principle_payment, interest, loan_amount);
}

int main()
{
	double loan_amount = getLoanAmount();
	const double interest_rate = getInterestRate();
	const int years = getYears();
	const double additional_payment = getAdditionalPayment();
	string file_name;
	cout << "Send the mortgage amortization table to a file (enter file name): ";
	cin >> file_name;

	double monthly_payment = (loan_amount * interest_rate / (100 * 12)) / (1 - 1 / pow(1 + interest_rate / (100 * 12), years * 12));
	// Round the monthly payment up to the nearest cent
	// monthly_payment = ceil(monthly_payment * 100)/100;

	ofstream outfile(file_name);

	writeIntro(outfile, loan_amount, interest_rate, years, monthly_payment, additional_payment);
	writeColumns(outfile);

	do
	{
		makePayment(outfile, loan_amount, interest_rate, monthly_payment, additional_payment);
	} while (round(loan_amount * 100) > 0); 
	/* Run the above loop only if there is a principle amount that rounds to one cent
	This helps alleviate rounding issues cause by the monthly payment being rounded down and leaving behind some principle after the final payment.
	*/

	outfile.close();
	return 0;
}
