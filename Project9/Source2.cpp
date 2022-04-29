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
int i, j, ss[200][150];
string s[26][60];
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
class player{
public: 
    wstring name, story;
    int lv ,mhp,hp,dex,move,isize,asize,x,y,turn,speed,abox,act,pose;
    player() {};   

};
class enemy {
public:
    wstring name, story;
    int lv, mhp,hp,dex, str,x,y,speed,turn,move,baid,pose;
    enemy() {};
   
};
class arms {
public:
    wstring name,Dmg;
    string  dmg, hit;
    int range,mbullet, bullet;
    arms() {};
};
class item {
public:
    wstring Name;
    string name;
    int number;
    item() {};
};    
void p_put(player *p,int psize) {
    IMAGE p1, p2, p3, p4;
    loadimage(&p1, L"./Game/picture/p1.png", 0, 0, false);
    loadimage(&p2, L"./Game/picture/p2.png", 0, 0, false); 
    loadimage(&p3, L"./Game/picture/p3.png", 0, 0, false);
    loadimage(&p4, L"./Game/picture/p4.png", 0, 0, false);
        for (int I = 0; I < psize; I++) {
            IMAGE c;
            loadimage(&c, L"./Game/picture/c.png", 0, 0, false);
            transparentimage(NULL, 0, 730, &c, 0xFF55FF);
            wchar_t* ptr = _wcsdup(p[I].name.c_str());
            outtextxy(60, 721, ptr);
            if (p[I].pose == 1) {                
                transparentimage(NULL, (p[I].x) * 48, ((p[I].y - 1) * 48), &p1, 0xFF55FF);
            }
            else if (p[I].pose == 2) {                
                transparentimage(NULL, (p[I].x) * 48, ((p[I].y - 1) * 48), &p2, 0xFF55FF);
            }
            else if (p[I].pose == 3) {              
                transparentimage(NULL, (p[I].x) * 48, ((p[I].y - 1) * 48), &p3, 0xFF55FF);
            }
            else if (p[I].pose == 4) {               
                transparentimage(NULL, (p[I].x) * 48, ((p[I].y - 1) * 48), &p4, 0xFF55FF);
            }
        }
    }
void e_put(enemy *e,int bsize) {
    IMAGE b1,b2,b3,b4;
    loadimage(&b1, L"./Game/picture/wolf1.png", 0, 0, false);
    loadimage(&b2, L"./Game/picture/wolf2.png", 0, 0, false);
    loadimage(&b3, L"./Game/picture/wolf3.png", 0, 0, false);
    loadimage(&b4, L"./Game/picture/wolf4.png", 0, 0, false);
        for (int I = 0; I < bsize; I++) {
            if (e[I].pose == 1) {
                transparentimage(NULL, (e[I].x) * 48, ((e[I].y) * 48), &b1, 0xFF55FF);
            }
            else if (e[I].pose == 2) {
                transparentimage(NULL, (e[I].x) * 48, ((e[I].y) * 48), &b2, 0xFF55FF);
            }
            else if (e[I].pose == 3) {
                transparentimage(NULL, (e[I].x) * 48, ((e[I].y) * 48), &b3, 0xFF55FF);
            }
            else if (e[I].pose == 4) {
                transparentimage(NULL, (e[I].x) * 48, ((e[I].y) * 48), &b4, 0xFF55FF);
            }
        }
    }
void e_bput(enemy *e, int id, int bsize) {
        for (int I = 0; I < bsize; I++) {
            if (I != id) {
                IMAGE bplayer;
                loadimage(&bplayer, L"./Game/picture/wolf1.png", 0, 0, false);
                transparentimage(NULL, (e[I].x) * 48, ((e[I].y) * 48), &bplayer, 0xFF55FF);
            }
        }
    }
