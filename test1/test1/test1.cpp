#include<iostream>
using namespace std;
int main()
{
	while (true)
	{
		cout << "�����룺";
		string a;
		cin >> a;
		if (a == "Dian")
		{
			cout << "2002" << endl;
		}
		else if (a == "Quit")
		{
			break;
		}
		else
		{
			cout << "Error" << endl;
		}
	}
	return 0;
}