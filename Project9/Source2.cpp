#pragma warning(disable:4996)  
#pragma comment(lib,"winmm.lib") 
#pragma comment(lib, "user32")
#pragma comment( lib, "MSIMG32.LIB")
#pragma comment(lib,"comsuppw.lib")
#pragma comment(lib,"jsoncpp.lib")
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
#include<comutil.h>
#include <json.h>
using namespace std;
int i, j, ss[200][150],F=0,Z=0;
string s[26][60];
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor)
{
    HDC dstDC = GetImageHDC(dstimg);
    HDC srcDC = GetImageHDC(srcimg);
    int w = srcimg->getwidth();
    int h = srcimg->getheight();

    // 使用 Windows GDI 函数实现透明位图
    TransparentBlt(dstDC, x, y, w, h, srcDC, 0,0, w, h, transparentcolor);
}
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor,int a,int b,int c,int d)
{
    HDC dstDC = GetImageHDC(dstimg);
    HDC srcDC = GetImageHDC(srcimg);
    TransparentBlt(dstDC, x, y, c,d, srcDC, a, b, c, d, transparentcolor);
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
wstring string2wstring(const string &s) {
    _bstr_t t= s.c_str();
    wchar_t* pwchar = (wchar_t*)t;
    wstring result = pwchar;
    return result;
}
std::wstring readFile(const char* filename)
{
    std::wifstream wif(filename);
    wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
    std::wstringstream wss;
    wss << wif.rdbuf();
    return wss.str();
}
wstring UTF8ToUnicode(const string& s)
{
    wstring result;

    // 獲得緩衝區的寬字符個數
    int length = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);

    wchar_t* buffer = new wchar_t[length];
    ::MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, length);
    result = buffer;

    delete[] buffer;
    return result;
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
class flag {
public:
    int check;
};
class player{
public: 
    wstring name, story;
    int lv ,mhp,hp,dex,move,isize,asize,x,y,turn,speed,abox,act,pose,str,INT,con,wis,cha,arms_id,stone_id,exp;
    player() {};   

};
class enemy_type {
public:
    wstring name, story;
    int lv, mhp, hp, dex, str, x, y, INT, con, wis, cha, speed, move, baid;
};
class enemy {
public:
    wstring name, story;
    int lv, mhp,hp,dex, str,x,y,speed, INT, con, wis, cha,turn,move,baid,pose,type;
    enemy() {};
   
};
class arms {
public:
    wstring name,Dmg,story;
    string  dmg, hit;
    int range,mbullet, bullet,number;
    arms() {};
};
class item {
public:
    wstring Name,story;
    string name,type;
    int number;
    item() {};
};
class Map {
public:
    int x;
    int y;
    int psize;
    int nsize;
    int esize;
    int px, py;
    int msize;
    int exitsize;
    string block,npcid,e_set,b_set,exit_set;
};
class b_map {
public:
    int x;
    int y;
    int psize;
    int nsize;
    int esize;
    string e_set,set,p_set;
};
class npc {
public:
    wstring name;
    int avatar,x,y;
};
class e_npc {
public:
    int x, y,move,pose,type,see,battle,live;
};
class stone {
public:
    wstring name, story;
    string type;
    int number;
};
class skill {
public:
    wstring name, story;
    string type;
};
class Exit {
public:
    int x;
    int y;
    int gx;
    int gy;
    int Mid;
    int gMid;
    bool state;
};
void readmapjson(Map* m,int m_id, const char* filename) {
    Json::Reader reader;
    Json::Value root;
    ifstream in(filename, ios::binary);
    int u;
    if (!in.is_open()) {
        return;
    }
    if (reader.parse(in, root)) {
        m[m_id].y = root["height"].asInt();
        m[m_id].x = root["width"].asInt();
        for (i = 0; i < m[m_id].x;i++) {
            for (j = 0; j < m[m_id].y; j++) {
                ss[i][j] = 0;
            }
        }
        for (unsigned int k = 0; k < root["layers"].size(); k++) {
            if (root["layers"][k]["properties"][0]["value"].asBool() == true) {
                for (unsigned int w = 0; w < root["layers"][k]["data"].size(); w++) {
                    if (root["layers"][k]["data"][w].asInt64() != 0) {
                        ss[w % m[m_id].x][w / m[m_id].x] = 1;
                    }
                }
            }
        }
    }
    u = root["layers"][0]["data"].size();
    in.close();
}
void readeventjson(player *p,npc *n,const char* filename,int &m_id) {
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    IMAGE t_block, a1, tri, c1;
    ExMessage ww;
    loadimage(&t_block, L"./Game/picture/talkblock.png", 0, 0, false);
    loadimage(&tri, L"./Game/picture/talktri.png", 0, 0, false);
    wstring mm,tt,ss,MM;
    Json::Reader reader;
    Json::Value root;
    ifstream in(filename, ios::binary);
    if (!in.is_open()) {
        return;
    }
    if (reader.parse(in, root)) {
        if (root.isMember("map")) {
            m_id = root["map"].asInt();
        }
        if (root.isMember("x")) {
            p[0].x = root["x"][0].asInt();
            p[0].y = root["y"][0].asInt();
        }
        for (int k = 0; k < root["talk"].size(); k++) {
            BeginBatchDraw();
            putimage(0, 696, &t_block);
            if (root["talk"][k]["cg"].asInt() != -1) {
                IMAGE cg;               
                mm = std::to_wstring(root["talk"][k]["cg"].asInt());
                mm = L"./Game/picture/cg" + mm + L".png";
                LPCTSTR path = mm.c_str();
                loadimage(&cg, path, 0, 0, false);
                putimage(0, 0, &cg);
            }
            if (root["talk"][k]["npc"].asInt() != -1) {
                settextcolor(BLACK);
                settextstyle(23, 0, _T("Taipei Sans TC Beta"));
                LPCTSTR path = n[root["talk"][k]["npc"].asInt()].name.c_str();
                outtextxy(45, 710, path);
                IMAGE p1;
                if (root["talk"][k]["npc"].asInt() != 0) {
                    mm = std::to_wstring(root["talk"][k]["npc"].asInt());
                    mm = L"./Game/picture/t_npc" + mm +L"_" + std::to_wstring(root["talk"][k]["face"].asInt()) + L".png";
                    LPCTSTR path = mm.c_str();
                    loadimage(&p1, path, 0, 0, false);
                    transparentimage(NULL, 0, 746, &p1, 0xFF55FF);
                }
            }
            else {
                settextcolor(BLACK);
                settextstyle(40, 0, _T("Taipei Sans TC Beta"));
                LPCTSTR path = p[root["talk"][k]["player"].asInt()].name.c_str();
                outtextxy(45, 710, path);
                IMAGE p1;
                mm = std::to_wstring(root["talk"][k]["player"].asInt());
                mm = L"./Game/picture/player" + mm +L"_" + std::to_wstring(root["talk"][k]["face"].asInt()) + L".png";
                 path = mm.c_str();
                loadimage(&p1, path, 0, 0, false);
                transparentimage(NULL, 0, 746, &p1, 0xFF55FF);
            }
            EndBatchDraw();
            wstring s = UTF8ToUnicode(root["talk"][k]["sentence"].asString());
            settextstyle(30, 0, _T("Taipei Sans TC Beta"));
            settextcolor(WHITE);
            int K = 0,W=0;
            for (int w = 0; w <s.size(); w++) {
                tt = L"";
                if (s[w] == '^') {
                    K++;
                    W = 0;
                }
                else {
                    tt += s[w];
                    LPCTSTR path = tt.c_str();
                    outtextxy(30 * W + 150, 696 + K * 50 + 5, path);
                    W++;
                    Sleep(50);
                }
            }
            transparentimage(NULL, 600, 910, &tri, 0xFF55FF);
            flushmessage(EM_KEY);
            while (1) {
                if (peekmessage(&ww, EM_KEY)) {
                    if (ww.vkcode == VK_SPACE) {
                        Sleep(1000);
                        break;
                    }
                }
            }
        }
    }
    in.close();
}
void readtalkjson(player* p, npc* n, const char* filename) {
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    IMAGE t_block, a1, tri, c1;
    ExMessage ww;
    loadimage(&t_block, L"./Game/picture/talkblock.png", 0, 0, false);
    loadimage(&tri, L"./Game/picture/talktri.png", 0, 0, false);
    wstring mm, tt, ss;
    Json::Reader reader;
    Json::Value root;
    ifstream in(filename, ios::binary);
    if (!in.is_open()) {
        return;
    }
    if (reader.parse(in, root)) {
        for (int k = 0; k < root["talk"].size(); k++) {
            BeginBatchDraw();
            putimage(0, 696, &t_block);
            if (root["talk"][k]["npc"].asInt() != -1) {
                settextcolor(BLACK);
                settextstyle(23, 0, _T("Taipei Sans TC Beta"));
                LPCTSTR path = n[root["talk"][k]["npc"].asInt()].name.c_str();
                outtextxy(45, 710, path);
                IMAGE p1;
                if (root["talk"][k]["npc"].asInt() != 0) {
                    mm = std::to_wstring(root["talk"][k]["npc"].asInt());
                    mm = L"./Game/picture/t_npc" + mm+L"_" + std::to_wstring(root["talk"][k]["npc"].asInt()) + L".png";
                    LPCTSTR path = mm.c_str();
                    loadimage(&p1, path, 0, 0, false);
                    transparentimage(NULL, 0, 746, &p1, 0xFF55FF);
                }
            }
            else {
                IMAGE p1;
                mm = std::to_wstring(root["talk"][k]["player"].asInt());
                mm = L"./Game/picture/player" + mm + L".png";
                LPCTSTR path = mm.c_str();
                loadimage(&p1, path, 0, 0, false);
                transparentimage(NULL, 0, 746, &p1, 0xFF55FF);
            }
            EndBatchDraw();
            wstring s = UTF8ToUnicode(root["talk"][k]["sentence"].asString());
            settextstyle(30, 0, _T("Taipei Sans TC Beta"));
            settextcolor(WHITE);
            int K = 0;
            for (int w = 0; w < s.size(); w++) {
                tt = L"";
                tt += s[w];
                LPCTSTR path = tt.c_str();
                outtextxy(30 * w + 150, 696 + K * 30 + 5, path);
                Sleep(50);
            }
            flushmessage(EM_KEY);
            while (1) {
                if (peekmessage(&ww, EM_KEY)) {
                    if (ww.vkcode == VK_SPACE) {
                        Sleep(1000);
                        break;
                    }
                }
            }
        }
    }
    in.close();
}
void change(player* p,int p_id, string otype,string ntype) {
    int k;
    string box="";
    for (k = 0; k < otype.size(); k++) {
        if (otype[k] == 'H') {
            k++;
            if (otype[k] == '+') {
                k++;
                while (1) {
                    if ((otype[k] < '0' && otype[k]>'9') || k >= otype.size()) {
                        k--;
                        break;
                    } 
                    box += otype[k];
                    k++;
                }
                p[p_id].mhp -= stoi(box);;
                box = "";
            }
            else if (otype[k] == '-') {
                k++;
                while (1) {
                    if ((otype[k] < '0' && otype[k]>'9') || k >= otype.size()) {
                        k--;
                        break;

                    }
                    box += otype[k];
                    k++;
                }
                p[p_id].mhp += stoi(box);;
                box = "";
            }
        }
        else if (otype[k]=='S') {
            k++;
            if (otype[k] == '+') {
                k++;
                while (1) {
                    if ((otype[k] < '0' && otype[k]>'9') || k >= otype.size()) {
                        k--;
                        break;
                    }
                    box += otype[k];
                    k++;
                }
                p[p_id].speed -= stoi(box);;
                box = "";
            }
            else if (otype[k] == '-') {
                k++;
                while (1) {
                    if ((otype[k] < '0' && otype[k]>'9') || k >= otype.size()) {
                        k--;
                        break;
                    }
                    box += otype[k];
                    k++;
                }
                p[p_id].speed += stoi(box);;
                box = "";
            }
        }
    }
    for (k = 0; k < ntype.size(); k++) {
        if (ntype[k] == 'H') {
            k++;
            if (ntype[k] == '+') {
                k++;
                while (1) {
                    if ((ntype[k] < '0' && ntype[k]>'9') || k >= ntype.size()) {
                        k--;
                        break;
                    }
                    box += ntype[k];
                    k++;
                }
                p[p_id].mhp += stoi(box);;
                box = "";
            }
            else if (ntype[k] == '-') {
                k++;
                while (1) {
                    if ((ntype[k] < '0' && ntype[k]>'9') || k >= ntype.size()) {
                        k--;
                        break;
                    }
                    box += ntype[k];
                    k++;
                }
                p[p_id].mhp -= stoi(box);;
                box = "";
            }
        }
        else if (ntype[k] == 'S') {
            k++;
            if (ntype[k] == '+') {
                k++;
                while (1) {
                    if ((ntype[k] < '0' && ntype[k]>'9') || k >= ntype.size()) {
                        k--;
                        break;
                    }
                    box += ntype[k];
                    k++;
                }
                p[p_id].speed += stoi(box);;
                box = "";
            }
            else if (ntype[k] == '-') {
                k++;
                while (1) {
                    if ((ntype[k] < '0' && ntype[k]>'9') || k >= ntype.size()) {
                        k--;
                        break;
                    }
                    box += ntype[k];
                    k++;
                }
                p[p_id].speed -= stoi(box);;
                box = "";
            }
        }
    }
}
void p_put(player *p,int psize) {
    IMAGE p1,c;
    wstring mm;
    LPCTSTR path;
        for (int I = 0; I < psize; I++) {
            mm = std::to_wstring(I);
            mm = L"./Game/picture/player" + mm + L"_1.png";
            path = mm.c_str();
            loadimage(&c, path, 0, 0, false);
            transparentimage(NULL, 0, 730, &c, 0xFF55FF);
            wchar_t* ptr = _wcsdup(p[I].name.c_str());
            outtextxy(60, 721, ptr);
            mm = std::to_wstring(I);
            mm = L"./Game/picture/p" + mm + L".png";
            path = mm.c_str();
            loadimage(&p1, path, 0, 0, false);
            if (p[I].pose == 1) {
                transparentimage(NULL, p[I].x * 48, p[I].y * 48 - 16, &p1, 0xFF55FF, 64 + 16, 0, 48, 64);
            }
            else if (p[I].pose == 2) {
                transparentimage(NULL, p[I].x * 48, p[I].y * 48 - 16, &p1, 0xFF55FF, 64 + 16, 64, 48, 64);
            }
            else if (p[I].pose == 3) {
                transparentimage(NULL, p[I].x * 48, p[I].y * 48 - 16, &p1, 0xFF55FF, 64 + 16, 128, 48, 64);
            }
            else if (p[I].pose == 4) {
                transparentimage(NULL, p[I].x * 48, p[I].y * 48 - 16, &p1, 0xFF55FF, 64 + 16, 192, 48, 64);
            }
        }
    }
