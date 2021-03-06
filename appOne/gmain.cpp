#define _SAMPLE1
#ifdef _EX

#endif



#ifdef _SAMPLE2
#include"libOne.h"
//外部変数------------------------------------------------------------
//シーケンス
const int INIT = 0;
const int PLAY = 1;
const int OVER = 2;
int GameState = INIT;
//色データ
float Satu = 200;
float Value = 255;
float Alpha = 255;
struct COLOR Color[9] = {
    COLOR(0,Satu,Value,Alpha),
    COLOR(30,Satu,Value,Alpha),
    COLOR(60,Satu,Value,Alpha),
    COLOR(120,Satu,Value,Alpha),
    COLOR(180,Satu,Value,Alpha),
    COLOR(220,Satu,Value,Alpha),
    COLOR(300,Satu,Value,Alpha),
    COLOR(200,44,88,Alpha),
    COLOR(0,0,20,Alpha),
};
//壁と背景の色番号
const int WALL = 7;
const int BACK = 8;
//ステージ（色番号を記憶する）
const int ROWS = 20;
const int COLS = 12;
int Stage[ROWS][COLS];
float Size = 50;
//基準ブロック位置と回転数
int X, Y, R;
//ブロックパターンの位置
int Px[4], Py[4];
//ブロック移動制御
int FallFlag = 0;
int LoopCnt = 0;
//ブロックパターン番号  ※ 兼色番号 ※
int PtnNo;
//ブロックパターンオフセット
int PtnOffsets[7][3][2] = {
    //■□■■...0
    -1,0,   1,0,  2,0,
    //■
    //■□■....1
    -1,-1, -1,0,  1,0,
    //  　■
    //■□■....2
    -1,0,   1,-1, 1,0,
    //■□
    //　■■....3
    -1,0,   0,1,  1,1,
    //　□■
    //■■  ....4
     1,0,   0,1, -1,1,
    //　■
    //■□■....5
    -1,0,   0,-1, 1,0,
    //□■
    //■■......6
     1,0,   0,1,  1,1,
};

//関数----------------------------------------------------------------
void setPtnPosition();

void init() {
    //壁と背景の色番号をStage2次元配列にセット
    for (int y = 0; y < ROWS; y++) {
        Stage[y][0] = Stage[y][COLS - 1] = WALL;
        for (int x = 1; x < COLS-1; x++) {
            Stage[y][x] = BACK;
            if (y == ROWS - 1) {
                Stage[y][x] = WALL;
            }
        }
    }
    //ブロック初期値
    X = 5;
    Y = 1;
    R = 0;
    PtnNo = random() % 7;
    setPtnPosition();
    //次のシーケンス
    GameState = PLAY;
}

void drawStage() {
    angleMode(DEGREES);
    colorMode(HSV);
    strokeWeight(20);
    rectMode(CENTER);
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            int no = Stage[y][x];
            stroke(Color[no]);
            fill(0, 0, 100);
            rect(Size/2+Size * x, Size/2+Size * y, Size/2, Size/2);
        }
    }
}
void setPtnPosition() {
    //基準ブロック位置
    Px[0] = X;
    Py[0] = Y;
    //回転数
    int r = R % 4;
    for (int i = 0; i < 3; i++) {
        //現在のブロックパターンのオフセット値を取得
        int ofsX = PtnOffsets[PtnNo][i][0];
        int ofsY = PtnOffsets[PtnNo][i][1];
        //回転数に応じて９０度ずつ回転させる
        for (int j = 0; j < r; j++) {
            int w = ofsX;
            ofsX = -ofsY;
            ofsY = w;
        }
        //位置決定
        Px[i + 1] = X + ofsX;
        Py[i + 1] = Y + ofsY;
    }
}
void setPtnNoToStage() {
    setPtnPosition();
    for (int i = 0; i < 4; i++) {
        int x = Px[i];
        int y = Py[i];
        Stage[y][x] = PtnNo;
    }
}
void delPtnNoFromStage() {
    for (int i = 0; i < 4; i++) {
        int x = Px[i];
        int y = Py[i];
        Stage[y][x] = BACK;
    }
}
int collision() {
    setPtnPosition();
    int flag = 0;
    for (int i = 0; i < 4; i++) {
        int x = Px[i];
        int y = Py[i];
        if (Stage[y][x] != BACK) {
            flag = 1;
        }
    }
    return flag;
}
void complete() {
    for (int y = 1; y < ROWS - 1; y++) {
        int flag = 1;
        for (int x = 1; x < COLS - 1; x++) {
            if (Stage[y][x] == BACK) {
                flag = 0;
            }
        }
        if (flag) {
            //そろったのでスライド
            for (int upy = y - 1; upy >= 0; upy--) {
                for (int x = 1; x < COLS - 1; x++) {
                    Stage[upy + 1][x] = Stage[upy][x];
                }
            }
        }
    }
}
void play() {
    //現在のパターン番号（色番号）をステージから消す
    delPtnNoFromStage();
    //移動、回転
    int dy = 0, dx = 0, dr = 0;;
    if ((++LoopCnt %= 15) == 0)dy = 1;
    if (isTrigger(KEY_D))dx = 1;
    if (isTrigger(KEY_A))dx = -1;
    if (isTrigger(KEY_W))dr = 1;
    if (isTrigger(KEY_S))FallFlag = 1;
    if (FallFlag)dy = 1;
    Y += dy;
    X += dx;
    R += dr;
    if (collision()) {
        //元の位置、回転に戻す
        Y -= dy;
        X -= dx;
        R -= dr;
        FallFlag = 0;
        if (dy == 1 && dx == 0 && dr == 0) {
            //積もらせる
            setPtnNoToStage();
            //行がそろっていたらスライドさせる
            complete();
            //新しいブロック誕生
            X = 5;
            Y = 1;
            R = 0;
            PtnNo = random() % 7;
            if (collision()) {
                GameState = OVER;
            }
        }
    }
    //パターン番号（色番号）をステージにセット
    setPtnNoToStage();

    //描画
    clear(0);
    drawStage();
}

