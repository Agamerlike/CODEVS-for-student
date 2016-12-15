/**
 * コンパイル： g++ -std=c++11 Main.cpp
 */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
using namespace std;

class Field;
class Pack;
class Beam;
class State;

/** ブロックが置かれていない場所の値 */
static const int EMPTY_BLOCK = 0;

/** お邪魔ブロックの値 */
int OBSTACLE_BLOCK = -1;

class Field {
public:
  int W, H;
  vector<vector<int>> blocks;

  Field() {}
  Field(int W, int H):
    W(W),
    H(H),
    blocks(vector<vector<int>>(H, vector<int>(W, 0))) {}

  /**
   * フィールド1つ分を入力します。
   */
  void input() {
    blocks.clear();
    for (int i = 0; i < H; i++) {
      vector<int> row;
      for (int j = 0; j < W; j++) {
        int block;
        cin >> block;
        row.push_back(block);
      }
      blocks.push_back(row);
    }
    string endStr;
    cin >> endStr;
  }

  /**
   * フィールドの状態を標準エラー出力します。
   */
  void show() {
    for (int i = 0; i < H; i++) {
      for (int j = 0; j < W; j++) {
        cerr << blocks[i][j] << " ";
      }
      cerr << endl;
    }
    cerr << "====" << endl;
    cerr.flush();
  }
};

class Pack {
public:
  int T;
  vector<vector<int>> blocks;
  int rotateCnt;

  Pack(int T, vector<vector<int>> blocks):
    T(T),
    blocks(blocks),
    rotateCnt(0) {}

  /**
   * T*Tのサイズのパックを1つ入力します。Packを生成するときは必ず呼び出してください。
   */
  static Pack inputFirst(int T) {
    vector<vector<int>> blocks;
    for (int i = 0; i < T; i++) {
      vector<int> row;
      for (int j = 0; j < T; j++) {
        int block;
        cin >> block;
        row.push_back(block);
      }
      blocks.push_back(row);
    }
    string endStr;
    cin >> endStr;
    return Pack(T, blocks);
  }

  /**
   * お邪魔ブロックでパックを埋めます。回転後の状態で呼び出さないでください。
   */
  int fillWithObstacle(int obstacleCnt) {
    int fillCnt = 0;
    for (int i = 0; i < T; i++) {
      for (int j = 0; j < T; j++) {
        if (fillCnt < obstacleCnt && blocks[i][j] == EMPTY_BLOCK) {
          blocks[i][j] = OBSTACLE_BLOCK;
          fillCnt++;
        }
        if (fillCnt >= obstacleCnt) {
          return fillCnt;
        }
      }
    }
    return fillCnt;
  }

  /**
   * このパックを90*rot度、回転させます。
   */
  void rotate(int rot) {
    rotateCnt += rot;
    rotateCnt %= 4;

    for (int r = 0; r < rot; r++) {
      vector<vector<int>> tmp = blocks;
      for (int i = 0; i < T; i++) {
        for (int j = 0; j < T; j++) {
          blocks[j][T - 1 - i] = tmp[i][j];
        }
      }
    }
  }

  /**
   * このパックの両端を削れるだけ削った、本来の両端のインデックスをpairで返します。
   * firstが左端、secondが右端のインデックス。
   */
  pair<int,int> getSides() {
    int minSide = T;
    int maxSide = 0;
    for (int i = 0; i < T; i++) {
      for (int j = 0; j < T; j++) {
        if (blocks[i][j] != EMPTY_BLOCK) {
          minSide = min(minSide, j);
          maxSide = max(maxSide, j);
        }
      }
    }
    return pair<int,int>(minSide, maxSide);
  }

  /**
   * このパックの状態を標準エラー出力します
   */
  void show() {
    for (int i = 0; i < T; i++) {
      for (int j = 0; j < T; j++) {
        cerr << blocks[i][j] << " ";
      }
      cerr << endl;
    }
    cerr.flush();
  }
};

/*
*ビームサーチの状態を格納するクラス
*盤面=状態
*/
class Beam
{
private:
public:
	int touka; //投下位置
	int prev_state;//直前にどの状態から遷移したか
	int rotate; //回転数
	int hyoka; //評価関数の値
	pair<int, int> hakka; //発火に必要な数字と投下位置(W)
	int hakkaturn; //発火までに最低かかるターン
	int state[19][10]; //盤面
	/*
	*コンストラクタ
	*/
	Beam(int field[][10],int t, int r,int prev)
	{	
		for(int i = 0; i < 19; i++)
		{
			for(int j = 0; j < 10; j++)
			{
				state[i][j] = field[i][j];
			}
		}
		touka = t;
		rotate = r;
		prev_state = prev;
	}

	/*直前の状態から移行
	*この状態が保存している手を出力する
	*/
	pair<int, int> getExecute()
	{
		return pair<int, int>(touka, rotate);
	}

