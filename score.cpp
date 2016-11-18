#include <iostream>
#include <string>
using namespace std;
void main()
{
	int score=0;
	int ojama=0;
	int chain=0;
	int cnt;
	while(cnt > 0)
	{
		chain++;
		cout << "chain" << chain << endl;
		cin >> cnt;
		if(cnt == 0)
		{
			break;
		}
		score += floor(pow(1.3, chain)) + floor((double)cnt / 2);
	}
	ojama = floor((double)score/5);
	cout << "score:" << score << endl;
	cout << "ojama:" << ojama << endl;
}