void over() {
    clear(0);
    drawStage();
    //ゲームオーバーテキスト
    fill(0, 0, 255);
    textSize(Size);
    text("ＧａｍｅＯｖｅｒ", (width - Size * 8) / 2, Size);
    if (isTrigger(KEY_SPACE)) {
        GameState = INIT;
    }
}

void gmain() {
    window(Size * COLS, Size * ROWS);
    while (notQuit) {
        if      (GameState == INIT)init();
        else if (GameState == PLAY)play();
        else if (GameState == OVER)over();
    }
}


#endif

#ifdef _SAMPLE1
#include"libOne.h"
//外部変数------------------------------------------------------------
//シーケンス
const int INIT = 0;
const int PLAY = 1;
const int OVER = 2;
int GameState = INIT;
//色データ
float Satu = 200;
float Value = 255;
float Alpha = 255;
struct COLOR Color[9] = {
    COLOR(0,Satu,Value,Alpha),
    COLOR(30,Satu,Value,Alpha),
    COLOR(60,Satu,Value,Alpha),
    COLOR(120,Satu,Value,Alpha),
    COLOR(180,Satu,Value,Alpha),
    COLOR(220,Satu,Value,Alpha),
    COLOR(300,Satu,Value,Alpha),
    COLOR(200,44,88,Alpha),
    COLOR(0,0,20,Alpha),
};
//壁と背景の色番号
const int WALL = 7;
const int BACK = 8;
//ステージ（色番号を記憶する）
const int ROWS = 20;
const int COLS = 12;
int Stage[ROWS][COLS];
float Size = 50;
//ブロックの回転数
int R;
//ブロックパターンの位置（基準は[0]）
int Px[4], Py[4];
//ブロック移動制御
int FallFlag = 0;
int LoopCnt = 0;
//ブロックパターン番号  ※ 兼色番号 ※
int PtnNo;
//ブロックパターンオフセット値(基準[0]からずらす値)
int PtnOffsets[7][3][2] = {
    //■□■■...0
    {{-1,0}, {1,0}, {2,0}},
    //■
    //■□■....1
    {{-1,-1},{-1,0},{1,0}},
    //  　■
    //■□■....2
    {{-1,0}, {1,0}, {1,-1}},
    //■□
    //　■■....3
    {{-1,0}, {0,1}, {1,1}},
    //　□■
    //■■  ....4
    {{ 1,0}, {0,1}, {-1,1}},
     //　■
     //■□■....5
    {{-1,0}, {0,-1}, {1,0}},
     //□■
     //■■......6
    {{1,0},  {0,1},  {1,1}},
};

//関数----------------------------------------------------------------
void init() {
    //壁と背景の色番号をStage 2次元配列 にセット
    for (int y = 0; y < ROWS; y++) {
        Stage[y][0] = Stage[y][COLS - 1] = WALL;
        for (int x = 1; x < COLS - 1; x++) {
            Stage[y][x] = BACK;
            if (y == ROWS - 1) {
                Stage[y][x] = WALL;
            }
        }
    }
    //ブロック初期位置（開始時は４つとも同じでよい）
    for (int i = 0; i < 4; i++) {
        Px[i] = 5;
        Py[i] = 1;
    }
    R = 0;
    PtnNo = random() % 7;
    //次のシーケンス
    GameState = PLAY;
}

