#define _EX
#ifdef _SAMPLE2
#include"libOne.h"
float Satu = 255;
float Value = 255;
float Alpha = 255;
struct COLOR Color[9] = {
    COLOR(0,Satu,Value,Alpha),
    COLOR(30,Satu,Value,Alpha),
    COLOR(60,Satu,Value,Alpha),
    COLOR(120,Satu,Value,Alpha),
    COLOR(180,Satu,Value,Alpha),
    COLOR(240,Satu,Value,Alpha),
    COLOR(300,Satu,Value,Alpha),
    COLOR(0,0,120,Alpha),//wall
    COLOR(0,0, 60,Alpha),//back
};
const int WALL = 7;
const int BACK = 8;
const int ROWS = 20;
const int COLS = 12;
int Stage[ROWS][COLS];
float Size = 50;
//シーケンス
const int INIT = 0;
const int PLAY = 1;
const int OVER = 2;
int GameState = INIT;
//ブロック
int X, Y, R;
int PtnNo;
int LoopCnt = 0;
//パターン
int Ptn[7][6] = {
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
int Px[4] = { 1,1,1,1 };
int Py[4] = { 1,1,1,1 };
int FallFlag = 0;

void setPtnPos() {
    Px[0] = X;
    Py[0] = Y;
    int r = R % 4;
    for (int i = 0; i < 3; i++) {
        int x = Ptn[PtnNo][i * 2];
        int y = Ptn[PtnNo][i * 2 + 1];
        for (int j = 0; j < r; j++) {
            int w = x;
            x = -y;
            y = w;
        }
        Px[i + 1] = X + x;
        Py[i + 1] = Y + y;
    }
}
void setPtnToStage() {
    setPtnPos();
    for (int i = 0; i < 4; i++) {
        int x = Px[i];
        int y = Py[i];
        Stage[y][x] = PtnNo;
    }
}
void clearPtnFromStage() {
    for (int i = 0; i < 4; i++) {
        int x = Px[i];
        int y = Py[i];
        Stage[y][x] = BACK;
    }
}


void init() {
    //壁
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
    PtnNo = random() % 7;
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
            stroke(Color[Stage[y][x]]);

            fill(0, 0, 80);
            rect(Size/2+Size * x, Size/2+Size * y, Size/2, Size/2);
        }
    }
}
int collision() {
    setPtnPos();
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
    
    //Stage[Y][X] = BACK;
    clearPtnFromStage();

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
    //if (Stage[Y][X] != BACK) {
    if (collision()) {
        Y -= dy;
        X -= dx;
        R -= dr;
        FallFlag = 0;
        if (dy == 1 && dx == 0 && dr == 0) {
            //Stage[Y][X] = PtnNo;
            setPtnToStage();
            complete();
            X = 5;
            Y = 1;
            R = 0;
            //++PtnNo %= 7;
            PtnNo = random() % 7;
            if (collision()) {
                GameState = OVER;
            }
        }
    }
    
    //Stage[Y][X] = PtnNo;
    setPtnToStage();

    clear(0);
    drawStage();
}
void over() {
    clear(0);
    drawStage();
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

        if (GameState == INIT)init();
        else if (GameState == PLAY)play();
        else if (GameState == OVER)over();
    }
}


#endif























