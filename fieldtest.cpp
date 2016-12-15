#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <time.h>
using namespace std;

mt19937 MT((unsigned int)time(NULL));

/**
 * from以上、to未満の乱数を返します。
 */
int randInt(int from, int to) {
  uniform_int_distribution<int> rand(from, to - 1);
  return rand(MT);
}

	/*与えられたフィールドに対してブロックを消すシミュレーションを行う関数（連鎖込み）
	**得られたスコアを返す
	*/
  int DelBrock(int field[][10])
  {
    int flag = 1;
    int score = 0;//獲得スコア
    int add = 0;//現在までの加算結果
    int chain = 0;//チェイン数
    int cnt = 0;//消滅カウント
	int h=0;
    bool flagfield[19][10];//消すブロックを管理するためのフィールド
    while (flag == 1)
		{
			flag = 0;
			chain++;
			for (int i = 0; i<19; i++)
			{
				for (int j = 0; j<10; j++)
				{
					flagfield[i][j] = false;
				}
			}
			//ブロックを落とす
			for (int i = 18;i > -1; i--)
			{
				for (int j = 0; j < 10; j++)
				{
					if (field[i][j] == 0)
					{
						h=i;
						while(h>-1&&field[h][j]==0)
						{
							h--;
						}
						if(h>-1)
						{
							field[i][j] = field[h][j];
							field[h][j] = 0;
						}
					}
				}
			}
			//盤面の出力
			cout << "chain" << chain-1 << endl;
			for(int i=0;i<19;i++)
			{
				for(int j=0;j<10;j++)
				{
					cout << field[i][j] << " ";
				}
				cout << endl;
			}
			//消すことをシミュレート
			for (int i = 0; i < 19; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					//上下左右斜めについて調べる
					//左
					if (j - 1 > -1)
					{
						//左上
						if (i - 1 > -1 && (add = field[i - 1][j - 1] + field[i][j]) == 10)
						{
							flag = 1;
							flagfield[i][j]=true;
              flagfield[i-1][j-1]=true;
						}else if(j-2 > -1 && i-2 > -1 &&field[i-1][j-1] != 0&& add < 10)
            {
              if((add += field[i-2][j-2]) == 10)
              {
                flag = 1;
                flagfield[i][j]=true;
                flagfield[i-1][j-1]=true;
                flagfield[i-2][j-2]=true;
							}
							else if (j - 3 > -1 && i - 3 > -1 && field[i - 2][j - 2] != 0 && add < 10)
							{
								if ((add += field[i - 3][j - 3]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i - 1][j - 1] = true;
									flagfield[i - 2][j - 2] = true;
									flagfield[i - 3][j - 3] = true;
								}
								else if (j - 4 > -1 && i - 4 > -1 && field[i - 3][j - 3] != 0 && add < 10)
								{
									if ((add += field[i - 4][j - 4]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i - 1][j - 1] = true;
										flagfield[i - 2][j - 2] = true;
										flagfield[i - 3][j - 3] = true;
										flagfield[i - 4][j - 4] = true;
									}
									else if (j - 5 > -1 && i - 5 > -1 && field[i - 4][j - 4] != 0 && add < 10)
									{
										if ((add += field[i - 5][j - 5]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i - 1][j - 1] = true;
											flagfield[i - 2][j - 2] = true;
											flagfield[i - 3][j - 3] = true;
											flagfield[i - 4][j - 4] = true;
											flagfield[i - 5][j - 5] = true;
										}
										else if (j - 6 > -1 && i - 6 > -1 && field[i - 5][j - 5] != 0 && add < 10)
										{
											if ((add += field[i - 6][j - 6]) == 10)
											{
												flag = 1;
												flagfield[i][j] = true;
												flagfield[i - 1][j - 1] = true;
												flagfield[i - 2][j - 2] = true;
												flagfield[i - 3][j - 3] = true;
												flagfield[i - 4][j - 4] = true;
												flagfield[i - 5][j - 5] = true;
												flagfield[i - 6][j - 6] = true;
											}
											else if (j - 7 > -1 && i - 7 > -1 && field[i - 6][j - 6] != 0 && add < 10)
											{
												if ((add += field[i - 7][j - 7]) == 10)
												{
													flag = 1;
													flagfield[i][j] = true;
													flagfield[i - 1][j - 1] = true;
													flagfield[i - 2][j - 2] = true;
													flagfield[i - 3][j - 3] = true;
													flagfield[i - 4][j - 4] = true;
													flagfield[i - 5][j - 5] = true;
													flagfield[i - 6][j - 6] = true;
													flagfield[i - 7][j - 7] = true;
												}
												else if (j - 8 > -1 && i - 8 > -1 && field[i - 7][j - 7] != 0 && add < 10)
												{
													if ((add += field[i - 8][j - 8]) == 10)
													{
														flag = 1;
														flagfield[i][j] = true;
														flagfield[i - 1][j - 1] = true;
														flagfield[i - 2][j - 2] = true;
														flagfield[i - 3][j - 3] = true;
														flagfield[i - 4][j - 4] = true;
														flagfield[i - 5][j - 5] = true;
														flagfield[i - 6][j - 6] = true;
														flagfield[i - 7][j - 7] = true;
														flagfield[i - 8][j - 8] = true;
													}
													else if (j - 9 > -1 && i - 9 > -1 && field[i - 8][j - 8] != 0 && add < 10)
													{
														if ((add += field[i - 9][j - 9]) == 10)
														{
															flag = 1;
															flagfield[i][j] = true;
															flagfield[i - 1][j - 1] = true;
															flagfield[i - 2][j - 2] = true;
															flagfield[i - 3][j - 3] = true;
															flagfield[i - 4][j - 4] = true;
															flagfield[i - 5][j - 5] = true;
															flagfield[i - 6][j - 6] = true;
															flagfield[i - 7][j - 7] = true;
															flagfield[i - 8][j - 8] = true;
															flagfield[i - 9][j - 9] = true;
														}
													}
												}
											}
										}
									}
								}
							}
            }
						//左
						if ((add = field[i][j - 1] + field[i][j]) == 10)
						{
							flag = 1;
							flagfield[i][j - 1] = true;
							flagfield[i][j] = true;
						}
						else if (j - 2 > -1 && field[i][j - 1] != 0 && add < 10) {
							if ((add += field[i][j - 2]) == 10)
							{
								flag = 1;
								flagfield[i][j] = true;
								flagfield[i][j - 1] = true;
								flagfield[i][j - 2] = true;
							}
							else if (j - 3 > -1 && field[i][j - 2] != 0 && add < 10) {
								if ((add += field[i][j - 3]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i][j - 1] = true;
									flagfield[i][j - 2] = true;
									flagfield[i][j - 3] = true;
								}
								else if (j - 4 > -1 && field[i][j - 3] != 0 && add < 10) {
									if ((add += field[i][j - 4]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i][j - 1] = true;
										flagfield[i][j - 2] = true;
										flagfield[i][j - 3] = true;
										flagfield[i][j - 4] = true;
									}
									else if (j - 5 > -1 && field[i][j - 4] != 0 && add < 10) {
										if ((add += field[i][j - 5]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i][j - 1] = true;
											flagfield[i][j - 2] = true;
											flagfield[i][j - 3] = true;
											flagfield[i][j - 4] = true;
											flagfield[i][j - 5] = true;
										}
										else if (j - 6 > -1 && field[i][j - 5] != 0 && add < 10) {
											if ((add += field[i][j - 6]) == 10)
											{
												flag = 1;
												flagfield[i][j] = true;
												flagfield[i][j - 1] = true;
												flagfield[i][j - 2] = true;
												flagfield[i][j - 3] = true;
												flagfield[i][j - 4] = true;
												flagfield[i][j - 5] = true;
												flagfield[i][j - 6] = true;
											}
											else if (j - 7 > -1 && field[i][j - 6] != 0 && add < 10) {
												if ((add += field[i][j - 7]) == 10)
												{
													flag = 1;
													flagfield[i][j] = true;
													flagfield[i][j - 1] = true;
													flagfield[i][j - 2] = true;
													flagfield[i][j - 3] = true;
													flagfield[i][j - 4] = true;
													flagfield[i][j - 5] = true;
													flagfield[i][j - 6] = true;
													flagfield[i][j - 7] = true;
												}
												else if (j - 8 > -1 && field[i][j - 7] != 0 && add < 10) {
													if ((add += field[i][j - 8]) == 10)
													{
														flag = 1;
														flagfield[i][j] = true;
														flagfield[i][j - 1] = true;
														flagfield[i][j - 2] = true;
														flagfield[i][j - 3] = true;
														flagfield[i][j - 4] = true;
														flagfield[i][j - 5] = true;
														flagfield[i][j - 6] = true;
														flagfield[i][j - 7] = true;
														flagfield[i][j - 8] = true;
													}
													else if (j - 9 > -1 && field[i][j - 8] != 0 && add < 10) {
														if ((add += field[i][j - 9]) == 10)
														{
															flag = 1;
															flagfield[i][j] = true;
															flagfield[i][j - 1] = true;
															flagfield[i][j - 2] = true;
															flagfield[i][j - 3] = true;
															flagfield[i][j - 4] = true;
															flagfield[i][j - 5] = true;
															flagfield[i][j - 6] = true;
															flagfield[i][j - 7] = true;
															flagfield[i][j - 8] = true;
															flagfield[i][j - 9] = true;
														}
													}
												}
											}
										}
									}
								}
							}
						}
						//左下
						if (i + 1 < 19 && (add=field[i + 1][j - 1] + field[i][j]) == 10)
						{
							flag = 1;
							flagfield[i + 1][j - 1] = true;
							flagfield[i][j] = true;
						}
						else if (j - 2 > -1 && i + 2 < 19 && field[i+1][j - 1] != 0 && add < 10) {
							if ((add += field[i + 2][j - 2]) == 10)
							{
								flag = 1;
								flagfield[i][j] = true;
								flagfield[i + 1][j - 1] = true;
								flagfield[i + 2][j - 2] = true;
							}
							else if (j - 3 > -1 && i + 3 < 19 && field[i + 2][j - 2] != 0 && add < 10) {
								if ((add += field[i + 3][j - 3]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i + 1][j - 1] = true;
									flagfield[i + 2][j - 2] = true;
									flagfield[i + 3][j - 3] = true;
								}
								else if (j - 4 > -1 && i + 4 < 19 && field[i + 3][j - 3] != 0 && add < 10) {
									if ((add += field[i + 4][j - 4]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i + 1][j - 1] = true;
										flagfield[i + 2][j - 2] = true;
										flagfield[i + 3][j - 3] = true;
										flagfield[i + 4][j - 4] = true;
									}
									else if (j - 5 > -1 && i + 5 < 19 && field[i + 4][j - 4] != 0 && add < 10) {
										if ((add += field[i + 5][j - 5]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i + 1][j - 1] = true;
											flagfield[i + 2][j - 2] = true;
											flagfield[i + 3][j - 3] = true;
											flagfield[i + 4][j - 4] = true;
											flagfield[i + 5][j - 5] = true;
										}
										else if (j - 6 > -1 && i + 6 < 19 && field[i + 5][j - 5] != 0 && add < 10) {
											if ((add += field[i + 6][j - 6]) == 10)
											{
												flag = 1;
												flagfield[i][j] = true;
												flagfield[i + 1][j - 1] = true;
												flagfield[i + 2][j - 2] = true;
												flagfield[i + 3][j - 3] = true;
												flagfield[i + 4][j - 4] = true;
												flagfield[i + 5][j - 5] = true;
												flagfield[i + 6][j - 6] = true;
											}
											else if (j - 7 > -1 && i + 7 < 19 && field[i + 6][j - 6] != 0 && add < 10) {
												if ((add += field[i + 7][j - 7]) == 10)
												{
													flag = 1;
													flagfield[i][j] = true;
													flagfield[i + 1][j - 1] = true;
													flagfield[i + 2][j - 2] = true;
													flagfield[i + 3][j - 3] = true;
													flagfield[i + 4][j - 4] = true;
													flagfield[i + 5][j - 5] = true;
													flagfield[i + 6][j - 6] = true;
													flagfield[i + 7][j - 7] = true;
												}
												else if (j - 8 > -1 && i + 8 < 19 && field[i + 7][j - 7] != 0 && add < 10) {
													if ((add += field[i + 8][j - 8]) == 10)
													{
														flag = 1;
														flagfield[i][j] = true;
														flagfield[i + 1][j - 1] = true;
														flagfield[i + 2][j - 2] = true;
														flagfield[i + 3][j - 3] = true;
														flagfield[i + 4][j - 4] = true;
														flagfield[i + 5][j - 5] = true;
														flagfield[i + 6][j - 6] = true;
														flagfield[i + 7][j - 7] = true;
														flagfield[i + 8][j - 8] = true;
													}
													else if (j - 9 > -1 && i +9 < 19 && field[i + 8][j - 8] != 0 && add < 10) {
														if ((add += field[i + 9][j - 9]) == 10)
														{
															flag = 1;
															flagfield[i][j] = true;
															flagfield[i + 1][j - 1] = true;
															flagfield[i + 2][j - 2] = true;
															flagfield[i + 3][j - 3] = true;
															flagfield[i + 4][j - 4] = true;
															flagfield[i + 5][j - 5] = true;
															flagfield[i + 6][j - 6] = true;
															flagfield[i + 7][j - 7] = true;
															flagfield[i + 8][j - 8] = true;
															flagfield[i + 9][j - 9] = true;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
					//上
					if (i - 1 > -1 && (add=field[i - 1][j] + field[i][j]) == 10)
					{
						flag = 1;
						flagfield[i - 1][j] = true;
						flagfield[i][j] = true;
					}
					else if (i - 2 > -1 && field[i - 1][j] != 0 && add < 10) {
						if ((add += field[i - 2][j]) == 10)
						{
							flag = 1;
							flagfield[i][j] = true;
							flagfield[i - 1][j] = true;
							flagfield[i - 2][j] = true;
						}
						else if (i - 3 > -1 && field[i - 2][j] != 0 && add < 10) {
							if ((add += field[i - 3][j]) == 10)
							{
								flag = 1;
								flagfield[i][j] = true;
								flagfield[i - 1][j] = true;
								flagfield[i - 2][j] = true;
								flagfield[i - 3][j] = true;
							}
							else if (i - 4 > -1 && field[i - 3][j] != 0 && add < 10) {
								if ((add += field[i - 4][j]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i - 1][j] = true;
									flagfield[i - 2][j] = true;
									flagfield[i - 3][j] = true;
									flagfield[i - 4][j] = true;
								}
								else if (i - 5 > -1 && field[i - 4][j] != 0 && add < 10) {
									if ((add += field[i - 5][j]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i - 1][j] = true;
										flagfield[i - 2][j] = true;
										flagfield[i - 3][j] = true;
										flagfield[i - 4][j] = true;
										flagfield[i - 5][j] = true;
									}
									else if (i - 6 > -1 && field[i - 5][j] != 0 && add < 10) {
										if ((add += field[i - 6][j]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i - 1][j] = true;
											flagfield[i - 2][j] = true;
											flagfield[i - 3][j] = true;
											flagfield[i - 4][j] = true;
											flagfield[i - 5][j] = true;
											flagfield[i - 6][j] = true;
										}
										else if (i - 7 > -1 && field[i - 6][j] != 0 && add < 10) {
											if ((add += field[i - 7][j]) == 10)
											{
												flag = 1;
												flagfield[i][j] = true;
												flagfield[i - 1][j] = true;
												flagfield[i - 2][j] = true;
												flagfield[i - 3][j] = true;
												flagfield[i - 4][j] = true;
												flagfield[i - 5][j] = true;
												flagfield[i - 6][j] = true;
												flagfield[i - 7][j] = true;
											}
											else if (i - 8 > -1 && field[i - 7][j] != 0 && add < 10) {
												if ((add += field[i - 8][j]) == 10)
												{
													flag = 1;
													flagfield[i][j] = true;
													flagfield[i - 1][j] = true;
													flagfield[i - 2][j] = true;
													flagfield[i - 3][j] = true;
													flagfield[i - 4][j] = true;
													flagfield[i - 5][j] = true;
													flagfield[i - 6][j] = true;
													flagfield[i - 7][j] = true;
													flagfield[i - 8][j] = true;
												}
												else if (i - 9 > -1 && field[i - 8][j] != 0 && add < 10) {
													if ((add += field[i - 9][j]) == 10)
													{
														flag = 1;
														flagfield[i][j] = true;
														flagfield[i - 1][j] = true;
														flagfield[i - 2][j] = true;
														flagfield[i - 3][j] = true;
														flagfield[i - 4][j] = true;
														flagfield[i - 5][j] = true;
														flagfield[i - 6][j] = true;
														flagfield[i - 7][j] = true;
														flagfield[i - 8][j] = true;
														flagfield[i - 9][j] = true;
													}
												}
											}
										}
									}
								}
							}
						}
					}
					//右
					if (j + 1 < 10)
					{
						//右上
						if (i - 1 > -1 && (add=field[i - 1][j + 1] + field[i][j]) == 10)
						{
							flag = 1;
							flagfield[i - 1][j + 1] = true;
							flagfield[i][j] = true;
						}
						else if (j + 2 < 10 && i - 2 > -1 && field[i - 1][j + 1] != 0 && add < 10) {
							if ((add += field[i - 2][j + 2]) == 10)
							{
								flag = 1;
								flagfield[i][j] = true;
								flagfield[i - 1][j + 1] = true;
								flagfield[i - 2][j + 2] = true;
							}
							else if (j + 3 < 10 && i - 3 > -1 && field[i - 2][j + 2] != 0 && add < 10) {
								if ((add += field[i - 3][j + 3]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i - 1][j + 1] = true;
									flagfield[i - 2][j + 2] = true;
									flagfield[i - 3][j + 3] = true;
								}
								else if (j + 4 < 10 && i - 4 > -1 && field[i - 3][j + 3] != 0 && add < 10) {
									if ((add += field[i - 4][j + 4]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i - 1][j + 1] = true;
										flagfield[i - 2][j + 2] = true;
										flagfield[i - 3][j + 3] = true;
										flagfield[i - 4][j + 4] = true;
									}
									else if (j + 5 < 10 && i - 5 > -1 && field[i - 4][j + 4] != 0 && add < 10) {
										if ((add += field[i - 5][j + 5]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i - 1][j + 1] = true;
											flagfield[i - 2][j + 2] = true;
											flagfield[i - 3][j + 3] = true;
											flagfield[i - 4][j + 4] = true;
											flagfield[i - 5][j + 5] = true;
										}
										else if (j + 6 < 10 && i - 6 > -1 && field[i - 5][j + 5] != 0 && add < 10) {
											if ((add += field[i - 6][j + 6]) == 10)
											{
												flag = 1;
												flagfield[i][j] = true;
												flagfield[i - 1][j + 1] = true;
												flagfield[i - 2][j + 2] = true;
												flagfield[i - 3][j + 3] = true;
												flagfield[i - 4][j + 4] = true;
												flagfield[i - 5][j + 5] = true;
												flagfield[i - 6][j + 6] = true;
											}
											else if (j + 7 < 10 && i - 7 > -1 && field[i - 6][j + 6] != 0 && add < 10) {
												if ((add += field[i - 7][j + 7]) == 10)
												{
													flag = 1;
													flagfield[i][j] = true;
													flagfield[i - 1][j + 1] = true;
													flagfield[i - 2][j + 2] = true;
													flagfield[i - 3][j + 3] = true;
													flagfield[i - 4][j + 4] = true;
													flagfield[i - 5][j + 5] = true;
													flagfield[i - 6][j + 6] = true;
													flagfield[i - 7][j + 7] = true;
												}
												else if (j + 8 < 10 && i - 8 > -1 && field[i - 7][j + 7] != 0 && add < 10) {
													if ((add += field[i - 8][j + 8]) == 10)
													{
														flag = 1;
														flagfield[i][j] = true;
														flagfield[i - 1][j + 1] = true;
														flagfield[i - 2][j + 2] = true;
														flagfield[i - 3][j + 3] = true;
														flagfield[i - 4][j + 4] = true;
														flagfield[i - 5][j + 5] = true;
														flagfield[i - 6][j + 6] = true;
														flagfield[i - 7][j + 7] = true;
														flagfield[i - 8][j + 8] = true;
													}
													else if (j + 9 < 10 && i - 9 > -1 && field[i - 8][j + 8] != 0 && add < 10) {
														if ((add += field[i - 9][j + 9]) == 10)
														{
															flag = 1;
															flagfield[i][j] = true;
															flagfield[i - 1][j + 1] = true;
															flagfield[i - 2][j + 2] = true;
															flagfield[i - 3][j + 3] = true;
															flagfield[i - 4][j + 4] = true;
															flagfield[i - 5][j + 5] = true;
															flagfield[i - 6][j + 6] = true;
															flagfield[i - 7][j + 7] = true;
															flagfield[i - 8][j + 8] = true;
															flagfield[i - 9][j + 9] = true;
														}
													}
												}
											}
										}
									}
								}
							}
						}
						//右
						if ((add=field[i][j + 1] + field[i][j]) == 10)
						{
							flag = 1;
							flagfield[i][j + 1] = true;
							flagfield[i][j] = true;
						}
						else if (j + 2 < 10 && field[i][j + 1] != 0 && add < 10)
						{
							if ((add += field[i][j + 2]) == 10)
							{
								flag = 1;
								flagfield[i][j] = true;
								flagfield[i][j + 1] = true;
								flagfield[i][j + 2] = true;
							}
							else if (j + 3 < 10 && field[i][j + 2] != 0 && add < 10)
							{
								if ((add += field[i][j + 3]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i][j + 1] = true;
									flagfield[i][j + 2] = true;
									flagfield[i][j + 3] = true;
								}
								else if (j + 4 < 10 && field[i][j + 3] != 0 && add < 10)
								{
									if ((add += field[i][j + 4]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i][j + 1] = true;
										flagfield[i][j + 2] = true;
										flagfield[i][j + 3] = true;
										flagfield[i][j + 4] = true;
									}
									else if (j + 5 < 10 && field[i][j + 4] != 0 && add < 10)
									{
										if ((add += field[i][j + 5]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i][j + 1] = true;
											flagfield[i][j + 2] = true;
											flagfield[i][j + 3] = true;
											flagfield[i][j + 4] = true;
											flagfield[i][j + 5] = true;
										}
										else if (j + 6 < 10 && field[i][j + 5] != 0 && add < 10)
										{
											if ((add += field[i][j + 6]) == 10)
											{
												flag = 1;
												flagfield[i][j] = true;
												flagfield[i][j + 1] = true;
												flagfield[i][j + 2] = true;
												flagfield[i][j + 3] = true;
												flagfield[i][j + 4] = true;
												flagfield[i][j + 5] = true;
												flagfield[i][j + 6] = true;
											}
											else if (j + 7 < 10 && field[i][j + 6] != 0 && add < 10)
											{
												if ((add += field[i][j + 7]) == 10)
												{
													flag = 1;
													flagfield[i][j] = true;
													flagfield[i][j + 1] = true;
													flagfield[i][j + 2] = true;
													flagfield[i][j + 3] = true;
													flagfield[i][j + 4] = true;
													flagfield[i][j + 5] = true;
													flagfield[i][j + 6] = true;
													flagfield[i][j + 7] = true;
												}
												else if (j + 8 < 10 && field[i][j + 7] != 0 && add < 10)
												{
													if ((add += field[i][j + 8]) == 10)
													{
														flag = 1;
														flagfield[i][j] = true;
														flagfield[i][j + 1] = true;
														flagfield[i][j + 2] = true;
														flagfield[i][j + 3] = true;
														flagfield[i][j + 4] = true;
														flagfield[i][j + 5] = true;
														flagfield[i][j + 6] = true;
														flagfield[i][j + 7] = true;
														flagfield[i][j + 8] = true;
													}
													else if (j + 9 < 10 && field[i][j + 8] != 0 && add < 10)
													{
														if ((add += field[i][j + 9]) == 10)
														{
															flag = 1;
															flagfield[i][j] = true;
															flagfield[i][j + 1] = true;
															flagfield[i][j + 2] = true;
															flagfield[i][j + 3] = true;
															flagfield[i][j + 4] = true;
															flagfield[i][j + 5] = true;
															flagfield[i][j + 6] = true;
															flagfield[i][j + 7] = true;
															flagfield[i][j + 8] = true;
															flagfield[i][j + 9] = true;
														}
													}
												}
											}
										}
									}
								}
							}
						}
						//右下
						if (i + 1 < 19 && (add=field[i + 1][j + 1] + field[i][j]) == 10)
						{
							flag = 1;
							flagfield[i + 1][j + 1] = true;
							flagfield[i][j] = true;
						}
						else if (i + 2 < 19 && j + 2 < 10 && field[i + 1][j + 1] != 0 && add < 10)
						{
							if ((add += field[i + 2][j + 2]) == 10)
							{
								flag = 1;
								flagfield[i][j] = true;
								flagfield[i + 1][j + 1] = true;
								flagfield[i + 2][j + 2] = true;
							}
							else if (i + 3 < 19 && j + 3 < 10 && field[i + 2][j + 2] != 0 && add < 10)
							{
								if ((add += field[i + 3][j + 3]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i + 1][j + 1] = true;
									flagfield[i + 2][j + 2] = true;
									flagfield[i + 3][j + 3] = true;
								}
								else if (i + 4 < 19 && j + 4 < 10 && field[i + 3][j + 3] != 0 && add < 10)
								{
									if ((add += field[i + 4][j + 4]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i + 1][j + 1] = true;
										flagfield[i + 2][j + 2] = true;
										flagfield[i + 3][j + 3] = true;
										flagfield[i + 4][j + 4] = true;
									}
									else if (i + 5 < 19 && j + 5 < 10 && field[i + 4][j + 4] != 0 && add < 10)
									{
										if ((add += field[i + 5][j + 5]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i + 1][j + 1] = true;
											flagfield[i + 2][j + 2] = true;
											flagfield[i + 3][j + 3] = true;
											flagfield[i + 4][j + 4] = true;
											flagfield[i + 5][j + 5] = true;
										}
										else if (i + 6 < 19 && j + 6 < 10 && field[i + 5][j + 5] != 0 && add < 10)
										{
											if ((add += field[i + 6][j + 6]) == 10)
											{
												flag = 1;
												flagfield[i][j] = true;
												flagfield[i + 1][j + 1] = true;
												flagfield[i + 2][j + 2] = true;
												flagfield[i + 3][j + 3] = true;
												flagfield[i + 4][j + 4] = true;
												flagfield[i + 5][j + 5] = true;
												flagfield[i + 6][j + 6] = true;
											}
											else if (i + 7 < 19 && j + 7 < 10 && field[i + 6][j + 6] != 0 && add < 10)
											{
												if ((add += field[i + 7][j + 7]) == 10)
												{
													flag = 1;
													flagfield[i][j] = true;
													flagfield[i + 1][j + 1] = true;
													flagfield[i + 2][j + 2] = true;
													flagfield[i + 3][j + 3] = true;
													flagfield[i + 4][j + 4] = true;
													flagfield[i + 5][j + 5] = true;
													flagfield[i + 6][j + 6] = true;
													flagfield[i + 7][j + 7] = true;
												}
												else if (i + 8 < 19 && j + 8 < 10 && field[i + 7][j + 7] != 0 && add < 10)
												{
													if ((add += field[i + 8][j + 8]) == 10)
													{
														flag = 1;
														flagfield[i][j] = true;
														flagfield[i + 1][j + 1] = true;
														flagfield[i + 2][j + 2] = true;
														flagfield[i + 3][j + 3] = true;
														flagfield[i + 4][j + 4] = true;
														flagfield[i + 5][j + 5] = true;
														flagfield[i + 6][j + 6] = true;
														flagfield[i + 7][j + 7] = true;
														flagfield[i + 8][j + 8] = true;
													}
													else if (i + 9 < 19 && j + 9 < 10 && field[i + 8][j + 8] != 0 && add < 10)
													{
														if ((add += field[i + 9][j + 9]) == 10)
														{
															flag = 1;
															flagfield[i][j] = true;
															flagfield[i + 1][j + 1] = true;
															flagfield[i + 2][j + 2] = true;
															flagfield[i + 3][j + 3] = true;
															flagfield[i + 4][j + 4] = true;
															flagfield[i + 5][j + 5] = true;
															flagfield[i + 6][j + 6] = true;
															flagfield[i + 7][j + 7] = true;
															flagfield[i + 8][j + 8] = true;
															flagfield[i + 9][j + 9] = true;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
					//下
					if (i + 1 < 19 && (add=field[i + 1][j] + field[i][j]) == 10)
					{
						flag = 1;
						flagfield[i + 1][j] = true;
						flagfield[i][j] = true;
					}
					else if (i + 2 < 19 && field[i + 1][j] != 0 && add < 10)
					{
						if ((add += field[i + 2][j]) == 10)
						{
							flag = 1;
							flagfield[i][j] = true;
							flagfield[i + 1][j] = true;
							flagfield[i + 2][j] = true;
						}
						else if (i + 3 < 19 && field[i + 2][j] != 0 && add < 10)
						{
							if ((add += field[i + 3][j]) == 10)
							{
								flag = 1;
								flagfield[i][j] = true;
								flagfield[i + 1][j] = true;
								flagfield[i + 2][j] = true;
								flagfield[i + 3][j] = true;
							}
							else if (i + 4 < 19 && field[i + 3][j] != 0 && add < 10)
							{
								if ((add += field[i + 4][j]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i + 1][j] = true;
									flagfield[i + 2][j] = true;
									flagfield[i + 3][j] = true;
									flagfield[i + 4][j] = true;
								}
								else if (i + 5 < 19 && field[i + 4][j] != 0 && add < 10)
								{
									if ((add += field[i + 5][j]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i + 1][j] = true;
										flagfield[i + 2][j] = true;
										flagfield[i + 3][j] = true;
										flagfield[i + 4][j] = true;
										flagfield[i + 5][j] = true;
									}
									else if (i + 6 < 19 && field[i + 5][j] != 0 && add < 10)
									{
										if ((add += field[i + 6][j]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i + 1][j] = true;
											flagfield[i + 2][j] = true;
											flagfield[i + 3][j] = true;
											flagfield[i + 4][j] = true;
											flagfield[i + 5][j] = true;
											flagfield[i + 6][j] = true;
										}
										else if (i + 7 < 19 && field[i + 6][j] != 0 && add < 10)
										{
											if ((add += field[i + 7][j]) == 10)
											{
												flag = 1;
												flagfield[i][j] = true;
												flagfield[i + 1][j] = true;
												flagfield[i + 2][j] = true;
												flagfield[i + 3][j] = true;
												flagfield[i + 4][j] = true;
												flagfield[i + 5][j] = true;
												flagfield[i + 6][j] = true;
												flagfield[i + 7][j] = true;
											}
											else if (i + 8 < 19 && field[i + 7][j] != 0 && add < 10)
											{
												if ((add += field[i + 8][j]) == 10)
												{
													flag = 1;
													flagfield[i][j] = true;
													flagfield[i + 1][j] = true;
													flagfield[i + 2][j] = true;
													flagfield[i + 3][j] = true;
													flagfield[i + 4][j] = true;
													flagfield[i + 5][j] = true;
													flagfield[i + 6][j] = true;
													flagfield[i + 7][j] = true;
													flagfield[i + 8][j] = true;
												}
												else if (i + 9 < 19 && field[i + 8][j] != 0 && add < 10)
												{
													if ((add += field[i + 9][j]) == 10)
													{
														flag = 1;
														flagfield[i][j] = true;
														flagfield[i + 1][j] = true;
														flagfield[i + 2][j] = true;
														flagfield[i + 3][j] = true;
														flagfield[i + 4][j] = true;
														flagfield[i + 5][j] = true;
														flagfield[i + 6][j] = true;
														flagfield[i + 7][j] = true;
														flagfield[i + 8][j] = true;
														flagfield[i + 9][j] = true;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			//消滅カウント数の計算
			for (int i = 0; i < 19; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (flagfield[i][j] == true)
					{
						cnt++;
						field[i][j] = 0; //ブロックを消す
					}
				}
			}
			//現在の獲得スコアの計算
			if(flag==1)
			{
				score += floor(pow(1.3, chain)) + floor((double)cnt / 2);
			}
			cnt = 0;
		}
		//ゲームオーバー時のスコアは評価しない
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (field[i][j] != 0)
				{
					score = -1;
				}
			}
		}
		return score;
  }

  void main()
  {
	  int field[19][10];
	  int score;
	  cout << "before" << endl;
	  for(int i=0;i<19;i++)
	  {
		  for(int j=0;j<10;j++)
		  {
			  field[i][j]=0;
		  }
	  }
	  for(int i=0;i<19;i++)
	  {
		  for(int j=0;j<10;j++)
		  {
			if(j==0&&i>1)
			{
				field[i][j]=9;
			}else{
				field[i][j]=0;
			}
			cout << field[i][j] << " ";
		  }
		  cout << endl;
	  }
	  score = DelBrock(field);
	  cout << "after" << endl;
	  for(int i=0;i<19;i++)
	  {
		  for(int j=0;j<10;j++)
		  {
			  cout << field[i][j] << " ";
		  }
		  cout << endl;
	  }
	  cout << score << endl;
	  return;
  }