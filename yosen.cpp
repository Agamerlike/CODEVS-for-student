/**
 * コンパイル： g++ -std=c++11 Main.cpp
 */
 /*
 アルゴリズム方針
 ・ブロックを消すことを最優先（縦＜横＜斜め）
 ・左側を中心にして積み上げる
 ・同じ数字はなるべくそろえる（できればカギ型に揃える）
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
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
  vector<Pack> packs;//vector型でPackを保管
  Field myField;
  Field enemyField;
  int myObstacle;
  int enemyObstacle;

  State() {}

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
   *ボードをシミュレートする
   *得られたスコアを返す
   */
  int BoardSim(int field[][10],int touka,int turns)
  {
		int flag = 1;
		int score = 0;
		int chain = 0;//チェイン数
		int cnt = 0; //消滅カウント
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
		while (flag == 1)
		{
			flag = 0;
			chain++;
			for (int k = 18; k > -1; k--)
			{
				for (int l = 0; l < 10; l++)
				{
					if (field[k][l] == 0 && field[k - 1][l] != 0 && k - 1 > -1)
					{
						field[k][l] = field[k - 1][l];
						field[k - 1][l] = 0;
					}
				}
			}
			//消すことをシミュレート
			for (int k = 0; k < 19; k++)
			{
				for (int l = 0; l < 10; l++)
				{
					//上下左右斜めについて調べる
					//左
					if (l - 1 > -1)
					{
						//左上
						if (k - 1 > -1 && field[k - 1][l - 1] + field[k][l] == 10)
						{
							cnt += 2;
							flag = 1;
							field[k - 1][l - 1] = 0;
							field[k][l] = 0;
						}
						//左
						if (field[k][l - 1] + field[k][l] == 10)
						{
							cnt += 2;
							flag = 1;
							field[k][l - 1] = 0;
							field[k][l] = 0;
						}
						//左下
						if (k + 1 < 19 && field[k + 1][l - 1] + field[k][l] == 10)
						{
							cnt += 2;
							flag = 1;
							field[k + 1][l - 1] = 0;
							field[k][l] = 0;
						}
					}
					//上
					if (k - 1 > -1 && field[k - 1][l] + field[k][l] == 10)
					{
						cnt += 2;
						flag = 1;
						field[k - 1][l] = 0;
						field[k][l] = 0;
					}
					//右
					if (l + 1 < 10)
					{
						//右上
						if (k - 1 > -1 && field[k - 1][l + 1] + field[k][l] == 10)
						{
							cnt += 2;
							flag = 1;
							field[k - 1][l + 1] = 0;
							field[k][l] = 0;
						}
						//右
						if (field[k][l + 1] + field[k][l] == 10)
						{
							cnt += 2;
							flag = 1;
							//cerr << "donef " << k << "," << l << endl;
							field[k][l + 1] = 0;
							field[k][l] = 0;
						}
						//右下
						if (k + 1 < 19 && field[k + 1][l + 1] + field[k][l] == 10)
						{
							cnt += 2;
							flag = 1;
							field[k + 1][l + 1] = 0;
							field[k][l] = 0;
						}
					}
					//下
					if (k + 1 < 19 && field[k + 1][l] + field[k][l] == 10)
					{
						cnt += 2;
						flag = 1;
						field[k + 1][l] = 0;
						field[k][l] = 0;
					}
				}
			}
			//現在の獲得スコアの計算
			score += floor(pow(1.3, chain)) + floor((double)cnt / 2);
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
   * 1番低い場所と1番高い場所を取得する
   * first...minheight
   * second..maxheight
   */
  pair<int,int> HighandLow()
   {
    int height[10]; //ブロックの高さ
    float ave_height[10]; //周辺の高さとの合計
    int minheight=0; //最もブロックが低い位置
    int maxheight=15; //最もブロックが高い位置
    for(int i=0;i<W;i++)
    {
      height[i]=15;
    }
    //ブロックの高さを取得
    for(int i=0;i < W;i++)
    {
      for(int j=0;j < H;j++)
      {
        if(myField.blocks[j][i]>0)
        {
          height[i]=j;
          if(height[i]<2)
          {
            height[i] = -10;
          }
          break;
        }
      }
    }
    //平均的に1番低い場所を取得
    for(int i=0;i<W;i++)
    {
			if (i == 0)
			{
				ave_height[i] = (float)(height[i] + height[i + 1]) / 2;
			}
			else if (i == 9)
			{
				ave_height[i] = (float)(height[i - 1] + height[i]) / 2;
			}
			else {
				ave_height[i] = (float)(height[i - 1] + height[i] + height[i + 1]) / 3;
			}
    }
    for(int i=0;i<W;i++)
    {
      if(ave_height[i] > ave_height[minheight])
      {
        minheight=i;
      }
      if(ave_height[i] < ave_height[maxheight])
      {
        maxheight=i;
      }
    }
    return pair<int,int>(minheight,maxheight);
   }
   /*最も場所を取らないようなブロックの回転のさせ方を返す（ただし向きは考慮しない）
   //最も場所を取るブロックの回転のさせ方も返す
   //first...maxside
   //second..minside（場所取る）
   */
   pair<int,int> size()
   {   
    int maxside=0;
    int minside=0;
    pair<int,int> sides[4];
    for(int i=0;i<4;i++)
    {
      sides[i]=packs[turn].getSides();
      if(W-sides[i].second-1+2*sides[i].first > W-sides[maxside].second-1+2*sides[maxside].first)
      {
        maxside = i;
      }
      if(W-sides[i].second-1+2*sides[i].first < W-sides[minside].second-1+2*sides[minside].first)
      {
        minside = i;
      }
      packs[turn].rotate(1);
    }
    return pair<int,int>(maxside+1,minside+1);
   }

   /**
    *落とす位置を決める
    *1.消せる組み合わせの探索
    *2.最も消せる組み合わせの記憶
    *出力first...pos second...rotate
    *失敗した場合...-1を返す
    */
   pair<int,int> execute() {
     pair<int,int> sides;
     pair<int,int> heights;
     int h=0;
     int score[4][12]; //各投下位置に落とした場合の消せる個数（-2する）
     int maxscore = 0;
     int maxscore_rotate = -1;
     int maxscore_pos = -1;
     int field[19][10]; //field[H+3][W]
     for(int i=0;i<4;i++)
     {
       for(int j=0;j<12;j++)
       {
         score[i][j] = -1;
       }
     }
     //シミュレートに使うフィールドを作成する
     for(int i=0;i<3;i++)
     {
       for(int j=0;j<10;j++)
       {
         field[i][j]=0;
       }
     }
     for(int i=0;i<16;i++)
     {
       for(int j=0;j<10;j++)
       {
         field[i+3][j]=myField.blocks[i][j];
       }
     }
     //回転→場所の順で調べる
     for(int i=0;i<4;i++)
     {
       sides = packs[turn].getSides();
       for(int j=0;j<12;j++)
       {
				 //落とせるかどうか確認
				 if (j - 2 < -sides.first || j - 2 > W - sides.second - 1)
				 {
					 continue;
				 }
				 score[i][j] = BoardSim(field, j, 0);
         //フィールドの初期化
				 for (int k = 0; k < 3; k++)
				 {
					 for (int l = 0; l < 10; l++)
					 {
						 field[k][l] = 0;
					 }
				 }
         for(int k=0;k<16;k++)
         {
					 for (int l = 0; l < 10; l++)
					 {
						 field[k + 3][l] = myField.blocks[k][l];
					 }
         }
       }
       packs[turn].rotate(1);
     }
     //最大を取る位置と回転を求める
     for(int i=0;i<4;i++)
     {
       for(int j=0;j<12;j++)
       {
         if(score[i][j] > maxscore)
         {
           maxscore = score[i][j];
           maxscore_rotate = i;
           maxscore_pos = j-2;
         }
       }
     }
     return pair<int,int>(maxscore_pos,maxscore_rotate);
   }
   /*exectuteでうまくいかなかったときの関数
   **次ターンに最も消せるように配置する
   */
   pair<int,int> exectute2()
   {
     pair<int,int> sides;
     pair<int,int> heights;
     int h=0;
     int score[4][12]; //各投下位置に落とした場合の隣接する数字の個数
		 int nextscore[4][12];
     int maxscore = 0;
     int maxscore_rotate = -1;
     int maxscore_pos = -1;
     int field[19][10]; //field[H][W]
		 int nextfield[19][10];
     for(int i=0;i<4;i++)
     {
       for(int j=0;j<12;j++)
       {
         score[i][j] = -1;
       }
     }
     //シミュレートに使うフィールドを作成する
		 for (int i = 0; i < 3; i++)
		 {
			 for (int j = 0; j < 10; j++)
			 {
				 field[i][j] = 0;
			 }
		 }
     for(int i=0;i<16;i++)
     {
       for(int j=0;j<10;j++)
       {
         field[i+3][j]=myField.blocks[i][j];
       }
     }
     //回転→場所の順で調べる
     for(int i=0;i<4;i++)
     {
       sides = packs[turn].getSides();
       for(int j=0;j<12;j++)
       {
         //落とすことをシミュレート
         //落とせるかどうか確認
         if(j-2 < -sides.first || j-2 > W-sides.second-1)
         {
           continue;
         }
         //消せるかどうか確認
         //落とすことをシミュレート
         for(int k=0;k<3;k++)
         {
           for(int l=2;l>-1;l--)
           {
             if(packs[turn].blocks[k][l]!=0)
             {
               while(field[h][j+k-2]==0)
               {
                 field[h][j+k-2]=packs[turn].blocks[k][l];
                 h++;
               }
             }
             h = 0;
           }
         }
				 for (int k = 0; k < 19; k++)
				 {
					 for (int l = 0; l < 10; l++)
					 {
						 nextfield[k][l] = field[k][l];
					 }
				 }
				 for (int k = 0; k < 4; k++)
				 {
					 sides = packs[turn + 1].getSides();
					 for (int l = 0; l < 12; l++)
					 {
						 //落とせるかどうか確認
						 if(l - 2 < -sides.first || l - 2 > W - sides.second - 1)
						 {
							 continue;
						 }
						 //消すことをシミュレート
						 nextscore[k][l] = BoardSim(field, l, 1);
						 //フィールドの初期化
						 for (int m = 0; m < 19; m++)
						 {
							 for (int n = 0; n < 10; n++)
							 {
								 field[m][n] = nextfield[m][n];
							 }
						 }
					 }
					 packs[turn + 1].rotate(1);
				 }
				 //スコアの評価
				 for (int k = 0; k < 4; k++)
				 {
					 for (int l = 0; l < 12; l++)
					 {
						 if (nextscore[k][l] > maxscore)
						 {
							 maxscore = nextscore[k][l];
						 }
					 }
				 }
				 score[i][j] = maxscore;
				 maxscore = 0;
				/*for(int k=0;k<16;k++)
         {
           for(int l=0;l<10;l++)
           {
             //上下左右斜めについて調べる
             //左
             if(l-1>-1)
             {
               //左上
               if(k-1>-1 && field[k-1][l-1]==field[k][l])
               {
                 score[i][j]+=2;
               }
               //左
               if(field[k][l-1]==field[k][l])
               {
                 score[i][j]+=2;
               }
               //左下
               if(k+1<16 && field[k+1][l-1]==field[k][l])
               {
                 score[i][j]+=2;
               }
             }
             //上
             if(k-1>-1 && field[k-1][l]==field[k][l])
             {
               score[i][j]+=2;
             }
             //右
             if(l+1<10)
             {
               //右上
               if(k-1>-1 && field[k-1][l+1]==field[k][l])
               {
                 score[i][j]+=2;
               }
               //右
               if(field[k][l+1]==field[k][l])
               {
                 score[i][j]+=2;
               }
               //右下
               if(k+1<16 && field[k+1][l+1]==field[k][l])
               {
                 score[i][j]+=2;
               }
             }
             //下
             if(k+1<16 && field[k+1][l]==field[k][l])
             {
               score[i][j]+=2;
             }
           }
         }
         for(int k=j-2;k<j+1;k++)
         {
           if(field[0][k]!=0)
           {
             score[i][j]=-1;
             break;
           }
         }*/
         //フィールドの初期化
         for(int k=0;k<16;k++)
         {
           for(int l=0;l<10;l++)
           {
						 field[k][l] = myField.blocks[k][l];
           }
         }
       }
       packs[turn].rotate(1);
     }
     //最大を取る位置と回転を求める
     for(int i=0;i<4;i++)
     {
       for(int j=0;j<12;j++)
       {
         if(score[i][j] > maxscore)
         {
           maxscore = score[i][j];
           maxscore_rotate = i;
           maxscore_pos = j-2;
         }
       }
     }
		 cerr << maxscore << endl;
     return pair<int,int>(maxscore_pos,maxscore_rotate);
   }

   /*すべての置ける位置・回転について平均的に最も低くなる位置に置く
   **none_exectuteでうまくいかなかったときの関数
   **うまく動作しなかったため凍結
   **/
   pair<int,int> none_execute2()
   {
     pair<int,int> sides;
     pair<int,int> heights;
     int h=0;
     int height[10];
     float score[4][12]; //各投下位置に落とした場合の周辺の平均の高さ
     int maxscore = 0;
     int maxscore_rotate = -1;
     int maxscore_pos = -1;
     int field[16][10]; //field[H][W]
     for(int i=0;i<4;i++)
     {
       for(int j=0;j<12;j++)
       {
         score[i][j] = 17;
       }
     }
     //シミュレートに使うフィールドを作成する
     for(int i=0;i<16;i++)
     {
       for(int j=0;j<10;j++)
       {
         field[i][j]=myField.blocks[i][j];
       }
     }
     //回転→場所の順で調べる
     for(int i=0;i<4;i++)
     {
       sides = packs[turn].getSides();
       for(int j=0;j<12;j++)
       {
         //落とすことをシミュレート
         //落とせるかどうか確認
         if(j-2 < -sides.first || j-2 > W-sides.second-1)
         {
           continue;
         }
         //消せるかどうか確認
         //落とすことをシミュレート
         for(int k=0;k<3;k++)
         {
           for(int l=2;l>-1;l--)
           {
             if(packs[turn].blocks[k][l]!=0)
             {
               while(field[h][j+k-2]==0)
               {
                 field[h][j+k-2]=packs[turn].blocks[k][l];
                 h++;
               }
             }
             h = 0;
           }
         }
         for(int k=0;k<10;k++)
         {
           while(field[h][k]==0)
           {
             h++;
           }
           height[k]=h;
         }
         score[i][j]=height[j-2]+height[j-1]+height[j];
         if(j==0 || j==11)
         {

         }else if(j==1 || j==10){
           score[i][j]/=2;
         }else{
           score[i][j]/=3;
         }
         for(int k=j-2;k<j+1;k++)
         {
           if(field[0][k]!=0)
           {
             score[i][j]=-1;
             break;
           }
         }
         //フィールドの初期化
         for(int k=0;k<16;k++)
         {
           for(int l=0;l<10;l++)
           {
             field[k][l]=myField.blocks[k][l];
           }
         }
       }
       packs[turn].rotate(1);
     }
     //最大を取る位置と回転を求める
     for(int i=0;i<4;i++)
     {
       for(int j=0;j<12;j++)
       {
         if(score[i][j] > maxscore)
         {
           maxscore = score[i][j];
           maxscore_rotate = i;
           maxscore_pos = j-2;
         }
       }
     }
     return pair<int,int>(maxscore_pos,maxscore_rotate);
   }

	 pair<int, int> none_execute()
	 {
		 pair<int, int> sides;
		 pair<int, int> heights;
		 int h = 0;
		 float score[4][12]; //各投下位置に落とした場合の消せる個数（-2する）
		 int height[3];
		 int cnt = 0;
		 int maxscore = 0;
		 int maxscore_rotate = -1;
		 int maxscore_pos = -1;
		 int field[19][10];
		 for (int i = 0; i < 3; i++)
		 {
			 height[3] = -1;
		 }
		 for (int i = 0; i<4; i++)
		 {
			 for (int j = 0; j<12; j++)
			 {
				 score[i][j] = -1;
			 }
		 }
		 //フィールドの初期化
		 for (int i = 0; i < 3; i++)
		 {
			 for (int j = 0; j < 10; j++)
			 {
				 field[i][j] = 0;
			 }
		 }
		 for (int i = 0; i < 16; i++)
		 {
			 for (int j = 0; j < 10; j++)
			 {
				 field[i + 3][j] = myField.blocks[i][j];
			 }
		 }
		 //回転→場所の順で調べる
		 for (int i = 0; i<4; i++)
		 {
			 sides = packs[turn].getSides();
			 for (int j = 0; j<12; j++)
			 {
				 //落とせるかどうか確認
				 if (j - 2 < -sides.first || j - 2 > W - sides.second - 1)
				 {
					 continue;
				 }
				 //落とすことをシミュレート
				 for (int k = 0; k < 3; k++)
				 {
					 for (int l = 2; l > -1; l--)
					 {
						 if (packs[turn].blocks[k][l] != 0)
						 {
							 while (field[h][j + k - 2] == 0 && h < 19)
							 {
								 field[h][j + k - 2] = packs[turn].blocks[k][l];
								 h++;
							 }
						 }
						 h = 0;
					 }
				 }
				 //高さの計算
				 for (int k = 0; k < 3; k++)
				 {
					 if (j + k - 2 > -1 && j + k - 2 < 10)
					 {
						 continue;
					 }
					 while (field[h][j + k - 2] == 0 && h<19)
					 {
						 h++;
					 }
					 score[i][j] += h;
					 cnt++;
					 h = 0;
				 }
				 score[i][j] /= cnt;
				 for (int k = 0; k < 3; k++)
				 {
					 for (int l = 0; l < 10; l++)
					 {
						 if (field[l][l] != 0)
						 {
							 score[i][j] = -1;
							 break;
						 }
					 }
				 }
				 //フィールドの初期化
				 for (int k = 0; k < 3; k++)
				 {
					 for (int l = 0; l < 10; l++)
					 {
						 field[k][l] = 0;
					 }
				 }
				 for (int k = 0; k<16; k++)
				 {
					 for (int l = 0; l < 10; l++)
					 {
						 field[k + 3][l] = myField.blocks[k][l];
					 }
				 }
			 }
			 packs[turn].rotate(1);
		 }
		 //最大を取る位置と回転を求める
		 for (int i = 0; i<4; i++)
		 {
			 for (int j = 0; j<12; j++)
			 {
				 if (score[i][j] > maxscore)
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
   * 現在のターンのパックをどこに落とすか決定して、標準出力します。
   * 落とす位置の決め方
   * - 回転角度はランダムで決める
   * - 落下位置はランダムで決める
   * - ただし、落下位置は、左端・右端に詰められる場合は、それも考慮する(-2, -1, 8, 9にも落とせる場合は落とす)

   * 目標
   * ・ブロックを消すことを最優先（縦＜横＜斜め）
   * ・左側を中心にして積み上げる
   * ・同じ数字はなるべくそろえる（できればカギ型に揃える）
   */
  /*以下ランダムに落とす場合（サンプル）
  /*int rot = randInt(0, 4); //回転角度をランダムにする
    myObstacle -= packs[turn].fillWithObstacle(myObstacle); //myobstacle...入力
    packs[turn].rotate(rot); //パックを回転させる

    pair<int,int> sides = packs[turn].getSides(); //移動できる範囲を取得(-sides.first...左端 W-sides.second-1+sides.first...右端 )
    int packWidth = sides.second - sides.first + 1; //移動できる範囲を計算
    int pos = randInt(0, W - packWidth + 1) - sides.first; //置ける位置からランダムで選ぶ
  //ここまで*/
  void executeTurn() {
    //myField.blocks[H][W]...自分のフィールドの状態
    //enemyField.blocks[H][W]...敵のフィールドの状態
    //packs[turn].block[i][j]...今のターンのブロック（turn+1でネクスト（おじゃまが挟まる場合があるが））
    int pos;
    pair<int,int> heights;
    pair<int,int> side;
    pair<int,int> sides;
		pair<int, int> exec;
    int width;
    cerr << turn << endl;
		exec = execute();
    if(exec.first==-1 || exec.second ==-1 || turn == 0)
    {
      cerr << "done1" <<endl;
      //exec = exectute2();
    }
		/*
    if(exec.first==-1 || exec.second ==-1 || turn == 0)
    {
      cerr << "done2" <<endl;
      //exec = none_execute();
    }
		*/
    if(exec.first==-1 || exec.second == -1 || turn == 0)
    {
      cerr << "done" <<endl;
      heights = HighandLow();
      pos = heights.first;
      side = size();
      packs[turn].rotate(side.second);
      sides = packs[turn].getSides();
      width = W-sides.second-1+2*sides.first; //投下可能な位置の範囲
      pos--;
      if(pos<0)
      {
        pos=0;
      }
      if(pos>7)
      {
        pos=7;
      }
      //posの調整
      //向きのことを考えていなかったので凍結
      /*switch(pos)
      {
        case 0:
          if(field == 11)
          {
            pos = -2;
            break;
          }
        case 1:
          if(field >= 9)
          {
            pos = -1;
          }else{
            pos = 0;
          }
          break;
        case 9:
          if(field == 11)
          {
            pos = 9;
            break;
          }
        case 8:
          if(field >= 9)
          {
            pos = 8;
          }else{
            pos = 7;
          }
          break;
        default:
          pos--;
          break;
      }*/
    }else{
      pos = exec.first;
      packs[turn].rotate(exec.second);
    }
    cerr << pos << " " << packs[turn].rotateCnt << endl;
    cout << pos << " " << packs[turn].rotateCnt << endl; //出力
    cout.flush();
  }
};

int main() {
  cout << "RensakumenAI" << endl;
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
