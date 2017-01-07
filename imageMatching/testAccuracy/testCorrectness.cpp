#include <fstream>
#include <iostream>

using namespace std;

int main()
{
	ifstream Correct;
	ifstream Test;
	Correct.open("CorrectRes");
	Test.open("result");
	int corr, test;
	int num=0, CorNum=0;
	while (Correct >> corr)
	{
		Test >> test;
		num++;
		if (corr == test)
			CorNum++;
	}
	cout<<"Precision: "<<float(CorNum)/num<<endl;
}