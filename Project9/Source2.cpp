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
#include<chrono>
using namespace std;
int i, j, ss[200][150], F = 0, Z = 0; int gg = 0; int voiceSize = 500; int soundSize = 500;/*F=時間計數器*/
clock_t ta=0, tb;
string s[26][60];
IMAGE mapP,text, back1, aline;
wchar_t vo[50];
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
void transparentimageA(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor,int a,int b,int c,int d)
{
    HDC dstDC = GetImageHDC(dstimg);
    HDC srcDC = GetImageHDC(srcimg);
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(dstDC, x, y, c, d, srcDC, a, b, c, d, bf);
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
    if (d == "0") {
        return 0;
    }
    int t = 0, s = 0, g = 0, n = 1, k, sum = 0;
    string box = "";
    if (a == 1) {
        for (k = 0; k < d.size(); k++) {
            if (d[k] == 'd') {
                t = stoi(box);
                box = "";
                k++;
            }
            if (d[k] == '+') {
                s = stoi(box);
                g = 1;
                box = "";
                k++;
            }
            else if (d[k] == '-') {
                s = stoi(box);
                g = -1;
                box = "";
                k++;
            }
            box += d[k];
        }
    }
    else if (a == 2) {
        int x = rand() % 20 + 1;
        return x;
    }    
    if (s != 0) {
        if (g == 1) {
            g *= stoi(box);
        }
        else if (g == -1) {
            g *= stoi(box);
       }
    }
    else {
        s = stoi(box);
    }
    while (t--) {
        sum += rand() % s + 1;
    }
    return sum + g;
}
class flag {
public:
    int check=0;
};
class player{
public: 
    wstring name, story;
    int lv ,mhp,hp,dex,move,isize,asize,x,y,turn,speed,abox,act,pose,str,INT,con,wis,cha,arms_id_1,arms_id_2,armor_id,stone_id,exp,Move,Act,t_id=-1;
    int dexExp=50, strExp=50, intExp=50, conExp=50, wisExp=50, chaExp=50;
    int dexUp,strUp, intUp, conUp, wisUp, chaUp;
    int s_check[100] = { 0 };
    int buff_check[100] = {0};
    int state=0;/*在不在隊伍*/
    int box_id[3];
    int box_size[3];
    int buff_Size=0;
    int buff_time[50];
    int buff_id[50];
    int arms_b_1,arms_b_2;
    int b_id_1,b_id_2;
    string buff_give[50];
    player() {
        for (i = 0; i < 3; i++) {
            box_id[i] = -1;
            box_size[i] = 0;
     }
    }

};
class enemy_type {
public:
    wstring name, story;
    int lv, mhp, hp, dex, str, x, y, INT, con, wis, cha, speed, move, baid,exp,Move;
};
class enemy {
public:
    wstring name, story;
    int lv, mhp,hp,dex, str,x,y,speed, INT, con, wis, cha,turn,move,baid,pose,type,exp,Move;
    int target = 0;
    /*0為鎖定玩家 1為鎖定npc*/
    int buff_Size = 0;
    int buff_time[50];
    int buff_id[50];
};
class b_npc_type {
public:
    wstring name, story;
    int lv, mhp, hp, dex, str, x, y, INT, con, wis, cha, speed, move, baid, exp,Move,pose;
};
class b_npc {
public:
    wstring name, story;
    int lv, mhp, hp, dex, str, x, y, INT, con, wis, cha, speed, move, baid, exp,pose,type,Move,turn;
    int buff_Size = 0;
    int buff_time[50];
    int buff_id[50];
    int buff_check[100] = { 0 };
    string buff_give[50];
};
class arms {
public:
    wstring name,Dmg,story;
    string  dmg, hit="0", type;
    int range,mbullet, bullet,number;
};
class armor {
public:
    wstring name, story;
};
class item {
public:
    wstring Name,story;
    string name;
    char type;
    int number;
    int range;
};
class Map {
public:
    int x;
    int y;
    int psize;
    int nsize;
    int esize;
    int px, py;
    int ox, oy;
    int mevent_size;
    int exitsize;
    int box_size;
    int bgm;
    string block,npcid,e_set,b_set,exit_set,mevent_set,box_set;
    int team_state[100] = { 0 };
};
class b_map {
public:
    int x;
    int y;
    int psize;
    int nsize;
    int esize;
    int fsize;
    int bgm=-1;
    int ox = 0;
    int oy = 0;
    int cx;
    int cy;
    int time = 0;
    int line=0;
    string e_set, set, p_set, f_set, log_name="",n_set;
};
class npc {
public:
    wstring name;
    int avatar,x,y;
};
class e_npc {
public:
    int x, y,move,pose,type,see,battle,live=1,kind;
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
    int cx;
    int cy;
    int Mid;
    bool state;
};
class m_flag {
public:
    int check;
    int x;
    int y;
};
class BOX {
public :
    int x, y;
    bool state;
    bool check;
};
class b_flag {
public:
    int id;
    int check;
};
class buff {
public:
    wstring name, story;
    string type;
};
class terrain {
public:
    int type, mA, mB,npc=0,player=0,enemy=0;
};
class t_equip {
public:
    wstring name,story;
    int number,size;
};
void ui(player* p, enemy* e, b_map* b_m, int P_id, int esize, int psize, int b_mid, int bu_id);
void p_put(player* p, b_map* b_m, int psize, int b_mid);
void b_nput(b_npc* b_n, b_map* b_m, int b_nid, int nsize, int b_mid);
void e_put(enemy* e, b_map* b_m, int esize, int b_mid);
void maps(player* p, int P_id, enemy* e, b_map* b_m, int b_mid);
int p_buff_check(string chose, player* p,enemy *e,b_npc *b_n, buff* bu, b_map* b_m, int P_id, int b_mid,int bu_id) {
    int I, k,bi,bt;
    string box = "";
    IMAGE be;
    if (chose[0] == 't') {
        if (p[P_id].buff_check[0] >= 1) {
            if (b_m[b_mid].time % 30 == 0) {
                for (k = 0; k < b_m[b_mid].psize; k++) {
                    p[k].buff_check[2]++;
                }
            }
        }
    }
    else if (chose[0] == 'a') {
        string box = "";
        for (int q = 1; q < chose.size(); q++) {
            box += chose[q];
        }
        return p[P_id].buff_check[stoi(box)];
    }
    else if (chose[0] == 'T') {
        for (I = 0; I < b_m[b_mid].psize; I++) {
        for (int q = 0; q < p[I].buff_Size; q++) {
            p[I].buff_time[q]--;
            if (p[I].buff_time[q] == 0) {
                if (p[I].buff_id[q] == 4) {
                    if (p[I].buff_give[q][0] == 'i') {
                        box = "";
                        for (bi = 1; bi < p[I].buff_give[q].size(); bi++) {
                            if (p[I].buff_give[q][bi] == 'w') {
                                    p[I].turn += 10000;
                                    bi++;
                            }
                            box += p[I].buff_give[q][bi];
                        }
                        bt = stoi(box);
                        if (bt == 1) {
                        if (p[I].hp + 5 > p[I].mhp) {
                        p[I].hp = p[I].mhp;
                        }
                        else {
                            p[I].hp += 5;
                         }
                         p[I].buff_check[6] += 2;                        
                    mciSendString(L"close se", NULL, 0, NULL);
                    mciSendString(L"open ./Game/Sound/SE/回復魔法4.mp3 alias se", NULL, 0, NULL);
                    wstring vos = L"setaudio se volume to " + to_wstring(soundSize);
                    mciSendString(vos.c_str(), NULL, 0, NULL);
                    mciSendString(L"play se from 0", NULL, 0, NULL);
                    loadimage(&be, L"./Game/picture/魔・回復2.png", 0, 0, false);
                    for (j = 0; j < 5; j++) {
                    for (i = 0; i < 3; i++) {
                        BeginBatchDraw();
                        maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                        e_put(e, b_m, b_m[b_mid].esize, b_mid);
                        b_nput(b_n, b_m, b_m[b_mid].nsize, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, b_m[b_mid].psize, b_mid);
                        transparentimageA(NULL, p[I].x * 48 - b_m[b_mid].ox-30, p[I].y * 48 - b_m[b_mid].oy-60, &be, 0x000000, i * 120, j * 120, 120, 120);
                        ui(p, e, b_m, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, bu_id);
                        EndBatchDraw();
                        Sleep(50);
                    }
                    }
                 }
                }
                    else if (p[I].buff_give[q][0] == 'w') {
                        p[I].turn += 10000;
                    }
                }
                else if (p[I].buff_id[q] == 5) {
                    if (p[I].buff_give[q][0] == 'i') {
                        box = "";
                        for (bi = 1; bi < p[I].buff_give[q].size()-1; bi++) {
                            if (p[I].buff_give[q][bi] == 'w') {
                                p[I].turn += 10000;
                                bi++;
                            }
                            box += p[I].buff_give[q][bi];
                        }
                        bt = stoi(box);
                            if (bt == 0) {
                                if (p[i].buff_give[q][bi] == 'a') {
                               
                                }
                                else if (p[i].buff_give[q][bi] == 'A') {
                                
                                }
                            }
                        }
                    }
                }
                p[I].buff_check[p[I].buff_id[q]]--;
                for (int e = q; e < p[I].buff_Size-1; e++) {
                    p[I].buff_time[e] = p[I].buff_time[e + 1];
                    p[I].buff_id[e] = p[I].buff_id[e + 1];
                    p[I].buff_give[e] = p[I].buff_give[e + 1];
                }
                p[I].buff_Size--;
                q--;
            }
        }
        }       
    else if (chose[0] == 'r') {

            if (p[P_id].buff_check[6] > 0) {
            p[P_id].buff_check[6]--;
            if (p[P_id].hp+3>p[P_id].mhp) {
                p[P_id].hp = p[P_id].mhp;
            }
            else {
                p[P_id].hp += 3;
            }
            }

      
    }
}
void n_buff_check(string chose, player* p, enemy* e, b_npc* b_n, buff* bu, b_map* b_m, int P_id, int b_mid, int bu_id) {
    int I,k,bi,bt;
    string box = "";
    IMAGE be;
    if (chose[0] == 'T') {
        for (I = 0; I < b_m[b_mid].nsize; I++) {
            for (int q = 0; q < b_n[I].buff_Size; q++) {
                b_n[I].buff_time[q]--;
                if (b_n[I].buff_time[q] == 0) {
                    if (b_n[I].buff_id[q] == 4) {
                        if (b_n[I].buff_give[q][0] == 'i') {
                            box = "";
                            for (bi = 1; bi < b_n[I].buff_give[q].size(); bi++) {
                                box += b_n[I].buff_give[q][bi];
                            }
                            bt = stoi(box);
                            if (bt == 1) {
                                if (b_n[I].hp + 5 > b_n[I].mhp) {
                                    b_n[I].hp = b_n[I].mhp;
                                }
                                else {
                                    b_n[I].hp += 5;
                                }
                                b_n[I].buff_check[6] += 2;
                                mciSendString(L"close se", NULL, 0, NULL);
                                mciSendString(L"open ./Game/Sound/SE/回復魔法4.mp3 alias se", NULL, 0, NULL);
                                wstring vos = L"setaudio se volume to " + to_wstring(soundSize);
                                mciSendString(vos.c_str(), NULL, 0, NULL);
                                mciSendString(L"play se from 0", NULL, 0, NULL);
                                loadimage(&be, L"./Game/picture/魔・回復2.png", 0, 0, false);
                                for (j = 0; j < 5; j++) {
                                    for (i = 0; i < 3; i++) {
                                        BeginBatchDraw();
                                        maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                                        e_put(e, b_m, b_m[b_mid].esize, b_mid);
                                        b_nput(b_n, b_m, b_m[b_mid].nsize, b_m[b_mid].nsize, b_mid);
                                        p_put(p, b_m, b_m[b_mid].psize, b_mid);
                                        transparentimageA(NULL, b_n[I].x * 48 - b_m[b_mid].ox - 30, b_n[I].y * 48 - b_m[b_mid].oy - 60, &be, 0x000000, i * 120, j * 120, 120, 120);
                                        ui(p, e, b_m, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, bu_id);
                                        EndBatchDraw();
                                        Sleep(50);
                                    }
                                }
                            }
                        }
                    }
                    b_n[I].buff_check[b_n[I].buff_id[q]]--;
                    for (int e = q; e < b_n[I].buff_Size - 1; e++) {
                        b_n[I].buff_time[e] = b_n[I].buff_time[e + 1];
                        b_n[I].buff_id[e] = b_n[I].buff_id[e + 1];
                        b_n[I].buff_give[e] = b_n[I].buff_give[e + 1];
                    }
                    b_n[I].buff_Size--;
                    q--;
                }
            }
        }
    }
    else if (chose[0] == 'r') {

        if (b_n[P_id].buff_check[6] > 0) {
            b_n[P_id].buff_check[6]--;
            if (b_n[P_id].hp + 3 > b_n[P_id].mhp) {
                b_n[P_id].hp = b_n[P_id].mhp;
            }
            else {
                b_n[P_id].hp += 3;
            }
        }


    }
}
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
        if (root.isMember("bgm")) {
            m[m_id].bgm = root["bgm"].asInt();
        }
        else {
            m[m_id].bgm = -1;
        }
    }
    u = root["layers"][0]["data"].size();
    in.close();
}
void m_map(player* p, Map* m, BOX* Box, int m_id, int b_id, npc* n, int z, string g);
void show(Map* m, player* p, enemy* e, e_npc* e_n, BOX* Box, int& m_id, int b_id, int z, npc* n, string& g);
void readeventjson(player *p,npc *n,flag *f,Map *m,BOX *Box,const char* filename,int &m_id,int b_id) {
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    IMAGE t_block, a1, tri, c1,mmp,p1,p2;
    loadimage(&t_block, L"./Game/picture/talkblock.png", 0, 0, false);
    loadimage(&tri, L"./Game/picture/talktri.png", 0, 0, false);
    loadimage(&mmp, L"./Game/picture/Mmap.png", 0, 0, false);
    loadimage(&p1, L"./Game/picture/p0.png", 0, 0, false);
    loadimage(&p2, L"./Game/picture/p1.png", 0, 0, false);
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
        if (root.isMember("cammer")) {
            int X, Y;
            X = root["cammer"]["x"].asInt();
            Y = root["cammer"]["y"].asInt();
            m[m_id].ox = (X > 13) * (X - 13) * 48 - (14 - (m[m_id].x - X)) * (X + 14 > m[m_id].x) * 48;
            m[m_id].oy = (Y > 10) * (Y - 10) * 48 - (10 - (m[m_id].y - Y)) * (Y+ 10 > m[m_id].y) * 48;
            m_map(p, m, Box, m_id, b_id, n, 0, "e");
        }
        for (int k = 0; k < root["talk"].size(); k++) {
            wstring s = UTF8ToUnicode(root["talk"][k]["sentence"].asString());
            int F = 0;
            int ubs = 0;
            int w = 0;
            int K = 0, W = 0;
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
                    mm = L"./Game/picture/t_npc" + mm + L"_" + std::to_wstring(root["talk"][k]["face"].asInt()) + L".png";
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
                mm = L"./Game/picture/player" + mm + L"_" + std::to_wstring(root["talk"][k]["face"].asInt()) + L".png";
                path = mm.c_str();
                loadimage(&p1, path, 0, 0, false);
                transparentimage(NULL, 0, 746, &p1, 0xFF55FF);
            }
          while (1) {
                int Bu = F / 10;
            BeginBatchDraw();
            if (root.isMember("mType")&&F%129==0) {
                transparentimage(NULL,1*(F%1295), 0, &mmp, 0xFF55FF,0,0,1295 - 1 * (F % 1295),696);
                transparentimage(NULL, 0, 0, &mmp, 0xFF55FF, 1295-1*(F%1295), 0, 1 * (F % 1295), 696);
                transparentimage(NULL, 14 * 48 , 10* 48-16, &p1, 0xFF55FF, 128*(ubs%2) + 16, 64, 48, 64);
                transparentimage(NULL, 14 * 48, 11 * 48 - 16, &p2, 0xFF55FF, 128 * (ubs % 2) + 16, 64, 48, 64);
                ubs++;
            }
            settextcolor(WHITE);
            settextstyle(30, 0, _T("Taipei Sans TC Beta"));
            for (w = w; (w <s.size()&&w<Bu)&&F%10; w++) {
                tt = L"";
                if (s[w] == '^') {
                    K++;
                    W = 0;
                }
                else if (s[w] == '#') {
                    break;
                }
                else if (s[w] == '~') {
                    break;
                }
                else {
                    tt += s[w];
                    LPCTSTR path = tt.c_str();
                    outtextxy(30 * W + 150, 696 + K * 50 + 5, path);
                    W++;
                }
            }              
            int ku = 0;
            if (s[w] == '#') {
                IMAGE q1;
                W = 0;
                loadimage(&q1, L"./Game/picture/c_blok.png", 0, 0, false);
                transparentimage(NULL, 250, 196 + K * 150, &q1, 0xFF55FF);
                for (int q = w+1; q < s.size(); q++) {
                    tt = L"";
                    if (s[q] == '^') {
                        K++;
                        transparentimage(NULL, 250, 196 + K * 150 , &q1, 0xFF55FF);
                        W = 0;
                    }
                    else {
                        tt += s[q];
                        LPCTSTR path = tt.c_str();
                        outtextxy(30 * W + 300, 196 + K * 150 + 5, path);
                        W++;
                    }
                }
                EndBatchDraw();
                ExMessage em;
 
                while (ku==0) {
                    if (peekmessage(&em,EM_MOUSE)) {
                        if (em.lbutton) {
                            for (int I = 0; I <= K; I++) {
                                if (em.x > 250 && em.x < 1050 && em.y>196 + I * 150 && em.y < 196 + I * 150 + 50) {
                                    if (root.isMember("chose_flag")) {
                                        f[root["chose_flag"][I].asInt()].check = 1;
                                        ku = 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (s[w] == '~') {
                string box = "";
                w++;
                while (1) {
                    if (s[w] >= '0' && s[w] <= '9') {
                        box += s[w];
                        w++;
                    }
                    else {
                        int eSe=stoi(box); 
                        wstring sPath = L"open ./Game/Sound/SE/eSe"+to_wstring(eSe) + L".mp3 alias se"+to_wstring(eSe);
                        mciSendString(sPath.c_str(), NULL, 0, NULL);
                        wstring vos = L"setaudio se"+to_wstring(eSe)+L" volume to " + to_wstring(soundSize);
                        mciSendString(vos.c_str(), NULL, 0, NULL);
                        mciSendString((L"play se"+to_wstring(eSe)+L" from 0").c_str(), NULL, 0, NULL);
                        break;
                    }
                }
            }
            if (ku == 1) {
                break;
            }
            if(w==s.size()){
                        transparentimage(NULL, 600, 910, &tri, 0xFF55FF);
            }
            flushmessage(EM_KEY);
            if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000)&&w==s.size()) {

                        break;
                    }
            if ((GetAsyncKeyState(VK_SPACE) & 0x8000 )&& w == s.size()) {

                        break;
                    }
            EndBatchDraw();
                    F++;
                    Sleep(1);
          }
        }
    }
    in.close();
}
void readmapeventjson(player* p, npc* n,m_flag *m_f,arms *ar,item *it,Map *m,t_equip *t_E,const char* filename,int &m_id,int s) {
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    IMAGE t_block, a1, tri, c1;
    loadimage(&t_block, L"./Game/picture/talkblock.png", 0, 0, false);
    loadimage(&tri, L"./Game/picture/talktri.png", 0, 0, false);
    wstring mm, tt, ss, MM;
    Json::Reader reader;
    Json::Value root;
    ifstream in(filename, ios::binary);
    if (!in.is_open()) {
        return;
    }
    if (reader.parse(in, root)) {
        if (root.isMember("talk")) {
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
                    mm = L"./Game/picture/t_npc" + mm + L"_" + std::to_wstring(root["talk"][k]["face"].asInt()) + L".png";
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
                mm = L"./Game/picture/player" + mm + L"_" + std::to_wstring(root["talk"][k]["face"].asInt()) + L".png";
                path = mm.c_str();
                loadimage(&p1, path, 0, 0, false);
                transparentimage(NULL, 0, 746, &p1, 0xFF55FF);
            }
            EndBatchDraw();
            wstring s = UTF8ToUnicode(root["talk"][k]["sentence"].asString());
            settextstyle(30, 0, _T("Taipei Sans TC Beta"));
            settextcolor(WHITE);
            int K = 0, W = 0;
            for (int w = 0; w < s.size(); w++) {
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
                if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
                    Sleep(1000);
                    break;
                }
                if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                    Sleep(1000);
                    break;
                }
            }
        }
      }
        else if (root.isMember("see")) {
            putimage(0, 696, &t_block);
            for (j = 0; j < root["see"].size();j++) {
                wstring s = UTF8ToUnicode(root["see"][j]["sentence"].asString());
                settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                settextcolor(WHITE);
                int K = 0, W = 0;
                for (int w = 0; w < s.size(); w++) {
                    tt = L"";
                    if (s[w] == '^') {
                        K++;
                        W = 0;
                    }
                    else {
                        tt += s[w];
                        LPCTSTR path = tt.c_str();
                        outtextxy(30 * W+10, 696 + K * 50 + 5, path);
                        W++;
                        Sleep(50);
                    }
                }
                transparentimage(NULL, 600, 910, &tri, 0xFF55FF);
            flushmessage(EM_KEY);
            while (1) {
                if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                    Sleep(1000);
                    break;
                }
            }            
            }

        }
        if (root.isMember("flag")) {
            m_f[s].check = root["flag"].asInt();
        }
        if (root.isMember("arms")) {
            for (int uc = 0; uc < root["arms"]["id"].size(); uc++) {
                ar[root["arms"]["id"][uc].asInt()].number += root["arms"]["number"][uc].asInt();
            }
        }
        if (root.isMember("item")) {
            for (int uc = 0; uc < root["item"]["id"].size(); uc++) {
                it[root["item"]["id"][uc].asInt()].number += root["item"]["number"][uc].asInt();
            }
        }
        if (root.isMember("team_state")) {
            p[root["team_state"].asInt()].state = 1;
            m[m_id].team_state[m[m_id].psize] = root["team_state"].asInt();
            m[m_id].psize++;
        }
        if (root.isMember("t_equip")) {
            for (int ut = 0; ut < root["t_equip"]["id"].size(); ut++) {
                t_E[root["t_equip"]["id"][ut].asInt()].number += root["t_equip"]["number"][ut].asInt();
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
void dcom() {

}
void b_camera(b_map *b_m,int x,int y,int b_mid) {
    if (x > 10 && x + 9 < b_m[b_mid].x) {

        b_m[b_mid].cx = x;
    }
    else if (x < 10) {
        b_m[b_mid].cx = 5;
    }
    else if (x+10 >b_m[b_mid].x) {
        b_m[b_mid].cx = x-5;
    }
    if (y > 7 && y + 7 < b_m[b_mid].y) {

        b_m[b_mid].cy = y;
    }
    else if (y < 10) {
        b_m[b_mid].cy = 0;
    }
    else if (y + 15 > b_m[b_mid].y) {
        b_m[b_mid].cy = b_m[b_mid].y - 8;
    }
}
void p_put(player *p,b_map *b_m,int psize,int b_mid) {
    IMAGE p1,c,hpB;
    wstring mm;
    LPCTSTR path;
        for (int I = 0; I < psize; I++) {
            mm = std::to_wstring(I);
            mm = L"./Game/picture/p" + mm + L".png";
            path = mm.c_str();
            loadimage(&p1, path, 0, 0, false);
            if (p[I].x * 48 - b_m[b_mid].ox < 960&& p[I].y * 48 - 16 - b_m[b_mid].oy<720) {
            loadimage(&hpB, L"./Game/picture/hp.png", 0, 0, false);
                if (p[I].pose == 1) {
                    transparentimage(NULL, p[I].x * 48 - b_m[b_mid].ox, p[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 8, 0, 48, 64);
                }
                else if (p[I].pose == 2) {
                    transparentimage(NULL, p[I].x * 48 - b_m[b_mid].ox, p[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 16, 64, 48, 64);
                }
                else if (p[I].pose == 3) {
                    transparentimage(NULL, p[I].x * 48 - b_m[b_mid].ox, p[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 0, 128, 48, 64);
                }
                else if (p[I].pose == 4) {
                    transparentimage(NULL, p[I].x * 48 - b_m[b_mid].ox, p[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 8, 192, 48, 64);
                }
                transparentimage(NULL, p[I].x * 48 - b_m[b_mid].ox, p[I].y * 48 - b_m[b_mid].oy+ (48 - ((p[I].hp * 48) / p[I].mhp)), &hpB, 0xFF55FF, 0,48-((p[I].hp * 48) / p[I].mhp), 48, ((p[I].hp * 48) / p[I].mhp));
            }
        }
    }
void p_bput(player* p, b_map* b_m, int psize, int b_mid,int P_id) {
    IMAGE p1, c;
    wstring mm;
    LPCTSTR path;
    for (int I = 0; I < psize; I++) {
        if (I != P_id) {
        mm = std::to_wstring(I);
        mm = L"./Game/picture/p" + mm + L".png";
        path = mm.c_str();
        loadimage(&p1, path, 0, 0, false);
        if (p[I].x * 48 - b_m[b_mid].ox < 960 && p[I].y * 48 - 16 - b_m[b_mid].oy < 720) {
            if (p[I].pose == 1) {
                transparentimage(NULL, p[I].x * 48 - b_m[b_mid].ox, p[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 8, 0, 48, 64);
            }
            else if (p[I].pose == 2) {
                transparentimage(NULL, p[I].x * 48 - b_m[b_mid].ox, p[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 16, 64, 48, 64);
            }
            else if (p[I].pose == 3) {
                transparentimage(NULL, p[I].x * 48 - b_m[b_mid].ox, p[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 0, 128, 48, 64);
            }
            else if (p[I].pose == 4) {
                transparentimage(NULL, p[I].x * 48 - b_m[b_mid].ox, p[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 8, 192, 48, 64);
            }
        }
        }
    }
}
void e_put(enemy *e,b_map *b_m,int esize,int b_mid) {
    IMAGE b1,b2,b3,b4,hpB;
    wstring mm;
    for (int I = 0; I < esize; I++) {
        if (e[I].x * 48 - b_m[b_mid].ox < 960 && e[I].y * 48 - b_m[b_mid].oy - 16 - 16 < 720) {
            mm = std::to_wstring(e[I].type);
            mm = L"./Game/picture/enemy" + mm + L".png";
            LPCTSTR path = mm.c_str();
            loadimage(&b1, path, 0, 0, false);
            loadimage(&hpB, L"./Game/picture/hp.png", 0, 0, false);
            if (e[I].pose == 1) {
                transparentimage(NULL, e[I].x * 48 - b_m[b_mid].ox, e[I].y * 48 - b_m[b_mid].oy - 16, &b1, 0xFF55FF, 64 + 8, 0, 48, 64);
            }
            else if (e[I].pose == 2) {
                transparentimage(NULL, e[I].x * 48 - b_m[b_mid].ox, e[I].y * 48 - b_m[b_mid].oy - 16, &b1, 0xFF55FF, 64 + 16, 64, 48, 64);

            }
            else if (e[I].pose == 3) {
                transparentimage(NULL, e[I].x * 48 - b_m[b_mid].ox, e[I].y * 48 - b_m[b_mid].oy - 16, &b1, 0xFF55FF, 64 + 0, 128, 48, 64);

            }
            else if (e[I].pose == 4) {
                transparentimage(NULL, e[I].x * 48 - b_m[b_mid].ox, e[I].y * 48 - b_m[b_mid].oy - 16, &b1, 0xFF55FF, 64 + 8, 192, 48, 64);

            }
            transparentimage(NULL, e[I].x * 48 - b_m[b_mid].ox, e[I].y * 48 - b_m[b_mid].oy + (48 - ((e[I].hp * 48) / e[I].mhp)), &hpB, 0xFF55FF, 0, 48 - ((e[I].hp * 48) / e[I].mhp), 48, ((e[I].hp * 48) / e[I].mhp));
        }
        }
    }
void e_bput(enemy *e,b_map *b_m, int id, int esize,int b_mid) {
    IMAGE b1, b2, b3, b4;
    wstring mm;
    for (int I = 0; I < esize; I++) {
        if (I != id) {
            if (e[I].x * 48 - b_m[b_mid].ox < 960 && e[I].y * 48 - b_m[b_mid].oy - 16 - 16 < 720) {
                mm = std::to_wstring(e[I].type);
                mm = L"./Game/picture/enemy" + mm + L".png";
                LPCTSTR path = mm.c_str();
                loadimage(&b1, path, 0, 0, false);
                if (e[I].pose == 1) {
                    transparentimage(NULL, e[I].x * 48 - b_m[b_mid].ox, e[I].y * 48 - b_m[b_mid].oy - 16, &b1, 0xFF55FF, 64 + 8, 0, 48, 64);
                }
                else if (e[I].pose == 2) {
                    transparentimage(NULL, e[I].x * 48 - b_m[b_mid].ox, e[I].y * 48 - b_m[b_mid].oy - 16, &b1, 0xFF55FF, 64 + 16, 64, 48, 64);

                }
                else if (e[I].pose == 3) {
                    transparentimage(NULL, e[I].x * 48 - b_m[b_mid].ox, e[I].y * 48 - b_m[b_mid].oy - 16, &b1, 0xFF55FF, 64 + 0, 128, 48, 64);

                }
                else if (e[I].pose == 4) {
                    transparentimage(NULL, e[I].x * 48 - b_m[b_mid].ox, e[I].y * 48 - b_m[b_mid].oy - 16, &b1, 0xFF55FF, 64 + 8, 192, 48, 64);

                }
            }
        }
    }
    }
void b_nput(b_npc* b_n, b_map *b_m,int b_nid,int nsize,int b_mid) {
    IMAGE p1, c,hpB;
    wstring mm;
    LPCTSTR path;
    for (int I = 0; I < nsize; I++) {
        mm = std::to_wstring(b_n[I].type);
        mm = L"./Game/picture/b_npc" + mm + L".png";
        path = mm.c_str();
        loadimage(&p1, path, 0, 0, false);
        if (b_n[I].x * 48 - b_m[b_mid].ox < 960 && b_n[I].y * 48 - 16 - b_m[b_mid].oy < 720) {
            loadimage(&hpB, L"./Game/picture/hp.png", 0, 0, false);
            if (b_n[I].pose == 1) {
                transparentimage(NULL, b_n[I].x * 48 - b_m[b_mid].ox, b_n[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 8, 0, 48, 64);
            }
            else if (b_n[I].pose == 2) {
                transparentimage(NULL, b_n[I].x * 48 - b_m[b_mid].ox, b_n[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 16, 64, 48, 64);
            }
            else if (b_n[I].pose == 3) {
                transparentimage(NULL, b_n[I].x * 48 - b_m[b_mid].ox, b_n[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 0, 128, 48, 64);
            }
            else if (b_n[I].pose == 4) {
                transparentimage(NULL, b_n[I].x * 48 - b_m[b_mid].ox, b_n[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 8, 192, 48, 64);
            }
            transparentimage(NULL, b_n[I].x * 48 - b_m[b_mid].ox, b_n[I].y * 48 - b_m[b_mid].oy + (48 - ((b_n[I].hp * 48) / b_n[I].mhp)), &hpB, 0xFF55FF, 0, 48 - ((b_n[I].hp * 48) / b_n[I].mhp), 48, ((b_n[I].hp * 48) / b_n[I].mhp));
        }
    }
}
void b_bnput(b_npc* b_n, b_map* b_m, int b_nid, int nsize, int b_mid) {
    IMAGE p1, c;
    wstring mm;
    LPCTSTR path;
    for (int I = 0; I < nsize; I++) {
        if (I != b_nid) {
        mm = std::to_wstring(b_n[I].type);
        mm = L"./Game/picture/b_npc" + mm + L".png";
        path = mm.c_str();
        loadimage(&p1, path, 0, 0, false);
        if (b_n[I].x * 48 - b_m[b_mid].ox < 960 && b_n[I].y * 48 - 16 - b_m[b_mid].oy < 720) {
            if (b_n[I].pose == 1) {
                transparentimage(NULL, b_n[I].x * 48 - b_m[b_mid].ox, b_n[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 8, 0, 48, 64);
            }
            else if (b_n[I].pose == 2) {
                transparentimage(NULL, b_n[I].x * 48 - b_m[b_mid].ox, b_n[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 16, 64, 48, 64);
            }
            else if (b_n[I].pose == 3) {
                transparentimage(NULL, b_n[I].x * 48 - b_m[b_mid].ox, b_n[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 0, 128, 48, 64);
            }
            else if (b_n[I].pose == 4) {
                transparentimage(NULL, b_n[I].x * 48 - b_m[b_mid].ox, b_n[I].y * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 8, 192, 48, 64);
            }
        }
        }
    }
}
void maps(player *p, int P_id,enemy *e,b_map *b_m,int b_mid) {
    b_m[b_mid].ox = (b_m[b_mid].cx-11)* (b_m[b_mid].cx > 11) * 48   - (9 - (b_m[b_mid].x - b_m[b_mid].cx)) * (b_m[b_mid].cx + 9 > b_m[b_mid].x) * 48;
    b_m[b_mid].oy = (b_m[b_mid].cy -7) * (b_m[b_mid].cy > 7) * 48 - (8 - (b_m[b_mid].y - b_m[b_mid].cy)) * (b_m[b_mid].cy + 8 > b_m[b_mid].y) * 48;
    putimage(0, 0, 960, 720, &mapP, b_m[b_mid].ox, b_m[b_mid].oy);
}
void ui(player *p,enemy *e,b_map *b_m,int P_id,int esize,int psize,int b_mid,int bu_id) {
    IMAGE back2;
    loadimage(&back2, L"./Game/picture/background.png", 0, 0, false);
    putimage(0, 720, &back2);
    putimage(960, 0, &text);
    putimage(960, 0, &back1);
    putimage(1025, 200, &aline);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    TCHAR t[5];
    wstring hp = L"HP[" + to_wstring(p[P_id].hp) + L"/" + to_wstring(p[P_id].mhp) + L"]";
    outtextxy(30, 900, hp.c_str());
    wstring ap = L"AP[" + to_wstring(p[P_id].act)+L"]";
    outtextxy(30, 917, ap.c_str());
    wstring mp = L"MP[" + to_wstring(p[P_id].move) + L"]";
    outtextxy(30, 934, mp.c_str());
    IMAGE atk;
    loadimage(&atk, L"./Game/picture/attack.png", 0, 0, false);
    putimage(200, 750, &atk);
    IMAGE movep;
    loadimage(&movep, L"./Game/picture/move.png", 0, 0, false);
    putimage(300, 750, &movep);
    IMAGE item;
    loadimage(&item, L"./Game/picture/item.png", 0, 0, false);
    putimage(400, 750, &item);
    IMAGE ski;
    loadimage(&ski, L"./Game/picture/skill.png", 0, 0, false);
    putimage(500, 750, &ski);    
    IMAGE save;
    loadimage(&save, L"./Game/picture/save.png", 0, 0, false);
    putimage(600, 750, &save);
    IMAGE load;
    loadimage(&load, L"./Game/picture/load.png", 0, 0, false);
    putimage(700, 750, &load);
    IMAGE end;
    loadimage(&end, L"./Game/picture/end.png", 0, 0, false);
    putimage(800, 750, &end);
    wstring mm;
    int I = P_id;
    LPCTSTR path;
    IMAGE c;
    mm = std::to_wstring(I);
    mm = L"./Game/picture/player" + mm + L"_1.png";
    path = mm.c_str();
    loadimage(&c, path, 0, 0, false);
    transparentimage(NULL, 0, 750, &c, 0xFF55FF);
    wchar_t* ptr = _wcsdup(p[I].name.c_str());
    outtextxy(60, 730, ptr);
    IMAGE enemy1,p1;
    for (int I = 0; I < psize; I++) {
        if (p[I].hp > 0) {
            mm = std::to_wstring(I);
            mm = L"./Game/picture/A_p" + mm + L".png";
            LPCTSTR path = mm.c_str();
            loadimage(&p1, path, 0, 0, false);
            transparentimage(NULL, 1040, 500 - (p[I].turn * 3), &p1, 0xFF55FF);
        }
    }
    for (int I = 0; I < esize; I++) {
        if (e[I].hp > 0) {
            mm = std::to_wstring(e[I].type);
            mm = L"./Game/picture/A_enemy" + mm + L".png";
            LPCTSTR path = mm.c_str();
            loadimage(&enemy1, path, 0, 0, false);
            transparentimage(NULL, 970, 500 - (e[I].turn * 3), &enemy1, 0xFF55FF);
        }
    }
    wstring s1= L":";
    outtextxy(1080 + 150, 0, s1.c_str());
    _stprintf(t, _T("%d"), b_m[b_mid].time % 60);
    outtextxy(1100 + 150, 0, t);
    _stprintf(t, _T("%d"), b_m[b_mid].time / 60);
    outtextxy(1060 + 150, 0, t);
    int u = 0;            
    IMAGE g;
    for (I = 0; I < bu_id; I++) {
        if (p[P_id].buff_check[I] >= 1) {

            mm = L"./Game/picture/buff" + to_wstring(I) + L".png";
            loadimage(&g, mm.c_str(), 0, 0, false);
            putimage( 200 + 30 * u, 900,&g);
            _stprintf(t, _T("%d"), p[P_id].buff_check[I]);
            outtextxy(200+30*u ,930, t);
            u++;
        }
    }
}
void clearQpair(std::queue<pair<int, int>>& q)
{
    queue<pair<int, int>> empty;
    std::swap(q, empty);
}
/*戰鬥近戰AI*/
void Bbfs(b_map *b_m, terrain(*te)[50], int sx, int sy, int x, int y, int* box,int b_mid,int MOVE) {
    int X=sx, Y=sy,road[200][150],move[200][150], k, w, min = 10000, mx=199, my=149,cost[200][150];
    for (k = 0; k < b_m[b_mid].x; k++) {
        for (w = 0; w < b_m[b_mid].y; w++) {
            road[k][w]=-1;
            cost[k][w] = 100000;
        }
    }
    cost[mx][my] = 1000000;
    road[X][Y]=0;
    queue<pair<int, int>> q;
    clearQpair(q);
    q.push({ X,Y });
    cost[X][Y] = 0;
    while (q.size()>0) {
        X = q.front().first;
        Y = q.front().second;
        if (cost[X+1][Y]>cost[X][Y]+te[X+1][Y].mB&& te[X + 1][Y].type != 1 &&te[X+1][Y].npc==0 && te[X + 1][Y].enemy == 0 && te[X + 1][Y].player == 0 && X + 1< b_m[b_mid].x) {
            q.push({X+1,Y});
            road[X + 1][Y] = 6;
            cost[X + 1][Y] = cost[X][Y] + te[X + 1][Y].mB;
        }
        if (cost[X -1][Y] > cost[X][Y] + te[X - 1][Y].mB && te[X - 1][Y].type != 1 && te[X - 1][Y].npc == 0 && te[X - 1][Y].enemy == 0 && te[X - 1][Y].player == 0 && X - 1 >= 0) {
            q.push({ X - 1,Y });
            road[X - 1][Y] = 4;
            cost[X - 1][Y] = cost[X][Y] + te[X - 1][Y].mB;
        }
        if (cost[X][Y+1] > cost[X][Y] + te[X][Y+1].mB && te[X ][Y+1].type != 1 && te[X ][Y+1].npc == 0 && te[X ][Y+1].enemy == 0 && te[X][Y+1].player == 0 && Y + 1 < b_m[b_mid].y) {
            q.push({ X ,Y+1 });
            road[X ][Y+1] = 2;
            cost[X][Y + 1] = cost[X][Y] + te[X][Y + 1].mB;
        }
        if (cost[X][Y-1] > cost[X][Y] + te[X][Y - 1].mB && te[X][Y-1].type != 1 && te[X][Y - 1].npc == 0 && te[X][Y - 1].enemy == 0 && te[X][Y - 1].player == 0 && Y - 1 >= 0) {
            q.push({ X ,Y-1 });
            road[X][Y -1] = 8;
            cost[X][Y - 1] = cost[X][Y] + te[X][Y - 1].mB;
        }            
        if (abs(X - x) + abs(Y - y) <= min&&cost[X][Y]!=0) {
            if (X==mx&&Y==my&&cost[X][Y] < cost[mx][my]) {
                min = abs(X - x) + abs(Y - y);
                mx = X;
                my = Y;
            }
            else if(cost[X][Y]<=MOVE) {
                min = abs(X - x) + abs(Y - y);
                mx = X;
                my = Y;
            }
        }
        q.pop();
    }
    int b[1000];
    for (w = 0; w < 1000; w++) { b[w] = -1; }
    w = 0;
    X = mx;
    Y = my;
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
void r_bfs(b_map *b_m, terrain(*te)[50],int sx,int sy,int x,int y,int *box,int Move,int b_mid,int range,int type) {
    int road[100][100], k, w, X = sx, Y = sy,mx,my,cost[100][100],min;
    for (k = 0; k < b_m[b_mid].x; k++) {
        for (w = 0; w < b_m[b_mid].y; w++) {
            road[k][w] = -1;
            cost[k][w] = 1000000;
        }
    }
    if (type == 0) {
    mx=99, my = 99;
    cost[mx][my] = 10000000;
    min = 100000;
    }
    else if (type == 1) {
     mx =99, my = 99;
     cost[mx][my] = 10000000;
    }
    road[X][Y] = 0;
    queue<pair<int, int>> q;
    clearQpair(q);
    q.push({ X,Y });
    cost[X][Y] = 0;
    if (type == 0) {
    while (q.size() > 0) {
        X = q.front().first;
        Y = q.front().second;
        if (cost[X+1][Y]>cost[X][Y]+te[X + 1][Y].mA && te[X + 1][Y].npc == 0 && te[X + 1][Y].enemy == 0 && te[X + 1][Y].player == 0 && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x) {
            q.push({ X + 1,Y });
            road[X + 1][Y] = 6;
            cost[X + 1][Y] = cost[X][Y] + te[X+1][Y].mA;

        }
        if (cost[X - 1][Y] > cost[X][Y] + te[X - 1][Y].mA && te[X - 1][Y].npc == 0 && te[X - 1][Y].enemy == 0 && te[X - 1][Y].player == 0 && te[X - 1][Y].type != 1&& X - 1 >= 0 ) {
            q.push({ X - 1,Y });
            road[X - 1][Y] = 4;
            cost[X - 1][Y] = cost[X][Y] + te[X-1][Y].mA;
        }
        if (cost[X][Y + 1] > cost[X][Y] + te[X][Y + 1].mA && te[X][Y + 1].npc == 0 && te[X][Y + 1].enemy == 0 && te[X][Y + 1].player == 0 && te[X - 1][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y ) {
            q.push({ X ,Y + 1 });
            road[X][Y + 1] = 2;
            cost[X][Y+1] = cost[X][Y] + te[X][Y+1].mA;
        }
        if (cost[X][Y - 1] > cost[X][Y] + te[X][Y - 1].mA && te[X][Y - 1].npc == 0 && te[X][Y - 1].enemy == 0 && te[X][Y - 1].player == 0 && te[X][Y - 1].type != 1  && Y - 1 >= 0 ) {
            q.push({ X ,Y - 1 });
            road[X][Y - 1] = 8;
            cost[X ][Y-1] = cost[X][Y] + te[X][Y-1].mA;
        }
        if (abs(X - x) + abs(Y - y) <= min&& abs(X - x) + abs(Y - y) >=range&& cost[X][Y] != 0) {
            if (X == mx && Y == my && cost[X][Y] < cost[mx][my]) {
                min = abs(X - x) + abs(Y - y);
                mx = X;
                my = Y;
            }
            else if (cost[X][Y] <= Move) {
                min = abs(X - x) + abs(Y - y);
                mx = X;
                my = Y;
            }
        }
        q.pop();
    }
    }
    else if (type == 1) {
        while (q.size() > 0) {
        X = q.front().first;
        Y = q.front().second;
        if (cost[X + 1][Y] > cost[X][Y] + te[X + 1][Y].mA && te[X + 1][Y].npc == 0 && te[X + 1][Y].enemy == 0 && te[X + 1][Y].player == 0 && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x) {
            q.push({ X + 1,Y });
            road[X + 1][Y] = 6;
            cost[X + 1][Y] = cost[X][Y] + te[X + 1][Y].mA;

        }
        if (cost[X - 1][Y] > cost[X][Y] + te[X - 1][Y].mA && te[X - 1][Y].npc == 0 && te[X - 1][Y].enemy == 0 && te[X - 1][Y].player == 0 && te[X - 1][Y].type != 1 && X - 1 >= 0) {
            q.push({ X - 1,Y });
            road[X - 1][Y] = 4;
            cost[X - 1][Y] = cost[X][Y] + te[X - 1][Y].mA;
        }
        if (cost[X][Y + 1] > cost[X][Y] + te[X][Y + 1].mA && te[X][Y + 1].npc == 0 && te[X][Y + 1].enemy == 0 && te[X][Y + 1].player == 0 && te[X - 1][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y) {
            q.push({ X ,Y + 1 });
            road[X][Y + 1] = 2;
            cost[X][Y + 1] = cost[X][Y] + te[X][Y + 1].mA;
        }
        if (cost[X][Y - 1] > cost[X][Y] + te[X][Y - 1].mA && te[X][Y - 1].npc == 0 && te[X][Y - 1].enemy == 0 && te[X][Y - 1].player == 0 && te[X][Y - 1].type != 1 && Y - 1 >= 0) {
            q.push({ X ,Y - 1 });
            road[X][Y - 1] = 8;
            cost[X][Y - 1] = cost[X][Y] + te[X][Y - 1].mA;
        }
        if (abs(X - x) + abs(Y - y) == range&& cost[X][Y] != 0&& cost[X][Y] < cost[mx][my]) {
             if (cost[X][Y] <= Move) {
                min = abs(X - x) + abs(Y - y);
                mx = X;
                my = Y;
            }
        }
        q.pop();
        }

    }
    X = mx;
    Y = my;
    if (X == 99 && Y == 99) {
        box[0] = 0;
        return;
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
void bfs(b_map *b_m, terrain(*te)[50],int sx, int sy, int x, int y, int* box, int &move, int P_id,int b_mid,int Move) {
    int X = sx, Y = sy, road[200][150], k, w,cost[200][150],min=1000000,mx=199,my=149;
    for (k = 0; k < b_m[b_mid].x; k++) {
        for (w = 0; w < b_m[b_mid].y; w++) {
            road[k][w] = -1;
            cost[k][w] = 100000;
        }
    }
    cost[mx][my] = 1000000;
    road[X][Y] = 0;
    cost[X][Y] = 0;
    queue<pair<int, int>> q;
    clearQpair(q);
    q.push({ X,Y });
    while (q.size() > 0) {
        X = q.front().first;
        Y = q.front().second;
        if (cost[X+1][Y]>cost[X][Y]+te[X + 1][Y].mA&& te[X + 1][Y].type != 1 && te[X + 1][Y].player==0 && te[X + 1][Y].npc==0&&te[X+1][Y].enemy==0 && X + 1 < b_m[b_mid].x) {
            q.push({ X + 1,Y });
            road[X + 1][Y] = 6;
            cost[X + 1][Y] = cost[X][Y] + te[X + 1][Y].mA;
        }
        if (cost[X - 1][Y] > cost[X][Y] + te[X - 1][Y].mA && te[X - 1][Y].npc == 0 && te[X - 1][Y].enemy == 0 && te[X - 1][Y].player == 0 && te[X - 1][Y].type != 1 && X - 1 >= 0) {
            q.push({ X - 1,Y });
            road[X - 1][Y] = 4;
            cost[X - 1][Y] = cost[X][Y] + te[X - 1][Y].mA;
        }
        if (cost[X][Y+1] > cost[X][Y] + te[X][Y+1].mA && te[X][Y + 1].npc == 0 && te[X][Y + 1].enemy == 0 && te[X][Y + 1].player == 0 && te[X][Y+1].type != 1 && Y + 1 < b_m[b_mid].y) {
            q.push({ X ,Y + 1 });
            road[X][Y + 1] = 2;
            cost[X][Y + 1] = cost[X][Y] + te[X][Y + 1].mA;
        }
        if (cost[X][Y - 1] > cost[X][Y] + te[X][Y - 1].mA && te[X][Y - 1].npc == 0 && te[X][Y - 1].enemy == 0 && te[X][Y - 1].player == 0 && te[X ][Y-1].type != 1  && Y - 1 >= 0) {
            q.push({ X ,Y - 1 });
            road[X][Y - 1] = 8;
            cost[X][Y - 1] = cost[X][Y] + te[X][Y - 1].mA;
        }
        if (abs(X - x) + abs(Y - y) <= min && cost[X][Y] != 0) {
            if (X == mx && Y == my && cost[X][Y] < cost[mx][my]) {
                min = abs(X - x) + abs(Y - y);
                mx = X;
                my = Y;
            }
            else if (cost[X][Y] <= Move) {
                min = abs(X - x) + abs(Y - y);
                mx = X;
                my = Y;
            }
        }
        q.pop();
    }
    int b[1000];
    for (w = 0; w < 1000; w++) { b[w] = -1; }
    w = 0;
    X = mx;
    Y = my;
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
void p_attack(wofstream* wofs, player *p,enemy *e,b_npc *b_n,arms *ar,b_map *b_m,buff *bu, terrain(*te)[50], int b_mid,int P_id,int id,string &chose,int esize,int psize,int bu_id,int b_nid) {
    int ax, ay,cost[50][50],X,Y,cho[50][50];
    int ar_id=-1;
    LPCTSTR path1;
    wstring mm,vos;
    TCHAR t[5];
    IMAGE ar1,hi;
        int sss = 1;
        ExMessage m, m1; string attack;
        if (chose == "a" && p[P_id].act > 0) {    
       setbkmode(TRANSPARENT);
        settextcolor(BLACK);
                IMAGE backa,armb;
                loadimage(&backa, L"./Game/picture/attackblock.png", 0, 0, false);
                loadimage(&armb, L"./Game/picture/armsblock.png", 0, 0, false);
                i = 0;
                if (p[P_id].arms_id_1 != -1) {
                putimage(175, 799 + 48 * i, &armb);
                mm = ar[p[P_id].arms_id_1].name;
                if (ar[p[P_id].arms_id_1].type[0] == 'r') {
                    mm+=  L"(" + std::to_wstring(ar[p[P_id].arms_id_1].bullet) + L"/" + to_wstring(ar[p[P_id].arms_id_1].mbullet) + L")";
                }              
                outtextxy(210, 815+48*i, mm.c_str());
                mm = L"./Game/picture/arms" + to_wstring(p[P_id].arms_id_1)+L".png";
                mm = L"./Game/picture/arms" + to_wstring(p[P_id].arms_id_1) + L".png";
                loadimage(&ar1, mm.c_str(), 0, 0, false);
                transparentimage(NULL, 180, 815 + 48 * i, &ar1, 0xFF55FF);
                i++;
                }
                if (p[P_id].arms_id_2 != -1) {
                    putimage(175, 799 + 48 * i, &armb);
                    mm = ar[p[P_id].arms_id_2].name;
                   
                    if (ar[p[P_id].arms_id_2].type[0] == 'r') {
                      mm += L"(" + std::to_wstring(ar[p[P_id].arms_id_2].bullet) + L"/" + to_wstring(ar[p[P_id].arms_id_2].mbullet) + L")";
                    }
                    outtextxy(210, 815 + 48 * i, mm.c_str());
                    mm = L"./Game/picture/arms" + to_wstring(p[P_id].arms_id_2) + L".png";
                    loadimage(&ar1, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 180, 815+48*i, &ar1, 0xFF55FF);
                }
            while (sss != 0)//選武器
            {
                // 获取一条鼠标消息
                m = getmessage(EM_MOUSE);
                switch (m.message) {

                case WM_LBUTTONDOWN://按鼠标左键分发事件
                    if (m.x >= 210 && m.x <= 296 && m.y >= 799 && m.y <= 895) {
                        attack = "a"; ar_id = p[P_id].arms_id_1;
                    }
                    if (m.x >= 200 && m.x <= 296 && m.y >= 799+48 && m.y <= 895+48&&i==1) {
                        attack = "a"; ar_id = p[P_id].arms_id_2;
                    }
                case WM_RBUTTONUP:
                    sss = 0;
                }
            }
            int rx, ry, AC, ATK, dmg;

            if (attack == "a" && ar[ar_id].bullet > 0) {
                IMAGE mb;
                loadimage(&mb, L"./Game/picture/ab.png", 0, 0, false);
            queue<pair<int, int>> q;
            clearQpair(q);
            q.push({ p[P_id].x,p[P_id].y});
            for (i = 0; i < b_m[b_mid].x; i++) {
                for (j = 0; j< b_m[b_mid].y; j++) {
                    cost[i][j] = 0;
                    cho[i][j] = 0;
                }
            }
                BeginBatchDraw();
                b_camera(b_m, p[P_id].x, p[P_id].y, b_mid);
                maps(p, P_id, e, b_m, b_mid);
                while (q.size() > 0) {
                    X = q.front().first;
                    Y = q.front().second;
                    if (te[X+1][Y].type!=1&&cost[X][Y]<ar[ar_id].range&&X+1<b_m[b_mid].x) {
                        cost[X+1][Y]=cost[X][Y]+1;
                        q.push({ X +1,Y  });
                    }
                    if (te[X-1][Y].type != 1 && cost[X][Y] < ar[ar_id].range&&X-1>=0) {
                        cost[X - 1][Y] = cost[X][Y] + 1;
                        q.push({ X - 1,Y });
                    }
                    if (te[X][Y+1].type != 1 && cost[X][Y] < ar[ar_id].range&&Y+1<b_m[b_mid].y) {
                        cost[X][Y+1] = cost[X][Y] + 1;
                        q.push({ X ,Y + 1 });
                    }
                    if (te[X][Y-1].type != 1 && cost[X][Y] < ar[ar_id].range&&Y-1>=0) {
                        cost[X][Y-1] = cost[X][Y] + 1;
                        q.push({ X ,Y - 1 });
                    }
                    if (te[X][Y].type != 1 && te[X][Y].npc != 1 && te[X][Y].player != 1&&cho[X][Y]!=1) {
                         transparentimage(NULL, 48 * X-b_m[b_mid].ox, 48 * Y-b_m[b_mid].oy, &mb);
                         cho[X][Y] = 1;
                    }
                    q.pop();
                }               
                e_put(e, b_m, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p,b_m, psize,b_mid);
                ui(p, e, b_m,P_id, esize, psize,b_mid,bu_id);
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
                                if (m1.x >= 48 * i && m1.x <= 48 * i + 48 && m1.y >= 48 * j && m1.y <= 48 * j + 48&&cho[i][j]==1) {
                                    for (int k = 0; k < esize; k++) {
                                        if (e[k].x == i+b_m[b_mid].ox/48 && e[k].y == j + b_m[b_mid].oy / 48) {
                                            id = k; sss = 0;
                                        }
                                    }
                                }
                            }
                        }
                    case WM_RBUTTONDOWN:
                        if (m1.rbutton) {
                            return;
                        }
                    }
                }
                *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"將" << e[id].name << L"選為目標" << endl;

                ry = (p[P_id].y - e[id].y) * (p[P_id].y > e[id].y) + (p[P_id].y - e[id].y) * (p[P_id].y < e[id].y) * -1; rx = (p[P_id].x - e[id].x) * (p[P_id].x > e[id].x) + (p[P_id].x - e[id].x) * (p[P_id].x < e[id].x) * -1;
                if (rx + ry <= ar[ar_id].range) {
                int r1 = p_buff_check("a1", p,e,b_n, bu, b_m, P_id, b_mid,bu_id),r2 = p_buff_check("a2", p, e, b_n, bu, b_m, P_id, b_mid, bu_id);
                    AC = rand() % 20 + 1 + (e[id].dex - 10) / 2;
                    if (ar[ar_id].type[0] == 'r') {
                     ATK = rand() % 20 + 1 + (p[P_id].dex - 10) / 2;
                    }
                    else if (ar[ar_id].type[0] == 'm') {
                        ATK = rand() % 20 + 1 + (p[P_id].str - 10) / 2;
                    }
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
                    if (r1 >= 1&&ATK<=AC) {
                        while (r1--) {
                            AC = rand() % 20 + 1 + (e[id].dex - 10) / 2;
                            if (ar[ar_id].type[0] == 'r') {
                                ATK = rand() % 20 + 1 + (p[P_id].dex - 10) / 2;
                            }
                            else if (ar[ar_id].type[0] == 'm') {
                                ATK = rand() % 20 + 1 + (p[P_id].str - 10) / 2;
                            }
                            if (ATK > AC) {
                                break;
                            }
                        }
                    }
                    if (r2 >= 1 && ATK <= AC) {
                        while (r2--) {
                            AC = rand() % 20 + 1 + (e[id].dex - 10) / 2;
                            if (ar[ar_id].type[0] == 'r') {
                                ATK = rand() % 20 + 1 + (p[P_id].dex - 10) / 2;
                            }
                            else if (ar[ar_id].type[0] == 'm') {
                                ATK = rand() % 20 + 1 + (p[P_id].str - 10) / 2;
                            }
                            if (ATK > AC) {
                                break;
                            }
                        }
                    }
                    /*SE*/
                    IMAGE ae;

                    if (ar_id == 0||ar_id==5) {
                        mciSendString(L"close se", NULL, 0, NULL);
                        mciSendString(L"open ./Game/Sound/SE/拳銃2.mp3 alias se", NULL, 0, NULL);
                        vos = L"setaudio se volume to " + to_wstring(soundSize);
                        mciSendString(vos.c_str(), NULL, 0, NULL);
                        mciSendString(L"play se from 0", NULL, 0, NULL);
                        loadimage(&ae, L"./Game/picture/武器・弓1.png", 0, 0, false);
                        for (j = 0; j < 5; j++) {
                            for (i = 0; i < 3; i++) {
                                BeginBatchDraw();
                                maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                                e_put(e, b_m, esize, b_mid);
                                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                p_put(p, b_m, psize, b_mid); 
                                transparentimageA(NULL, e[id].x * 48 - b_m[b_mid].ox-30, e[id].y * 48 - b_m[b_mid].oy-160, &ae, 0x000000, i * 120, j * 300, 120, 300);
                                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);                               
                                EndBatchDraw();
                                Sleep(5);
                            }
                        }
                    }
                    else if (ar_id == 4||ar_id==6) {
                        mciSendString(L"close se", NULL, 0, NULL);
                        mciSendString(L"open ./Game/Sound/SE/剣の素振り3.mp3 alias se", NULL, 0, NULL);
                        vos = L"setaudio se volume to " + to_wstring(soundSize);
                        mciSendString(vos.c_str(), NULL, 0, NULL);
                        mciSendString(L"play se from 0", NULL, 0, NULL);
                        loadimage(&ae, L"./Game/picture/技・一閃.png", 0, 0, false);
                        for (j = 0; j < 5; j++) {
                            for (i = 0; i < 3; i++) {
                                BeginBatchDraw();
                                maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                                e_put(e, b_m, esize, b_mid);
                                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                p_put(p, b_m, psize, b_mid);
                                transparentimageA(NULL, e[id].x * 48 - b_m[b_mid].ox-110, e[id].y * 48 - b_m[b_mid].oy-47, &ae, 0x000000, i * 320, j * 120, 320, 120);
                                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);                                
                                EndBatchDraw();
                                Sleep(5);
                            }
                        }
                    }
                    if (ATK > AC) {
                         BeginBatchDraw();
                        maps(p, P_id, e,b_m,b_mid);/*地圖繪製*/
                        e_put(e,b_m, esize,b_mid);
                        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, psize, b_mid);
                        dmg = roll(ar[ar_id].dmg, 1);
                        *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"用" << ar[ar_id].name << L"命中" <<  e[id].name<< L"造成" << dmg << L"點傷害(" << ATK << ">" << AC << ")" << endl;
                        _stprintf(t, _T("%d"), -dmg);		
                        settextcolor(RGB(255, 0, 0));
                        settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                        outtextxy(e[id].x*48-b_m[b_mid].ox, e[id].y * 48-b_m[b_mid].oy, t);
                        settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                        settextcolor(RGB(0, 0, 0));
                        e[id].hp -= dmg;                      
                        EndBatchDraw();
                        if (ar[ar_id].type[0] == 'r') {
                            ar[ar_id].bullet--;
                        }
                        p[P_id].act--;
                        Sleep(1000);
                        if (e[id].hp <= 0) {
                            te[e[id].x][e[id].y].mA = 1;
                            te[e[id].x][e[id].y].mB = 1;
                            te[e[id].x][e[id].y].enemy = 0;
                            e[id].x = -1; e[id].y = -1;
                            e[id].speed = -1;
                        }
                        if (r1 == 0) {
                            p[P_id].buff_check[1] = 0;
                        }
                        else if (r1 >= 1) {
                            p[P_id].buff_check[1] = r1;
                        }
                        if (r2 == 0) {
                            p[P_id].buff_check[2] = 0;
                        }
                        else if (r2 >= 1) {
                            p[P_id].buff_check[2] = r2;
                        }
                    }
                    else {
                        BeginBatchDraw();
                        maps(p, P_id, e,b_m,b_mid);/*地圖繪製*/
                        e_put(e, b_m, esize, b_mid);
                        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, psize, b_mid);
                        *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"用" << ar[ar_id].name << L"攻擊" << e[id].name << L"未能命中(" << ATK << "<=" << AC << ")" << endl;
                        settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                        settextcolor(RGB(255, 0, 0));
                        mm = L"miss";
                        outtextxy(e[id].x * 48 - b_m[b_mid].ox, e[id].y * 48 - b_m[b_mid].oy, mm.c_str());
                        settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                        settextcolor(RGB(0, 0, 0));
                        EndBatchDraw();
                        if (ar[ar_id].type[0] == 'r') {
                            ar[ar_id].bullet--;
                        }
                        p[P_id].act--;
                        Sleep(1000);
                        if (r1 == 0) {
                            p[P_id].buff_check[1] = 0;
                        }
                        else if (r1 >= 1) {
                            p[P_id].buff_check[1] = r1;
                        }
                        if (r2 == 0) {
                            p[P_id].buff_check[2] = 0;
                        }
                        else if (r2 >= 1) {
                            p[P_id].buff_check[2] = r2;
                        }
                    }
                }
                else {
                }
            }
            else if (chose == "a" && ar[ar_id].bullet < 1) {
            mciSendString(L"close se", NULL, 0, NULL);
            mciSendString(L"open ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 alias se", NULL, 0, NULL);
            vos = L"setaudio se volume to " + to_wstring(soundSize);
            mciSendString(vos.c_str(), NULL, 0, NULL);
            mciSendString(L"play se from 0", NULL, 0, NULL);
 }
        }
        else if (chose == "a" && p[P_id].act < 1) {
        mciSendString(L"close se", NULL, 0, NULL);
        mciSendString(L"open ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 alias se", NULL, 0, NULL);
        vos = L"setaudio se volume to " + to_wstring(soundSize);
        mciSendString(vos.c_str(), NULL, 0, NULL);
        mciSendString(L"play se from 0", NULL, 0, NULL);
        }
    }
void p_item(player *p,enemy *e,b_npc *b_n,arms *ar, item *it, t_equip *t_E,b_map *b_m, terrain(*te)[50],string &chose, int P_id, int i_id, int t_Eid,int b_mid,int bu_id) {
    if (chose == "i") {
    IMAGE p1,ar1,hi;
    wstring mm;        
    ExMessage m;
    RECT t;
    int cost[50][50], cho[50][50], X, Y;
    loadimage(&p1, L"./Game/picture/tbox.png", 0, 0, false);
    loadimage(&hi, L"./Game/picture/b_hint.png", 0, 0, false);
    vector<IMAGE> itemImages;
    int I, J,ry,rx,n,k;
    for (int i = 0; i < t_E[P_id].size; i++) {
        if (p[P_id].box_id[i] != -1 && p[P_id].box_size != 0) {
            mm = L"./Game/picture/item" + to_wstring(p[P_id].box_id[i]) + L".png";
            IMAGE p2;
            loadimage(&p2, mm.c_str(), 0, 0, false);
            itemImages.push_back(p2);
        }
    }
   IMAGE ib;
   loadimage(&ib, L"./Game/picture/ib.png", 0, 0, false);
    while (1) {
        BeginBatchDraw();
        for (i = 0; i < t_E[p[P_id].t_id].size; i++) {
            putimage(400 + i * 30, 720, &p1);
            if (p[P_id].box_id[i] != -1 && p[P_id].box_size != 0) {
                transparentimage(NULL, 405 + i * 30, 725, &itemImages[i], 0xFF55FF);
                outtextxy(405 + i * 30, 700, to_wstring(p[P_id].box_size[i]).c_str());
            }
        }
        m = getmessage(EM_MOUSE);
        for (i = 0; i < t_E[p[P_id].t_id].size; i++) {
           if (m.x >= 400 + i * 30 && m.x <= 430 + i * 30 && m.y >= 720 && m.y <= 750) {
            if (m.lbutton) {
                maps(p, P_id, e, b_m, b_mid);             
                if (p[P_id].box_id[i] != -1 && p[P_id].box_size[i] != 0) {
                    if (it[p[P_id].box_id[i]].type == 'b' || it[p[P_id].box_id[i]].type=='h') {
                        queue<pair<int, int>> q;
                        clearQpair(q);
                        q.push({ p[P_id].x,p[P_id].y });
                        for (int I = 0; I < b_m[b_mid].x; I++) {
                            for (J = 0; J < b_m[b_mid].y; J++) {
                                cost[I][J] = 0;
                                cho[I][J] = 0;
                            }
                        }
                        while (q.size() > 0) {
                            X = q.front().first;
                            Y = q.front().second;
                            if (te[X + 1][Y].type != 1 && cost[X][Y] < it[p[P_id].box_id[i]].range && X + 1 < b_m[b_mid].x) {
                                cost[X + 1][Y] = cost[X][Y] + 1;
                                q.push({ X + 1,Y });
                            }
                            if (te[X - 1][Y].type != 1 && cost[X][Y] < it[p[P_id].box_id[i]].range && X - 1 >= 0) {
                                cost[X - 1][Y] = cost[X][Y] + 1;
                                q.push({ X - 1,Y });
                            }
                            if (te[X][Y + 1].type != 1 && cost[X][Y] < it[p[P_id].box_id[i]].range && Y + 1 < b_m[b_mid].y) {
                                cost[X][Y + 1] = cost[X][Y] + 1;
                                q.push({ X ,Y + 1 });
                            }
                            if (te[X][Y - 1].type != 1 && cost[X][Y] < it[p[P_id].box_id[i]].range && Y - 1 >= 0) {
                                cost[X][Y - 1] = cost[X][Y] + 1;
                                q.push({ X ,Y - 1 });
                            }
                            if (te[X][Y].type != 1 && te[X][Y].enemy!=1 && cho[X][Y] != 1) {
                                transparentimage(NULL, 48 * X - b_m[b_mid].ox, 48 * Y - b_m[b_mid].oy, &ib);
                                cho[X][Y] = 1;
                            }
                            q.pop();
                        }
                        e_put(e, b_m, b_m[b_mid].esize, b_mid);
                        b_nput(b_n, b_m, b_m[b_mid].nsize, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, b_m[b_mid].psize, b_mid);
                        ui(p, e, b_m, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, bu_id);
                        EndBatchDraw();
                        while (1) {
                            m = getmessage(EM_MOUSE);
                            if (m.lbutton) {
                                for (int I = 0; I < b_m[b_mid].psize; I++) {
                                    if (m.x >= 48 * p[I].x -b_m[b_mid].ox && m.x <= 48 * p[I].x + 48 - b_m[b_mid].ox && m.y >= 48 * p[I].y - b_m[b_mid].oy&& m.y <= 48 * p[I].y + 48 - b_m[b_mid].oy) {
                                        if (cho[p[I].x][p[I].y] == 1) {
                                        if (p[P_id].box_id[i] == 1) {
                                            if (I == P_id) {
                                       
                                                p[P_id].buff_time[p[P_id].buff_Size]=10;
                                                p[P_id].buff_id[p[P_id].buff_Size] = 4;
                                                p[P_id].buff_check[4] += 1;
                                                if (p[P_id].t_id == 0) {
                                                    p[P_id].buff_time[p[P_id].buff_Size] = p[P_id].buff_time[p[P_id].buff_Size]  *80/100;
                                                }         
                                                p[P_id].buff_give[p[P_id].buff_Size] = "iw1";
                                                p[P_id].buff_Size++;
                                                p[P_id].box_size[i]--;
                                            }
                                            else {
                                                p[P_id].buff_time[p[P_id].buff_Size] = 10;
                                                p[I].buff_time[p[I].buff_Size] = 10;
                                                p[P_id].buff_id[p[P_id].buff_Size] = 4;
                                                p[I].buff_id[p[I].buff_Size] = 4;
                                                p[P_id].buff_check[4] += 1;
                                                p[I].buff_check[4] += 1;
                                                if (p[P_id].t_id == 0) {
                                                    p[P_id].buff_time[p[P_id].buff_Size] = p[P_id].buff_time[p[P_id].buff_Size] * 80 / 100;
                                                    p[I].buff_time[p[I].buff_Size] = p[I].buff_time[p[I].buff_Size] * 80 / 100;
                                                }
                                                p[P_id].buff_give[p[P_id].buff_Size] = "w";
                                                p[I].buff_give[p[I].buff_Size] = "i1";
                                                p[P_id].buff_Size++;
                                                p[I].buff_Size++;
                                                p[P_id].box_size[i]--;
                                            }
                                        }
                                        else if (p[P_id].box_id[i] == 0) {
                                            if (I == P_id) {
                                                IMAGE backa, armb;
                                                loadimage(&backa, L"./Game/picture/attackblock.png", 0, 0, false);
                                                loadimage(&armb, L"./Game/picture/armsblock.png", 0, 0, false);
                                                n = 0;
                                                if (p[P_id].arms_id_1 != -1) {
                                                    putimage(175, 799 + 48 * n, &armb);
                                                    mm = ar[p[P_id].arms_id_1].name;
                                                    if (ar[p[P_id].arms_id_1].type[0] == 'r') {
                                                        mm += L"(" + std::to_wstring(ar[p[P_id].arms_id_1].bullet) + L"/" + to_wstring(ar[p[P_id].arms_id_1].mbullet) + L")";
                                                    }
                                                    outtextxy(210, 815 + 48 * n, mm.c_str());
                                                    mm = L"./Game/picture/arms" + to_wstring(p[P_id].arms_id_1) + L".png";
                                                    mm = L"./Game/picture/arms" + to_wstring(p[P_id].arms_id_1) + L".png";
                                                    loadimage(&ar1, mm.c_str(), 0, 0, false);
                                                    transparentimage(NULL, 180, 815 + 48 * n, &ar1, 0xFF55FF);
                                                    n++;
                                                }
                                                if (p[P_id].arms_id_2 != -1) {
                                                    putimage(175, 799 + 48 * n, &armb);
                                                    mm = ar[p[P_id].arms_id_2].name;

                                                    if (ar[p[P_id].arms_id_2].type[0] == 'r') {
                                                        mm += L"(" + std::to_wstring(ar[p[P_id].arms_id_2].bullet) + L"/" + to_wstring(ar[p[P_id].arms_id_2].mbullet) + L")";
                                                    }
                                                    outtextxy(210, 815 + 48 * n, mm.c_str());
                                                    mm = L"./Game/picture/arms" + to_wstring(p[P_id].arms_id_2) + L".png";
                                                    loadimage(&ar1, mm.c_str(), 0, 0, false);
                                                    transparentimage(NULL, 180, 815 + 48 * n, &ar1, 0xFF55FF);
                                                }
                                                ExMessage im;
                                                while (1) {
                                                    im = getmessage(EM_MOUSE);
                                                    for (k = 0; k <= n; k++) {
                                                        if (im.x >= 180 && im.x <= im.x <= 330 && im.y >= 815 + 48 * k && im.y <= 863 + 48 * k&&im.lbutton) {
                                                            if (k == 0) {
                                                                if (ar[p[P_id].arms_id_1].type[0] == 'r') {
                                                                    if (ar[p[P_id].arms_id_1].type[1] == 'r') {
                                                                        p[P_id].buff_time[p[P_id].buff_Size] = 5;
                                                                        p[P_id].buff_id[p[P_id].buff_Size] = 5;
                                                                        p[P_id].buff_check[5] += 1;                                                                        
                                                                        p[P_id].buff_give[p[P_id].buff_Size] = "iw0a";
                                                                        p[P_id].buff_Size++;
                                                                        p[P_id].box_size[i]--;
                                                                    }
                                                                    else {
                                                                        putimage(400, 300, &hi);
                                                                        t = { 400,300,650,400 };
                                                                        mm = L"無法將" + it[p[P_id].box_id[i]].Name + L"裝填於" + ar[p[P_id].arms_id_1].name;
                                                                        drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                                                                        Sleep(2000);
                                                                    }
                                                                }
                                                                else {
                                                                    putimage(400, 300, &hi);
                                                                    t = { 400,300,650,400 };
                                                                    mm = ar[p[P_id].arms_id_2].name + L"並非槍械";
                                                                    drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                                                                    Sleep(2000);
                                                                }
                                                            }
                                                            else if (k == 1) {
                                                                if (ar[p[P_id].arms_id_2].type[0] == 'r') {
                                                                    if (ar[p[P_id].arms_id_2].type[1] == 'r') {

                                                                    }
                                                                    else {
                                                                        putimage(400, 300, &hi);
                                                                        t = { 400,300,650,400 };
                                                                        mm = L"無法將" + it[p[P_id].box_id[i]].Name + L"裝填於" + ar[p[P_id].arms_id_2].name;
                                                                        drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                                                                        Sleep(2000);
                                                                    }
                                                                }
                                                                else {
                                                                    putimage(400, 300, &hi);
                                                                    t = { 400,300,650,400 };
                                                                    mm = ar[p[P_id].arms_id_2].name+L"並非槍械";
                                                                    drawtext(mm.c_str(), &t, DT_CENTER| DT_VCENTER| DT_SINGLELINE);
                                                                    Sleep(2000);
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                              chose = "e";
                                              return;
                                        }

                                    }
                                }
                                for (I = 0; I < b_m[b_mid].nsize; I++) {
                                    if (m.x >= 48 * b_n[I].x - b_m[b_mid].ox && m.x <= 48 * b_n[I].x + 48 - b_m[b_mid].ox && m.y >= 48 * b_n[I].y - b_m[b_mid].oy && m.y <= 48 * b_n[I].y + 48 - b_m[b_mid].oy) {
                                        if (cho[b_n[I].x][b_n[I].y] == 1) {
                                            if (p[P_id].box_id[i] == 1) {
                                                p[P_id].buff_time[p[P_id].buff_Size] = 10;
                                                b_n[I].buff_time[b_n[I].buff_Size] = 10;
                                                p[P_id].buff_id[p[P_id].buff_Size] = 4;
                                                b_n[I].buff_id[b_n[I].buff_Size] = 4;
                                                p[P_id].buff_check[4] += 1;
                                                b_n[I].buff_check[4] += 1;
                                                if (p[P_id].t_id == 0) {
                                                    p[P_id].buff_time[p[P_id].buff_Size] = p[P_id].buff_time[p[P_id].buff_Size] * 80 / 100;
                                                    b_n[I].buff_time[b_n[I].buff_Size] = b_n[I].buff_time[b_n[I].buff_Size] * 80 / 100;
                                                }
                                                p[P_id].buff_give[p[P_id].buff_Size] = "w";
                                                b_n[I].buff_give[b_n[I].buff_Size] = "i1";
                                                p[P_id].buff_Size++;
                                                b_n[I].buff_Size++;
                                                p[P_id].box_size[i]--;
                                            }
                                            else if (p[P_id].box_id[i]==0) {

                                            }
                                            chose = "e";
                                            return;
                                        }
                                    }
                              }
                            }
                            if (m.rbutton) {
                                break;
                            }
                        }
                    }
                }
                else {
                    break;
                }
           }
        }
        }

        if (m.rbutton) {
            return;
        }
        EndBatchDraw();
        Sleep(10);
    }
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
void p_walk(wofstream *wofs,player *p,enemy *e,b_npc *b_n,b_map *b_m, terrain(*te)[50], string chose, int P_id, int id, int psize, int esize,int b_mid,int bu_id,int b_nid) {
    wstring vos;
    if (chose == "w" && p[P_id].move > 0) {
            b_camera(b_m, p[P_id].x,p[P_id].y, b_mid);
            int rx, ry, m, X, Y,dr[100][100],cost[100][100];
            BeginBatchDraw();
            maps(p, P_id, e, b_m, b_mid);
            IMAGE mb;
            loadimage(&mb, L"./Game/picture/mb.png", 0, 0, false);
            string walk;
            queue<pair<int, int>> q;
            clearQpair(q);
            q.push({ p[P_id].x,p[P_id].y });
            for (i = 0; i < b_m[b_mid].x; i++) {
                for (j = 0; j < b_m[b_mid].y; j++) {
                    dr[i][j] = 0;
                    cost[i][j] = 10000;
                }
            }
            X = q.front().first;
            Y = q.front().second;
            cost[X][Y] = 0;
            while (1) {
                if (q.empty()) {
                    break;
                }
                X = q.front().first;
                Y = q.front().second;
                if (cost[X+1][Y]>cost[X][Y]+te[X + 1][Y].mA&& te[X + 1][Y].type!=1 && X + 1 <b_m[b_mid].x) {
                    cost[X + 1][Y] = cost[X][Y] + te[X + 1][Y].mA;
                    q.push({ X + 1,Y });
                }
                if (cost[X - 1][Y] > cost[X][Y] + te[X - 1][Y].mA && te[X -1][Y].type != 1 && X - 1 >= 0) {
                    cost[X - 1][Y] = cost[X][Y] + te[X - 1][Y].mA;
                    q.push({ X - 1,Y });
                }
                if (cost[X][Y+1] > cost[X][Y] + te[X ][Y+1].mA && te[X][Y+1].type != 1 && Y + 1 < b_m[b_mid].y) {
                    cost[X][Y + 1] = cost[X][Y] + te[X][Y + 1].mA;
                    q.push({ X ,Y + 1 });
                }
                if (cost[X][Y - 1] > cost[X][Y] + te[X][Y - 1].mA && te[X][Y-1].type != 1  && Y - 1 >= 0) {
                    cost[X][Y - 1] = cost[X][Y] + te[X][Y - 1].mA;
                    q.push({ X ,Y - 1 });
                }
                if (dr[X][Y] == 0 &&te[X][Y].enemy==0&&te[X][Y].npc==0&&te[X][Y].player==0&&cost[X][Y]<=p[P_id].move) {
                    dr[X][Y] = 1;
                }
                q.pop();
            }
            for (i = 0; i < b_m[b_mid].x; i++) {
                for (j = 0; j < b_m[b_mid].y; j++) {
                    if (dr[i][j] == 1) {
                             transparentimage(NULL, 48 * i - b_m[b_mid].ox, 48 * j - b_m[b_mid].oy, &mb);
                    }
                }
            }
       
            IMAGE p1;
            wstring mm;
            LPCTSTR path;
            mm = std::to_wstring(P_id);
            mm = L"./Game/picture/p" + mm + L".png";
            path = mm.c_str();
            loadimage(&p1, path, 0, 0, false);
            e_put(e, b_m, esize, b_mid);
            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
            p_put(p, b_m, psize, b_mid);
            ui(p, e, b_m, P_id, esize, psize, b_mid,bu_id);
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
                            if (m.x <= j * 48 + 48 && m.x >= j * 48 && m.y <= i * 48 + 48 && m.y >= i * 48 && dr[j + b_m[b_mid].ox / 48][i + b_m[b_mid].oy / 48] == 1) {
                                    int box[3000];
                                    bfs(b_m,te,p[P_id].x, p[P_id].y, j + b_m[b_mid].ox / 48, i + b_m[b_mid].oy / 48, box, p[P_id].move, P_id,b_mid,p[P_id].move);
                                    int k, w = 0;
                                    int xbox = p[P_id].x, ybox = p[P_id].y;
                                    
                                    while (1) {
                                        if (box[w] == 2) {
                                            if (p[P_id].move >= te[xbox][ybox + 1].mA) {
                                                p[P_id].move = p[P_id].move - te[xbox][ybox + 1].mA;
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                        else if (box[w] == 4) {
                                            if (p[P_id].move >= te[xbox - 1][ybox].mA) {
                                                p[P_id].move =p[P_id].move - te[xbox - 1][ybox].mA;
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                        else if (box[w] == 6) {
                                            if (p[P_id].move >= te[xbox + 1][ybox].mA) {
                                                p[P_id].move = p[P_id].move - te[xbox + 1][ybox].mA;
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                        else if (box[w] == 8) {
                                            if (p[P_id].move >= te[xbox][ybox - 1].mA) {
                                                p[P_id].move = p[P_id].move - te[xbox][ybox - 1].mA;
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                        if (box[w] == 2) {

                                            for (k = 0; k <= 48; k += 16) {

                                                BeginBatchDraw();
                                                maps(p, P_id, e, b_m, b_mid);
                                                e_put(e, b_m, esize, b_mid);
                                                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                                p_bput(p, b_m, b_m[b_mid].psize, b_mid,P_id);
                                                if (xbox * 48 - b_m[b_mid].ox < 960 && ybox * 48 - 16 + k - b_m[b_mid].oy < 720) {
                                                    transparentimage(NULL, xbox * 48 - b_m[b_mid].ox, ybox * 48 - 16 + k - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 8, 0, 48, 64);
                                                }
                                                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                                                EndBatchDraw();
                                                Sleep(30);
                                                p[P_id].pose = 1;
                                            }
                                            ybox++;
                                        }
                                        else if (box[w] == 4) {
                                            for (k = 0; k <= 48; k += 16) {

                                                BeginBatchDraw();
                                                maps(p, P_id, e, b_m, b_mid);
                                                e_put(e, b_m, esize, b_mid);                        
                                                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                                p_bput(p, b_m, b_m[b_mid].psize, b_mid, P_id);
                                                if (xbox * 48 - k - b_m[b_mid].ox < 960-k && ybox * 48 - 16 - b_m[b_mid].oy < 720) {
                                                    transparentimage(NULL, xbox * 48 - k - b_m[b_mid].ox, ybox * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 16, 64, 48, 64);
                                                }
                                                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                                                EndBatchDraw();
                                                Sleep(30);
                                                p[P_id].pose =2 ;
                                            }
                                            xbox--;

                                        }
                                        else if (box[w] == 6) {
                                            for (k = 0; k <= 48; k += 16) {

                                                BeginBatchDraw();
                                                maps(p, P_id, e, b_m, b_mid);
                                                e_put(e, b_m, esize, b_mid);                  
                                                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                                p_bput(p, b_m, b_m[b_mid].psize, b_mid, P_id);
                                                if (xbox * 48 + k - b_m[b_mid].ox < 960 && ybox * 48 - 16 - b_m[b_mid].oy < 720) {
                                                    transparentimage(NULL, xbox * 48 + k - b_m[b_mid].ox, ybox * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 0, 128, 48, 64);
                                                }
                                                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                                                EndBatchDraw();
                                                Sleep(30);
                                                p[P_id].pose = 3;
                                            }
                                            xbox++;
                                        }
                                        else if (box[w] == 8) {
                                            for (k = 0; k <= 48; k += 16) {

                                                BeginBatchDraw();
                                                maps(p, P_id, e, b_m, b_mid);
                                                e_put(e, b_m, esize, b_mid);                        
                                                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                                p_bput(p, b_m, b_m[b_mid].psize, b_mid, P_id);
                                                if (xbox * 48 - b_m[b_mid].ox < 960 && ybox * 48 - 16 - k - b_m[b_mid].oy < 720) {
                                                    transparentimage(NULL, xbox * 48 - b_m[b_mid].ox, ybox * 48 - 16 - k - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 8, 192, 48, 64);
                                                }
                                                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
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
                                    int x = p[P_id].x, y = p[P_id].y;
                                    if (x + 1 < b_m[b_mid].x && te[x + 1][y].type == 0) {
                                        te[x + 1][y].mB -= 1;
                                    }
                                    if (x - 1 >= 0 && te[x - 1][y].type == 0) {
                                        te[x - 1][y].mB -= 1;
                                    }
                                    if (y + 1 < b_m[b_mid].y && te[x][y + 1].type == 0) {
                                        te[x][y + 1].mB -= 1;
                                    }
                                    if (y - 1 >= 0 && te[x][y - 1].type == 0) {
                                        te[x][y - 1].mB -= 1;
                                    }
                                    te[p[P_id].x][p[P_id].y].mA -= 10000;
                                    te[p[P_id].x][p[P_id].y].mB -= 10000;
                                    te[p[P_id].x][p[P_id].y].player = 0;
                                    *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"從(" << p[P_id].x << L"," << p[P_id].y << L")到(" << j + b_m[b_mid].ox / 48 << L"," << i + b_m[b_mid].oy / 48 << L")" << endl;
                                    p[P_id].x = j + b_m[b_mid].ox / 48; p[P_id].y = i + b_m[b_mid].oy / 48;
                                    x = p[P_id].x, y = p[P_id].y;
                                    if (x + 1 < b_m[b_mid].x && te[x + 1][y].type == 0) {
                                        te[x + 1][y].mB += 1;
                                    }
                                    if (x - 1 >= 0 && te[x - 1][y].type == 0) {
                                        te[x - 1][y].mB += 1;
                                    }
                                    if (y + 1 < b_m[b_mid].y && te[x][y + 1].type == 0) {
                                        te[x][y + 1].mB += 1;
                                    }
                                    if (y - 1 >= 0 && te[x][y - 1].type == 0) {
                                        te[x][y - 1].mB += 1;
                                    }
                                    te[p[P_id].x][p[P_id].y].mA += 10000;
                                    te[p[P_id].x][p[P_id].y].mB += 10000;
                                    te[p[P_id].x][p[P_id].y].player = 1;
                                    j = 20; i = 15;
                                    sss = 0;        
                            }
                        }
                    }
                case WM_RBUTTONUP:
                    sss = 0;	// 按鼠标右键退出程序

                }
            }
        }
        else if (chose == "w" && p[P_id].move == 0) {
        mciSendString(L"close se", NULL, 0, NULL);
        mciSendString(L"open ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 alias se", NULL, 0, NULL);
        vos = L"setaudio se volume to " + to_wstring(soundSize);
        mciSendString(vos.c_str(), NULL, 0, NULL);
        mciSendString(L"play se from 0", NULL, 0, NULL);
        }
    }
void e_attack(wofstream *wofs,arms *ar,player *p,enemy *e,b_npc *b_n,b_map *b_m, int b_mid, int id, int P_id, int esize, int psize, int bu_id, int b_nid) {
    int rx, ry, ATK, AC, ax, ay;
    wstring nn,vos;
    TCHAR t[5];
    if (e[id].target == 0) {
        if (ar[e[id].baid].range >= abs(e[id].x-p[P_id].x) + abs(e[id].y-p[P_id].y)) {
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
            IMAGE ae;
            if (e[id].baid == 0) {
                mciSendString(L"close se", NULL, 0, NULL);
                mciSendString(L"open ./Game/Sound/SE/拳銃2.mp3 alias se", NULL, 0, NULL);
                vos = L"setaudio se volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play se from 0", NULL, 0, NULL);
            }
            else if (e[id].baid == 4) {
                mciSendString(L"close se", NULL, 0, NULL);
                mciSendString(L"open ./Game/Sound/SE/剣の素振り3.mp3 alias se", NULL, 0, NULL);
                vos = L"setaudio se volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play se from 0", NULL, 0, NULL);
            }
            else if (e[id].baid ==1 ) {
                mciSendString(L"close se", NULL, 0, NULL);
                mciSendString(L"open ./Game/Sound/SE/斧で斬る1.mp3 alias se", NULL, 0, NULL);
                vos = L"setaudio se volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play se from 0", NULL, 0, NULL);
                loadimage(&ae, L"./Game/picture/武器・ツメ2.png", 0, 0, false);
                for (j = 0; j < 5; j++) {
                    for (i = 0; i < 3; i++) {
                        BeginBatchDraw();
                        maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                        e_put(e, b_m, esize, b_mid);
                        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, psize, b_mid); 
                        transparentimageA(NULL, p[P_id].x * 48 - b_m[b_mid].ox - 25, p[P_id].y * 48 - 16 - b_m[b_mid].oy - 20, &ae, 0x000000, i * 120, j * 120, 120, 120);
                        ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);                       
                        EndBatchDraw();
                        Sleep(5);
                    }
                }
            }
            if (ATK > AC) {
                BeginBatchDraw();
                maps(p, P_id, e,b_m,b_mid);/*地圖繪製*/
                e_put(e, b_m, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                int  DMG = roll(ar[e[id].baid].dmg, 1);
                _stprintf(t, _T("%d"), -DMG);
                settextcolor(RGB(255,0,0));
                settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                outtextxy(p[P_id].x * 48 - b_m[b_mid].ox, p[P_id].y * 48 - b_m[b_mid].oy, t);
                settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                settextcolor(RGB(0,0,0));
                p[P_id].hp -= DMG;
                *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"用"<<ar[e[id].baid].name<<L"命中"<<p[P_id].name<<L"造成"<<DMG<<L"點傷害("<<ATK<<">"<<AC<<")" << endl;
                EndBatchDraw();
                IMAGE tri;
                loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                transparentimage(NULL, (e[id].x) * 48 - b_m[b_mid].ox, ((e[id].y - 1) * 48) - b_m[b_mid].oy, &tri, 0xFF55FF);
                Sleep(1000);
            }
            else {         
                BeginBatchDraw();
                maps(p, P_id, e, b_m,b_mid);/*地圖繪製*/
                e_put(e, b_m, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"用" <<ar[e[id].baid].name<<L"攻擊"<<p[P_id].name<<L"未能命中("<<ATK<<"<="<<AC<<")" << endl;
                nn = L"miss";
                settextcolor(RGB(255, 0, 0));
                settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                outtextxy(p[P_id].x * 48 - b_m[b_mid].ox, p[P_id].y * 48 - b_m[b_mid].oy, nn.c_str());
                settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                settextcolor(RGB(0, 0, 0));
                EndBatchDraw();
                IMAGE tri;
                loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                transparentimage(NULL, (e[id].x) * 48-b_m[b_mid].ox, ((e[id].y - 1) * 48) - b_m[b_mid].oy, &tri, 0xFF55FF);
                Sleep(1000);
            }
        }
    }
    else if (e[id].target == 1) {
        if (ar[e[id].baid].range >= abs(e[id].x - b_n[b_nid].x) + abs(e[id].y - b_n[b_nid].y)) {
            ay = e[id].y - b_n[b_nid].y; ax = e[id].x - b_n[b_nid].x;
            if (ay > 0 && ay > abs(ax)) {
                e[id].pose = 4;
            }
            else if (ax > 0 && ax >= abs(ay)) {
                e[id].pose = 2;
            }
            else if (ax < 0 && abs(ax) >= abs(ay)) {
                e[id].pose = 3;
            }
            else if (ay < 0 && abs(ay) > abs(ax)) {
                e[id].pose = 1;
            }
            ATK = roll("", 2) + roll(ar[e[id].baid].hit, 1);
            AC = roll("", 2) + (b_n[b_nid].dex - 10) / 2;
            IMAGE ae;
            if (e[id].baid == 0) {
                mciSendString(L"close se", NULL, 0, NULL);
                mciSendString(L"open ./Game/Sound/SE/拳銃2.mp3 alias se", NULL, 0, NULL);
                vos = L"setaudio se volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play se from 0", NULL, 0, NULL);
            }
            else if (e[id].baid == 4) {
                mciSendString(L"close se", NULL, 0, NULL);
                mciSendString(L"open ./Game/Sound/SE/剣の素振り3.mp3 alias se", NULL, 0, NULL);
                vos = L"setaudio se volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play se from 0", NULL, 0, NULL);
            }
            else if (e[id].baid == 1) {
                mciSendString(L"close se", NULL, 0, NULL);
                mciSendString(L"open ./Game/Sound/SE/斧で斬る1.mp3 alias se", NULL, 0, NULL);
                vos = L"setaudio se volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play se from 0", NULL, 0, NULL);
                loadimage(&ae, L"./Game/picture/武器・ツメ2.png", 0, 0, false);
                for (j = 0; j < 5; j++) {
                    for (i = 0; i < 3; i++) {
                        BeginBatchDraw();
                        maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                        e_put(e, b_m, esize, b_mid);
                        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, psize, b_mid);
                        transparentimageA(NULL, b_n[b_nid].x * 48 - b_m[b_mid].ox - 25, b_n[b_nid].y * 48 - 16 - b_m[b_mid].oy - 20, &ae, 0x000000, i * 120, j * 120, 120, 120);
                        ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                        EndBatchDraw();
                        Sleep(5);
                    }
                }
            }
            if (ATK > AC) {
                BeginBatchDraw();
                maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                e_put(e, b_m, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                int  DMG = roll(ar[e[id].baid].dmg, 1);
                _stprintf(t, _T("%d"), -DMG);
                settextcolor(RGB(255, 0, 0));
                settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                outtextxy(b_n[b_nid].x * 48 - b_m[b_mid].ox, b_n[b_nid].y * 48 - b_m[b_mid].oy, t);
                settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                settextcolor(RGB(0, 0, 0));
                b_n[b_nid].hp -= DMG;
                *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"用" << ar[e[id].baid].name << L"命中" << b_n[b_nid].name << L"造成" << DMG << L"點傷害(" << ATK << ">" << AC << ")" << endl;
                EndBatchDraw();
                IMAGE tri;
                loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                transparentimage(NULL, (e[id].x) * 48 - b_m[b_mid].ox, ((e[id].y - 1) * 48) - b_m[b_mid].oy, &tri, 0xFF55FF);
                Sleep(1000);
            }
            else {
                BeginBatchDraw();
                maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                e_put(e, b_m, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"用" << ar[e[id].baid].name << L"攻擊" << b_n[b_nid].name << L"未能命中(" << ATK << "<=" << AC << ")" << endl;
                nn = L"miss";
                settextcolor(RGB(255, 0, 0));
                settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                outtextxy(b_n[b_nid].x * 48 - b_m[b_mid].ox, b_n[b_nid].y * 48 - b_m[b_mid].oy, nn.c_str());
                settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                settextcolor(RGB(0, 0, 0));
                EndBatchDraw();
                IMAGE tri;
                loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                transparentimage(NULL, (e[id].x) * 48 - b_m[b_mid].ox, ((e[id].y - 1) * 48) - b_m[b_mid].oy, &tri, 0xFF55FF);
                Sleep(1000);
            }
        }
    }
    }
void e_target(wostream *wofs,enemy *e,player *p,b_map *b_m,b_npc *b_n,int &P_id,int id,int b_mid,int psize,int nsize,int &b_nid) {
        int min = 1000;
        int box;
        for (i = 0; i < psize; i++) {
            box = abs(e[id].x - p[i].x) + abs(e[id].y - p[i].y);
            if (box < min) {
                min = box;
                P_id = i;
                e[id].target = 0;
            }
        }
        for (i = 0; i < nsize; i++) {
            box = abs(e[id].x - b_n[b_nid].x) + abs(e[id].y - b_n[b_nid].y);
            if (box < min) {
                min = box;
                b_nid = i;
                e[id].target = 1;
            }
        }
        if (e[id].target == 0) {
            *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name<<L"將"<<p[P_id].name<<L"選為目標" << endl;
        }
        else if (e[id].target == 1) {
            *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"將" << b_n[b_nid].name << L"選為目標" << endl;
        }
    }
void e_walk(wofstream *wofs, enemy* e, player* p,b_npc *b_n, b_map* b_m, terrain(*te)[50], int id, int P_id, int esize, int psize, int b_mid, int bu_id, int b_nid, int target) {
    int box[1000], w = 0, k, xbox, ybox;
    if (target == 0) {
        if(abs(p[P_id].x-e[id].x)+abs(p[P_id].y - e[id].y)<=1) {
        return;
    }
        Bbfs(b_m,te,e[id].x, e[id].y, p[P_id].x, p[P_id].y, box,b_mid,e[id].Move);

     }
    else if (target == 1) {
    if (abs(b_n[b_nid].x - e[id].x) + abs(b_n[b_nid].y - e[id].y) <= 1) {
        return;
    }
    Bbfs(b_m, te, e[id].x, e[id].y, b_n[b_nid].x, b_n[b_nid].y, box,b_mid,e[id].Move);
    }    
    xbox = e[id].x; ybox = e[id].y;
    wstring mm;
    mm = std::to_wstring(e[id].type);
    mm = L"./Game/picture/enemy" + mm + L".png";
    IMAGE enemy1;
    loadimage(&enemy1, mm.c_str(), 0, 0, false);
    b_camera(b_m, xbox, ybox, b_mid);
    while (1) {
        if (box[w] == 2) {
            if (e[id].move >= te[xbox][ybox+1].mB) {
                e[id].move = e[id].move - te[xbox][ybox+1].mB;
            }
            else {
                break;
            }
        }
        else if (box[w] == 4) {
            if (e[id].move >= te[xbox-1][ybox].mB) {
                e[id].move = e[id].move - te[xbox-1][ybox].mB;
            }
            else {
                break;
            }
        }
        else if (box[w] == 6) {
            if (e[id].move >= te[xbox+1][ybox].mB) {
                e[id].move = e[id].move - te[xbox+1][ybox].mB;
            }
            else {
                break;
            }
        }
        else if (box[w] == 8) {
            if (e[id].move >= te[xbox][ybox-1].mB) {
                e[id].move = e[id].move - te[xbox][ybox-1].mB;
            }
            else {
                break;
            }
        }
        if (box[w] == 2) {

            for (k = 0; k <= 48; k += 16) {

                BeginBatchDraw();
                maps(p, P_id, e, b_m, b_mid);
                e_bput(e, b_m, id, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                if (xbox * 48 - b_m[b_mid].ox < 960 && ((ybox) * 48 + k) - b_m[b_mid].oy - 16 < 720) {
                    transparentimage(NULL, xbox * 48 - b_m[b_mid].ox, ((ybox) * 48 + k) - b_m[b_mid].oy - 16, &enemy1, 0xFF55FF, 64 + 8, 0, 48, 64);
                }
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                EndBatchDraw();
                Sleep(30);
                e[id].pose = 1;
            }
            ybox++;
        }
        else if (box[w] == 4) {
            for (k = 0; k <= 48; k += 16) {

                BeginBatchDraw();
                maps(p, P_id, e, b_m, b_mid);
                e_bput(e, b_m, id, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                if (xbox * 48 - k - b_m[b_mid].ox < 960 && ybox * 48 - b_m[b_mid].oy - 16) {
                    transparentimage(NULL, xbox * 48 - k - b_m[b_mid].ox, ybox * 48 - b_m[b_mid].oy - 16, &enemy1, 0xFF55FF, 64 + 16, 64, 48, 64);
                }
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                EndBatchDraw();
                Sleep(30);
                e[id].pose = 2;
            }
            xbox--;

        }
        else if (box[w] == 6) {
            for (k = 0; k <= 48; k += 16) {

                BeginBatchDraw();
                maps(p, P_id, e, b_m, b_mid);
                e_bput(e, b_m, id, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                if (xbox * 48 + k - b_m[b_mid].ox < 960 && ybox * 48 - b_m[b_mid].oy - 16 < 720) {
                    transparentimage(NULL, xbox * 48 + k - b_m[b_mid].ox, ybox * 48 - b_m[b_mid].oy - 16, &enemy1, 0xFF55FF, 64 + 0, 128, 48, 64);
                }
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                EndBatchDraw();
                Sleep(30);
                e[id].pose = 3;
            }
            xbox++;
        }
        else if (box[w] == 8) {
            for (k = 0; k <= 48; k += 16) {
                BeginBatchDraw();
                maps(p, P_id, e, b_m, b_mid);
                e_bput(e, b_m, id, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                if (xbox * 48 - b_m[b_mid].ox < 960 && (ybox * 48) - k - b_m[b_mid].oy - 16 < 720) {
                    transparentimage(NULL, xbox * 48 - b_m[b_mid].ox, (ybox * 48) - k - b_m[b_mid].oy - 16, &enemy1, 0xFF55FF, 64 + 8, 192, 48, 64);
                }
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
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
        int x = e[id].x,y=e[id].y;
        if (x + 1 < b_m[b_mid].x && te[x + 1][y].type == 0) {
            te[x + 1][y].mA -= 1;
        }
        if (x - 1 >= 0 && te[x - 1][y].type == 0) {
            te[x - 1][y].mA -= 1;
        }
        if (y + 1 < b_m[b_mid].y && te[x][y + 1].type == 0) {
            te[x][y + 1].mA -= 1;
        }
        if (y - 1 >= 0 && te[x][y - 1].type == 0) {
            te[x][y - 1].mA -= 1;
        }
        te[e[id].x][e[id].y].mA -= 10000;
        te[e[id].x][e[id].y].mB -= 10000;
        te[e[id].x][e[id].y].enemy = 0;
        *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"從(" << e[id].x << L"," << e[id].y << L")到(" << xbox << L"," << ybox << L")" << endl;
        e[id].x = xbox; e[id].y = ybox;
        x = e[id].x, y = e[id].y;
        if (x + 1 < b_m[b_mid].x && te[x + 1][y].type == 0) {
            te[x + 1][y].mA += 1;
        }
        if (x - 1 >= 0 && te[x - 1][y].type == 0) {
            te[x - 1][y].mA += 1;
        }
        if (y + 1 < b_m[b_mid].y && te[x][y + 1].type == 0) {
            te[x][y + 1].mA += 1;
        }
        if (y - 1 >= 0 && te[x][y - 1].type == 0) {
            te[x][y - 1].mA += 1;
        }
        te[e[id].x][e[id].y].mA += 10000;
        te[e[id].x][e[id].y].mB += 10000;
        te[e[id].x][e[id].y].enemy = 1;
    }
void b_log(b_map* b_m, int b_mid,int line) {
        int J=0;
        int I = 0;
        wstring word;
        wifstream wifs;
        wifs.open(b_m[b_mid].log_name, ios::in);
        BeginBatchDraw();
        putimage(960, 0, &text);
        while (getline(wifs, word)) {
            if (word[0] != '[') {
                if (J < b_m[b_mid].line - line+3 && J >= b_m[b_mid].line-line) {
                    outtextxy(960, I * 48, word.c_str());
                    I++;
                }
            }
            J++;
        }
        EndBatchDraw();
        wifs.close();
        if (b_m[b_mid].line -line< J&&b_m[b_mid].line-line>0) {
            b_m[b_mid].line -= line;
        }
}
void acts( player *p,enemy *e,b_npc *b_n,b_map *b_m,arms *ar,buff *bu, terrain(*te)[50],string& chose,int b_mid, int P_id,int esize,int id,int psize,int bu_id,int b_nid) {
    ExMessage m;
    int sss = 1;
    int Flag = -1;
    int flag1 = 1;
    int flag2 = 1;
    int move[50][50];
    int cost[50][50];
    int u=0,un=0,ue=0,up=0;
    int uk[50],kn[50],ke[50],kp[50];
    for (i = 0; i < bu_id; i++) {
        if (p[P_id].buff_check[i] >= 1) {           
            uk[u]=i; 
            u++;
        }
    }

    IMAGE g,ug,get;
    TCHAR t[5];   
    loadimage(&ug, L"./Game/picture/bblock.png", 0, 0, false);
    while (sss != 0)
    {
        Sleep(10);
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            if (b_m[b_mid].cy > 7) {
                b_m[b_mid].cy--;
            }
            sss = 0;
        }
        else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            if (b_m[b_mid].cy + 7 < b_m[b_mid].y) {
                b_m[b_mid].cy++;
            }
            sss = 0;
        }
        else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            if (b_m[b_mid].cx + 9 < b_m[b_mid].x) {
                b_m[b_mid].cx++;
            }
            sss = 0;
        }
        else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            if (b_m[b_mid].cx > 10) {
                b_m[b_mid].cx--;
            }
            sss = 0;
        }
        else {
            while (peekmessage(&m, EM_MOUSE)) {
                if (peekmessage(&m, EM_KEY)) {
                    break;
                }
                Sleep(10);
                if (m.message == WM_MOUSEMOVE) {
                    for (i = 0; i < esize; i++) {
                        if (m.x >= e[i].x * 48 - b_m[b_mid].ox && m.x <= e[i].x * 48 + 48 - b_m[b_mid].ox && m.y >= e[i].y * 48 - b_m[b_mid].oy && m.y <= e[i].y * 48 + 48 - b_m[b_mid].oy && Flag != i && m.x < 960 && m.y < 720) {
                            BeginBatchDraw();
                            maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                            IMAGE tri,ab,mb;
                            loadimage(&ab, L"./Game/picture/ab.png", 0, 0, false);
                            loadimage(&mb, L"./Game/picture/mb.png", 0, 0, false);
                            int MR=e[i].Move + ar[e[i].baid].range;
                            int X = e[i].x,Y=e[i].y;
                            int x = e[i].x,y=e[i].y;
                            int road[50][50],dr[50][50];
                            for (int I = 0; I < 50; I++) {
                                for (int J = 0; J < 50; J++) {
                                    road[I][J] = 0;
                                    dr[I][J] = 0;
                                    move[I][J] = 0;
                                    cost[I][J] = 100000;
                                }
                            }
                            queue<pair<int, int>> q;
                            clearQpair(q);
                            q.push({ X,Y });
                            cost[X][Y] = 0;
                            while (q.size() > 0) {
                                X = q.front().first;
                                Y = q.front().second;
                                if (te[X + 1][Y].mA < 10000 && te[X + 1][Y].npc == 0 && te[X + 1][Y].player == 0 && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x && (abs(X + 1 - x) + abs(Y - y) <= MR) && road[X][Y] < e[i].Move && road[X][Y] >= 0) {
                                    if (cost[X + 1][Y] > cost[X][Y] + te[X][Y].mA) {
                                        q.push({ X + 1,Y });
                                        if (road[X][Y] + te[X + 1][Y].mA <= e[i].Move) {
                                            road[X + 1][Y] = road[X][Y] + te[X + 1][Y].mA;
                                            cost[X + 1][Y] = cost[X][Y] + te[X][Y].mA;
                                            move[X + 1][Y] = 0;
                                        }
                                    }
                                }
                                else if (dr[X + 1][Y] == 0 && move[X][Y] < ar[e[i].baid].range && te[X + 1][Y].type != 1 && road[X+1][Y] == 0) {
                                    dr[X + 1][Y] = 1;
                                    move[X + 1][Y] = move[X][Y] + 1;
                                    road[X + 1][Y] = -1;
                                    q.push({ X + 1,Y });
                                }
                                if (te[X - 1][Y].mA < 10000 && te[X - 1][Y].npc == 0 && te[X - 1][Y].player == 0 && te[X - 1][Y].type != 1 && X - 1 >= 0 && (abs(X - 1 - x) + abs(Y - y) <= MR) && road[X][Y] < e[i].Move && road[X][Y] >= 0) {
                                    if (cost[X - 1][Y] > cost[X][Y] + te[X][Y].mA) {
                                        q.push({ X - 1,Y });
                                        if (road[X][Y] + te[X - 1][Y].mA <= e[i].Move) {
                                            road[X - 1][Y] = road[X][Y] + te[X - 1][Y].mA;
                                            cost[X - 1][Y] = cost[X][Y] + te[X][Y].mA;
                                            move[X - 1][Y] = 0;
                                        }
                                    }
                                }
                                else if (dr[X - 1][Y] == 0 && move[X][Y] < ar[e[i].baid].range && te[X - 1][Y].type != 1 && road[X-1][Y] == 0) {
                                    dr[X - 1][Y] = 1;
                                    move[X - 1][Y] = move[X][Y] + 1;
                                    road[X - 1][Y] = -1;
                                    q.push({ X - 1,Y });
                                }
                                if (te[X][Y + 1].mA < 10000 && te[X][Y + 1].npc == 0 && te[X][Y + 1].player == 0 && te[X][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y && (abs(X - x) + abs(Y + 1 - y) <= MR) && road[X][Y] < e[i].Move && road[X][Y] >= 0) {
                                    if (cost[X][Y + 1] > cost[X][Y] + te[X][Y].mA) {
                                        q.push({ X ,Y + 1 });
                                        if (road[X][Y] + te[X][Y + 1].mA <= e[i].Move) {
                                            road[X][Y + 1] = road[X][Y] + te[X][Y + 1].mA;
                                            cost[X][Y + 1] = cost[X][Y] + te[X][Y].mA;
                                            move[X][Y + 1] = 0;
                                        }
                                    }
                                }
                                else if (dr[X][Y + 1] == 0 && move[X][Y] < ar[e[i].baid].range && te[X][Y + 1].type != 1 && road[X][Y+1] == 0) {
                                    dr[X][Y + 1] = 1;
                                    move[X][Y + 1] = move[X][Y] + 1;
                                    road[X][Y + 1] = -1;
                                    q.push({ X,Y + 1 });
                                }
                                if (te[X][Y - 1].mA < 10000 && te[X][Y - 1].npc == 0 && te[X][Y - 1].player == 0 && te[X][Y - 1].type != 1 && Y - 1 >= 0 && (abs(X - x) + abs(Y - 1 - y) <= MR) && road[X][Y] < e[i].Move && road[X][Y] >= 0) {
                                    if (cost[X][Y - 1] > cost[X][Y] + te[X][Y].mA) {
                                        q.push({ X ,Y - 1 });
                                        if (road[X][Y] + te[X][Y - 1].mA <= e[i].Move) {
                                            road[X][Y - 1] = road[X][Y] + te[X][Y - 1].mA;
                                            cost[X][Y - 1] = cost[X][Y] + te[X][Y].mA;
                                            move[X][Y - 1] = 0;
                                        }
                                    }
                                }
                                else if (dr[X][Y - 1] == 0 && move[X][Y] < ar[e[i].baid].range && te[X][Y - 1].type != 1 && road[X][Y-1] == 0) {
                                    dr[X][Y - 1] = 1;
                                    move[X][Y - 1] = move[X][Y] + 1;
                                    road[X][Y - 1] = -1;
                                    q.push({ X ,Y - 1 });
                                }
                                if ((X != x || Y != y) && road[X][Y] != 0) {
                                    dr[X][Y] = 1;
                                }
                                q.pop();
                            }
                            for (int I = 0; I < b_m[b_mid].x; I++) {
                                for (int J = 0; J < b_m[b_mid].y; J++) {
                                    if (move[I][J] == 0 && (I != x || J != y) && dr[I][J] == 1&&te[I][J].player==0&&te[I][J].npc==0) {
                                        transparentimage(NULL, 48 * I - b_m[b_mid].ox, 48 * J - b_m[b_mid].oy, &mb);
                                    }
                                    else if ((I != x || J != y) && dr[I][J] == 1 && te[I][J].enemy == 0) {
                                        transparentimage(NULL, 48 * I - b_m[b_mid].ox, 48 *J - b_m[b_mid].oy, &ab);
                                    }
                                }
                            }
                            e_put(e, b_m, b_m[b_mid].esize, b_mid);
                            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                            p_put(p, b_m, psize, b_mid);
                            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                            outtextxy(960, 0, e[i].name.c_str());
                            Flag = 1;
                            wstring mm = L"HP[" + to_wstring(e[i].hp) + L"/" + to_wstring(e[i].mhp) + L"]";
                            outtextxy(960, 30, mm.c_str());
                            loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);                            
                            transparentimage(NULL, (e[i].x) * 48 - b_m[b_mid].ox, ((e[i].y - 1) * 48) - b_m[b_mid].oy, &tri, 0xFF55FF);
                            ue = 0;
                            for (int I = 0; I < bu_id; I++) {
                                if (e[i].buff_id[I] >= 1) {
                                    mm = L"./Game/picture/buff" + to_wstring(I) + L".png";
                                    loadimage(&g, mm.c_str(), 0, 0, false);
                                    putimage(960 + 30 * ue, 100, &g);
                                    _stprintf(t, _T("%d"), b_n[i].buff_check[I]);
                                    outtextxy(960 + 30 * ue, 130, t);
                                    ke[ue] = I;
                                    ue++;
                                }
                            }
                            EndBatchDraw();
                            j = i;
                            Flag = i;
                            flag1 = -1;
                            flag2 = -1;
                            break;
                        }

                    }
                    for (i = 0; i <b_m[b_mid].nsize; i++) {
                        if (m.x >= b_n[i].x * 48 - b_m[b_mid].ox && m.x <= b_n[i].x * 48 + 48 - b_m[b_mid].ox && m.y >= b_n[i].y * 48 - b_m[b_mid].oy && m.y <= b_n[i].y * 48 + 48 - b_m[b_mid].oy && flag1 != i && m.x < 960 && m.y < 720) {
                            BeginBatchDraw();
                            maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/                           
                            IMAGE tri, ab, mb;
                            loadimage(&ab, L"./Game/picture/ab.png", 0, 0, false);
                            loadimage(&mb, L"./Game/picture/mb.png", 0, 0, false);
                            int MR = b_n[i].Move + ar[b_n[i].baid].range;
                            int X = b_n[i].x, Y = b_n[i].y;
                            int x = b_n[i].x, y = b_n[i].y;
                            int road[50][50], dr[50][50];
                            for (int I = 0; I < 50; I++) {
                                for (int J = 0; J < 50; J++) {
                                    road[I][J] = 0;
                                    dr[I][J] = 0;
                                    move[I][J] = 0;
                                    cost[I][J] = 100000;
                                }
                            }
                            queue<pair<int, int>> q;
                            clearQpair(q);
                            q.push({ X,Y });
                            cost[X][Y] = 0;
                            while (q.size() > 0) {
                                X = q.front().first;
                                Y = q.front().second;
                                if (te[X + 1][Y].mA < 10000 && te[X + 1][Y].npc == 0 && te[X + 1][Y].player == 0 && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x && (abs(X + 1 - x) + abs(Y - y) <= MR) && road[X][Y] < b_n[i].Move && road[X][Y] >= 0) {
                                    if (cost[X + 1][Y] > cost[X][Y] + te[X][Y].mA) {
                                        q.push({ X + 1,Y });
                                        if (road[X][Y] + te[X + 1][Y].mA <= b_n[i].Move) {
                                            road[X + 1][Y] = road[X][Y] + te[X + 1][Y].mA;
                                            cost[X + 1][Y] = cost[X][Y] + te[X][Y].mA;
                                            move[X+1][Y] = 0;
                                        }
                                    }
                                }
                                else if (dr[X + 1][Y] == 0 && move[X][Y] <ar[b_n[i].baid].range&&te[X+1][Y].type!=1 && road[X+1][Y] == 0) {
                                    dr[X + 1][Y] = 1;
                                    move[X + 1][Y] = move[X][Y] + 1;
                                    road[X +1][Y] = -1;
                                    q.push({ X + 1,Y });
                                }
                                if (te[X - 1][Y].mA < 10000 && te[X - 1][Y].npc == 0 && te[X -1][Y].player == 0 && te[X - 1][Y].type != 1 && X - 1 >= 0 && (abs(X - 1 - x) + abs(Y - y) <= MR) && road[X][Y] < b_n[i].Move && road[X][Y] >= 0) {
                                    if (cost[X - 1][Y] > cost[X][Y] + te[X][Y].mA) {
                                        q.push({ X - 1,Y });
                                        if (road[X][Y] + te[X - 1][Y].mA <= b_n[i].Move) {
                                            road[X - 1][Y] = road[X][Y] + te[X - 1][Y].mA;
                                            cost[X - 1][Y] = cost[X][Y] + te[X][Y].mA;
                                            move[X-1][Y] = 0;
                                        }
                                    }
                                }
                                else if (dr[X - 1][Y] == 0 && move[X][Y] <ar[b_n[i].baid].range && te[X - 1][Y].type != 1 && road[X-1][Y] == 0) {
                                    dr[X - 1][Y] = 1;
                                    move[X - 1][Y] = move[X][Y] + 1;
                                    road[X-1][Y] = -1;
                                    q.push({ X - 1,Y });
                                }
                                if (te[X][Y + 1].mA < 10000 && te[X][Y + 1].npc == 0 && te[X][Y + 1].player == 0&& te[X][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y && (abs(X - x) + abs(Y + 1 - y) <= MR) && road[X][Y] < b_n[i].Move && road[X][Y] >= 0) {
                                    if (cost[X][Y + 1] > cost[X][Y] + te[X][Y].mA) {
                                        q.push({ X ,Y + 1 });
                                        if (road[X][Y] + te[X][Y + 1].mA <= b_n[i].Move ) {
                                            road[X][Y + 1] = road[X][Y] + te[X][Y + 1].mA;
                                            cost[X][Y + 1] = cost[X][Y] + te[X][Y].mA;
                                            move[X][Y + 1] = 0;
                                        }
                                    }
                                }
                                else if (dr[X][Y + 1] == 0 && move[X][Y] < ar[b_n[i].baid].range && te[X][Y+1].type != 1 && road[X][Y+1] == 0) {
                                    dr[X][Y + 1] = 1;
                                    move[X][Y+1] = move[X][Y] + 1;
                                    road[X][Y + 1] = -1;
                                    q.push({ X,Y+1 });
                                }
                                if (te[X][Y - 1].mA < 10000 && te[X][Y - 1].npc == 0 && te[X][Y - 1].player == 0 && te[X][Y - 1].type != 1 && Y - 1 >= 0 && (abs(X - x) + abs(Y - 1 - y) <= MR) && road[X][Y] < b_n[i].Move &&road[X][Y]>=0) {
                                    if (cost[X][Y - 1] > cost[X][Y] + te[X][Y].mA) {
                                        q.push({ X ,Y - 1 });
                                        if (road[X][Y] + te[X][Y - 1].mA <= b_n[i].Move ) {
                                            road[X][Y - 1] = road[X][Y] + te[X][Y - 1].mA;
                                            cost[X][Y - 1] = cost[X][Y] + te[X][Y].mA;
                                            move[X][Y - 1] = 0;
                                        }
                                    }
                                }
                                else if ( dr[X][Y - 1] == 0 && move[X][Y] <ar[b_n[i].baid].range && te[X][Y- 1].type != 1&&road[X][Y-1]==0) {
                                    dr[X][Y - 1] = 1;
                                    move[X][Y-1] = move[X][Y] + 1;
                                    road[X][Y - 1] = -1;
                                    q.push({ X ,Y-1 });
                                }
                                if ((X != x || Y != y) && road[X][Y] != 0) {
                                    dr[X][Y] = 1;
                                }                                
                                q.pop();
                            }
                            for (int I = 0; I < b_m[b_mid].x; I++) {
                                for (int J = 0; J < b_m[b_mid].y; J++) {
                                    if (move[I][J] == 0 && (I != x || J != y) && dr[I][J] == 1 && te[I][J].enemy == 0) {
                                        transparentimage(NULL, 48 * I - b_m[b_mid].ox, 48 * J - b_m[b_mid].oy, &mb);
                                    }
                                    else if ((I != x || J != y) && dr[I][J] == 1 && te[I][J].npc == 0 && te[I][J].player == 0) {
                                        transparentimage(NULL, 48 * I - b_m[b_mid].ox, 48 * J - b_m[b_mid].oy, &ab);
                                    }
                                }
                            }
                            e_put(e, b_m, b_m[b_mid].esize, b_mid);
                            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                            p_put(p, b_m, psize, b_mid);
                            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                            outtextxy(960, 0, b_n[i].name.c_str());
                            Flag = 1;
                            wstring mm = L"HP[" + to_wstring(b_n[i].hp) + L"/" + to_wstring(b_n[i].mhp) + L"]";
                            outtextxy(960, 30, mm.c_str());
                            Flag = 1;
                            loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                            transparentimage(NULL, (b_n[i].x) * 48 - b_m[b_mid].ox, ((b_n[i].y - 1) * 48) - b_m[b_mid].oy, &tri, 0xFF55FF);  
                            un = 0;
                            for (int I = 0; I < bu_id; I++) {
                                if (b_n[i].buff_check[I] >= 1) {
                                    mm = L"./Game/picture/buff" + to_wstring(I) + L".png";
                                    loadimage(&g, mm.c_str(), 0, 0, false);
                                    putimage(960 + 30 * un,100, &g);
                                    _stprintf(t, _T("%d"), b_n[i].buff_check[I]);
                                    outtextxy(960 + 30 * un, 130, t);
                                    kn[un] = I;
                                    un++;
                                }
                            }
                            EndBatchDraw();
                            getimage(&get, 0, 0, 1296, 960);
                            j = i;
                            flag1 = i;
                            Flag = -1;
                            flag2 = -1;
                            break;
                        }
                    }
                    for (i = 0; i < b_m[b_mid].psize; i++) {
                        if (m.x >= p[i].x * 48 - b_m[b_mid].ox && m.x <= p[i].x * 48 + 48 - b_m[b_mid].ox && m.y >= p[i].y * 48 - b_m[b_mid].oy && m.y <= p[i].y * 48 + 48 - b_m[b_mid].oy && flag2 != i&&P_id!=i && m.x < 960 && m.y < 720) {
                            IMAGE tri;
                            BeginBatchDraw();
                            maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                            e_put(e, b_m, b_m[b_mid].esize, b_mid);
                            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                            p_put(p, b_m, psize, b_mid);
                            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                            outtextxy(960, 0, p[i].name.c_str());
                            Flag = 1;
                            wstring mm = L"HP[" + to_wstring(p[i].hp) + L"/" + to_wstring(p[i].mhp) + L"]";
                            outtextxy(960, 30, mm.c_str());
                            Flag = 1;
                            loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                            transparentimage(NULL, (p[i].x) * 48 - b_m[b_mid].ox, ((p[i].y - 1) * 48) - b_m[b_mid].oy, &tri, 0xFF55FF);
                            up = 0;
                            for (int I = 0; I < bu_id; I++) {
                                if (p[i].buff_check[I] >= 1) {
                                    mm = L"./Game/picture/buff" + to_wstring(I) + L".png";
                                    loadimage(&g, mm.c_str(), 0, 0, false);
                                    putimage(960 + 30 * up, 100, &g);
                                    _stprintf(t, _T("%d"), p[i].buff_check[I]);
                                    outtextxy(960 + 30 * up, 130, t);
                                    kp[up] = I;
                                    up++;
                                }
                            }
                            EndBatchDraw();
                            getimage(&get, 0, 0, 1296, 960);
                            j = i;
                            flag2 = i;
                            Flag = -1;
                            flag1 = -1;
                            break;
                        }
                    }
                    for (i = 0; i < u; i++) {
                        if (m.x>200+30*i&&m.x<200+30*i+20&&m.y<920&&m.y>900) {
                            BeginBatchDraw();
                            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                            RECT r = { 245+30*i, 810, 245+30*i+190, 880 };
                            putimage(240 + 30 * i, 800, &ug);
                            drawtext(bu[uk[i]].story.c_str(), &r, DT_WORDBREAK);
                            EndBatchDraw();
                        }
                    }
                    if (Flag != -1) {

                    }
                    else if (flag1 != -1) {
                        for (i = 0; i < un; i++) {
                            if (m.x>960+30*i&&m.x<960+30*i+20&&m.y>100&&m.y<120) {
                                BeginBatchDraw();
                                putimage(0, 0, &get);
                                RECT r = { 1005 + 30 * i, 100, 1005 + 30 * i + 190, 170 };
                                putimage(1005 + 30 * i, 100, &ug);
                                drawtext(bu[kn[i]].story.c_str(), &r, DT_WORDBREAK);
                                EndBatchDraw();
                            }
                        }
                    }
                    else if (flag2 != -1) {
                        for (i = 0; i < up; i++) {
                            if (m.x > 960 + 30 * i && m.x < 960 + 30 * i + 20 && m.y>100 && m.y < 120) {
                                BeginBatchDraw();
                                putimage(0, 0, &get);
                                RECT r = { 1005 + 30 * i, 100, 1005 + 30 * i + 190, 170 };
                                putimage(1005 + 30 * i, 100, &ug);
                                drawtext(bu[kp[i]].story.c_str(), &r, DT_WORDBREAK);
                                EndBatchDraw();
                            }
                        }
                    }
                }
                if (m.message == WM_LBUTTONDOWN) {//按鼠标左键分发事件
                    if (m.x >= 200 && m.x <= 296 && m.y >= 750 && m.y <= 798) {
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
                        chose = "k";
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
                    if (m.x >= 800 && m.x <= 896 && m.y >= 750 && m.y <= 798) {
                        chose = "e";
                        sss = 0;
                    }
                }
                if (m.message == WM_MOUSEWHEEL) {
                    int delta = m.wheel;
                    int lines = delta / WHEEL_DELTA;
                    b_log(b_m, b_mid, lines);
                }
            }
        }
        }
        
    }
void Save(player* p, enemy* e, arms* ar, item* it,stone *st,flag *f,int P_id,int i_id,int ar_id,int st_id,int f_id,int psize,int esize,int roundp,int roundb,time_t first, time_t two, time_t three,string chose) {
    int u = 0;
    string a, b = ".txt", read = "";
    wstring vos;
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
                            sss = 0;
                            mciSendString(L"close se", NULL, 0, NULL);
                            mciSendString(L"open ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 alias se", NULL, 0, NULL);
                            vos = L"setaudio se volume to " + to_wstring(soundSize);
                            mciSendString(vos.c_str(), NULL, 0, NULL);
                            mciSendString(L"play se from 0", NULL, 0, NULL);
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
                    fout << p[i].arms_id_1 << endl;
                    fout << p[i].arms_id_2 << endl;
                }
                for (i = 0; i < esize; i++) {
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
                fout << 0<< endl;

            }
        }
        putimage(960, 0, &back1);
        IMAGE wolf, p1;
        loadimage(&wolf, L"./Game/picture/A_wolf.png", 0, 0, false);
        loadimage(&p1, L"./Game/picture/A_p1.png", 0, 0, false);
        for (i = 0; i < esize; i++) {
            transparentimage(NULL, 970, 500 - (e[i].turn * 3), &wolf, 0xFF55FF);
        }
        for (i = 0; i < psize; i++) {
            transparentimage(NULL, 1040, 500 - (p[i].turn * 3), &p1, 0xFF55FF);
        }
        }
}
void Load(player* p, enemy* e, arms* ar, item* it, stone* st, flag* f, int &i_id, int &ar_id, int &st_id, int &f_id,  int &P_id, int &second, int &psize, int &esize, int &roundp, int &roundb, time_t &first, time_t &two, time_t &three, string &chose) {
    wstring vos;
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
                            sss = 0;
                            mciSendString(L"close se", NULL, 0, NULL);
                            mciSendString(L"open ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 alias se", NULL, 0, NULL);
                            vos = L"setaudio se volume to " + to_wstring(soundSize);
                            mciSendString(vos.c_str(), NULL, 0, NULL);
                            mciSendString(L"play se from 0", NULL, 0, NULL);
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
                    p[i].arms_id_1 = z[k]; k++;
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
                    p[i].arms_id_1 = z[k]; k++;
                }
                for (i = 0; i < esize; i++) {
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
                putimage(960, 0, &back1);
                IMAGE wolf, p1;
                loadimage(&wolf, L"./Game/picture/A_wolf.png", 0, 0, false);
                loadimage(&p1, L"./Game/picture/A_p1.png", 0, 0, false);
                for (i = 0; i < esize; i++) {
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
void menu_json_save(player* p, arms* ar, item* it, stone* st, flag* f,Exit* EX,m_flag *m_f,t_equip *t_E,int P_id, int i_id, int ar_id, int st_id, int f_id, int psize, time_t first, time_t two, time_t three,string a,int m_id,int ex_id,int sk_id,int m_fid,int t_Eid) {
    Json::Value root;
    two = time(NULL);
    int T = two - first + three;
    root["time"] = Json::Value(T);
    Json::Value player;        
    Json::Value Skill;
    Json::Value t_box;
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
        player["arms_id_1"].append(p[i].arms_id_1);
        player["arms_id_2"].append(p[i].arms_id_2);
        player["state"].append(p[i].state);
        player["exp"].append(p[i].exp);
        player["strExp"].append(p[i].strExp);
        player["dexExp"].append(p[i].dexExp);
        player["intExp"].append(p[i].intExp);
        player["conExp"].append(p[i].conExp);
        player["chaExp"].append(p[i].chaExp);
        player["wisExp"].append(p[i].wisExp);
        player["t_id"].append(p[i].t_id);
        for (j = 0; j < 3; j++) {
            string sj = "size"+to_string(i);
            t_box[sj].append(p[i].box_size[j]);
            sj = "id" + to_string(i);
            t_box[sj].append(p[i].box_id[j]);
        }
        for (j = 0; j < sk_id; j++) {
            string sj = "s_check_" + to_string(i);
            Skill[sj].append(p[i].s_check[j]);
        }

    }        
    player["skill"] = Skill;
    player["t_Box"] = t_box;
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
    for (i = 0; i < ex_id; i++) {
        root["exit"].append(EX[i].state);
    }
    for (i = 0; i < m_fid; i++) {
        root["m_flag"].append(m_f[i].check);
    }
    Json::Value t_equip;
    for (i = 0; i < t_Eid; i++) {
        t_equip["number"].append(t_E[i].number);
    }
    root["t_equip"] = t_equip;
    root["flag"] = flag;
    root["P_id"] =Json::Value( P_id);
    root["m_id"] = Json::Value(m_id);
    root["psize"] = Json::Value(psize);
    root["voiceSize"] = Json::Value(voiceSize);
    root["soundSize"] = Json::Value(soundSize);
    Json::StyledWriter sw;
    string file="./Game/save/"+a+".json";
    ofstream os;
    os.open(file, std::ios::out | std::ios::app);
    os << sw.write(root);
    os.close();
}
void menu_save(player* p, arms* ar, item* it, stone* st, flag* f,Exit* EX,m_flag *m_f,t_equip *t_E, int P_id, int i_id, int ar_id, int st_id, int f_id, int psize, time_t first, time_t two, time_t three,int m_id,int ex_id,int sk_id,int m_fid,int t_Eid) {
    while (1) {
        flushmessage(EM_MOUSE);
        int u = 0;
        string a, b = ".txt", read = "";
        wstring vos;
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
                            sss = 0; 
                            mciSendString(L"close se", NULL, 0, NULL);
                            mciSendString(L"open ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 alias se", NULL, 0, NULL);
                            vos = L"setaudio se volume to " + to_wstring(soundSize);
                            mciSendString(vos.c_str(), NULL, 0, NULL);
                            mciSendString(L"play se from 0", NULL, 0, NULL);
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
            menu_json_save(p, ar, it, st, f,EX,m_f,t_E,P_id, i_id, ar_id, st_id, f_id, psize, first, two, three, a,m_id,ex_id,sk_id,m_fid,t_Eid);
        }
    }
}
void menu_load(player* p, arms* ar, item* it, stone* st, flag* f,Exit* EX, m_flag* m_f,t_equip *t_E, int &P_id, int i_id, int ar_id, int st_id, int f_id, int &psize, time_t first, time_t two, time_t &three,int &m_id,int ex_id,int sk_id,int m_fid,int t_Eid) {
    string a, b = ".txt", read = "";
    int ssss = 1; j = 0;
    wstring vos;
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
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
                        sss = 0; 
                        mciSendString(L"close se", NULL, 0, NULL);
                        mciSendString(L"open ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 alias se", NULL, 0, NULL);
                        vos = L"setaudio se volume to " + to_wstring(soundSize);
                        mciSendString(vos.c_str(), NULL, 0, NULL);
                        mciSendString(L"play se from 0", NULL, 0, NULL);
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
        psize = root["psize"].asInt();
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
            p[i].exp = root["player"]["exp"][i].asInt();
            p[i].stone_id = root["player"]["stone_id"][i].asInt();
            p[i].arms_id_1 = root["player"]["arms_id_1"][i].asInt();
            p[i].arms_id_2 = root["player"]["arms_id_2"][i].asInt();
            p[i].state = root["player"]["state"][i].asInt();
            p[i].strExp = root["player"]["strExp"][i].asInt();
            p[i].dexExp = root["player"]["dexExp"][i].asInt();
            p[i].conExp = root["player"]["conExp"][i].asInt();
            p[i].wisExp = root["player"]["wisExp"][i].asInt();
            p[i].chaExp = root["player"]["chaExp"][i].asInt();
            p[i].intExp = root["player"]["intExp"][i].asInt();
            p[i].t_id = root["player"]["t_id"][i].asInt();
            for (j = 0; j < sk_id; j++) {
                string sj="s_check_"+to_string(i);
                p[i].s_check[j] = root["player"]["skill"][sj][j].asInt();
            }
            for (j = 0; j < 3; j++) {
                string sj = "size" + to_string(i);
                p[i].box_size[j] = root["player"]["t_Box"][sj][j].asInt();
                sj = "id" + to_string(i);
                p[i].box_id[j] = root["player"]["t_Box"][sj][j].asInt();
            }
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
        for (i = 0; i < ex_id; i++) {
            EX[i].state = root["exit"][i].asBool();
        }
        for (i = 0; i < m_fid; i++) {
            m_f[i].check = root["m_flag"][i].asInt();
        }
        for (i = 0; i < t_Eid; i++) {
            t_E[i].number = root["t_equip"]["number"][i].asInt();
        }
        P_id = root["P_id"].asInt();
        three = root["time"].asInt();
        m_id = root["m_id"].asInt();
        voiceSize = root["voiceSize"].asInt();
        soundSize = root["soundSize"].asInt();
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
void event(flag *f,player *p,npc *n,m_flag *m_f,Map *m,BOX *Box,Exit *EX,enemy *e,e_npc *e_n,int &m_id,int &b_mid,int b_id) {
    i = 0;
    IMAGE p1,p2,p3;
    if (f[0].check == 0) {
        string filename;
        filename = "./Game/story/event" + to_string(0) + string(".json");
        const char* path = filename.c_str();
        readeventjson(p, n,f,m,Box,path,m_id,b_id);
        f[0].check = 1;
    }
    else if (f[1].check ==0) {
        string filename;
        filename = "./Game/story/event" + to_string(1) + string(".json");
        const char* path = filename.c_str();
        readeventjson(p, n, f, m, Box, path, m_id, b_id);
        f[1].check = 1;
    }
    else if (f[2].check == 0) {
        if (m_id == 2) {
            string filename;
            filename = "./Game/story/event" + to_string(2) + string(".json");
            const char* path = filename.c_str();
            readeventjson(p, n, f, m, Box, path, m_id, b_id);
            f[2].check = 1;
        }
    }
     if (f[3].check == 0) {
        if (m_id == 2) {
            if (p[0].x == 6 && p[0].y == 14) {
                string filename;
                filename = "./Game/story/event" + to_string(3) + string(".json");
                const char* path = filename.c_str();
                readeventjson(p, n, f, m, Box, path, m_id, b_id);
                f[3].check = 1;
            }
        }
    }
     if (f[4].check == 0) {
        if (m_id == 2&&m_f[1].check==0) {
            if (p[0].x == 19 && p[0].y == 19) {
                string filename;
                filename = "./Game/story/event" + to_string(4) + string(".json");
                const char* path = filename.c_str();
                readeventjson(p, n, f, m, Box, path, m_id, b_id);
                f[4].check = 1;
            }
        }
    }
     if (f[5].check == 1) {
         if (m_id == 4) {
             b_mid = 1;
             f[5].check = 1;
         }
     }
     /*測試戰鬥*/
     if (f[6].check == 1) {
         p[0].s_check[0] = 1;
         f[6].check = 0;
     }
     if (f[7].check == 1) {
         p[0].s_check[1] = 1;
         f[7].check = 0;
     }
     if (f[8].check == 1) {
         p[0].s_check[2] = 1;
         f[8].check = 0;
     }
     if (f[9].check == 0) {
         if (m_id == 4) {
             string filename;
             filename = "./Game/story/event" + to_string(6) + string(".json");
             const char* path = filename.c_str();
             readeventjson(p, n, f, m, Box, path, m_id, b_id);
             f[9].check = 1;
         }
     }
     if (f[10].check == 0) {
         if (m_f[2].check == 1) {
             ss[27][9] = 0;
             n[5].avatar = -1;
             f[10].check = 1;
         }
     }
     if (f[11].check == 0) {
         if (p[0].x <= 27 && p[0].x >= 26 &&p[0].y==0&&m_f[2].check==0) {
             string filename;
             filename = "./Game/story/event" + to_string(7) + string(".json");
             const char* path = filename.c_str();
             readeventjson(p, n, f, m, Box, path, m_id, b_id);
             f[11].check = 1;
         }
     }
     if (f[12].check == 1) {
         if (m_id == 5) {
             string filename;
             filename = "./Game/story/event" + to_string(5) + string(".json");
             const char* path = filename.c_str();
             readeventjson(p, n, f, m, Box, path, m_id, b_id);             
             filename = "./Game/story/event" + to_string(8) + string(".json");
             path = filename.c_str();
             readeventjson(p, n, f, m, Box, path, m_id, b_id);
             f[12].check = 1;
         }
     }
     if (f[13].check == 0) {
         if (m_id == 5) {
             f[13].check = 1;
             b_mid = 2;
         }
     }
     if (m_f[1].check == 1) {
         EX[2].state = true;
     }
     if (m_f[2].check == 1) {
         EX[6].state = true;
     }
}
void n_put(npc* n,Map *M,player *p,int m_id,string g,int z) {
    IMAGE t;
    wstring m;
    string sum = "";
    int uk[5] = {-1,8,16,0,8};
    for (int k = 0; k < M[m_id].npcid.size(); k++) {
        if (M[m_id].npcid[k] == '-') {
            int a = stoi(sum);
            m = std::to_wstring(a);
            m = L"./Game/picture/npc" + m + L".png";
            LPCTSTR path = m.c_str();
            loadimage(&t, path, 0, 0, false);

                if (g == "w") {
                    if (p[0].pose == 1) {
                        transparentimage(NULL, n[a].x * 48 - M[m_id].ox, n[a].y * 48 - 16 - M[m_id].oy- (-48 + 16 * (z + 1)) * (p[0].y + 10 <= M[m_id].y) * (p[0].y > 10), &t, 0xFF55FF, 64 + uk[n[a].avatar], 64*(n[a].avatar-1), 48, 64);
                    }
                    else if (p[0].pose == 2) {
                        transparentimage(NULL, n[a].x * 48 - M[m_id].ox - (48 - 16 * (z + 1)) * (p[0].x >= 13) * (p[0].x + 14 < M[m_id].x), n[a].y * 48 - 16 - M[m_id].oy , &t, 0xFF55FF, 64 + uk[n[a].avatar], 64 * (n[a].avatar - 1), 48, 64);
                    }
                    else if (p[0].pose == 3) {
                        transparentimage(NULL, n[a].x * 48 - M[m_id].ox - (-48 + 16 * (z + 1)) * (p[0].x > 13) * (p[0].x + 14 <= M[m_id].x), n[a].y * 48 - 16 - M[m_id].oy , &t, 0xFF55FF, 64 + uk[n[a].avatar], 64 * (n[a].avatar - 1), 48, 64);
                    }
                    else if (p[0].pose == 4) {
                        transparentimage(NULL, n[a].x * 48 - M[m_id].ox, n[a].y * 48 - 16 - M[m_id].oy -(48 - 16 * (z + 1)) * (p[0].y >= 10) * (p[0].y + 10 < M[m_id].y), &t, 0xFF55FF, 64 + uk[n[a].avatar], 64 * (n[a].avatar - 1), 48, 64);
                    }
                }
                else {
                    transparentimage(NULL, n[a].x * 48 - M[m_id].ox, n[a].y * 48 - 16 - M[m_id].oy, &t, 0xFF55FF, 64 + uk[n[a].avatar], 64 * (n[a].avatar - 1), 48, 64);
                }

            k++;
            sum = "";
        }
        sum += M[m_id].npcid[k];
}
}
void m_put(player* p,Map *m,int m_id) {
    IMAGE p1;
    loadimage(&p1, L"./Game/picture/p0.png", 0, 0, false);
    if (p[0].pose == 1) {
            transparentimage(NULL, p[0].x * 48-m[m_id].ox, p[0].y * 48 - 16 - m[m_id].oy, &p1, 0xFF55FF, 64+8, 0, 48, 64);
    }
    else if (p[0].pose == 2) {
        transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48-16 - m[m_id].oy, &p1, 0xFF55FF, 64+16, 64, 48, 64);
    }
    else if (p[0].pose == 3) {
        transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48-16 - m[m_id].oy, &p1, 0xFF55FF, 64, 128,48, 64);
    }
    else if (p[0].pose == 4) {
        transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48-16 - m[m_id].oy, &p1, 0xFF55FF, 64+8, 192, 48, 64);
    }
}
void box_put(BOX* box,int b_id) {
    IMAGE  p1,p2;
    loadimage(&p1, L"./Game/picture/box.png", 0.0, false);
    loadimage(&p2, L"./Game/picture/box1.png", 0.0, false);
    for (i = 0; i < b_id; i++) {
        if (box[i].check == true) {
            if (box[i].state == true) {
                transparentimage(NULL, 48*box[i].x, 48*box[i].y, &p1, 0xFF55FF);
            }
            else if (box[i].state == false) {
                transparentimage(NULL, 48 * box[i].x, 48 * box[i].y, &p2, 0xFF55FF);
            }
        }
    }
}
void m_map(player* p, Map* m, BOX* Box, int m_id, int b_id, npc* n, int z, string g) {
    IMAGE p1;
    if (g == "e") {

    }
    else {
    m[m_id].ox = (p[0].x > 13) * (p[0].x - 13) * 48 - (14 - (m[m_id].x - p[0].x)) * (p[0].x + 14 > m[m_id].x) * 48;
    m[m_id].oy = (p[0].y > 10) * (p[0].y - 10) * 48 - (10 - (m[m_id].y - p[0].y)) * (p[0].y + 10 > m[m_id].y) * 48;
    }
    if (g == "w") {

            if (p[0].pose == 1) {
                putimage(0, 0, 1296, 960, &mapP, m[m_id].ox, m[m_id].oy + ( -48+16 * (z + 1)) * (p[0].y + 10 <= m[m_id].y) * (p[0].y >10));
            }
            else if (p[0].pose == 2) {
                putimage(0, 0, 1296, 960, &mapP, m[m_id].ox + (48 - 16 * (z + 1)) * (p[0].x >= 13) * (p[0].x + 14 < m[m_id].x), m[m_id].oy);
            }
            else if (p[0].pose == 3) {
                putimage(0, 0, 1296, 960, &mapP, m[m_id].ox + (-48 + 16 * (z + 1)) * (p[0].x > 13) * (p[0].x + 14 <= m[m_id].x), m[m_id].oy);
            }
            else if (p[0].pose == 4) {
                putimage(0, 0, 1296, 960, &mapP, m[m_id].ox, m[m_id].oy + (48 - 16 * (z + 1)) * (p[0].y >=10) * (p[0].y + 10 < m[m_id].y));
            }
    }
    else {
        putimage(0, 0, 1296, 960, &mapP, m[m_id].ox, m[m_id].oy);
    }
    box_put(Box, b_id);
    for (int I = 0; I < m[m_id].nsize; I++) {
        n_put(n, m,p,m_id,g,z);
    }
}
string  m_act(player *p,Map *m,npc *n,int m_id) {
    char w;
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        return"esc";
    }
    else if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        return "t";
    }
                if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
                    if (p[0].y < m[m_id].y - 1 && ss[p[0].x][p[0].y + 1] == 0) {
                        p[0].y++;
                            p[0].pose = 1;
                            return "w";
                    }
                    else {
                        p[0].pose = -1; return"w";
                    }
                }
                else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
                    if (p[0].x > 0 && ss[p[0].x - 1][p[0].y] == 0) {
                        p[0].x--;
                        p[0].pose = 2;
                        return"w";
                    }
                    else {
                        p[0].pose = -2; return"w";
                    }
                }
                else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
                    if (p[0].x < m[m_id].x - 1 && ss[p[0].x + 1][p[0].y] == 0) {
                        p[0].x++;
                        p[0].pose = 3;
                        return"w";
                    }
                    else {
                        p[0].pose = -3; return"w";
                    }
                }
                else if (GetAsyncKeyState(VK_UP) & 0x8000) {
                    if (p[0].y > 0 && ss[p[0].x][p[0].y - 1] == 0) {
                        p[0].y--;
                        p[0].pose = 4;
                        return"w";
                    }
                    else {
                        p[0].pose = -4; return"w";
                    }
                }
                

        return"";
}
void m_set(Map* m, npc* n, player* p,e_npc *e_n ,BOX *Box,int m_id,int b_id) {
    mciSendString(L"close bgm", NULL, 0, NULL);
    int k, w, x, y;
    string number="",box="";   
    string filename;
        filename= "./Game/map/map"+to_string(m_id) + string(".json");
    const char* path = filename.c_str();
    readmapjson(m,m_id,path);
    wstring mm,vos;
    LPCTSTR Path;
    if (m[m_id].bgm != -1) {
        mm = L"open ./Game/Sound/bgm/bgm" + std::to_wstring(m[m_id].bgm) + L".mp3 alias bgm";
        Path = mm.c_str();
        mciSendString(Path, NULL, 0, NULL);
        mciSendString(L"play bgm repeat", NULL, 0, NULL);
        vos = L"setaudio bgm volume to " + to_wstring(voiceSize);
        mciSendString(vos.c_str(), NULL, 0, NULL);
    }
    mm = std::to_wstring(m_id);
    mm = L"./Game/map/map" + mm + L".png";
    LPCTSTR patH = mm.c_str();
    IMAGE boxx; 
    loadimage(&boxx, patH, 0, 0, false);
    mapP = boxx;
    i = 0;
    for (i = 0; i < b_id; i++) {
        Box[i].check = false;
    }
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
            else if (m[m_id].e_set[w] == 'k') {
                w++;
                while (1) {

                        if (m[m_id].e_set[w] >= '0' && m[m_id].e_set[w] <= '9') {
                            number += m[m_id].e_set[w];
                            w++;
                        }
                        else if (w == m[m_id].e_set.size()) {
                            e_n[i].move = 0; e_n[i].pose = 1; e_n[i].type = 1; e_n[i].see = 0; e_n[i].live = 1; ss[e_n[i].x][e_n[i].y] = 1; e_n[i].kind = stoi(number); i++;
                            w--;
                            break;
                        }
                        else {
                            e_n[i].move = 0; e_n[i].pose = 1; e_n[i].type = 1; e_n[i].see = 0; e_n[i].live = 1; ss[e_n[i].x][e_n[i].y] = 1; e_n[i].kind = stoi(number); i++;
                            w--;
                            break;
                        }
                }
            }
        }
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
    /*敵人參數設定*/
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
    box = "";
    k = 0;
    for (i = 0; i < m[m_id].box_set.size(); i++) {
        if (m[m_id].box_set[i] == 'n') {
            k= stoi(box);
            ss[Box[k].x][Box[k].y] = 1;
            Box[k].check = true;
            box = "";
        }
        else {
            box += m[m_id].box_set[i];
        }
    }
    int I = 0;
    for (i = 0; i < 3; i++) {
        if (p[i].state == 1) {
            m[m_id].team_state[I] = i;
            I++;
        }
    }
    m[m_id].psize = I;
}
void m_check(player* p, Map* m, Exit* ex,BOX *Box, int& m_id,int b_id,npc *n,e_npc *e_n) {
    string s = "";
    int uuu,u,k;

        for (j = 0; j < m[m_id].exit_set.size(); j++) {
            if (m[m_id].exit_set[j] == '_') {
                uuu = stoi(s);
                if (ex[uuu].state == true) {
                    for (u = ex[uuu].x; u <= ex[uuu].cx;u++) {
                        for (k = ex[uuu].y; k <= ex[uuu].cy; k++) {
                          if (p[0].x == u && p[0].y == k) {
                            m_id = ex[uuu].Mid;
                            p[0].x = ex[uuu].gx;
                            p[0].y = ex[uuu].gy;
                            m_set(m, n, p, e_n,Box, m_id,b_id);
                          }   
                        }
                    }

                }
                s = "";
            }
            else {
                s += m[m_id].exit_set[j];
            }
        }
}
void m_walk(Map *m,player *p,BOX *Box,int &m_id,int b_id,npc *n,string &g) {
    if (p[0].pose < 0) {
        p[0].pose *= -1;
        g = "";
        m_map(p, m, Box, m_id, b_id, n, Z, g);
        m_put(p,m,m_id);
        
        return;
    }
    IMAGE p1;
    loadimage(&p1, L"./Game/picture/p0.png", 0, 0, false);
    if (p[0].pose == 1) {
        if (Z == 0) {
            
            transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y*48 +(- 48 + 16 * (Z + 1) )* ((p[0].y + 10 >m[m_id].y) || (p[0].y <= 10)) - 16 - m[m_id].oy, &p1, 0xFF55FF, 8, 0, 48, 64);
        }
        else if (Z == 1) {
            transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48 + (-48 + 16 * (Z + 1)) *( (p[0].y + 10 > m[m_id].y)|| (p[0].y <= 10) )- 16 - m[m_id].oy, &p1, 0xFF55FF, 128 + 8, 0, 48, 64);
        }
        else if (Z == 2) {
            transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48 + (-48 + 16 * (Z + 1)) * ((p[0].y + 10 > m[m_id].y) || (p[0].y <= 10)) - 16 - m[m_id].oy, &p1, 0xFF55FF, 64 + 8, 0, 48, 64);
            ss[p[0].x][p[0].y - 1] = 0;
            ss[p[0].x][p[0].y] = 1;
        }
        }
    else if (p[0].pose == 2) {

            if (Z == 0) {
                transparentimage(NULL, p[0].x*48 +(48 - 16 * (Z + 1) )* ((p[0].x < 13) || (p[0].x + 14 >= m[m_id].x)) - m[m_id].ox, p[0].y * 48 - 16 - m[m_id].oy, &p1, 0xFF55FF, 16, 64, 48, 64);
            }
            else if (Z == 1) {
                transparentimage(NULL, p[0].x * 48 + (48 - 16 * (Z + 1)) * ((p[0].x < 13) || (p[0].x + 14 >= m[m_id].x)) - m[m_id].ox, p[0].y * 48 - 16 - m[m_id].oy, &p1, 0xFF55FF, 128 + 16, 64, 48, 64);
            }
            else if (Z == 2) {
                transparentimage(NULL, p[0].x * 48 + (48 - 16 * (Z + 1)) * ((p[0].x < 13) ||(p[0].x + 14 >= m[m_id].x)) - m[m_id].ox, p[0].y * 48 - 16 - m[m_id].oy, &p1, 0xFF55FF, 64 + 16, 64, 48, 64);
                ss[p[0].x + 1][p[0].y] = 0;
                ss[p[0].x][p[0].y] = 1;
            }

    }
    else if (p[0].pose == 3) {

            if (Z == 0) {
                transparentimage(NULL, p[0].x * 48 + (-48 + 16 * (Z + 1)) * ((p[0].x <=13) || (p[0].x + 14 > m[m_id].x)) - m[m_id].ox, p[0].y * 48 - 16 - m[m_id].oy, &p1, 0xFF55FF, 0, 128, 48, 64);
            }
            else if (Z == 1) {
                transparentimage(NULL, p[0].x * 48 + (-48 + 16 * (Z + 1)) * ((p[0].x <= 13) || (p[0].x + 14 > m[m_id].x)) - m[m_id].ox, p[0].y * 48 - 16 - m[m_id].oy, &p1, 0xFF55FF, 128 + 0, 128, 48, 64);
            }
            else if (Z == 2) {
                transparentimage(NULL, p[0].x * 48 + (-48 + 16 * (Z + 1)) * ((p[0].x <= 13) || (p[0].x + 14 > m[m_id].x)) - m[m_id].ox, p[0].y * 48 - 16 - m[m_id].oy, &p1, 0xFF55FF, 64 + 0, 128, 48, 64);
                ss[p[0].x - 1][p[0].y] = 0;
                ss[p[0].x][p[0].y] = 1;
            }

        }
    else if (p[0].pose == 4) {
            if (Z == 0) {
                transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48 + (48 - 16 * (Z + 1)) * ((p[0].y + 10 >= m[m_id].y) || (p[0].y < 10)) - 16 - m[m_id].oy, &p1, 0xFF55FF, 8, 192, 48, 64);
            }
            else if (Z == 1) {
                transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48 + (48 - 16 * (Z + 1)) * ((p[0].y + 10 >= m[m_id].y) || (p[0].y < 10)) - 16 - m[m_id].oy, &p1, 0xFF55FF, 128 + 8, 192, 48, 64);
            }
            else if (Z == 2) {
                transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48 + (48 - 16 * (Z + 1)) * ((p[0].y + 10 >=m[m_id].y) || (p[0].y < 10)) - 16 - m[m_id].oy, &p1, 0xFF55FF, 64 + 8, 192, 48, 64);
                ss[p[0].x][p[0].y + 1] = 0;
                ss[p[0].x][p[0].y] = 1;
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
char menu_item(player *p,item *it,arms *ar,t_equip *t_E,int i_id,int ar_id,int st_id,int t_Eid) {
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    ExMessage m;
    IMAGE p1,p2,p3;
    RECT r = { 420, 90, 1250, 180 };
    int k = 0,J,chose=-1,pchose=0,mc;
    loadimage(&p1, L"./Game/picture/item_back.png", 0, 0, false);
    loadimage(&p3, L"./Game/picture/item_block.png", 0, 0, false);       
    mc = -1;
    while (1) {
        BeginBatchDraw();
        putimage(400, 0, &p1);
        m = getmessage(EM_MOUSE );
        j = 0;
        if (m.rbutton) {
            return NULL;
        }
        else if(m.lbutton) {
            if (m.x>400&&m.x<685&&m.y>0&&m.y<75) {
                pchose = 0;
                mc = -1;
            }
            else if(m.x > 700 && m.x < 990 && m.y>0 && m.y < 75) {
                pchose = 1;
                mc = -1;
            }
            else if(m.x > 1000 && m.x < 1280 && m.y>0 && m.y < 75) {
                pchose = 2;
                mc = -1;
            }
        }
            if (pchose == 0) {
                for (i = 0; i < i_id; i++) {
                  if (it[i].number != 0&&it[i].type=='b') {
                                if (chose == j) {
                                    return(it[i].type);
                                }
                                J = j / 2;
                                wstring mm;
                                mm = std::to_wstring(i);
                                mm = L"./Game/picture/item" + mm + L".png";
                                LPCTSTR path = mm.c_str();
                                loadimage(&p2, path, 0, 0, false);
                                if (j % 2 == 0) {
                                    transparentimage(NULL, 420, 200 + J * 40, &p2, 0xFF55FF);
                                    LPCTSTR path1 = it[i].Name.c_str();
                                    outtextxy(445, 200 + J * 40, path1);
                                    outtextxy(780, 200 + J * 40, _T(":"));
                                    mm = std::to_wstring(it[i].number);
                                    path1 = mm.c_str();
                                    outtextxy(790, 200 + J * 40, path1);
                                    j++;

                                }
                                else if (j % 2 == 1) {
                                    transparentimage(NULL, 834, 200 + J * 40, &p2, 0xFF55FF);
                                    LPCTSTR path1 = it[i].Name.c_str();
                                    outtextxy(859, 200 + J * 40, path1);
                                    outtextxy(1194, 200 + J * 40, _T(":"));
                                    mm = std::to_wstring(it[i].number);
                                    path1 = mm.c_str();
                                    outtextxy(1204, 200 + J * 40, path1);
                                    j++;
                                }
                                if (mc == j) {
                                    LPCTSTR path1 = it[i].story.c_str();
                                    drawtext(path1, &r, DT_WORDBREAK);
                                }
                            }
                }  
                for (i = 0; i < i_id; i++) {
                    if (it[i].number != 0 && it[i].type == 'h') {
                        if (chose == j) {
                            return(it[i].type);
                        }
                        J = j / 2;
                        wstring mm;
                        mm = std::to_wstring(i);
                        mm = L"./Game/picture/item" + mm + L".png";
                        LPCTSTR path = mm.c_str();
                        loadimage(&p2, path, 0, 0, false);
                        if (j % 2 == 0) {
                            transparentimage(NULL, 420, 200 + J * 40, &p2, 0xFF55FF);
                            LPCTSTR path1 = it[i].Name.c_str();
                            outtextxy(445, 200 + J * 40, path1);
                            outtextxy(780, 200 + J * 40, _T(":"));
                            mm = std::to_wstring(it[i].number);
                            path1 = mm.c_str();
                            outtextxy(790, 200 + J * 40, path1);
                            j++;

                        }
                        else if (j % 2 == 1) {
                            transparentimage(NULL, 834, 200 + J * 40, &p2, 0xFF55FF);
                            LPCTSTR path1 = it[i].Name.c_str();
                            outtextxy(859, 200 + J * 40, path1);
                            outtextxy(1194, 200 + J * 40, _T(":"));
                            mm = std::to_wstring(it[i].number);
                            path1 = mm.c_str();
                            outtextxy(1204, 200 + J * 40, path1);
                            j++;
                        }
                        if (mc == j) {
                            LPCTSTR path1 = it[i].story.c_str();
                            drawtext(path1, &r, DT_WORDBREAK);
                        }
                    }
                }
            } 
            else if (pchose == 1) {       
                for (i = 0; i < ar_id; i++) {
                     if (ar[i].number != 0 ) {
                        if (chose == j) {
                            return NULL;
                        }
                        J = j / 2;
                        wstring mm;
                        mm = std::to_wstring(i);
                        mm = L"./Game/picture/arms" + mm + L".png";
                        LPCTSTR path = mm.c_str();
                        loadimage(&p2, path, 0, 0, false);
                        if (j % 2 == 0) {
                            transparentimage(NULL, 420, 200 + J * 40, &p2, 0xFF55FF);
                            LPCTSTR path1 = ar[i].name.c_str();
                            outtextxy(445, 200 + J * 40, path1);
                            outtextxy(780, 200 + J * 40, _T(":"));
                            mm = std::to_wstring(ar[i].number);
                            path1 = mm.c_str();
                            outtextxy(790, 200 + J * 40, path1);
                            j++;

                        }
                        else if (j % 2 == 1) {
                            transparentimage(NULL, 834, 200 + J * 40, &p2, 0xFF55FF);
                            LPCTSTR path1 = ar[i].name.c_str();
                            outtextxy(859, 200 + J * 40, path1);
                            outtextxy(1194, 200 + J * 40, _T(":"));
                            mm = std::to_wstring(ar[i].number);
                            path1 = mm.c_str();
                            outtextxy(1204, 200 + J * 40, path1);
                            j++;
                        }
                        if (mc == j) {
                            LPCTSTR path1 = ar[i].story.c_str();
                            drawtext(path1, &r, DT_WORDBREAK);
                        }
                    }
                }              
                for (i = 0; i < t_Eid; i++) {
                    if (t_E[i].number!=0) {
                        if (chose == j) {
                            return NULL;
                        }
                        J = j / 2;
                        wstring mm;
                        mm = std::to_wstring(i);
                        mm = L"./Game/picture/te" + mm + L".png";
                        LPCTSTR path = mm.c_str();
                        loadimage(&p2, path, 0, 0, false);
                        if (j % 2 == 0) {
                            transparentimage(NULL, 420, 200 + J * 40, &p2, 0xFF55FF);
                            LPCTSTR path1 = t_E[i].name.c_str();
                            outtextxy(445, 200 + J * 40, path1);
                            outtextxy(780, 200 + J * 40, _T(":"));
                            mm = std::to_wstring(t_E[i].number);
                            path1 = mm.c_str();
                            outtextxy(790, 200 + J * 40, path1);
                            j++;

                        }
                        else if (j % 2 == 1) {
                            transparentimage(NULL, 834, 200 + J * 40, &p2, 0xFF55FF);
                            LPCTSTR path1 = t_E[i].name.c_str();
                            outtextxy(859, 200 + J * 40, path1);
                            outtextxy(1194, 200 + J * 40, _T(":"));
                            mm = std::to_wstring(t_E[i].number);
                            path1 = mm.c_str();
                            outtextxy(1204, 200 + J * 40, path1);
                            j++;
                        }
                        if (mc == j) {
                            LPCTSTR path1 = t_E[i].story.c_str();
                            drawtext(path1, &r, DT_WORDBREAK);
                        }
                    }
                }
            }
            else if (pchose == 2) {
                for (i = 0; i < i_id; i++) {
                  if (it[i].number != 0 && it[i].type == 'm') {
                    if (chose == j) {
                        return(it[i].type);
                    }
                    J = j / 2;
                    wstring mm;
                    mm = std::to_wstring(i);
                    mm = L"./Game/picture/item" + mm + L".png";
                    LPCTSTR path = mm.c_str();
                    loadimage(&p2, path, 0, 0, false);
                    if (j % 2 == 0) {
                        transparentimage(NULL, 420, 200 + J * 40, &p2, 0xFF55FF);
                        LPCTSTR path1 = it[i].Name.c_str();
                        outtextxy(445, 200 + J * 40, path1);
                        outtextxy(780, 200 + J * 40, _T(":"));
                        mm = std::to_wstring(it[i].number);
                        path1 = mm.c_str();
                        outtextxy(790, 200 + J * 40, path1);
                        j++;

                    }
                    else if (j % 2 == 1) {
                        transparentimage(NULL, 834, 200 + J * 40, &p2, 0xFF55FF);
                        LPCTSTR path1 = it[i].Name.c_str();
                        outtextxy(859, 200 + J * 40, path1);
                        outtextxy(1194, 200 + J * 40, _T(":"));
                        mm = std::to_wstring(it[i].number);
                        path1 = mm.c_str();
                        outtextxy(1204, 200 + J * 40, path1);
                        j++;
                    }
                    if (mc == j) {
                        LPCTSTR path1 = it[i].story.c_str();
                        drawtext(path1, &r, DT_WORDBREAK);
                    }
                  }              
                }
            
            }        
            for (i = 0; i < j; i++) {
                if (m.x < 824 + (i % 2) * 414 && m.x>410 + (i % 2) * 414 && m.y > 197 + (i / 2) * 40 && m.y < 232 + (i / 2) * 40) {
                    mc = i+1;
                    transparentimage(NULL, 410 + (i % 2) * 414, 197 + (i / 2) * 40, &p3);
                    if (m.lbutton) {
                        chose = i;
                    }
                }
            }
        EndBatchDraw();
    }
}
void menu_player(player *p,Map *M,int m_id) {
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    IMAGE p1,p2,p3,p4;
    loadimage(&p1, L"./Game/picture/menu_back.png", 0, 0, false);
    loadimage(&p3, L"./Game/picture/larrow.png", 0, 0, false);
    loadimage(&p4, L"./Game/picture/rarrow.png", 0, 0, false);
    int I = 0;
    int q = 0;
    while (1) {
        settextstyle(30, 0, _T("Taipei Sans TC Beta"));
        Sleep(10);
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
        mm = std::to_wstring(p[I].exp);
        path1 = mm.c_str();
        outtextxy(700, 65, _T("Exp"));
        outtextxy(800, 65, path1);
        outtextxy(850, 65, _T("/"));
        mm = std::to_wstring(int(p[I].lv * 100 + pow(p[I].lv, 3)));
        path1 = mm.c_str();
        outtextxy(860, 65, path1);
        outtextxy(700, 115, _T("Hp"));
        outtextxy(800, 115, to_wstring(p[I].hp).c_str());
        outtextxy(850, 115, _T("/"));
        outtextxy(860, 115, to_wstring(p[I].mhp).c_str());
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
        wstring vos;
        m = getmessage(EM_MOUSE);
        if(m.x>415&&m.x<463&&m.y>450&&m.y<498) {
            if (m.lbutton&&q>0) {
                mciSendString(L"open ./Game/Sound/SE/page-flip-01a.mp3 alias se", NULL, 0, NULL);
                vos = L"setaudio se volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play se from 0", NULL, 0, NULL);
                q--;
                I = M[m_id].team_state[q];
            }
        }
        else if (m.x>1230&&m.x<1278&&m.y>450&&m.y<498) {
            if (m.lbutton&&q<M[m_id].psize-1) {
                mciSendString(L"open ./Game/Sound/SE/page-flip-01a.mp3 alias se", NULL, 0, NULL);
                vos = L"setaudio se volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play se from 0", NULL, 0, NULL);
                q++;
                I = M[m_id].team_state[q];
            }
        }
        if (m.rbutton) {
            return;
        }
    }
}
void menu_equip(player* p, arms* ar,armor *Ar,stone *st,Map *M,t_equip *t_E,item *it,int ar_id,int Ar_id,int st_id,int m_id,int t_Eid,int i_id) {
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    IMAGE p1, p2, p3, p4,p5,p6,p7,p8,p9,p10,p11,p12;
    int I = 0,d=-1,q=0,box[50],ut;
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
        mm = std::to_wstring(p[I].arms_id_1);
        mm = L"./Game/picture/arms" + mm + L".png";
        path1 = mm.c_str();
        loadimage(&p6, path1, 0, 0, false);
        transparentimage(NULL, 950, 215, &p6,0xFF55FF);
        mm = ar[p[I].arms_id_1].name;
        if (ar[p[I].arms_id_1].type[0] == 'r') {           
            loadimage(&p11, L"./Game/picture/tbox.png", 0, 0, false);
            putimage(1215, 210, &p11); 
            if (p[I].b_id_1 != -1) {
                mm = L"./Game/picture/item" + to_wstring(p[I].b_id_1) + L".png";
                loadimage(&p12, mm.c_str(), 0, 0, false);
                transparentimage(NULL, 1220, 215, &p12, 0xFF55FF);
            }
            mm = ar[p[I].arms_id_1].name+L"(" + to_wstring(p[I].arms_b_1) + L"/" + to_wstring(ar[p[I].arms_id_1].mbullet) + L")";
        }
        outtextxy(1000, 213,mm.c_str());
        outtextxy(865, 250, _T("武器"));
        if (p[I].arms_id_2 != -1) {
        mm = std::to_wstring(p[I].arms_id_2);
        mm = L"./Game/picture/arms" + mm + L".png";
        path1 = mm.c_str();
        loadimage(&p9, path1, 0, 0, false);
        transparentimage(NULL, 950, 255, &p9, 0xFF55FF);
        mm = ar[p[I].arms_id_2].name;
        if (ar[p[I].arms_id_2].type[0] == 'r') {
          
            loadimage(&p11, L"./Game/picture/tbox.png", 0, 0, false);
            putimage(1215, 250, &p11); 
            if (p[I].b_id_2 != -1) {
                mm = L"./Game/picture/item" + to_wstring(p[I].b_id_2) + L".png";
                loadimage(&p12, mm.c_str(), 0, 0, false);
                transparentimage(NULL,1220, 255, &p12, 0xFF55FF);
            }
            mm = ar[p[I].arms_id_2].name+ L"(" + to_wstring(p[I].arms_b_2) + L"/" + to_wstring(ar[p[I].arms_id_2].mbullet) + L")";
        }
        outtextxy(1000, 253, mm.c_str());  
        }
       
        outtextxy(865, 290, _T("防具"));
        mm = std::to_wstring(p[I].armor_id);
        mm = L"./Game/picture/armor" + mm + L".png";
        path1 = mm.c_str();
        loadimage(&p9, path1, 0, 0, false);
        transparentimage(NULL, 950, 295, &p9, 0xFF55FF);
        path1 = Ar[p[I].armor_id].name.c_str();
        outtextxy(1000, 293, path1);
        outtextxy(865, 330, _T("核心"));
        mm = std::to_wstring(p[I].stone_id);
        mm = L"./Game/picture/stone" + mm + L".png";
        path1 = mm.c_str();
        loadimage(&p9, path1, 0, 0, false);
        transparentimage(NULL, 950, 335, &p9, 0xFF55FF);
        path1 = st[p[I].stone_id].name.c_str();
        outtextxy(1000, 333, path1);
        outtextxy(865, 370, _T("配件"));
        if (p[I].t_id != -1) {
        mm = L"./Game/picture/te" + to_wstring(p[I].t_id) + L".png";
        loadimage(&p10, mm.c_str(), 0, 0, false);
        transparentimage(NULL, 950, 375, &p10, 0xFF55FF);
        outtextxy(1000, 373, t_E[p[I].t_id].name.c_str());
        mm = L"./Game/picture/tbox.png";
        loadimage(&p11, mm.c_str(), 0, 0, false);
        settextstyle(15, 0, _T("Taipei Sans TC Beta"));
        for (i = 0; i < t_E[p[I].t_id].size; i++) {
            putimage(950+i*30, 415, &p11);
            if (p[I].box_id[i] != -1) {
                if (p[I].box_size[i] != 0) {
                    outtextxy(960+i*30, 403, to_wstring(p[I].box_size[i]).c_str());
                    mm = L"./Game/picture/item" + to_wstring(p[I].box_id[i]) + L".png";
                    loadimage(&p10, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 955+i*30, 420, &p10, 0xFF55FF);
                }           
            }
        }        
        settextstyle(30, 0, _T("Taipei Sans TC Beta"));
        }
        ExMessage m;
        m = getmessage(EM_MOUSE);
        for (i = 0; i < 5; i++) {
            if (m.x >860 && m.x< 1210&& m.y>208+i*40 && m.y <243+i*40 ) {                    
                transparentimage(NULL, 860, 208 + i * 40, &p5);
                if (i==0) {

                    path1 = ar[p[I].arms_id_1].story.c_str();
                }
                else if (i == 1) {
                    if (p[I].arms_id_2!=-1) {
                        path1 = ar[p[I].arms_id_2].story.c_str();
                    }
                    else {
                        path1 = L"";
                    }                
                }
                else if (i == 2) {
                    path1 = Ar[p[I].armor_id].story.c_str();
                }
                else if (i == 3) {
                    path1 = st[p[I].stone_id].story.c_str();
                }
                else if (i == 4) {
                    path1 = t_E[p[I].t_id].story.c_str();
                }
                drawtext(path1,&r, DT_WORDBREAK);
                if (m.lbutton) {
                    d = i+1;
                }
            }
        }
        if (p[I].t_id != -1) {
            for (i = 0; i < t_E[p[I].t_id].size; i++) {
                if (m.x >= 955 + i * 30 && m.x <= 985 + i * 30 && m.y >= 420 && m.y <= 450) {
                    if (m.lbutton) {
                       d = 6; q = i;
                    }
                 
                }
            }
        }
        if (ar[p[I].arms_id_1].type[0] == 'r') {
            if (m.x >= 1215 && m.x <= 1245 && m.y >= 210 && m.y <= 240) {
                if (m.lbutton) {
                    d = 7;
                }
            }
        }
        if (ar[p[I].arms_id_2].type[0] == 'r') {
            if (m.x >= 1215 && m.x <= 1245 && m.y >= 250 && m.y <= 280) {
                if (m.lbutton) {
                    d = 8;
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
                                    ar[p[I].arms_id_1].number++;
                                    p[I].arms_b_1 = 1;
                                    if (ar[arm[i]].type[0] == 'r') {
                                        p[I].arms_b_1 = 0;
                                    }
                                    p[I].arms_id_1 = arm[i];
                                    ar[arm[i]].number--;
                                }
                            }
                       }
        }
        else if (d == 2) {
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

                if (m.x > 420 + (i % 2) * 414 && m.x < 830 + (i % 2) * 414 && m.y>570 + (i / 2) * 40 && m.y < 605 + (i / 2) * 40) {
                    transparentimage(NULL, 415 + (i % 2) * 414, 563 + (i / 2) * 40, &p8);
                    path1 = ar[arm[i]].story.c_str();
                    drawtext(path1, &r, DT_WORDBREAK);
                    if (m.lbutton) {
                        ar[p[I].arms_id_2].number++;
                        p[I].arms_b_2 = 1;
                        if (ar[arm[i]].type[0] == 'r') {
                            p[I].arms_b_2 = 0;
                        }
                        p[I].arms_id_2 = arm[i];                        
                        ar[arm[i]].number--;
                    }
                }
            }
        }
        else if (d == 4) {
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
        else if (d == 5) {
            int J = 0, j = 0;
            for (i = 0; i < t_Eid; i++) {
                if (t_E[i].number != 0) {
                sto[j] = i;
                J = j / 2;
                wstring mm;
                mm = std::to_wstring(i);
                mm = L"./Game/picture/te" + mm + L".png";
                path = mm.c_str();
                loadimage(&p7, path, 0, 0, false);
                if (j % 2 == 0) {
                    transparentimage(NULL, 420, 570 + J * 40, &p7, 0xFF55FF);
                    path1 = t_E[i].name.c_str();
                    outtextxy(445, 570 + J * 40, path1);
                    outtextxy(780, 570 + J * 40, _T(":"));
                    mm = std::to_wstring(t_E[i].number);
                    path1 = mm.c_str();
                    outtextxy(790, 570 + J * 40, path1);
                    j++;
                }
                else if (j % 2 == 1) {
                    transparentimage(NULL, 834, 570 + J * 40, &p7, 0xFF55FF);
                    path1 = t_E[i].name.c_str();
                    outtextxy(859, 570 + J * 40, path1);
                    outtextxy(1194, 570 + J * 40, _T(":"));
                    mm = std::to_wstring(t_E[i].number);
                    path1 = mm.c_str();
                    outtextxy(1204, 570 + J * 40, path1);
                    j++;
                }
                }
            }
            for (i = 0; i < j; i++) {

                if (m.x > 420 + (i % 2) * 414 && m.x < 830 + (i % 2) * 414 && m.y>570 + (i / 2) * 40 && m.y < 605 + (i / 2) * 40) {
                    transparentimage(NULL, 415 + (i % 2) * 414, 563 + (i / 2) * 40, &p8);
                    path1 = t_E[sto[i]].story.c_str();
                    drawtext(path1, &r, DT_WORDBREAK);
                    if (m.lbutton) {
                        if (I != -1) {
                           t_E[p[I].t_id].number++;
                        }                     
                        p[I].t_id = sto[i];
                        t_E[sto[i]].number--;
                    }
                }
            }
        }
        else if (d == 6) {
        int J = 0, j = 0;
        for (i = 0; i < i_id; i++) {
            if (it[i].number != 0&&it[i].type=='b') {
                sto[j] = i;
                J = j / 2;
                wstring mm;
                mm = std::to_wstring(i);
                mm = L"./Game/picture/item" + mm + L".png";
                path = mm.c_str();
                loadimage(&p7, path, 0, 0, false);
                if (j % 2 == 0) {
                    transparentimage(NULL, 420, 570 + J * 40, &p7, 0xFF55FF);
                    path1 = it[i].Name.c_str();
                    outtextxy(445, 570 + J * 40, path1);
                    outtextxy(780, 570 + J * 40, _T(":"));
                    mm = std::to_wstring(it[i].number);
                    path1 = mm.c_str();
                    outtextxy(790, 570 + J * 40, path1);
                    j++;
                }
                else if (j % 2 == 1) {
                    transparentimage(NULL, 834, 570 + J * 40, &p7, 0xFF55FF);
                    path1 = it[i].Name.c_str();
                    outtextxy(859, 570 + J * 40, path1);
                    outtextxy(1194, 570 + J * 40, _T(":"));
                    mm = std::to_wstring(it[i].number);
                    path1 = mm.c_str();
                    outtextxy(1204, 570 + J * 40, path1);
                    j++;
                }
            }
        }
        for (i = 0; i < i_id; i++) {
            if (it[i].number != 0 && it[i].type == 'h') {
                sto[j] = i;
                J = j / 2;
                wstring mm;
                mm = std::to_wstring(i);
                mm = L"./Game/picture/item" + mm + L".png";
                path = mm.c_str();
                loadimage(&p7, path, 0, 0, false);
                if (j % 2 == 0) {
                    transparentimage(NULL, 420, 570 + J * 40, &p7, 0xFF55FF);
                    path1 = it[i].Name.c_str();
                    outtextxy(445, 570 + J * 40, path1);
                    outtextxy(780, 570 + J * 40, _T(":"));
                    mm = std::to_wstring(it[i].number);
                    path1 = mm.c_str();
                    outtextxy(790, 570 + J * 40, path1);
                    j++;
                }
                else if (j % 2 == 1) {
                    transparentimage(NULL, 834, 570 + J * 40, &p7, 0xFF55FF);
                    path1 = it[i].Name.c_str();
                    outtextxy(859, 570 + J * 40, path1);
                    outtextxy(1194, 570 + J * 40, _T(":"));
                    mm = std::to_wstring(it[i].number);
                    path1 = mm.c_str();
                    outtextxy(1204, 570 + J * 40, path1);
                    j++;
                }
            }
        }
        for (i = 0; i < j; i++) {
            if (m.x > 420 + (i % 2) * 414 && m.x < 830 + (i % 2) * 414 && m.y>570 + (i / 2) * 40 && m.y < 605 + (i / 2) * 40) {
                transparentimage(NULL, 415 + (i % 2) * 414, 563 + (i / 2) * 40, &p8);
                path1 = it[sto[i]].story.c_str();
                drawtext(path1, &r, DT_WORDBREAK);
                if (m.lbutton) {
                    if (p[I].box_id[q] != -1) {
                        it[p[I].box_id[q]].number += p[I].box_size[q];
                        p[I].box_size[q] = 0;
                    }                    
                    if (p[I].t_id == 0) {
                        if (it[sto[i]].type == 'h') {
                            for (ut = 0; ut < 3&& it[sto[i]].number>0; ut++) {
                                it[sto[i]].number--;
                                p[I].box_size[q]++;
                            }
                        }
                        else {
                            it[sto[i]].number--;
                            p[I].box_size[q]++;
                        }
                    }
                    else if (p[I].t_id == 1) {
                        if (it[sto[i]].type == 'b') {
                            for (ut = 0; ut < 3 && it[sto[i]].number>0; ut++) {
                                it[sto[i]].number--;
                                p[I].box_size[q]++;
                            }
                        }
                        else {
                            it[sto[i]].number--;
                            p[I].box_size[q]++;
                        }
                    }
                    p[I].box_id[q] = sto[i];
                }
            }
        }
        }
        else if (d == 7) {
        int J = 0, j = 0;
        if (ar[p[I].arms_id_1].type[1] == 'r') {
        for (i = 0; i < i_id; i++) {
            if ((it[i].number != 0 && it[i].type == 'b')&&i==0) {
                sto[j] = i;
                J = j / 2;
                wstring mm;
                mm = std::to_wstring(i);
                mm = L"./Game/picture/item" + mm + L".png";
                path = mm.c_str();
                loadimage(&p7, path, 0, 0, false);
                if (j % 2 == 0) {
                    transparentimage(NULL, 420, 570 + J * 40, &p7, 0xFF55FF);
                    path1 = it[i].Name.c_str();
                    outtextxy(445, 570 + J * 40, path1);
                    outtextxy(780, 570 + J * 40, _T(":"));
                    mm = std::to_wstring(it[i].number);
                    path1 = mm.c_str();
                    outtextxy(790, 570 + J * 40, path1);
                    j++;
                }
                else if (j % 2 == 1) {
                    transparentimage(NULL, 834, 570 + J * 40, &p7, 0xFF55FF);
                    path1 = it[i].Name.c_str();
                    outtextxy(859, 570 + J * 40, path1);
                    outtextxy(1194, 570 + J * 40, _T(":"));
                    mm = std::to_wstring(it[i].number);
                    path1 = mm.c_str();
                    outtextxy(1204, 570 + J * 40, path1);
                    j++;
                }
            }
        }
        }
        else if (ar[p[I].arms_id_1].type[1] == 'p') {
            if ((it[i].number != 0 && it[i].type == 'b') && i == 3) {
                sto[j] = i;
                J = j / 2;
                wstring mm;
                mm = std::to_wstring(i);
                mm = L"./Game/picture/item" + mm + L".png";
                path = mm.c_str();
                loadimage(&p7, path, 0, 0, false);
                if (j % 2 == 0) {
                    transparentimage(NULL, 420, 570 + J * 40, &p7, 0xFF55FF);
                    path1 = it[i].Name.c_str();
                    outtextxy(445, 570 + J * 40, path1);
                    outtextxy(780, 570 + J * 40, _T(":"));
                    mm = std::to_wstring(it[i].number);
                    path1 = mm.c_str();
                    outtextxy(790, 570 + J * 40, path1);
                    j++;
                }
                else if (j % 2 == 1) {
                    transparentimage(NULL, 834, 570 + J * 40, &p7, 0xFF55FF);
                    path1 = it[i].Name.c_str();
                    outtextxy(859, 570 + J * 40, path1);
                    outtextxy(1194, 570 + J * 40, _T(":"));
                    mm = std::to_wstring(it[i].number);
                    path1 = mm.c_str();
                    outtextxy(1204, 570 + J * 40, path1);
                    j++;
                }
            }
        }
        for (i = 0; i < j; i++) {
            if (m.x > 420 + (i % 2) * 414 && m.x < 830 + (i % 2) * 414 && m.y>570 + (i / 2) * 40 && m.y < 605 + (i / 2) * 40) {
                transparentimage(NULL, 415 + (i % 2) * 414, 563 + (i / 2) * 40, &p8);
                path1 = it[sto[i]].story.c_str();
                drawtext(path1, &r, DT_WORDBREAK);
                if (m.lbutton) {
                    it[sto[i]].number--;
                    p[I].arms_b_1 = ar[p[I].arms_id_1].mbullet;
                    p[I].b_id_1 = sto[i];
                }
            }
        }
        }
        else if (d == 8) {
        int J = 0, j = 0;
        if (ar[p[I].arms_id_2].type[1] == 'r') {
          for (i = 0; i < i_id; i++) {
            if ((it[i].number != 0 && it[i].type == 'b')&&i==0) {
                sto[j] = i;
                J = j / 2;
                wstring mm;
                mm = std::to_wstring(i);
                mm = L"./Game/picture/item" + mm + L".png";
                path = mm.c_str();
                loadimage(&p7, path, 0, 0, false);
                if (j % 2 == 0) {
                    transparentimage(NULL, 420, 570 + J * 40, &p7, 0xFF55FF);
                    path1 = it[i].Name.c_str();
                    outtextxy(445, 570 + J * 40, path1);
                    outtextxy(780, 570 + J * 40, _T(":"));
                    mm = std::to_wstring(it[i].number);
                    path1 = mm.c_str();
                    outtextxy(790, 570 + J * 40, path1);
                    j++;
                }
                else if (j % 2 == 1) {
                    transparentimage(NULL, 834, 570 + J * 40, &p7, 0xFF55FF);
                    path1 = it[i].Name.c_str();
                    outtextxy(859, 570 + J * 40, path1);
                    outtextxy(1194, 570 + J * 40, _T(":"));
                    mm = std::to_wstring(it[i].number);
                    path1 = mm.c_str();
                    outtextxy(1204, 570 + J * 40, path1);
                    j++;
                }
            }
        }
        }
        else if (ar[p[I].arms_id_2].type[1] == 'p') {
            if ((it[i].number != 0 && it[i].type == 'b') && i == 3) {
                sto[j] = i;
                J = j / 2;
                wstring mm;
                mm = std::to_wstring(i);
                mm = L"./Game/picture/item" + mm + L".png";
                path = mm.c_str();
                loadimage(&p7, path, 0, 0, false);
                if (j % 2 == 0) {
                    transparentimage(NULL, 420, 570 + J * 40, &p7, 0xFF55FF);
                    path1 = it[i].Name.c_str();
                    outtextxy(445, 570 + J * 40, path1);
                    outtextxy(780, 570 + J * 40, _T(":"));
                    mm = std::to_wstring(it[i].number);
                    path1 = mm.c_str();
                    outtextxy(790, 570 + J * 40, path1);
                    j++;
                }
                else if (j % 2 == 1) {
                    transparentimage(NULL, 834, 570 + J * 40, &p7, 0xFF55FF);
                    path1 = it[i].Name.c_str();
                    outtextxy(859, 570 + J * 40, path1);
                    outtextxy(1194, 570 + J * 40, _T(":"));
                    mm = std::to_wstring(it[i].number);
                    path1 = mm.c_str();
                    outtextxy(1204, 570 + J * 40, path1);
                    j++;
                }
            }
        }
        for (i = 0; i < j; i++) {
            if (m.x > 420 + (i % 2) * 414 && m.x < 830 + (i % 2) * 414 && m.y>570 + (i / 2) * 40 && m.y < 605 + (i / 2) * 40) {
                transparentimage(NULL, 415 + (i % 2) * 414, 563 + (i / 2) * 40, &p8);
                path1 = it[sto[i]].story.c_str();
                drawtext(path1, &r, DT_WORDBREAK);
                if (m.lbutton) {
                    it[sto[i]].number--;
                    p[I].arms_b_2 = ar[p[I].arms_id_2].mbullet;
                    p[I].b_id_2 = sto[i];
                }
            }
        }
        }
        if (m.x > 415 && m.x < 463 && m.y>450 && m.y < 498) {
            if (m.lbutton && q > 0) {
                q--;
                I = M[m_id].team_state[q];                
            }
        }
        else if (m.x > 1230 && m.x < 1278 && m.y>450 && m.y < 498) {
            if (m.lbutton && q <M[m_id].psize-1) {   
                q++;
                I = M[m_id].team_state[q];             
            }
        }
        if (m.rbutton) {
            return;
        }
        EndBatchDraw();
    }
}
void menu_skill(player *p,skill *sk,Map *m,int m_id,int sk_id) {
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    IMAGE p1,p2,tri1,tri2,s1,p3;
    ExMessage em;
    wstring mm,ww;
    RECT r = { 625, 15, 1250, 180 };
    loadimage(&p1, L"./Game/picture/skill_back.png", 0, 0, false);
    loadimage(&tri1, L"./Game/picture/rarrow.png", 0, 0, false);
    loadimage(&tri2, L"./Game/picture/larrow.png", 0, 0, false);
    loadimage(&p3, L"./Game/picture/skill_block.png", 0, 0, false);
    int I = 0,a[100],q=0;
    while (1) {
        j = 0;
        BeginBatchDraw();
        putimage(400, 0, &p1);
        transparentimage(NULL, 415, 450, &tri1, 0xFF55FF);
        transparentimage(NULL, 1230, 450, &tri2, 0xFF55FF);
        mm = L"./Game/picture/player"+to_wstring(I)+L"_1.png";
        loadimage(&p2, mm.c_str(), 0, 0, false);
        transparentimage(NULL, 450, 45, &p2, 0xFF55FF);
        outtextxy(490, 15, p[I].name.c_str());
        for (i = 0; i < sk_id; i++) {
            if (p[I].s_check[i] == 1 ) {
                ww = L"./Game/picture/skill" + to_wstring(i) + L".png";
                loadimage(&s1, ww.c_str(), 0, 0, false);
                if (j % 2 == 0) {
                    putimage(420, (j/2) * 50 + 200, &s1);
                    outtextxy(451, (j/2) * 50 + 200, sk[i].name.c_str());
                }
                else {
                    putimage(850, (j/2) * 50 + 200, &s1);
                    outtextxy(881, (j/2) * 50 + 200, sk[i].name.c_str());
                }
                a[j] = i;
                j++;
            }
        }

        em = getmessage(EM_MOUSE);
        for (i = 0; i < j; i++) {
            if (em.x >410+414*(i%2)&&em.x<824+(i%2)*414&&em.y>(i / 2) * 50 + 195&&em.y< (i / 2) * 50 + 245) {
                transparentimage(NULL, 410 + 414 * (i % 2), (i / 2) * 50 + 195, &p3);
                LPCTSTR path1 = sk[a[i]].story.c_str();
                drawtext(path1, &r, DT_WORDBREAK);
            }
        }
        if (em.x > 415 && em.x < 463 && em.y>450 && em.y < 498) {
            if (em.lbutton && q > 0) {
                q--;
                I = m[m_id].team_state[q];
            }
        }
        else if (em.x > 1230 && em.x < 1278 && em.y>450 && em.y < 498) {
            if (em.lbutton && q < m[m_id].psize - 1) {
                q++;
                I = m[m_id].team_state[q];
            }
        }
        if (em.rbutton) {
            break;
        }       
        EndBatchDraw();
    }
}
void menu_system() {
    IMAGE b1,tri1,tri2;
    ExMessage em;
    wstring text,vos;
    mciSendString(L"open ./Game/Sound/SE/決定ボタンを押す2.mp3 alias se", NULL, 0, NULL);    
    loadimage(&b1, L"./Game/picture/menu_back.png", 0, 0, false);
    loadimage(&tri1, L"./Game/picture/rarrow.png", 0, 0, false);
    loadimage(&tri2, L"./Game/picture/larrow.png", 0, 0, false);
    while (1) {
        BeginBatchDraw();
        putimage(400, 0, &b1);    
        transparentimage(NULL, 1200, 80, &tri2, 0xFF55FF);
        transparentimage(NULL, 425,80, &tri1, 0xFF55FF);
        text = L"BGM";
        outtextxy(800, 50, text.c_str());
        text = to_wstring(voiceSize / 5);
        outtextxy(810, 80,text.c_str());
        transparentimage(NULL, 1200, 180, &tri2, 0xFF55FF);
        transparentimage(NULL, 425, 180, &tri1, 0xFF55FF);
        text = L"SE";
        outtextxy(800, 150, text.c_str());
        text = to_wstring(soundSize / 5);
        outtextxy(810, 180, text.c_str());
        EndBatchDraw();
        em = getmessage(EM_MOUSE | EM_KEY);
        if (em.x >= 425 && em.x <= 475 && em.y > 80 && em.y < 130&&voiceSize>0&&em.lbutton) {
            voiceSize -= 50;
            vos = L"setaudio bgm volume to " + to_wstring(voiceSize);
            mciSendString(L"close se", NULL, 0, NULL);
            mciSendString(L"open ./Game/Sound/SE/決定ボタンを押す2.mp3 alias se", NULL, 0, NULL);
            mciSendString(vos.c_str(), NULL, 0, NULL);
            mciSendString(L"play se", NULL, 0, NULL);
        }
        if (em.x >= 1200 && em.x <= 1250 && em.y > 80 && em.y < 130 && voiceSize<500 && em.lbutton) {
            voiceSize += 50;
            vos = L"setaudio bgm volume to " + to_wstring(voiceSize);
            mciSendString(L"close se", NULL, 0, NULL);
            mciSendString(L"open ./Game/Sound/SE/決定ボタンを押す2.mp3 alias se", NULL, 0, NULL);
            mciSendString(vos.c_str(), NULL, 0, NULL);
            mciSendString(L"play se", NULL, 0, NULL);
        }
        if (em.x >= 425 && em.x <= 475 && em.y > 180 && em.y < 230 && soundSize >0 && em.lbutton) {
            soundSize -= 50;
            vos = L"setaudio se volume to " + to_wstring(soundSize);
            mciSendString(L"close se", NULL, 0, NULL);
            mciSendString(L"open ./Game/Sound/SE/決定ボタンを押す2.mp3 alias se", NULL, 0, NULL);
            mciSendString(vos.c_str(), NULL, 0, NULL);
            mciSendString(L"play se", NULL, 0, NULL);
        }
        if (em.x >= 1200 && em.x <= 1250 && em.y > 180 && em.y < 230 && soundSize < 500 && em.lbutton) {
            soundSize += 50;
            vos = L"setaudio se volume to " + to_wstring(soundSize);
            mciSendString(L"close se", NULL, 0, NULL);
            mciSendString(L"open ./Game/Sound/SE/決定ボタンを押す2.mp3 alias se", NULL, 0, NULL);
            mciSendString(vos.c_str(), NULL, 0, NULL);
            mciSendString(L"play se", NULL, 0, NULL);
        }
        if (em.rbutton) {

            return;
        }
    }
}
void menu(player* p,item *it,arms *ar,armor *Ar,stone *st,flag* f,enemy *e,Map *M,npc *n,e_npc *e_n,Exit* EX,BOX *Box,skill *sk, m_flag* m_f,t_equip *t_E,int &P_id,int &f_id,int &i_id,int &ar_id,int &Ar_id,int &st_id,int b_id, int &psize,int &roundp, int &roundb, time_t &first, time_t &two, time_t &three,int &m_id,int ex_id,int sk_id,int m_fid,int t_Eid) {
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    wstring vos;
    mciSendString(L"close bgm", NULL, 0, NULL);
    mciSendString(L"open ./Game/Sound/SE/oylpu-opc5i.mp3 alias SE1", NULL, 0, NULL);    
    mciSendString(L"open ./Game/Sound/bgm/bgm2.mp3 alias bgm", NULL, 0, NULL);    
    mciSendString(L"play bgm repeat", NULL, 0, NULL);
    vos = L"setaudio bgm volume to " + to_wstring(voiceSize);
    mciSendString(vos.c_str(), NULL, 0, NULL);
    vos = L"setaudio SE1 volume to " + to_wstring(soundSize);
    mciSendString(vos.c_str(), NULL, 0, NULL);
    IMAGE p1, option,p2,p3,p4,p5,p6,p7,p8;
    loadimage(&p1, L"./Game/picture/menu.png", 0, 0, false);
    loadimage(&option, L"./Game/picture/option.png", 0, 0, false);
    loadimage(&p2, L"./Game/picture/e_item.png", 0, 0, false);
    loadimage(&p3, L"./Game/picture/e_player.png", 0, 0, false);
    loadimage(&p4, L"./Game/picture/e_equip.png", 0, 0, false);
    loadimage(&p5, L"./Game/picture/e_skill.png", 0, 0, false);
    loadimage(&p6, L"./Game/picture/e_save.png", 0, 0, false);
    loadimage(&p7, L"./Game/picture/e_load.png", 0, 0, false);
    loadimage(&p8, L"./Game/picture/e_system.png", 0, 0, false);
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
                if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                    while (_kbhit())
                    {
                        _getch();
                    }
                    mciSendString(L"close bgm", NULL, 0, NULL);
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
                    else if (i == 6) {
                        putimage(75, 20 + 100 * i, &p8);
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
                    else if (i == 6) {
                        putimage(25, 20 + 100 * i, &p8);
                    }
                }
              }
        if (u == 1) {
            if (c == 0) {
                mciSendString(L"play SE1", NULL, 0, NULL);               
                Sleep(30);
            }
            c = 1;
        }
        else{ 
            if (c == 1) {
                mciSendString(L"close SE1", NULL, 0, NULL);
                mciSendString(L"open ./Game/Sound/SE/oylpu-opc5i.mp3 alias SE1", NULL, 0, NULL);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                c = 0;
            }
        }
        EndBatchDraw();
        if (g == 0) {
            chose = menu_item(p, it, ar,t_E,i_id,ar_id,st_id,t_Eid);
            g = -1;
        }
        else if (g == 1) {
            menu_player(p,M,m_id);
            g = -1;
        }
        else if (g == 2) {
            menu_equip(p, ar,Ar,st,M,t_E,it,ar_id,Ar_id,st_id,m_id,t_Eid,i_id);
            g = -1;
        }
        else if (g == 3) {
            menu_skill(p, sk, M, m_id, sk_id);
            g = -1;
        }
        else if (g == 4) {
           menu_save(p, ar, it, st, f,EX,m_f,t_E,P_id, i_id, ar_id, st_id,f_id, psize, first,two, three,m_id,ex_id,sk_id,m_fid,t_Eid);
           g = -1;
        }
        else if (g == 5) {
            menu_load(p, ar, it, st, f,EX, m_f,t_E,P_id, i_id, ar_id, st_id, f_id, psize, first, two, three,m_id,ex_id,sk_id,m_fid,t_Eid);
            m_map(p,M,Box, m_id,b_id, n,NULL,""); m_put(p, M, m_id); m_set(M, n, p, e_n, Box, m_id, b_id); m_put(p, M, m_id);
            return;
        }
        else if (g == 6) {
            mciSendString(L"close SE1", NULL, 0, NULL);
      
            menu_system();     
            mciSendString(L"open ./Game/Sound/SE/oylpu-opc5i.mp3 alias SE1", NULL, 0, NULL);
            vos = L"setaudio SE1 volume to " + to_wstring(soundSize);
            g = -1;
        }
    }
}
void m_e_ai(enemy* e, player* p, Map* m, e_npc* e_n, int m_id) {
    int k,w,box[100];
    wstring vos;
    for (k = 0; k < m[m_id].esize; k++) {
        if (e_n[k].see == 0&&e_n[k].live==1) {
            if (abs(e_n[k].x - p[0].x) + abs(e_n[k].y - p[0].y) <= 4) {
                mciSendString(L"close se", NULL, 0, NULL);
                mciSendString(L"open ./Game/Sound/SE/oylpu-opc5i.mp3 alias se", NULL, 0, NULL);
                vos = L"setaudio se volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play se from 0", NULL, 0, NULL);
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
            mm = std::to_wstring(e_n[k].kind);
            mm = L"./Game/picture/enemy" + mm + L".png";
            LPCTSTR path = mm.c_str();
            loadimage(&p1, path, 0, 0, false);
            if (e_n[k].pose == 1) {
                if (e_n[k].y + 1 > m[m_id].y||ss[e_n[k].x][e_n[k].y+1]==1) {
                    transparentimage(NULL, e_n[k].x * 48-m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &p1, 0xFF55FF, 64+8, 0, 48, 64);
                    e_n[k].type++;
                    if (e_n[k].type == 3) {
                        e_n[k].type = 1;
                        e_n[k].move = 0;
                        e_n[k].see = 0;
                    }
                }
                else if (e_n[k].type == 1) {
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 + 16 * e_n[k].type - m[m_id].oy-16, &p1, 0xFF55FF, 64+8-48, 0, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 2) {
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 + 16 * e_n[k].type - m[m_id].oy - 16, &p1, 0xFF55FF, 64+8+48, 0, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 3) {
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 + 16 * e_n[k].type - m[m_id].oy - 16, &p1, 0xFF55FF, 64+8, 0, 48, 64);
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
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &p1, 0xFF55FF, 64+16, 64, 48, 64);
                    e_n[k].type++;
                    if (e_n[k].type == 3) {
                        e_n[k].type = 1;
                        e_n[k].move = 0;
                        e_n[k].see = 0;
                    }
                }
                else if (e_n[k].type == 1) {
                    transparentimage(NULL, e_n[k].x * 48 - 16 * e_n[k].type - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &p1, 0xFF55FF, 64+16-48, 64, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 2) {
                    transparentimage(NULL, e_n[k].x * 48 - 16 * e_n[k].type - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &p1, 0xFF55FF, 64+16+48, 64, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 3) {
                    transparentimage(NULL, e_n[k].x * 48 - 16 * e_n[k].type - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &p1, 0xFF55FF, 64+16, 64, 48, 64);
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
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &p1, 0xFF55FF, 64, 128, 48, 64);
                    e_n[k].type++;
                    if (e_n[k].type == 3) {
                        e_n[k].type = 1;
                        e_n[k].move = 0;
                        e_n[k].see = 0;
                    }
                }
                else if (e_n[k].type == 1) {
                    transparentimage(NULL, e_n[k].x * 48 + 16 * e_n[k].type - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &p1, 0xFF55FF, 64-48, 128, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 2) {
                    transparentimage(NULL, e_n[k].x * 48 + 16 * e_n[k].type - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &p1, 0xFF55FF, 64+48, 128, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 3) {
                    transparentimage(NULL, e_n[k].x * 48 + 16 * e_n[k].type - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &p1, 0xFF55FF, 64, 128, 48, 64);
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
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &p1, 0xFF55FF, 64+8, 192, 48, 64);
                    e_n[k].type++;
                    if (e_n[k].type == 3) {
                        e_n[k].type = 1;
                        e_n[k].move = 0;
                        e_n[k].see = 0;
                    }
                }
                else if (e_n[k].type == 1) {
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 - 16 * e_n[k].type - m[m_id].oy - 16, &p1, 0xFF55FF,64+8-48, 192, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 2) {
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 - 16 * e_n[k].type - m[m_id].oy - 16, &p1, 0xFF55FF, 64+8+48, 192, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 3) {
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 - 16 * e_n[k].type - m[m_id].oy - 16, &p1, 0xFF55FF, 64+8, 192, 48, 64);
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
            mm = std::to_wstring(e_n[k].kind);
            mm = L"./Game/picture/enemy" + mm + L".png";
            LPCTSTR path = mm.c_str();
            loadimage(&p2, path, 0, 0, false);
            if (e_n[k].pose == 1) {
                transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &p2, 0xFF55FF, 64+8, 0, 48, 64);
            }
            else if (e_n[k].pose == 2) {
                transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &p2, 0xFF55FF, 64+16, 64, 48, 64);

            }
            else if (e_n[k].pose == 3) {
                transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &p2, 0xFF55FF, 64, 128, 48, 64);

            }
            else if (e_n[k].pose == 4) {
                transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &p2, 0xFF55FF, 64+8, 192, 48, 64);

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
void show(Map* m, player* p, enemy *e,e_npc *e_n,BOX *Box,int &m_id,int b_id,int z, npc* n,string &g) {
    IMAGE map;
    int k = 0;
    BeginBatchDraw();
    m_map(p,m, Box, m_id, b_id, n,z,g);
    if (g == "w") {
        m_walk(m, p,Box, m_id,b_id, n,g);
    }
    else { 
        m_put(p,m,m_id); 
    }
    m_e_ai(e, p, m, e_n, m_id);
    EndBatchDraw();
    tb = clock();
    if ((tb - ta) < 50) {
        Sleep(50 - tb + ta);
    }
    else {
        gg++;
    }
    ta = tb;
    F++;
}
void readBmapjson(b_map *b_m,terrain (*te)[50], const char* filename,int b_mid) {
    Json::Reader reader;
    Json::Value root;

    ifstream in(filename, ios::binary);
    int u;
    if (!in.is_open()) {
        return;
    }
    if (reader.parse(in, root)) {
        b_m[b_mid].y = root["height"].asInt();
        b_m[b_mid].x = root["width"].asInt();
        for (i = 0; i < b_m[b_mid].x; i++) {
            for (j = 0; j < b_m[b_mid].y; j++) {
                te[i][j].type = 0;
                te[i][j].mA = 1;
                te[i][j].mB = 1;
            }
        }
        for (unsigned int k = 0; k < root["layers"].size(); k++) {
            if (root["layers"][k]["properties"][0]["value"].asBool() == true) {
                for (unsigned int w = 0; w < root["layers"][k]["data"].size(); w++) {
                    if (root["layers"][k]["data"][w].asInt64() != 0) {
                        te[w % b_m[b_mid].x][w / b_m[b_mid].x].type = 1;
                    }
                }
            }
        }
        if (root.isMember("bgm")) {
            b_m[b_mid].bgm = root["bgm"].asInt();
        }
        else {
            b_m[b_mid].bgm = -1;
        }
    }
    u = root["layers"][0]["data"].size();
    in.close();
}
void battle_set(enemy *e,enemy_type *e_t,player *p,b_map *b_m,b_flag *b_f,skill *sk,buff *bu,b_npc *b_n,b_npc_type *b_nt,terrain (* te)[50], int& b_mid, int sk_id,int bu_id) {
    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
    string box = "";
    wstring vos;
    int x, y,k=0,id,bgm;
    string filename;
    filename = "./Game/map/b_map" + to_string(b_mid) + string(".json");
    const char* path = filename.c_str();
    readBmapjson(b_m,te,path,b_mid);
    loadimage(&back1, L"./Game/picture/background2.png", 0, 0, false);
    loadimage(&text, L"./Game/picture/text.png", 0, 0, false);
    loadimage(&aline, L"./Game/picture/aline.png", 0, 0, false);
    if (b_m[b_mid].bgm != -1) {
        mciSendString(L"close bgm", NULL, 0, NULL);
        wstring mm;
        LPCTSTR Path;
        mm = L"open ./Game/Sound/bgm/Bbgm" + std::to_wstring(b_m[b_mid].bgm) + L".mp3 alias bgm";
        Path = mm.c_str();
        mciSendString(Path, NULL, 0, NULL);
        mciSendString(L"play bgm repeat", NULL, 0, NULL);
        mciSendString (L"status bgm volume", vo, 50, 0);
        vos = L"setaudio bgm volume to " + to_wstring(voiceSize);
        mciSendString(vos.c_str(), NULL, 0, NULL);
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
                    te[x][y].mA += 10000;
                    te[x][y].mB += 10000;
                    e[k].x = x;
                    e[k].y = y;
                    te[x][y].enemy = 1;
                    if (x+1<b_m[b_mid].x&& te[x + 1][y].type==0) {
                        te[x + 1][y].mA +=1;
                    }
                    if (x - 1 >=0 && te[x - 1][y].type == 0) {
                        te[x-1][y].mA += 1;
                    }                    
                    if (y + 1 < b_m[b_mid].y && te[x ][y+1].type == 0) {
                        te[x ][y+1].mA += 1;
                    }
                    if (y - 1 >=0 && te[x][y-1].type == 0) {
                        te[x][y-1].mA += 1;
                    }
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
                    char b='A';
                    wchar_t a;
                    e[k].baid = e_t[id].baid; e[k].str = e_t[id].str; e[k].dex = e_t[id].dex; e[k].con = e_t[id].con; e[k].INT = e_t[id].INT;
                    e[k].wis = e_t[id].wis; e[k].cha = e_t[id].cha; e[k].lv = e_t[id].lv; e[k].mhp = e_t[id].mhp; e[k].hp = e_t[id].hp; 
                    e[k].Move = e_t[id].Move; e[k].speed = e_t[id].speed; e[k].turn = 0; e[k].type = id; e[k].pose = 1; e[k].exp = e_t[id].exp;
                    b = b + k;mbtowc(&a, &b, 1); e[k].name = e_t[id].name + a;
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
                    te[x][y].mA += 10000;
                    te[x][y].mB += 10000;
                    te[x][y].player = 1;
                    if (x + 1 < b_m[b_mid].x && te[x + 1][y].type == 0) {
                        te[x + 1][y].mB += 1;
                    }
                    if (x - 1 >= 0 && te[x - 1][y].type == 0) {
                        te[x - 1][y].mB += 1;
                    }
                    if (y + 1 < b_m[b_mid].y && te[x][y + 1].type == 0) {
                        te[x][y + 1].mB += 1;
                    }
                    if (y - 1 >= 0 && te[x][y - 1].type == 0) {
                        te[x][y - 1].mB += 1;
                    }
                    i--;
                    k++;
                    break;
                }
            }
        }
    }
    b_m[b_mid].psize = k;
    box = "";
    k = 0;
    for (i = 0; i < b_m[b_mid].f_set.size(); i++) {
        if (b_m[b_mid].f_set[i] <='9'&&b_m[b_mid].f_set[i]>='0') {
            box += b_m[b_mid].f_set[i];
        }
        if (b_m[b_mid].f_set[i] == 'n') {
            b_f[k].id = stoi(box);
            b_f[k].check = 1;
            box = "";
            k++;
        }
        if (b_m[b_mid].f_set[i] == 'f') {
            b_f[k].id = stoi(box);
            b_f[k].check = 0;
            box = "";
            k++;
        }
    }
    box = "";
    k = 0;
    for (i = 0; i < b_m[b_mid].n_set.size(); i++) {
        if (b_m[b_mid].n_set[i] == 'x') {
            i++;
            while (1) {
                if (b_m[b_mid].n_set[i] >= '0' && b_m[b_mid].n_set[i] <= '9') {
                    box += b_m[b_mid].n_set[i];
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
        else if (b_m[b_mid].n_set[i] == 'y') {
            i++;
            while (1) {
                if (b_m[b_mid].n_set[i] >= '0' && b_m[b_mid].n_set[i] <= '9') {
                    box += b_m[b_mid].n_set[i];
                    i++;
                }
                else {
                    y = stoi(box);
                    b_n[k].x = x;
                    b_n[k].y = y;
                    te[x][y].mA += 10000;
                    te[x][y].mB += 10000;
                    te[x][y].npc = 1;
                    if (x + 1 < b_m[b_mid].x && te[x + 1][y].type == 0) {
                        te[x + 1][y].mB += 1;
                    }
                    if (x - 1 >= 0 && te[x - 1][y].type == 0) {
                        te[x - 1][y].mB += 1;
                    }
                    if (y + 1 < b_m[b_mid].y && te[x][y + 1].type == 0) {
                        te[x][y + 1].mB += 1;
                    }
                    if (y - 1 >= 0 && te[x][y - 1].type == 0) {
                        te[x][y - 1].mB += 1;
                    }
                    k++;
                    box = "";
                    i--;
                    break;
                }
            }
        }
        else if (b_m[b_mid].n_set[i] == 'n') {
            i++;
            while (1) {
                if (b_m[b_mid].n_set[i] >= '0' && b_m[b_mid].n_set[i] <= '9') {
                    box += b_m[b_mid].n_set[i];
                    i++;
                }
                else {
                    id = stoi(box);
                    box = "";
                    char b = 'A';
                    b_n[k].baid = b_nt[id].baid;  b_n[k].str = b_nt[id].str;  b_n[k].dex = b_nt[id].dex;  b_n[k].con = b_nt[id].con; b_n[k].INT = b_nt[id].INT;
                    b_n[k].wis = b_nt[id].wis; b_n[k].cha = b_nt[id].cha;  b_n[k].lv = b_nt[id].lv;  b_n[k].mhp = b_nt[id].mhp;  b_n[k].hp = b_nt[id].mhp;
                    b_n[k].Move = b_nt[id].Move; b_n[k].speed = b_nt[id].speed;  b_n[k].turn = 0;  b_n[k].type = id;  b_n[k].pose = b_nt[id].pose;  b_n[k].exp = b_nt[id].exp;
                    b_n[k].name = b_nt[id].name;
                    i--;
                    break;
                }
            }
        }

    }
    wstring mm;
    mm = std::to_wstring(b_mid);
    mm = L"./Game/map/b_map" + mm + L".png";
    LPCTSTR patH = mm.c_str();
    IMAGE boxx;
    loadimage(&boxx, patH, 0, 0, false);
    mapP = boxx;
    b_m[b_mid].ox = b_m[b_mid].cx * 48*(b_m[b_mid].cx>11)*(b_m[b_mid].cx-11)-(9-(b_m[b_mid].x- b_m[b_mid].cx))*(b_m[b_mid].cx+9> b_m[b_mid].x)*48;
    b_m[b_mid].oy = b_m[b_mid].cy * 48 * (b_m[b_mid].cy > 7) * (b_m[b_mid].cy - 7) - (8 - (b_m[b_mid].y - b_m[b_mid].cy)) * (b_m[b_mid].cy + 8 > b_m[b_mid].y) * 48;
    for (j = 0; j < b_m[b_mid].psize; j++) {
        for (i = 0; i < sk_id; i++) {
            if (p[j].s_check[i] >=1) {
                if (sk[i].type[0] == 'b') {
                    box = "";
                    for (int q = 1; q < sk[i].type.size(); q++) {
                        box += sk[i].type[q];
                    }
                    p[j].buff_check[stoi(box)]=1;
                }
            }
        }
    }
    for (i = 0; i < b_m[b_mid].psize; i++) {
        p_buff_check("t", p, e, b_n,bu, b_m, i, b_mid,bu_id);
    }
}
void battle2map(player *p,npc *n,e_npc *e_n,Map *m,int m_id) {
    int k, w,x,y; i = 0;
    string number = "", box = "",path;
    wstring mm;
    path="./Game/map/map" + to_string(m_id) + string(".json");
    readmapjson(m, m_id, path.c_str());
    if (m[m_id].bgm != -1) {
        mm = L"open ./Game/Sound/bgm/bgm" + std::to_wstring(m[m_id].bgm) + L".mp3 alias bgm";
        mciSendString(mm.c_str(), NULL, 0, NULL);
        mciSendString(L"play bgm", NULL, 0, NULL);
    }
    mm = L"./Game/map/map" + to_wstring(m_id) + L".png";
    IMAGE boxx;
    loadimage(&boxx, mm.c_str(), 0, 0, false);
    mapP = boxx;
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
void m_event_check(player* p, Map* m, m_flag* m_f, arms* ar, item* it, BOX* Box,t_equip *t_E, int m_id, int b_id, npc* n) {
    string box = "";
    for (i = 0; i < m[m_id].mevent_set.size(); i++) {
        if (m[m_id].mevent_set[i] == '_') {
            int s = stoi(box);
            if (p[0].pose == 1) {
                if (m_f[s].check == 0) {
                    if (m_f[s].x == p[0].x && p[0].y + 1 == m_f[s].y) {
                        string filename;
                        filename = "./Game/story/m_event" + to_string(s) + string(".json");
                        const char* path = filename.c_str();
                        readmapeventjson(p, n, m_f, ar, it,m,t_E, path, m_id, s);
                    }
                }
            }
            else if (p[0].pose == 2) {
                if (m_f[s].check == 0) {
                    if (m_f[s].x == p[0].x - 1 && p[0].y == m_f[s].y) {
                        string filename;
                        filename = "./Game/story/m_event" + to_string(s) + string(".json");
                        const char* path = filename.c_str();
                        readmapeventjson(p, n, m_f, ar, it,m,t_E, path, m_id, s);
                    }
                }
            }
            else if (p[0].pose == 3) {
                if (m_f[s].check == 0) {
                    if (m_f[s].x == p[0].x + 1 && p[0].y == m_f[s].y) {
                        string filename;
                        filename = "./Game/story/m_event" + to_string(s) + string(".json");
                        const char* path = filename.c_str();
                        readmapeventjson(p, n, m_f, ar, it, m, t_E, path, m_id, s);
                    }
                }
            }
            else if (p[0].pose == 4) {
                if (m_f[s].check == 0) {
                    if (m_f[s].x == p[0].x && p[0].y - 1 == m_f[s].y) {
                        string filename;
                        filename = "./Game/story/m_event" + to_string(s) + string(".json");
                        const char* path = filename.c_str();
                        readmapeventjson(p, n, m_f, ar, it, m, t_E, path, m_id, s);
                    }
                }
            }
            box = "";
        }
        else {
            box += m[m_id].mevent_set[i];
        }
    }
    int dx[4] = { 0,-1,1,0 }, dy[4] = { 1,0,0,-1 };
    for (j = 0; j < 4;j++) {
        if (p[0].pose == j+1) {
            for (i = 0; i < b_id; i++) {
                if (Box[i].check == true) {
                    if (Box[i].state == true) {
                        int check_x = dx[j] + p[0].x;
                        int check_y = dy[j] + p[0].y;
                        if (check_x == Box[i].x && check_y == Box[i].y) {
                             string filename;
                            filename = "./Game/story/box" + to_string(i) + string(".json");
                            const char* path = filename.c_str();
                            readmapeventjson(p, n, m_f, ar, it, m, t_E, path, m_id, -1);
                            Box[i].state = false;
                        }
                    }
                }
            }
        }
    }
}
void enemy_type_get(enemy *e,enemy_type *e_t,int k,int id,int x,int y) {
    char b = 'A';
    wchar_t a;
    e[k].x = 49;
    e[k].y = 44;
    e[k].baid = e_t[id].baid; e[k].str = e_t[id].str; e[k].dex = e_t[id].dex; e[k].con = e_t[id].con; e[k].INT = e_t[id].INT;
    e[k].wis = e_t[id].wis; e[k].cha = e_t[id].cha; e[k].lv = e_t[id].lv; e[k].mhp = e_t[id].mhp; e[k].hp = e_t[id].hp;
    e[k].move = e_t[id].move; e[k].speed = e_t[id].speed; e[k].turn = 0; e[k].type = id; e[k].pose = 1;
    b = b + k; mbtowc(&a, &b, 1); e[k].name = e_t[id].name + a;
}
void b_event(flag *f,b_flag *b_f,player *p,npc *n,b_map *b_m,enemy_type *e_t,enemy *e,Map *m,BOX *Box,b_npc *b_n,int b_mid,int P_id,int psize,int bu_id,int b_id,int b_nid) {
    int m_id;
    for (int fk = 0; fk < b_m[b_mid].fsize; fk++) {
        if (b_f[fk].check == 1) {
            if (b_f[fk].id == 0) {
                string filename;
                filename = "./Game/story/b_event" + to_string(0) + string(".json");
                const char* path = filename.c_str();
                readeventjson(p, n, f, m, Box, path, m_id, b_id);
                b_f[fk].check = 0;
            }
            if (b_f[fk].id == 1) {
                if (b_m[b_mid].time >= 12) {
                    string filename;
                    filename = "./Game/story/b_event" + to_string(1) + string(".json");
                    const char* path = filename.c_str();

                    b_camera(b_m, 49, 44, b_mid);
                    enemy_type_get(e, e_t, b_m[b_mid].esize, 1, 49, 44);
                    b_m[b_mid].esize++;
                    BeginBatchDraw();
                    maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                    e_put(e, b_m, b_m[b_mid].esize, b_mid);
                    b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                    p_put(p, b_m, psize, b_mid);
                    ui(p, e, b_m, P_id, b_m[b_mid].esize, psize, b_mid, bu_id);
                    EndBatchDraw();
                    readeventjson(p, n, f, m, Box, path, m_id, b_id);
                    b_f[fk].check = 0;
                }
            }
            if (b_f[fk].id == 2) {
                if (P_id == 0) {
                    string filename;
                    filename = "./Game/story/b_event" + to_string(2) + string(".json");
                    const char* path = filename.c_str();
                    readeventjson(p, n, f, m, Box, path, m_id, b_id);
                    b_f[fk].check = 0;
                }
            }

        }
    }

}
void END(wofstream *wofs,player* p, enemy* e,b_npc *b_n, string chose, b_map* b_m, b_flag* b_f,flag *f, npc* n, enemy_type* e_t,buff *bu,Map *m,BOX *Box, int b_mid, int& P_id, int& id, int& roundp, int& roundb,int& roundn,int psize,int bu_id,int b_id,int b_nid) {
    int t, pmax, bmax,nmax;
    wstring mm;
    IMAGE enemy1, p1,n1;
    putimage(1025, 200, &aline);
    if (chose == "e") {
        P_id = 0; id = 0; pmax = 0; bmax = 0; nmax = 0;
        for (t = 0; t < 100; t++) {
            BeginBatchDraw();
            putimage(960, 0, &back1);
            putimage(1025, 200, &aline);
            for (i = 0; i < b_m[b_mid].psize; i++) {
                if (p[i].hp > 0) {
                    mm = std::to_wstring(i);
                    mm = L"./Game/picture/A_p" + mm + L".png";
                    LPCTSTR path = mm.c_str();
                    loadimage(&p1, path, 0, 0, false);
                    transparentimage(NULL, 1040, 500 - (p[i].turn * 3), &p1, 0xFF55FF);
                }
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
            for (i = 0; i < b_m[b_mid].nsize; i++) {
                if (b_n[i].hp > 0) {
                    mm = std::to_wstring(b_n[i].type);
                    mm = L"./Game/picture/A_npc" + mm + L".png";
                    LPCTSTR path = mm.c_str();
                    loadimage(&n1, path, 0, 0, false);
                    transparentimage(NULL, 1040, 500 - (b_n[i].turn * 3), &n1, 0xFF55FF);
                }
            }
            Sleep(60);
            for (i = 0; i < b_m[b_mid].psize; i++) {
                if (p[i].turn >= 100 && p[i].buff_check[4] == 0 && p[i].buff_check[5] == 0) {
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
            for (i = 0; i < b_m[b_mid].nsize; i++) {
                if (b_n[i].turn >= 100) {
                    if (b_n[i].turn >= 100 && b_n[i].buff_check[4] == 0 && b_n[i].buff_check[5] == 0) {
                     if (b_n[nmax].turn < b_n[i].turn || nmax == i) {
                        nmax = i;
                    }
                    }
                   
                }
            }
            if (e[bmax].turn > p[pmax].turn && e[bmax].turn >= 100) {
                id = bmax; e[bmax].turn -= 100;  roundp = 0; roundb = 1; roundn = 0; e[id].move = e[id].Move; break;
            }
            else if (p[pmax].turn >= e[bmax].turn && p[pmax].turn >= 100) {
                if (p[pmax].buff_check[4] == 0 && p[pmax].buff_check[5] == 0) {
                P_id = pmax;
                p[pmax].turn -= 100;
                p[P_id].move = p[P_id].Move;
                p[P_id].act = p[P_id].Act;
                roundp = 1; roundb = 0; roundn=0;
                p_buff_check("r", p, e, b_n, bu, b_m, P_id, b_mid, bu_id);
                break;
                }                
            }
            else if(b_n[nmax].turn>=100) {
                if (b_n[nmax].buff_check[4] == 0 && b_n[nmax].buff_check[5] == 0) {
                b_nid = nmax;
                b_n[nmax].turn -= 100;
                b_n[b_nid].move = b_n[b_nid].Move;
                roundp = 0; roundb = 0; roundn = 1;
                n_buff_check("r", p, e, b_n, bu, b_m, P_id, b_mid, bu_id);
                break;
                }                
            }
            for (i = 0; i < b_m[b_mid].psize; i++) {
                if (p[i].buff_check[4] == 0 && p[i].buff_check[5] ==0 ) {
                     p[i].turn += p[i].speed;
                }                
            }
            for (i = 0; i < b_m[b_mid].esize; i++) {
                if (e[i].speed > 0) {
                    e[i].turn += e[i].speed;
                }
            }
            for (i = 0; i < b_m[b_mid].nsize; i++) {
                if (b_n[i].buff_check[4]==0&&b_n[i].buff_check[5]==0) {
                    b_n[i].turn += b_n[i].speed;
                }
               
            }
            b_m[b_mid].time++;
            p_buff_check("t", p, e, b_n, bu, b_m, P_id, b_mid,bu_id);
            p_buff_check("T", p, e, b_n, bu, b_m, P_id, b_mid,bu_id);
            n_buff_check("T", p, e, b_n, bu, b_m, P_id, b_mid, bu_id);
            for (i = 0; i < b_m[b_mid].psize;i++) {
            if (p[i].turn >= 10000) {
              p[i].turn -= 10000;
              P_id = i;
              roundp = 1; roundb = 0; roundn = 0;
              return;
            }            
            }
            wchar_t s1[] = L":";
            TCHAR t[5];
            outtextxy(1080 + 150, 0, s1);
            _stprintf(t, _T("%d"), b_m[b_mid].time % 60);
            outtextxy(1100 + 150, 0, t);
            _stprintf(t, _T("%d"), b_m[b_mid].time / 60);
            outtextxy(1060 + 150, 0, t);
            EndBatchDraw();
            b_event(f,b_f, p, n, b_m, e_t, e,m,Box,b_n, b_mid, P_id, psize,bu_id,b_id,b_nid);
        }
    }
}
void buff_skill(player *p,enemy *e,skill *sk,int sk_id,int P_id) {
    
}
void p_skill(string chose,player *p,enemy *e,b_npc *b_n,skill *sk,b_map *b_m,int sk_id,int P_id,int esize,int b_mid,int psize,int bu_id,int b_nid) { 
    wstring vos;
    if (chose == "k") {
    int a[100];
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    BeginBatchDraw();
    IMAGE m1,ae;
    j = 0;
    loadimage(&m1, L"./Game/picture/s_block.png", 0, 0, false);
    putimage(0, 0, &m1);
        for (i = 0; i < sk_id; i++) {
            if (p[P_id].s_check[i]==1&&sk[i].type[0] != 'b') {
                IMAGE s1;
                wstring ww;
                ww = L"./Game/picture/skill" + to_wstring(i) + L".png";
                loadimage(&s1,ww.c_str(), 0, 0, false);
                putimage(0, j * 50,&s1);
                outtextxy(31, j * 50, sk[i].name.c_str());
                a[j] = i;
                j++;
            }
        }
        EndBatchDraw();
        int uk = 1;
        while (uk) {
            ExMessage em;
            Sleep(10);
            if (peekmessage(&em, EM_MOUSE)) {
                for (i = 0; i < j; i++) {
                    if (em.x >= 0 && em.x <= 400 && em.y >= i * 50 && em.y <= i * 50 + 50) {
                        if (em.lbutton) {

                            uk = 0;
                            break;
                        }
                        else if (em.rbutton) {
                            uk = -1;
                            break;
                        }
                    }
                }
            }
        }
        if (uk == 0) {
            if (a[i] == 1&&p[P_id].act>0&&p[P_id].move>=3) {
                mciSendString(L"close se", NULL, 0, NULL);
                mciSendString(L"open ./Game/Sound/SE/maou_se_magical14.mp3 alias se", NULL, 0, NULL);
                vos = L"setaudio se volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play se from 0", NULL, 0, NULL);
                while (p[P_id].move >= 3) {
                    p[P_id].move -= 3;
                    p[P_id].buff_check[1]++;
                    p[P_id].act--;
                }
                loadimage(&ae, L"./Game/picture/上昇.png",0,0,false);
                    for (j = 0; j < 3; j++) {
                        for (i = 0; i < 5; i++) {
                            BeginBatchDraw();
                            maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                            e_put(e, b_m, esize, b_mid);
                            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                            p_put(p, b_m, psize, b_mid);                            
                            transparentimageA(NULL, p[P_id].x * 48 - b_m[b_mid].ox-12, p[P_id].y * 48 - b_m[b_mid].oy-18, &ae, 0x000000, i * 80, j * 80,80,80);
                            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                            EndBatchDraw();
                            Sleep((j*5+i*3)*2);
                        }
                    }
            }
        }
    }
}
void b_end_menu(player *p,enemy *e,b_map *b_m,int b_mid) {
    IMAGE p1,fa;
    wstring path;
    int a[10]={0};
    for (i = 0; i < b_m[b_mid].esize; i++) {
        for (j = 0; j < b_m[b_mid].psize; j++) {
            p[j].exp += e[i].exp;
            if (p[j].exp >= (p[j].lv * 100 + pow(p[j].lv, 3))) {
                p[j].exp -= p[j].lv * 100 + pow(p[j].lv, 3);
                p[j].lv++;
                a[j] ++;
            }
        }
    }
    loadimage(&p1, L"./Game/picture/b_end_menu.png", 0, 0, false);
    BeginBatchDraw();
    transparentimage(NULL, 0, 0, &p1);
    for (i = 0; i < b_m[b_mid].psize; i++) {
        path = L"./Game/picture/player" + to_wstring(i) + L"_1.png";
        loadimage(&fa, path.c_str(), 0, 0, false);
        transparentimage(NULL,20, 30+180*i, &fa,0xFF55FF);
        outtextxy(80, 12+180*i, p[i].name.c_str());
        path = L"LV " + to_wstring(p[i].lv);
        outtextxy(165, 25 + 180 * i, path.c_str());
        path = L"EXP " + to_wstring(p[i].exp) + L"/"+to_wstring(int(p[i].lv * 100 + pow(p[i].lv, 3)));
        outtextxy(165, 45 + 180 * i, path.c_str());
        if (a[i] >= 1) {
            outtextxy(320, 25 + 180 * i, L"^");
            for (j = 0; j < a[i]; j++) {
                p[i].dexExp += p[i].dexUp;
                p[i].strExp += p[i].strUp;
                p[i].intExp += p[i].intUp;
                p[i].conExp += p[i].conUp;
                p[i].chaExp += p[i].chaUp;
                p[i].wisExp += p[i].wisUp;

            }                
            if (p[i].strExp >= 100) {
                p[i].str += p[i].strExp / 100;
                p[i].strExp -= p[i].strExp / 100 * 100;
                outtextxy(320, 65 + 180 * i,L"^");
            }
            if (p[i].dexExp >= 100) {
                    p[i].dex += p[i].dexExp / 100;
                    p[i].dexExp -= p[i].dexExp / 100 * 100;
                    outtextxy(320, 80 + 180 * i, L"^");
            }
            if (p[i].intExp >= 100) {
                p[i].INT += p[i].intExp / 100;
                p[i].intExp -= p[i].intExp / 100 * 100;
                outtextxy(320, 95 + 180 * i, L"^");
            }
            if (p[i].conExp >= 100) {
                p[i].con += p[i].conExp / 100;
                p[i].conExp -= p[i].conExp / 100 * 100;
                outtextxy(320, 110 + 180 * i, L"^");
            }
            if (p[i].chaExp >= 100) {
                p[i].cha += p[i].chaExp / 100;
                p[i].chaExp -= p[i].chaExp / 100 * 100;
                outtextxy(320, 125 + 180 * i, L"^");
            }
            if (p[i].wisExp >= 100) {
                p[i].wis += p[i].wisExp / 100;
                p[i].wisExp -= p[i].wisExp / 100 * 100;
                outtextxy(320, 140 + 180 * i, L"^");
            }
        }
        path = L"STR "+to_wstring(p[i].str);
        outtextxy(165, 65 + 180 * i, path.c_str());
        path = L"DEX " + to_wstring(p[i].dex);
        outtextxy(165, 80 + 180 * i, path.c_str());
        path = L"INT " + to_wstring(p[i].INT);
        outtextxy(165, 95 + 180 * i, path.c_str());
        path = L"CON " + to_wstring(p[i].con);
        outtextxy(165, 110 + 180 * i, path.c_str());
        path = L"CHA " + to_wstring(p[i].cha);
        outtextxy(165, 125 + 180 * i, path.c_str());
        path = L"WIS " + to_wstring(p[i].wis);
        outtextxy(165, 140 + 180 * i, path.c_str());
    }
    EndBatchDraw();
    Sleep(3000);   
    ExMessage fq;
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    while (1) {    
        if (peekmessage(&fq, EM_MOUSE)) {
            if (fq.lbutton ) {
                return;
            }
        }
    }
}
void n_target(b_npc *b_n,b_map *b_m,enemy *e,int b_mid,int b_nid,int &id) {
    int box, min = 1000;
    for (i = 0; i < b_m[b_mid].esize; i++) {
        if (e[i].hp > 0) {
            box = abs(b_n[b_nid].x - e[i].x) + abs(b_n[b_nid].y - e[i].y);
            if (box < min) {
                min = box;
                id = i;
            }
        }
    }
}
void n_walk(wofstream* wofs, b_npc* b_n, b_map* b_m, enemy* e,player *p,arms *ar,terrain(*te)[50],int b_mid, int b_nid, int& id,int psize,int esize,int bu_id,int P_id) {
    int k,w=0,type=0;
    IMAGE npc1;
    if (abs(e[id].x - b_n[b_nid].x) + abs(e[id].y - b_n[b_nid].y) == ar[b_n[b_nid].baid].range) {
        return;
    }
    else if (abs(e[id].x - b_n[b_nid].x) + abs(e[id].y - b_n[b_nid].y) < ar[b_n[b_nid].baid].range) {
        type = 1;
    }
    int xbox = b_n[b_nid].x,ybox=b_n[b_nid].y;
    int box[100];
    r_bfs(b_m, te, b_n[b_nid].x,b_n[b_nid].y , e[id].x, e[id].y, box, b_n[b_nid].move, b_mid, ar[b_n[b_nid].baid].range,type);
    b_camera(b_m, xbox, ybox, b_mid);
    wstring mm;
    mm = std::to_wstring(b_n[b_nid].type);
    mm = L"./Game/picture/b_npc" + mm + L".png";
    loadimage(&npc1, mm.c_str(), 0, 0, false);
    while (1) {
        if (box[w] == 2) {
            if (b_n[b_nid].move >= te[xbox][ybox + 1].mA) {
                b_n[b_nid].move = b_n[b_nid].move - te[xbox][ybox + 1].mA;
            }
            else {
                break;
            }
        }
        else if (box[w] == 4) {
            if (b_n[b_nid].move >= te[xbox - 1][ybox].mA) {
                b_n[b_nid].move = b_n[b_nid].move - te[xbox - 1][ybox].mA;
            }
            else {
                break;
            }
        }
        else if (box[w] == 6) {
            if (b_n[b_nid].move >= te[xbox + 1][ybox].mA) {
                b_n[b_nid].move = b_n[b_nid].move - te[xbox + 1][ybox].mA;
            }
            else {
                break;
            }
        }
        else if (box[w] == 8) {
            if (b_n[b_nid].move >= te[xbox][ybox - 1].mA) {
                b_n[b_nid].move = b_n[b_nid].move - te[xbox][ybox - 1].mA;
            }
            else {
                break;
            }
        }
        if (box[w] == 2) {

            for (k = 0; k <= 48; k += 16) {

                BeginBatchDraw();
                maps(p, P_id, e, b_m, b_mid);
                e_put(e, b_m, esize, b_mid);
                b_bnput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                if (xbox * 48 - b_m[b_mid].ox < 960 && ((ybox) * 48 + k) - b_m[b_mid].oy - 16 < 720) {
                    transparentimage(NULL, xbox * 48 - b_m[b_mid].ox, ((ybox) * 48 + k) - b_m[b_mid].oy - 16, &npc1, 0xFF55FF, 64 + 8, 0, 48, 64);
                }
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                EndBatchDraw();
                Sleep(30);
                b_n[b_nid].pose = 1;
            }
            ybox++;
        }
        else if (box[w] == 4) {
            for (k = 0; k <= 48; k += 16) {

                BeginBatchDraw();
                maps(p, P_id, e, b_m, b_mid);
                e_put(e, b_m, esize, b_mid);
                b_bnput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                if (xbox * 48 - k - b_m[b_mid].ox < 960 && ybox * 48 - b_m[b_mid].oy - 16) {
                    transparentimage(NULL, xbox * 48 - k - b_m[b_mid].ox, ybox * 48 - b_m[b_mid].oy - 16, &npc1, 0xFF55FF, 64 + 16, 64, 48, 64);
                }
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                EndBatchDraw();
                Sleep(30);
                b_n[b_nid].pose = 2;
            }
            xbox--;

        }
        else if (box[w] == 6) {
            for (k = 0; k <= 48; k += 16) {

                BeginBatchDraw();
                maps(p, P_id, e, b_m, b_mid);
                e_put(e, b_m, esize, b_mid);
                b_bnput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                if (xbox * 48 + k - b_m[b_mid].ox < 960 && ybox * 48 - b_m[b_mid].oy - 16 < 720) {
                    transparentimage(NULL, xbox * 48 + k - b_m[b_mid].ox, ybox * 48 - b_m[b_mid].oy - 16, &npc1, 0xFF55FF, 64 + 0, 128, 48, 64);
                }
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                EndBatchDraw();
                Sleep(30);
                b_n[b_nid].pose = 3;
            }
            xbox++;
        }
        else if (box[w] == 8) {
            for (k = 0; k <= 48; k += 16) {
                BeginBatchDraw();
                maps(p, P_id, e, b_m, b_mid);
                e_put(e, b_m, esize, b_mid);
                b_bnput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                if (xbox * 48 - b_m[b_mid].ox < 960 && (ybox * 48) - k - b_m[b_mid].oy - 16 < 720) {
                    transparentimage(NULL, xbox * 48 - b_m[b_mid].ox, (ybox * 48) - k - b_m[b_mid].oy - 16, &npc1, 0xFF55FF, 64 + 8, 192, 48, 64);
                }
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                EndBatchDraw();
                Sleep(30);
                b_n[b_nid].pose = 4;
            }

            ybox--;
        }
        else if (box[w] == 0) {
            break;
        }
        w++;
    }
    te[b_n[b_nid].x][b_n[b_nid].y].mA -= 10000;
    te[b_n[b_nid].x][b_n[b_nid].y].mB -= 10000;
    int x = b_n[b_nid].x,y= b_n[b_nid].y;
    te[x][y].npc = 0;
    if (x + 1 < b_m[b_mid].x && te[x + 1][y].type == 0) {
        te[x + 1][y].mB -= 1;
    }
    if (x - 1 >= 0 && te[x - 1][y].type == 0) {
        te[x - 1][y].mB -= 1;
    }
    if (y + 1 < b_m[b_mid].y && te[x][y + 1].type == 0) {
        te[x][y + 1].mB -= 1;
    }
    if (y - 1 >= 0 && te[x][y - 1].type == 0) {
        te[x][y - 1].mB -= 1;
    }
    *wofs << L"(T" << b_m[b_mid].time << L")" << b_n[b_nid].name << L"從(" << b_n[b_nid].x << L"," << b_n[b_nid].y << L")到(" << xbox << L"," << ybox << L")" << endl;
    b_n[b_nid].x = xbox; b_n[b_nid].y = ybox;
    x = b_n[b_nid].x, y = b_n[b_nid].y;
    if (x + 1 < b_m[b_mid].x && te[x + 1][y].type == 0) {
        te[x + 1][y].mB += 1;
    }
    if (x - 1 >= 0 && te[x - 1][y].type == 0) {
        te[x - 1][y].mB += 1;
    }
    if (y + 1 < b_m[b_mid].y && te[x][y + 1].type == 0) {
        te[x][y + 1].mB += 1;
    }
    if (y - 1 >= 0 && te[x][y - 1].type == 0) {
        te[x][y - 1].mB += 1;
    }
    te[b_n[b_nid].x][b_n[b_nid].y].mA += 10000;
    te[b_n[b_nid].x][b_n[b_nid].y].mB += 10000;
    te[b_n[b_nid].x][b_n[b_nid].y].npc = 1;
}
void n_attack(wofstream* wofs, b_npc* b_n, b_map* b_m, enemy* e, player* p, arms* ar, terrain(*te)[50], int b_mid, int b_nid, int& id, int psize, int esize, int bu_id, int P_id) {
    int rx, ry, ATK, AC, ax, ay;
    wstring nn, vos;
    TCHAR t[5];
    if (ar[b_n[b_nid].baid].range >= abs(b_n[b_nid].x - e[id].x) + abs(b_n[b_nid].y - e[id].y)) {
        ay = b_n[b_nid].y - e[id].y; ax = b_n[b_nid].x - e[id].x;
        if (ay > 0 && ay > abs(ax)) {
            b_n[b_nid].pose = 4;
        }
        else if (ax > 0 && ax >= abs(ay)) {
            b_n[b_nid].pose = 2;
        }
        else if (ax < 0 && abs(ax) >= abs(ay)) {
            b_n[b_nid].pose = 3;
        }
        else if (ay < 0 && abs(ay) > abs(ax)) {
            b_n[b_nid].pose = 1;
        }
        ATK = roll("", 2) + roll(ar[b_n[b_nid].baid].hit, 1);
        AC = roll("", 2) + (e[id].dex - 10) / 2;
        IMAGE ae;
        if (b_n[b_nid].baid == 0) {
            mciSendString(L"close se", NULL, 0, NULL);
            mciSendString(L"open ./Game/Sound/SE/拳銃2.mp3 alias se", NULL, 0, NULL);
            vos = L"setaudio se volume to " + to_wstring(soundSize);
            mciSendString(vos.c_str(), NULL, 0, NULL);
            mciSendString(L"play se from 0", NULL, 0, NULL);
            loadimage(&ae, L"./Game/picture/武器・弓1.png", 0, 0, false);
            for (j = 0; j < 5; j++) {
                for (i = 0; i < 3; i++) {
                    BeginBatchDraw();
                    maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                    e_put(e, b_m, esize, b_mid);
                    b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                    p_put(p, b_m, psize, b_mid);
                    transparentimageA(NULL, e[id].x * 48 - b_m[b_mid].ox - 30, e[id].y * 48 - b_m[b_mid].oy - 160, &ae, 0x000000, i * 120, j * 300, 120, 300);
                    ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                    EndBatchDraw();
                    Sleep(5);
                }
            }
        }
        else if (b_n[b_nid].baid == 4) {
            mciSendString(L"close se", NULL, 0, NULL);
            mciSendString(L"open ./Game/Sound/SE/剣の素振り3.mp3 alias se", NULL, 0, NULL);
            vos = L"setaudio se volume to " + to_wstring(soundSize);
            mciSendString(vos.c_str(), NULL, 0, NULL);
            mciSendString(L"play se from 0", NULL, 0, NULL);
        }
        else if (b_n[b_nid].baid == 1) {
            mciSendString(L"close se", NULL, 0, NULL);
            mciSendString(L"open ./Game/Sound/SE/斧で斬る1.mp3 alias se", NULL, 0, NULL);
            vos = L"setaudio se volume to " + to_wstring(soundSize);
            mciSendString(vos.c_str(), NULL, 0, NULL);
            mciSendString(L"play se from 0", NULL, 0, NULL);
            loadimage(&ae, L"./Game/picture/武器・ツメ2.png", 0, 0, false);
            for (j = 0; j < 5; j++) {
                for (i = 0; i < 3; i++) {
                    BeginBatchDraw();
                    maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                    e_put(e, b_m, esize, b_mid);
                    b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                    p_put(p, b_m, psize, b_mid);
                    transparentimageA(NULL, e[id].x * 48 - b_m[b_mid].ox - 25, e[id].y * 48 - 16 - b_m[b_mid].oy - 20, &ae, 0x000000, i * 120, j * 120, 120, 120);
                    ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                    EndBatchDraw();
                    Sleep(5);
                }
            }
        }
        if (ATK > AC) {
            BeginBatchDraw();
            maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
            e_put(e, b_m, esize, b_mid);
            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
            p_put(p, b_m, psize, b_mid);
            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
            int  DMG = roll(ar[b_n[b_nid].baid].dmg, 1);
            _stprintf(t, _T("%d"), -DMG);
            settextcolor(RGB(255, 0, 0));
            settextstyle(25, 0, _T("Taipei Sans TC Beta"));
            outtextxy(e[id].x * 48 - b_m[b_mid].ox, e[id].y * 48 - b_m[b_mid].oy, t);
            settextstyle(18, 0, _T("Taipei Sans TC Beta"));
            settextcolor(RGB(0, 0, 0));
            e[id].hp -= DMG;
            *wofs << L"(T" << b_m[b_mid].time << L")" << b_n[b_nid].name << L"用" << ar[b_n[b_nid].baid].name << L"命中" << e[id].name << L"造成" << DMG << L"點傷害(" << ATK << ">" << AC << ")" << endl;
            EndBatchDraw();
            IMAGE tri;
            loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
            transparentimage(NULL, (b_n[b_nid].x) * 48 - b_m[b_mid].ox, ((b_n[b_nid].y - 1) * 48) - b_m[b_mid].oy, &tri, 0xFF55FF);
            Sleep(1000);
            if (e[id].hp <= 0) {
                te[e[id].x][e[id].y].mA = 1;
                te[e[id].x][e[id].y].mB = 1;
                te[e[id].x][e[id].y].enemy = 0;
                e[id].x = -1; e[id].y = -1;
                e[id].speed = -1;
            }
        }
        else {
            BeginBatchDraw();
            maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
            e_put(e, b_m, esize, b_mid);
            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
            p_put(p, b_m, psize, b_mid);
            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
            *wofs << L"(T" << b_m[b_mid].time << L")" << b_n[b_nid].name << L"用" << ar[b_n[b_nid].baid].name << L"攻擊" << e[id].name << L"未能命中(" << ATK << "<=" << AC << ")" << endl;
            nn = L"miss";
            settextcolor(RGB(255, 0, 0));
            settextstyle(25, 0, _T("Taipei Sans TC Beta"));
            outtextxy(e[id].x * 48 - b_m[b_mid].ox, e[id].y * 48 - b_m[b_mid].oy, nn.c_str());
            settextstyle(18, 0, _T("Taipei Sans TC Beta"));
            settextcolor(RGB(0, 0, 0));
            EndBatchDraw();
            IMAGE tri;
            loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
            transparentimage(NULL, (b_n[b_nid].x) * 48 - b_m[b_mid].ox, ((b_n[b_nid].y - 1) * 48) - b_m[b_mid].oy, &tri, 0xFF55FF);
            Sleep(1000);
        }
    }
}
int main() {    
    HWND hwnd=initgraph(1296, 960);
    SetWindowPos(hwnd, HWND_TOP, 250, 10, 1296, 960,0);
    initgraph(1296, 960);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    hwnd = GetHWnd();    
    HMENU hmenu = GetSystemMenu(hwnd, FALSE);
    if (hmenu != NULL) {
        int count = GetMenuItemCount(hmenu);
        if (count > 0) {
            // 关闭菜单项通常是最后一项，因此 index = count - 1
            int index = count - 1;
            RemoveMenu(hmenu, index, MF_BYPOSITION);
            DrawMenuBar(hwnd);
        }
    }
    SetWindowText(hwnd,L"隕星傳奇");
    srand(time(NULL));
    int  id=0, P_id=0,m_id=1, psize=1, load = 0,n_id=1,i_id=6,ar_id=7,Ar_id=2,st_id=2,f_id=12,b_mid,ex_id=8,b_id=2,sk_id=7,buff_id=7,m_fid=3,b_nid=0,t_Eid=2;
    /*變數數量*/
    wofstream wofs;
    player p[3];
    enemy_type e_t[2];
    enemy e[10];
    arms  ar[7];
    armor Ar[2];
    item  it[6];
    Map   m[6];
    npc  n[100];
    stone st[2];
    flag f[100];
    e_npc e_n[100];
    b_map b_m[3];
    Exit EX[8];
    m_flag m_f[100];
    BOX Box[10];
    b_flag b_f[100];
    skill sk[100];
    buff bu[100];
    b_npc b_n[10];
    b_npc_type b_nt[10];
    terrain te[50][50];
    t_equip t_E[2];
    int ix, iy, abox1[1];
    int  t, roundp = 0, roundb = 0,roundn=0;
    string  chose = "l" ;
    wstring vos;
    string a, b = ".txt", read = "";    
    time_t first=0, two=0,three=0;
    m[2].psize = 1; m[2].nsize = 1; m[2].esize = 0; m[2].exitsize = 3; m[2].box_size = 1; m[2].exit_set = "1_2_3_"; m[2].npcid = "3-"; m[2].mevent_size = 1; m[2].mevent_set = "1_"; m[2].box_set = "1n";
    m[1].psize = 1; m[1].nsize = 0; m[1].esize = 0; m[1].exitsize = 1; m[1].box_size = 1; m[1].exit_set = "0_"; m[1].mevent_size = 1; m[1].mevent_set = "0_"; m[1].box_set = "0n";
    m[0].x = 27; m[0].y = 20; m[0].psize = 1; m[0].nsize = 1; m[0].esize = 3; m[0].npcid = "1-";  m[0].b_set = "b0b0b0z"; m[0].e_set = "x10y13zx5y5zx17y17z"; m[0].block = "x2y4";
    m[3].psize = 1; m[3].nsize = 0; m[3].esize = 0; m[3].exitsize = 1; m[3].exit_set = "5_";
    m[4].psize = 1; m[4].nsize = 1; m[4].npcid = "5-"; m[4].esize = 0; m[4].exitsize = 2; m[4].exit_set = "4_6_"; m[4].mevent_size = 1; m[4].mevent_set = "2_";
    m[5].nsize = 1;m[5].npcid="6-"; m[5].esize = 2; m[5].b_set = "b0zb0z"; m[5].e_set = "x14y11k0x3y0k0"; m[5].exitsize = 1; m[5].exit_set = "7_"; m[5].mevent_size = 0;
    b_m[0].esize = 1; b_m[0].e_set = "e0x5y4"; b_m[0].p_set = "x18y14x19y14"; b_m[0].cx = 0; b_m[0].cy = 0;
    b_m[1].esize = 8; b_m[1].e_set = "e1x0y26e1x0y28e1x0y27e1x0y44e1x49y44e1x5y0e1x49y15e1x26y44"; b_m[1].p_set = "x12y25"; b_m[1].cx = 12; b_m[1].cy = 25; b_m[1].fsize = 2; b_m[1].f_set = "0n1n";
    b_m[2].esize = 2; b_m[2].e_set = "e0x5y4e0x6y3"; b_m[2].p_set = "x18y13x19y13"; b_m[2].cx = 0; b_m[2].cy = 0; b_m[2].fsize = 1; b_m[2].f_set = "2n"; b_m[2].nsize = 1; b_m[2].n_set = "n0x16y10";
    p[0].name = L"艾倫"; p[0].story = L"見習醫生"; p[0].lv = 1; p[0].mhp = 12; p[0].hp = 12; p[0].dex = 10; p[0].Move = 6; p[0].isize = 1; p[0].asize = 1; p[0].x = 10; p[0].y = 10; p[0].speed = 10; p[0].turn = 0; p[0].abox = 0; p[0].pose = 1; p[0].str = 10; p[0].INT = 10; p[0].con = 10; p[0].cha = 10; p[0].wis = 10; p[0].arms_id_1 = 4; p[0].arms_id_2 = -1; p[0].armor_id = 1; p[0].stone_id = 0; p[0].state = 1; p[0].exp = 0; p[0].dexUp = 50; p[0].strExp = 40; p[0].intUp = 60; p[0].conUp = 20; p[0].chaUp = 60; p[0].wisUp = 20; p[0].act = 1; p[0].Act = 1; p[0].arms_b_1 = 1; p[0].arms_b_2 = 0; p[0].b_id_1 = -1; p[0].b_id_2 = -1;
    p[1].name = L"夏洛特"; p[1].story = L"獵人"; p[1].lv = 1; p[1].mhp = 12; p[1].hp = 12; p[1].dex = 10; p[1].Move = 6; p[1].isize = 1; p[1].asize = 1; p[1].x = 10; p[1].y = 10; p[1].speed = 10; p[1].turn = 0; p[1].abox = 0; p[1].pose = 1; p[1].str = 10; p[1].INT = 10; p[1].con = 10; p[1].cha = 10; p[1].wis = 10; p[1].arms_id_1 = 0; p[1].arms_id_2 = 6; p[1].armor_id = 0; p[1].stone_id = 0; p[1].s_check[3] = 1; p[1].exp = 0; p[1].dexUp = 60; p[1].strExp = 30; p[1].intUp = 20; p[1].conUp = 20; p[1].chaUp = 60; p[1].wisUp = 60; p[1].act = 1; p[1].Act = 1; p[1].t_id = 1; p[1].arms_b_1 = 5; p[1].arms_b_2 = 1; p[1].b_id_1 = 0; p[1].b_id_2 = -1;
    p[2].name = L"愛麗絲"; p[2].story = L"騎士"; p[2].lv = 1; p[2].mhp = 10; p[2].hp = 10; p[2].dex = 10; p[2].move = 6; p[2].isize = 1; p[2].asize = 1; p[2].x = 10; p[2].y = 10; p[2].speed = 12; p[2].turn = 0; p[2].abox = 0; p[2].pose = 1; p[2].str = 11; p[2].INT = 10; p[2].con = 11; p[2].cha = 10; p[2].wis = 10; p[2].arms_id_1 = 2; p[2].stone_id = 1;
    e_t[0].name = L"野狼"; e_t[0].story = L"團體行動的動物 隨著數量增加危險性也會大幅上升"; e_t[0].baid = 1; e_t[0].str = 12; e_t[0].dex = 15; e_t[0].con = 12; e_t[0].INT = 3; e_t[0].wis = 14; e_t[0].cha = 7; e_t[0].lv = 1; e_t[0].mhp = 11; e_t[0].hp = 11; e_t[0].Move = 7; e_t[0].speed = 12; e_t[0].exp = 105; 
    e_t[1].name = L"帝國動員兵"; e_t[1].story = L"帝國戰時動員的士兵 只接受過基礎的軍事訓練"; e_t[1].baid = 0; e_t[1].str = 11; e_t[1].dex = 10; e_t[1].con = 11; e_t[1].INT = 10; e_t[1].wis = 10; e_t[1].cha = 10; e_t[1].lv = 1; e_t[1].mhp = 10; e_t[1].hp = 10; e_t[1].Move = 5; e_t[1].speed = 11; e_t[1].exp = 50; 
    b_nt[0].name = L"戴恩"; b_nt[0].story = L"老練的獵手，曾在巴蘭斯獨立戰爭中擔任狙擊手"; b_nt[0].lv = 15; b_nt[0].mhp = 20; b_nt[0].Move = 8; b_nt[0].speed = 15; b_nt[0].pose = 2; b_nt[0].str = 15; b_nt[0].dex = 20; b_nt[0].con = 15; b_nt[0].INT = 10; b_nt[0].wis = 12; b_nt[0].cha = 9; b_nt[0].baid = 0;
    ar[0].name = L"巡林者"; ar[0].dmg = "1d12"; ar[0].Dmg = L"1d12"; ar[0].range = 5; ar[0].story = L"巴蘭斯獵戶所喜愛的獵槍，能精準發射足以殺傷大型獵物的彈葯，是帝國在大戰後遺留的產物\n傷害:1d12"; ar[0].number = 0; ar[0].bullet = 5; ar[0].mbullet = 5; ar[0].type = "rr";
    ar[1].name = L"爪子"; ar[1].dmg = "2d4+2"; ar[1].Dmg = L"2d4+2"; ar[1].hit = "1d4+2"; ar[1].range = 1; ar[1].number = 0; ar[1].bullet = 1; ar[1].mbullet = 1; ar[1].type = "m";
    ar[2].name = L"長劍"; ar[2].dmg = "1d8"; ar[2].Dmg = L"1d8"; ar[2].hit = "1d2"; ar[2].range = 1; ar[2].story = L"巴蘭斯騎士所使用的制式長劍\n傷害:1d8\n命中加值:1d2"; ar[2].number = 0; ar[2].bullet = 1; ar[2].mbullet = 1; ar[2].type = "m";
    ar[3].name = L"石中劍"; ar[3].dmg = "1d8"; ar[3].Dmg = L"1d8"; ar[3].hit = "1d2"; ar[3].range = 1; ar[3].story = L"傳說只有神選之人才能使用的聖劍\n傷害:1d8\n命中加值:1d2+3"; ar[3].number = 0; ar[3].bullet = 1; ar[3].mbullet = 1; ar[3].type = "m";
    ar[4].name = L"短刀"; ar[4].dmg = "1d4"; ar[4].Dmg = L"1d4"; ar[4].hit = "1d2+1"; ar[4].range = 1; ar[4].story = L"生活中常見的工具，作為武器用來近戰或投擲都很實用\n傷害:1d4\n命中加值:1d2+1"; ar[4].number = 0; ar[4].bullet = 1; ar[4].mbullet = 1; ar[4].type = "m";
    ar[5].name = L"30式手槍"; ar[5].dmg = "2d4"; ar[5].Dmg = L"2d4"; ar[5].hit = "1d2"; ar[5].range = 3; ar[5].story = L"大戰中配發給共和軍官使用的手槍，全稱是諾曼30式輪轉手槍，槍身刻有守護二字\n傷害:2d4\n命中加值:1d2"; ar[5].number = 0; ar[5].bullet = 6; ar[5].mbullet = 6; ar[5].type = "rp";
    ar[6].name = L"獵刀"; ar[6].dmg = "1d5"; ar[6].Dmg = L"1d5"; ar[6].hit = "1d2"; ar[6].range = 1; ar[6].story = L"用來給獵物剝皮和切肉的刀具\n傷害:1d5\n命中加值:1d2"; ar[6].number = 0; ar[6].bullet = 1; ar[6].mbullet = 1; ar[6].type = "m";
    Ar[0].name = L"獵手服"; Ar[0].story = L"與森林近似的顏色能讓獵手不容易被發現，特殊的設計使穿著者能靈巧的移動\n防禦加值:1，閃避加值:1";
    Ar[1].name = L"粗布襯衫"; Ar[1].story = L"便宜實惠耐穿，一天著裝的好選擇\n防禦加值:1";
    it[0].Name = L"步槍子彈"; it[0].name = "步槍鉛彈"; it[0].number = 10; it[0].type = 'b'; it[0].story = L"可以裝填步槍跟機槍的子彈\n傷害加值:2\n命中加值:2"; it[0].range = 0;
    it[1].Name = L"繃帶"; it[1].name = "繃帶"; it[1].number = 0; it[1].type = 'h'; it[1].story = L"保護傷處與止血，避免傷口惡化\n10秒內回復5HP並獲得兩層生命回復"; it[1].range = 1;
    it[2].Name = L"通常彈2"; it[2].name = "通常彈"; it[2].number = 0; it[2].type = 'b'; it[2].story = L"";
    it[3].Name = L"手槍子彈"; it[3].name = "手槍子彈"; it[3].number = 0; it[3].type = 'b'; it[3].story = L"可以裝填手槍和衝鋒槍的子彈\n傷害加值:1"; it[3].range = 0;
    it[4].Name = L"藥物清單"; it[4].name = "藥物清單"; it[4].number = 0; it[4].type = 'm'; it[4].story = L"父親紀錄需要藥物的內容";
    it[5].Name = L"百三聖水"; it[5].name = "百三聖水"; it[5].number = 1; it[5].type = 'h'; it[5].story = L"由白教生產的聖水藥劑，3%的聖水濃度足以治療非創傷的一切傷口\n回復20HP"; it[5].range = 1;
    st[0].name = L"長青石"; st[0].number = 1; st[0].story = L"從星隕礦採掘出的伴生礦 沾染了生命的氣息\nHP+2"; st[0].type = "H+2";
    st[1].name = L"烈風石"; st[1].number = 1; st[1].story = L"從星隕礦採掘出的伴生礦 沾染了狂風的氣息\n速度+2"; st[1].type = "S+2";
    t_E[0].name = L"簡易醫療箱"; t_E[0].story = L"父親為艾倫準備的醫療箱 能夠很好的存放藥物跟器材\n戰鬥中能攜帶的藥物數量增加 進行治療時所需的時間減少20%"; t_E[0].size = 3; t_E[0].number = 0;
    t_E[1].name = L"彈藥袋"; t_E[1].story = L"能讓彈藥便於攜帶與使用的布袋\n減少裝填時間1秒"; t_E[1].size = 3; t_E[1].number = 0;
    n[0].name = L"";
    n[1].name = L"朱利安"; n[1].x = 3; n[1].y=6; n[1].avatar=1;
    n[2].name = L"???";
    n[3].name = L"愛德華"; n[3].x = 23; n[3].y = 12; n[3].avatar = 1;
    n[4].name = L"士兵"; n[4].x = 13; n[4].y = 26; n[4].avatar = 3;
    n[5].name = L"夏洛特"; n[5].x = 27; n[5].y = 9; n[5].avatar = 1;
    n[6].name = L"戴恩"; n[6].x = 19; n[6].y = 13; n[6].avatar = 2;
    EX[0].x = 14; EX[0].y = 13; EX[0].gx = 6; EX[0].gy = 13; EX[0].Mid = 2; EX[0].state = true;  EX[0].cx = 14; EX[0].cy = 13;
    EX[1].x = 19; EX[1].y = 12; EX[1].gx = 13; EX[1].gy = 12; EX[1].Mid = 3; EX[1].state = true; EX[1].cx = 19; EX[1].cy = 12;
    EX[2].x = 19; EX[2].y = 19; EX[2].gx = 12; EX[2].gy = 25; EX[2].Mid = 4; EX[2].state = false;  EX[2].cx = 19; EX[2].cy = 19;
    EX[3].x = 6; EX[3].y = 12; EX[3].gx = 14; EX[3].gy = 12; EX[3].Mid = 1; EX[3].state = true; EX[3].cx = 6; EX[3].cy = 12;
    EX[4].x = 12; EX[4].y = 24; EX[4].gx = 19; EX[4].gy = 18; EX[4].Mid = 2; EX[4].state = true; EX[4].cx = 12; EX[4].cy = 24;
    EX[5].x = 13; EX[5].y = 13; EX[5].gx = 19; EX[5].gy = 13; EX[5].Mid = 2; EX[5].state = true; EX[5].cx = 13; EX[5].cy = 13;
    EX[6].x = 26; EX[6].y = 0; EX[6].gx = 23; EX[6].gy = 18; EX[6].Mid = 5; EX[6].state = false; EX[6].cx = 27; EX[6].cy = 0;
    EX[7].x = 22; EX[7].y =19; EX[7].gx = 26; EX[7].gy = 1; EX[7].Mid = 4; EX[7].state = true; EX[7].cx = 23; EX[7].cy = 19;
    m_f[0].x = 14; m_f[0].y = 9; m_f[0].check = 0;
    m_f[1].x = 23; m_f[1].y = 12; m_f[1].check = 0;
    m_f[2].x = 27; m_f[2].y = 9; m_f[2].check = 0;
    Box[0].state = true; Box[0].x = 16; Box[0].y=9;
    Box[1].state = true; Box[1].x = 15; Box[1].y = 12;
    sk[0].name = L"戰場敏銳"; sk[0].story = L"抓準敵人攻勢的破綻\n被動:每30秒為我方全員取得一層戰術優勢"; sk[0].type = "b0";
    sk[1].name = L"瞄準"; sk[1].story = L"進行射擊前的精準估算 每3點移動力換取一層攻擊優勢"; sk[1].type = "B";
    sk[2].name = L"巴頓術"; sk[2].story = L"一種運用杖技、腿法與柔術的綜合護身術\n近戰命中計算時加入靈巧 命中時機率使對手獲得失衡"; sk[2].type = "A";
    sk[3].name = L"快速裝填"; sk[3].story = L"長期的訓練所掌握的裝填技巧\n裝填裝備中槍械且減少裝填時間30%，根據裝填方式不同扣除所需MP"; sk[3].type = "B";
    sk[4].name = L"傷勢評估"; sk[4].story = L"快速排查傷患生命狀態，給予初步治療\n被動:治療前會先額外回復10%當前生命"; sk[4].type = "b1";
    sk[5].name = L"傷患轉移"; sk[5].story = L"將傷患轉移到更適合治療的地形\n消耗1AP，支付所需MP轉移單位與自身到指定位置";
    sk[6].name = L"白之加護"; sk[6].story = L"為一名友方單位注入生命之力\n消耗1AP，給予10護盾，以及1護甲";
    bu[0].name = L"戰場敏銳"; bu[0].story = L"戰場敏銳:每30秒為我方全員取得一層戰術優勢"; 
    bu[1].name = L"攻擊優勢"; bu[1].story = L"當進攻失敗時消耗一層優勢 進行一次重新判定 直到成功或優勢耗盡";
    bu[2].name = L"戰術優勢"; bu[2].story = L"戰術優勢:當進攻或閃躲失敗時消耗一層優勢 進行一次重新判定 直到成功或優勢耗盡";
    bu[3].name = L"失衡"; bu[3].story = L"移動力減半、行動時獲得一層劣勢";
    bu[4].name = L"治療中"; bu[4].story = L"治療中:治療時不能行動以及暫停累積行動條";
    bu[5].name = L"裝填中"; bu[5].story = L"裝填中:裝填時不能行動以及暫停累積行動條";
    bu[6].name = L"生命回復"; bu[6].story = L"生命回復:單位行動開始時，消耗一層回復3HP";
/*開始畫面*/
    while (1) {
        int s = start();
        if (s == 1) {
            first = time(NULL);
            p[0].state = 1;
            event(f, p, n,m_f,m,Box, EX,e,e_n,m_id,b_mid,b_id);
            /*事件在這裡event(f,p,n);*/
            break;
        }
        else if (s == 0) {
            menu_load(p, ar, it, st, f,EX, m_f,t_E,P_id, i_id, ar_id, st_id, f_id, psize, first, two, three, m_id, ex_id,sk_id,m_fid,t_Eid);
            if (three>0 ) {
                first = time(NULL);
                break;
            }
        }
    }
    F = 0;
        b_mid = -1;
        
        m_set(m, n, p, e_n, Box, m_id, b_id);m_map(p,m, Box, m_id, b_id, n,NULL,""); m_put(p, m, m_id);;
            string g;
            Z = 2;

   while (1) {
/*一般模式*/
            while (1) {
                event(f, p, n, m_f, m, Box,EX,e,e_n, m_id, b_mid, b_id);
                if (b_mid == -1) {
                    b_mid = b_check(p, e_n, m, m_id);
                }
                if (Z == 2) {
                    m_check(p, m, EX, Box, m_id,b_id, n, e_n);
                    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
                }
                /*!=-1*/
                if (b_mid !=-1) {
                    m[m_id].px = p[0].x;
                    m[m_id].py = p[0].y;
                    battle_set(e, e_t, p, b_m, b_f,sk,bu,b_n,b_nt,te, b_mid, sk_id,buff_id);
                    /*製作時間戳*/
                    time_t o;
                    char uuu[80];
                    tm* info;
                    time(&o);
                    info = localtime(&o);
                    strftime(uuu, 80, "%Y_%m_%d_%H_%M_%S", info);
                    string UUU = "./Game/log/" + string(uuu) + ".log";
                    if (b_m[b_mid].log_name == "") {
                         wofs.open(UUU, ios::in | ios::out | ios::app);
                        b_m[b_mid].log_name = UUU;
                    }
                    else {
                       wofs.open(b_m[b_mid].log_name, ios::in | ios::out | ios::app);
                    }
                    locale::global(locale("zh_TW.UTF-8"));
                    wofs.imbue(locale());
                    BeginBatchDraw();
                    maps(p, P_id, e, b_m, b_mid);/*地圖繪製*/
                    e_put(e, b_m, b_m[b_mid].esize, b_mid);
                    b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                    p_put(p, b_m, b_m[b_mid].psize, b_mid);
                    ui(p, e, b_m, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id);
                    EndBatchDraw();
                    b_event(f,b_f,p,n,b_m,e_t,e,m,Box,b_n,b_mid,P_id, b_m[b_mid].psize, buff_id,b_id,b_nid);
                    END(&wofs,p, e,b_n, "e",b_m,b_f,f,n,e_t,bu,m,Box,b_mid, P_id, id, roundp, roundb, roundn, b_m[b_mid].psize, buff_id,b_id,b_nid);
                    chose = "";
                    break;
                }
                if (Z == 2) {
                    g = m_act(p, m, n, m_id);
                    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
                    if (g == "w") {
                        Z = 0;
                    }
                }
                else {
                    Z++;
                }
                if (g == "t") {
                    m_event_check(p,m,m_f,ar,it,Box,t_E,m_id,b_id,n);
                }
                if (g == "esc") {
                    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
                    Sleep(500);
                    menu(p, it, ar,Ar, st, f, e,m,n,e_n, EX,Box,sk,m_f,t_E,P_id, f_id, i_id, ar_id, Ar_id,st_id,b_id, m[m_id].psize, roundp, roundb, first, two, three, m_id, ex_id, sk_id,m_fid,t_Eid);
                    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));                    
                    Sleep(500);
                    if (m[m_id].bgm != -1) {
                        wstring mm,vos;
                        mciSendString(L"close bgm", NULL, 0, NULL);
                        mm = L"open ./Game/Sound/bgm/bgm" + std::to_wstring(m[m_id].bgm) + L".mp3 alias bgm";
                        mciSendString(mm.c_str(), NULL, 0, NULL);
                        mciSendString(L"play bgm repeat", NULL, 0, NULL);
                        vos = L"setaudio bgm volume to " + to_wstring(voiceSize);
                        mciSendString(vos.c_str(), NULL, 0, NULL);
                    }
                    m_map(p,m, Box, m_id, b_id, n,Z,g);
                    m_put(p,m,m_id);
                    EndBatchDraw();
                }
                show(m, p, e, e_n,Box, m_id,b_id,Z, n, g);

            }
/*戰鬥模式*/
        mciSendString(L"open ./Game/Sound/SE/魔王魂_戦闘18.mp3", NULL, 0, NULL);
        mciSendString(L"open ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3", NULL, 0, NULL);
        mciSendString(L"open ./Game/Sound/SE/決定ボタンを押す9.mp3 alias click", NULL, 0, NULL);
        while (1) {
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            if (battle_check(p, e, b_m, b_mid) == 1) {
                b_end_menu(p, e, b_m, b_mid);
                battle2map(p, n, e_n, m, m_id); m_map(p, m, Box, m_id, b_id, n, Z, g); m_put(p, m, m_id); b_mid = -1; break;
            }
            b_event(f,b_f, p, n, b_m, e_t, e,m,Box,b_n, b_mid,P_id, b_m[b_mid].psize,buff_id,b_id,b_nid);
            settextstyle(18, 0, _T("Taipei Sans TC Beta"));
            if (roundp == 1) {
                b_camera(b_m, p[P_id].x, p[P_id].y, b_mid);
                while (chose=="") {
                BeginBatchDraw();                
                maps(p, P_id, e,b_m,b_mid);/*地圖繪製*/
                e_put(e, b_m, b_m[b_mid].esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, b_m[b_mid].psize, b_mid);
                ui(p, e, b_m, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id);
                EndBatchDraw();               
                acts(p, e,b_n, b_m, ar,bu,te,chose, b_mid, P_id, b_m[b_mid].esize, id, b_m[b_mid].psize,buff_id,b_nid);/*選項*/
                Sleep(20);
                }
                mciSendString(L"close click", NULL, 0, NULL);
                mciSendString(L"open ./Game/Sound/SE/決定ボタンを押す9.mp3 alias click", NULL, 0, NULL);
                vos = L"setaudio click volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play click", NULL, 0, NULL);                
                Save(p, e, ar, it, st, f, P_id, i_id, ar_id, st_id, f_id, b_m[b_mid].time, b_m[b_mid].psize, roundp, roundb, first, two, three, chose);
                Load(p, e, ar, it, st, f, i_id, ar_id, st_id, f_id, P_id, b_m[b_mid].time, b_m[b_mid].psize, b_m[b_mid].esize, roundp, roundb, first, two, three, chose);
                p_attack(&wofs,p, e,b_n, ar, b_m,bu,te,b_mid,P_id, id, chose, b_m[b_mid].esize, b_m[b_mid].psize, buff_id,b_nid);/*攻擊*/
                p_item(p,e,b_n, ar, it, t_E,b_m,te,chose, P_id,i_id,t_Eid,b_mid,buff_id);/*物品*/
                p_walk(&wofs,p, e,b_n,b_m,te, chose, P_id, id, b_m[b_mid].psize, b_m[b_mid].esize,b_mid, buff_id,b_nid);/*移動*/
                p_skill(chose,p,e,b_n,sk,b_m,sk_id,P_id,b_m[b_mid].esize,b_mid,b_m[b_mid].psize,buff_id,b_nid);
                END(&wofs, p, e,b_n, chose, b_m, b_f, f, n, e_t, bu, m, Box, b_mid, P_id, id, roundp, roundb, roundn, b_m[b_mid].psize, buff_id, b_id,b_nid);/*結束行動*/
                chose = "";
                /*存檔*/
                /*讀檔*/
            }
            else if (roundb == 1) {
                e_target(&wofs,e, p,b_m,b_n, P_id, id,b_mid, b_m[b_mid].psize,b_m[b_mid].nsize,b_nid);
                e_walk(&wofs,e, p, b_n,b_m,te,id, P_id, b_m[b_mid].esize, b_m[b_mid].psize,b_mid, buff_id,b_nid,e[id].target);
                e_attack(&wofs,ar, p, e,b_n,b_m,b_mid, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize,buff_id,b_nid);
                END(&wofs, p, e,b_n, "e", b_m, b_f, f, n, e_t, bu, m, Box, b_mid, P_id, id, roundp, roundb,roundn, b_m[b_mid].psize, buff_id, b_id,b_nid);/*結束行動*/
            }
            else if (roundn == 1) {
                n_target(b_n, b_m, e, b_mid, b_nid, id);
                n_walk(&wofs,b_n,b_m,e,p,ar,te,b_mid,b_nid,id,b_m[b_mid].psize,b_m[b_mid].esize,buff_id,P_id);
                n_attack(&wofs, b_n, b_m, e, p, ar, te, b_mid, b_nid, id, b_m[b_mid].psize, b_m[b_mid].esize, buff_id, P_id);
                END(&wofs, p, e, b_n, "e", b_m, b_f, f, n, e_t, bu, m, Box, b_mid, P_id, id, roundp, roundb, roundn, b_m[b_mid].psize, buff_id, b_id, b_nid);/*結束行動*/
            }
        }
    }
}

