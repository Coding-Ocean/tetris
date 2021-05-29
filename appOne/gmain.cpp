#define _EX
#ifdef _EX

#endif
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
    COLOR(200,44,88,Alpha),
    COLOR(0,0,20,Alpha),
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
int Ptn[7][3][2] = {
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
        int x = Ptn[PtnNo][i][0];
        int y = Ptn[PtnNo][i][1];
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

            fill(0, 0, 100);
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
