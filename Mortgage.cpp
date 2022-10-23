// Author: Michael Tarasov
// CIS 3100 BMWA Fall 2022
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
using namespace std;

const int MONTHSINYEAR = 12;

const int CENTFACTOR = 100;
const int PERCENTFACTOR = 1000;
const int PERCENTDIVISOR = 100;

const int THREEDIGIT = 100;
const int TWODIGIT = 10;
const int SEVENDIGIT = 1000000;

const int COMMASLOTS = 3; // The amount of digits between a comma
const int DECIMALDIVIDER = 10;

const int TOPLOAN = 9999999;
const int TOPINTEREST = 30;
const int TOPYEARS = 99;

const int SPACEADJUSTER = 14;	   // The space to be allocated for values in columns
const int HEADERCURRENCYSIZE = 12; // The space to be allocated for currency values in the header

double getLoanAmount()
{
	double input;
	do
	{
		cout << "Enter the loan amount (0-9999999) ";
		cin >> input;
	} while (input <= 0 || input > TOPLOAN);
	return input;
}
double getInterestRate()
{
	double input;
	do
	{
		cout << "Enter the anual interest rate (0-30) ";
		cin >> input;
	} while (input <= 0 || input > TOPINTEREST);
	return input;
}
int getYears()
{
	int input;
	do
	{
		cout << "Enter the loan term in years (1-99) ";
		cin >> input;
	} while (input < 1 || input > TOPYEARS);
	return input;
}
double getAdditionalPayment()
{
	double input;
	do
	{
		cout << "Enter additional principle paid each month (0-9999999) ";
		cin >> input;
	} while (input < 0 || input > TOPLOAN);
	return input;
}
string getFileName()
{
	string input;
	cout << "Send the mortgage amortization table to a file (enter file name): ";
	cin >> input;
	return input;
}

void addCommaSlots(int value, string &result)
{
	// Takes a string as an argument and the number that will be formatted with commas
	// Formats the number as a string with commas
	do
	{
		const int COMMADIVIDER = pow(DECIMALDIVIDER, COMMASLOTS);
		int addition = value % COMMADIVIDER;
		value = value / COMMADIVIDER;
		result = to_string(addition) + result;
		if (value > 0)
		{
			// Add extra zeros to lower numbers before starting another loop if another loop is necessary
			for (int i = 1; i < COMMASLOTS; i++)
			{
				if (addition < pow(DECIMALDIVIDER, i))
				{
					result = '0' + result;
				}
			}
			result = ',' + result; // Add a comma before beginning another round
		}
	} while (value > 0);
}

string formatCurrency(double value)
{
	// Takes a double as an input. Returns the value formatted as currency.
	string result = "";
	int intval = round(value * CENTFACTOR);
	int cents = intval % CENTFACTOR;
	intval = intval / CENTFACTOR;
	if (cents < TWODIGIT)
	{
		// If the number after the decimal point is a single digit, append a 0 before the digit.
		result = ".0" + to_string(cents);
	}
	else
	{
		result = '.' + to_string(cents);
	}
	addCommaSlots(intval, result);
	return result;
}

string formatPercent(double value)
{
	// Used to output the interest rate to the smallest possible amount of decimal places
	int intval = value * PERCENTFACTOR;
	int predecimal = intval / PERCENTFACTOR;
	int postdecimal = intval % PERCENTFACTOR;
	string result;

	while (postdecimal % DECIMALDIVIDER == 0 && postdecimal != 0)
	{
		postdecimal = postdecimal / DECIMALDIVIDER;
	}

	if (postdecimal == 0)
	{
		result = to_string(predecimal);
	}
	else
	{
		result = to_string(predecimal) + '.' + to_string(postdecimal);
	}

	result = result + "%";
	return result;
}

string headerCurrency(double value)
{
	// Formats the value as a currency with extra spacing for smaller values
	string result = formatCurrency(value);

	while (result.length() < HEADERCURRENCYSIZE)
	{
		result = ' ' + result;
	}

	return result;
}

void writeIntro(ofstream &outfile, double loan_amount, double interest_rate, int years, double monthly_payment, double additional_payment)
{
	outfile << "\t MORTGAGE AMORTIZATION TABLE\n";
	outfile << "\n";
	outfile << "Amount:\t\t\t$" << headerCurrency(loan_amount) << "\n";
	outfile << "Interest Rate:\t\t" << formatPercent(interest_rate) << "\n";
	outfile << "Term(Years):\t\t" << years << "\n";
	outfile << "Monthly Payment:\t$" << headerCurrency(monthly_payment) << "\n";
	outfile << "Additonal Principal:\t$" << headerCurrency(additional_payment) << "\n";
	outfile << "Actual Payment:\t\t$" << headerCurrency(monthly_payment + additional_payment) << "\n";
	outfile << "\n";
}

void writeColumns(ofstream &outfile)
{
	outfile << "\t   Principal\t    Interest\t     Balance\n";
	// outfile << "1234567890223456789032345678904234567890523456789062345678907234567890\n"; // Measuring stick
}

void columnAdjustment(string &row, int length, int payment)
{
	while (row.length() < length)
	{
		row = " " + row;
	}
	if (payment == 1)
	{
		row = "$" + row;
	}
	else
	{
		row = " " + row;
	}
}

void writeRow(ofstream &outfile, double principle, double interest, double remaining)
{
	static int payment = 0;
	payment++;

	string row = "\n";

	// Remaining Balance
	row = formatCurrency(remaining) + row;
	int maxlength = SPACEADJUSTER - 1; // Decreased by one to match spec
	columnAdjustment(row, maxlength, payment);
	row = "\t" + row;

	// Interest Paid
	row = formatCurrency(interest) + row;
	maxlength = maxlength + SPACEADJUSTER;
	columnAdjustment(row, maxlength, payment);
	row = "\t" + row;

	// Principle Paid
	row = formatCurrency(principle) + row;
	maxlength = maxlength + SPACEADJUSTER;
	columnAdjustment(row, maxlength, payment);
	row = "\t" + row;

	row = to_string(payment) + row;

	outfile << row;
}

void makePayment(ofstream &outfile, double &loan_amount, double interest_rate, double monthly_payment, double additional_payment)
{
	double interest = loan_amount * interest_rate / (PERCENTDIVISOR * MONTHSINYEAR);
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
	const string file_name = getFileName();

	double monthly_payment = (loan_amount * interest_rate / (PERCENTDIVISOR * MONTHSINYEAR)) / (1 - 1 / pow(1 + interest_rate / (PERCENTDIVISOR * MONTHSINYEAR), years * MONTHSINYEAR));
	// Round the monthly payment up to the nearest cent
	// monthly_payment = ceil(monthly_payment * CENTFACTOR)/CENTFACTOR;

	ofstream outfile(file_name);

	writeIntro(outfile, loan_amount, interest_rate, years, monthly_payment, additional_payment);
	writeColumns(outfile);

	do
	{
		makePayment(outfile, loan_amount, interest_rate, monthly_payment, additional_payment);
	} while (round(loan_amount * CENTFACTOR) > 0);
	/* Run the above loop only if there is a principle amount that rounds to one cent
	This helps alleviate rounding issues caused by the monthly payment being rounded down and leaving behind some principle after the final payment.
	*/

	outfile.close();
	return 0;
}