void drawStage() {
    angleMode(DEGREES);
    colorMode(HSV);
    strokeWeight(20);
    rectMode(CENTER);
    float px, py, halfSize = Size / 2;
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            //Stage配列の番号に従った色のrectを描画する
            int no = Stage[y][x];
            stroke(Color[no]);
            fill(0, 0, 180);
            px = halfSize + Size * x;
            py = halfSize + Size * y;
            rect(px, py, halfSize, halfSize);
        }
    }
}
void setPtnPosition() {
    //回転数
    int r = R % 4;
    for (int i = 0; i < 3; i++) {
        //現在のブロックパターンのオフセット値を取得
        int ofsX = PtnOffsets[PtnNo][i][0];
        int ofsY = PtnOffsets[PtnNo][i][1];
        //回転数に応じて９０度ずつ回転させる
        for (int j = 0; j < r && PtnNo != 6; j++) {
            int w = ofsX;
            ofsX = -ofsY;
            ofsY = w;
        }
        //位置決定
        Px[i + 1] = Px[0] + ofsX;
        Py[i + 1] = Py[0] + ofsY;
    }
}
void delPtnNoFromStage() {
    for (int i = 0; i < 4; i++) {
        int x = Px[i];
        int y = Py[i];
        Stage[y][x] = BACK;
    }
}
void setPtnNoToStage() {
    setPtnPosition();
    for (int i = 0; i < 4; i++) {
        int x = Px[i];
        int y = Py[i];
        Stage[y][x] = PtnNo;
    }
}
int collision() {
    //仮の位置を決める
    setPtnPosition();
    //当たっているかチェック
    int flag = 0;
    for (int i = 0; i < 4; i++) {
        int x = Px[i];
        int y = Py[i];
        if (Stage[y][x] != BACK) {
            flag = 1;
        }
    }
    return flag;
}
void complete() {
    for (int y = 1; y < ROWS - 1; y++) {
        //y行がそろったかチェック
        int flag = 1;
        for (int x = 1; x < COLS - 1; x++) {
            if (Stage[y][x] == BACK) {
                flag = 0;
            }
        }
        if (flag) {
            //そろったのでスライド
            for (int upy = y - 1; upy >= 0; upy--) {
                for (int x = 1; x < COLS - 1; x++) {
                    Stage[upy + 1][x] = Stage[upy][x];
                }
            }
        }
    }
}
void play() {
    //現在のパターン番号（色番号）をステージから消す
    delPtnNoFromStage();
    //移動、回転
    int dy = 0, dx = 0, dr = 0;;
    if ((++LoopCnt %= 15) == 0)dy = 1;
    if (isTrigger(KEY_D))dx = 1;
    if (isTrigger(KEY_A))dx = -1;
    if (isTrigger(KEY_W))dr = 1;
    if (isTrigger(KEY_S))FallFlag = 1;
    if (FallFlag)dy = 1;
    Px[0] += dx;
    Py[0] += dy;
    R += dr;
    if (collision()) {
        //元の位置、回転に戻す
        Px[0] -= dx;
        Py[0] -= dy;
        R -= dr;
        FallFlag = 0;
        if (dy == 1 && dx == 0 && dr == 0) {
            //積もらせる
            setPtnNoToStage();
            //行がそろっていたらスライドさせる
            complete();
            //新しいブロック誕生
            Px[0] = 5;
            Py[0] = 1;
            R = 0;
            PtnNo = random() % 7;
            if (collision()) {
                GameState = OVER;
            }
        }
    }
    //パターン番号（色番号）をステージにセット
    setPtnNoToStage();

    //描画
    clear(0);
    drawStage();
}

void over() {
    clear(0);
    drawStage();
    //ゲームオーバーテキスト
    for (int x = 1; x < COLS - 1; x++)Stage[0][x] = BACK;
    fill(0, 0, 255);
    textSize(Size);
    text("ＧａｍｅＯｖｅｒ", (width - Size * 8) / 2, Size);
    if (isTrigger(KEY_SPACE)) {
        GameState = INIT;
    }
}

void gmain() {
    window(Size * COLS, Size * ROWS, full);
    ShowCursor(false);
    while (notQuit) {
        if (GameState == INIT)init();
        else if (GameState == PLAY)play();
        else if (GameState == OVER)over();
    }
    ShowCursor(true);
}
#endif