void e_put(enemy *e,int bsize) {
    IMAGE b1,b2,b3,b4;
    wstring mm;
    for (int I = 0; I < bsize; I++) {
        mm = std::to_wstring(e[I].type);
        mm = L"./Game/picture/enemy" + mm + L".png";
        LPCTSTR path = mm.c_str();
        loadimage(&b1, path, 0, 0, false);
        if (e[I].pose == 1) {
            transparentimage(NULL, e[I].x * 48, e[I].y * 48, &b1, 0xFF55FF, 48, 0, 48, 48);
        }
        else if (e[I].pose == 2) {
            transparentimage(NULL, e[I].x * 48, e[I].y * 48, &b1, 0xFF55FF, 48, 48, 48, 48);

        }
        else if (e[I].pose == 3) {
            transparentimage(NULL, e[I].x * 48, e[I].y * 48, &b1, 0xFF55FF, 48, 96, 48, 48);

        }
        else if (e[I].pose == 4) {
            transparentimage(NULL, e[I].x * 48, e[I].y * 48, &b1, 0xFF55FF, 48, 144, 48, 48);

        }
        }
    }
void e_bput(enemy *e, int id, int bsize) {
    wstring mm;
        for (int I = 0; I < bsize; I++) {
            if (I != id) {
                IMAGE bplayer;
                LPCTSTR path;
                mm = std::to_wstring(e[I].type);
                mm = L"./Game/picture/enemy" + mm + L".png";
                path = mm.c_str();
                loadimage(&bplayer,path, 0, 0, false);
                transparentimage(NULL, e[I].x * 48, e[I].y * 48, &bplayer, 0xFF55FF, 48, 0, 48, 48);
            }
        }
    }