	/*与えられたフィールドに対してブロックを消すシミュレーションを行う関数（連鎖込み）
	**得られたスコア(first)とチェイン数(second)を返す
	*/
	pair<int,int> DelBrock(int field[][10])
	{
		int flag = 1;
		int score = 0;//獲得スコア
		int add = 0;//現在までの加算結果
		int chain = 0;//チェイン数
		int cnt = 0;//消滅カウント
		int h = 0;
		bool flagfield[19][10];//消すブロックを管理するためのフィールド
		while(flag == 1)
		{
			flag = 0;
			for(int i = 0; i<19; i++)
			{
				for(int j = 0; j<10; j++)
				{
					flagfield[i][j] = false;
				}
			}
			//ブロックを落とす
			for(int i = 18; i > -1; i--)
			{
				for(int j = 0; j < 10; j++)
				{
					if(field[i][j] == 0)
					{
						h = i;
						while(h>-1 && field[h][j] == 0)
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
			//消すことをシミュレート
			for(int i = 0; i < 19; i++)
			{
				for(int j = 0; j < 10; j++)
				{
					//上下左右斜めについて調べる
					//左
					if(j - 1 > -1)
					{
						//左上
						if(i - 1 > -1 && (add = field[i - 1][j - 1] + field[i][j]) == 10)
						{
							flag = 1;
							flagfield[i][j] = true;
							flagfield[i - 1][j - 1] = true;
						} else if(j - 2 > -1 && i - 2 > -1 && field[i - 1][j - 1] != 0 && add < 10)
						{
							if((add += field[i - 2][j - 2]) == 10)
							{
								flag = 1;
								flagfield[i][j] = true;
								flagfield[i - 1][j - 1] = true;
								flagfield[i - 2][j - 2] = true;
							} else if(j - 3 > -1 && i - 3 > -1 && field[i - 2][j - 2] != 0 && add < 10)
							{
								if((add += field[i - 3][j - 3]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i - 1][j - 1] = true;
									flagfield[i - 2][j - 2] = true;
									flagfield[i - 3][j - 3] = true;
								} else if(j - 4 > -1 && i - 4 > -1 && field[i - 3][j - 3] != 0 && add < 10)
								{
									if((add += field[i - 4][j - 4]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i - 1][j - 1] = true;
										flagfield[i - 2][j - 2] = true;
										flagfield[i - 3][j - 3] = true;
										flagfield[i - 4][j - 4] = true;
									} else if(j - 5 > -1 && i - 5 > -1 && field[i - 4][j - 4] != 0 && add < 10)
									{
										if((add += field[i - 5][j - 5]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i - 1][j - 1] = true;
											flagfield[i - 2][j - 2] = true;
											flagfield[i - 3][j - 3] = true;
											flagfield[i - 4][j - 4] = true;
											flagfield[i - 5][j - 5] = true;
										} else if(j - 6 > -1 && i - 6 > -1 && field[i - 5][j - 5] != 0 && add < 10)
										{
											if((add += field[i - 6][j - 6]) == 10)
											{
												flag = 1;
												flagfield[i][j] = true;
												flagfield[i - 1][j - 1] = true;
												flagfield[i - 2][j - 2] = true;
												flagfield[i - 3][j - 3] = true;
												flagfield[i - 4][j - 4] = true;
												flagfield[i - 5][j - 5] = true;
												flagfield[i - 6][j - 6] = true;
											} else if(j - 7 > -1 && i - 7 > -1 && field[i - 6][j - 6] != 0 && add < 10)
											{
												if((add += field[i - 7][j - 7]) == 10)
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
												} else if(j - 8 > -1 && i - 8 > -1 && field[i - 7][j - 7] != 0 && add < 10)
												{
													if((add += field[i - 8][j - 8]) == 10)
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
													} else if(j - 9 > -1 && i - 9 > -1 && field[i - 8][j - 8] != 0 && add < 10)
													{
														if((add += field[i - 9][j - 9]) == 10)
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
						if((add = field[i][j - 1] + field[i][j]) == 10)
						{
							flag = 1;
							flagfield[i][j - 1] = true;
							flagfield[i][j] = true;
						} else if(j - 2 > -1 && field[i][j - 1] != 0 && add < 10) {
							if((add += field[i][j - 2]) == 10)
							{
								flag = 1;
								flagfield[i][j] = true;
								flagfield[i][j - 1] = true;
								flagfield[i][j - 2] = true;
							} else if(j - 3 > -1 && field[i][j - 2] != 0 && add < 10) {
								if((add += field[i][j - 3]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i][j - 1] = true;
									flagfield[i][j - 2] = true;
									flagfield[i][j - 3] = true;
								} else if(j - 4 > -1 && field[i][j - 3] != 0 && add < 10) {
									if((add += field[i][j - 4]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i][j - 1] = true;
										flagfield[i][j - 2] = true;
										flagfield[i][j - 3] = true;
										flagfield[i][j - 4] = true;
									} else if(j - 5 > -1 && field[i][j - 4] != 0 && add < 10) {
										if((add += field[i][j - 5]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i][j - 1] = true;
											flagfield[i][j - 2] = true;
											flagfield[i][j - 3] = true;
											flagfield[i][j - 4] = true;
											flagfield[i][j - 5] = true;
										} else if(j - 6 > -1 && field[i][j - 5] != 0 && add < 10) {
											if((add += field[i][j - 6]) == 10)
											{
												flag = 1;
												flagfield[i][j] = true;
												flagfield[i][j - 1] = true;
												flagfield[i][j - 2] = true;
												flagfield[i][j - 3] = true;
												flagfield[i][j - 4] = true;
												flagfield[i][j - 5] = true;
												flagfield[i][j - 6] = true;
											} else if(j - 7 > -1 && field[i][j - 6] != 0 && add < 10) {
												if((add += field[i][j - 7]) == 10)
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
												} else if(j - 8 > -1 && field[i][j - 7] != 0 && add < 10) {
													if((add += field[i][j - 8]) == 10)
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
													} else if(j - 9 > -1 && field[i][j - 8] != 0 && add < 10) {
														if((add += field[i][j - 9]) == 10)
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
						if(i + 1 < 19 && (add = field[i + 1][j - 1] + field[i][j]) == 10)
						{
							flag = 1;
							flagfield[i + 1][j - 1] = true;
							flagfield[i][j] = true;
						} else if(j - 2 > -1 && i + 2 < 19 && field[i + 1][j - 1] != 0 && add < 10) {
							if((add += field[i + 2][j - 2]) == 10)
							{
								flag = 1;
								flagfield[i][j] = true;
								flagfield[i + 1][j - 1] = true;
								flagfield[i + 2][j - 2] = true;
							} else if(j - 3 > -1 && i + 3 < 19 && field[i + 2][j - 2] != 0 && add < 10) {
								if((add += field[i + 3][j - 3]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i + 1][j - 1] = true;
									flagfield[i + 2][j - 2] = true;
									flagfield[i + 3][j - 3] = true;
								} else if(j - 4 > -1 && i + 4 < 19 && field[i + 3][j - 3] != 0 && add < 10) {
									if((add += field[i + 4][j - 4]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i + 1][j - 1] = true;
										flagfield[i + 2][j - 2] = true;
										flagfield[i + 3][j - 3] = true;
										flagfield[i + 4][j - 4] = true;
									} else if(j - 5 > -1 && i + 5 < 19 && field[i + 4][j - 4] != 0 && add < 10) {
										if((add += field[i + 5][j - 5]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i + 1][j - 1] = true;
											flagfield[i + 2][j - 2] = true;
											flagfield[i + 3][j - 3] = true;
											flagfield[i + 4][j - 4] = true;
											flagfield[i + 5][j - 5] = true;
										} else if(j - 6 > -1 && i + 6 < 19 && field[i + 5][j - 5] != 0 && add < 10) {
											if((add += field[i + 6][j - 6]) == 10)
											{
												flag = 1;
												flagfield[i][j] = true;
												flagfield[i + 1][j - 1] = true;
												flagfield[i + 2][j - 2] = true;
												flagfield[i + 3][j - 3] = true;
												flagfield[i + 4][j - 4] = true;
												flagfield[i + 5][j - 5] = true;
												flagfield[i + 6][j - 6] = true;
											} else if(j - 7 > -1 && i + 7 < 19 && field[i + 6][j - 6] != 0 && add < 10) {
												if((add += field[i + 7][j - 7]) == 10)
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
												} else if(j - 8 > -1 && i + 8 < 19 && field[i + 7][j - 7] != 0 && add < 10) {
													if((add += field[i + 8][j - 8]) == 10)
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
													} else if(j - 9 > -1 && i + 9 < 19 && field[i + 8][j - 8] != 0 && add < 10) {
														if((add += field[i + 9][j - 9]) == 10)
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
					if(i - 1 > -1 && (add = field[i - 1][j] + field[i][j]) == 10)
					{
						flag = 1;
						flagfield[i - 1][j] = true;
						flagfield[i][j] = true;
					} else if(i - 2 > -1 && field[i - 1][j] != 0 && add < 10) {
						if((add += field[i - 2][j]) == 10)
						{
							flag = 1;
							flagfield[i][j] = true;
							flagfield[i - 1][j] = true;
							flagfield[i - 2][j] = true;
						} else if(i - 3 > -1 && field[i - 2][j] != 0 && add < 10) {
							if((add += field[i - 3][j]) == 10)
							{
								flag = 1;
								flagfield[i][j] = true;
								flagfield[i - 1][j] = true;
								flagfield[i - 2][j] = true;
								flagfield[i - 3][j] = true;
							} else if(i - 4 > -1 && field[i - 3][j] != 0 && add < 10) {
								if((add += field[i - 4][j]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i - 1][j] = true;
									flagfield[i - 2][j] = true;
									flagfield[i - 3][j] = true;
									flagfield[i - 4][j] = true;
								} else if(i - 5 > -1 && field[i - 4][j] != 0 && add < 10) {
									if((add += field[i - 5][j]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i - 1][j] = true;
										flagfield[i - 2][j] = true;
										flagfield[i - 3][j] = true;
										flagfield[i - 4][j] = true;
										flagfield[i - 5][j] = true;
									} else if(i - 6 > -1 && field[i - 5][j] != 0 && add < 10) {
										if((add += field[i - 6][j]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i - 1][j] = true;
											flagfield[i - 2][j] = true;
											flagfield[i - 3][j] = true;
											flagfield[i - 4][j] = true;
											flagfield[i - 5][j] = true;
											flagfield[i - 6][j] = true;
										} else if(i - 7 > -1 && field[i - 6][j] != 0 && add < 10) {
											if((add += field[i - 7][j]) == 10)
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
											} else if(i - 8 > -1 && field[i - 7][j] != 0 && add < 10) {
												if((add += field[i - 8][j]) == 10)
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
												} else if(i - 9 > -1 && field[i - 8][j] != 0 && add < 10) {
													if((add += field[i - 9][j]) == 10)
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
					if(j + 1 < 10)
					{
						//右上
						if(i - 1 > -1 && (add = field[i - 1][j + 1] + field[i][j]) == 10)
						{
							flag = 1;
							flagfield[i - 1][j + 1] = true;
							flagfield[i][j] = true;
						} else if(j + 2 < 10 && i - 2 > -1 && field[i - 1][j + 1] != 0 && add < 10) {
							if((add += field[i - 2][j + 2]) == 10)
							{
								flag = 1;
								flagfield[i][j] = true;
								flagfield[i - 1][j + 1] = true;
								flagfield[i - 2][j + 2] = true;
							} else if(j + 3 < 10 && i - 3 > -1 && field[i - 2][j + 2] != 0 && add < 10) {
								if((add += field[i - 3][j + 3]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i - 1][j + 1] = true;
									flagfield[i - 2][j + 2] = true;
									flagfield[i - 3][j + 3] = true;
								} else if(j + 4 < 10 && i - 4 > -1 && field[i - 3][j + 3] != 0 && add < 10) {
									if((add += field[i - 4][j + 4]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i - 1][j + 1] = true;
										flagfield[i - 2][j + 2] = true;
										flagfield[i - 3][j + 3] = true;
										flagfield[i - 4][j + 4] = true;
									} else if(j + 5 < 10 && i - 5 > -1 && field[i - 4][j + 4] != 0 && add < 10) {
										if((add += field[i - 5][j + 5]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i - 1][j + 1] = true;
											flagfield[i - 2][j + 2] = true;
											flagfield[i - 3][j + 3] = true;
											flagfield[i - 4][j + 4] = true;
											flagfield[i - 5][j + 5] = true;
										} else if(j + 6 < 10 && i - 6 > -1 && field[i - 5][j + 5] != 0 && add < 10) {
											if((add += field[i - 6][j + 6]) == 10)
											{
												flag = 1;
												flagfield[i][j] = true;
												flagfield[i - 1][j + 1] = true;
												flagfield[i - 2][j + 2] = true;
												flagfield[i - 3][j + 3] = true;
												flagfield[i - 4][j + 4] = true;
												flagfield[i - 5][j + 5] = true;
												flagfield[i - 6][j + 6] = true;
											} else if(j + 7 < 10 && i - 7 > -1 && field[i - 6][j + 6] != 0 && add < 10) {
												if((add += field[i - 7][j + 7]) == 10)
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
												} else if(j + 8 < 10 && i - 8 > -1 && field[i - 7][j + 7] != 0 && add < 10) {
													if((add += field[i - 8][j + 8]) == 10)
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
													} else if(j + 9 < 10 && i - 9 > -1 && field[i - 8][j + 8] != 0 && add < 10) {
														if((add += field[i - 9][j + 9]) == 10)
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
						if((add = field[i][j + 1] + field[i][j]) == 10)
						{
							flag = 1;
							flagfield[i][j + 1] = true;
							flagfield[i][j] = true;
						} else if(j + 2 < 10 && field[i][j + 1] != 0 && add < 10)
						{
							if((add += field[i][j + 2]) == 10)
							{
								flag = 1;
								flagfield[i][j] = true;
								flagfield[i][j + 1] = true;
								flagfield[i][j + 2] = true;
							} else if(j + 3 < 10 && field[i][j + 2] != 0 && add < 10)
							{
								if((add += field[i][j + 3]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i][j + 1] = true;
									flagfield[i][j + 2] = true;
									flagfield[i][j + 3] = true;
								} else if(j + 4 < 10 && field[i][j + 3] != 0 && add < 10)
								{
									if((add += field[i][j + 4]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i][j + 1] = true;
										flagfield[i][j + 2] = true;
										flagfield[i][j + 3] = true;
										flagfield[i][j + 4] = true;
									} else if(j + 5 < 10 && field[i][j + 4] != 0 && add < 10)
									{
										if((add += field[i][j + 5]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i][j + 1] = true;
											flagfield[i][j + 2] = true;
											flagfield[i][j + 3] = true;
											flagfield[i][j + 4] = true;
											flagfield[i][j + 5] = true;
										} else if(j + 6 < 10 && field[i][j + 5] != 0 && add < 10)
										{
											if((add += field[i][j + 6]) == 10)
											{
												flag = 1;
												flagfield[i][j] = true;
												flagfield[i][j + 1] = true;
												flagfield[i][j + 2] = true;
												flagfield[i][j + 3] = true;
												flagfield[i][j + 4] = true;
												flagfield[i][j + 5] = true;
												flagfield[i][j + 6] = true;
											} else if(j + 7 < 10 && field[i][j + 6] != 0 && add < 10)
											{
												if((add += field[i][j + 7]) == 10)
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
												} else if(j + 8 < 10 && field[i][j + 7] != 0 && add < 10)
												{
													if((add += field[i][j + 8]) == 10)
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
													} else if(j + 9 < 10 && field[i][j + 8] != 0 && add < 10)
													{
														if((add += field[i][j + 9]) == 10)
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
						if(i + 1 < 19 && (add = field[i + 1][j + 1] + field[i][j]) == 10)
						{
							flag = 1;
							flagfield[i + 1][j + 1] = true;
							flagfield[i][j] = true;
						} else if(i + 2 < 19 && j + 2 < 10 && field[i + 1][j + 1] != 0 && add < 10)
						{
							if((add += field[i + 2][j + 2]) == 10)
							{
								flag = 1;
								flagfield[i][j] = true;
								flagfield[i + 1][j + 1] = true;
								flagfield[i + 2][j + 2] = true;
							} else if(i + 3 < 19 && j + 3 < 10 && field[i + 2][j + 2] != 0 && add < 10)
							{
								if((add += field[i + 3][j + 3]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i + 1][j + 1] = true;
									flagfield[i + 2][j + 2] = true;
									flagfield[i + 3][j + 3] = true;
								} else if(i + 4 < 19 && j + 4 < 10 && field[i + 3][j + 3] != 0 && add < 10)
								{
									if((add += field[i + 4][j + 4]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i + 1][j + 1] = true;
										flagfield[i + 2][j + 2] = true;
										flagfield[i + 3][j + 3] = true;
										flagfield[i + 4][j + 4] = true;
									} else if(i + 5 < 19 && j + 5 < 10 && field[i + 4][j + 4] != 0 && add < 10)
									{
										if((add += field[i + 5][j + 5]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i + 1][j + 1] = true;
											flagfield[i + 2][j + 2] = true;
											flagfield[i + 3][j + 3] = true;
											flagfield[i + 4][j + 4] = true;
											flagfield[i + 5][j + 5] = true;
										} else if(i + 6 < 19 && j + 6 < 10 && field[i + 5][j + 5] != 0 && add < 10)
										{
											if((add += field[i + 6][j + 6]) == 10)
											{
												flag = 1;
												flagfield[i][j] = true;
												flagfield[i + 1][j + 1] = true;
												flagfield[i + 2][j + 2] = true;
												flagfield[i + 3][j + 3] = true;
												flagfield[i + 4][j + 4] = true;
												flagfield[i + 5][j + 5] = true;
												flagfield[i + 6][j + 6] = true;
											} else if(i + 7 < 19 && j + 7 < 10 && field[i + 6][j + 6] != 0 && add < 10)
											{
												if((add += field[i + 7][j + 7]) == 10)
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
												} else if(i + 8 < 19 && j + 8 < 10 && field[i + 7][j + 7] != 0 && add < 10)
												{
													if((add += field[i + 8][j + 8]) == 10)
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
													} else if(i + 9 < 19 && j + 9 < 10 && field[i + 8][j + 8] != 0 && add < 10)
													{
														if((add += field[i + 9][j + 9]) == 10)
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
					if(i + 1 < 19 && (add = field[i + 1][j] + field[i][j]) == 10)
					{
						flag = 1;
						flagfield[i + 1][j] = true;
						flagfield[i][j] = true;
					} else if(i + 2 < 19 && field[i + 1][j] != 0 && add < 10)
					{
						if((add += field[i + 2][j]) == 10)
						{
							flag = 1;
							flagfield[i][j] = true;
							flagfield[i + 1][j] = true;
							flagfield[i + 2][j] = true;
						} else if(i + 3 < 19 && field[i + 2][j] != 0 && add < 10)
						{
							if((add += field[i + 3][j]) == 10)
							{
								flag = 1;
								flagfield[i][j] = true;
								flagfield[i + 1][j] = true;
								flagfield[i + 2][j] = true;
								flagfield[i + 3][j] = true;
							} else if(i + 4 < 19 && field[i + 3][j] != 0 && add < 10)
							{
								if((add += field[i + 4][j]) == 10)
								{
									flag = 1;
									flagfield[i][j] = true;
									flagfield[i + 1][j] = true;
									flagfield[i + 2][j] = true;
									flagfield[i + 3][j] = true;
									flagfield[i + 4][j] = true;
								} else if(i + 5 < 19 && field[i + 4][j] != 0 && add < 10)
								{
									if((add += field[i + 5][j]) == 10)
									{
										flag = 1;
										flagfield[i][j] = true;
										flagfield[i + 1][j] = true;
										flagfield[i + 2][j] = true;
										flagfield[i + 3][j] = true;
										flagfield[i + 4][j] = true;
										flagfield[i + 5][j] = true;
									} else if(i + 6 < 19 && field[i + 5][j] != 0 && add < 10)
									{
										if((add += field[i + 6][j]) == 10)
										{
											flag = 1;
											flagfield[i][j] = true;
											flagfield[i + 1][j] = true;
											flagfield[i + 2][j] = true;
											flagfield[i + 3][j] = true;
											flagfield[i + 4][j] = true;
											flagfield[i + 5][j] = true;
											flagfield[i + 6][j] = true;
										} else if(i + 7 < 19 && field[i + 6][j] != 0 && add < 10)
										{
											if((add += field[i + 7][j]) == 10)
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
											} else if(i + 8 < 19 && field[i + 7][j] != 0 && add < 10)
											{
												if((add += field[i + 8][j]) == 10)
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
												} else if(i + 9 < 19 && field[i + 8][j] != 0 && add < 10)
												{
													if((add += field[i + 9][j]) == 10)
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
			for(int i = 0; i < 19; i++)
			{
				for(int j = 0; j < 10; j++)
				{
					if(flagfield[i][j] == true)
					{
						cnt++;
						field[i][j] = 0; //ブロックを消す
					}
				}
			}
			//現在の獲得スコアの計算
			if(flag == 1)
			{
				chain++;
				score += floor(pow(1.3, chain)) + floor((double)cnt / 2);
			}
			cnt = 0;
		}
		//ゲームオーバー時のスコアは評価しない
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 10; j++)
			{
				if(field[i][j] != 0)
				{
					score = -1;
				}
			}
		}
		return pair<int, int>(score, chain);
	}

	/**
	*フィールドにブロックを落とすことをシミュレートします
	*field,投下位置(W基準),相対ターン
	*/
	pair<int, int> BoardSim(int field[][10], int touka, Pack pack)
	{
		int h = 0;
		pair<int, int> score;
		//消せるかどうか確認
		//落とすことをシミュレート
		for(int k = 0; k < 3; k++)
		{
			for(int l = 2; l > -1; l--)
			{
				if(pack.blocks[k][l] != 0)
				{
					while(field[h][touka + k - 2] == 0 && h < 19)
					{
						field[h][touka + k - 2] = pack.blocks[k][l];
						h++;
					}
				}
				h = 0;
			}
		}
		//消すことをシミュレート
		score = DelBrock(field);
		return score;
	}

	/*
	*評価関数から評価値を求めるための関数
	*評価値=発火時のスコア(-発火後に残ったゴミ*連鎖数)
	*/
	void RensaSim(int field[][10],Pack pack)
	{
		int Simfield[19][10];
		hyoka = -10000; //直前のスコア
		int temp; //評価関数の計算
		pair <int, int> score = pair<int, int>(0, 0); //スコア
		pair<int, int>sides;
		int cnt = 0; //ゴミを数える
		//開始時点でゲームオーバーだったら評価しない
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 10; j++)
			{
				if(field[i][j] != 0)
				{
					return;
				}
			}
		}
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 12; j++)
			{
				//盤面の初期化
				temp = 0;
				for(int k = 0; k < 19; k++)
				{
					for(int l = 0; l < 10; l++)
					{
						Simfield[i][j] = field[i][j];
					}
				}
				sides = pack.getSides();
				if(j - 2 < -sides.first || j - 2 > 10 - sides.second - 1)
				{
					continue;
				}
				score = BoardSim(Simfield, j, pack);
				temp += score.first * score.second; //連鎖数*スコア
				//重み付け
				for(int k = 0; k < 19; k++)
				{
					for(int l = 0; l < 10; l++)
					{
						if(Simfield[k][l] > 0)
						{
							temp -= score.second;
						}
					}
				}
				if(temp > hyoka)
				{
					hyoka = temp;
					hakka = pair<int, int>(i, j - 2);
				}
			}
			pack.rotate(1);
		}
		return;
	}

};

class State {
public:
  int W, H, T, S, N;
  int turn;
	int first;
	int keika;
  int remTime;
	int beamres[51];//どのステートを利用するか（0は使用しない）
  vector<Pack> packs;
	vector<Beam> states[51]; //探索最大ターン：50
	//Beam init_state; //初期状態
	//Beam next_state; //次の状態
  Field myField;
  Field enemyField;
  int myObstacle;
  int enemyObstacle;
	
  State() {}
  /**
   *フィールドにブロックを落とすことをシミュレートします
	 *field,投下位置(W基準),相対ターン
   */
   int BoardSim(int field[][10],int touka,Pack pack)
  {
		int h = 0;
    //消せるかどうか確認
    //落とすことをシミュレート
		for (int k = 0; k < 3; k++)
		{
			for (int l = 2; l > -1; l--)
			{
				if (pack.blocks[k][l] != 0)
				{
					while (field[h][touka + k - 2] == 0 && h < 19)
					{
						field[h][touka + k - 2] = pack.blocks[k][l];
						h++;
					}
				}
				h = 0;
			}
		}
    //消すことをシミュレート
    return DelBrock(field);
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
		int h = 0;
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
			for (int i = 18; i > -1; i--)
			{
				for (int j = 0; j < 10; j++)
				{
					if (field[i][j] == 0)
					{
						h = i;
						while (h>-1 && field[h][j] == 0)
						{
							h--;
						}
						if (h>-1)
						{
							field[i][j] = field[h][j];
							field[h][j] = 0;
						}
					}
				}
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
			if (flag == 1)
			{
				score += floor(pow(1.3, chain)) + floor((double)cnt / 2);
			}
			cnt = 0;
		}
		//ゲームオーバー時のスコアは評価しない
		for (int i = 0; i < 4; i++)
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
	/*
	 *ビームサーチを行う
	 *ビームサーチを行ったターンを返す
	*/
	int Beam_Search()
	{
		cerr << "Beam_Search() begin" << endl;
		int field[19][10];
		int flag = 0;
		int beam_WIDTH = 70; //ビーム幅
		pair<int, int> sides;
		//フィールドの初期化
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 10; j++)
			{
				field[i][j] = 0;
			}
		}
		for(int i = 0; i < 16; i++)
		{
			for(int j = 0; j < 10; j++)
			{
				field[i + 3][j] = myField.blocks[i][j];
			}
		}
		//ここからビームサーチ
		//評価関数：発火時のスコア-発火後残ったゴミ*連鎖数
		Beam init_state = Beam(field, 0, 0, 0);
		states[0].push_back(init_state);
		for(int depth = 1; depth < 51; depth++)
		{
			cerr << "depth" << depth << endl;
			//cerr << states[depth - 1].size() << endl;
			//探索開始(depth)
			//直前の状態を写し取る
			for(int cnt = 0; cnt < states[depth - 1].size(); cnt++)
			{
				for(int i = 0; i < 4; i++)
				{
					for(int j = 0; j < 12; j++)
					{
						//盤面の初期化
						for(int k = 0; k < 19; k++)
						{
							for(int l = 0; l < 10; l++)
							{
								field[i][j] = states[depth - 1][cnt].state[i][j];
							}
						}
						myObstacle -= packs[turn + depth - 1].fillWithObstacle(myObstacle);
						sides = packs[turn + depth - 1].getSides();
						if(j - 2 < -sides.first || j - 2 > W - sides.second - 1)
						{
							continue;
						}
						flag = BoardSim(field, j, packs[turn + depth - 1]);
						/*if(flag == -1)
						{
							continue;
						}*/
						//新しい状態の完成
						Beam next_state = Beam(field, j - 2, i, cnt);
						//評価関数の計算
						if(flag < 10)
						{
							next_state.RensaSim(field, packs[turn + depth]);
						} else {
							next_state.hyoka = flag * 200;
						}
						states[depth].push_back(next_state);
					}
					packs[turn + depth - 1].rotate(1);
				}
			}
			//探索終了
			//評価値でソート
			sort(states[depth].begin(), states[depth].end(), [](const Beam &x, const Beam &y) {return x.hyoka > y.hyoka; });
			//上位評価値に入らないものを削除
			if(states[depth].size() > beam_WIDTH)
			{
				states[depth].erase(states[depth].begin() + beam_WIDTH, states[depth].end());
			}
			//ゲームオーバーになっているものを削除
			/*
			flag = 0;
			while(states[depth][flag].hyoka != -10000)
			{
				flag++;
			}
			states[depth].erase(states[depth].begin() + flag, states[depth].end());
			*/
			for(int i = 0; i < states[depth].size(); i++)
			{
				cerr << states[depth][i].hyoka << " ";
			}
			cerr << endl;
		}
		//ここまでビームサーチ
		cerr << "beam search done" << endl;
		return turn;
	}

	/*
	*そのターンに最もスコアを稼げる落とし方を返す（連鎖考慮済み）
	*/
	pair<int, int> execute()
	{
		pair<int, int> sides;
		pair<int, int> heights;
		int h = 0;
		int score[4][12]; //各投下位置に落とした場合の消せる個数（-2する）
		int maxscore = 0;
		int maxscore_rotate = rand() % 8;
		int maxscore_pos = rand() % 4;
		int field[19][10]; //field[H+3][W]
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 12; j++)
			{
				score[i][j] = -1;
			}
		}
		//シミュレートに使うフィールドを作成する
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 10; j++)
			{
				field[i][j] = 0;
			}
		}
		for(int i = 0; i < 16; i++)
		{
			for(int j = 0; j < 10; j++)
			{
				field[i + 3][j] = myField.blocks[i][j];
			}
		}
		//回転→場所の順で調べる
		for(int i = 0; i < 4; i++)
		{
			myObstacle -= packs[turn].fillWithObstacle(myObstacle);
			sides = packs[turn].getSides();
			for(int j = 0; j < 12; j++)
			{
				//落とせるかどうか確認
				if(j - 2 < -sides.first || j - 2 > W - sides.second - 1)
				{
					continue;
				}
				score[i][j] = BoardSim(field, j, packs[turn]);
				//フィールドの初期化
				for(int k = 0; k < 3; k++)
				{
					for(int l = 0; l < 10; l++)
					{
						field[k][l] = 0;
					}
				}
				for(int k = 0; k < 16; k++)
				{
					for(int l = 0; l < 10; l++)
					{
						field[k + 3][l] = myField.blocks[k][l];
					}
				}
			}
			packs[turn].rotate(1);
		}
		//最大を取る位置と回転を求める
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 12; j++)
			{
				if(score[i][j] > maxscore)
				{
					maxscore = score[i][j];
					maxscore_rotate = i;
					maxscore_pos = j - 2;
				}
			}
		}
		return pair<int, int>(maxscore_pos, maxscore_rotate);
	}

  /**
   * Stateを入力します。Stateを生成するときは必ず呼び出してください。
   */
  static State inputFirst() {
    State st;
    cin >> st.W >> st.H >> st.T >> st.S >> st.N;
    st.packs.clear();
    for (int i = 0; i < st.N; i++) {
      Pack pack = Pack::inputFirst(st.T);
      st.packs.push_back(pack);
    }
    st.myField = Field(st.W, st.H);
    st.enemyField = Field(st.W, st.H);
		st.keika = 55;
    return st;
  }

  /**
   * 1ターン分の入力を行います。
   */
  bool inputTurn() {
    if (!(cin >> turn >> remTime)) return false;
    cin >> myObstacle;
    myField.input();
    cin >> enemyObstacle;
    enemyField.input();
    return true;
  }

  /**
   * 現在のターンのパックをどこに落とすか決定して、標準出力します。
   */
	void executeTurn() {
		cerr << "turn" << turn << " begin" << endl;
		pair<int, int> exec;
		/*if (PackSurv() == 2)
		{
			//なるべく早く発火する手順を探す
		} else if(PackSurv() == 1) {
			//ビームサーチを使って新しく連鎖を組み立てる
		} else {
			//ビームサーチを使って手数を探す
		}*/
		//以下普通にビームサーチするだけ（例外処理あり）
		//おじゃまが1個でもあればビームサーチをやり直す
		if(myObstacle > 0 || keika == 51)
		{
			cerr << "ojama detect" << endl;
			exec = states[keika - 1][beamres[keika - 1]].hakka;
			keika = 52;
		} else {
			while(keika > 51)
			{
				first = Beam_Search();
				keika = 1;
				beamres[50] = 0;
				for(int i = 1; i < 50; i++)
				{
					beamres[50 - i] = states[i][beamres[51 - i]].prev_state;
				}
			}
			exec = states[keika][beamres[keika]].getExecute();
		}
		keika++;
		packs[turn].rotate(exec.second);

		cout << exec.first << " " << packs[turn].rotateCnt << endl;
		cout.flush();
	}

};

int main() {
  cout << "MyAI.cpp" << endl;
  cout.flush();

  State state = State::inputFirst();

  OBSTACLE_BLOCK = state.S + 1;

  for (int i = 0; i < state.N; i++) {
    bool inputSuccess = state.inputTurn();

    if (!inputSuccess) {
      cerr << "input Failed" << endl;
      cerr.flush();
      break;
    }
    state.executeTurn();
  }
}