void maps(player *p, int P_id,enemy *e,int psize,int bsize) {
    IMAGE text;
    loadimage(&text, L"./Game/picture/text.png", 0, 0, false);
    IMAGE aline;
    loadimage(&aline, L"./Game/picture/aline.png", 0, 0, false);
    IMAGE back2;
    loadimage(&back2, L"./Game/picture/background.png", 0, 0, false);
    putimage(0, 720, &back2); 
    IMAGE map01;
    loadimage(&map01, L"./Game/picture/map01.png",0,0,false);
    putimage(0, 0, &map01);
    putimage(960, 0, &text);
    putimage(1025, 200, &aline);

    for (i = 0; i < 20; i++) {
        for (j = 0; j < 15; j++) {
            ss[i][j] = 0;
        }
    }
    for (i = 0; i < psize; i++) {
        ss[p[i].x][p[i].y] = 1;
    }
    for (i = 0; i < bsize; i++) {
        ss[e[i].x][e[i].y] = 1;
    }
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    wchar_t s[] = L"HP[";
    outtextxy(30, 875, s);
    TCHAR t[5];
    _stprintf(t, _T("%d"),p[P_id].hp);		// 高版本 VC 推荐使用 _stprintf_s 函数
    outtextxy(55, 875, t);
    wchar_t s1[] = L"/";
    outtextxy(90, 875, s1);
    _stprintf(t, _T("%d"), p[P_id].mhp);		// 高版本 VC 推荐使用 _stprintf_s 函数
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
void bfs(int sx, int sy, int x, int y, int* box, int &move, int P_id) {
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
            move -= w;
            break;
        }
        w++;
    }
}
void p_attack(player *p,enemy *e,arms *ar,int P_id,int id,string &chose,int bsize,int psize) {
    int ax, ay;
   
        int sss = 1;
        ExMessage m, m1; string attack;
        if (chose == "a" && p[P_id].act > 0) {
            for (i = 0; i < p[P_id].asize; i++) {
                IMAGE back1;
                loadimage(&back1, L"./Game/picture/attackblock.png", 0, 0, false);
                putimage(200, 799 + 48 * i, &back1);
            }
            for (i = 0; i < p[P_id].asize; i++) {
                if (p[P_id].abox == 0) {
                    setbkmode(TRANSPARENT);
                    settextcolor(BLACK);
                    wchar_t s[] = L"栓動步槍(";
                    outtextxy(200, 815, s);
                    TCHAR t[3];
                    _stprintf(t, _T("%d"), ar[P_id].bullet);		// 高版本 VC 推荐使用 _stprintf_s 函数
                    outtextxy(267, 815, t);
                    wchar_t s2[] = L")";
                    outtextxy(290, 815, s2);
                }
            }
            while (sss != 0)//選武器
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
            if (attack == "a" && ar[0].bullet > 0) {
                BeginBatchDraw();
                for (i = 0; i < 20; i++) {
                    for (j = 0; j < 15; j++) {
                        rx = 0; ry = 0;
                        ry = (p[P_id].y - j) * (p[P_id].y > j) + (p[P_id].y - j) * (p[P_id].y < j) * -1; rx = (p[P_id].x - i) * (p[P_id].x > i) + (p[P_id].x - i) * (p[P_id].x < i) * -1;
                        if (ry + rx <= ar[0].range && rx + ry > 0) {
                            IMAGE mb;
                            loadimage(&mb, L"./Game/picture/ab.png", 0, 0, false);
                            transparentimage(NULL, 48 * i, 48 * j, &mb);
                        }
                    }
                }
                p_put(p, psize);
                EndBatchDraw();
                for (j = 0; j < 15; j++) {
                    for (i = 0; i < 20; i++) {
                        for (int k = 0; k < bsize; k++) {
                            if (j == e[k].y && i == e[k].x) {
                            }
                        }
                        if (j == p[P_id].y && i == p[P_id].x) {
                        }
                        rx = 0; ry = 0;
                        ry = (p[P_id].y - j) * (p[P_id].y > j) + (p[P_id].y - j) * (p[P_id].y < j) * -1; rx = (p[P_id].y - i) * (p[P_id].x > i) + (p[P_id].x - i) * (p[P_id].x < i) * -1;
                        if (ry + rx <= ar[0].range && rx + ry > 0) {
                        }
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
                                if (m1.x >= 48 * i && m1.x <= 48 * i + 48 && m1.y >= 48 * j && m1.y <= 48 * j + 48) {
                                    for (int k = 0; k < bsize; k++) {
                                        if (ss[e[k].x][e[k].y] == 1 && e[k].x == i && e[k].y == j) {
                                            id = k; sss = 0;
                                        }
                                    }
                                }
                            }
                        }
                    case WM_RBUTTONDOWN:
                        if (m1.rbutton == true) {
                            return;
                        }
                    }
                }
                ry = (p[P_id].y - e[id].y) * (p[P_id].y > e[id].y) + (p[P_id].y - e[id].y) * (p[P_id].y < e[id].y) * -1; rx = (p[P_id].x - e[id].x) * (p[P_id].x > e[id].x) + (p[P_id].x - e[id].x) * (p[P_id].x < e[id].x) * -1;
                if (rx + ry <= ar[0].range) {
                    AC = rand() % 20 + 1 + (e[id].dex - 10) / 2;
                    ATK = rand() % 20 + 1 + (p[P_id].dex - 10) / 2;
                    ay = p[P_id].y - e[id].y; ax = p[P_id].x - e[id].x;
                    if (ay > 0 && ay > abs(ax)) {
                        p[P_id].pose = 4;
                    }
                    else if (ax > 0 && ax >=abs(ay)) {
                        p[P_id].pose = 2;
                    }
                    else if (ax<0 && abs(ax)>=abs(ay)) {
                        p[P_id].pose = 3;
                    }
                    else if (ay < 0 && abs(ay) > abs(ax)) {
                        p[P_id].pose = 1;
                    }
                    if (ATK > AC) {
                         BeginBatchDraw();
                        maps(p, P_id, e, psize, bsize);/*地圖繪製*/
                        e_put(e, bsize);
                        p_put(p, psize);                       
                        mciSendString(L"play ./Game/Sound/sgm/魔王魂_戦闘18.mp3 from 0", NULL, 0, NULL);
                        dmg = roll(ar[0].dmg, 1);
                        e[id].hp -= dmg;                      
                        TCHAR d[5];
                        _stprintf(d, _T("%d"), -dmg);		// 高版本 VC 推荐使用 _stprintf_s 函数
                        outtextxy(e[id].x * 48 + 24, e[id].y * 48, d);
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
                        wchar_t* ptr = _wcsdup(ar[0].Dmg.c_str());
                        outtextxy(960, 30, ptr);
                        wchar_t dnu[] = L"=";
                        outtextxy(1020, 30, dnu);
                        _stprintf(d, _T("%d"), dmg);
                        outtextxy(1030, 30, d);
                        EndBatchDraw();
                        ar[0].bullet--;
                        p[P_id].act--;
                        Sleep(1000);
                        if (e[id].hp <= 0) {
                            ss[e[id].y][e[id].x] = 0;
                            e[id].x = -1; e[id].y = -1;
                            e[id].speed = -1;
                        }

                    }
                    else {
                        BeginBatchDraw();
                        maps(p, P_id, e, psize, bsize);/*地圖繪製*/
                        e_put(e, bsize);
                        p_put(p, psize);                       
                        mciSendString(L"play ./Game/Sound/sgm/魔王魂_戦闘18.mp3 from 0", NULL, 0, NULL);
                        wchar_t w[] = L"未命中";
                        outtextxy(48 * e[id].x, 48 * e[id].y, w);                      
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
                        ar[0].bullet--;
                        p[P_id].act--;
                        Sleep(1000);
                    }
                }
                else {
                }
            }
            else if (chose == "a" && ar[0].bullet < 1) {

            mciSendString(L"play ./Game/Sound/sgm/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
 }
        }
        else if (chose == "a" && p[P_id].act < 1) {
            mciSendString(L"play ./Game/Sound/sgm/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
        }
    }