#ifdef _SAMPLE
#include"libOne.h"
//global value--------------------------------------------------------------------
float Satu = 255;
float Value = 255;
float Alpha = 255;
struct COLOR Color[9] = {
    COLOR(0,Satu,Value,Alpha),
    COLOR(30,Satu,Value,Alpha),
    COLOR(60,Satu,Value,Alpha),
    COLOR(120,Satu,Value,Alpha),
    COLOR(180,Satu,Value,Alpha),
    COLOR(240,Satu,Value,Alpha),
    COLOR(300,Satu,Value,Alpha),
    COLOR(0,0,120,Alpha),//7
    COLOR(0,0,30,Alpha)//8
};
const int WALL = 7;
const int EMPTY = 8;
const int Rows = 20;
const int Cols = 12;
int Stage[Rows][Cols];
float Size = 50;
const int INIT = 0;
const int PLAY = 1;
const int OVER = 2;
int GameState = INIT;
int X, Y, R, PtnNo;
int Ptn[7][6] = {
    //■□■■
    -1,0,   1,0,  2,0,
    //■
    //■□■
    -1,-1, -1,0,  1,0,
    //  ■
    //■□■
    -1,0,   1,-1, 1,0,
    //■□
    //　■■
    - 1,0,   0,1,  1,1,
    //　□■
    //■■
     1,0,   0,1, -1,1,
    //　■
    //■□■
    -1,0,   0,-1, 1,0,
    //□■
    //■■
     1,0,   0,1,  1,1,
};
int Px[4], Py[4];
int Fall = 0;
float Angle = 0;
int LoopCnt = 0;
//functions------------------------------------------------------------------------
void setPosition() {
    Px[0] = X;
    Py[0] = Y;
    int r = R % 4;
    for (int i = 0; i < 3; i++) {
        int ofsx = Ptn[PtnNo][i * 2 + 0];
        int ofsy = Ptn[PtnNo][i * 2 + 1];
        for (int i = 0; i < r; i++) {
            int w = ofsx;
            ofsx = -ofsy;
            ofsy = w;
        }
        Px[i + 1] = X+ofsx;
        Py[i + 1] = Y+ofsy;
    }
}
void setPattern() {
    setPosition();
    for (int i = 0; i < 4; i++) {
        Stage[Py[i]][Px[i]] = PtnNo;
    }
}
void clearPattern() {
    for (int i = 0; i < 4; i++) {
        Stage[Py[i]][Px[i]] = EMPTY;
    }
}
int collision() {
    setPosition();
    int flag = 0;
    for (int i = 0; i < 4; i++) {
        if (Stage[Py[i]][Px[i]] != EMPTY) {
            flag = 1;
        }
    }
    return flag;
}
void init() {
    for (int y = 0; y < Rows; y++) {
        Stage[y][0] = WALL;
        Stage[y][Cols - 1] = WALL;
        for (int x = 1; x < Cols-1; x++) {
            Stage[y][x] = EMPTY;
            if (y == Rows - 1) {
                Stage[y][x] = WALL;
            }
        }
    }
    X = 5;
    Y = 1;
    R = 0;
    PtnNo = random()%7;
    setPosition();
    Angle = 0;
    Alpha = 255;
    GameState = PLAY;
}

void drawStage() {
    angleMode(DEGREES);
    colorMode(HSV);
    rectMode(CENTER);
    strokeWeight(20);
    for (int y = 0; y < Rows; y++) {
        for (int x = 0; x < Cols; x++) {
            stroke(Color[Stage[y][x]]);
            fill(0, 0, 90);
            rect(Size / 2 + Size * x, Size / 2 + Size * y, Size/2, Size/2,Angle);
            //circle(Size/2+Size * x, Size/2+Size * y, Size);
        }
    }
}
void complete() {
    for (int y = 1; y <= Rows - 2; y++) {
        int flag = 1;
        for (int x = 1; x <= Cols - 2; x++) {
            if (Stage[y][x] == EMPTY) {
                flag = 0;
            }
        }
        if (flag) {
            //スライドさせる
            for (int upy = y - 1; upy >= 0; upy--) {
                for (int x = 1; x <= Cols - 2; x++) {
                    Stage[upy + 1][x] = Stage[upy][x];
                }
            }
        }
    }
}
void play() {
    //まずパターン（色）番号をステージから消す
    clearPattern();
    //移動方向、回転角度決定
    int dx = 0, dy = 0, dr = 0;
    if ((++LoopCnt %= 20) == 0)dy = 1;
    if (isTrigger(KEY_D))dx = 1;
    if (isTrigger(KEY_A))dx = -1;
    if (isTrigger(KEY_W))dr = 1;
    if (isTrigger(KEY_S))Fall = 1;
    if (Fall)dy = 1;
    //とりあえず移動回転してみる
    X += dx;
    Y += dy;
    R += dr;
    //他のブロックと当たった
    if (collision()) {
        //元の値に戻す
        X -= dx;
        Y -= dy;
        R -= dr;
        Fall = 0;
        if (dy == 1 && dx == 0 && dr == 0) {
            //左右移動、回転でなく「下に落ちただけの時」は、積もらせる
            setPattern();
            //行がそろったらスライドする
            complete();
            //次のパターンデータ生成
            X = 5;
            Y = 1;
            R = 0;
            PtnNo = random() % 7;
            //++PtnNo %= 7;
            //いきなり当たったらゲームオーバー
            if (collision()) {
                GameState = OVER;
            }
        }
    }
    //パターン（色）番号をステージにセット
    setPattern();
    //描画
    clear();
    drawStage();
}
void over() {
    clear();
    Angle += 0.2f;
    drawStage();
    strokeWeight(20);
    stroke(0, 0, 60, 200);
    fill(0, 0, 60, 200);
    rectMode(CORNER);
    rect(Size * 2, Size * 6, Size * 8, Size);
    fill(255);
    textSize(Size);
    text("ＧａｍｅＯｖｅｒ", (width - Size * 8) / 2, Size*7);
    if (isTrigger(KEY_SPACE)) {
        GameState = INIT;
    }
}
void gmain() {
    window(Size * Cols, Size * Rows, full);
    ShowCursor(false);
    while (notQuit) {
        if (GameState == INIT)init();
        else if (GameState == PLAY)play();
        else if (GameState == OVER)over();
    }
    ShowCursor(true);
}


#endif
