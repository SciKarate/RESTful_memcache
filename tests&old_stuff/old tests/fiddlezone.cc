#include <iostream>
using namespace std;

void called_boy(int& val_size)
{
	val_size = val_size + 5;
}

int main()
{
	int val_guy = 10;
	cout << val_guy << endl;
	called_boy(val_guy);
	cout << val_guy << endl;
	return 0;
}