#pragma warning(disable:4996)  
#pragma comment(lib,"winmm.lib") 
#pragma comment(lib, "user32")
#pragma comment( lib, "MSIMG32.LIB")
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<string>
#include<Windows.h>
#include<mmsystem.h>
#include <mciapi.h>
#include<stdio.h>
#include<fstream>
#include<sstream>
#include<stdlib.h>
#include<algorithm>
#include <stdint.h>
#include<graphics.h>
#include <easyx.h>
#include <conio.h>
#include<button.h>
#include <tchar.h>
#include <codecvt>
#include<queue>
using namespace std;
int i, j, ss[20][15];
string s[26][60];
class player {
public: 
    wstring name, story;
    int lv ,mhp,hp,dex,move,isize,asize,x,y,pturn,speed,abox,act;
    player(wstring a,wstring b,int c,int d,int e,int f,int g,int h,int i,int j,int k,int l,int m,int n,int o):name(a),story(b), lv(c), mhp(d), hp(e), dex(f), move(g), isize(h), asize(i), x(j), y(k), pturn(l), speed(m), abox(n), act(o) {}
   
};
class enemy {
public:
    wstring name, story;
    int lv, mhp,hp,dex, str,x,y,speed,turn,move,baid;
    enemy(wstring a,wstring b,int c,int d,int e,int f,int g,int h,int i,int j,int k,int l,int m):name(a),story(b),lv(c),mhp(d),hp(e),dex(f),str(g),x(h),y(i),speed(j),turn(k),move(l),baid(m){}
};
void clearQpair(std::queue<pair<int, int>>& q)
{
    queue<pair<int, int>> empty;
    std::swap(q, empty);
}
void Bbfs(int sx, int sy, int x, int y, int* box) {
    int X=sx, Y=sy,road[20][15],k,w;
    for (k = 0; k < 20; k++) {
        for (w = 0; w < 15; w++) {
            road[k][w]=-1;
        }
    }
    road[X][Y]=0;
    queue<pair<int, int>> q;
    clearQpair(q);
    q.push({ X,Y });
    while (q.size()>0) {
        X = q.front().first;
        Y = q.front().second;
        if (abs(X-x)+abs(Y-y)==1) {
            break;
        }
        if (ss[X+1][Y]==0&&road[X+1][Y]==-1&&X+1<=19) {
            q.push({X+1,Y});
            road[X + 1][Y] = 6;
        }
        if (ss[X -1][Y] == 0 && road[X - 1][Y] == -1 && X - 1 >= 0) {
            q.push({ X - 1,Y });
            road[X - 1][Y] = 4;
        }
        if (ss[X ][Y+1] == 0 && road[X ][Y+1] == -1 && Y + 1 <= 14) {
            q.push({ X ,Y+1 });
            road[X ][Y+1] = 2;
        }
        if (ss[X][Y-1] == 0 && road[X][Y-1] == -1 && Y - 1 >= 0) {
            q.push({ X ,Y-1 });
            road[X][Y -1] = 8;
        }
        q.pop();
    }
    int b[1000];
    for (w = 0; w < 1000; w++) { b[w] = -1; }
    w = 0;
    while (1) {
        if (road[X][Y] == 8) {
            Y++; b[w] = 8;
        }
        else if (road[X][Y] == 6) {
            X--; b[w] = 6;
        }
        else if (road[X][Y] == 4) {
            X++; b[w] = 4;
        }
        else if (road[X][Y] == 2) {
            Y--; b[w] = 2;
        }
        else if (road[X][Y] == 0) {
            for (k = 0; k < w; k++) {
                box[k] = b[w - k - 1];
            }
            box[k] = 0;
            break;
        }
        w++;
    }
}
void bfs(int sx, int sy, int x, int y, int* box, int* move, int P_id) {
    int X = sx, Y = sy, road[20][15], k, w;
    for (k = 0; k < 20; k++) {
        for (w = 0; w < 15; w++) {
            road[k][w] = -1;
        }
    }
    road[X][Y] = 0;
    queue<pair<int, int>> q;
    clearQpair(q);
    q.push({ X,Y });
    while (q.size() > 0) {
        X = q.front().first;
        Y = q.front().second;
        if (X == x && Y == y) {
            break;
        }
        if (ss[X + 1][Y] == 0 && road[X + 1][Y] == -1 && X + 1 <= 19) {
            q.push({ X + 1,Y });
            road[X + 1][Y] = 6;
        }
        if (ss[X - 1][Y] == 0 && road[X - 1][Y] == -1 && X - 1 >= 0) {
            q.push({ X - 1,Y });
            road[X - 1][Y] = 4;
        }
        if (ss[X][Y + 1] == 0 && road[X][Y + 1] == -1 && Y + 1 <= 14) {
            q.push({ X ,Y + 1 });
            road[X][Y + 1] = 2;
        }
        if (ss[X][Y - 1] == 0 && road[X][Y - 1] == -1 && Y - 1 >= 0) {
            q.push({ X ,Y - 1 });
            road[X][Y - 1] = 8;
        }
        q.pop();
    }
    int b[1000];
    for (w = 0; w < 1000; w++) { b[w] = -1; }
    w = 0;
    while (1) {
        if (road[X][Y] == 8) {
            Y++; b[w] = 8;
        }
        else if (road[X][Y] == 6) {
            X--; b[w] = 6;
        }
        else if (road[X][Y] == 4) {
            X++; b[w] = 4;
        }
        else if (road[X][Y] == 2) {
            Y--; b[w] = 2;
        }
        else if (road[X][Y] == 0) {
            for (k = 0; k < w; k++) {
                box[k] = b[w - k - 1];
            }
            box[k] = 0;
            move[P_id] -= w;
            break;
        }
        w++;
    }
}
void A_star(int sx, int sy, int x, int y,int* box) {
    int road[15][20], g[15][20], h[15][20], a = sx, b = sy,c[15][20];
    int k, w;
     for (w = 0; w < 15; w++) {
      for (k = 0; k < 20; k++)  {
            road[w][k] = -1;
            c[w][k] = 0;
        }
    }
     for (w = 0; w < 15; w++){
         for (k = 0; k < 20; k++){
            g[w][k] = 10000; h[w][k] = 10000;
        }
    }
    road[sx][sy] = 0;
    if (b - 1 >= 0&&ss[a][b-1]!=1) {
        road[b - 1][a] = 8;
        g[b - 1][a] = 10; h[b - 1][a] = abs(a - x) * 10 + abs(b-1 - y) * 10;
    }
    if (a - 1 >= 0 && ss[a-1][b] != 1) {
        road[b][a - 1] = 4;
        g[b][a - 1] = 10; h[b][a - 1] = abs(a -1- x)*10 + abs(b - y)*10;
    }
    if (a + 1 < 20 && ss[a+1][b] != 1) {
        road[b][a + 1] = 6;
        g[b][a + 1] = 10; h[b][a + 1] = abs(a +1- x) * 10 + abs(b - y) * 10;
    }
    if (b + 1 < 15 && ss[a][b+1] != 1) {
        road[b + 1][a] = 2;
        g[b + 1][a] = 10; h[b + 1][a] = abs(a - x) * 10 + abs(b +1- y) * 10;
    }
    while (1) {
        int min = 20000;
        for (w = 0; w < 15; w++) {
             for (k = 0; k < 20; k++){
                if (h[w][k] == 0) {
                    break;
                }
                if (min > g[w][k] + h[w][k] && c[w][k] != 1) {
                    min = g[w][k] + h[w][k];
                    b = w; a = k;
                }
            }
            if (h[w][k] == 0) {
                break;
            }
        }
        c[b][a] = 1;
        if (h[w][k] == 0) {
            break;
        }
        if (b - 1 >= 0&& road[b - 1][a] ==-1&&ss[a][b-1]!=1) {
            road[b - 1][a] = 8;
            g[b - 1][a] =g[b][a] + 10; h[b - 1][a] = abs(a - x)*10 + abs(b -1- y)*10;
        }
        if (a - 1 >= 0&& road[b][a - 1] ==-1 && ss[a-1][b ] != 1) {
            road[b][a - 1] = 4;
            g[b][a - 1] = g[b][a] + 10; h[b][a - 1] = abs(a-1 - x) *10+ abs(b - y)*10;
        }
        if (a + 1 < 20&& road[b][a + 1] ==-1 && ss[a+1][b] != 1) {
            road[b][a + 1] = 6;
            g[b][a + 1] = g[b][a] + 10; h[b][a + 1] = abs(a +1- x) *10+ abs(b - y)*10;
        }
        if (b + 1 < 15&& road[b + 1][a] ==-1 && ss[b + 1][a] != 1) {
            road[b + 1][a] = 2;
            g[b + 1][a] = g[b][a] + 10; h[b + 1][a] = abs(a - x)*10 + abs(b +1- y)*10;
        }
    }
    b = w; a = k;
    w = 0;
    int boxa[3000];
    while (1) {
        if (road[b][a] == 8) {
            b++; boxa[w] = 8;
        }
        else if (road[b][a] ==6 ) {
            a--; boxa[w] = 6;
        }
        else if (road[b][a] ==4 ) {
            a++; boxa[w] = 4;
        }
        else if (road[b][a] == 2) {
            b--; boxa[w] = 2;
        }
        else if (road[b][a] ==0 ) {
            for (k = 0; k < w; k++) {
                box[k] = boxa[w - k-1];
            }
            box[k] = 0;
            break;
        }
        w++;
}
}
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor)
{
    HDC dstDC = GetImageHDC(dstimg);
    HDC srcDC = GetImageHDC(srcimg);
    int w = srcimg->getwidth();
    int h = srcimg->getheight();

    // 使用 Windows GDI 函数实现透明位图
    TransparentBlt(dstDC, x, y, w, h, srcDC, 0, 0, w, h, transparentcolor);
}
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
    // 变量初始化
    DWORD* dst = GetImageBuffer(dstimg);
    DWORD* src = GetImageBuffer(srcimg);
    int src_width = srcimg->getwidth();
    int src_height = srcimg->getheight();
    int dst_width = (dstimg == NULL ? getwidth() : dstimg->getwidth());
    int dst_height = (dstimg == NULL ? getheight() : dstimg->getheight());

    // 计算贴图的实际长宽
    int iwidth = (x + src_width > dst_width) ? dst_width - x : src_width;		// 处理超出右边界
    int iheight = (y + src_height > dst_height) ? dst_height - y : src_height;	// 处理超出下边界
    if (x < 0) { src += -x;				iwidth -= -x;	x = 0; }				// 处理超出左边界
    if (y < 0) { src += src_width * -y;	iheight -= -y;	y = 0; }				// 处理超出上边界

    // 修正贴图起始位置
    dst += dst_width * y + x;

    // 实现透明贴图
    for (int iy = 0; iy < iheight; iy++)
    {
        for (int ix = 0; ix < iwidth; ix++)
        {
            int sa = ((src[ix] & 0xff000000) >> 24);
            int sr = ((src[ix] & 0xff0000) >> 16);	// 源值已经乘过了透明系数
            int sg = ((src[ix] & 0xff00) >> 8);		// 源值已经乘过了透明系数
            int sb = src[ix] & 0xff;				// 源值已经乘过了透明系数
            int dr = ((dst[ix] & 0xff0000) >> 16);
            int dg = ((dst[ix] & 0xff00) >> 8);
            int db = dst[ix] & 0xff;

            dst[ix] = ((sr + dr * (255 - sa) / 255) << 16)
                | ((sg + dg * (255 - sa) / 255) << 8)
                | (sb + db * (255 - sa) / 255);
        }
        dst += dst_width;
        src += src_width;
    }
}
void Bplayer( int* bx, int* by, int bsize) {
    for (int I = 0; I < bsize; I++) {
        IMAGE bplayer;
        loadimage(&bplayer, L"./Game/picture/wolf1.png", 0, 0, false);
        transparentimage(NULL, (bx[I]) * 48, ((by[I]) * 48), &bplayer, 0xFF55FF);
    }
}
void mbplayer(int* bx, int* by, int bid,int bsize) {
    for (int I = 0; I < bsize; I++) {
        if (I != bid) {
            IMAGE bplayer;
            loadimage(&bplayer, L"./Game/picture/wolf1.png", 0, 0, false);
            transparentimage(NULL, (bx[I]) * 48, ((by[I]) * 48), &bplayer, 0xFF55FF);
        }
    }
}
void Player(int P_id,int *x,int*y,wstring *name) {
    IMAGE c;
    loadimage(&c, L"./Game/picture/c.png", 0, 0, false);
    transparentimage(NULL,0, 730, &c, 0xFF55FF);
    wchar_t* ptr = _wcsdup(name[P_id].c_str());
    outtextxy(60, 721, ptr);
    IMAGE player;
    loadimage(&player, L"./Game/picture/p1.png", 0, 0, false);
transparentimage(NULL, (x[P_id]) * 48, ((y[P_id]-1) * 48), &player, 0xFF55FF);
}
int roll(string d, int a) {
    int t = 0, s = 0, g = 0, n = 1, k, j, l, sum = 0;
    if (a == 1) {
        for (k = 0; k < d.size(); k++) {

            if (d[k] == 'd') {
                break;
            }  n *= 10;
        }
        for (k = 0; k < d.size(); k++) {

            if (d[k] == 'd') {
                break;
            } n /= 10;
            t += (d[k] - 48) * n;
        }
        for (j = k + 1; j < d.size(); j++) {

            if (d[j] == '+') {
                break;
            }n *= 10;
        }
        for (j = k + 1; j < d.size(); j++) {
            if (d[j] == '+') {
                break;
            }
            n /= 10;
            s += (d[j] - 48) * n;
        }
        for (l = j + 1; l < d.size(); l++) {
            n *= 10;
        }
        for (l = j + 1; l < d.size(); l++) {
            n /= 10; g += (d[l] - 48) * n;

        }
        for (k = 0; k < t; k++) {
            int x = rand() % (s)+1;
            sum += x;
        }
        return sum + g;
    }
    else if (a == 2) {
        int x = rand() % 20 + 1;
        return x;
    }
}
void rows() {
    for (i = 0; i < 40; i++) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
        cout << "▔";
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << endl;
}
void maps(int* y, int* x, int* bx, int* by, int P_id, int id, int psize, int bsize, string* mark, string* bmark,int *hp,int *mhp,string *bname) {
    IMAGE back1;
    loadimage(&back1, L"./Game/picture/background2.png", 0, 0, false);
    putimage(960, 0, &back1);
    IMAGE map01;
    loadimage(&map01, L"./Game/picture/map01.png",0,0,false);
    putimage(0, 0, &map01);
    IMAGE back2;
    loadimage(&back2, L"./Game/picture/background.png", 0, 0, false);
    putimage(0, 720, &back2);
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 15; j++) {
            ss[i][j] = 0;
        }
    }
    for (i = 0; i < psize; i++) {
        ss[x[i]][y[i]] = 1;
        s[x[i]][y[i]] = mark[i];
    }
    for (i = 0; i < bsize; i++) {
        ss[bx[i]][by[i]] = 1;
        s[bx[i]][by[i]] = bmark[i];
    }

    for (j = 0; j < 15; j++) {
        for (i = 0; i < 20; i++) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
            for (int k = 0; k < psize; k++) {
                if (j == y[k] && i == x[k]) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }
            }
            for (int k = 0; k < bsize; k++) {
                if (j == by[k] && i == bx[k]) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }
            }
            cout << s[j][i];
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        cout << " " << j + 1;
        cout << endl;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    for (i = 0; i < 20; i++) {
        if (i < 9) {
            cout << i + 1 << " ";
        }
        else {
            cout << i + 1;
        }
    }
    cout << endl << endl;    
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    wchar_t s[] = L"HP[";
    outtextxy(30, 875, s);
    TCHAR t[5];
    _stprintf(t, _T("%d"),hp[P_id]);		// 高版本 VC 推荐使用 _stprintf_s 函数
    outtextxy(55, 875, t);
    wchar_t s1[] = L"/";
    outtextxy(90, 875, s1);
    _stprintf(t, _T("%d"), mhp[P_id]);		// 高版本 VC 推荐使用 _stprintf_s 函数
    outtextxy(95, 875, t);
    wchar_t s2[] = L"]";
    outtextxy(130, 875, s2);
    IMAGE atk;
    loadimage(&atk, L"./Game/picture/attack.png", 0, 0, false);
    putimage(200, 750, &atk);
    IMAGE movep;
    loadimage(&movep, L"./Game/picture/move.png", 0, 0, false);
    putimage(300, 750, &movep);
    IMAGE item;
    loadimage(&item, L"./Game/picture/item.png", 0, 0, false);
    putimage(400, 750, &item);
    IMAGE end;
    loadimage(&end, L"./Game/picture/end.png", 0, 0, false);
    putimage(500, 750, &end);
    IMAGE save;
    loadimage(&save, L"./Game/picture/save.png", 0, 0, false);
    putimage(600, 750, &save);
    IMAGE load;
    loadimage(&load, L"./Game/picture/load.png", 0, 0, false);
    putimage(700, 750, &load);
}
void acts(int x, int y, int move, int act, string& chose, int P_id,wstring* name, int* hp, int* mhp,int bsize,int id,int *bx,int *by,wstring *Bname,int *bhp,int *bmhp) {
    ExMessage m;
    int sss = 1;
    int flag = -1;
    int flag1 = 0;
    while (sss != 0)
    {
        // 获取一条鼠标消息
        m = getmessage(EM_MOUSE);            
        flag1 = 0;
        switch (m.message) {
        case WM_MOUSEMOVE:
            for (i = 0; i < bsize; i++) {
                if (m.x >= bx[i] * 48 && m.x <= bx[i] * 48 + 48 && m.y >= by[i] * 48 && m.y <= by[i] * 48 + 48) {
                    wchar_t* ptr = _wcsdup(Bname[i].c_str());
                    outtextxy(960,0, ptr);
                    flag = 1;
                    wchar_t s[] = L"HP[";
                    outtextxy(960,30, s);
                    TCHAR t[5];
                    _stprintf(t, _T("%d"), bhp[i]);		// 高版本 VC 推荐使用 _stprintf_s 函数
                    outtextxy(985, 30, t);
                    wchar_t s1[] = L"/";
                    outtextxy(1020, 30, s1);
                    _stprintf(t, _T("%d"), bmhp[i]);		// 高版本 VC 推荐使用 _stprintf_s 函数
                    outtextxy(1025, 30, t);
                    wchar_t s2[] = L"]";
                    outtextxy(1060, 30, s2);
                    IMAGE tri;
                    loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                    transparentimage(NULL, (bx[i]) * 48, ((by[i] - 1) * 48), &tri, 0xFF55FF);
                    j = i;
                    break;
                }
                if (i == bsize - 1) {
                    flag1 = 1;
                }
            }
            if (flag == 1 && flag1 == 1) {
                flag = 0;
            }
            if (flag == 0) {
                IMAGE back1;
                loadimage(&back1, L"./Game/picture/background2.png", 0, 0, false);
                putimage(960, 0, &back1);
                IMAGE img;
                loadimage(&img, L"./Game/picture/grass.png", 0, 0, false);
                putimage((bx[j]) * 48, 48 * (by[j] - 1), &img);
                flag = -1;
            }
            break;
        case WM_LBUTTONDOWN://按鼠标左键分发事件
            if (m.x >= 200 && m.x <= 296&&m.y>=750&&m.y<=798) {
                chose = "a";
                sss = 0;
            }
            if (m.x >= 300 && m.x <= 396 && m.y >= 750 && m.y <= 798) {
                chose = "w";
                sss = 0;
            }
            if (m.x >= 400 && m.x <= 496 && m.y >= 750 && m.y <= 798) {
                chose = "i";
                sss = 0;
            }
            if (m.x >= 500 && m.x <= 596 && m.y >= 750 && m.y <= 798) {
                chose = "e";
                sss = 0;
            }
            if (m.x >= 600 && m.x <= 696 && m.y >= 750 && m.y <= 798) {
                chose = "s";
                sss = 0;
            }
            if (m.x >= 700 && m.x <= 796 && m.y >= 750 && m.y <= 798) {
                chose = "l";
                sss = 0;
            }
        }

    }
    cout << "座標 P[" << x + 1 << "," << y + 1 << "] ";
    cout << "HP[" << hp[P_id] << "/" << mhp[P_id] << "] ";
    cout << "移動力[" << move << "/6] ";
    cout << "行動力[" << act << "/1]" << endl;
    cout << "[a]攻擊 " << "[d]偵查 " << "[i]物品 " << "[w]移動 " << "[s]存檔 " << "[l]讀檔 " << "[e]結束行動" << endl;
    cout << ">";
}
void ATTACK(string chose, int* act, int* bullet, int* mbullet, int P_id, int* y, int* x, int* dex, wstring* name, int id, int* bx, int* by, int* bmhp, int* bhp, int* bdex, string* bname, int* range, int* bspeed, int bsize,int*asize,int *abox1,string *admg,wstring *Admg) {
    int sss=1;
    ExMessage m,m1; string attack;
    if (chose == "a" && act[P_id] > 0) {
        for (i = 0; i < asize[P_id]; i++) {
            IMAGE back1;
            loadimage(&back1, L"./Game/picture/attackblock.png", 0, 0, false);
            putimage(200, 799+48*i, &back1);
        }
        for (i = 0; i < asize[P_id]; i++) {
            if (abox1[P_id] == 0) {
                setbkmode(TRANSPARENT);
                settextcolor(BLACK);
                wchar_t s[] = L"栓動步槍(";
                outtextxy(200, 815, s);
                TCHAR t[3];
                _stprintf(t, _T("%d"), bullet[P_id]);		// 高版本 VC 推荐使用 _stprintf_s 函数
                outtextxy(267, 815, t);
                wchar_t s2[] = L")";
                outtextxy(290, 815, s2);
            }
        }
        while (sss != 0)
        {
            // 获取一条鼠标消息
            m = getmessage(EM_MOUSE);
            switch (m.message) {

            case WM_LBUTTONDOWN://按鼠标左键分发事件
                if (m.x >= 200 && m.x <= 296 && m.y >= 799 && m.y <= 895) {
                    attack = "a";
             }
            case WM_RBUTTONUP:
                sss = 0;
            }
        }      
        int rx, ry, AC, ATK, dmg;
        if (attack == "a" && bullet[0] > 0) {
            BeginBatchDraw();
            for (i = 0; i < 20; i++) {
                for (j = 0; j < 15; j++) {
                    rx = 0; ry = 0;
                    ry = (y[P_id] - j) * (y[P_id] > j) + (y[P_id] - j) * (y[P_id] < j) * -1; rx = (x[P_id] - i) * (x[P_id] > i) + (x[P_id] - i) * (x[P_id] < i) * -1;
                    if (ry + rx <= range[P_id] && rx + ry > 0 ) {
                        IMAGE mb;
                        loadimage(&mb, L"./Game/picture/ab.png", 0, 0, false);
                        transparentimage(NULL, 48 * i, 48 * j, &mb);
                    }
                }
            }
            IMAGE player;
            loadimage(&player, L"./Game/picture/p1.png", 0, 0, false);
            transparentimage(NULL, (x[P_id]) * 48, ((y[P_id] - 1) * 48), &player, 0xFF55FF);
            EndBatchDraw();
            bullet[0]--;
            act[P_id]--;
            rows();
            for (j = 0; j < 15; j++) {
                for (i = 0; i < 20; i++) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
                    for (int k = 0; k < bsize; k++) {
                        if (j == by[k] && i == bx[k]) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                        }
                    }
                    if (j == y[P_id] && i == x[P_id]) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    }
                    rx = 0; ry = 0;
                    ry = (y[P_id] - j) * (y[P_id] > j) + (y[P_id] - j) * (y[P_id] < j) * -1; rx = (x[P_id] - i) * (x[P_id] > i) + (x[P_id] - i) * (x[P_id] < i) * -1;
                    if (ry + rx <= range[P_id] && rx + ry > 0) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | BACKGROUND_RED);
                    }
                    cout << s[j][i];
                }
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                cout << " " << j + 1;
                cout << endl;
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            for (i = 0; i < 20; i++) {
                if (i < 9) {
                    cout << i + 1 << " ";
                }
                else {
                    cout << i + 1;
                }
            }
            sss = 1;
            while (sss != 0)
            {
                // 获取一条鼠标消息
                m1 = getmessage(EM_MOUSE);
                switch (m1.message) {

                case WM_LBUTTONDOWN://按鼠标左键分发事件
                    for (i = 0; i < 20; i++) {
                        for (j = 0; j < 15; j++) {
                            if (m1.x >= 48*i && m1.x <= 48*i+48 && m1.y >=48*j && m1.y <= 48*j+48) {
                                for (int k = 0; k < bsize; k++) {
                                    if (ss[bx[k]][by[k]] == 1&& bx[k]==i&&by[k] == j) {
                                        id = k; sss = 0;
                                    }
                                }
                            }
                        }
                    }
                case WM_RBUTTONUP:
                    sss = 0;
                }
            }
            cout << endl << endl << "選擇目標" << endl << ">";
            ry = (y[P_id] - by[id]) * (y[P_id] > by[id]) + (y[P_id] - by[id]) * (y[P_id] < by[id]) * -1; rx = (x[P_id] - bx[id]) * (x[P_id] > bx[id]) + (x[P_id] - bx[id]) * (x[P_id] < bx[id]) * -1;
            if (rx + ry <= range[P_id]) {
                AC = rand() % 20 + 1 + (bdex[id] - 10) / 2;
                ATK = rand() % 20 + 1 + (dex[P_id] - 10) / 2;
                if (ATK > AC) {
                    mciSendString(L"play ./Game/Sound/sgm/魔王魂_戦闘18.mp3 from 0", NULL, 0, NULL);
                    cout << "命中(" << ATK << ")" << ">防禦等級(" << AC << ")" << endl;
                    dmg=roll(admg[id], 1);
                    cout << bname[id] << "的HP(" << bhp[id] << ") - " << dmg << "(2d8)=" << bhp[id] - dmg << endl;
                    bhp[id] -= dmg;
                    BeginBatchDraw();
                    TCHAR d[5];
                    _stprintf(d, _T("%d"), -dmg);		// 高版本 VC 推荐使用 _stprintf_s 函数
                    outtextxy(bx[id]*48+24, by[id]*48, d);
                    wchar_t atk[] = L"命中";
                    outtextxy(960, 0, atk);
                    _stprintf(d, _T("%d"), ATK);
                    outtextxy(990, 0, d);
                    wchar_t du[] = L">";
                    outtextxy(1010, 0, du);
                    wchar_t aC[] = L"防禦等級";
                    outtextxy(1020, 0, aC);
                    _stprintf(d, _T("%d"), AC);
                    outtextxy(1090, 0, d);
                    wchar_t* ptr = _wcsdup(Admg[P_id].c_str());
                    outtextxy(960,30 , ptr);
                    wchar_t dnu[] = L"=";
                    outtextxy(1020,30, dnu);
                    _stprintf(d, _T("%d"), dmg);
                    outtextxy(1030, 30, d);
                    EndBatchDraw();
                    Sleep(1000);       
                    if (bhp[id] <= 0) {
                        cout << bname[id] << "已經死亡" << endl;
                        ss[by[id]][bx[id]] = 0;
                        bx[id] =-1; by[id] = -1;
                        bspeed[id] = -1;
                    }

                }
                else {
                    mciSendString(L"play ./Game/Sound/sgm/魔王魂_戦闘18.mp3 from 0", NULL, 0, NULL);
                    wchar_t w[] = L"未命中";
                    outtextxy(48*bx[id], 48*by[id], w);
                    BeginBatchDraw();
                    TCHAR d[5];
                    wchar_t atk[] = L"命中";
                    outtextxy(960, 0, atk);
                    _stprintf(d, _T("%d"), ATK);
                    outtextxy(990, 0, d);
                    wchar_t du[] = L"<";
                    outtextxy(1010, 0, du);
                    wchar_t aC[] = L"防禦等級";
                    outtextxy(1020, 0, aC);
                    _stprintf(d, _T("%d"), AC);
                    outtextxy(1090, 0, d);
                    EndBatchDraw();
                    Sleep(1000);
                }
            }
            else {
                cout << "射程不足" << endl;
                act[P_id]++;
                bullet[0]++;
            }
        }
        else if (chose == "a" && bullet[0] < 1) { cout << "彈藥不足" << endl; }
    }
    else if (chose == "a" && act[P_id] < 1) { cout << "行動力不足" << endl; 
    mciSendString(L"play ./Game/Sound/sgm/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
}
}
void DETECT(string chose, int id, int* blv, string* bname, string* bstory, int* bhp, int* bmhp) {
    if (chose == "d") {
        cout << "選擇目標" << endl << ">";
        cin >> id;
        id--;
        rows();
        cout << "LV " << blv[id] << " " << bname[id] << endl << bstory[id] << endl << "HP[" << bhp[id] << "/" << bmhp[id] << "]" << endl;
    }
}
void ITEM(string chose, int P_id, int* act, int* isize, string* iname, int* item, string* aname, int* bullet, int* mbullet, int* asize,wstring *Iname,int *abox1) {
    if (chose == "i" && act[P_id] > 0) {
        int i_id=-1;
        IMAGE itemblock;
        loadimage(&itemblock, L"./Game/picture/itemblock.png", 0, 0, false);
        putimage(200, 798, &itemblock);
        j = 0;
        for (i = 0; i < isize[P_id]; i++) {
            wchar_t* ptr = _wcsdup(Iname[i].c_str());
            outtextxy(210 + 100 * i, 812 + j * 49, ptr);
            TCHAR t[3];
            _stprintf(t, _T("%d"), item[i]);		// 高版本 VC 推荐使用 _stprintf_s 函数
            outtextxy(280 + 100 * i, 812 + j * 49, t);
            if (i % 4 == 0) {
                j++;
                i = 0;
            }
        }
        ExMessage m;
        int sss = 1;
        while (sss != 0)
        {
            // 获取一条鼠标消息
            m = getmessage(EM_MOUSE);
            switch (m.message) {

            case WM_LBUTTONDOWN://按鼠标左键分发事件
                for (j = 0; j < 3; j++) {
                    for (i = 0; i < 4; i++) {
                        if (m.x >= 200 + i * 100 && m.x <= 300 + i * 100 && m.y >= 798 + 49 * j && m.y <= 847 + 49 * j) {
                            i_id = i + 1 + j * 4;
                        }
                    }
                }
            case WM_RBUTTONUP:
                sss = 0;
            }
        }
        cout << "選擇要使用的物品" << endl;
        for (i = 0; i < isize[P_id]; i++) {
            cout << "[" << i + 1 << "]" << iname[i] << "(" << item[i] << ")" << " ";
        }
        cout << endl;
        if (i_id == 1) {
            cout << "要裝瑱在哪一把槍上" << endl;
            for (i = 0; i < asize[P_id]; i++) {
                IMAGE back1;
                loadimage(&back1, L"./Game/picture/attackblock.png", 0, 0, false);
                putimage(200, 799 - 48 * i-96, &back1);
            }
            for (i = 0; i < asize[P_id]; i++) {
                if (abox1[i] == 0) {
                    setbkmode(TRANSPARENT);
                    settextcolor(BLACK);
                    wchar_t s[] = L"栓動步槍(";
                    outtextxy(200, 815-48*i-96, s);
                    TCHAR t[3];
                    _stprintf(t, _T("%d"), bullet[P_id]);		// 高版本 VC 推荐使用 _stprintf_s 函数
                    outtextxy(267, 815 - 48 * i - 96, t);
                    wchar_t s2[] = L")";
                    outtextxy(290, 815 - 48 * i - 96, s2);
                }
            }
            int sss = 1; int cho=0;
            ExMessage m1;
            while (sss != 0)
            {
                // 获取一条鼠标消息
                m1 = getmessage(EM_MOUSE);
                switch (m1.message) {

                case WM_LBUTTONDOWN://按鼠标左键分发事件
                    for ( i= 0; i < asize[P_id]; i++) {
                        if (m1.x >= 200 + i * 100 && m1.x <= 300 + i * 100 && m1.y >= 798 - 49 * i - 98 && m1.y <= 847 - 49 * i - 98) {
                             cho = i + 1;
                        }
                    }
                case WM_RBUTTONUP:
                    sss = 0;
                }
            }            
            if (cho == 1 && bullet[0] < 5) {
                    item[cho - 1]--;
                    bullet[0] = mbullet[0];
                    act[P_id]--;
                }
            else if (cho == 1 && bullet[0] == 5) {
                cout << "武器裝填已滿" << endl;
                mciSendString(L"play ./Game/Sound/sgm/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
            }
        }
    }
    else if (chose == "i" && act[P_id] < 1) { cout << "行動力不足" << endl; 
    mciSendString(L"play ./Game/Sound/sgm/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
    }
}
void WALK(string chose, int* move, int P_id, int* x, int* y, int id, int* by, int* bx, int psize, int bsize) {
    if (chose == "w" && move[P_id] > 0) {
        int rx, ry,m,X,Y,range[15][20];
        BeginBatchDraw();
        IMAGE mb;
        loadimage(&mb, L"./Game/picture/mb.png", 0, 0, false);
        string walk;
        queue<pair<int, int>> q;
        clearQpair(q);
        q.push({ x[P_id],y[P_id] });
        for (i = 0; i < 15; i++) {
            for (j = 0; j < 20; j++) {
                range[i][j] = 0;
            }
        }
        X = q.front().first;
        Y = q.front().second;
        range[Y][X] = 1;
        while (1) {
            if (q.empty()) {
                break;
            }
            X = q.front().first;
            Y = q.front().second;
            if (X == 21) {
                X = X;
            }
            if ((abs(X - x[P_id]) + abs(Y - y[P_id] ))== move[P_id] ){
                break;
            }
            if (ss[X+1][Y] == 0 && range[Y][X+1] == 0&&X+1<=19) {
                range[Y][X + 1] = 1;
                q.push({ X + 1,Y });
                transparentimage(NULL, 48 * (X+1), 48 * Y, &mb);
            }
            if (ss[X - 1][Y] == 0 &&range[Y][X-1] ==0&&X-1>=0) {
                range[Y][X - 1] = 1;
                q.push({ X - 1,Y });
                transparentimage(NULL, 48 * (X-1), 48 * Y, &mb);
            }
            if (ss[X ][Y+1] == 0 && range[Y+1][X] == 0&&Y+1<=14) {
                range[Y + 1][X] = 1;
                q.push({ X ,Y+1 });
                transparentimage(NULL, 48 * X, 48 * (Y+1), &mb);
            }
            if (ss[X ][Y-1] == 0 && range[Y-1][X] == 0&&Y-1>=0) {
                range[Y - 1][X] = 1;
                q.push({ X ,Y-1 });
                transparentimage(NULL, 48 * X, 48 * (Y-1), &mb);
            }
            q.pop();
        }
        IMAGE p1,p2,p3,p4;
        loadimage(&p1, L"./Game/picture/p1.png", 0, 0, false);
        loadimage(&p2, L"./Game/picture/p2.png", 0, 0, false);
        loadimage(&p3, L"./Game/picture/p3.png", 0, 0, false);
        loadimage(&p4, L"./Game/picture/p4.png", 0, 0, false);
        transparentimage(NULL, (x[P_id]) * 48, ((y[P_id] - 1) * 48), &p1, 0xFF55FF);
        EndBatchDraw();       
        int sss = 1;
        while (sss != 0)
        {ExMessage m;
            // 获取一条鼠标消息
            m = getmessage(EM_MOUSE);
            switch (m.message) {

            case WM_LBUTTONDOWN://按鼠标左键分发事件
                for (j = 0; j < 20; j++) {
                    for (i = 0; i < 15; i++) {
                        if (m.x <= j * 48 + 48 && m.x >= j * 48&& m.y <= i * 48 + 48 && m.y >= i * 48&&ss[j][i]!=1) {                            
                                if (range[i][j]==1) {
                                    ss[x[P_id]][y[P_id]] = 0;
                                    int box[3000];
                                    bfs(x[P_id], y[P_id], j,i,box,move,P_id);
                                    int k, w = 0;
                                    int xbox=x[P_id], ybox=y[P_id];
                                    IMAGE map;
                                    loadimage(&map, L"./Game/picture/map01.png", 0, 0, false);
                                    while (1) {
                                        clearrectangle(0, 0, 960, 720);
                                        putimage(0, 0, &map);
                                        
                                        if (box[w] == 2) {
                                           
                                            for (k = 0; k <= 48; k+=16) {                    
                                              
                                                BeginBatchDraw();
                                                putimage(0, 0, &map);
                                                Bplayer(bx, by, bsize);
                                                transparentimage(NULL, xbox * 48, ((ybox - 1) * 48+k), &p1, 0xFF55FF);  
                                                EndBatchDraw();
                                                Sleep(30);
                                            }
                                            ybox++;
                                        }
                                        else if (box[w] == 4) {
                                            for (k = 0; k <= 48; k +=16) {
                                               
                                                BeginBatchDraw();
                                                putimage(0, 0, &map);
                                                Bplayer(bx, by, bsize);
                                            transparentimage(NULL, xbox * 48-k, ((ybox - 1) * 48), &p2, 0xFF55FF);
                                            EndBatchDraw();
                                            Sleep(30);
                                            }
                                            xbox--;
                                            
                                        }
                                        else if (box[w] == 6) {
                                            for (k = 0; k <=48; k +=16) {
                                               
                                                BeginBatchDraw();
                                                putimage(0, 0, &map);
                                                Bplayer(bx, by, bsize);
                                                transparentimage(NULL, xbox * 48 + k, ((ybox - 1) * 48), &p3, 0xFF55FF);
                                                EndBatchDraw();
                                                Sleep(30);
                                            }
                                            xbox++;
                                        }
                                        else if (box[w] == 8) {
                                            for (k = 0; k <= 48; k+=16 ) {
                                               
                                                BeginBatchDraw();
                                                putimage(0, 0, &map);
                                                Bplayer(bx, by, bsize);
                                                transparentimage(NULL, xbox * 48, ((ybox - 1) * 48) - k , &p4, 0xFF55FF);
                                                EndBatchDraw();
                                                Sleep(30);
                                            }
                                           
                                            ybox--;
                                        }
                                        else if (box[w] == 0) {
                                            break;
                                        }
                                        w++;
                                    }
                                    x[P_id] = j; y[P_id] = i;
                                    ss[x[P_id]][y[P_id]] = 1;
                                    j = 20; i = 15;
                                    sss = 0;
                                }
                                else {
                                    mciSendString(L"play ./Game/Sound/sgm/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
                                }
                        }
                    }
                }
            case WM_RBUTTONUP:
                sss = 0;	// 按鼠标右键退出程序

            }
        }
    }
    else if (chose == "w"&&move[P_id] == 0) {
        mciSendString(L"play ./Game/Sound/sgm/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
    }
}
void END(string chose, int& second, int* move, int& P_id, int* act, int& id, int psize, int* pturn, int bsize, int* bturn, int* speed, int* bspeed, int& roundp, int& roundb, int* bmove) {
    int t, pmax, bmax;
    if (chose == "e") {
        P_id = 0; id = 0; pmax = 0; bmax = 0;
        for (t = 0; t < 100; t++) {
            for (i = 0; i < psize; i++) {
                if (pturn[i] >= 100) {
                    if (pturn[pmax] < pturn[i] || pmax == i) {
                        pmax = i;
                    }
                }
            }
            for (i = 0; i < bsize; i++) {
                if (bturn[i] >= 100) {
                    if (bturn[bmax] < bturn[i] || bmax == i) {
                        bmax = i;
                    }
                }
            }
            if (bturn[bmax] > pturn[pmax] && bturn[bmax] >= 100) {
                id = bmax; bturn[bmax] -= 100;  roundp = 0; roundb = 1; bmove[id] = 8; break;
            }
            else if (pturn[pmax] >= bturn[bmax] && pturn[pmax] >= 100) {
                P_id = pmax;
                pturn[pmax] -= 100;
                move[P_id] = 6;
                act[P_id] = 1;
                roundp = 1; roundb = 0;
                break;
            }
            for (i = 0; i < psize; i++) {
                pturn[i] += speed[i];
            }
            for (i = 0; i < bsize; i++) {
                if (bspeed[i] > 0) {
                    bturn[i] += bspeed[i];
                }
            }
            second++;
        }
    }
}
void Precedence(int* pturn, int* bturn, int psize, int bsize, wstring* name, string* bname, int P_id, int* speed, int* bspeed) {
    cout << "現在的行動順序" << endl;
    int turn[3], max = 0, tt = 0, spd[3];
    for (i = 0; i < psize; i++) {
        turn[i] = pturn[i];
    }
    for (i = 0; i < bsize; i++) {
        turn[psize + i] = bturn[i];
    }
    for (i = 0; i < psize; i++) {
        spd[i] = speed[i];
    }for (i = 0; i < bsize; i++) {
        spd[psize + i] = bspeed[i];
    }
    while (1) {
        max = 0;
        for (i = 0; i < psize + bsize; i++) {
            if (spd[i] <= 0 && turn[i] != 2000000) {
                max = i; break;
            }
            else if ((100 - turn[i]) / spd[i] >= (100 - turn[max]) / spd[max] && turn[i] != 2000000) {
                max = i;
            }
        }
        if (max >= psize) {
            if (spd[max] <= 0) {
                cout << bname[max - psize] << "(已死亡)" << "-->";
                turn[max] = 2000000; tt++;
            }
            else {
                cout << bname[max - psize] << "(再" << (100 - turn[max]) / spd[max] << "秒)" << "-->";
                turn[max] = 2000000; tt++;
            }
        }
        else if (max < psize) {
            cout<< "(再" << (100 - turn[max]) / spd[max] << "秒)" << "-->";
            turn[max] = 20000000; tt++;
        }
        if (tt == psize + bsize) {
            Sleep(500);
            break;
        }
    }
}
void Target(int& P_id, int id, int* bx, int* by, int* x, int* y, int psize, string* bname, wstring* name) {
    int min = 1000;
    for (i = 0; i < psize; i++) {
        int rx = 0, ry = 0;
        ry = (by[id] - y[i]) * (by[id] > y[i]) + (by[id] - y[i]) * (by[id] < y[i]) * -1; rx = (bx[id] - x[i]) * (bx[id] > x[i]) + (bx[id] - x[i]) * (bx[id] < x[i]) * -1;
        if (rx + ry < min) {
            min = rx + ry;
            P_id = i;
        }
    }
    cout << bname[id] << "將"  << "視為目標" << endl;
}
void Bmove(int id, int P_id, int* bmove, int* bx, int* by, int* x, int* y) {
    int rx, ry, d;
    d = (by[id] - y[P_id]) * (by[id] > y[P_id]) + (by[id] - y[P_id]) * (by[id] < y[P_id]) * -1 + (bx[id] - x[P_id]) * (bx[id] > x[P_id]) + (bx[id] - x[P_id]) * (bx[id] < x[P_id]) * -1;
    while (bmove[id]--) {
        if (d > 1) {
            if (d > (by[id] - 1 - y[P_id]) * (by[id] - 1 > y[P_id]) + (by[id] - 1 - y[P_id]) * (by[id] - 1 < y[P_id]) * -1 + (bx[id] - x[P_id]) * (bx[id] > x[P_id]) + (bx[id] - x[P_id]) * (bx[id] < x[P_id]) * -1) {
                d = (by[id] - 1 - y[P_id]) * (by[id] - 1 > y[P_id]) + (by[id] - 1 - y[P_id]) * (by[id] - 1 < y[P_id]) * -1 + (bx[id] - x[P_id]) * (bx[id] > x[P_id]) + (bx[id] - x[P_id]) * (bx[id] < x[P_id]) * -1;
                by[id]--;
                ss[by[id]][bx[id]] = 2;
            }
            else if (d > (by[id] + 1 - y[P_id]) * (by[id] + 1 > y[P_id]) + (by[id] + 1 - y[P_id]) * (by[id] + 1 < y[P_id]) * -1 + (bx[id] - x[P_id]) * (bx[id] > x[P_id]) + (bx[id] - x[P_id]) * (bx[id] < x[P_id]) * -1) {
                d = (by[id] + 1 - y[P_id]) * (by[id] + 1 > y[P_id]) + (by[id] + 1 - y[P_id]) * (by[id] + 1 < y[P_id]) * -1 + (bx[id] - x[P_id]) * (bx[id] > x[P_id]) + (bx[id] - x[P_id]) * (bx[id] < x[P_id]) * -1;
                by[id]++;
                ss[by[id]][bx[id]] = 2;
            }
            else if (d > (by[id] - y[P_id]) * (by[id] > y[P_id]) + (by[id] - y[P_id]) * (by[id] < y[P_id]) * -1 + (bx[id] + 1 - x[P_id]) * (bx[id] + 1 > x[P_id]) + (bx[id] + 1 - x[P_id]) * (bx[id] + 1 < x[P_id]) * -1) {
                d = (by[id] - y[P_id]) * (by[id] > y[P_id]) + (by[id] - y[P_id]) * (by[id] < y[P_id]) * -1 + (bx[id] + 1 - x[P_id]) * (bx[id] + 1 > x[P_id]) + (bx[id] + 1 - x[P_id]) * (bx[id] + 1 < x[P_id]) * -1;
                bx[id]++;
                ss[by[id]][bx[id]] = 2;
            }
            else if (d > (by[id] - y[P_id]) * (by[id] > y[P_id]) + (by[id] - y[P_id]) * (by[id] < y[P_id]) * -1 + (bx[id] - 1 - x[P_id]) * (bx[id] - 1 > x[P_id]) + (bx[id] - 1 - x[P_id]) * (bx[id] - 1 < x[P_id]) * -1) {
                d = d > (by[id] - y[P_id]) * (by[id] > y[P_id]) + (by[id] - y[P_id]) * (by[id] < y[P_id]) * -1 + (bx[id] - 1 - x[P_id]) * (bx[id] - 1 > x[P_id]) + (bx[id] - 1 - x[P_id]) * (bx[id] - 1 < x[P_id]) * -1;
                bx[id]--;
                ss[by[id]][bx[id]] = 2;
            }
        }
    }
}
void BMove(int id, int P_id, int* bmove, int* bx, int* by, int* x, int* y,wstring* name,int bsize) {
    int box[1000],w=0,k,xbox,ybox;
    Bbfs(bx[id],by[id],x[P_id],y[P_id],box);
    xbox = bx[id]; ybox = by[id];
    IMAGE wolf1;
    loadimage(&wolf1, L"./Game/picture/wolf1.png", 0, 0, false);
    IMAGE wolf2;
    loadimage(&wolf2, L"./Game/picture/wolf2.png", 0, 0, false);
    IMAGE wolf3;
    loadimage(&wolf3, L"./Game/picture/wolf3.png", 0, 0, false);
    IMAGE wolf4;
    loadimage(&wolf4, L"./Game/picture/wolf4.png", 0, 0, false);
    IMAGE map;
    loadimage(&map, L"./Game/picture/map01.png", 0, 0, false);
    while (bmove[id]--) {
        clearrectangle(0, 0, 960, 720);
        putimage(0, 0, &map);
        
        if (box[w] == 2) {

            for (k = 0; k <= 48; k += 16) {

                BeginBatchDraw();
                putimage(0, 0, &map);
                Player(P_id,x,y,name);
                mbplayer(bx, by, id, bsize);
                transparentimage(NULL, xbox * 48, ((ybox - 1) * 48 + k), &wolf1, 0xFF55FF);
                EndBatchDraw();
                Sleep(30);
            }
            ybox++;
        }
        else if (box[w] == 4) {
            for (k = 0; k <= 48; k += 16) {

                BeginBatchDraw();
                putimage(0, 0, &map);
                Player(P_id, x, y, name);
                mbplayer(bx, by, id, bsize);
                transparentimage(NULL, xbox * 48 - k, ((ybox - 1) * 48), &wolf2, 0xFF55FF);
                EndBatchDraw();
                Sleep(30);
            }
            xbox--;

        }
        else if (box[w] == 6) {
            for (k = 0; k <= 48; k += 16) {

                BeginBatchDraw();
                putimage(0, 0, &map);
                Player(P_id, x, y, name);
                mbplayer(bx, by, id, bsize);
                transparentimage(NULL, xbox * 48 + k, ((ybox - 1) * 48), &wolf3, 0xFF55FF);
                EndBatchDraw();
                Sleep(30);
            }
            xbox++;
        }
        else if (box[w] == 8) {
            for (k = 0; k <= 48; k += 16) {
                BeginBatchDraw();
                
                putimage(0, 0, &map);
                Player(P_id, x, y, name);
                mbplayer(bx, by, id, bsize);
                transparentimage(NULL, xbox * 48, ((ybox - 1) * 48) - k, &wolf4, 0xFF55FF);
                EndBatchDraw();
                Sleep(30);
            }

            ybox--;
        }
        else if (box[w] == 0) {
            break;
        }
        w++;
}
    bx[id] = xbox; by[id] = ybox;
}
void Bmaps(int* y, int* x, int* bx, int* by, int P_id, int id, int psize, int bsize, string* mark, string* bmark) {
    for (j = 0; j < 15; j++) {
        for (i = 0; i < 20; i++) {
            s[j][i] = "。";
            if (ss[j][i] != 2) {
                ss[j][i] = 0;
            }
        }
    }
    for (i = 0; i < psize; i++) {
        ss[y[i]][x[i]] = 1;
        s[y[i]][x[i]] = mark[i];
    }
    for (i = 0; i < bsize; i++) {
        ss[by[i]][bx[i]] = 1;
        s[by[i]][bx[i]] = bmark[i];
    }

    for (j = 0; j < 15; j++) {
        for (i = 0; i < 20; i++) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
            for (int k = 0; k < psize; k++) {
                if (j == y[k] && i == x[k]) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }
            }
            for (int k = 0; k < bsize; k++) {
                if (j == by[k] && i == bx[k]) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }
            }
            if (ss[j][i] == 2) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | BACKGROUND_BLUE);
                ss[j][i] = 0;
            }
            cout << s[j][i];
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        cout << " " << j + 1;
        cout << endl;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    for (i = 0; i < 20; i++) {
        if (i < 9) {
            cout << i + 1 << " ";
        }
        else {
            cout << i + 1;
        }
    }
    cout << endl << endl;
}
void Battack(int id, string* aname, int* range, int P_id, int* dex, wstring* name, string* bname, string* admg, int* baid, int* by, int* bx, int* y, int* x, string* ahit, int* hp,wstring *Admg) {
    int rx, ry, ATK, AC;
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    ry = (by[id] - y[P_id]) * (by[id] > y[P_id]) + (by[id] - y[P_id]) * (by[id] < y[P_id]) * -1; rx = (bx[id] - x[P_id]) * (bx[id] > x[P_id]) + (bx[id] - x[P_id]) * (bx[id] < x[P_id]) * -1;
    if (range[baid[id]] >= rx + ry) {
        ATK = roll("", 2) + roll(ahit[baid[id]], 1);
        AC = roll("", 2) + (dex[P_id] - 10) / 2;
        if (ATK > AC) {
            cout << bname[id] << "用" << aname[baid[id]] << "攻擊"  << endl << "Atk" << "(1d20+" << ahit[baid[id]] << ")" << ATK << ">" << "Ac" << "(1d20+" << (dex[P_id] - 10) / 2 << ")" << AC << "=命中" << endl;
            int  DMG = roll(admg[baid[id]], 1);
            hp[P_id] -= DMG;
            cout << "造成" << admg[baid[id]] << "=" << DMG << "點傷害" << endl;
            TCHAR k[5];
            _stprintf(k, _T("%d"), -DMG);		// 高版本 VC 推荐使用 _stprintf_s 函数
            outtextxy(x[P_id] * 48 + 24, (y[P_id]-1) * 48-20, k);
            TCHAR d[5];
            wchar_t atk[] = L"命中";
            outtextxy(960, 0, atk);
            _stprintf(d, _T("%d"), ATK);
            outtextxy(990, 0, d);
            wchar_t du[] = L">";
            outtextxy(1010, 0, du);
            wchar_t aC[] = L"防禦等級";
            outtextxy(1020, 0, aC);
            _stprintf(d, _T("%d"), AC);
            outtextxy(1090, 0, d);
            wchar_t* ptr = _wcsdup(Admg[baid[id]].c_str());
            outtextxy(960, 30, ptr);
            wchar_t dnu[] = L"=";
            outtextxy(1020, 30, dnu);
            _stprintf(d, _T("%d"), DMG);
            outtextxy(1030, 30, d);
            IMAGE tri;
            loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
            transparentimage(NULL, (bx[id]) * 48, ((by[id] - 1) * 48), &tri, 0xFF55FF);
            EndBatchDraw();
            Sleep(1000);
        }
        else { 
        cout << bname[id] << "用" << aname[baid[id]] << "攻擊" << endl << "Atk" << "(1d20+" << ahit[baid[id]] << ")" << ATK << "<=" << "Ac" << "(1d20+" << (dex[P_id] - 10) / 2 << ")" << AC << "=未命中" << endl; 
        wchar_t w1[] = L"未命中";
        outtextxy(48 * x[P_id], 48 * (y[P_id]-1)-20, w1);
        TCHAR d[5];
        wchar_t atk[] = L"命中";
        outtextxy(960, 0, atk);
        _stprintf(d, _T("%d"), ATK);
        outtextxy(990, 0, d);
        wchar_t du[] = L"<";
        outtextxy(1010, 0, du);
        wchar_t aC[] = L"防禦等級";
        outtextxy(1020, 0, aC);
        _stprintf(d, _T("%d"), AC);
        outtextxy(1090, 0, d);
        IMAGE tri;
        loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
        transparentimage(NULL, (bx[id]) * 48, ((by[id] - 1) * 48), &tri, 0xFF55FF);
        EndBatchDraw();
        Sleep(1000);
        }
    }
}



int main() {    
    initgraph(1280, 960, EW_SHOWCONSOLE);
    HWND hWnd = GetHWnd();
    SetWindowText(hWnd,L"RPG");
    srand(time(NULL));
    mciSendString(L"open ./Game/Sound/bgm/魔王魂_ファンタジー12.mp3", NULL, 0, NULL);
    mciSendString(L"open ./Game/Sound/sgm/魔王魂_戦闘18.mp3", NULL, 0, NULL);
    mciSendString(L"open ./Game/Sound/sgm/魔王魂_効果音_ワンポイント33.mp3", NULL, 0, NULL);
    mciSendString(L"play ./Game/Sound/bgm/魔王魂_ファンタジー12.mp3 repeat", NULL, 0, NULL);
    int x[1], y[1], bx[2], by[2], hp[1], mhp[1], str[1], dex[1], con[1], Int[1], wis[1], cha[1], id, P_id, dmg = 0, lv[1], second, range[2], move[1], item[1], isize[1], i_id, bullet[1], mbullet[1], load = 0;
    player pl(L"夏洛特", L"主人公", 1, 10, 10, 10, 6, 1, 1, 10, 10, 1, 10, 0, 1);
    enemy  b1(L"野狼1",L"團體行動的動物 隨著數量增加危險性也會大幅上升",1,11,11,15,12,1,3,14,0,8,1);
    enemy  b2(L"野狼1", L"團體行動的動物 隨著數量增加危險性也會大幅上升", 1, 11, 11, 15, 12, 11, 11, 14, 0, 8, 1);
    int ix, iy, n,abox1[1];
    int act[1], asize[1], bhp[2], bmhp[2], bdex[2], blv[2], pturn[1], bturn[2], t, speed[1], bspeed[2], psize = 1, bsize = 2, roundp = 0, roundb = 0, bmove[2], baid[2], bstr[2];
    string  chose = "e", walk, attack, story[2], iname[1], aname[2], bstory[2], bname[2], mark[1], bmark[2], admg[2], ahit[2];
    string a, b = ".txt", read = "";
    wstring name[2],Admg[2],Bname[2],Iname[1];
    mark[0] = "P "; name[0] = L"夏洛特"; lv[0] = 1; story[0] = "主人公"; mhp[0] = 10; hp[0] = 10; dex[0] = 10; move[0] = 6, isize[0] = 1; act[0] = 1; asize[0] = 1; x[0] = 10; y[0] = 10; speed[0] = 10; pturn[0] = 0; abox1[0] = 0;/*主角*/
    
    bmark[0] = "1 "; bname[0] = "野狼1"; Bname[0] = L"野狼1"; blv[0] = 1; bstory[0] = "團體行動的動物 隨著數量增加危險性也會大幅上升"; bmhp[0] = 11; bhp[0] = 11; bdex[0] = 15; bstr[0] = 12; bx[0] = 1; by[0] = 3; bspeed[0] = 14; bturn[0] = 0; bmove[0] = 8; baid[0] = 1;
    bmark[1] = "2 "; bname[1] = "野狼2"; Bname[1] = L"野狼2"; blv[1] = 1; bstory[1] = "團體行動的動物 隨著數量增加危險性也會大幅上升"; bmhp[1] = 11; bhp[1] = 11; bdex[1] = 15; bstr[1] = 12; bx[1] = 11; by[1] = 11; bspeed[1] = 14; bturn[1] = 0; bmove[1] = 8; baid[1] = 1;
    /*敵人*/
    aname[0] = "栓動步槍"; mbullet[0] = 5; bullet[0] = mbullet[0]; range[0] = 5; admg[0] = "2d8"; Admg[0] =L"2d8";
    aname[1] = "啃咬"; range[1] = 1; admg[1] = "2d4+2"; ahit[1] = "1d4+2"; Admg[1] = L"2d4+2";
    /*武器*/
    iname[0] = "通常彈"; Iname[0] = L"通常彈"; item[0] = 30;/*道具*/
    second = 0; P_id = 0; id = 0;
    time_t first, two,three=0;
    cout << "開始戰鬥" << endl;
    END(chose, second, move, P_id, act, id, psize, pturn, bsize, bturn, speed, bspeed, roundp, roundb, bmove);
    first = time(NULL);
    while (1) {
        BeginBatchDraw();
        if (roundp == 1) {
            rows();/*分割線*/
            cout << "第" << second << "秒" << endl;
            Precedence(pturn, bturn, psize, bsize, name, bname, P_id, speed, bspeed);
            maps(y, x, bx, by, P_id, id, psize, bsize, mark, bmark,hp,mhp,bname);/*地圖繪製*/
            Player(P_id, x, y,name);
            Bplayer(bx, by, bsize);
            EndBatchDraw();
            acts(x[P_id], y[P_id], move[P_id], act[P_id], chose, P_id, name, hp, mhp,bsize,id,bx,by,Bname,bhp,bmhp);/*選項*/
            ATTACK(chose, act, bullet, mbullet, P_id, y, x, dex, name, id, bx, by, bmhp, bhp, bdex, bname, range, bspeed, bsize,asize,abox1,admg,Admg);/*攻擊*/
            DETECT(chose, id, blv, bname, bstory, bhp, bmhp);/*偵查*/
            ITEM(chose, P_id, act, isize, iname, item, aname, bullet, mbullet, asize,Iname,abox1);/*物品*/
            WALK(chose, move, P_id, x, y, id, by, bx, psize, bsize);/*移動*/
            END(chose, second, move, P_id, act, id, psize, pturn, bsize, bturn, speed, bspeed, roundp, roundb, bmove);/*結束行動*/
            if (chose == "s") {           
                int ssss = 1; j = 0;
                while (ssss != 0) {
                    int sss = 1;
                    IMAGE saveblock;
                    IMAGE rarrow;
                    IMAGE larrow; TCHAR d[5];
                    cleardevice();
                    BeginBatchDraw();
                    for (i = 0; i < 7; i++) {
                        loadimage(&saveblock, L"./Game/picture/saveblock.png", 0, 0, false);
                        putimage(100, 30 + 128 * i, &saveblock); _stprintf(d, _T("%d"), i + 1+j*14); outtextxy(110, 40 + 128 * i, d);
                        putimage(700, 30 + 128 * i, &saveblock); _stprintf(d, _T("%d"), i + 7 + 1+j*14); outtextxy(710, 40 + 128 * i, d);
                    }
                    loadimage(&rarrow, L"./Game/picture/rarrow.png", 0, 0, false);
                    transparentimage(NULL, 0, 430, &rarrow, 0xFF55FF);
                    loadimage(&larrow, L"./Game/picture/larrow.png", 0, 0, false);
                    transparentimage(NULL, 1230, 430, &larrow, 0xFF55FF);
                    for(i=0;i<7;i++){  
                        a = std::to_string(i + 1+j*14);
                    string file = "./Game/save/" + a + b;
                    std::ifstream ifs(file, std::ios::in);
                    if (ifs.is_open()) {                   
                        std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
                        string INT="";
                        for (int J = 1; J < str.size(); J++) {
                            if (str[J] =='\n') {
                                break;
                            }
                            INT += str[J];
                        }
                        int sto;
                        sto=stoi(INT);
                        int hr=0, min=0, sec=0;
                        for (int T = 0; T < sto; T++) {
                            sec++;
                            if (sec == 60) {
                                min++;
                                sec = 0;
                            }
                            if (min == 60) {
                                hr++;
                                min = 0;
                            }
                        }
                        _stprintf(d, _T("%d"), hr); outtextxy(110 , 60 + 128 * i, d);
                        wchar_t s1[] = L":";
                        outtextxy(130, 60 + 128 * i, s1);
                        _stprintf(d, _T("%d"), min); outtextxy(140, 60 + 128 * i, d);
                        outtextxy(160, 60 + 128 * i, s1);
                        _stprintf(d, _T("%d"), sec); outtextxy(170, 60 + 128 * i, d);
                    }
                    
                    }
                    for (i = 0; i < 7; i++) {
                        a = std::to_string(i + 8+j*14);
                    string file = "./Game/save/" + a + b;
                    std::ifstream ifs(file, std::ios::in);
                    if (ifs.is_open()) {
                        std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
                        string INT = "";
                        for (int J = 1; J < str.size(); J++) {
                            if (str[J] == '\n') {
                                break;
                            }
                            INT += str[J];
                        }
                        int sto;
                        sto = stoi(INT);
                        int hr = 0, min = 0, sec = 0;
                        for (int T = 0; T < sto; T++) {
                            sec++;
                            if (sec == 60) {
                                min++;
                                sec = 0;
                            }
                            if (min == 60) {
                                hr++;
                                min = 0;
                            }
                        }
                        _stprintf(d, _T("%d"), hr); outtextxy(710, 60 + 128 * i, d);
                        wchar_t s1[] = L":";
                        outtextxy(730, 60 + 128 * i, s1);
                        _stprintf(d, _T("%d"), min); outtextxy(740, 60 + 128 * i, d);
                        outtextxy(760, 60 + 128 * i, s1);
                        _stprintf(d, _T("%d"), sec); outtextxy(770, 60 + 128 * i, d);
                    }
                    }
                    EndBatchDraw();
                    while (sss != 0) {
                        ExMessage m;
                        m = getmessage(EM_MOUSE);
                        switch (m.message) {
                        case WM_LBUTTONDOWN:
                            for (i = 0; i < 7; i++) {
                                if (m.x >= 100 && m.x <= 600 && m.y >= 30 + 128 * i && m.y <= 30 + 128 * i + 98) {
                                    a = std::to_string(i + 1 + j * 14); sss = 0; ssss = 0;
                                }
                                if (m.x >= 700 && m.x <= 1200 && m.y >= 30 + 128 * i && m.y <= 30 + 128 * i + 98) {
                                    a = std::to_string(i + 8 + j * 14); sss = 0; ssss = 0;
                                }
                            }
                            if (m.x >= 0 && m.x <=48 && m.y >= 430 && m.y <= 478) {
                                if (j > 0) {
                                    j--; sss = 0;
                                }
                                else { sss = 0; mciSendString(L"play ./Game/Sound/sgm/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
                                }
                            }
                            if (m.x >= 1230 && m.x <= 1278 && m.y >= 430 && m.y <=478) {
                                j++; sss = 0;
                            }
                        case WM_RBUTTONDOWN:
                            if (m.rbutton) {
                                sss = 0; ssss = 0;
                            }
                        }
                    }
                }
                two = time(NULL);
                cout << "請命名你的存檔" << endl << ">";
                string file = "./Game/save/" + a + b;
                cout << file << endl;
                ofstream fout(file);
                if (fout) {
                    fout <<"="<< two - first +three<< endl;
                    for (i = 0; i < sizeof(x) / sizeof(x[0]); i++) {
                        fout << "x["<<i<<"]=" << x[i] << endl;
                    }
                    for (i = 0; i < sizeof(y) / sizeof(y[0]); i++) {
                        fout << "x[" << i << "]=" << y[i] << endl;
                    }
                    for (i = 0; i < sizeof(bx) / sizeof(bx[0]); i++) {
                        fout << "bx[" << i << "]=" << bx[i] << endl;
                    }
                    for (i = 0; i < sizeof(by) / sizeof(by[0]); i++) {
                        fout << "by[" << i << "]=" << by[i] << endl;
                    }
                    for (i = 0; i < sizeof(hp) / sizeof(hp[0]); i++) {
                        fout << "hp[" << i << "]=" << hp[i] << endl;
                    }
                    for (i = 0; i < sizeof(mhp) / sizeof(mhp[0]); i++) {
                        fout << "mhp[" << i << "]=" << mhp[i] << endl;
                    }
                    for (i = 0; i < sizeof(dex) / sizeof(dex[0]); i++) {
                        fout << "dex[" << i << "]=" << dex[i] << endl;
                    }
                    for (i = 0; i < sizeof(move) / sizeof(move[0]); i++) {
                        fout << "move[" << i << "]=" << move[i] << endl;
                    }
                    for (i = 0; i < sizeof(isize) / sizeof(isize[0]); i++) {
                        fout << "isize[" << i << "]=" << isize[i] << endl;
                    }
                    for (i = 0; i < sizeof(act) / sizeof(act[0]); i++) {
                        fout << "act[" << i << "]=" << act[i] << endl;
                    }
                    for (i = 0; i < sizeof(asize) / sizeof(asize[0]); i++) {
                        fout << "asize[" << i << "]=" << asize[i] << endl;
                    }
                    for (i = 0; i < sizeof(lv) / sizeof(lv[0]); i++) {
                        fout << "lv[" << i << "]=" << lv[i] << endl;
                    }
                    for (i = 0; i < sizeof(blv) / sizeof(blv[0]); i++) {
                        fout << "blv[" << i << "]=" << blv[i] << endl;
                    }
                    for (i = 0; i < sizeof(bmhp) / sizeof(bmhp[0]); i++) {
                        fout << "bmhp[" << i << "]=" << bmhp[i] << endl;
                    }
                    for (i = 0; i < sizeof(bhp) / sizeof(bhp[0]); i++) {
                        fout << "bhp[" << i << "]=" << bhp[i] << endl;
                    }
                    for (i = 0; i < sizeof(bdex) / sizeof(bdex[0]); i++) {
                        fout << "bdex[" << i << "]=" << bdex[i] << endl;
                    }
                    for (i = 0; i < sizeof(bx) / sizeof(bx[0]); i++) {
                        fout << "bx[" << i << "]=" << bx[i] << endl;
                    }
                    for (i = 0; i < sizeof(by) / sizeof(by[0]); i++) {
                        fout << "by[" << i << "]=" << by[i] << endl;
                    }
                    for (i = 0; i < sizeof(mbullet) / sizeof(mbullet[0]); i++) {
                        fout << "mbullet[" << i << "]=" << mbullet[i] << endl;
                    }
                    for (i = 0; i < sizeof(bullet) / sizeof(bullet[0]); i++) {
                        fout << "bullet[" << i << "]=" << bullet[i] << endl;
                    }
                    for (i = 0; i < sizeof(range) / sizeof(range[0]); i++) {
                        fout << "range[" << i << "]=" <<range[i] << endl;
                    }
                    for (i = 0; i < sizeof(item) / sizeof(item[0]); i++) {
                        fout << "item[" << i << "]=" << item[i] << endl;
                    }
                    fout << "second=" << second << endl;/*回合數int變數*/
                    for (i = 0; i < sizeof(pturn) / sizeof(pturn[0]); i++) {
                        fout << "pturn[" << i << "]=" << pturn[i] << endl;
                    }
                    for (i = 0; i < sizeof(bturn) / sizeof(bturn[0]); i++) {
                        fout << "bturn[" << i << "]=" << bturn[i] << endl;
                    }
                    for (i = 0; i < sizeof(speed) / sizeof(speed[0]); i++) {
                        fout << "speed[" << i << "]=" << speed[i] << endl;
                    }
                    for (i = 0; i < sizeof(bspeed) / sizeof(bspeed[0]); i++) {
                        fout << "bspeed[" << i << "]=" << bspeed[i] << endl;
                    }
                    fout << "psize=" << psize << endl; 
                    fout << "bsize=" << bsize << endl; 
                    fout << "roundp=" << roundp << endl;
                    fout << "roundb=" << roundb << endl;
                    for (i = 0; i < sizeof(bmove) / sizeof(bmove[0]); i++) {
                        fout << "bmove[" << i << "]=" << bmove[i] << endl;
                    }
                    for (i = 0; i < sizeof(baid) / sizeof(baid[0]); i++) {
                        fout << "baid[" << i << "]=" << baid[i] << endl;
                    }
                    for (i = 0; i < sizeof(bstr) / sizeof(bstr[0]); i++) {
                        fout << "bstr[" << i << "]=" << bstr[i] << endl;
                    }
                }
            }
            /*存檔*/
            j = 0;
            if (chose == "l") {
                int ssss = 1; j = 0;
                while (ssss != 0) {
                    int sss = 1;
                    IMAGE saveblock;
                    IMAGE rarrow;
                    IMAGE larrow; TCHAR d[5];
                    cleardevice();
                    BeginBatchDraw();
                    for (i = 0; i < 7; i++) {
                        loadimage(&saveblock, L"./Game/picture/saveblock.png", 0, 0, false);
                        putimage(100, 30 + 128 * i, &saveblock); _stprintf(d, _T("%d"), i + 1 + j * 14); outtextxy(110, 40 + 128 * i, d);
                        putimage(700, 30 + 128 * i, &saveblock); _stprintf(d, _T("%d"), i + 7 + 1 + j * 14); outtextxy(710, 40 + 128 * i, d);
                    }
                    loadimage(&rarrow, L"./Game/picture/rarrow.png", 0, 0, false);
                    transparentimage(NULL, 0, 430, &rarrow, 0xFF55FF);
                    loadimage(&larrow, L"./Game/picture/larrow.png", 0, 0, false);
                    transparentimage(NULL, 1230, 430, &larrow, 0xFF55FF);
                    for (i = 0; i < 7; i++) {
                        a = std::to_string(i + 1 + j * 14);
                        string file = "./Game/save/" + a + b;
                        std::ifstream ifs(file, std::ios::in);
                        if (ifs.is_open()) {
                            std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
                            string INT = "";
                            for (int J = 1; J < str.size(); J++) {
                                if (str[J] == '\n') {
                                    break;
                                }
                                INT += str[J];
                            }
                            int sto;
                            sto = stoi(INT);
                            int hr = 0, min = 0, sec = 0;
                            for (int T = 0; T < sto; T++) {
                                sec++;
                                if (sec == 60) {
                                    min++;
                                    sec = 0;
                                }
                                if (min == 60) {
                                    hr++;
                                    min = 0;
                                }
                            }
                            _stprintf(d, _T("%d"), hr); outtextxy(110, 60 + 128 * i, d);
                            wchar_t s1[] = L":";
                            outtextxy(130, 60 + 128 * i, s1);
                            _stprintf(d, _T("%d"), min); outtextxy(140, 60 + 128 * i, d);
                            outtextxy(160, 60 + 128 * i, s1);
                            _stprintf(d, _T("%d"), sec); outtextxy(170, 60 + 128 * i, d);
                        }

                    }
                    for (i = 0; i < 7; i++) {
                        a = std::to_string(i + 8 + j * 14);
                        string file = "./Game/save/" + a + b;
                        std::ifstream ifs(file, std::ios::in);
                        if (ifs.is_open()) {
                            std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
                            string INT = "";
                            for (int J = 1; J < str.size(); J++) {
                                if (str[J] == '\n') {
                                    break;
                                }
                                INT += str[J];
                            }
                            int sto;
                            sto = stoi(INT);
                            int hr = 0, min = 0, sec = 0;
                            for (int T = 0; T < sto; T++) {
                                sec++;
                                if (sec == 60) {
                                    min++;
                                    sec = 0;
                                }
                                if (min == 60) {
                                    hr++;
                                    min = 0;
                                }
                            }
                            _stprintf(d, _T("%d"), hr); outtextxy(710, 60 + 128 * i, d);
                            wchar_t s1[] = L":";
                            outtextxy(730, 60 + 128 * i, s1);
                            _stprintf(d, _T("%d"), min); outtextxy(740, 60 + 128 * i, d);
                            outtextxy(760, 60 + 128 * i, s1);
                            _stprintf(d, _T("%d"), sec); outtextxy(770, 60 + 128 * i, d);
                        }
                    }
                    EndBatchDraw();
                    while (sss != 0) {
                        ExMessage m;
                        m = getmessage(EM_MOUSE);
                        switch (m.message) {
                        case WM_LBUTTONDOWN:
                            for (i = 0; i < 7; i++) {
                                if (m.x >= 100 && m.x <= 600 && m.y >= 30 + 128 * i && m.y <= 30 + 128 * i + 98) {
                                    a = std::to_string(i + 1 + j * 14); sss = 0; ssss = 0;
                                }
                                if (m.x >= 700 && m.x <= 1200 && m.y >= 30 + 128 * i && m.y <= 30 + 128 * i + 98) {
                                    a = std::to_string(i + 8 + j * 14); sss = 0; ssss = 0;
                                }
                            }
                            if (m.x >= 0 && m.x <= 48 && m.y >= 430 && m.y <= 478) {
                                if (j > 0) {
                                    j--; sss = 0;
                                }
                                else {
                                    sss = 0; mciSendString(L"play ./Game/Sound/sgm/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
                                }
                            }
                            if (m.x >= 1230 && m.x <= 1278 && m.y >= 430 && m.y <= 478) {
                                j++; sss = 0;
                            }
                        case WM_RBUTTONDOWN:
                            if (m.rbutton) {
                                sss = 0; ssss = 0;
                                a = "";
                            }
                        }
                    }
                }
                IMAGE saveblock;
                loadimage(&saveblock, L"./Game/picture/saveblock.png", 0, 0, false);
                int LOAD = 0;
                cout << "要讀取哪個檔案" << endl;
                string file = "./Game/save/" + a + b;
                std::ifstream ifs(file, std::ios::in);
                if (!ifs.is_open()&&a!="") {
                    putimage(400,400, &saveblock);
                    wchar_t s2[] = L"無此存檔";
                    outtextxy(610, 440, s2);
                    Sleep(1000);
                    cout << "無此存檔" << endl;
                }
                else if (a=="") {

                }
                else {
                    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
                    int L[49];
                    for (i = 0; i < 49; i++) {
                        L[i] = 0;
                    }
                    int R = -1;
                    for (i = 0; i < str.size(); i++) {
                        if (str[i] == '=' && load != 1) {
                            load = 1;
                            read = "";
                            R++;
                        }
                        if (str[i] == '\n') {
                            int ten = 1;
                            for (j = 0; j < read.size(); j++) {
                                ten *= 10;
                            }
                            for (j = read.size(); j > 0; j--) {
                                ten /= 10;
                                L[R] += (read[read.size() - j] - 48) * ten;
                            }
                            load = 0;
                        }
                        if (load == 1 && str[i] != '=') {
                            read += str[i];
                        }
                    }
                    R = 0;
                    three = L[R]; R++;
                    for (i = 0; i < sizeof(x) / sizeof(x[0]); i++) {
                        x[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(y) / sizeof(y[0]); i++) {
                        y[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(bx) / sizeof(bx[0]); i++) {
                        bx[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(by) / sizeof(by[0]); i++) {
                        by[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(hp) / sizeof(hp[0]); i++) {
                        hp[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(mhp) / sizeof(mhp[0]); i++) {
                        mhp[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(dex) / sizeof(dex[0]); i++) {
                        dex[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(move) / sizeof(move[0]); i++) {
                        move[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(isize) / sizeof(isize[0]); i++) {
                        isize[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(act) / sizeof(act[0]); i++) {
                        act[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(asize) / sizeof(asize[0]); i++) {
                        asize[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(lv) / sizeof(lv[0]); i++) {
                        lv[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(blv) / sizeof(blv[0]); i++) {
                        blv[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(bmhp) / sizeof(bmhp[0]); i++) {
                        bmhp[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(bhp) / sizeof(bhp[0]); i++) {
                        bhp[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(bdex) / sizeof(bdex[0]); i++) {
                        bdex[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(bx) / sizeof(bx[0]); i++) {
                        bx[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(by) / sizeof(by[0]); i++) {
                        by[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(mbullet) / sizeof(mbullet[0]); i++) {
                        mbullet[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(bullet) / sizeof(bullet[0]); i++) {
                        bullet[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(range) / sizeof(range[0]); i++) {
                        range[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(item) / sizeof(item[0]); i++) {
                        item[i] = L[R]; R++;
                    }
                    second = L[R]; R++;
                    for (i = 0; i < sizeof(pturn) / sizeof(pturn[0]); i++) {
                        pturn[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(bturn) / sizeof(bturn[0]); i++) {
                        bturn[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(speed) / sizeof(speed[0]); i++) {
                        speed[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(bspeed) / sizeof(bspeed[0]); i++) {
                        bspeed[i] = L[R]; R++;
                    }
                    psize = L[R]; R++;
                    bsize = L[R]; R++;
                    roundp = L[R]; R++;
                    roundb = L[R]; R++;
                    for (i = 0; i < sizeof(bmove) / sizeof(bmove[0]); i++) {
                        bmove[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(baid) / sizeof(baid[0]); i++) {
                        baid[i] = L[R]; R++;
                    }
                    for (i = 0; i < sizeof(bstr) / sizeof(bstr[0]); i++) {
                        bstr[i] = L[R]; R++;
                    }
                    ifs.close();
                    first = time(NULL);
                }
            }
            /*讀檔*/
        }
        else if (roundb == 1) {
            rows();
            cout << "第" << second << "秒" << endl;
            Target(P_id, id, bx, by, x, y, psize, bname, name); 
            maps(y, x, bx, by, P_id, id, psize, bsize, mark, bmark, hp, mhp,bname);
            BMove(id, P_id, bmove, bx, by, x, y,name,bsize);
            Player(P_id, x, y,name);
            Bplayer(bx, by, bsize);
            Battack(id, aname, range, P_id, dex, name, bname, admg, baid, by, bx, y, x, ahit, hp,Admg);
            END(chose, second, move, P_id, act, id, psize, pturn, bsize, bturn, speed, bspeed, roundp, roundb, bmove);/*結束行動*/
        }
    }
}
