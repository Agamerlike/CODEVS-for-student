/*スコアの上では同時大量消し<<連鎖数
 *よって2消しを連続させる方法が最も有効
 *17連鎖あたりを狙うのがベストか？（先制攻撃なら）
 *10連鎖2消し...おじゃま15個
 *15連鎖2消し...おじゃま58個
 *16連鎖2消し...おじゃま75個
 *17連鎖2消し...おじゃま98個
 *18連鎖2消し...おじゃま127個
 *19連鎖2消し...おじゃま165個
 *20連鎖2消し...おじゃま215個
 */
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
		score += floor(pow(1.3, chain)) + floor((double)cnt / 2);
	}
	ojama = floor((double)score/5);
	cout << "score:" << score << endl;
	cout << "ojama:" << ojama << endl;
}