/**
 * コンパイル： g++ -std=c++11 Main.cpp
 */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
using namespace std;

/** ブロックが置かれていない場所の値 */
static const int EMPTY_BLOCK = 0;

/** お邪魔ブロックの値 */
int OBSTACLE_BLOCK = -1;

mt19937 MT(8410325);

/**
 * from以上、to未満の乱数を返します。
 */
int randInt(int from, int to) {
  uniform_int_distribution<int> rand(from, to - 1);
  return rand(MT);
}

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

class State {
public:
  int W, H, T, S, N;
  int turn;
  int remTime;
  vector<Pack> packs;
  Field myField;
  Field enemyField;
  int myObstacle;
  int enemyObstacle;

  State() {}

  /**
   *フィールドにブロックを落とすことをシミュレートします
   */
   int BoardSim(int field[][10],int touka,int turns)
  {
		int h = 0;
    //消せるかどうか確認
    //落とすことをシミュレート
		for (int k = 0; k < 3; k++)
		{
			for (int l = 2; l > -1; l--)
			{
				if (packs[turn+turns].blocks[k][l] != 0)
				{
					while (field[h][touka + k - 2] == 0 && h < 19)
					{
						field[h][touka + k - 2] = packs[turn+turns].blocks[k][l];
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
						cout << h << endl;
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
    int rot = randInt(0, 4);

    myObstacle -= packs[turn].fillWithObstacle(myObstacle);
    packs[turn].rotate(rot);

    pair<int,int> sides = packs[turn].getSides();
    int packWidth = sides.second - sides.first + 1;
    int pos = randInt(0, W - packWidth + 1) - sides.first;

    cout << pos << " " << packs[turn].rotateCnt << endl;
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