void maps(player *p, int P_id,enemy *e,b_map *b_m,int b_mid) {
    IMAGE text;
    loadimage(&text, L"./Game/picture/text.png", 0, 0, false);
    IMAGE aline;
    loadimage(&aline, L"./Game/picture/aline.png", 0, 0, false);
    IMAGE back2;
    loadimage(&back2, L"./Game/picture/background.png", 0, 0, false);
    putimage(0, 720, &back2); 
    IMAGE map01;
    wstring mm;
    mm = std::to_wstring(b_mid);
    mm = L"./Game/picture/b_map" + mm + L".png";
    LPCTSTR path = mm.c_str();
    loadimage(&map01, path, 0, 0, false);
    putimage(0, 0, &map01);
    putimage(960, 0, &text);
    putimage(1025, 200, &aline);
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
void Ai_bfs(int sx, int sy, int x, int y, int* box,Map *m,int m_id) {
    int X = sx, Y = sy, road[100][100], k, w;
    for (k = 0; k < m[m_id].x; k++) {
        for (w = 0; w < m[m_id].y; w++) {
            road[k][w] = -1;
            if (ss[k][w] == 1) {
                road[k][w]= - 2;
            }
        }
    }
    road[X][Y] = 0;
    queue<pair<int, int>> q;
    clearQpair(q);
    q.push({ X,Y });
    while (q.size() > 0) {
        X = q.front().first;
        Y = q.front().second;
        if (abs(X - x) + abs(Y - y) == 1) {
            break;
        }
        if (ss[X + 1][Y] == 0 && road[X + 1][Y] == -1 && X + 1< m[m_id].x) {
            
            q.push({ X + 1,Y });
            road[X + 1][Y] = 6;
        }
        if (ss[X - 1][Y] == 0 && road[X - 1][Y] == -1 && X - 1 >= 0) {
            q.push({ X - 1,Y });
            road[X - 1][Y] = 4;
        }
        if (ss[X][Y + 1] == 0 && road[X][Y + 1] == -1 && Y + 1 < m[m_id].y) {
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
void p_attack(player *p,enemy *e,arms *ar,b_map *b_m,int b_mid,int P_id,int id,string &chose,int bsize,int psize) {
    int ax, ay;
    LPCTSTR path1;
    wstring mm;
        int sss = 1;
        ExMessage m, m1; string attack;
        if (chose == "a" && p[P_id].act > 0) {    
       setbkmode(TRANSPARENT);
        settextcolor(BLACK);
            for (i = 0; i < p[P_id].asize; i++) {
                IMAGE back1;
                loadimage(&back1, L"./Game/picture/attackblock.png", 0, 0, false);
                putimage(200, 799 + 48 * i, &back1);
                path1 = ar[p[i].arms_id].name.c_str();
                outtextxy(200, 815, path1);
                mm = std::to_wstring(ar[p[i].arms_id].bullet);
                path1 = mm.c_str();
                outtextxy(267, 815, path1);
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
            if (attack == "a" && ar[p[P_id].arms_id].bullet > 0) {
                BeginBatchDraw();
                for (i = 0; i < b_m[b_mid].x; i++) {
                    for (j = 0; j < b_m[b_mid].y; j++) {
                        rx = 0; ry = 0;
                        ry = (p[P_id].y - j) * (p[P_id].y > j) + (p[P_id].y - j) * (p[P_id].y < j) * -1; rx = (p[P_id].x - i) * (p[P_id].x > i) + (p[P_id].x - i) * (p[P_id].x < i) * -1;
                        if (ry + rx <= ar[p[P_id].arms_id].range && rx + ry > 0) {
                            IMAGE mb;
                            loadimage(&mb, L"./Game/picture/ab.png", 0, 0, false);
                            transparentimage(NULL, 48 * i, 48 * j, &mb);
                        }
                    }
                }
                p_put(p, psize);
                EndBatchDraw();
                sss = 1;
                while (sss != 0)
                {
                    // 获取一条鼠标消息
                    m1 = getmessage(EM_MOUSE);
                    switch (m1.message) {

                    case WM_LBUTTONDOWN://按鼠标左键分发事件
                        for (i = 0; i < b_m[b_mid].x; i++) {
                            for (j = 0; j < b_m[b_mid].y; j++) {
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
                if (rx + ry <= ar[p[P_id].arms_id].range) {
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
                        maps(p, P_id, e,b_m,b_mid);/*地圖繪製*/
                        e_put(e, bsize);
                        p_put(p, psize);                       
                        mciSendString(L"play ./Game/Sound/SE/魔王魂_戦闘18.mp3 from 0", NULL, 0, NULL);
                        dmg = roll(ar[p[P_id].arms_id].dmg, 1);
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
                        wchar_t* ptr = _wcsdup(ar[p[P_id].arms_id].Dmg.c_str());
                        outtextxy(960, 30, ptr);
                        wchar_t dnu[] = L"=";
                        outtextxy(1020, 30, dnu);
                        _stprintf(d, _T("%d"), dmg);
                        outtextxy(1030, 30, d);
                        EndBatchDraw();
                        ar[p[P_id].arms_id].bullet--;
                        p[P_id].act--;
                        Sleep(1000);
                        if (e[id].hp <= 0) {
                            ss[e[id].x][e[id].y] = 0;
                            e[id].x = -1; e[id].y = -1;
                            e[id].speed = -1;
                        }

                    }
                    else {
                        BeginBatchDraw();
                        maps(p, P_id, e,b_m,b_mid);/*地圖繪製*/
                        e_put(e, bsize);
                        p_put(p, psize);                       
                        mciSendString(L"play ./Game/Sound/SE/魔王魂_戦闘18.mp3 from 0", NULL, 0, NULL);
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
                        ar[p[P_id].arms_id].bullet--;
                        p[P_id].act--;
                        Sleep(1000);
                    }
                }
                else {
                }
            }
            else if (chose == "a" && ar[p[P_id].arms_id].bullet < 1) {

            mciSendString(L"play ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
 }
        }
        else if (chose == "a" && p[P_id].act < 1) {
            mciSendString(L"play ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
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
                    mciSendString(L"play ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
                }
            }
        }
        else if (chose == "i" && p[P_id].act < 1) {
            mciSendString(L"play ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
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
void p_walk(player *p,enemy *e,b_map *b_m,string chose, int P_id, int id, int psize, int bsize,int b_mid) {
        if (chose == "w" && p[P_id].move > 0) {
            int rx, ry, m, X, Y, range[15][20];
            BeginBatchDraw();
            IMAGE mb;
            loadimage(&mb, L"./Game/picture/mb.png", 0, 0, false);
            string walk;
            queue<pair<int, int>> q;
            clearQpair(q);
            q.push({ p[P_id].x,p[P_id].y });
            for (i = 0; i < b_m[b_mid].y; i++) {
                for (j = 0; j < b_m[b_mid].x; j++) {
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
                if (ss[X + 1][Y] == 0 && range[Y][X + 1] == 0 && X + 1 <b_m[b_mid].x) {
                    range[Y][X + 1] = 1;
                    q.push({ X + 1,Y });
                    transparentimage(NULL, 48 * (X + 1), 48 * Y, &mb);
                }
                if (ss[X - 1][Y] == 0 && range[Y][X - 1] == 0 && X - 1 >= 0) {
                    range[Y][X - 1] = 1;
                    q.push({ X - 1,Y });
                    transparentimage(NULL, 48 * (X - 1), 48 * Y, &mb);
                }
                if (ss[X][Y + 1] == 0 && range[Y + 1][X] == 0 && Y + 1 < b_m[b_mid].y) {
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
            IMAGE p1;
            wstring mm;
            LPCTSTR path;
            mm = std::to_wstring(P_id);
            mm = L"./Game/picture/p" + mm + L".png";
            path = mm.c_str();
            loadimage(&p1, path, 0, 0, false);
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
                                    mm = std::to_wstring(b_mid);
                                    mm = L"./Game/picture/b_map" + mm + L".png";
                                    path = mm.c_str();
                                    loadimage(&map, path, 0, 0, false);
                                    while (1) {
                                        putimage(0, 0, &map);
                                        if (box[w] == 2) {

                                            for (k = 0; k <= 48; k += 16) {

                                                BeginBatchDraw();
                                                putimage(0, 0, &map);
                                                e_put(e,bsize);
                                                transparentimage(NULL, xbox * 48, ybox * 48-16 + k, &p1, 0xFF55FF, 64 + 16, 0, 48, 64);
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
                                                transparentimage(NULL, xbox * 48 - k, ybox * 48-16, &p1, 0xFF55FF, 64 + 16, 64, 48, 64);
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
                                                transparentimage(NULL, xbox * 48 + k, ybox* 48-16, &p1, 0xFF55FF, 64 + 16, 128, 48, 64);
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
                                                transparentimage(NULL, xbox * 48, ybox * 48-16 - k, &p1, 0xFF55FF, 64 + 16, 192, 48, 64);
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
                                    mciSendString(L"play ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
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
            mciSendString(L"play ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
        }
    }
void e_attack(arms *ar,player *p,enemy *e,b_map *b_m,int b_mid,int id,int P_id,int bsize,int psize) {
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
                maps(p, P_id, e,b_m,b_mid);/*地圖繪製*/
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
                maps(p, P_id, e, b_m,b_mid);/*地圖繪製*/
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
        int box;
        for (i = 0; i < psize; i++) {
            box = abs(e[id].x - p[i].x) + abs(e[id].y - p[i].y);
            if (box < min) {
                min = box;
                P_id = i;
            }
        }
    }
void e_walk(enemy *e,player *p,int id,int P_id,int bsize,int psize,int b_mid) {
    if(abs(p[P_id].x-e[id].x)+abs(p[P_id].y - e[id].y)<=1) {
        return;
    }
        int box[1000], w = 0, k, xbox, ybox;
        Bbfs(e[id].x, e[id].y, p[P_id].x, p[P_id].y, box);
        xbox = e[id].x; ybox = e[id].y;
        wstring mm;
        mm = std::to_wstring(e[id].type);
        mm = L"./Game/picture/enemy" + mm + L".png";
        LPCTSTR path = mm.c_str();
        IMAGE enemy1;
        loadimage(&enemy1, path, 0, 0, false);
        mm = std::to_wstring(b_mid);
        mm = L"./Game/picture/b_map" + mm + L".png";
        path = mm.c_str();
        IMAGE map1;
        loadimage(&map1, path, 0, 0, false);
        while (e[id].move--) {
            putimage(0, 0, &map1);

            if (box[w] == 2) {

                for (k = 0; k <= 48; k += 16) {

                    BeginBatchDraw();
                    putimage(0, 0, &map1);
                    p_put(p, psize);
                    e_bput(e, id, bsize);
                    transparentimage(NULL, xbox * 48, ((ybox) * 48 + k), &enemy1, 0xFF55FF, 48, 0, 48, 48);
                    EndBatchDraw();
                    Sleep(30);
                    e[id].pose = 1;
                }
                ybox++;
            }
            else if (box[w] == 4) {
                for (k = 0; k <= 48; k += 16) {

                    BeginBatchDraw();
                    putimage(0, 0, &map1);
                    p_put(p, psize);
                    e_bput(e, id, bsize);
                    transparentimage(NULL, xbox * 48 - k,ybox * 48, &enemy1, 0xFF55FF, 48, 48, 48, 48);
                    EndBatchDraw();
                    Sleep(30);
                    e[id].pose = 2;
                }
                xbox--;

            }
            else if (box[w] == 6) {
                for (k = 0; k <= 48; k += 16) {

                    BeginBatchDraw();
                    putimage(0, 0, &map1);
                    p_put(p, psize);
                    e_bput(e, id, bsize);
                    transparentimage(NULL, xbox * 48 + k, ybox * 48, &enemy1, 0xFF55FF, 48, 96, 48, 48);
                    EndBatchDraw();
                    Sleep(30);
                    e[id].pose = 3;
                }
                xbox++;
            }
            else if (box[w] == 8) {
                for (k = 0; k <= 48; k += 16) {
                    BeginBatchDraw();

                    putimage(0, 0, &map1);
                    p_put(p, psize);
                    e_bput(e, id, bsize);
                    transparentimage(NULL, xbox * 48, (ybox * 48) - k, &enemy1, 0xFF55FF, 48, 144, 48, 48);
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
        ss[e[id].x][e[id].y] = 0;
        e[id].x = xbox; e[id].y = ybox; 
        ss[e[id].x][e[id].y] = 1;
        p_put(p, psize);
        e_put(e,bsize);
    }
void acts( player *p,enemy *e,b_map *b_m,string& chose,int b_mid, int P_id,int bsize,int id,int psize) {
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
                maps(p,P_id,e,b_m,b_mid);
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
void END(player *p,enemy *e,string chose,b_map *b_m,int b_mid, int& second, int& P_id, int& id,int& roundp, int& roundb) {
    int t, pmax, bmax;
    wstring mm;
    IMAGE enemy1, p1;
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
        for (i = 0; i < b_m[b_mid].psize; i++) {
            mm = std::to_wstring(i);
            mm = L"./Game/picture/A_p" + mm + L".png";
            LPCTSTR path = mm.c_str();
            loadimage(&p1, path, 0, 0, false);
            transparentimage(NULL, 1040, 500 - (p[i].turn * 3), &p1, 0xFF55FF);
           
        }
        for (i = 0; i < b_m[b_mid].esize; i++) {
            if (e[i].hp > 0) {
                mm = std::to_wstring(e[i].type);
                mm = L"./Game/picture/A_enemy" + mm + L".png";
                LPCTSTR path = mm.c_str();
                loadimage(&enemy1, path, 0, 0, false);
                transparentimage(NULL, 970, 500 - (e[i].turn * 3), &enemy1, 0xFF55FF);
            }
        }
        Sleep(60);
        EndBatchDraw();
            for (i = 0; i < b_m[b_mid].psize; i++) {
                if (p[i].turn >= 100) {
                    if (p[pmax].turn < p[i].turn || pmax == i) {
                        pmax = i;
                    }
                }
            }
            for (i = 0; i < b_m[b_mid].esize; i++) {
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
            for (i = 0; i < b_m[b_mid].psize; i++) {
                p[i].turn += p[i].speed;
            }
            for (i = 0; i < b_m[b_mid].esize; i++) {
                if (e[i].speed > 0) {
                    e[i].turn += e[i].speed;
                }
            }
            second++;
        }
    }
}
void Save(player* p, enemy* e, arms* ar, item* it,stone *st,flag *f,int P_id,int i_id,int ar_id,int st_id,int f_id,int second,int psize,int bsize,int roundp,int roundb,time_t first, time_t two, time_t three,string chose) {
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
                            sss = 0; mciSendString(L"play ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
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
                    fout << p[i].str << endl;
                    fout << p[i].INT << endl;
                    fout << p[i].con << endl;
                    fout << p[i].wis << endl;
                    fout << p[i].cha << endl;
                    fout << p[i].stone_id << endl;
                    fout << p[i].arms_id << endl;
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
                for (i = 0; i < ar_id; i++) {
                    fout << ar[i].mbullet << endl;
                    fout << ar[i].bullet << endl;
                    fout << ar[i].number << endl;
                }
                for (i = 0; i < i_id; i++) {
                    fout << it[i].number << endl;
                }
                for (i = 0; i < st_id; i++) {
                    fout << st[i].number << endl;
                }
                for (i = 0; i < f_id; i++) {
                    fout << f[i].check << endl;
                }
                fout << P_id << endl;
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
void Load(player* p, enemy* e, arms* ar, item* it, stone* st, flag* f, int &i_id, int &ar_id, int &st_id, int &f_id,  int &P_id, int &second, int &psize, int &bsize, int &roundp, int &roundb, time_t &first, time_t &two, time_t &three, string &chose) {
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
                            sss = 0; mciSendString(L"play ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
                        }
                    }
                    if (m.x >= 1230 && m.x <= 1278 && m.y >= 430 && m.y <= 478) {
                        j++; sss = 0;
                    }
                case WM_RBUTTONDOWN:
                    if (m.rbutton) {
                        sss = 0; ssss = 0;
                        a = ""; return;
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
            int k = 0, z[1000];
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
            three = z[0];
            if (z[1] == 0) {
                k = 2;
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
                    p[i].pose = z[k]; k++;
                    p[i].str = z[k]; k++;
                    p[i].INT = z[k]; k++;
                    p[i].con = z[k]; k++;
                    p[i].wis = z[k]; k++;
                    p[i].cha = z[k]; k++;
                    p[i].stone_id = z[k]; k++;
                    p[i].arms_id = z[k]; k++;
                }
                for (i = 0; i < ar_id; i++) {
                    ar[i].mbullet = z[k]; k++;
                    ar[i].bullet = z[k]; k++;
                    ar[i].number = z[k]; k++;
                }
                for (i = 0; i < i_id; i++) {
                    it[i].number = z[k]; k++;
                }
                for (i = 0; i < st_id; i++) {
                    st[i].number = z[k]; k++;
                }
                for (i = 0; i < f_id; i++) {
                    f[i].check = z[k]; k++;
                }
                P_id = z[k]; k++;
                second = z[k]; k++;

                first = time(NULL);
            }
            else if (z[1] == 1) {
                k = 2;
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
                    p[i].str = z[k]; k++;
                    p[i].INT = z[k]; k++;
                    p[i].con = z[k]; k++;
                    p[i].wis = z[k]; k++;
                    p[i].cha = z[k]; k++;
                    p[i].stone_id = z[k]; k++;
                    p[i].arms_id = z[k]; k++;
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
                for (i = 0; i < ar_id; i++) {
                    ar[i].mbullet = z[k]; k++;
                    ar[i].bullet = z[k]; k++;
                    ar[i].number = z[k]; k++;
                }
                for (i = 0; i < i_id; i++) {
                    it[i].number = z[k]; k++;
                }
                for (i = 0; i < st_id; i++) {
                    st[i].number = z[k]; k++;
                }
                for (i = 0; i < f_id; i++) {
                    f[i].check = z[k]; k++;
                }
                P_id = z[k]; k++;
                roundp = z[k]; k++;
                roundb = z[k]; k++;
                second = z[k]; k++;

                first = time(NULL);
                BeginBatchDraw();
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
    }

}
int battle_check(player *p,enemy *e,b_map *b_m,int b_mid) {
    for (i = 0; i < b_m[b_mid].esize; i++) {
        if (e[i].hp <= 0) {
        }
        else { return 0; }
    }
    return 1;
}
void menu_json_save(player* p, arms* ar, item* it, stone* st, flag* f, int P_id, int i_id, int ar_id, int st_id, int f_id, int second, int psize, time_t first, time_t two, time_t three,string a) {
    Json::Value root;
    two = time(NULL);
    int T = two - first + three;
    root["time"] = Json::Value(T);
    Json::Value player;
    for (i = 0; i < psize; i++) {
        player["lv"].append(p[i].lv);
        player["mhp"].append(p[i].mhp);
        player["hp"].append(p[i].hp);
        player["dex"].append(p[i].dex);
        player["move"].append(p[i].move);
        player["isize"].append(p[i].isize);
        player["asize"].append(p[i].asize);
        player["x"].append(p[i].x);
        player["y"].append(p[i].y);
        player["turn"].append(p[i].turn);
        player["speed"].append(p[i].speed);
        player["abox"].append(p[i].abox);
        player["pose"].append(p[i].pose);
        player["str"] .append(p[i].str);
        player["INT"] .append(p[i].INT);
        player["con"] .append(p[i].con);
        player["wis"].append(p[i].wis);
        player["cha"] .append(p[i].cha);
        player["stone_id"].append(p[i].stone_id);
        player["arms_id"].append(p[i].arms_id);
    }
    root["player"] = player;
    Json::Value arms;
    for (i = 0; i < ar_id; i++) {
        arms["mbullet"].append(ar[i].mbullet);
        arms["bullet"].append(ar[i].bullet);
        arms["number"].append(ar[i].number);
    }
    root["arms"] = arms;
    Json::Value item;
    for (i = 0; i < i_id; i++) {
        item["number"].append(it[i].number);
    }
    root["item"] = item;
    Json::Value stone;
    for (i = 0; i < st_id; i++) {
        stone["number"].append(st[i].number);
    }
    root["stone"] = stone;
    Json::Value flag;
    for (i = 0; i < f_id; i++) {
        flag["check"].append(f[i].check);
    }
    root["flag"] = flag;
    root["P_id"] =Json::Value( P_id);
    Json::StyledWriter sw;
    string file="./Game/save/"+a+".json";
    ofstream os;
    os.open(file, std::ios::out | std::ios::app);
    os << sw.write(root);
    os.close();
}
void menu_save(player* p, arms* ar, item* it, stone* st, flag* f, int P_id, int i_id, int ar_id, int st_id, int f_id, int second, int psize, time_t first, time_t two, time_t three) {
    while (1) {
        flushmessage(EM_MOUSE);
        int u = 0;
        string a, b = ".txt", read = "";
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
                string file = "./Game/save/" + a + ".json";
                Json::Reader reader;
                Json::Value root;
                ifstream in(file, ios::binary);
                if (in.is_open()) {
                    reader.parse(in, root);
                    int sto;
                    sto = root["time"].asInt();
                    in.close();
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
                string file = "./Game/save/" + a + ".json";
                Json::Reader reader;
                Json::Value root;
                ifstream in(file, ios::binary);
                if (in.is_open()) {
                    reader.parse(in, root);
                    int sto;
                    sto = root["time"].asInt();
                    in.close();
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
                            sss = 0; mciSendString(L"play ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
                        }
                    }
                    if (m.x >= 1230 && m.x <= 1278 && m.y >= 430 && m.y <= 478) {
                        j++; sss = 0;
                    }
                case WM_RBUTTONDOWN:
                    if (m.rbutton) {
                        sss = 0; ssss = 0;  u = 1; return;
                    }
                }
            }
        }
        if (u == 0) {
            menu_json_save(p, ar, it, st, f, P_id, i_id, ar_id, st_id, f_id, second, psize, first, two, three, a);
        }
    }
}
void menu_load(player* p, arms* ar, item* it, stone* st, flag* f, int P_id, int i_id, int ar_id, int st_id, int f_id, int second, int psize, time_t first, time_t two, time_t &three) {
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
            string file = "./Game/save/" + a + ".json";
            Json::Reader reader;
            Json::Value root;
            ifstream in(file, ios::binary);
            if (in.is_open()) {
                reader.parse(in, root);
                int sto;
                sto = root["time"].asInt();
                in.close();
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
            string file = "./Game/save/" + a + ".json";
            Json::Reader reader;
            Json::Value root;
            ifstream in(file, ios::binary);
            if (in.is_open()) {
                reader.parse(in, root);
                int sto;
                sto = root["time"].asInt();
                in.close();
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
                        sss = 0; mciSendString(L"play ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 from 0", NULL, 0, NULL);
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
    string file = "./Game/save/" + a + ".json";
    Json::Reader reader;
    Json::Value root;
    ifstream in(file, ios::binary);
    if (!in.is_open() && a != "") {
        putimage(400, 400, &saveblock);
        wchar_t s2[] = L"無此存檔";
        outtextxy(610, 440, s2);
        Sleep(1000);
    }
    else if (a == "") {

    }
    else {
        reader.parse(in, root);
        three = root["time"].asInt();
        for (i = 0; i < psize; i++) {
            p[i].lv = root["player"]["lv"][i].asInt();
            p[i].mhp = root["player"]["mhp"][i].asInt();
            p[i].hp = root["player"]["hp"][i].asInt();
            p[i].dex = root["player"]["dex"][i].asInt();
            p[i].move = root["player"]["move"][i].asInt();
            p[i].isize = root["player"]["isize"][i].asInt();
            p[i].asize = root["player"]["asize"][i].asInt();
            p[i].x = root["player"]["x"][i].asInt();
            p[i].y = root["player"]["y"][i].asInt();
            p[i].turn = root["player"]["turn"][i].asInt();
            p[i].speed = root["player"]["speed"][i].asInt();
            p[i].abox = root["player"]["abox"][i].asInt();
            p[i].pose = root["player"]["pose"][i].asInt();
            p[i].str = root["player"]["str"][i].asInt();
            p[i].INT = root["player"]["INT"][i].asInt();
            p[i].con = root["player"]["con"][i].asInt();
            p[i].wis = root["player"]["wis"][i].asInt();
            p[i].cha = root["player"]["cha"][i].asInt();
            p[i].stone_id = root["player"]["stone_id"][i].asInt();
            p[i].arms_id = root["player"]["arms_id"][i].asInt();
        }
        for (i = 0; i < ar_id; i++) {
            ar[i].mbullet = root["arms"]["mbullet"][i].asInt();
            ar[i].bullet = root["arms"]["bullet"][i].asInt();
            ar[i].number = root["arms"]["number"][i].asInt();
        }
        for (i = 0; i < i_id; i++) {
            it[i].number = root["item"]["number"][i].asInt();
        }
        for (i = 0; i < st_id; i++) {
            st[i].number = root["stone"]["number"][i].asInt();
        }
        for (i = 0; i < f_id; i++) {
            f[i].check = root["flag"]["check"][i].asInt();
        }
        P_id = root["P_id"].asInt();
        three = root["time"].asInt();
        first = time(NULL);
    }
}
void event_talk(player *p,npc *n) {
    int k = 0, w = 0, q = 0, P = 0, N = 0,C=0;
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    string a, b = ".txt";
    string number;
    IMAGE t_block, a1, tri,c1;
    ExMessage ww;
    loadimage(&t_block, L"./Game/picture/talkblock.png", 0, 0, false);
    loadimage(&tri, L"./Game/picture/talktri.png", 0, 0, false);
    putimage(0, 696, &t_block);
    a = std::to_string(i);
    string file = "./Game/story/event" + a + b;
    const char* c = file.c_str();
    std::wstring src = readFile(c);
    for (i = 0; i < src.size(); i++) {
        if (src[i] == 'n') {
            N = 1;
           if (C == 1) {
            file = "./Game/picture/cg" + number + ".png";
            wstring wfile = string2wstring(file);
            LPCTSTR path1 = wfile.c_str();
            loadimage(&c1, path1, 0, 0, false);
            putimage(0, 0, &c1);
            C = 0;
        }
            if (q == 1) {
                transparentimage(NULL, 630, 900, &tri, 0xFF55FF);
                flushmessage(EM_KEY);
                while (1) {
                    if (peekmessage(&ww, EM_KEY)) {
                        if (ww.vkcode == VK_SPACE) {
                            k = 0; w = 0;
                            flushmessage(EM_KEY);
                            Sleep(500);
                            putimage(0, 696, &t_block);
                            break;
                        }
                    }
                }
            }
            putimage(0, 696, &t_block);
            number = "";
        }
        else if (src[i] == 'p') {
            if (C == 1) {
            file = "./Game/picture/cg" + number + ".png";
            wstring wfile = string2wstring(file);
            LPCTSTR path1 = wfile.c_str();
            loadimage(&c1, path1, 0, 0, false);
            putimage(0, 0, &c1);
            C = 0;
        }
            P = 1;
            if (q == 1) {
                transparentimage(NULL, 630, 900, &tri, 0xFF55FF);
                flushmessage(EM_KEY);
                while (1) {
                    if (peekmessage(&ww, EM_KEY)) {
                        if (ww.vkcode == VK_SPACE) {
                            k = 0; w = 0;
                            flushmessage(EM_KEY);
                            Sleep(500);
                            putimage(0, 696, &t_block);
                            break;
                        }
                    }
                }
            }
            putimage(0, 696, &t_block);
            number = "";
        }
        else if (src[i] >= '0' && src[i] <= '9') {
            number += src[i];
        }
        else if (src[i] == '\n') {
            k++;
        }
        else if(src[i]=='c') {
            C = 1;
            number = "";
        }
        else {
            q = 1;
            if (N == 1) {
                settextcolor(BLACK);
                settextstyle(23, 0, _T("Taipei Sans TC Beta"));
                int J = stoi(number);
                LPCTSTR path = n[J].name.c_str();
                outtextxy(45, 710, path);
                if (J != 0) {
                    file = "./Game/picture/t_npc" + number + ".png";
                    wstring wfile = string2wstring(file);
                    LPCTSTR path1 = wfile.c_str();
                    loadimage(&a1, path1, 0, 0, false);
                    transparentimage(NULL, 0, 746, &a1, 0xFF55FF);
                }
                N = 0;
                settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                settextcolor(WHITE);
            }
            else if (P == 1) {
                settextcolor(BLACK);
                settextstyle(23, 0, _T("Taipei Sans TC Beta"));
                int J = stoi(number);
                LPCTSTR path = p[J].name.c_str();
                outtextxy(45, 710, path);
                file = "./Game/picture/player" + number + "_1.png";
                wstring wfile = string2wstring(file);
                LPCTSTR path1 = wfile.c_str();
                loadimage(&a1, path1, 0, 0, false);
                transparentimage(NULL, 0, 746, &a1, 0xFF55FF);
                P = 0;
                settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                settextcolor(WHITE);
            }
           
            wstring tt = L"";
            tt += src[i];
            LPCTSTR path = tt.c_str();
            outtextxy(30 * w + 150, 696 + k * 30+5, path);
            w++;
            Sleep(50);
        }

    }
    transparentimage(NULL, 630, 900, &tri, 0xFF55FF);
    flushmessage(EM_KEY);
    while (1) {
        if (peekmessage(&ww, EM_KEY)) {
            if (ww.vkcode == VK_SPACE) {
                Sleep(1000);
                break;
            }
        }
    }
}
/*txt版*/
void event(flag *f,player *p,npc *n,int &m_id) {
    i = 0;
    IMAGE p1,p2,p3;
    if (f[0].check == 0) {
        string filename;
        filename = "./Game/story/event" + to_string(0) + string(".json");
        const char* path = filename.c_str();
        readeventjson(p, n,path,m_id);
        f[0].check = 1;
    }
    else if (f[1].check ==0) {
        string filename;
        filename = "./Game/story/event" + to_string(1) + string(".json");
        const char* path = filename.c_str();
        readeventjson(p, n, path,m_id);
        f[1].check = 1;
    }
}
void n_put(npc* n,Map *M,int m_id) {
    IMAGE t;
    wstring m;
    string sum = "";
    for (int k = 0; k < M[m_id].npcid.size(); k++) {
        if (M[m_id].npcid[k] == '-') {
            m = std::to_wstring(n[stoi(sum)].avatar);
            m = L"./Game/picture/npc" + m + L".png";
            LPCTSTR path = m.c_str();
            loadimage(&t, path, 0, 0, false);

            transparentimage(NULL, n[stoi(sum)].x * 48, n[stoi(sum)].y * 48 - 16, &t, 0xFF55FF, 80, 0, 48, 64);
            k++;
            sum = "";
        }
        sum += M[m_id].npcid[k];
}
}
void m_put(player* p) {
    IMAGE p1;
    loadimage(&p1, L"./Game/picture/p0.png", 0, 0, false);
    if (p[0].pose == 1) {
            transparentimage(NULL, p[0].x * 48,p[0].y*48-16, &p1, 0xFF55FF, 64+16, 0, 48, 64);
    }
    else if (p[0].pose == 2) {
        transparentimage(NULL, p[0].x * 48, p[0].y * 48-16, &p1, 0xFF55FF, 64+16, 64, 48, 64);
    }
    else if (p[0].pose == 3) {
        transparentimage(NULL, p[0].x * 48, p[0].y * 48-16, &p1, 0xFF55FF, 64+16, 128,48, 64);
    }
    else if (p[0].pose == 4) {
        transparentimage(NULL, p[0].x * 48, p[0].y * 48-16, &p1, 0xFF55FF, 64+16, 192, 48, 64);
    }
}
void m_map(Map *m,int m_id,npc *n) {
    IMAGE map;
    wstring mm;
    mm=std::to_wstring(m_id);
    mm= L"./Game/map/map"+mm+L".png";
    LPCTSTR path=mm.c_str();
    loadimage(&map, path, 0, 0, false);
    putimage(0, 0, &map);
    for (int I = 0; I < m[m_id].nsize; I++) {
        n_put(n, m,m_id);
    }
}
string  m_act(player *p,Map *m,npc *n,int m_id) {
    ExMessage ww;
    char w;
        if (peekmessage(&ww, EM_KEY) ){
            switch (ww.message) {
            case WM_KEYDOWN:
                if (ww.vkcode == VK_DOWN) {
                    if (p[0].y < m[m_id].y - 1 && ss[p[0].x][p[0].y + 1] == 0) {
                        p[0].y++;
                            p[0].pose = 1;
                            return "w";
                    }
                    else {
                        p[0].pose = -1; return"w";
                    }
                }
                else if (ww.vkcode == VK_LEFT) {
                    if (p[0].x > 0 && ss[p[0].x - 1][p[0].y] == 0) {
                        p[0].x--;
                        p[0].pose = 2;
                        return"w";
                    }
                    else {
                        p[0].pose = -2; return"w";
                    }
                }
                else if (ww.vkcode == VK_RIGHT) {
                    if (p[0].x < m[m_id].x - 1 && ss[p[0].x + 1][p[0].y] == 0) {
                        p[0].x++;
                        p[0].pose = 3;
                        return"w";
                    }
                    else {
                        p[0].pose = -3; return"w";
                    }
                }
                else if (ww.vkcode == VK_UP) {
                    if (p[0].y > 0 && ss[p[0].x][p[0].y - 1] == 0) {
                        p[0].y--;
                        p[0].pose = 4;
                        return"w";
                    }
                    else {
                        p[0].pose = -4; return"w";
                    }
                }
                else if (ww.vkcode == VK_ESCAPE) {
                    return"esc";
                }
                else if (ww.vkcode == VK_SPACE) {
                    if (p[0].pose == 1) {
                        for (i = 0; i < m[m_id].nsize; i++) {
                            if (n[i].x == p[0].x && n[i].y == p[0].y + 1) {
                                return"t";
                            }
                        }
                    }
                    else if (p[0].pose == 2) {
                        for (i = 0; i < m[m_id].nsize; i++) {
                            if (n[i].x == p[0].x - 1 && n[i].y == p[0].y) {
                                return"t";
                            }
                        }
                    }
                    else if (p[0].pose == 3) {
                        for (i = 0; i < m[m_id].nsize; i++) {
                            if (n[i].x == p[0].x + 1 && n[i].y == p[0].y) {
                                return"t";
                            }
                        }
                    }
                    else if (p[0].pose == 4) {
                        for (i = 0; i < m[m_id].nsize; i++) {
                            if (n[i].x == p[0].x && n[i].y == p[0].y - 1) {
                                return"t";
                            }
                        }
                    }
                }
            }
        }
        return"";
}
void m_walk(Map *m,player *p,int m_id,npc *n,string &g) {
    if (p[0].pose < 0) {
        p[0].pose *= -1;
        m_put(p);
        g = "";
        return;
    }
    IMAGE p1;
    loadimage(&p1, L"./Game/picture/p0.png", 0, 0, false);
    if (p[0].pose == 1) {
        if (Z == 0) {
            transparentimage(NULL, p[0].x * 48, (p[0].y - 1) * 48 + 16*(Z+1) - 16, &p1, 0xFF55FF, 16, 0, 48, 64);
        }
        else if (Z == 1) {
            transparentimage(NULL, p[0].x * 48, (p[0].y - 1) * 48 + 16 * (Z+1) - 16, &p1, 0xFF55FF, 128 + 16, 0, 48, 64);
        }
        else if (Z == 2) {
            transparentimage(NULL, p[0].x * 48, (p[0].y - 1) * 48 + 16 * (Z+1) - 16, &p1, 0xFF55FF, 64 + 16, 0, 48, 64);            
            ss[p[0].x][p[0].y - 1] = 0;
            ss[p[0].x][p[0].y] = 1;
        }

        }
    else if (p[0].pose == 2) {
        if (Z == 0) {
            transparentimage(NULL, (p[0].x + 1) * 48 - 16 * (Z + 1), p[0].y * 48 - 16, &p1, 0xFF55FF, 16, 64, 48, 64);
        }
        else if (Z == 1) {
            transparentimage(NULL, (p[0].x + 1) * 48 -16 * (Z + 1), p[0].y * 48 - 16, &p1, 0xFF55FF, 128 + 16, 64, 48, 64);
        }
        else if (Z == 2) {
            transparentimage(NULL, (p[0].x + 1) * 48 -16 * (Z + 1), p[0].y * 48 - 16, &p1, 0xFF55FF, 64 + 16, 64, 48, 64);
            ss[p[0].x+ 1][p[0].y] = 0;
            ss[p[0].x][p[0].y] = 1;
        }

    }
    else if (p[0].pose == 3) {
        if (Z == 0) {
            transparentimage(NULL, (p[0].x - 1) * 48 + 16 * (Z + 1), p[0].y * 48 - 16, &p1, 0xFF55FF, 16, 128, 48, 64);
        }
        else if (Z == 1) {
            transparentimage(NULL, (p[0].x - 1) * 48 + 16 * (Z + 1), p[0].y * 48 - 16, &p1, 0xFF55FF, 128 + 16, 128, 48, 64);
        }
        else if (Z == 2) {
            transparentimage(NULL, (p[0].x - 1) * 48 + 16 * (Z + 1), p[0].y * 48 - 16, &p1, 0xFF55FF, 64 + 16, 128, 48, 64);
            ss[p[0].x-1 ][p[0].y] = 0;
            ss[p[0].x][p[0].y] = 1;
        }

        }
    else if (p[0].pose == 4) {
        if (Z == 0) {
            transparentimage(NULL, p[0].x * 48, (p[0].y + 1) * 48 - 16 * (Z + 1) - 16, &p1, 0xFF55FF, 16, 192, 48, 64);
        }
        else if (Z == 1) {
            transparentimage(NULL, p[0].x * 48, (p[0].y + 1) * 48 - 16 * (Z + 1) - 16, &p1, 0xFF55FF, 128 + 16, 192, 48, 64);
        }
        else if (Z == 2) {
            transparentimage(NULL, p[0].x * 48, (p[0].y + 1) * 48 - 16 * (Z + 1) - 16, &p1, 0xFF55FF, 64 + 16, 192, 48, 64);
            ss[p[0].x][p[0].y + 1] = 0;
            ss[p[0].x][p[0].y] = 1;
        }
        }
}
void m_set(Map* m, npc* n, player* p,e_npc *e_n ,int m_id) {
    int k, w, x, y; i = 0;
    string number="",box="";   
    string filename;
        filename= "./Game/map/map"+to_string(m_id) + string(".json");
    const char* path = filename.c_str();
    readmapjson(m,m_id,path);
    i = 0;
    for (w = 0; w < m[m_id].e_set.size(); w++) {
            if (m[m_id].e_set[w] == 'x') {
                w++;
                while (1) {
                    if (m[m_id].e_set[w] >= '0' && m[m_id].e_set[w] <= '9') {
                        number += m[m_id].e_set[w];
                        w++;
                    }
                    else {
                        e_n[i].x = stoi(number);number = "";
                        w--;
                        break;
                    }
                }
                
            }
            else  if (m[m_id].e_set[w] == 'y') {
                w++;
                while (1) {
                    if (m[m_id].e_set[w] >= '0' && m[m_id].e_set[w] <= '9') {
                        number += m[m_id].e_set[w];
                        w++;
                    }
                    else {
                        e_n[i].y = stoi(number); number = "";
                        w--;
                        break;
                    }
                }

            }
            else if (m[m_id].e_set[w] == 'z') {
                e_n[i].move = 0; e_n[i].pose = 1; e_n[i].type = 1; e_n[i].see = 0; e_n[i].live = 1; ss[e_n[i].x][e_n[i].y] = 1; i++;
            }
        }

    ss[p[0].x][p[0].y] = 1;
    for (i = 0; i < m[m_id].esize; i++) {
        for (j = 0; j < m[m_id].b_set.size(); j++) {
            if (m[m_id].b_set[j] == 'b') {
                while (1) {
                    j++;
                    if (m[m_id].b_set[j] >= '0' && m[m_id].b_set[j] <= '9') {
                        box += m[m_id].b_set[j];
                    }
                    else
                    {
                        e_n[i].battle = stoi(box);
                        box = "";
                        i++;
                        j--;
                        break;
                    }
                }
            }
        }
    }
    for (i = 0; i < m[m_id].npcid.size(); i++) {
        while (1) {            
            if (m[m_id].npcid[i] == '-') {
               ss[n[stoi(box)].x][n[stoi(box)].y]=1;
               break;
            }
            box += m[m_id].npcid[i];
            i++;
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
void talk(npc* n, player* p) {
    int k=0, w=0,q=0,P=0,N=0;
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    string a, b = ".txt";
    string number;
    IMAGE t_block,a1,tri;
    ExMessage ww;
    loadimage(&t_block, L"./Game/picture/talkblock.png", 0, 0, false);
    loadimage(&tri,L"./Game/picture/talktri.png",0,0,false);
    putimage(0, 696, &t_block);
    a = std::to_string(i);    
    string file = "./Game/story/talk" + a + b;
    const char* c = file.c_str();
    std::wstring src = readFile(c);
    for (i = 0; i < src.size(); i++) {
        if (src[i] == 'n') {
            N = 1; P = 0;
            if (q == 1) {
                transparentimage(NULL, 630, 900, &tri, 0xFF55FF);
                flushmessage(EM_KEY);
                while (1) {
                    if (peekmessage(&ww, EM_KEY)) {
                        if (ww.vkcode == VK_SPACE) {
                            k = 0; w = 0;
                            flushmessage(EM_KEY);
                            Sleep(500);
                            putimage(0, 696, &t_block);
                            break;
                        }
                    }
                }
            }
            putimage(0, 696, &t_block);
            number = "";
        }
        else if (src[i] == 'p') {
            P = 1; N = 0;
            if (q == 1) {
                transparentimage(NULL, 630, 900, &tri, 0xFF55FF);
                flushmessage(EM_KEY);
                while (1) {
                    if (peekmessage(&ww, EM_KEY)) {
                        if (ww.vkcode == VK_SPACE) {
                            k = 0; w = 0;
                            flushmessage(EM_KEY);
                            Sleep(500);
                            putimage(0, 696, &t_block);
                            break;
                        }
                    }
                }
            }
            putimage(0, 696, &t_block);
            number = "";
        }
        else if (src[i] >= '0' && src[i]<='9') {
            number += src[i];
        }
        else if (src[i] == '\n') {
            k++;
        }
        else {
            q = 1;
            if (N == 1) {
                settextcolor(BLACK);
                settextstyle(23, 0, _T("Taipei Sans TC Beta"));
                int J = stoi(number);
                LPCTSTR path = n[J].name.c_str();
                outtextxy(33, 710, path);
                file = "./Game/picture/t_npc" + number + ".png";
                wstring wfile = string2wstring(file);
                LPCTSTR path1 = wfile.c_str();
                loadimage(&a1, path1, 0, 0, false);
                transparentimage(NULL, 0, 746, &a1, 0xFF55FF);
                N = 0;
                settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                settextcolor(WHITE);
            }
            else if (P == 1) {
                settextcolor(BLACK);
                settextstyle(23, 0, _T("Taipei Sans TC Beta"));
                int J = stoi(number);
                LPCTSTR path = p[J].name.c_str();
                outtextxy(33, 710, path);
                file = "./Game/picture/player" + number + ".png";
                wstring wfile = string2wstring(file);
                LPCTSTR path1 = wfile.c_str();
                loadimage(&a1, path1, 0, 0, false);
                transparentimage(NULL, 0, 746, &a1, 0xFF55FF);
                P = 0;
                settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                settextcolor(WHITE);
            }
            wstring tt = L"";
            tt+=src[i];
            LPCTSTR path = tt.c_str();
            outtextxy(30*w+150, 696+k*30+5, path);
            w++;
            Sleep(50);
        }
        
    }
    transparentimage(NULL, 630, 900, &tri, 0xFF55FF);
    flushmessage(EM_KEY);
    while (1) {   
        if (peekmessage(&ww, EM_KEY)) {
            if (ww.vkcode == VK_SPACE) {
                Sleep(1000);
                break;
            }
        }
    }
}
string menu_item(player *p,item *it,int i_id) {
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    ExMessage m;
    IMAGE p1,p2,p3;
    RECT r = { 420, 90, 1250, 180 };
    int k = 0,J,chose=-1,ite[100];
    loadimage(&p1, L"./Game/picture/item_back.png", 0, 0, false);
    loadimage(&p3, L"./Game/picture/item_block.png", 0, 0, false);
    while (1) {
        BeginBatchDraw();
        putimage(400, 0, &p1);
        m = getmessage(EM_MOUSE );
        j = 0;
        if (m.rbutton) {
            return"";
        }
        for (i = 0; i < i_id; i++) {
            if (it[i].number != 0) {
                if (chose == j) {
                    return(it[i].type);
                }
                J = j / 2;
                wstring mm;
                mm = std::to_wstring(i);
                mm = L"./Game/picture/item" + mm + L".png";
                LPCTSTR path = mm.c_str();
                loadimage(&p2, path, 0, 0, false);
                if (j % 2 == 0){
                    transparentimage(NULL, 420, 200+J*40, &p2,0xFF55FF);
                    LPCTSTR path1 = it[i].Name.c_str();
                    outtextxy(445, 200 + J * 40, path1);
                    outtextxy(780, 200 + J * 40, _T(":"));
                    mm = std::to_wstring(it[i].number);
                    path1 = mm.c_str();
                    outtextxy(790, 200 + J * 40, path1);
                    ite[j] = i;
                    j++;
                    
                }
                else if (j % 2 == 1) {
                    transparentimage(NULL, 834, 200 + J * 40, &p2,0xFF55FF);
                    LPCTSTR path1 = it[i].Name.c_str();
                    outtextxy(859, 200 + J * 40, path1);
                    outtextxy(1194, 200 + J * 40, _T(":"));
                    mm = std::to_wstring(it[i].number);
                    path1 = mm.c_str();
                    outtextxy(1204, 200 + J * 40, path1);
                    ite[j] = i;
                    j++;
                }
            }
        }
        for (i = 0; i < j; i++) {
            if (m.x<824 + (i % 2) * 414 && m.x>410 + (i % 2) * 414 && m.y>197 + (i / 2) * 40 && m.y<232 + (i / 2) * 40)  {
                transparentimage(NULL, 410 + (i % 2) * 414, 197+ (i / 2) * 40 , &p3);
                LPCTSTR path1=it[ite[i]].story.c_str();
                drawtext(path1, &r, DT_WORDBREAK);
                if (m.lbutton) {
                    chose = i;
                }

            }

       }
        EndBatchDraw();
    }
}
void menu_player(player *p) {
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    IMAGE p1,p2,p3,p4;
    loadimage(&p1, L"./Game/picture/menu_back.png", 0, 0, false);
    loadimage(&p3, L"./Game/picture/larrow.png", 0, 0, false);
    loadimage(&p4, L"./Game/picture/rarrow.png", 0, 0, false);
    int I = 0;
    while (1) {
        settextstyle(30, 0, _T("Taipei Sans TC Beta"));
        BeginBatchDraw();
        putimage(400, 0, &p1);
        wstring mm;
        LPCTSTR path1;
        outtextxy(700, 15, _T("Lv"));
        mm = std::to_wstring(p[I].lv);
        path1 = mm.c_str();
        outtextxy(800, 15, path1);
        path1 = p[I].name.c_str();
        outtextxy(490,15, path1);
        mm = std::to_wstring(p[I].hp);
        path1 = mm.c_str();
        outtextxy(700, 65, _T("HP"));
        outtextxy(800, 65, path1);
        outtextxy(850, 65, _T("/"));
        mm = std::to_wstring(p[I].mhp);
        path1 = mm.c_str();
        outtextxy(860, 65, path1);
        settextstyle(40, 0, _T("Taipei Sans TC Beta"));
        outtextxy(500, 250, _T("力量"));
        mm = std::to_wstring(p[I].str);
        path1 = mm.c_str();
        outtextxy(650, 250, path1);
        outtextxy(500, 350, _T("敏捷"));
        mm = std::to_wstring(p[I].dex);
        path1 = mm.c_str();
        outtextxy(650, 350, path1);
        outtextxy(500, 450, _T("體質"));
        mm = std::to_wstring(p[I].con);
        path1 = mm.c_str();
        outtextxy(650, 450, path1);
        outtextxy(500, 550, _T("智力"));
        mm = std::to_wstring(p[I].INT);
        path1 = mm.c_str();
        outtextxy(650, 550, path1);
        outtextxy(500, 650, _T("感知"));
        mm = std::to_wstring(p[I].wis);
        path1 = mm.c_str();
        outtextxy(650, 650, path1);
        outtextxy(500, 750, _T("魅力"));
        mm = std::to_wstring(p[I].cha);
        path1 = mm.c_str();
        outtextxy(650, 750, path1);
        mm = std::to_wstring(I);
        mm = L"./Game/picture/player" + mm + L"_1.png";
        LPCTSTR path = mm.c_str();
        loadimage(&p2, path, 0, 0, false);
        transparentimage(NULL,450,45, &p2, 0xFF55FF);
        transparentimage(NULL, 415, 450, &p4, 0xFF55FF);
        transparentimage(NULL, 1230, 450, &p3, 0xFF55FF);
        EndBatchDraw();
        ExMessage m;
        m = getmessage(EM_MOUSE);
        if(m.x>415&&m.x<463&&m.y>450&&m.y<498) {
            if (m.lbutton&&I>0) {
                I--;
            }
        }
        else if (m.x>1230&&m.x<1278&&m.y>450&&m.y<498) {
            if (m.lbutton&&I<2) {
                I++;
            }
        }
        if (m.rbutton) {
            return;
        }
    }
}
void menu_equip(player* p, arms* ar,stone *st,int ar_id,int st_id) {
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    IMAGE p1, p2, p3, p4,p5,p6,p7,p8,p9;
    int I = 0,d=-1;
    wstring mm;
    LPCTSTR path1, path;
    RECT r = { 645, 35, 1200, 400 };
    loadimage(&p1, L"./Game/picture/equip_back.png", 0, 0, false);
    loadimage(&p3, L"./Game/picture/larrow.png", 0, 0, false);
    loadimage(&p4, L"./Game/picture/rarrow.png", 0, 0, false);
    loadimage(&p5, L"./Game/picture/equip_block.png", 0, 0, false);
    loadimage(&p8, L"./Game/picture/item_block.png", 0, 0, false);
    while (1) {
        BeginBatchDraw();
        putimage(400, 0, &p1);
        transparentimage(NULL, 415, 450, &p4, 0xFF55FF);
        transparentimage(NULL, 1230, 450, &p3, 0xFF55FF);
        mm = std::to_wstring(I);
        mm = L"./Game/picture/player" + mm + L"_1.png";
        path = mm.c_str();
        loadimage(&p2, path, 0, 0, false);
        path1 = p[I].name.c_str();
        outtextxy(490, 15, path1);
        transparentimage(NULL, 450, 45, &p2, 0xFF55FF);
        outtextxy(480, 200, _T("HP"));
        mm = std::to_wstring(p[I].mhp);
        path1 = mm.c_str();
        outtextxy(560, 200, path1);
        outtextxy(480, 230, _T("力量"));
        mm = std::to_wstring(p[I].str);
        path1 = mm.c_str();
        outtextxy(560, 230, path1);
        outtextxy(480, 260, _T("敏捷"));
        mm = std::to_wstring(p[I].dex);
        path1 = mm.c_str();
        outtextxy(560, 260, path1);
        outtextxy(480, 290, _T("體質"));
        mm = std::to_wstring(p[I].con);
        path1 = mm.c_str();
        outtextxy(560, 290, path1);
        outtextxy(480, 320, _T("智力"));
        mm = std::to_wstring(p[I].INT);
        path1 = mm.c_str();
        outtextxy(560, 320, path1);
        outtextxy(480, 350, _T("感知"));
        mm = std::to_wstring(p[I].wis);
        path1 = mm.c_str();
        outtextxy(560, 350, path1);
        outtextxy(480, 380, _T("魅力"));
        mm = std::to_wstring(p[I].cha);
        path1 = mm.c_str();
        outtextxy(560, 380, path1);
        outtextxy(865, 210, _T("武器"));
        mm = std::to_wstring(p[I].arms_id);
        mm = L"./Game/picture/arms" + mm + L".png";
        path1 = mm.c_str();
        loadimage(&p6, path1, 0, 0, false);
        transparentimage(NULL, 950, 215, &p6,0xFF55FF);
        path1 = ar[p[I].arms_id].name.c_str();
        outtextxy(1000, 213,path1);
        outtextxy(865, 250, _T("核心"));
        mm = std::to_wstring(p[I].stone_id);
        mm = L"./Game/picture/stone" + mm + L".png";
        path1 = mm.c_str();
        loadimage(&p9, path1, 0, 0, false);
        transparentimage(NULL, 950, 255, &p9, 0xFF55FF);
        path1 = st[p[I].stone_id].name.c_str();
        outtextxy(1000, 253, path1);
        ExMessage m;
        m = getmessage(EM_MOUSE);
        for (i = 0; i < 2; i++) {
            if (m.x >860 && m.x< 1210&& m.y>208+i*40 && m.y <243+i*40 ) {                    
                transparentimage(NULL, 860, 208 + i * 40, &p5);
                if (i==0) {

                    path1 = ar[p[I].arms_id].story.c_str();
                }
                else if (i == 1) {
                    path1 = st[p[I].stone_id].story.c_str();

                }
                drawtext(path1,&r, DT_WORDBREAK);
                if (m.lbutton) {
                    d = i+1;
                }
            }
        }
        int arm[100],sto[100];
        if (d == 1) {
            int J = 0, j = 0;
            for (i = 0; i < ar_id; i++) {
                if (ar[i].number != 0) {
                    arm[j] = i;
                    J = j / 2;
                    wstring mm;
                    mm = std::to_wstring(i);
                    mm = L"./Game/picture/arms" + mm + L".png";
                     path = mm.c_str();
                    loadimage(&p7, path, 0, 0, false);
                    if (j % 2 == 0) {
                        transparentimage(NULL, 420, 570 + J * 40, &p7, 0xFF55FF);
                         path1 = ar[i].name.c_str();
                        outtextxy(445, 570 + J * 40, path1);
                        outtextxy(780, 570 + J * 40, _T(":"));
                        mm = std::to_wstring(ar[i].number);
                        path1 = mm.c_str();
                        outtextxy(790, 570 + J * 40, path1);
                        j++;
                    }
                    else if (j % 2 == 1) {
                        transparentimage(NULL, 834, 570 + J * 40, &p7, 0xFF55FF);
                        path1 = ar[i].name.c_str();
                        outtextxy(859, 570 + J * 40, path1);
                        outtextxy(1194, 570 + J * 40, _T(":"));
                        mm = std::to_wstring(ar[i].number);
                        path1 = mm.c_str();
                        outtextxy(1204, 570 + J * 40, path1);
                        j++;
                    }
                }
            }
                        for (i = 0; i < j; i++) {

                            if (m.x > 420+(i%2)*414 && m.x < 830+(i%2)*414 && m.y>570+(i/2)*40 && m.y < 605+(i/2)*40) {
                                transparentimage(NULL, 415 + (i % 2) * 414,563 + (i / 2) * 40, &p8);     
                                path1 = ar[arm[i]].story.c_str();
                                drawtext(path1, &r, DT_WORDBREAK);
                                if (m.lbutton) {
                                    ar[p[I].arms_id].number++;
                                    p[I].arms_id = arm[i];
                                    ar[arm[i]].number--;
                                }
                            }
                       }
        }
        else if (d == 2) {
            int J = 0, j = 0;
            for (i = 0; i < st_id; i++) {
                if (st[i].number != 0) {
                    sto[j] = i;
                    J = j / 2;
                    wstring mm;
                    mm = std::to_wstring(i);
                    mm = L"./Game/picture/stone" + mm + L".png";
                     path = mm.c_str();
                    loadimage(&p7, path, 0, 0, false);
                    if (j % 2 == 0) {
                        transparentimage(NULL, 420, 570 + J * 40, &p7, 0xFF55FF);
                         path1 = st[i].name.c_str();
                        outtextxy(445, 570 + J * 40, path1);
                        outtextxy(780, 570 + J * 40, _T(":"));
                        mm = std::to_wstring(st[i].number);
                        path1 = mm.c_str();
                        outtextxy(790, 570 + J * 40, path1);
                        j++;
                    }
                    else if (j % 2 == 1) {
                        transparentimage(NULL, 834, 570 + J * 40, &p7, 0xFF55FF);
                        path1 = st[i].name.c_str();
                        outtextxy(859, 570 + J * 40, path1);
                        outtextxy(1194, 570 + J * 40, _T(":"));
                        mm = std::to_wstring(st[i].number);
                        path1 = mm.c_str();
                        outtextxy(1204, 570 + J * 40, path1);
                        j++;
                    }
                }
            }
            for (i = 0; i < j; i++) {

                if (m.x > 420 + (i % 2) * 414 && m.x < 830 + (i % 2) * 414 && m.y>570 + (i / 2) * 40 && m.y < 605 + (i / 2) * 40) {
                    transparentimage(NULL, 415 + (i % 2) * 414, 563 + (i / 2) * 40, &p8);
                    path1 = st[sto[i]].story.c_str();
                    drawtext(path1, &r, DT_WORDBREAK);
                    if (m.lbutton) {
                        change(p,I,st[p[I].stone_id].type,st[sto[i]].type);
                        st[p[I].stone_id].number++;
                        p[I].stone_id = sto[i];
                        st[sto[i]].number--;
                    }
                }
            }
        }


        if (m.x > 415 && m.x < 463 && m.y>450 && m.y < 498) {
            if (m.lbutton && I > 0) {
                I--;
            }
        }
        else if (m.x > 1230 && m.x < 1278 && m.y>450 && m.y < 498) {
            if (m.lbutton && I < 1) {
                I++;
            }
        }
        if (m.rbutton) {
            return;
        }
        EndBatchDraw();
    }
}
void menu_skill() {

}
void menu(player* p,item *it,arms *ar,stone *st,flag* f,enemy *e,int &P_id,int &f_id,int &i_id,int &ar_id,int &st_id, int &psize, int &bsize,int &roundp, int &roundb, int &second, time_t &first, time_t &two, time_t &three) {
    flushmessage(EM_KEY);
    mciSendString(L"open ./Game/Sound/SE/m-art_OK5.wav", NULL, 0, NULL);
    
    IMAGE p1, option,p2,p3,p4,p5,p6,p7;
    loadimage(&p1, L"./Game/picture/menu.png", 0, 0, false);
    loadimage(&option, L"./Game/picture/option.png", 0, 0, false);
    loadimage(&p2, L"./Game/picture/e_item.png", 0, 0, false);
    loadimage(&p3, L"./Game/picture/e_player.png", 0, 0, false);
    loadimage(&p4, L"./Game/picture/e_equip.png", 0, 0, false);
    loadimage(&p5, L"./Game/picture/e_skill.png", 0, 0, false);
    loadimage(&p6, L"./Game/picture/e_save.png", 0, 0, false);
    loadimage(&p7, L"./Game/picture/e_load.png", 0, 0, false);
    ExMessage m,m1;
    int u=0,c=0,g=-1;
    string chose = "";
    while (1) {
        u = 0;
        chose = "";
        BeginBatchDraw();
        putimage(0, 0, &p1);
        for (i = 0; i < 9; i++) {
            transparentimage(NULL, 0, 100 * i, &option, 0xFF55FF);
        }
        putimage(25, 20, &p2);
            putimage(0, 0, &p1);
            m = getmessage(EM_MOUSE|EM_KEY);
                if (m.vkcode == VK_ESCAPE) {
                    flushmessage(EM_KEY);
                    return;
                }
            for (i = 0; i < 9; i++) {
                if (m.x >= 0 && m.x < 350 && m.y>10 + i * 100&& m.y < (i + 1) * 100-10) {
                    transparentimage(NULL, 50, 100 * i, &option, 0xFF55FF); u=1;
                    if (i == 0) {
                        putimage(75, 20 + 100 * i, &p2);
                    }
                    else if (i == 1) {
                        putimage(75, 20 + 100 * i, &p3);
                    }
                    else if (i == 2) {
                        putimage(75, 20 + 100 * i, &p4);
                    }
                    else if (i == 3) {
                        putimage(75, 20 + 100 * i, &p5);
                    }
                    else if (i == 4) {
                        putimage(75, 20 + 100 * i, &p6);
                    }
                    else if (i == 5) {
                        putimage(75, 20 + 100 * i, &p7);
                    }
                    if (m.lbutton) {
                        g = i;
                    }
                    }
                else {
                    transparentimage(NULL, 0, 100 * i, &option, 0xFF55FF);
                    if (i == 0) {
                        putimage(25, 20 + 100 * i, &p2);
                    }
                    else if (i == 1) {
                        putimage(25, 20 + 100 * i, &p3);
                    }
                    else if (i == 2) {
                        putimage(25, 20 + 100 * i, &p4);
                    }
                    else if (i == 3) {
                        putimage(25, 20 + 100 * i, &p5);
                    }
                    else if (i == 4) {
                        putimage(25, 20 + 100 * i, &p6);

                    }
                    else if (i == 5) {
                        putimage(25, 20 + 100 * i, &p7);
                    }
                }
              }
        if (u == 1) {
            if (c == 0) {
                mciSendString(L"play SE1", NULL, 0, NULL);
            }
            c = 1;
        }
        else{ 
            if (c == 1) {
                mciSendString(L"close SE1", NULL, 0, NULL);
                mciSendString(L"open ./Game/Sound/SE/m-art_OK5.wav alias SE1", NULL, 0, NULL);
                c = 0;
            }
        }
        EndBatchDraw();
        if (g == 0) {
            chose = menu_item(p, it, i_id);
            g = -1;
        }
        else if (g == 1) {
            menu_player(p);
            g = -1;
        }
        else if (g == 2) {
            menu_equip(p, ar,st,ar_id,st_id);
            g = -1;
        }
        else if (g == 3) {

        }
        else if (g == 4) {
           menu_save(p, ar, it, st, f,P_id, i_id, ar_id, st_id,f_id, second, psize, first,two, three);
            return;
        }
        else if (g == 5) {
            menu_load(p, ar, it, st, f, P_id, i_id, ar_id, st_id, f_id, second, psize, first, two, three);
            return;
        }
    }
}
void m_e_ai(enemy* e, player* p, Map* m, e_npc* e_n, int m_id) {
    int k,w,box[100];
    for (k = 0; k < m[m_id].esize; k++) {
        if (e_n[k].see == 0&&e_n[k].live==1) {
            if (abs(e_n[k].x - p[0].x) + abs(e_n[k].y - p[0].y) <= 4) {
                mciSendString(L"close SE1", NULL, 0, NULL);
                mciSendString(L"open ./Game/Sound/SE/m-art_Select1.wav alias SE1", NULL, 0, NULL);
                mciSendString(L"play SE1", NULL, 0, NULL);
                Ai_bfs(e_n[k].x, e_n[k].y, p[0].x, p[0].y, box, m, m_id);
                if (box[0] == 2) {
                    e_n[k].pose = 1;
                    e_n[k].move = -1;
                    e_n[k].see = 1;
                    box[0] = 0;
                }
                else if (box[0] == 4) {
                    e_n[k].pose = 2;
                    e_n[k].move = -1;
                    e_n[k].see = 1;
                    box[0] = 0;
                }
                else if (box[0] == 6) {
                    e_n[k].pose = 3;
                    e_n[k].move = -1;
                    e_n[k].see = 1;
                    box[0] = 0;
                }
                else if (box[0] == 8) {
                    e_n[k].pose = 4;
                    e_n[k].move = -1;
                    e_n[k].see = 1;
                    box[0] = 0;
                }
            }
        }
    }
    for ( k = 0; k < m[m_id].esize; k++) {
        if (e_n[k].move == -1 && e_n[k].live == 1) {
            IMAGE p1;
            wstring mm;
            mm = std::to_wstring(k);
            mm = L"./Game/picture/enemy" + mm + L".png";
            LPCTSTR path = mm.c_str();
            loadimage(&p1, path, 0, 0, false);
            if (e_n[k].pose == 1) {
                if (e_n[k].y + 1 > m[m_id].y||ss[e_n[k].x][e_n[k].y+1]==1) {
                    transparentimage(NULL, e_n[k].x * 48, e_n[k].y * 48, &p1, 0xFF55FF, 48, 0, 48, 48);
                    e_n[k].type++;
                    if (e_n[k].type == 3) {
                        e_n[k].type = 1;
                        e_n[k].move = 0;
                        e_n[k].see = 0;
                    }
                }
                else if (e_n[k].type == 1) {
                    transparentimage(NULL, e_n[k].x * 48, e_n[k].y * 48 + 16 * e_n[k].type, &p1, 0xFF55FF, 0, 0, 48, 48);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 2) {
                    transparentimage(NULL, e_n[k].x * 48, e_n[k].y * 48 + 16 * e_n[k].type, &p1, 0xFF55FF, 96, 0, 48, 48);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 3) {
                    transparentimage(NULL, e_n[k].x * 48, e_n[k].y * 48 + 16 * e_n[k].type, &p1, 0xFF55FF, 48, 0, 48, 48);
                    e_n[k].type = 1;
                    e_n[k].move = 0;
                    e_n[k].y++;
                    ss[e_n[k].x][e_n[k].y-1] = 0;
                    ss[e_n[k].x][e_n[k].y] = 1;
                    e_n[k].see = 0;
                }

            }
            else if (e_n[k].pose == 2) {
                if (e_n[k].x - 1 < 0 || ss[e_n[k].x-1][e_n[k].y] == 1) {
                    transparentimage(NULL, e_n[k].x * 48, e_n[k].y * 48, &p1, 0xFF55FF, 48, 48, 48, 48);
                    e_n[k].type++;
                    if (e_n[k].type == 3) {
                        e_n[k].type = 1;
                        e_n[k].move = 0;
                        e_n[k].see = 0;
                    }
                }
                else if (e_n[k].type == 1) {
                    transparentimage(NULL, e_n[k].x * 48 - 16 * e_n[k].type, e_n[k].y * 48, &p1, 0xFF55FF, 0, 48, 48, 48);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 2) {
                    transparentimage(NULL, e_n[k].x * 48 - 16 * e_n[k].type, e_n[k].y * 48, &p1, 0xFF55FF, 96, 48, 48, 48);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 3) {
                    transparentimage(NULL, e_n[k].x * 48 - 16 * e_n[k].type, e_n[k].y * 48, &p1, 0xFF55FF, 48, 48, 48, 48);
                    e_n[k].type = 1;
                    e_n[k].move = 0;
                    e_n[k].x--;
                    ss[e_n[k].x+1][e_n[k].y] = 0;
                    ss[e_n[k].x][e_n[k].y] = 1;
                    e_n[k].see = 0;
                }
            }
            else if (e_n[k].pose == 3) {
                if (e_n[k].x+1> m[m_id].x || ss[e_n[k].x+1 ][e_n[k].y] == 1) {
                    transparentimage(NULL, e_n[k].x * 48, e_n[k].y * 48, &p1, 0xFF55FF, 48, 96, 48, 48);
                    e_n[k].type++;
                    if (e_n[k].type == 3) {
                        e_n[k].type = 1;
                        e_n[k].move = 0;
                        e_n[k].see = 0;
                    }
                }
                else if (e_n[k].type == 1) {
                    transparentimage(NULL, e_n[k].x * 48 + 16 * e_n[k].type, e_n[k].y * 48, &p1, 0xFF55FF, 0, 96, 48, 48);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 2) {
                    transparentimage(NULL, e_n[k].x * 48 + 16 * e_n[k].type, e_n[k].y * 48, &p1, 0xFF55FF, 96, 96, 48, 48);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 3) {
                    transparentimage(NULL, e_n[k].x * 48 + 16 * e_n[k].type, e_n[k].y * 48, &p1, 0xFF55FF, 48, 96, 48, 48);
                    e_n[k].type = 1;
                    e_n[k].move = 0;
                    e_n[k].x++;
                    ss[e_n[k].x - 1][e_n[k].y] = 0;
                    ss[e_n[k].x][e_n[k].y] = 1;
                    e_n[k].see = 0;
                }
            }
            else if (e_n[k].pose == 4) {
                if (e_n[k].y - 1 <0 || ss[e_n[k].x][e_n[k].y-1] == 1) {
                    transparentimage(NULL, e_n[k].x * 48, e_n[k].y * 48, &p1, 0xFF55FF, 48, 144, 48, 48);
                    e_n[k].type++;
                    if (e_n[k].type == 3) {
                        e_n[k].type = 1;
                        e_n[k].move = 0;
                        e_n[k].see = 0;
                    }
                }
                else if (e_n[k].type == 1) {
                    transparentimage(NULL, e_n[k].x * 48, e_n[k].y * 48 - 16 * e_n[k].type, &p1, 0xFF55FF, 0, 144, 48, 48);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 2) {
                    transparentimage(NULL, e_n[k].x * 48, e_n[k].y * 48 - 16 * e_n[k].type, &p1, 0xFF55FF, 96, 144, 48, 48);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 3) {
                    transparentimage(NULL, e_n[k].x * 48, e_n[k].y * 48 - 16 * e_n[k].type, &p1, 0xFF55FF, 48, 144, 48, 48);
                    e_n[k].type = 1;
                    e_n[k].move = 0;
                    e_n[k].y--;
                    ss[e_n[k].x ][e_n[k].y+1] = 0;
                    ss[e_n[k].x][e_n[k].y] = 1;
                    e_n[k].see = 0;
                }
            }

        }
        else if ((e_n[k].move == 0|| e_n[k].move == 1) && e_n[k].live == 1) {
            IMAGE p2;
            wstring mm;
            mm = std::to_wstring(k);
            mm = L"./Game/picture/enemy" + mm + L".png";
            LPCTSTR path = mm.c_str();
            loadimage(&p2, path, 0, 0, false);
            if (e_n[k].pose == 1) {
                transparentimage(NULL, e_n[k].x * 48, e_n[k].y * 48, &p2, 0xFF55FF, 48, 0, 48, 48);
            }
            else if (e_n[k].pose == 2) {
                transparentimage(NULL, e_n[k].x * 48, e_n[k].y * 48, &p2, 0xFF55FF, 48, 48, 48, 48);

            }
            else if (e_n[k].pose == 3) {
                transparentimage(NULL, e_n[k].x * 48, e_n[k].y * 48, &p2, 0xFF55FF, 48, 96, 48, 48);

            }
            else if (e_n[k].pose == 4) {
                transparentimage(NULL, e_n[k].x * 48, e_n[k].y * 48, &p2, 0xFF55FF, 48, 144, 48, 48);

            }
        }
    }
    if (F % 10 == 0) {
        for ( k = 0; k < m[m_id].esize; k++) {
            if (e_n[k].move == 1 && e_n[k].live == 1) {
                    int s = rand() % 10 + 1;
                    if (s > 3) {
                        s = rand() % 4 + 1;
                        if (s == 1) {
                            e_n[k].pose = 1; e_n[k].move = -1;
                        }
                        else if (s == 2) {
                            e_n[k].pose = 2; e_n[k].move = -1;
                        }
                        else if (s == 3) {
                            e_n[k].pose = 3; e_n[k].move = -1;
                        }
                        else if (s == 4) {
                            e_n[k].pose = 4; e_n[k].move = -1;
                        }
                    }
                    else {
                        e_n[k].move = 0;
                    }
                }
                else if (e_n[k].move == 0 && e_n[k].live == 1) {
                    int q;
                    q = rand() % 10 + 1;
                    if (q > 5) {
                        e_n[k].move = 1;
                    }
                }

        }
    }
}
int b_check(player *p,e_npc *e_n,Map *m,int m_id) {
    for (i = 0; i < m[m_id].esize; i++) {
        if (abs(e_n[i].x - p[0].x) + abs(e_n[i].y - p[0].y)==1 && e_n[i].live == 1) {
            e_n[i].live = 0;
            return e_n[i].battle;
        }
    }
    return -1;
}
void show(Map* m, player* p, enemy *e,e_npc *e_n,int m_id, npc* n,string &g) {
    IMAGE map;
    int k = 0;
    BeginBatchDraw();
    m_map(m,m_id,n);
    if (g == "w") {
        m_walk(m, p, m_id, n,g);
    }
    else { 
        m_put(p); 
    }
    m_e_ai(e, p, m, e_n, m_id);

    EndBatchDraw();
    F++;
}
void battle_set(enemy *e,enemy_type *e_t,player *p,b_map *b_m,int &b_mid) {
    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
    string box = "";
    int x, y,k=0,id,bgm;
    for (i = 0; i < b_m[b_mid].x; i++) {
        for (j = 0; j < b_m[b_mid].y; j++) {
            ss[i][j]=0;
        }
    }
    for (i = 0; i < b_m[b_mid].set.size(); i++) {
        if (b_m[b_mid].set[i] == 'b') {
            i++;
            while (1) {
                if (b_m[b_mid].set[i] >= '0' && b_m[b_mid].set[i] <= '9') {
                    box += b_m[b_mid].set[i];
                    i++;
                }
                else {
                    
                    wstring mm,mm1;
                    mm1 = string2wstring(box);
                    mm = L"open ./Game/Sound/bgm/bgm" + mm1 + L".mp3";
                    LPCTSTR path = mm.c_str();
                    mciSendString(path, NULL, 0, NULL);
                    mm= L"play ./Game/Sound/bgm/bgm" + mm1 + L".mp3";
                    path = mm.c_str();
                    mciSendString(path, NULL, 0, NULL);
                    i--;
                    break;
                }
            }
        }
        else if (b_m[b_mid].set[i] == 'x') {
            i++;
            while (1) {
                if (b_m[b_mid].set[i] >= '0' && b_m[b_mid].set[i] <= '9') {
                    box += b_m[b_mid].set[i];
                    i++;
                }
                else {
                    x = stoi(box);
                    box = "";
                    i--;
                    break;
                }
            }
        }
        else if (b_m[b_mid].set[i]=='y') {
            i++;
            while (1) {
                if (b_m[b_mid].set[i] >= '0' && b_m[b_mid].set[i] <= '9') {
                    box += b_m[b_mid].set[i];
                    i++;
                }
                else {
                    y = stoi(box);
                    box = "";
                    ss[x][y] = 1;
                    i--;
                    break;
                }
            }
        }
    }
    for (i = 0; i < b_m[b_mid].e_set.size(); i++) {
        if (b_m[b_mid].e_set[i] == 'x') {
            i++;
            while (1) {
                if (b_m[b_mid].e_set[i] >= '0' && b_m[b_mid].e_set[i] <= '9') {
                    box += b_m[b_mid].e_set[i];
                    i++;
                }
                else {
                    x = stoi(box);
                    box = "";
                    i--;
                    break;
                }
            }
        }
        else if (b_m[b_mid].e_set[i] == 'y') {
            i++;
            while (1) {
                if (b_m[b_mid].e_set[i] >= '0' && b_m[b_mid].e_set[i] <= '9') {
                    box += b_m[b_mid].e_set[i];
                    i++;
                }
                else {
                    y = stoi(box);
                    ss[x][y] = 1;
                    e[k].x = x;
                    e[k].y = y;
                    ss[x][y] = 1;
                    k++;
                    box = "";
                    i--;
                    break;
                }
            }
        }
        else if (b_m[b_mid].e_set[i] == 'e') {
            i++;
            while (1) {
                if (b_m[b_mid].e_set[i] >= '0' && b_m[b_mid].e_set[i] <= '9') {
                    box += b_m[b_mid].e_set[i];
                    i++;
                }
                else {
                    id = stoi(box);
                    box = "";
                    e[k].baid = e_t[id].baid; e[k].str = e_t[id].str; e[k].dex = e_t[id].dex; e[k].con = e_t[id].con; e[k].INT = e_t[id].INT; e[k].wis = e_t[id].wis; e[k].cha = e_t[id].cha; e[k].lv = e_t[id].lv; e[k].mhp = e_t[id].mhp; e[k].hp = e_t[id].hp; e[k].move = e_t[id].move; e[k].speed = e_t[id].speed; e[k].turn = 0; e[k].type = id; e[k].pose = 1;
                    i--;
                    break;
                }
            }
        }

    }
    b_m[b_mid].esize = k;
    k = 0;
    for (i = 0; i < b_m[b_mid].p_set.size(); i++) {
        if (b_m[b_mid].p_set[i] == 'x') {
            i++;
            while (1) {
                if (b_m[b_mid].p_set[i] >= '0' && b_m[b_mid].p_set[i] <= '9') {
                    box += b_m[b_mid].p_set[i];
                    i++;
                }
                else {
                    x = stoi(box);
                    box = "";
                    i--;
                    break;
                }
            }
        }
        else if (b_m[b_mid].p_set[i] == 'y') {
            i++;
            while (1) {
                if (b_m[b_mid].p_set[i] >= '0' && b_m[b_mid].p_set[i] <= '9') {
                    box += b_m[b_mid].p_set[i];
                    i++;
                }
                else {
                    y = stoi(box);
                    box = "";
                    p[k].x = x;
                    p[k].y = y;
                    p[i].turn = 0;
                    ss[x][y] = 1;
                    i--;
                    k++;
                    break;
                }
            }
        }
    }
    b_m[b_mid].psize = k;
}
void battle2map(player *p,npc *n,e_npc *e_n,Map *m,int m_id) {
    int k, w,x,y; i = 0;
    string number = "", box = "";
    for (i = 0; i < m[m_id].x; i++) {
        for (j = 0; j < m[m_id].y; j++) {
            ss[i][j] = 0;
        }
    }
    i = 0;
    for (i = 0; i < m[m_id].esize; i++) {
        if (e_n[i].live == 1) {
            ss[e_n[i].x][e_n[i].y]=1;
        }
    }
    for (i = 0; i < m[m_id].block.size(); i++) {
        if (m[m_id].block[i] == 'x') {
            i++;
            while (1) {
                if (m[m_id].block[i] >= '0' && m[m_id].block[i] <= '9') {
                    box += m[m_id].block[i];
                    i++;
                }
                else {
                    x = stoi(box);
                    box = "";
                    i--;
                    break;
                }
            }
        }
        else if (m[m_id].block[i] == 'y') {
            i++;
            while (1) {
                if (m[m_id].block[i] >= '0' && m[m_id].block[i] <= '9') {
                    box += m[m_id].block[i];
                    i++;
                }
                else {
                    y = stoi(box);
                    ss[x][y] = 1;
                    box = "";
                    i--;
                    break;
                }
            }
        }
    }
    p[0].x = m[m_id].px;
    p[0].y = m[m_id].py;
         ss[p[0].x][p[0].y] = 1;
         for (i = 0; i < m[m_id].npcid.size(); i++) {
             while (1) {
                 if (m[m_id].npcid[i] == '-') {
                     ss[n[stoi(box)].x][n[stoi(box)].y] = 1;
                     break;
                 }
                 box += m[m_id].npcid[i];
                 i++;
             }
         }

}
void m_check(player *p,Map *m,Exit *ex,int &m_id) {
    string s="";
    int uuu;
    for (i = 0; i < m[m_id].exitsize; i++) {
        for (j = 0; j < m[m_id].exit_set.size(); j++) {
            if (m[m_id].exit_set[j] == '_') {
                uuu = stoi(s);
                if (ex[uuu].state == true) {
                    if (p[0].x == ex[uuu].x) {

                    }
                }
            }
            else {
                s += m[m_id].exit_set[j];
            }
        }
    }
}
int main() {    
    HWND hwnd=initgraph(1296, 960);
    SetWindowPos(hwnd, HWND_TOP, 250, 10, 1296, 960,0);
    initgraph(1296, 960);
    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
    HWND hWnd = GetHWnd();
    SetWindowText(hWnd,L"RPG");
    srand(time(NULL));
    int  id=0, P_id=0,m_id=1, second=0,  load = 0,n_id=1,i_id=4,ar_id=5,st_id=2,f_id=2,b_mid;
    player p[3];
    enemy_type e_t[2];
    enemy e[2];
    arms  ar[5];
    item  it[4];
    Map   m[2];
    npc  n[3];
    stone st[2];
    flag f[2];
    e_npc e_n[100];
    b_map b_m[1];
    Exit EX[3];
    int ix, iy, abox1[1];
    int  t, psize = 1, bsize = 2, roundp = 0, roundb = 0;
    string  chose = "l" ;
    string a, b = ".txt", read = "";    
    time_t first=0, two=0,three=0;
    f[0].check = 0; f[1].check = 0;
    m[1].psize = 1; m[1].nsize = 0; m[1].esize = 0; m[1].exitsize = 1; m[1].exit_set = "0_";
    m[0].x = 27; m[0].y = 20; m[0].psize = 1; m[0].nsize = 1; m[0].esize = 3; m[0].npcid = "1-";  m[0].b_set = "b0b0b0z"; m[0].e_set = "x10y13zx5y5zx17y17z"; m[0].block = "x2y4"; m[0].msize = 1;
    b_m[0].x = 20; b_m[0].y = 15; b_m[0].esize = 1; b_m[0].set = "b0x3y2x1y2x7y2x18y1x13y2x10y3x14y4x16y4x18y4x19y4x3y5x1y6x9y6x12y6"; b_m[0].e_set = "e0x5y4"; b_m[0].p_set = "x12y14";
    p[0].name = L"艾倫"; p[0].story = L"白之子"; p[0].lv = 1; p[0].mhp = 12; p[0].hp = 12; p[0].dex = 10; p[0].move = 6; p[0].isize = 1; p[0].asize = 1; p[0].x = 10; p[0].y = 10; p[0].speed = 10; p[0].turn = 0; p[0].abox = 0; p[0].pose = 1; p[0].str = 10; p[0].INT = 10; p[0].con = 10; p[0].cha = 10; p[0].wis = 10; p[0].arms_id = 0; p[0].stone_id = 0;
    p[1].name = L"夏洛特"; p[1].story = L"獵人"; p[1].lv = 1; p[1].mhp = 12; p[1].hp = 12; p[1].dex = 10; p[1].move = 6; p[1].isize = 1; p[1].asize = 1; p[1].x = 10; p[1].y = 10; p[1].speed = 10; p[1].turn = 0; p[1].abox = 0; p[1].pose = 1; p[1].str = 10; p[1].INT = 10; p[1].con = 10; p[1].cha = 10; p[1].wis = 10; p[1].arms_id = 0; p[1].stone_id = 0;
    p[2].name = L"愛麗絲"; p[2].story = L"騎士"; p[2].lv = 1; p[2].mhp = 10; p[2].hp = 10; p[2].dex = 10; p[2].move = 6; p[2].isize = 1; p[2].asize = 1; p[2].x = 10; p[2].y = 10; p[2].speed = 12; p[2].turn = 0; p[2].abox = 0; p[2].pose = 1; p[2].str = 11; p[2].INT = 10; p[2].con = 11; p[2].cha = 10; p[2].wis = 10; p[2].arms_id = 2; p[2].stone_id = 1;
    e_t[0].name = L"野狼"; e_t[0].story = L"團體行動的動物 隨著數量增加危險性也會大幅上升"; e_t[0].baid = 1; e_t[0].str = 12; e_t[0].dex = 15; e_t[0].con = 12; e_t[0].INT = 3; e_t[0].wis = 14; e_t[0].cha = 7; e_t[0].lv = 1; e_t[0].mhp = 11; e_t[0].hp = 11; e_t[0].move = 7; e_t[0].speed = 12;
    ar[0].name = L"栓動步槍"; ar[0].dmg = "2d8"; ar[0].Dmg = L"2d8"; ar[0].range = 5; ar[0].story = L"精準可靠的動能武器\n傷害:2d8"; ar[0].number = 1; ar[0].bullet = 5; ar[0].mbullet = 5;
    ar[1].name = L"爪子"; ar[1].dmg = "2d4+2"; ar[1].Dmg = L"2d4+2"; ar[1].hit = "1d4+2"; ar[1].range = 1; ar[1].number = 0; ar[1].bullet = 1; ar[1].mbullet = 1;
    ar[2].name = L"長劍"; ar[2].dmg = "1d8"; ar[2].Dmg = L"1d8"; ar[2].hit = "1d2"; ar[2].range = 1; ar[2].story = L"在大陸上廣泛使用的制式長劍\n傷害:1d8\n命中加值:1d2"; ar[2].number = 1; ar[2].bullet = 1; ar[2].mbullet = 1;
    ar[3].name = L"石中劍"; ar[3].dmg = "1d8"; ar[3].Dmg = L"1d8"; ar[3].hit = "1d2"; ar[3].range = 1; ar[3].story = L"傳說只有神選之人才能使用的聖劍\n傷害:1d8\n命中加值:1d2"; ar[3].number = 77; ar[3].bullet = 1; ar[3].mbullet = 1;
    ar[4].name = L"長劍2"; ar[4].dmg = "1d8"; ar[4].Dmg = L"1d8"; ar[4].hit = "1d2"; ar[3].range = 1; ar[4].story = L"在大陸上廣泛使用的制式長劍2\n傷害:1d8\n命中加值:1d2"; ar[4].number = 88; ar[4].bullet = 1; ar[4].mbullet = 1;
    it[0].Name = L"步槍鉛彈"; it[0].name = "步槍鉛彈"; it[0].number = 30; it[0].type = "B"; it[0].story = L"鉛製外殼可以裝填步槍的子彈\n傷害加值:2\n命中加值:2";
    it[1].Name = L"繃帶"; it[1].name = "繃帶"; it[1].number = 10; it[1].type = "B"; it[1].story = L"";
    it[2].Name = L"通常彈2"; it[2].name = "通常彈"; it[2].number = 10; it[2].type = "B"; it[2].story = L"";
    it[3].Name = L"手槍鉛彈"; it[3].name = "步槍鉛彈"; it[3].number = 99; it[3].type = "B"; it[3].story = L"鉛製外殼可以裝填手槍和衝鋒槍的子彈\n傷害加值:1";
    st[0].name = L"長青石"; st[0].number = 1; st[0].story = L"從星隕礦採掘出的伴生礦 沾染了生命的氣息\nHP+2"; st[0].type = "H+2";
    st[1].name = L"烈風石"; st[1].number = 1; st[1].story = L"從星隕礦採掘出的伴生礦 沾染了狂風的氣息\n速度+2"; st[1].type = "S+2";
    n[0].name = L"旁白";
    n[1].name = L"朱利安"; n[1].x = 3; n[1].y=6; n[1].avatar=1;
    n[2].name = L"???";     
    EX[0].x = 14; EX[0].y = 13; EX[0].gx = 6; EX[0].gy = 12; EX[0].Mid = 2; EX[0].state = true;
    while (1) {
        int s = start();
        if (s == 1) {
            first = time(NULL);
            event(f, p, n,m_id);
            /*事件在這裡event(f,p,n);*/
            break;
        }
        else if (s == 0) {
            menu_load(p, ar, it, st, f, P_id, i_id, ar_id, st_id, f_id, second, psize, first, two,three);
            if (three>0 ) {
                first = time(NULL);
                break;
            }
        }
    }
    F = 0;
        b_mid = -1;
        
            m_map(m, m_id, n); m_put(p); m_set(m, n, p, e_n, m_id); m_put(p);
            string g;
            Z = 2;
   while (1) {
            while (1) {
                event(f, p, n,m_id);
                b_mid = b_check(p, e_n, m, m_id);
                /*!=-1*/
                if (b_mid == -2) {
                    m[m_id].px = p[0].x;
                    m[m_id].py = p[0].y;
                    battle_set(e, e_t, p, b_m, b_mid); 
                    BeginBatchDraw();
                    maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                    e_put(e, b_m[b_mid].esize);
                    p_put(p, b_m[b_mid].psize);
                    EndBatchDraw();
                    END(p, e, "e",b_m,b_mid, second, P_id, id, roundp, roundb);
                    break;
                }
                if (Z == 2) {
                    g = m_act(p, m, n, m_id);
                    flushmessage(EM_KEY);
                    if (g == "w") {
                        Z = 0;
                    }
                }
                else {
                    Z++;
                }
                if (g == "t") {
                    string filename;
                    filename = "./Game/story/event" + to_string(i) + string(".json");
                    const char* path = filename.c_str();
                    readtalkjson(p, n, path);
                    flushmessage(EM_KEY);
                    m_map(m, m_id, n); m_put(p); n_put(n, m, m_id);
                }
                if (g == "esc") {
                    Sleep(500);
                    rewind(stdin);
                    menu(p, it, ar, st, f, e, P_id, f_id, i_id, ar_id, st_id, psize, bsize, roundp, roundb, second, first, two, three);
                    m_map(m, m_id, n);
                    m_put(p);
                    EndBatchDraw();
                }
                show(m, p, e, e_n, m_id, n, g);

            }

        mciSendString(L"open ./Game/Sound/SE/魔王魂_戦闘18.mp3", NULL, 0, NULL);
        mciSendString(L"open ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3", NULL, 0, NULL);
        while (1) {
            if (battle_check(p, e, b_m, b_mid) == 1) {
                m_map(m, m_id, n); m_put(p); battle2map(p,n,e_n,m,m_id); break;
            }
            if (roundp == 1) {
                BeginBatchDraw();
                maps(p, P_id, e,b_m,b_mid);/*地圖繪製*/
                e_put(e, b_m[b_mid].esize);
                p_put(p, b_m[b_mid].psize);
                EndBatchDraw();
                acts(p, e, b_m,chose,b_mid, P_id, b_m[b_mid].esize, id, b_m[b_mid].psize);/*選項*/
                Save(p, e, ar, it, st, f, P_id, i_id, ar_id, st_id, f_id, second, psize, bsize, roundp, roundb, first, two, three, chose);
                Load(p, e, ar, it, st, f, i_id, ar_id, st_id, f_id, P_id, second, psize, bsize, roundp, roundb, first, two, three, chose);
                p_attack(p, e, ar, b_m,b_mid,P_id, id, chose, b_m[b_mid].esize, b_m[b_mid].psize);/*攻擊*/
                p_item(p, ar, it, chose, P_id);/*物品*/
                p_walk(p, e,b_m, chose, P_id, id, psize, bsize,b_mid);/*移動*/
                END(p, e, chose, b_m, b_mid, second, P_id, id,roundp, roundb);/*結束行動*/
                /*存檔*/
                /*讀檔*/
            }
            else if (roundb == 1) {
                BeginBatchDraw();
                maps(p, P_id, e, b_m, b_mid);
                e_put(e, b_m[b_mid].esize);
                p_put(p, b_m[b_mid].psize);
                EndBatchDraw();
                e_target(e, p, P_id, id, b_m[b_mid].psize);
                e_walk(e, p, id, P_id, bsize, psize,b_mid);
                e_attack(ar, p, e,b_m,b_mid, id, P_id, bsize, psize);
                END(p, e, "e", b_m, b_mid, second, P_id, id, roundp, roundb);/*結束行動*/
            }
        }
    }
}