void p_item(player *p, arms *ar, item *it, string chose, int P_id) {
        if (chose == "i" && p[P_id].act > 0) {
            int i_id = -1;
            IMAGE itemblock;
            loadimage(&itemblock, L"./Game/picture/itemblock.png", 0, 0, false);
            putimage(200, 798, &itemblock);
            j = 0;
            for (i = 0; i < p[P_id].isize; i++) {
                wchar_t* ptr = _wcsdup(it[i].Name.c_str());
                outtextxy(210 + 100 * i, 812 + j * 49, ptr);
                TCHAR t[3];
                _stprintf(t, _T("%d"), it[i].number);		// 高版本 VC 推荐使用 _stprintf_s 函数
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
            if (i_id == 1) {
                for (i = 0; i < p[P_id].asize; i++) {
                    IMAGE back1;
                    loadimage(&back1, L"./Game/picture/attackblock.png", 0, 0, false);
                    putimage(200, 799 - 48 * i - 96, &back1);
                }
                for (i = 0; i <p[P_id]. asize; i++) {
                    if (p[i].abox == 0) {
                        setbkmode(TRANSPARENT);
                        settextcolor(BLACK);
                        wchar_t s[] = L"栓動步槍(";
                        outtextxy(200, 815 - 48 * i - 96, s);
                        TCHAR t[3];
                        _stprintf(t, _T("%d"), ar[0].bullet);		// 高版本 VC 推荐使用 _stprintf_s 函数
                        outtextxy(267, 815 - 48 * i - 96, t);
                        wchar_t s2[] = L")";
                        outtextxy(290, 815 - 48 * i - 96, s2);
                    }
                }
                int sss = 1; int cho = 0;
                ExMessage m1;
                while (sss != 0)
                {
                    // 获取一条鼠标消息
                    m1 = getmessage(EM_MOUSE);
                    switch (m1.message) {

                    case WM_LBUTTONDOWN://按鼠标左键分发事件
                        for (i = 0; i < p[P_id].asize; i++) {
                            if (m1.x >= 200 + i * 100 && m1.x <= 300 + i * 100 && m1.y >= 798 - 49 * i - 98 && m1.y <= 847 - 49 * i - 98) {
                                cho = i + 1;
                            }
                        }
                    case WM_RBUTTONUP:
                        sss = 0;
                    }
                }
                if (cho == 1 && ar[0].bullet < 5) {
                    it[cho - 1].number--;
                    ar[0].bullet = ar[0].mbullet;
                    p[P_id].act--;
                }
                else if (cho == 1 && ar[0].bullet == 5) {
                    mciSendString(L"play ./Game/Sound/sgm/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
                }
            }
        }
        else if (chose == "i" && p[P_id].act < 1) {
            mciSendString(L"play ./Game/Sound/sgm/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
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
void p_walk(player *p,enemy *e,string chose, int P_id, int id, int psize, int bsize) {
        if (chose == "w" && p[P_id].move > 0) {
            int rx, ry, m, X, Y, range[15][20];
            BeginBatchDraw();
            IMAGE mb;
            loadimage(&mb, L"./Game/picture/mb.png", 0, 0, false);
            string walk;
            queue<pair<int, int>> q;
            clearQpair(q);
            q.push({ p[P_id].x,p[P_id].y });
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
                if ((abs(X - p[P_id].x) + abs(Y - p[P_id].y)) == p[P_id].move) {
                    break;
                }
                if (ss[X + 1][Y] == 0 && range[Y][X + 1] == 0 && X + 1 <= 19) {
                    range[Y][X + 1] = 1;
                    q.push({ X + 1,Y });
                    transparentimage(NULL, 48 * (X + 1), 48 * Y, &mb);
                }
                if (ss[X - 1][Y] == 0 && range[Y][X - 1] == 0 && X - 1 >= 0) {
                    range[Y][X - 1] = 1;
                    q.push({ X - 1,Y });
                    transparentimage(NULL, 48 * (X - 1), 48 * Y, &mb);
                }
                if (ss[X][Y + 1] == 0 && range[Y + 1][X] == 0 && Y + 1 <= 14) {
                    range[Y + 1][X] = 1;
                    q.push({ X ,Y + 1 });
                    transparentimage(NULL, 48 * X, 48 * (Y + 1), &mb);
                }
                if (ss[X][Y - 1] == 0 && range[Y - 1][X] == 0 && Y - 1 >= 0) {
                    range[Y - 1][X] = 1;
                    q.push({ X ,Y - 1 });
                    transparentimage(NULL, 48 * X, 48 * (Y - 1), &mb);
                }
                q.pop();
            }
            IMAGE p1, p2, p3, p4;
            loadimage(&p1, L"./Game/picture/p1.png", 0, 0, false);
            loadimage(&p2, L"./Game/picture/p2.png", 0, 0, false);
            loadimage(&p3, L"./Game/picture/p3.png", 0, 0, false);
            loadimage(&p4, L"./Game/picture/p4.png", 0, 0, false);
            p_put(p, psize);
            EndBatchDraw();
            int sss = 1;
            while (sss != 0)
            {
                ExMessage m;
                // 获取一条鼠标消息
                m = getmessage(EM_MOUSE);
                switch (m.message) {

                case WM_LBUTTONDOWN://按鼠标左键分发事件
                    for (j = 0; j < 20; j++) {
                        for (i = 0; i < 15; i++) {
                            if (m.x <= j * 48 + 48 && m.x >= j * 48 && m.y <= i * 48 + 48 && m.y >= i * 48 && ss[j][i] != 1) {
                                if (range[i][j] == 1) {
                                    ss[p[P_id].x][p[P_id].y] = 0;
                                    int box[3000];
                                    bfs(p[P_id].x, p[P_id].y, j, i, box, p[P_id].move, P_id);
                                    int k, w = 0;
                                    int xbox = p[P_id].x, ybox = p[P_id].y;
                                    IMAGE map;
                                    loadimage(&map, L"./Game/picture/map01.png", 0, 0, false);
                                    while (1) {
                                        putimage(0, 0, &map);

                                        if (box[w] == 2) {

                                            for (k = 0; k <= 48; k += 16) {

                                                BeginBatchDraw();
                                                putimage(0, 0, &map);
                                                e_put(e,bsize);
                                                transparentimage(NULL, xbox * 48, ((ybox - 1) * 48 + k), &p1, 0xFF55FF);
                                                EndBatchDraw();
                                                Sleep(30);
                                                p[P_id].pose = 1;
                                            }
                                            ybox++;
                                        }
                                        else if (box[w] == 4) {
                                            for (k = 0; k <= 48; k += 16) {

                                                BeginBatchDraw();
                                                putimage(0, 0, &map);
                                                e_put(e, bsize);
                                                transparentimage(NULL, xbox * 48 - k, ((ybox - 1) * 48), &p2, 0xFF55FF);
                                                EndBatchDraw();
                                                Sleep(30);
                                                p[P_id].pose =2 ;
                                            }
                                            xbox--;

                                        }
                                        else if (box[w] == 6) {
                                            for (k = 0; k <= 48; k += 16) {

                                                BeginBatchDraw();
                                                putimage(0, 0, &map);
                                                e_put(e, bsize);
                                                transparentimage(NULL, xbox * 48 + k, ((ybox - 1) * 48), &p3, 0xFF55FF);
                                                EndBatchDraw();
                                                Sleep(30);
                                                p[P_id].pose = 3;
                                            }
                                            xbox++;
                                        }
                                        else if (box[w] == 8) {
                                            for (k = 0; k <= 48; k += 16) {

                                                BeginBatchDraw();
                                                putimage(0, 0, &map);
                                                e_put(e, bsize);
                                                transparentimage(NULL, xbox * 48, ((ybox - 1) * 48) - k, &p4, 0xFF55FF);
                                                EndBatchDraw();
                                                Sleep(30);
                                                p[P_id].pose = 4;
                                            }

                                            ybox--;
                                        }
                                        else if (box[w] == 0) {
                                            break;
                                        }
                                        w++;
                                    }
                                    p[P_id].x = j; p[P_id].y = i;
                                    ss[p[P_id].x][p[P_id].y] = 1;
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
        else if (chose == "w" && p[P_id].move == 0) {
            mciSendString(L"play ./Game/Sound/sgm/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
        }
    }
void e_attack(arms *ar,player *p,enemy *e,int id,int P_id,int bsize,int psize) {
    int rx, ry, ATK, AC, ax, ay;
        ry = (e[id].y - p[P_id].y) * (e[id].y > p[P_id].y) + (e[id].y - p[P_id].y) * (e[id].y < p[P_id].y) * -1; rx = (e[id].x - p[P_id].x) * (e[id].x > p[P_id].x) + (e[id].x - p[P_id].x) * (e[id].x < p[P_id].x) * -1;
        if (ar[e[id].baid].range >= rx + ry) {
            ay = e[id].y - p[P_id].y; ax = e[id].x - p[P_id].x;
            if (ay > 0 && ay > abs(ax)) {
                e[id].pose = 4;
            }
            else if (ax > 0 && ax >=abs(ay)) {
                e[id].pose = 2;
            }
            else if (ax<0 && abs(ax)>=abs(ay)) {
                e[id].pose = 3;
            }
            else if (ay < 0 && abs(ay) > abs(ax)) {
                e[id].pose = 1;
            }
            ATK = roll("", 2) + roll(ar[e[id].baid].hit, 1);
            AC = roll("", 2) + (p[P_id].dex - 10) / 2;
            if (ATK > AC) {
                BeginBatchDraw();
                maps(p, P_id, e, psize, bsize);/*地圖繪製*/
                e_put(e, bsize);
                p_put(p, psize);
                int  DMG = roll(ar[e[id].baid].dmg, 1);
                p[P_id].hp -= DMG;
                TCHAR k[5];
                _stprintf(k, _T("%d"), -DMG);		// 高版本 VC 推荐使用 _stprintf_s 函数
                outtextxy(p[P_id].x * 48 + 24, (p[P_id].y - 1) * 48 - 20, k);
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
                wchar_t* ptr = _wcsdup(ar[e[id].baid].Dmg.c_str());
                outtextxy(960, 30, ptr);
                wchar_t dnu[] = L"=";
                outtextxy(1020, 30, dnu);
                _stprintf(d, _T("%d"), DMG);
                outtextxy(1030, 30, d);
                EndBatchDraw();
                IMAGE tri;
                loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                transparentimage(NULL, (e[id].x) * 48, ((e[id].y - 1) * 48), &tri, 0xFF55FF);
                Sleep(1000);
            }
            else {         
                BeginBatchDraw();
                maps(p, P_id, e, psize, bsize);/*地圖繪製*/
                e_put(e, bsize);
                p_put(p, psize);
                wchar_t w1[] = L"未命中";
                outtextxy(48 * p[P_id].x, 48 * (p[P_id].y - 1) - 20, w1);
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
                IMAGE tri;
                loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                transparentimage(NULL, (e[id].x) * 48, ((e[id].y - 1) * 48), &tri, 0xFF55FF);
                Sleep(1000);
            }
        }
    }
void e_target(enemy *e,player *p,int &P_id,int id,int psize) {
        int min = 1000;
        for (i = 0; i < psize; i++) {
            int rx = 0, ry = 0;
            ry = (e[id].y - p[i].y) * (e[id].y > p[i].y) + (e[id].y - p[i].y) * (e[id].y < p[i].y) * -1; rx = (e[id].x - p[i].x) * (e[id].x > p[i].x) + (e[id].x - p[i].x) * (e[id].x < p[i].x) * -1;
            if (rx + ry < min) {
                min = rx + ry;
                P_id = i;
            }
        }
    }
void e_walk(enemy *e,player *p,int id,int P_id,int bsize,int psize) {
    if(abs(p[P_id].x-e[id].x)+abs(p[P_id].y - e[id].y)<=1) {
        return;
    }
        int box[1000], w = 0, k, xbox, ybox;
        Bbfs(e[id].x, e[id].y, p[P_id].x, p[P_id].y, box);
        xbox = e[id].x; ybox = e[id].y;
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
        while (e[id].move--) {
            putimage(0, 0, &map);

            if (box[w] == 2) {

                for (k = 0; k <= 48; k += 16) {

                    BeginBatchDraw();
                    putimage(0, 0, &map);
                    p_put(p, psize);
                    e_bput(e, id, bsize);
                    transparentimage(NULL, xbox * 48, ((ybox) * 48 + k), &wolf1, 0xFF55FF);
                    EndBatchDraw();
                    Sleep(30);
                    e[id].pose = 1;
                }
                ybox++;
            }
            else if (box[w] == 4) {
                for (k = 0; k <= 48; k += 16) {

                    BeginBatchDraw();
                    putimage(0, 0, &map);
                    p_put(p, psize);
                    e_bput(e, id, bsize);
                    transparentimage(NULL, xbox * 48 - k, ((ybox ) * 48), &wolf2, 0xFF55FF);
                    EndBatchDraw();
                    Sleep(30);
                    e[id].pose = 2;
                }
                xbox--;

            }
            else if (box[w] == 6) {
                for (k = 0; k <= 48; k += 16) {

                    BeginBatchDraw();
                    putimage(0, 0, &map);
                    p_put(p, psize);
                    e_bput(e, id, bsize);
                    transparentimage(NULL, xbox * 48 + k, ((ybox ) * 48), &wolf3, 0xFF55FF);
                    EndBatchDraw();
                    Sleep(30);
                    e[id].pose = 3;
                }
                xbox++;
            }
            else if (box[w] == 8) {
                for (k = 0; k <= 48; k += 16) {
                    BeginBatchDraw();

                    putimage(0, 0, &map);
                    p_put(p, psize);
                    e_bput(e, id, bsize);
                    transparentimage(NULL, xbox * 48, ((ybox) * 48) - k, &wolf4, 0xFF55FF);
                    EndBatchDraw();
                    Sleep(30);
                    e[id].pose = 4;
                }

                ybox--;
            }
            else if (box[w] == 0) {
                break;
            }
            w++;
        }
        e[id].x = xbox; e[id].y = ybox;               
        p_put(p, psize);
        e_put(e,bsize);
    }
void acts( player *p,enemy *e,string& chose, int P_id,int bsize,int id,int psize) {
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
                if (m.x >= e[i].x * 48 && m.x <= e[i].x * 48 + 48 && m.y >= e[i].y * 48 && m.y <= e[i].y * 48 + 48) {
                    wchar_t* ptr = _wcsdup(e[i].name.c_str());
                    outtextxy(960,0, ptr);
                    flag = 1;
                    wchar_t s[] = L"HP[";
                    outtextxy(960,30, s);
                    TCHAR t[5];
                    _stprintf(t, _T("%d"), e[i].hp);		// 高版本 VC 推荐使用 _stprintf_s 函数
                    outtextxy(985, 30, t);
                    wchar_t s1[] = L"/";
                    outtextxy(1020, 30, s1);
                    _stprintf(t, _T("%d"), e[i].mhp);		// 高版本 VC 推荐使用 _stprintf_s 函数
                    outtextxy(1025, 30, t);
                    wchar_t s2[] = L"]";
                    outtextxy(1060, 30, s2);
                    IMAGE tri;
                    loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                    transparentimage(NULL, (e[i].x) * 48, ((e[i] .y- 1) * 48), &tri, 0xFF55FF);
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
                BeginBatchDraw();
                maps(p,P_id,e,psize,bsize);
                e_put(e,bsize);
                p_put(p,psize);
                EndBatchDraw();
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
}
void END(player *p,enemy *e,string chose, int& second, int& P_id, int& id, int psize, int bsize, int& roundp, int& roundb) {
    int t, pmax, bmax;
    IMAGE wolf, p1;
    loadimage(&wolf, L"./Game/picture/A_wolf.png", 0, 0, false);
    loadimage(&p1, L"./Game/picture/A_p1.png", 0, 0, false);
    IMAGE back1;
    loadimage(&back1, L"./Game/picture/background2.png", 0, 0, false);
    IMAGE aline;
    loadimage(&aline, L"./Game/picture/aline.png", 0, 0, false);
    putimage(1025, 200, &aline);
    if (chose == "e") {
        
        P_id = 0; id = 0; pmax = 0; bmax = 0;
        for (t = 0; t < 100; t++) {
BeginBatchDraw();
        putimage(960, 0, &back1);
        putimage(1025, 200, &aline);
        for (i = 0; i < psize; i++) {
            
            transparentimage(NULL, 1040, 500 - (p[i].turn * 3), &p1, 0xFF55FF);
           
        }
        for (i = 0; i < bsize; i++) {
            if (e[i].hp > 0) {
                transparentimage(NULL, 970, 500 - (e[i].turn * 3), &wolf, 0xFF55FF);
            }
        }
        Sleep(60);
        EndBatchDraw();
            for (i = 0; i < psize; i++) {
                if (p[i].turn >= 100) {
                    if (p[pmax].turn < p[i].turn || pmax == i) {
                        pmax = i;
                    }
                }
            }
            for (i = 0; i < bsize; i++) {
                if (e[i].turn >= 100) {
                    if (e[bmax].turn < e[i].turn || bmax == i) {
                        bmax = i;
                    }
                }
            }
            if (e[bmax].turn> p[pmax].turn && e[bmax].turn >= 100) {
                id = bmax; e[bmax].turn -= 100;  roundp = 0; roundb = 1; e[id].move = 8; break;
            }
            else if (p[pmax].turn >= e[bmax].turn && p[pmax].turn >= 100) {
                P_id = pmax;
                p[pmax].turn -= 100;
                p[P_id].move = 6;
                p[P_id].act = 1;
                roundp = 1; roundb = 0;
                break;
            }
            for (i = 0; i < psize; i++) {
                p[i].turn += p[i].speed;
            }
            for (i = 0; i < bsize; i++) {
                if (e[i].speed > 0) {
                    e[i].turn += e[i].speed;
                }
            }
            second++;
        }
    }
}
void Save(player* p, enemy* e, arms* ar, item* it,int P_id,int second,int psize,int bsize,int roundp,int roundb,time_t first, time_t two, time_t three,string chose) {
    int u = 0;
    string a, b = ".txt", read = "";
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
                    for (int J = 0; J < str.size(); J++) {
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
                    for (int J = 0; J < str.size(); J++) {
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
                        sss = 0; ssss = 0;  u = 1;
                    }
                }
            }
        }
        if (u == 0) {
            two = time(NULL);
            string file = "./Game/save/" + a + b;
            ofstream fout(file);
            if (fout) {
                fout << two - first + three << endl;
                for (i = 0; i < psize; i++) {
                    fout << p[i].lv << endl;
                    fout << p[i].mhp << endl;
                    fout << p[i].hp << endl;
                    fout << p[i].dex << endl;
                    fout << p[i].move << endl;
                    fout << p[i].isize << endl;
                    fout << p[i].asize << endl;
                    fout << p[i].x << endl;
                    fout << p[i].y << endl;
                    fout << p[i].turn << endl;
                    fout << p[i].speed << endl;
                    fout << p[i].abox << endl;
                    fout << p[i].act << endl;
                    fout << p[i].pose << endl;
                }
                for (i = 0; i < bsize; i++) {
                    fout << e[i].lv << endl;
                    fout << e[i].mhp << endl;
                    fout << e[i].hp << endl;
                    fout << e[i].dex << endl;
                    fout << e[i].str << endl;
                    fout << e[i].x << endl;
                    fout << e[i].y << endl;
                    fout << e[i].speed << endl;
                    fout << e[i].turn << endl;
                    fout << e[i].move << endl;
                    fout << e[i].baid << endl;
                    fout << e[i].pose << endl;
                }
                for (i = 0; i < 2; i++) {
                    fout << ar[i].mbullet << endl;
                    fout << ar[i].bullet << endl;
                }
                for (i = 0; i < 1; i++) {
                    fout << it[i].number << endl;
                }
                fout << P_id << endl;
                fout << psize << endl;
                fout << bsize << endl;
                fout << roundp << endl;
                fout << roundb << endl;
                fout << second << endl;

            }
        }
        IMAGE back1;
        loadimage(&back1, L"./Game/picture/background2.png", 0, 0, false);
        putimage(960, 0, &back1);
        IMAGE wolf, p1;
        loadimage(&wolf, L"./Game/picture/A_wolf.png", 0, 0, false);
        loadimage(&p1, L"./Game/picture/A_p1.png", 0, 0, false);
        for (i = 0; i < bsize; i++) {
            transparentimage(NULL, 970, 500 - (e[i].turn * 3), &wolf, 0xFF55FF);
        }
        for (i = 0; i < psize; i++) {
            transparentimage(NULL, 1040, 500 - (p[i].turn * 3), &p1, 0xFF55FF);
        }
        }
}
void Load(player* p, enemy* e, arms* ar, item* it, int &P_id, int &second, int &psize, int &bsize, int &roundp, int &roundb, time_t &first, time_t &two, time_t &three, string &chose) {
    if (chose == "l") {

        string a, b = ".txt", read = "";
        int ssss = 1; j = 0;
        while (ssss != 0) {
            int sss = 1;
            IMAGE saveblock;
            IMAGE rarrow;
            IMAGE larrow; TCHAR d[5];
            cleardevice();
            BeginBatchDraw();
            settextcolor(BLACK);
            setbkmode(TRANSPARENT);
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
                    for (int J = 0; J < str.size(); J++) {
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
                    for (int J = 0; J < str.size(); J++) {
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
        string file = "./Game/save/" + a + b;
        std::ifstream ifs(file, std::ios::in);
        if (!ifs.is_open() && a != "") {
            putimage(400, 400, &saveblock);
            wchar_t s2[] = L"無此存檔";
            outtextxy(610, 440, s2);
            Sleep(1000);
        }
        else if (a == "") {

        }
        else {
            std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
            int k = 0,z[1000];
            string w[1000];
            for (i = 0; i < str.size(); i++) {
                if (str[i] != 10) {
                    w[k] += str[i];
                }
                else { 
                    z[k] = stoi(w[k]);
                    k++; 
                }
            }
            ifs.close();
            k = 1;
            for (i = 0; i < psize; i++) {
                p[i].lv = z[k]; k++;
                 p[i].mhp = z[k]; k++;
                p[i].hp = z[k]; k++;
                 p[i].dex = z[k]; k++;
                 p[i].move = z[k]; k++;
                 p[i].isize = z[k]; k++;
                 p[i].asize = z[k]; k++;
                p[i].x = z[k]; k++;
                p[i].y = z[k]; k++;
                 p[i].turn = z[k]; k++;
                p[i].speed = z[k]; k++;
                p[i].abox = z[k]; k++;
                 p[i].act = z[k]; k++;
                 p[i].pose = z[k]; k++;
}
            for (i = 0; i < bsize; i++) {
                 e[i].lv = z[k]; k++;
                 e[i].mhp = z[k]; k++;
                 e[i].hp = z[k]; k++;
               e[i].dex = z[k]; k++;
                 e[i].str = z[k]; k++;
                e[i].x = z[k]; k++;
                 e[i].y = z[k]; k++;
                 e[i].speed = z[k]; k++;
                 e[i].turn = z[k]; k++;
                 e[i].move = z[k]; k++;
                 e[i].baid = z[k]; k++;
                 e[i].pose = z[k]; k++;
            }
            for (i = 0; i < 2; i++) {
               ar[i].mbullet = z[k]; k++;
                ar[i].bullet = z[k]; k++;
            }
            for (i = 0; i < 1; i++) {
                 it[i].number = z[k]; k++;
            }
             P_id = z[k]; k++;
             psize = z[k]; k++;
             bsize = z[k]; k++;
             roundp = z[k]; k++;
             roundb = z[k]; k++;
             second = z[k]; k++;

            first = time(NULL);
        }
        BeginBatchDraw();
        IMAGE back1;
        loadimage(&back1, L"./Game/picture/background2.png", 0, 0, false);
        putimage(960,0,&back1);
        IMAGE wolf, p1;
        loadimage(&wolf, L"./Game/picture/A_wolf.png", 0, 0, false);
        loadimage(&p1, L"./Game/picture/A_p1.png", 0, 0, false);
        for (i = 0; i < bsize; i++) {
            transparentimage(NULL, 970, 500 - (e[i].turn * 3), &wolf, 0xFF55FF);
        }
        for (i = 0; i < psize; i++) {
            transparentimage(NULL, 1040, 500 - (p[i].turn * 3), &p1, 0xFF55FF);
        }
    }

}

int start() {
    IMAGE start1, start2, startblock,loadblock;
    loadimage(&start1, L"./Game/picture/start.png", 0, 0, false);
    loadimage(&startblock, L"./Game/picture/start_block.png", 0, 0, false);
    loadimage(&start2, L"./Game/picture/start2.png", 0, 0, false);
    loadimage(&loadblock, L"./Game/picture/load_block.png", 0, 0, false);
    bool T = true;
    while (1) {
        if (T == true) {
            putimage(0, 0, &start1);
            putimage(493, 739, &startblock);
        }
        else {
            putimage(0, 0, &start2);
            putimage(493, 739, &loadblock);
        }
        ExMessage m;
        m = getmessage(EM_MOUSE);
        switch (m.message)
        {
        case WM_LBUTTONDOWN:
            if (m.x>300&&m.x<370&&m.y>730&&m.y<840) {
                if (T == true) { T = false; }
                else { T = true; }
        }
            else if (m.x>930&&m.y>740&&m.x<1000&&m.y<840) {
                if (T == true) { T = false; }
                else { T = true; }
            }
            else if (m.x>493&&m.y>739&& m.y<844&&m.x<802) {
                if (T == true) {
                    return 1;
                }
                else {

                    return 0;
                }
            }
            }
    }
}
int main() {    
    initgraph(1296, 960);
    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
    HWND hWnd = GetHWnd();
    SetWindowText(hWnd,L"RPG");
    srand(time(NULL));
    int s=start();
    int  id=0, P_id=0, second=0,  load = 0;
    player p[1];
    enemy e[2];
    arms  ar[2];
    item  it[1];
    int ix, iy, n,abox1[1];
    int  t, psize = 1, bsize = 2, roundp = 0, roundb = 0;
    string  chose = "l" ;
    string a, b = ".txt", read = "";    
    time_t first=0, two=0,three=0;
    p[0].name = L"夏洛特"; p[0].story = L"主人公";
    e[0].name = L"野狼1"; e[0].story = L"團體行動的動物 隨著數量增加危險性也會大幅上升";
    e[1].name = L"野狼2"; e[1].story = L"團體行動的動物 隨著數量增加危險性也會大幅上升";
    ar[0].name = L"栓動步槍"; ar[0].dmg = "2d8"; ar[0].Dmg = L"2d8"; ar[0].range = 5;
    ar[1].name = L"爪子"; ar[1].dmg = "2d8"; ar[1].Dmg = L"2d8"; ar[1].hit = "1d4+2"; ar[1].range = 1;
    it[0].Name = L"通常彈"; it[0].name = "通常彈";
    if (s == 1) {
        chose = "e";
        maps(p, P_id, e, psize, bsize);/*地圖繪製*/
        e_put(e, bsize);
        p_put(p, psize);
        END(p, e, chose, second, P_id, id, psize, bsize, roundp, roundb);
    }
    else if (s == 0) {
        Load(p, e, ar, it, P_id, second, psize, bsize, roundp, roundb, first, two, three, chose);
    }
    mciSendString(L"open ./Game/Sound/bgm/魔王魂_ファンタジー12.mp3", NULL, 0, NULL);
    mciSendString(L"open ./Game/Sound/sgm/魔王魂_戦闘18.mp3", NULL, 0, NULL);
    mciSendString(L"open ./Game/Sound/sgm/魔王魂_効果音_ワンポイント33.mp3", NULL, 0, NULL);
    mciSendString(L"play ./Game/Sound/bgm/魔王魂_ファンタジー12.mp3 repeat", NULL, 0, NULL);
    first = time(NULL);
    while (1) {
        if (roundp == 1) {
          
            BeginBatchDraw();
            maps(p,P_id,e,psize,bsize);/*地圖繪製*/   
            e_put(e, bsize);
            p_put(p, psize);
            EndBatchDraw();
            acts(p,e,chose,P_id,bsize,id,psize);/*選項*/
            Save(p, e, ar, it, P_id, second, psize, bsize, roundp, roundb, first, two, three, chose);
            Load(p, e, ar, it, P_id, second, psize, bsize, roundp, roundb, first, two, three, chose);
            p_attack(p,e,ar,P_id,id,chose,bsize,psize);/*攻擊*/
            p_item(p,ar,it,chose,P_id);/*物品*/
            p_walk(p,e,chose, P_id ,id, psize, bsize);/*移動*/
            END(p,e,chose, second, P_id, id, psize,  bsize, roundp, roundb);/*結束行動*/
            /*存檔*/
            /*讀檔*/
        }
        else if (roundb == 1) {
            BeginBatchDraw();
            maps(p, P_id, e, psize, bsize);
            e_put(e, bsize);
            p_put(p, psize);
            EndBatchDraw();
            e_target(e, p, P_id, id, psize);         
            e_walk(e, p, id, P_id, bsize, psize);
            e_attack(ar,p,e,id,P_id,bsize,psize);
            END(p, e, chose, second, P_id, id, psize, bsize, roundp, roundb);/*結束行動*/
        }
    }
}
