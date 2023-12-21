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
#include<thread>
#include<csignal>
#include <mutex>
#include<condition_variable>;
using namespace std;
int i, j, ss[200][150], Z = 0; int gg = 0; int voiceSize = 500; int soundSize = 500; int oms;/*F=時間計數器*/
long long int FP = 0;
clock_t ta=0, tb,start_time;
string s[26][60];
IMAGE mapP,text, back1, aline,player_image,enemyimage[10],save_image;
HWND bc_l;
HANDLE hMutex;
CRITICAL_SECTION g_cs;
WNDPROC g_OriginWndProc = NULL;
mutex mtx;
condition_variable cv;
bool isReady = true;
bool iskle = true;
bool wmc = true;
wchar_t vo[50];
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{    
    if (uMsg == WM_CLOSE) {
        if (wmc) {
            wmc = false;
            isReady = false;
            int screen_width = GetSystemMetrics(SM_CXSCREEN);
            int screen_height = GetSystemMetrics(SM_CYSCREEN);
            int x_position = (screen_width - 250) / 2; 
            int y_position = (screen_height - 100) / 2;           
        HWND childWindow = CreateWindowEx(
            0,                              // 擴展樣式，這裡設為0
            L"STATIC",                      // 類名，這裡使用靜態文本框
            L"                            \n                     是否要退出?",      // 子窗口標題
            WS_POPUP| BS_PUSHBUTTON | WS_VISIBLE,          // 子窗口樣式
            x_position, y_position,                          // 子窗口位置（相對於主窗口）
            250, 100,  // 子窗口寬度和高度
            hwnd,                           // 父窗口句柄
            NULL,                           // 菜單句柄
            NULL,          // 實例句柄
            NULL                            // 附加參數
        );
       
        HWND button1 = CreateWindowEx(
            0,
            L"BUTTON",
            L"是",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            40, 60, 70, 30,  // 按钮的位置和大小
            childWindow,
            NULL,
            NULL,
            NULL
        );
        HWND button2 = CreateWindowEx(
            0,
            L"BUTTOn",
            L"否",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            130, 60, 70, 30,  // 按钮的位置和大小
            childWindow,
            NULL,
            NULL,
            NULL
        );
        UpdateWindow(childWindow);   
        POINT point;
        while (1) {
        GetCursorPos(&point);
        ScreenToClient(childWindow, &point);
            if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
                if (point.x > 40 && point.x < 110 && point.y>60 && point.y < 90) {
                    SystemParametersInfo(SPI_SETMOUSESPEED, 0, (LPVOID)oms, SPIF_SENDCHANGE);
                    exit(0);
                    break;
                }
                else if (point.x > 130 && point.x < 200 && point.y>60 && point.y < 90) {
                    wmc = true;
                    isReady = true;
                    DestroyWindow(childWindow);
                    break;
                }
            }
            Sleep(10);
        }
        }

    }
    else {    
        LRESULT result = CallWindowProc((WNDPROC)g_OriginWndProc, hwnd, uMsg, wParam, lParam);
        return result;
    }
}
void back_listen() {
        POINT point;
    while (1)
    {           
        ExMessage em;
        em.lParam;
        GetCursorPos(&point);  // 获取全局坐标
        ScreenToClient(bc_l, &point);
        if (GetForegroundWindow() != bc_l) {
            mtx.lock();
            isReady = false;
            Sleep(1000);
            mtx.unlock();
         }
        else {
            mtx.lock();
            if (wmc) {
            isReady = true;
            }
            mtx.unlock();
        }
        Sleep(10);
    }
}
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
void transparentimageA(IMAGE* dstimg, int x, int y, IMAGE* srcimg,int a,int b,int c,int d)
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
    /*1:nDn;2:1D20*/
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
    int dexExp=0, strExp=0, intExp=0, conExp=0, wisExp=0, chaExp=0;
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
    int sp=0;
    int s_cd[50]={0};
    int r_cd[50] = { 0 };
    int EDV = 0;
    int DEF = 0;
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
    string drop, species;
    int lv, mhp, hp, dex, str, x, y, INT, con, wis, cha, speed, move, baid,exp,Move,s_range=0,e_range=0;
    int EDV = 0, DEF = 0;
};
class enemy {
public:
    wstring name, story;
    int lv, mhp,hp,dex, str,x,y,speed, INT, con, wis, cha,turn,move,baid,pose,type,exp,Move;
    int target = -1;
    int target_x, target_y;
    int target_id;
    /*0為鎖定玩家 1為鎖定npc*/
    int buff_Size = 0;
    int buff_time[50];
    int buff_id[50];
    int cd[10]={0};
    int ap = 1;
    int buff_check[100] = { 0 };
    int buff_state[50] = { 0 };
    int s_range;
    int e_range;
    int lastx=-1;
    int lasty=-1;
    int b_num_1=0,b_num_2=0;
    int EDV = 0,DEF=0;
};
class b_npc_type {
public:
    wstring name, story;
    int lv, mhp, hp, dex, str, x, y, INT, con, wis, cha, speed, move, baid, exp,Move,pose;
    int EDV = 0, DEF = 0;
};
class b_npc {
public:
    wstring name, story;
    int lv, mhp, hp, dex, str, x, y, INT, con, wis, cha, speed, move, baid, exp,pose,type,Move,turn;
    int buff_Size = 0;
    int buff_time[50];
    int buff_state[50]={0};
    int buff_id[50];
    int buff_check[100] = { 0 };
    string buff_give[50];
    int EDV = 0, DEF = 0;
};
class arms {
public:
    wstring name,Dmg,story;
    string  dmg, hit="0", type;
    int value=1;
    int range,mbullet, bullet,number,time=0;
};
class armor {
public:
    wstring name, story;
    int EDV, DEF;
};
class item {
public:
    wstring Name,story;
    string name;
    char type;
    int number;
    int range;
    int value=1;
    int time = 0;
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
    int mx=-1, my=-1;
    int mevent_size;
    int exitsize;
    int box_size;
    int bgm;
    string block,npcid="", e_set, b_set, exit_set, mevent_set, box_set;
    wstring name=L"";
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
    int mx = -1, my = -1;
    int ox = 0;
    int oy = 0;
    int cx;
    int cy;
    int time = 0;
    int line=0;
    int type = 0;
    unsigned int time_seed;
    string e_set, set, p_set, f_set, log_name="",n_set;
    wstring vc,lc;
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
    int number,lv;
};
class skill {
public:
    wstring name, story;
    string type;
    int cost;
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
    int x, y,type;
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
    int type, mA, mB,npc=0,player=0,enemy=0,fire=0,trap=0,dark=0;
    /*dark:1=黑暗,2=夜視範圍,3=光照範圍*/
    int fire_time = 0;
};
class t_equip {
public:
    wstring name,story;
    int number,size;
    int value=1;
};
class shop {
public:
    int i_number[100]={0};
    int ar_number[100] = { 0 };
    int t_E_number[100]={0};
    string type = "";
};
class task {
public:
    int state,flag,type;
    wstring name,story;
};
void ui(player* p, enemy* e, b_map* b_m, int P_id, int esize, int psize, int b_mid, int bu_id);
void p_put(player* p, b_map* b_m, int psize, int b_mid);
void b_nput(b_npc* b_n, b_map* b_m, int b_nid, int nsize, int b_mid);
void e_put(enemy* e, b_map* b_m,terrain(*te)[50], int esize, int b_mid);
void maps(player* p, int P_id, enemy* e, b_map* b_m,arms *ar, terrain(*te)[50], int b_mid);
void pHP(player* p,b_map *b_m,int id,int b_mid,int type,int &DMG) {
    /*護甲只防護對HP傷害*/
    if (type == 0) {
        if (DMG >=  p[id].buff_check[7] + p[id].buff_check[10]) {
            DMG -=  p[id].buff_check[7] + p[id].buff_check[10];
        }
        else {
            DMG = 0;
        }
        /*減傷計算*/
        settextcolor(RGB(255, 0, 0));
        settextstyle(25, 0, _T("Taipei Sans TC Beta"));
        outtextxy(p[id].x * 48 - b_m[b_mid].ox, p[id].y * 48 - b_m[b_mid].oy - 40, to_wstring(-DMG).c_str());
        settextstyle(18, 0, _T("Taipei Sans TC Beta"));
        settextcolor(RGB(0, 0, 0));
        int dmg = DMG;
        if (p[id].buff_check[8] - dmg >= 0) {
            p[id].buff_check[8] -= dmg;
        }
        else {
            dmg -= p[id].buff_check[8];
            p[id].buff_check[8] = 0;
            if (p[id].buff_check[16] - dmg >= 0) {
                p[id].buff_check[16] -= dmg;
            }
            else {
                if (dmg-p[id].DEF > 0) {
                    dmg -= p[id].DEF;
                p[id].hp -= dmg;
                p[id].hp += p[id].buff_check[16];
                p[id].buff_check[16] = 0;
                }
                else {
                    dmg = 0;
                }
            }
        }
    }
    else if (type == 1) {

    }
}
void eHP(enemy *e,b_map *b_m,int id,int b_mid,int type,int DMG) {
    if (type == 0) {
        if (DMG >= e[id].buff_check[7] + e[id].buff_check[10]) {
            DMG -= e[id].buff_check[7] + e[id].buff_check[10];
        }
        else {
            DMG = 0;
        }
        /*減傷計算*/
        settextcolor(RGB(255, 0, 0));
        settextstyle(25, 0, _T("Taipei Sans TC Beta"));
        outtextxy(e[id].x * 48 - b_m[b_mid].ox, e[id].y * 48 - b_m[b_mid].oy - 40, to_wstring(-DMG).c_str());
        settextstyle(18, 0, _T("Taipei Sans TC Beta"));
        settextcolor(RGB(0, 0, 0));
    if (e[id].buff_check[8] - DMG >= 0) {
        e[id].buff_check[8] -= DMG;
    }
    else {
        DMG -= e[id].buff_check[8];
        e[id].buff_check[8] = 0;
        if (e[id].buff_check[16] - DMG >= 0) {
            e[id].buff_check[16] -= DMG;
        }
        else {
            e[id].hp -= DMG;
            e[id].hp += e[id].buff_check[16];
            e[id].buff_check[16] = 0;
        }
    }
    }

}
void nHP(b_npc *b_n,b_map *b_m,int id,int b_mid,int type,int DMG) {
    if (type == 0) {
        if (DMG >= b_n[id].buff_check[7] + b_n[id].buff_check[10]) {
            DMG -= b_n[id].buff_check[7] + b_n[id].buff_check[10];
        }
        else {
            DMG = 0;
        }
        /*減傷計算*/
        settextcolor(RGB(255, 0, 0));
        settextstyle(25, 0, _T("Taipei Sans TC Beta"));
        outtextxy(b_n[id].x * 48 - b_m[b_mid].ox, b_n[id].y * 48 - b_m[b_mid].oy - 40, to_wstring(-DMG).c_str());
        settextstyle(18, 0, _T("Taipei Sans TC Beta"));
        settextcolor(RGB(0, 0, 0));
        if (b_n[id].buff_check[8] - DMG >= 0) {
            b_n[id].buff_check[8] -= DMG;
        }
        else {
            DMG -= b_n[id].buff_check[8];
            b_n[id].buff_check[8] = 0;
            if (b_n[id].buff_check[16] - DMG >= 0) {
                b_n[id].buff_check[16] -= DMG;
            }
            else {
                b_n[id].hp -= DMG;
                b_n[id].hp += b_n[id].buff_check[16];
                b_n[id].buff_check[16] = 0;
            }
        }
    }
}
int rollcountA(string hit,wstring b19,int h,enemy *e,int id) {
    int k,kt,g=0,s=0,t=0,sumA=0,sumB=0,a,b,num=0;
    string box="";
    if (hit != "0") {
    for (k = 0; k < hit.size(); k++) {
        if (hit[k] == 'd') {
            t = stoi(box);
            box = "";
            k++;
        }
        if (hit[k] == '+') {
            s = stoi(box);
            g = 1;
            box = "";
            k++;
        }
        else if (hit[k] == '-') {
            s = stoi(box);
            g = -1;
            box = "";
            k++;
        }
        box += hit[k];
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
    h += g;
    }
    int g1 = 0, s1 = 0, t1 = 0;
    box = "";
    if (b19 != L"") {
   for (k = 0; k < b19.size(); k++) {
        if (hit[k] == 'd') {
            t1 = stoi(box);
            box = "";
            k++;
        }
        if (hit[k] == '+') {
            s1 = stoi(box);
            g1 = 1;
            box = "";
            k++;
        }
        else if (hit[k] == '-') {
            s1 = stoi(box);
            g1 = -1;
            box = "";
            k++;
        }
        box += b19[k];
    }
    if (s1 != 0) {
        if (g1 == 1) {
            g1*= stoi(box);
        }
        else if (g1 == -1) {
            g1*= stoi(box);
        }
    }
    else {
        s1 = stoi(box);
    }
    h += g1;
    }
 
    for (a = 1; a <= 20; a++) {
        sumA = 0; sumB = 0;
        sumA += a;
        for (b = 1; b <= 20; b++) {
            sumB += b;
            for (k = 1; k <= s; k++) {
                sumA += k;
                for (kt = 1; kt <= s1; kt++) {
                    sumA += kt;
                    if (e[id].buff_check[27] > 0) {
                        sumB /= 2;
                    }
                if (sumA +h> sumB) {
                      num++;
                }
                    sumA -= kt;
                }
                if (s1 == 0) {
                    if (e[id].buff_check[27] > 0) {
                        sumB /= 2;
                    }
                    if (sumA + h > sumB) {
                        num++;
                    }
                }
                sumA -= k;
            }
            if (s == 0) {
                for (kt = 1; kt <= s1; kt++) {
                    sumA += kt;
                    if (e[id].buff_check[27] > 0) {
                        sumB /= 2;
                    }
                    if (sumA + h > sumB) {
                        num++;
                    }
                    sumA -= kt;
                }
                if (s1 == 0) {
                    if (e[id].buff_check[27] >0) {
                        sumB/= 2;
                    }
                    if (sumA + h > sumB) {
                        num++;
                    }
                }
            }
            sumB -= b;
        }
        sumA -= a;
    }
    if (s == 0) {
        s = 1;
    }
    if (s1 == 0) {
        s1 = 1;
    }
    return num*100 / (20 * 20 * s * s1);
}
int rollcountB(string hit, int h, player* p, int P_id) {
    int k, kt, g = 0, s = 0, t = 0, sumA = 0, sumB = 0, a, b, num = 0;
    string box = "";
    if (hit != "0") {
        for (k = 0; k < hit.size(); k++) {
            if (hit[k] == 'd') {
                t = stoi(box);
                box = "";
                k++;
            }
            if (hit[k] == '+') {
                s = stoi(box);
                g = 1;
                box = "";
                k++;
            }
            else if (hit[k] == '-') {
                s = stoi(box);
                g = -1;
                box = "";
                k++;
            }
            box += hit[k];
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
        h += g;
    }
    for (a = 1; a <= 20; a++) {
        sumA = 0; sumB = 0;
        sumA += a;
        for (b = 1; b <= 20; b++) {
            sumB += b;
            for (k = 1; k <= s; k++) {
                sumA += k;
                if (p[P_id].buff_check[27] > 0) {
                    sumB /= 2;
                }
                if (sumA + h > sumB) {
                    num++;
                }
                sumA -= k;
            }
            if (s == 0) {
                    if (p[P_id].buff_check[27] > 0) {
                        sumB /= 2;
                    }
                    if (sumA + h > sumB) {
                        num++;
                    }
            }
           sumB -= b;         
        }
         sumA -= a;
    }   
    if (s == 0) {
        s = 1;
     }  
    return num * 100 / (20 * 20 * s);
}
int PKTVS(player* p, enemy* e, b_map* b_m,arms *ar,enemy_type *e_t, int P_id, int id, int b_mid,int ar_id,int type,wstring dmg) {
    /*type0:我方普攻*/
    int hx, hy, ha, hc, hv = 0, hd, hcv = 0, hrc, absa, wheel = 0,add=0;
    float mul=1;
    ExMessage m1;
    wstring mm;
    IMAGE h_r;
    RECT rect;
    loadimage(&h_r, L"./Game/picture/H_R.png", 0, 0, false);
    while (1) {
     m1 = getmessage(EM_MOUSE);
     if (type == 0) {
         if (m1.rbutton) {
             return  -2334;
         }
         if (m1.x <= 48 * i || m1.x >= 48 * i + 48 || m1.y <= 48 * j || m1.y >= 48 * j + 48) {
             return -12334;
         }
     }
    if (m1.message == WM_MOUSEWHEEL) {
        int delta = m1.wheel;
        int lines = delta / WHEEL_DELTA;
        if (wheel + lines >= 0 && wheel + lines <= hv) {
            wheel += lines;
        }
    }
    if (e[id].x * 48 - b_m[b_mid].ox + 48 + 200 > 960) {
        hx = e[id].x * 48 - b_m[b_mid].ox - 200;
    }
    else {
        hx = e[id].x * 48 - b_m[b_mid].ox + 48;
    }
    ha = 0, hc = 0;
    hv = 0, hcv = 0, hd = 0; add = 0; mul = 1;
    if (e[id].y * 48 - b_m[b_mid].oy + 48 + 250 > 720) {
        hy = e[id].y * 48 - b_m[b_mid].oy - 250;
    }
    else {
        hy = e[id].y * 48 - b_m[b_mid].oy + 48;
    }
    putimage(hx, hy, &h_r);
    if (type == 0) {
    if (ar[ar_id].type[0] == 'r') {
        ha += (p[P_id].dex - 10) / 2;
    }
    else  if (ar[ar_id].type[0] == 'm') {
        ha += (p[P_id].str - 10) / 2;
    }
    }
    hc += (e[id].dex - 10) / 2;
    mm = to_wstring(1 + ha) + L"~" + to_wstring(20 + ha);
    if (type == 0) {
    if (ar[ar_id].hit != "0") {
        mm += L"+" + string2wstring(ar[ar_id].hit);
    }
    }
    if (e[id].buff_check[19] > 0) {
        mm += L"+1d" + to_wstring(p[P_id].cha / 2);
        if (e[id].buff_check[20] == 1 || (ar[ar_id].type[0] == 'r' && p[P_id].buff_check[25] == 1)) {
            hrc = rollcountA(ar[ar_id].hit, mm, 10000 - hc, e, id);
        }
        else {
            hrc = rollcountA(ar[ar_id].hit, mm, ha - hc, e, id);
        }
    }
    else {
        if (e[id].buff_check[20] == 1 || (ar[ar_id].type[0] == 'r' && p[P_id].buff_check[25] == 1)) {
            hrc = rollcountA(ar[ar_id].hit, L"", 10000 - hc, e, id);
        }
        else {
            hrc = rollcountA(ar[ar_id].hit, L"", ha - hc, e, id);
        }
    }
    rect = { hx + 5,hy + 40,hx + 95,hy + 95 };
    drawtext(mm.c_str(), &rect, DT_WORDBREAK);
    mm = to_wstring(1 + hc) + L"~" + to_wstring(20 + hc);
    if (e[id].buff_check[27] == 1) {
        mm = to_wstring(0 + hc) + L"~" + to_wstring(10 + hc);
    }
    if (e[id].buff_check[20] == 1 || (ar[ar_id].type[0] == 'r' && p[P_id].buff_check[25] == 1&&type==0)) {
        mm = L"0";
        ha = 100000;
    }    
    rect = { hx + 105,hy + 40,hx + 195,hy + 95 };
    hv = p[P_id].buff_check[1] + p[P_id].buff_check[2];
    hcv = e[id].buff_check[29] + wheel;
    drawtext(mm.c_str(), &rect, DT_WORDBREAK);
    mm = to_wstring(hcv) + L"(" + to_wstring(hv) + L")";
    rect = { hx + 50,hy + 100,hx + 100,hy + 150 };
    drawtext(mm.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    hd = p[P_id].buff_check[29] + p[P_id].buff_check[30];
    mm = to_wstring(hd);
    rect = { hx + 150,hy + 100,hx + 200,hy + 150 };
    drawtext(mm.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    /*傷害區*/
    mm = dmg;
    if (p[P_id].s_check[15] == 1) {
        if (abs(p[P_id].x - e[id].x) + abs(p[P_id].y - e[id].y) > 5) {
            add += abs(p[P_id].x - e[id].x) + abs(p[P_id].y - e[id].y) - 5;
        }
    }
    if (p[P_id].s_check[17] == 1 && e_t[e[id].type].species == "a") {
        mul *= 1.3;
    }
    if (type == 0 ) {
        if (ar[ar_id].type[0] == 'm') {
        add += (p[P_id].str - 10) / 5;
        }
    if (p[P_id].buff_check[21] > 0) {
        add += (p[P_id].wis - 5) / 2;
    }
    }
    add -= (e[id].buff_check[7] + e[id].buff_check[10]+e[id].DEF);
    if (add > 0) {
        mm += L"+";
    }
    if (add!=0) {
    mm += to_wstring(add);
    }
    if (mul != 1) {
    mm += L"X"+to_wstring(mul);
    }
    rect = { hx + 85,hy + 200,hx + 195,hy + 246 };
    drawtext(mm.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    if (hcv >= hd) {
        absa = 100 - hrc;
        for (int I = 0; I < hcv - hd; I++) {
            absa *= (100 - hrc);
            absa /= 100;
        }
        rect = { hx + 120,hy + 156,hx + 163,hy + 189 };
        mm = to_wstring(100 - absa);
        drawtext(mm.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    else {
        absa = hrc;
        for (int I = 0; I < hd - hcv; I++) {
            absa *= hrc;
            absa /= 100;
        }
        rect = { hx + 120,hy + 156,hx + 163,hy + 189 };
        mm = to_wstring(absa);
        drawtext(mm.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    if (m1.lbutton) {
        if (p[P_id].buff_check[1] - (hcv - e[id].buff_check[29]) < 0) {
            if (p[P_id].buff_check[2] - (hcv - e[id].buff_check[29] - p[P_id].buff_check[1]) > 0) {
                p[P_id].buff_check[2] = 0;
            }
            else {
                p[P_id].buff_check[2] -= (hcv - e[id].buff_check[29] - p[P_id].buff_check[1]);
            }
            p[P_id].buff_check[1] = 0;
        }
        else {
            p[P_id].buff_check[1] -= (hcv - e[id].buff_check[29]);
        }
        e[id].buff_check[29] = 0;
        return hcv-=hd;
    }
    FlushBatchDraw();
    }

}
int EKTVS(player* p, enemy* e, b_map* b_m, arms* ar, enemy_type* e_t, armor *Ar,int P_id, int id, int b_mid, int ar_id, int type, wstring dmg) {
    int hx, hy, ha, hc, hv = 0, hd, hcv = 0, hrc, absa, wheel = 0, add = 0;
    float mul = 1;
    hv = p[P_id].buff_check[2];
    ExMessage m1;
    wstring mm;
    IMAGE h_r;
    RECT rect;
    loadimage(&h_r, L"./Game/picture/H_R.png", 0, 0, false);
    while (1) {
        m1 = getmessage(EM_MOUSE);
            if (m1.rbutton) {
                return  0;
            }
        if (m1.message == WM_MOUSEWHEEL) {
            int delta = m1.wheel;
            int lines = delta / WHEEL_DELTA;
            if (wheel + lines >= 0 && wheel + lines <= hv) {
                wheel += lines;
            }
        }
        hx = 400, hy = 250;
        ha = 0, hc = 0;
        hcv = 0, hd = 0;
        putimage(hx, hy, &h_r);
        if (type == 0) {
        if (ar[ar_id].type[0] == 'r') {
            ha += (e[id].dex - 10) / 2;
        }
        else  if (ar[ar_id].type[0] == 'm') {
            ha += (e[id].str - 10) / 2;
        }
        }
        hc += (p[P_id].dex - 10) / 2 + Ar[p[P_id].armor_id].EDV;
        mm = to_wstring(1 + ha) + L"~" + to_wstring(20 + ha);
        if (type == 0) {
        if (ar[ar_id].hit != "0") {
            mm += L"+" + string2wstring(ar[ar_id].hit);
        }
        }
        if (p[P_id].buff_check[14] == 1 || p[P_id].buff_check[20] == 1) {
            hrc = rollcountB(ar[ar_id].hit, 10000 - hc, p, P_id);
        }
        else {
            hrc = rollcountB(ar[ar_id].hit, ha - hc, p, P_id);
        }
        rect = { hx + 5,hy + 40,hx + 95,hy + 95 };
        drawtext(mm.c_str(), &rect, DT_WORDBREAK);
        mm = to_wstring(1 + hc) + L"~" + to_wstring(20 + hc);
        if (p[P_id].buff_check[27] == 1) {
            mm = to_wstring(0 + hc) + L"~" + to_wstring(10 + hc);
        }
        if (p[P_id].buff_check[14] == 1 || p[P_id].buff_check[20] == 1) {
            mm = L"0";
            ha = 100000;
        }
        rect = { hx + 105,hy + 40,hx + 195,hy + 95 };
        hcv = e[id].buff_check[29] + e[id].buff_check[30] + wheel;
        drawtext(mm.c_str(), &rect, DT_WORDBREAK);
        mm = to_wstring(hcv) + L"(" + to_wstring(hv) + L")";
        rect = { hx + 50,hy + 100,hx + 100,hy + 150 };
        drawtext(mm.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        hd = e[id].buff_check[1] + e[id].buff_check[2];
        mm = to_wstring(hd);
        rect = { hx + 150,hy + 100,hx + 200,hy + 150 };
        drawtext(mm.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        /*傷害區*/
        mm =dmg;
        if (type == 0) {
            if (ar[e[id].baid].type[0] == 'm') {
            add += (e[id].str - 10) / 5;
            }
        }
         add -= p[id].buff_check[7] + p[id].buff_check[10];
        if (add != 0) {
            if (add > 0) {
                mm += L"+" + to_wstring(add);
            }
            else {
                mm = to_wstring(add);
            }
        }
        rect = { hx + 85,hy + 200,hx + 195,hy + 246 };
        drawtext(mm.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        if (hcv >= hd) {
            absa = hrc;
            for (int I = 0; I < hcv - hd; I++) {
                absa *= hrc;
                absa /= 100;
            }
            rect = { hx + 120,hy + 156,hx + 163,hy + 189 };
            mm = to_wstring(absa);
            drawtext(mm.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        }
        else {
            absa = 100 - hrc;
            for (int I = 0; I < hd - hcv; I++) {
                absa *= (100 - hrc);
                absa /= 100;
            }
            rect = { hx + 120,hy + 156,hx + 163,hy + 189 };
            mm = to_wstring(100 - absa);
            drawtext(mm.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
        FlushBatchDraw();
        if (m1.lbutton) {
            if (p[P_id].buff_check[2] - (hcv - e[id].buff_check[29] - e[id].buff_check[30]) < 0) {
                p[P_id].buff_check[2] = 0;
            }
            else {
                p[P_id].buff_check[2] -= (hcv - e[id].buff_check[29] - e[id].buff_check[30]);
            }
            e[id].buff_check[29] = 0;
            e[id].buff_check[30] = 0;
            e[id].buff_check[1] = 0;
            e[id].buff_check[2] = 0;
            return hcv -= hd;
        }
    }
}
int retrye(wofstream* wofs, player *p,enemy *e,arms *ar,armor *Ar,int P_id,int id,int hcv,int type) {
    /*type0:普攻*/
    int ATK,AC;
    int ak[100], ac[100];
    int K = 0,C=0;
    while (1) {
        if (type == 0) {
        if (ar[e[id].baid].type[0] == 'r') {
            ATK = roll("", 2) + roll(ar[e[id].baid].hit, 1) + (e[id].dex - 10) / 2;
        }
        else if (ar[e[id].baid].type[0] == 'm') {
            ATK = roll("", 2) + roll(ar[e[id].baid].hit, 1) + (e[id].str - 10) / 2;
        }
        }
        else if (type == 1) {
            ATK = roll("", 2)  + (e[id].str - 10) / 2;
        }
        AC = roll("", 2) + (p[P_id].dex - 10) / 2 + Ar[p[P_id].armor_id].EDV;
        if (p[P_id].buff_check[27] == 1) {
            AC /= 2;
        }
        if (p[P_id].buff_check[14] == 1 || p[P_id].buff_check[20] == 1) {
            AC = 0;
        }
        if (hcv == 0) {
            if (ATK > AC) {
                *wofs << L",命中(" ;
                for (int I = 0; I < K; I++) {
                    *wofs << ak[K];
                    if (I != K - 1) {
                        *wofs << L"->";
                    }
                }
                *wofs <<L">"<<AC << L")" << endl;
                return 1;
            }
            else {
                return 0;
            }
        }
        if (hcv < 0) {
            if (ATK > AC) {
                *wofs << L",命中(";
                for (int I = 0; I < K; I++) {
                    *wofs << ak[K];
                    if (I != K - 1) {
                        *wofs << L"->";
                    }
                }
                *wofs << L">" << AC << L")" << endl;
                return 1;
            }
            hcv++;
        }
        if (hcv > 0) {
            if (ATK <= AC) {
                *wofs << L",未能命中(";
                for (int I = 0; I < K; I++) {
                    *wofs << ak[K];
                    if (I != K - 1) {
                        *wofs << L",";
                    }
                }
                *wofs << L"<="<<AC<<L")" << endl;
                return 0;
            }
            hcv--;
        }
    }
}
int retryn(wofstream* wofs, b_npc* b_n, enemy* e, arms* ar, armor* Ar, int P_id, int id, int hcv, int type) {
    /*type0:普攻*/
    int ATK, AC;
    int ak[100], ac[100];
    int K = 0, C = 0;
    while (1) {
        if (type == 0) {
            if (ar[e[id].baid].type[0] == 'r') {
                ATK = roll("", 2) + roll(ar[e[id].baid].hit, 1) + (e[id].dex - 10) / 2;
            }
            else if (ar[e[id].baid].type[0] == 'm') {
                ATK = roll("", 2) + roll(ar[e[id].baid].hit, 1) + (e[id].str - 10) / 2;
            }
        }
        else if (type == 1) {
            ATK = roll("", 2) + (e[id].str - 10) / 2;
        }
        AC = roll("", 2) + (p[P_id].dex - 10) / 2 + Ar[p[P_id].armor_id].EDV;
        if (p[P_id].buff_check[27] == 1) {
            AC /= 2;
        }
        if (p[P_id].buff_check[14] == 1 || p[P_id].buff_check[20] == 1) {
            AC = 0;
        }
        if (hcv == 0) {
            if (ATK > AC) {
                *wofs << L",命中(";
                for (int I = 0; I < K; I++) {
                    *wofs << ak[K];
                    if (I != K - 1) {
                        *wofs << L"->";
                    }
                }
                *wofs << L">" << AC << L")" << endl;
                return 1;
            }
            else {
                return 0;
            }
        }
        if (hcv < 0) {
            if (ATK > AC) {
                *wofs << L",命中(";
                for (int I = 0; I < K; I++) {
                    *wofs << ak[K];
                    if (I != K - 1) {
                        *wofs << L"->";
                    }
                }
                *wofs << L">" << AC << L")" << endl;
                return 1;
            }
            hcv++;
        }
        if (hcv > 0) {
            if (ATK <= AC) {
                *wofs << L",未能命中(";
                for (int I = 0; I < K; I++) {
                    *wofs << ak[K];
                    if (I != K - 1) {
                        *wofs << L",";
                    }
                }
                *wofs << L"<=" << AC << L")" << endl;
                return 0;
            }
            hcv--;
        }
    }
}
void skill_show(wstring name) {
    IMAGE sb;
    LOGFONT f;
    gettextstyle(&f);
    loadimage(&sb, L"./Game/picture/b_skill_block.png", 0, 0, false);
    transparentimage(NULL, 400, 20, &sb);
    RECT r = { 400, 20, 600, 70 };
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    drawtext(name.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    settextstyle(&f);
    FlushBatchDraw();
    Sleep(1000);
}
int p_buff_check(wofstream* wofs, string chose, player* p,enemy *e,b_npc *b_n, buff* bu,arms *ar, b_map* b_m, terrain(*te)[50], int P_id, int b_mid,int bu_id) {
    int I, k,bi,bt;
    string box = "";
    IMAGE be,get;
    if (chose[0] == 't') {
        if (p[P_id].buff_check[0] >= 1) {
            if (b_m[b_mid].time % 30 == 0) {
                for (k = 0; k < b_m[b_mid].psize; k++) {
                    p[k].buff_check[2]++;
                }
            }
        }
            if ((b_m[b_mid].time % 30 == 0||b_m[b_mid].time==1)&& b_m[b_mid].time != 0) {
                for (k = 0; k < b_m[b_mid].psize; k++) {
                    if (p[k].buff_check[17] >= 1) {
                         wstring mm;
                    settextcolor(RGB(47, 255, 197));
                    settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                    if (p[k].hp + 2 > p[k].mhp) {
                     mm = L"+" + to_wstring(p[k].mhp - p[k].hp);
                        p[k].hp = p[k].mhp;
                    }
                    else {
                        mm = L"+" + to_wstring(2);
                        p[k].hp += 2;
                    }
                    outtextxy(p[k].x * 48 - b_m[b_mid].ox, p[k].y * 48 - b_m[b_mid].oy - 40, mm.c_str());
                    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                    settextcolor(RGB(0, 0, 0));
                  
                }  
                FlushBatchDraw();
                Sleep(1000);
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
                        settextcolor(RGB(47, 255, 197));
                        settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                        wstring mm;
                        if (bt == 1) {
                        if (p[I].hp + 5 > p[I].mhp) {
                            mm = L"+" + to_wstring(p[I].mhp - p[I].hp);           
                        p[I].hp = p[I].mhp;
                        }
                        else {
                            mm = L"+" + to_wstring(5);
                            p[I].hp += 5;
                         }
                         p[I].buff_check[6] += 2;                                        
                        }
                        else if (bt == 5) {
                            if (p[I].hp + 10 > p[I].mhp) {
                                mm = L"+" + to_wstring(p[I].mhp - p[I].hp);
                                p[I].hp = p[I].mhp;
                            }
                            else {
                                mm = L"+" + to_wstring(10);
                                p[I].hp += 10;
                            }
                        }
                        else if(bt == 16) {
                            if (p[I].hp + p[I].mhp/4 > p[I].mhp) {
                                mm = L"+" + to_wstring(p[I].mhp - p[I].hp);
                                p[I].hp = p[I].mhp;
                            }
                            else {
                                mm = L"+" + to_wstring(p[I].mhp/4);
                                p[I].hp += p[I].mhp / 4;
                            }
                            p[I].act++;
                        }
                        outtextxy(p[I].x * 48 - b_m[b_mid].ox, p[I].y * 48 - b_m[b_mid].oy-40, mm.c_str());
                            settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                            settextcolor(RGB(0, 0, 0));
                            FlushBatchDraw();
                            Sleep(500);
                            getimage(&get, 0, 0, 1296, 960);
                    mciSendString(L"close se", NULL, 0, NULL);
                    mciSendString(L"open ./Game/Sound/SE/回復魔法4.mp3 alias se", NULL, 0, NULL);
                    wstring vos = L"setaudio se volume to " + to_wstring(soundSize);
                    mciSendString(vos.c_str(), NULL, 0, NULL);
                    mciSendString(L"play se from 0", NULL, 0, NULL);
                    loadimage(&be, L"./Game/picture/魔・回復2.png", 0, 0, false);             
                    BeginBatchDraw();
                    for (j = 0; j < 5; j++) {
                    for (i = 0; i < 3; i++) {           
                        putimage(0, 0, &get);
                        transparentimageA(NULL, p[I].x * 48 - b_m[b_mid].ox-30, p[I].y * 48 - b_m[b_mid].oy-60, &be, i * 120, j * 120, 120, 120);
                        FlushBatchDraw();
                        Sleep(50);
                    }
                    }
                    EndBatchDraw();
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

                                if (p[I].buff_give[q][bi] == 'a') {
                                    p[I].b_id_1 = bt;
                                    p[I].arms_b_1 = ar[p[I].arms_id_1].mbullet;
                                }
                                else if (p[I].buff_give[q][bi] == 'A') {
                                    p[I].b_id_2 = bt;
                                    p[I].arms_b_2 = ar[p[I].arms_id_2].mbullet;
                                }
 
                        }
                    }
                else if (p[I].buff_id[q] == 14) {
                    *wofs  <<L"(T" << b_m[b_mid].time << L")"<< p[I].name << L"退出戰鬥" << endl;
                    te[p[I].x][p[I].y].mA -= 10000;
                    te[p[I].x][p[I].y].mB -= 10000;
                    te[p[I].x][p[I].y].player = 0;
                    p[I].x = -1; p[I].y = -1;
                    p[I].buff_check[14]++;
                    if (p[I].buff_check[20] > 0) {
                        p[I].buff_check[20] = 0;
                        for (int lk = 0; lk < b_m[b_mid].esize; lk++) {
                            if (e[lk].buff_check[20] > 0) {
                                    e[lk].buff_check[20] = 0;
                            }
                        }
                    }
                    int x = p[I].x, y = p[I].y;
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
                }
                else if (p[I].buff_id[q] == 24) {
                     p[I].buff_check[25]++;
                }
                else if (p[I].buff_id[q] == 26) {
                    for (int ti = 0; ti < b_m[b_mid].x; ti++) {
                        for (int tj = 0; tj < b_m[b_mid].y; tj++) {
                            if (te[ti][tj].trap == 1) {
                                te[ti][tj].trap = 2;
                            }
                        }
                    }
                }
                p[I].buff_check[p[I].buff_id[q]]--;
                p[I].buff_time[q] =-1;
                p[I].buff_id[q] = -1;
                p[I].buff_give[q] = "";
                for (int e = q; e < p[I].buff_Size-1; e++) {
                    p[I].buff_time[e] = p[I].buff_time[e + 1];
                    p[I].buff_id[e] = p[I].buff_id[e + 1];
                    p[I].buff_give[e] = p[I].buff_give[e + 1];
                }
                p[I].buff_Size--;
                q--;
                }
            if (p[I].buff_check[14] > 0) {
                if (p[I].hp > 0) {
                    p[I].buff_check[14] = 0;
                    for (int q = 0; q < p[I].buff_Size; q++) {
                        if (p[I].buff_id[q] == 14) {
                            p[I].buff_time[q] = -1;
                            p[I].buff_id[q] = -1;
                            p[I].buff_give[q] = "";
                            for (int e = q; e < p[I].buff_Size - 1; e++) {
                                p[I].buff_time[e] = p[I].buff_time[e + 1];
                                p[I].buff_id[e] = p[I].buff_id[e + 1];
                            }
                            p[I].buff_Size--;
                            q--;
                        }
                    }
                }
            }
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
          if (p[P_id].buff_check[12] > 0) {
              settextcolor(RGB(255, 0, 0));
              settextstyle(25, 0, _T("Taipei Sans TC Beta"));
              if ((p[P_id].mhp) * p[P_id].buff_check[12] / 20 < 1) {
                  outtextxy(p[P_id].x * 48 - b_m[b_mid].ox, p[P_id].y * 48 - b_m[b_mid].oy - 40, to_wstring(-1).c_str());
                  settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                  settextcolor(RGB(0, 0, 0));
                  *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"受到" <<1<< L"點燒灼傷害" << endl;
                  p[P_id].hp -= 1;
              }
              else {
              outtextxy(p[P_id].x * 48 - b_m[b_mid].ox, p[P_id].y * 48 - b_m[b_mid].oy - 40, to_wstring(-(p[P_id].mhp )*p[P_id].buff_check[12]/20).c_str());
              settextstyle(18, 0, _T("Taipei Sans TC Beta"));
              settextcolor(RGB(0, 0, 0));
              *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"受到" << (p[P_id].mhp) * p[P_id].buff_check[12] / 20 << L"點燒灼傷害" << endl;
              int ro = (p[P_id].mhp) * p[P_id].buff_check[12] / 20;
              p[P_id].hp -= ro;
              }
              p[P_id].buff_check[12]--;
              FlushBatchDraw();
              Sleep(500);
              if (p[P_id].hp <= 0 && p[P_id].buff_check[14] == 0) {
                  p[P_id].buff_check[14] = 1;
                  p[P_id].buff_time[p[P_id].buff_Size] = 15;
                  p[P_id].buff_id[p[P_id].buff_Size] = 14;
                  p[P_id].buff_Size++;
              }
          }
          if (p[P_id].buff_check[20] > 0) {
              p[P_id].move = 0;
          }
          if (p[P_id].buff_check[27] > 0) {
              if (p[P_id].move >=5) {
                  p[P_id].move -= 5;
              }
              else {
                  p[P_id].move = 0;
              }
          }
          if (p[P_id].buff_check[28] > 0) {
              p[P_id].buff_check[28] = 0;
          }
          if (p[P_id].buff_check[31] > 0) {
              if (roll("", 2) > p[P_id].con) {
                  settextcolor(RGB(15, 79, 53));
                  settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                  outtextxy(p[P_id].x * 48 - b_m[b_mid].ox, p[P_id].y * 48 - b_m[b_mid].oy - 40, to_wstring(-3).c_str());
                  settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                  settextcolor(RGB(0, 0, 0));
                  p[P_id].hp -= 3;
                  *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"受到" << 3 << L"點中毒傷害" << endl;
                  FlushBatchDraw();
                  Sleep(500);
              }
              else {
                  p[P_id].buff_check[31]--;
              }
          }
    }
}
void n_buff_check(wofstream* wofs, string chose, player* p, enemy* e, b_npc* b_n, buff* bu, b_map* b_m, terrain(*te)[50], int P_id, int b_mid, int bu_id,int b_nid) {
    int I,k,bi,bt;
    string box = "";
    IMAGE be,get;
    
    if (chose[0] == 'T') {
        for (I = 0; I < b_m[b_mid].nsize; I++) {
            for (int q = 0; q < b_n[I].buff_Size; q++) {
                b_n[I].buff_time[q]--;
                if (b_n[I].buff_time[q] == 0) {
                    if (b_n[I].buff_id[q] == 4) {
                        if (b_n[I].buff_give[q][0] == 'i') {
                            BeginBatchDraw();
                            wstring mm;
                            settextcolor(RGB(47, 255, 197));
                            settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                            box = "";
                            for (bi = 1; bi < b_n[I].buff_give[q].size(); bi++) {
                                box += b_n[I].buff_give[q][bi];
                            }
                            bt = stoi(box);
                            if (bt == 1) {
                                if (b_n[I].hp + 5 > b_n[I].mhp) {
                                    mm = L"+" + to_wstring(b_n[I].mhp - b_n[I].hp);
                                    b_n[I].hp = b_n[I].mhp;
                                }
                                else {
                                    mm = L"+" + to_wstring(5);
                                    b_n[I].hp += 5;
                                }
                                b_n[I].buff_check[6] += 2;
                            }
                            else if (bt == 5) {
                                if (b_n[I].hp + 10 > b_n[I].mhp) {
                                    mm = L"+" + to_wstring(b_n[I].mhp - b_n[I].hp);
                                    b_n[I].hp = b_n[I].mhp;
                                }
                                else {
                                    mm = L"+" + to_wstring(10);
                                    b_n[I].hp += 10;
                                }
                            }
                            else if (bt == 16) {
                                if (b_n[I].hp + b_n[I].mhp / 4 > b_n[I].mhp) {
                                    mm = L"+" + to_wstring(b_n[I].mhp - b_n[I].hp);
                                    b_n[I].hp = b_n[I].mhp;
                                }
                                else {
                                    mm = L"+" + to_wstring(b_n[I].mhp / 4);
                                    b_n[I].hp += b_n[I].mhp / 4;
                                }                                
                            }
                            outtextxy(b_n[I].x * 48 - b_m[b_mid].ox, b_n[I].y * 48 - b_m[b_mid].oy - 40, mm.c_str());
                            settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                            settextcolor(RGB(0, 0, 0));
                            FlushBatchDraw();
                            Sleep(500);
                            getimage(&get, 0, 0, 1296, 960);
                                mciSendString(L"close se", NULL, 0, NULL);
                                mciSendString(L"open ./Game/Sound/SE/回復魔法4.mp3 alias se", NULL, 0, NULL);
                                wstring vos = L"setaudio se volume to " + to_wstring(soundSize);
                                mciSendString(vos.c_str(), NULL, 0, NULL);
                                mciSendString(L"play se from 0", NULL, 0, NULL);
                                loadimage(&be, L"./Game/picture/魔・回復2.png", 0, 0, false); 
                                for (j = 0; j < 5; j++) {
                                    for (i = 0; i < 3; i++) {                                     
                                        putimage(0, 0, &get);
                                        transparentimageA(NULL, b_n[I].x * 48 - b_m[b_mid].ox - 30, b_n[I].y * 48 - b_m[b_mid].oy - 60, &be, i * 120, j * 120, 120, 120);
                                        FlushBatchDraw();
                                        Sleep(50);
                                    }
                                }
                                EndBatchDraw();
                            }
                        
                    }
                    else if (b_n[I].buff_id[q] == 10) {
                        b_n[I].buff_check[10] = 1;
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
        if (b_n[b_nid].buff_check[6] > 0) {
            b_n[b_nid].buff_check[6]--;
            if (b_n[b_nid].hp + 3 > b_n[b_nid].mhp) {
                b_n[b_nid].hp = b_n[b_nid].mhp;
            }
            else {
                b_n[b_nid].hp += 3;
            }
        }
        if (b_n[b_nid].buff_check[12] > 0) {
            settextcolor(RGB(255, 0, 0));
            settextstyle(25, 0, _T("Taipei Sans TC Beta"));
            if (b_n[b_nid].mhp * b_n[b_nid].buff_check[12] / 20 < 1) {
                outtextxy(b_n[b_nid].x * 48 - b_m[b_mid].ox, b_n[b_nid].y * 48 - b_m[b_mid].oy - 40, to_wstring(-1).c_str());
                settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                settextcolor(RGB(0, 0, 0));
                *wofs << L"(T" << b_m[b_mid].time << L")" << b_n[b_nid].name << L"受到" << 1 << L"點燒灼傷害" << endl;
                b_n[b_nid].hp--;
            }
            else {
            outtextxy(b_n[b_nid].x * 48 - b_m[b_mid].ox, b_n[b_nid].y * 48 - b_m[b_mid].oy - 40, to_wstring(-b_n[b_nid].mhp *b_n[b_nid].buff_check[12]/ 20).c_str());
            settextstyle(18, 0, _T("Taipei Sans TC Beta"));
            settextcolor(RGB(0, 0, 0));
            *wofs << L"(T" << b_m[b_mid].time << L")" << b_n[b_nid].name << L"受到" << b_n[b_nid].mhp * b_n[b_nid].buff_check[12] / 20 << L"點燒灼傷害" << endl;
            int ro = (b_n[b_nid].mhp) * b_n[b_nid].buff_check[12] / 20;
            b_n[b_nid].hp -= ro;
            }
            b_n[b_nid].buff_check[12]--;
            FlushBatchDraw();
            Sleep(500);
            if (b_n[b_nid].hp <= 0) {
                te[b_n[b_nid].x][b_n[b_nid].y].mA -= 10000;
                te[b_n[b_nid].x][b_n[b_nid].y].mB -= 10000;
                int x = b_n[b_nid].x, y = b_n[b_nid].y;
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
                te[b_n[b_nid].x][b_n[b_nid].y].npc = 0;
                b_n[b_nid].x = -1; b_n[b_nid].y = -1;
                b_n[b_nid].speed = -1;
            }
        }
        if (b_n[b_nid].buff_check[27] > 0) {
            if (b_n[b_nid].move >= 5) {
                b_n[b_nid].move -= 5;
            }
            else {
                b_n[b_nid].move = 0;
            }
        }
    }
}
void e_buff_check(wofstream* wofs,string chose,armor *Ar, player* p, enemy* e, b_npc* b_n, buff* bu, b_map* b_m, terrain(*te)[50], int P_id, int b_mid, int bu_id,int id) {
    int I;
    if (chose == "T") {
        for (I = 0; I < b_m[b_mid].esize; I++) {
            for (int q = 0; q < e[I].buff_Size; q++) {
                if (e[I].buff_time[q] > 0) {
                    e[I].buff_time[q]--;                    
                    if (e[I].buff_time[q] == 0) {
                        if (e[I].buff_id[q] == 5) {
                            if (e[I].baid == 10) {
                                e[I].b_num_1 = 1;
                                e[I].b_num_2 = 1;
                             }
                         }
                        else if (e[I].buff_id[q] == 13) {
                            BeginBatchDraw();
                            for (int sx = -1; sx < 2; sx++) {
                                for (int sy = -1; sy < 2; sy++) {
                                    for (int k=0; k < b_m[b_mid].psize; k++) {
                                        if (p[k].x == e[I].target_x + sx && p[k].y == e[I].target_y + sy && te[e[I].target_x + sx][e[I].target_y + sy].type == 0) {
                                            int ro = roll("2d4", 1);                                            
                                            if (p[k].buff_check[14] == 0) {
                                                if (p[k].dex + p[k].EDV >= roll("", 2)) {
                                                    ro /= 2;
                                                }
                                            }
                                            pHP(p,b_m, k,b_mid, 0, ro);
                                            p[k].buff_check[12]++;
                                            *wofs << L"(T" << b_m[b_mid].time << L")" << e[I].name << L"用火球對" << p[k].name << L"造成" << ro << L"點傷害" << endl;
                                            te[e[I].target_x + sx][e[I].target_y + sy].fire = 1;
                                            te[e[I].target_x + sx][e[I].target_y + sy].fire_time = 60;
                                            if (p[k].hp <= 0 && p[k].buff_check[14] == 0) {
                                                p[k].buff_check[14] = 1;
                                                p[k].buff_time[p[k].buff_Size] = 15;
                                                p[k].buff_id[p[k].buff_Size] = 14;
                                                p[k].buff_Size++;
                                            }
                                            else if (p[k].hp <= 0 && p[k].buff_check[14] == 1) {
                                                *wofs << L"(T" << b_m[b_mid].time << L")" << p[k].name << L"退出戰鬥" << endl;
                                                if (p[k].buff_check[20] > 0) {
                                                    p[k].buff_check[20] = 0;
                                                    for (int lk = 0; lk < b_m[b_mid].esize; lk++) {
                                                        if (e[lk].buff_check[20] > 0) {
                                                            e[lk].buff_check[20] = 0;
                                                        }
                                                    }
                                                }
                                                te[p[k].x][p[k].y].mA -= 10000;
                                                te[p[k].x][p[k].y].mB -= 10000;
                                                int x = p[k].x, y = p[k].y;
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
                                                te[p[k].x][p[k].y].player = 0;
                                                p[k].x = -1; p[k].y = -1;
                                            }
                                        }
                                        else if(te[e[I].target_x + sx][e[I].target_y + sy].type == 0) {
                                            te[e[I].target_x + sx][e[I].target_y + sy].fire = 1;
                                            te[e[I].target_x + sx][e[I].target_y + sy].fire_time = 60;
                                        }
                                    }
                                    for (int k=0; k < b_m[b_mid].nsize; k++) {
                                        if (b_n[k].x == e[I].target_x + sx && b_n[k].y == e[I].target_y + sy&& te[e[I].target_x + sx][e[I].target_y + sy].type==0) {
                                            int ro = roll("2d4", 1);
                                            nHP(b_n,b_m, k,b_mid, 0, ro);
                                            b_n[k].buff_check[12]++;
                                            *wofs << L"(T" << b_m[b_mid].time << L")" << e[I].name << L"用火球對" << b_n[k].name << L"造成" << ro << L"點傷害"<<endl;
                                            te[e[I].target_x + sx][e[I].target_y + sy].fire = 1;
                                            te[e[I].target_x + sx][e[I].target_y + sy].fire_time = 60;
                                            if (b_n[k].hp <= 0) {
                                                te[b_n[k].x][b_n[k].y].mA -= 10000;
                                                te[b_n[k].x][b_n[k].y].mB -= 10000;
                                                int x = b_n[k].x, y = b_n[k].y;
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
                                                te[b_n[k].x][b_n[k].y].npc = 0;
                                                b_n[k].x = -1; b_n[k].y = -1;
                                                b_n[k].speed = -1;
                                            }
                                        }
                                        else if(te[e[I].target_x + sx][e[I].target_y + sy].type == 0) {
                                            te[e[I].target_x + sx][e[I].target_y + sy].fire = 1;
                                            te[e[I].target_x + sx][e[I].target_y + sy].fire_time = 60;
                                        }
                                    }
                                }
                            }
                            IMAGE sb;
                            loadimage(&sb, L"./Game/picture/b_skill_block.png", 0, 0, false);
                            transparentimage(NULL, 400, 20, &sb);
                            RECT r = { 400, 20, 600, 70 };
                            settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                            drawtext(L"火球", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                            IMAGE get,ef;
                            getimage(&get, 0, 0, 1296, 960);
                            mciSendString(L"close se", NULL, 0, NULL);
                            mciSendString(L"open ./Game/Sound/SE/火炎魔法2.mp3 alias se", NULL, 0, NULL);
                            wstring vos = L"setaudio se volume to " + to_wstring(soundSize);
                            mciSendString(vos.c_str(), NULL, 0, NULL);
                            mciSendString(L"play se from 0", NULL, 0, NULL);
                            loadimage(&ef, L"./Game/picture/魔・炎1.png", 0, 0, false);                         
                                for (int ej = 0; ej < 5; ej++) {  
                                    for (int ei = 0; ei < 3; ei++) {
                                    putimage(0, 0, &get);
                                    transparentimageA(NULL, 48 * e[I].target_x - b_m[b_mid].ox-40, e[I].target_y* 48 - b_m[b_mid].oy-60, &ef, ei * 120, ej * 120, 120, 120);
                                    FlushBatchDraw();
                                    Sleep(100);
                                }
                            }
                            putimage(0, 0, &get);
                            FlushBatchDraw();
                            Sleep(500);
                        }
                        else if (e[I].buff_id[q] == 15) {
                            BeginBatchDraw();
                            for (int sx = -5; sx <= 5; sx++) {
                                for (int sy = -5; sy <= 5; sy++) {          
                                    if (te[e[I].x + sx][e[I].y + sy].type == 0 && abs(sx) + abs(sy) < 6) {
                                    for (int k = 0; k < b_m[b_mid].psize; k++) {
                                            if (p[k].x == e[I].x + sx && p[k].y == e[I].y + sy ) {
                                                int ro = roll("3d3", 1);
                                                if (p[k].buff_check[14] == 0) {
                                                    if (p[k].dex + p[k].EDV >= roll("", 2)) {
                                                        ro /= 2;
                                                    }
                                                }  
                                                ro += p[k].buff_check[12] * 2;
                                                pHP(p,b_m ,k,b_mid, 0, ro);
                                                *wofs << L"(T" << b_m[b_mid].time << L")" << e[I].name << L"用炎浪對" << p[k].name << L"造成" << ro << L"點傷害" << endl;
                                                te[e[I].x + sx][e[I].y + sy].fire = 1;
                                                te[e[I].x + sx][e[I].y + sy].fire_time = 60;
                                                if (p[k].hp <= 0 && p[k].buff_check[14] == 0) {
                                                    p[k].buff_check[14] = 1;
                                                    p[k].buff_time[p[k].buff_Size] = 15;
                                                    p[k].buff_id[p[k].buff_Size] = 14;
                                                    p[k].buff_Size++;
                                                }
                                                else if (p[k].hp <= 0 && p[k].buff_check[14] == 1) {
                                                    *wofs << L"(T" << b_m[b_mid].time << L")" << p[k].name << L"退出戰鬥" << endl;
                                                    if (p[k].buff_check[20] > 0) {
                                                        p[k].buff_check[20] = 0;
                                                        for (int lk = 0; lk < b_m[b_mid].esize; lk++) {
                                                            if (e[lk].buff_check[20] > 0) {
                                                                e[lk].buff_check[20] = 0;
                                                            }
                                                        }
                                                    }
                                                    te[p[k].x][p[k].y].mA -= 10000;
                                                    te[p[k].x][p[k].y].mB -= 10000;
                                                    int x = p[k].x, y = p[k].y;
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
                                                    te[p[k].x][p[k].y].player = 0;
                                                    p[k].x = -1; p[k].y = -1;
                                                }
                                            }
                                            else if (te[e[I].x + sx][e[I].y + sy].type == 0) {
                                                te[e[I].x + sx][e[I].y + sy].fire = 1;
                                                te[e[I].x + sx][e[I].y + sy].fire_time = 60;
                                            }


                                    }
                                    for (int k = 0; k < b_m[b_mid].nsize; k++) {
                                        if (b_n[k].x == e[I].x + sx && b_n[k].y == e[I].y + sy && te[e[I].x + sx][e[I].y + sy].type == 0) {
                                            int ro = roll("3d3", 1);
                                            if (b_n[k].dex  >= roll("", 2)) {
                                                ro /= 2;
                                            }
                                            ro += b_n[k].buff_check[12] * 2;
                                            nHP(b_n, b_m,k,b_mid, 0, ro);
                                            *wofs << L"(T" << b_m[b_mid].time << L")" << e[I].name << L"用炎浪對" << b_n[k].name << L"造成" << ro << L"點傷害" << endl;
                                            te[e[I].x + sx][e[I].y + sy].fire = 1;
                                            te[e[I].x + sx][e[I].y + sy].fire_time = 60;
                                            if (b_n[k].hp <= 0) {
                                                te[b_n[k].x][b_n[k].y].mA -= 10000;
                                                te[b_n[k].x][b_n[k].y].mB -= 10000;
                                                int x = b_n[k].x, y = b_n[k].y;
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
                                                te[b_n[k].x][b_n[k].y].npc = 0;
                                                b_n[k].x = -1; b_n[k].y = -1;
                                                b_n[k].speed = -1;
                                            }
                                        }
                                        else if (te[e[I].x + sx][e[I].y + sy].type == 0) {
                                            te[e[I].x + sx][e[I].y + sy].fire = 1;
                                            te[e[I].x + sx][e[I].y + sy].fire_time = 60;
                                        }
                                    }                                   
                                    }
                                }
                            }
                            IMAGE sb;
                            loadimage(&sb, L"./Game/picture/b_skill_block.png", 0, 0, false);
                            transparentimage(NULL, 400, 20, &sb);
                            RECT r = { 400, 20, 600, 70 };
                            settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                            drawtext(L"炎浪", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                            IMAGE get, ef;
                            getimage(&get, 0, 0, 1296, 960);
                            mciSendString(L"close se", NULL, 0, NULL);
                            mciSendString(L"open ./Game/Sound/SE/火炎魔法3.mp3 alias se", NULL, 0, NULL);
                            wstring vos = L"setaudio se volume to " + to_wstring(soundSize);
                            mciSendString(vos.c_str(), NULL, 0, NULL);
                            mciSendString(L"play se from 0", NULL, 0, NULL);
                            loadimage(&ef, L"./Game/picture/魔・炎2.png", 0, 0, false);
                            for (int ej = 0; ej < 5; ej++) {
                                for (int ei = 0; ei < 3; ei++) {
                                    putimage(0, 0, &get);
                                    transparentimageA(NULL, 48 * e[I].x - b_m[b_mid].ox - 40, e[I].y * 48 - b_m[b_mid].oy - 60, &ef, ei * 150, ej * 130, 150, 130);
                                    FlushBatchDraw();
                                    Sleep(100);
                                }
                            }
                            putimage(0, 0, &get);
                            FlushBatchDraw();
                            Sleep(500);
                        }
                        e[I].buff_check[e[I].buff_id[q]]--;
                        for (int E = q; E < e[I].buff_Size - 1; E++) {
                            e[I].buff_time[E] = e[I].buff_time[E + 1];
                            e[I].buff_id[E] = e[I].buff_id[E + 1];
                        }
                        e[I].buff_Size--;
                        q--;
                    }
                }
            }

        }
    }
    else if (chose == "r") {
            if (e[id].buff_check[3] > 0) {
                e[id].move /= 2;
                e[id].buff_check[29]++;
            }
            if (e[id].buff_check[11] > 0&& e[id].buff_state[11]==1) {
                if (e[id].hp + e[id].mhp / 20 < e[id].mhp) {
                    e[id].hp += e[id].mhp / 20;
                }
                else {
                    e[id].hp = e[id].mhp;
                }
            }
            if (e[id].buff_check[12] >0 ) {
                settextcolor(RGB(255, 0, 0));
                settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                if (e[id].mhp * e[id].buff_check[12] / 20 < 1) {
                    outtextxy(e[id].x * 48 - b_m[b_mid].ox, e[id].y * 48 - b_m[b_mid].oy - 40, to_wstring(-1).c_str());
                    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                    settextcolor(RGB(0, 0, 0));
                    *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"受到" << 1 << L"點燒灼傷害" << endl;
                    e[id].hp -= 1;
                }
                else {
                outtextxy(e[id].x * 48 - b_m[b_mid].ox, e[id].y * 48 - b_m[b_mid].oy - 40, to_wstring(-e[id].mhp*e[id].buff_check[12]/20).c_str());
                settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                settextcolor(RGB(0, 0, 0));
                *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"受到" << e[id].mhp * e[id].buff_check[12] / 20 << L"點燒灼傷害" << endl;
                int ro = (e[id].mhp) * e[id].buff_check[12] / 20;
                e[id].hp -= ro;
                }
                e[id].buff_check[12]--;
                FlushBatchDraw();
                Sleep(500);
                if (e[id].hp <= 0) {
                    te[e[id].x][e[id].y].mA -= 10000;
                    te[e[id].x][e[id].y].mB -= 10000;
                    int x = e[id].x, y = e[id].y;
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
                    te[e[id].x][e[id].y].enemy = 0;
                    e[id].x = -1; e[id].y = -1;
                    e[id].speed = -1;
                }
            }
            if (e[id].buff_check[20] > 0) {
                e[id].move = 0;
            }
            if (e[id].buff_check[27] > 0) {
                if (e[id].move >= 5) {
                    e[id].move -= 5;
                }
                else {
                    e[id].move = 0;
                }
            }
    }
    else if (chose == "s") {
         for (I = 0; I < b_m[b_mid].esize; I++) {
             if (e[I].buff_state[11] == 0) {
                 if (te[e[I].x][e[I].y].fire == 1) {
                     e[I].str += 2;
                     e[I].dex -= 1;
                     e[I].buff_state[11] = 1;
                 }
              }
             else if (e[I].buff_state[11] == 1) {
                 if (te[e[I].x][e[I].y].fire == 0) {
                     e[I].str -= 2;
                     e[I].dex += 1;
                     e[I].buff_state[11] = 0;
                 }
             }
         }
    }
}
void cd_check(string chose, player* p, enemy* e, b_npc* b_n, b_map* b_m,int b_mid,int sk_id,int P_id) {
    if (chose == "t") {
        for (int I = 0; I < b_m[b_mid].esize; I++) {
            for (int J = 0; J < 10; J++) {
                if (e[I].cd[J] > 0) {
                    e[I].cd[J]--;
                }
            }
        }
        for (int I = 0; I < b_m[b_mid].psize; I++) {
            for (int S = 0; S < sk_id; S++) {
                if (p[I].s_cd[S] > 0) {
                    p[I].s_cd[S]--;
                }
            }
        }
    }
    else if (chose == "p") {
            for (int S = 0; S < sk_id; S++) {
                if (p[P_id].r_cd[S] > 0) {
                    p[P_id].r_cd[S]--;
                }
            }
    }
}
void te_check(wofstream* wofs, string chose,player *p,enemy*e,b_npc *b_n,b_map *b_m, terrain(*te)[50],int b_mid,int P_id,int id,int b_nid){
    if (chose == "t") {
        if (b_m[b_mid].type == 1) {
            for (int I = 0; I < b_m[b_mid].x; I++) {
                for (int J = 0; J < b_m[b_mid].y; J++) {
                    if (te[I][J].dark == 2|| te[I][J].dark == 3) {
                        te[I][J].dark = 1;
                    }
                }
            }
        for (int I = 0; I < b_m[b_mid].psize; I++) {
            for (int fI = -2; fI <= 2; fI++) {
                for (int fJ = -2; fJ <= 2; fJ++) {
                    if (te[p[I].x + fI][p[I].y + fJ].dark == 1) {
                        te[p[I].x + fI][p[I].y + fJ].dark = 2;
                    }
                }
            }
        }
        }
        for (int I = 0; I < b_m[b_mid].x; I++) {
            for (int J = 0; J < b_m[b_mid].y; J++) {                
                if (te[I][J].fire == 1) {
                    te[I][J].fire_time--;
                    if (te[I][J].fire_time == 0) {
                        te[I][J].fire = 0;
                    }
                    else if(b_m[b_mid].type==1) {
                        for (int fi = -3; fi <=3; fi++) {
                            for (int fj = -3; fj <=3; fj++) {
                                if ((te[I+fi][J+fj].dark ==1|| te[I + fi][J + fj].dark == 2) &&(abs(fi)+abs(fj))<=3) {
                                    te[I+fi][J+fj].dark = 3;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (chose == "p") {
        if (te[p[P_id].x][p[P_id].y].fire == 1) {
            p[P_id].buff_check[12]++;
        }
        if (te[p[P_id].x][p[P_id].y].trap == 2) {
            p[P_id].move = 0;
            p[P_id].buff_check[27]++;
            te[p[P_id].x][p[P_id].y].trap = 0;
            *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name<<L"觸發狩獵陷阱" << endl;
            int DMG = 5;
            pHP(p, b_m, P_id, b_mid, 0, DMG);
            FlushBatchDraw();
            Sleep(500);
            if (p[P_id].hp <= 0 && p[P_id].buff_check[14] == 0) {
                p[P_id].buff_check[14] = 1;
                p[P_id].buff_time[p[P_id].buff_Size] = 15;
                p[P_id].buff_id[p[P_id].buff_Size] = 14;
                p[P_id].buff_Size++;
            }
        }
    }
    else if (chose == "e") {
        if (te[e[id].x][e[id].y].fire == 1) {
            if (e[id].buff_check[11] == 1) {

            }
            else {
                e[id].buff_check[12]++;
            }
        }
        if (te[e[id].x][e[id].y].trap == 2) {
            e[id].move = 0;
            e[id].buff_check[27]++;
            te[e[id].x][e[id].y].trap = 0;
            *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"觸發狩獵陷阱" << endl;
            int DMG = 5;
            eHP(e, b_m, id, b_mid, 0, DMG);
            FlushBatchDraw();
            Sleep(500);
            if (e[id].hp <= 0) {
                te[e[id].x][e[id].y].mA -= 10000;
                te[e[id].x][e[id].y].mB -= 10000;
                int x = e[id].x, y = e[id].y;
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
                te[e[id].x][e[id].y].enemy = 0;
                e[id].x = -1; e[id].y = -1;
                e[id].speed = -1;
                if (e[id].buff_check[20] > 0) {
                    p[0].buff_check[20] = 0;
                    e[id].buff_check[20] = 0;
                }
                return;
            }
        }
    }
    else if (chose == "n") {
        if (te[b_n[b_nid].x][b_n[b_nid].y].fire == 1) {
            b_n[b_nid].buff_check[12]++;
        }
        if (te[b_n[b_nid].x][b_n[b_nid].y].trap == 2) {
            b_n[b_nid].move = 0;
            b_n[b_nid].buff_check[27]++;
            te[b_n[b_nid].x][b_n[b_nid].y].trap = 0;
            *wofs << L"(T" << b_m[b_mid].time << L")" << b_n[b_nid].name << L"觸發狩獵陷阱" << endl;
            int DMG = 5;
            nHP(b_n, b_m, b_nid, b_mid, 0, DMG);
            FlushBatchDraw();
            Sleep(500);
            if (b_n[b_nid].hp <= 0) {
                te[b_n[b_nid].x][b_n[b_nid].y].mA -= 10000;
                te[b_n[b_nid].x][b_n[b_nid].y].mB -= 10000;
                int x = b_n[b_nid].x, y = b_n[b_nid].y;
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
                te[b_n[b_nid].x][b_n[b_nid].y].npc = 0;
                b_n[b_nid].x = -1; b_n[b_nid].y = -1;
                b_n[b_nid].speed = -1;
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
void m_map(player* p, Map* m, BOX* Box, int m_id, int b_id, npc* n, string g);
void show(Map* m, player* p, enemy* e, e_npc* e_n, BOX* Box, int& m_id, int b_id, npc* n,flag *f, string g);
void n_put(npc *n,Map *M,player *p,int m_id);
void  m_set(Map* m, npc* n, player* p,e_npc *e_n ,BOX *Box,int m_id,int b_id);
void readeventjson(player *p,npc *n,flag *f,Map *m,BOX *Box,task *tk,m_flag *m_f,item *it,stone *st,const char* filename,int &m_id,int b_id) {
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    IMAGE t_block, a1, tri, c1,mmp,p1,p2,p3,p4,ma1_0,ma1_1,ma1_2,ma2_0, ma2_1, ma2_2, ma3_0, ma3_1, ma3_2, ma4_0, ma4_1, ma4_2,ef_1,ef_2,ef_3,ef_4,Get,can_0,can_1,ene1,ene2,ene3,ene4,get;
    RECT t={50,696+5,210,696+60+5};
    time_t bom=time(NULL),ts=time(NULL);
    int ui=0, uj=0, uk=0,uK=0,ei=0,ej=0,pa=0,Ei=0,Ej=0,ep=0,EI=0,EJ=0,po=0,uI=0, numFrames;
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
        if (root.isMember("mType")) {
            mm = L"./Game/picture/Mmap_" + to_wstring(root["mType"].asInt()) + L".png";
            loadimage(&mmp, mm.c_str(), 0, 0, false);
            if (root["mType"].asInt() == 2) {
                loadimage(&ma1_0, L"./Game/picture/馬1_0.png", 0, 0, false);
                loadimage(&ma1_1, L"./Game/picture/馬1_1.png", 0, 0, false);
                loadimage(&ma1_2, L"./Game/picture/馬1_2.png", 0, 0, false);
                loadimage(&ma2_0, L"./Game/picture/馬2_0.png", 0, 0, false);
                loadimage(&ma2_1, L"./Game/picture/馬2_1.png", 0, 0, false);
                loadimage(&ma2_2, L"./Game/picture/馬2_2.png", 0, 0, false);
                loadimage(&ma3_0, L"./Game/picture/死馬.png", 0, 0, false);
                loadimage(&ene1, L"./Game/picture/enemy0.png", 0, 0, false);
                loadimage(&ene2, L"./Game/picture/enemy3.png", 0, 0, false);
                loadimage(&ef_1, L"./Game/picture/技・炎1.png", 0, 0, false);
                loadimage(&p1, L"./Game/picture/p2.png", 0, 0, false);
                loadimage(&ef_2, L"./Game/picture/鎌1.png", 0, 0, false);
                loadimage(&p2, L"./Game/picture/死狼.png", 0, 0, false);
                wstring sPath = L"open ./Game/Sound/SE/馬が走る1.mp3 alias ma";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                wstring vos = L"setaudio ma volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                vos = L"play ma repeat";
                mciSendString(vos.c_str(), NULL, 0, NULL);
                sPath = L"open ./Game/Sound/SE/eSe0.mp3 alias woo";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                vos = L"setaudio woo volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                sPath = L"open ./Game/Sound/SE/火炎魔法2.mp3 alias fir";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                vos = L"setaudio fir volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                sPath = L"open ./Game/Sound/SE/eSe2.mp3 alias sss";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                vos = L"setaudio sss volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                sPath = L"open ./Game/Sound/SE/剣の素振り1.mp3 alias sword";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                vos = L"setaudio sword volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                sPath = L"open ./Game/Sound/SE/打撃8.mp3 alias blood";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                vos = L"setaudio blood volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
            }
            else if (root["mType"].asInt() == 3) {
                loadimage(&ef_1, L"./Game/picture/shot.png", 0, 0, false);
                loadimage(&ef_2, L"./Game/picture/爆発2.png", 0, 0, false);
                loadimage(&ef_3, L"./Game/picture/msg.png", 0, 0, false);
                wstring sPath = L"open ./Game/Sound/SE/爆発1.mp3 alias boom";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                wstring vos = L"setaudio boom volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);               
                sPath = L"open ./Game/Sound/SE/重機関銃を乱射1.mp3 alias shoot";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                vos = L"setaudio shoot volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play shoot repeat", NULL, 0, NULL);
            }
            else if (root["mType"].asInt() == 4) {
                mciSendString(L"close shoot", NULL, 0, NULL);
                loadimage(&p1, L"./Game/picture/npc10.png", 0, 0, false);
                loadimage(&p2, L"./Game/picture/p2.png", 0, 0, false);
                loadimage(&p3, L"./Game/picture/npc11.png", 0, 0, false);
                wstring sPath = L"open ./Game/Sound/SE/爆発1.mp3 alias boom";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                wstring vos = L"setaudio boom volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
            }
            else if (root["mType"].asInt() == 5) {
                loadimage(&ma1_0, L"./Game/picture/巴蘭斯騎兵0.png", 0, 0, false);
                loadimage(&ma1_1, L"./Game/picture/巴蘭斯騎兵1.png", 0, 0, false);
                loadimage(&ma1_2, L"./Game/picture/巴蘭斯騎兵2.png", 0, 0, false);
                loadimage(&ma2_0, L"./Game/picture/路易斯1.png", 0, 0, false);
                loadimage(&ma2_1, L"./Game/picture/路易斯2.png", 0, 0, false);
                loadimage(&ma2_2, L"./Game/picture/路易斯3.png", 0, 0, false);
                loadimage(&ma3_0, L"./Game/picture/馬3_0.png", 0, 0, false);
                loadimage(&ma3_1, L"./Game/picture/馬3_1.png", 0, 0, false);
                loadimage(&ma3_2, L"./Game/picture/馬3_2.png", 0, 0, false);
                loadimage(&ma4_0, L"./Game/picture/馬2_0.png", 0, 0, false);
                loadimage(&ma4_1, L"./Game/picture/馬2_1.png", 0, 0, false);
                loadimage(&ma4_2, L"./Game/picture/馬2_2.png", 0, 0, false);
                loadimage(&p3, L"./Game/picture/npc11.png", 0, 0, false);
                wstring sPath = L"open ./Game/Sound/SE/馬が走る1.mp3 alias ma";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                wstring vos = L"setaudio ma volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
            }
            else if (root["mType"].asInt() == 6) {
                loadimage(&can_0, L"./Game/picture/炮兵_0.png", 0, 0, false);
                loadimage(&can_1, L"./Game/picture/炮兵_1.png", 0, 0, false);
                loadimage(&p1, L"./Game/picture/npc14.png", 0, 0, false);
                loadimage(&p2, L"./Game/picture/npc15.png", 0, 0, false);
                wstring sPath = L"open ./Game/Sound/SE/大砲2.mp3 alias boom";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                wstring vos = L"setaudio boom volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
            }
            else if (root["mType"].asInt() == 7) {
                loadimage(&p1, L"./Game/picture/p0.png", 0, 0, false);
                loadimage(&p2, L"./Game/picture/p1.png", 0, 0, false);
            }
            else if (root["mType"].asInt() == 8) {
                loadimage(&p1, L"./Game/picture/p0.png", 0, 0, false);
                loadimage(&p2, L"./Game/picture/p1.png", 0, 0, false);
                loadimage(&p3, L"./Game/picture/p2.png", 0, 0, false);
                loadimage(&ene1, L"./Game/picture/enemy0.png", 0, 0, false);
                loadimage(&ene2, L"./Game/picture/enemy3.png", 0, 0, false);
                loadimage(&ene4, L"./Game/picture/enemy4.png", 0, 0, false);
                loadimage(&ene3, L"./Game/picture/死狼.png", 0, 0, false);
                loadimage(&ef_1, L"./Game/picture/武器・剣4.png", 0, 0, false);
                loadimage(&ef_2, L"./Game/picture/武器・ツメ2.png", 0, 0, false);
                loadimage(&ef_3, L"./Game/picture/mask.png", 0, 0, false);
                loadimage(&ef_4, L"./Game/picture/技・炎1_2.png", 0, 0, false);
                wstring sPath = L"open ./Game/Sound/SE/eSe0.mp3 alias woo";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                wstring vos = L"setaudio woo volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                sPath = L"open ./Game/Sound/SE/剣で斬る4.mp3 alias sword";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                vos = L"setaudio sword volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                sPath = L"open ./Game/Sound/SE/打撃8.mp3 alias blood";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                vos = L"setaudio blood volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                sPath = L"open ./Game/Sound/SE/斧で斬る1.mp3 alias zua";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                vos = L"setaudio zua volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                sPath = L"open ./Game/Sound/SE/火炎魔法2.mp3 alias fir";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                vos = L"setaudio fir volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                sPath = L"open ./Game/Sound/SE/重力魔法2.mp3 alias zun";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                vos = L"setaudio zun volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
            }
            else if (root["mType"].asInt() == 9) {
            EJ = 4;
            loadimage(&p1, L"./Game/picture/p0.png", 0, 0, false);
            loadimage(&p2, L"./Game/picture/p1.png", 0, 0, false);
            loadimage(&p3, L"./Game/picture/alice_sleep.png", 0, 0, false);
            loadimage(&p4, L"./Game/picture/eba.png", 0, 0, false);
            loadimage(&ene1, L"./Game/picture/enemy0.png", 0, 0, false);
            loadimage(&ene2, L"./Game/picture/enemy3.png", 0, 0, false);
            loadimage(&ene4, L"./Game/picture/enemy4.png", 0, 0, false);
            loadimage(&ene3, L"./Game/picture/死狼.png", 0, 0, false);
            loadimage(&ef_1, L"./Game/picture/shot.png", 0, 0, false);
            loadimage(&ef_2, L"./Game/picture/shot_2.png", 0, 0, false);
            wstring sPath = L"open ./Game/Sound/SE/拳銃2.mp3 alias shot";
            mciSendString(sPath.c_str(), NULL, 0, NULL);
            wstring vos = L"setaudio shot volume to " + to_wstring(soundSize);
            mciSendString(vos.c_str(), NULL, 0, NULL);
            sPath = L"open ./Game/Sound/SE/eSe0.mp3 alias woo";
            mciSendString(sPath.c_str(), NULL, 0, NULL);
             vos = L"setaudio woo volume to " + to_wstring(soundSize);
            mciSendString(vos.c_str(), NULL, 0, NULL);
            }
            else if (root["mType"].asInt() == 10) {
            loadimage(&p1, L"./Game/picture/p0.png", 0, 0, false);
            loadimage(&p2, L"./Game/picture/p1.png", 0, 0, false);
            loadimage(&p3, L"./Game/picture/p3.png", 0, 0, false);
            }
        }
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
            putimage(0, 0, 1296, 960, &mapP, m[m_id].ox, m[m_id].oy);
            n_put(n, m, p, m_id);
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
                settextstyle(23, 0, _T("Taipei Sans TC Beta"));
                LPCTSTR path = n[root["talk"][k]["npc"].asInt()].name.c_str();
                drawtext(path, &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                IMAGE p1;
                if (root["talk"][k]["npc"].asInt() != 0) {
                    mm = std::to_wstring(root["talk"][k]["npc"].asInt());
                    mm = L"./Game/picture/t_npc" + mm + L"_" + std::to_wstring(root["talk"][k]["face"].asInt()) + L".png";
                    LPCTSTR path = mm.c_str();
                    loadimage(&p1, path, 0, 0, false);
                    transparentimage(NULL, 50, 746, &p1, 0xFF55FF);
                }
            }
            else {
                settextstyle(23, 0, _T("Taipei Sans TC Beta"));
                LPCTSTR path = p[root["talk"][k]["player"].asInt()].name.c_str();
                drawtext(path, &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                IMAGE p1;
                mm = std::to_wstring(root["talk"][k]["player"].asInt());
                mm = L"./Game/picture/player" + mm + L"_" + std::to_wstring(root["talk"][k]["face"].asInt()) + L".png";
                path = mm.c_str();
                loadimage(&p1, path, 0, 0, false);
                transparentimage(NULL, 50, 746, &p1, 0xFF55FF);
            }
            settextcolor(WHITE);
            settextstyle(30, 0, _T("Taipei Sans TC Beta"));
          while (1) {
                int Bu = F / 10;
            BeginBatchDraw();
            if (root.isMember("mType")) {
            if ( root["mType"].asInt()==1 && F % 129 == 0) {
                transparentimage(NULL,1*(F%1295), 0, &mmp, 0xFF55FF,0,0,1295 - 1 * (F % 1295),696);
                transparentimage(NULL, 0, 0, &mmp, 0xFF55FF, 1295-1*(F%1295), 0, 1 * (F % 1295), 696);
                transparentimage(NULL, 14 * 48 , 10* 48-16, &p1, 0xFF55FF, 128*(ubs%2) + 16, 64, 48, 64);
                transparentimage(NULL, 14 * 48, 11 * 48 - 16, &p2, 0xFF55FF, 128 * (ubs % 2) + 16, 64, 48, 64);
                ubs++;
            }
            else if (root["mType"].asInt() == 2 && F % 30 == 0) {      
                putimage(0, 0, 1296, 696, &mmp, ui * 1296 + 48 * uk, uj * 960 + 48 * uK);
                if (k == 4&&ep==0) {
                    pa = 1;
                }
                if (k == 5&&ubs!=15) {
                    pa = 1;
                }
                if (k == 6 && w >= s.size() - 1 && pa == 0) {                   
                    pa = 1;
                }
                if (ep == 0) {
                if (uj!=1&&uK==0) {
                if ( uk % 3 == 1) {
                transparentimage(NULL, 528, 528-96, &ma1_1, 0xFF55FF);
                }
                else if ( uk % 3 == 2) {
                    transparentimage(NULL, 528, 528 - 96, &ma1_2, 0xFF55FF);
                }
                else if (uk % 3 == 0) {
                    transparentimage(NULL, 528, 528 - 96, &ma1_0, 0xFF55FF);
                }
                }
                else {
                    if (uK % 3 == 1) {
                        transparentimage(NULL, 528, 528 - 96, &ma2_1, 0xFF55FF);
                    }
                    else if (uK % 3 == 0) {
                        transparentimage(NULL, 528, 528 - 96, &ma2_2, 0xFF55FF);
                       
                    }
                    else if (uK % 3 == 2) {
                        transparentimage(NULL, 528, 528 - 96, &ma2_0, 0xFF55FF); Sleep(100);
                    }
                }
                if (uk == 26 && (ui == 0 || uj == 2)) {
                    ui++; uk = 0;
                }
                else if (ui == 0 || uj == 2) {
                    uk++;
                }
                if (uK == 19 && ui == 1 && uj < 2) {
                    uj++; uK = 0;
                }
                else if (ui == 1 && uj < 2) {
                    uK++;
                }
                if (ui == 2 && uj == 2) {
                    ui = 0; uj = 0; uk = 0; uK = 0;
                    if (k == 4) {
                        ep = 1;      
                        mciSendString(L"stop ma", NULL, 0, NULL);
                    }
                }
                if (k >2 ) {
                    if (uj != 1 && uK == 0) {
                        if (Ei >0) {
                            Ei -= 2;
                        }
                        if (Ej >0) {
                            Ej--;
                        }
                    if (uk % 3 == 0) {
                        transparentimage(NULL, 4 * 48+Ei*16, 10 * 48 - 16-Ej*16 - 30, &ene1, 0xFF55FF, 64 + 48+2, 128, 48, 64);
                        transparentimage(NULL, 3 * 48 + Ei * 16, 9* 48 - 16 - Ej * 16 - 30, &ene1, 0xFF55FF, 64 + 48 + 2, 128, 48, 64);

                    }
                    else if (uk % 3 == 1) {
                        transparentimage(NULL, 4 * 48 + Ei * 16, 10 * 48 - 16 - Ej * 16 - 30, &ene1, 0xFF55FF, 64+2, 128, 48, 64);
                        transparentimage(NULL, 3 * 48 + Ei * 16, 9 * 48 - 16 - Ej * 16 - 30, &ene1, 0xFF55FF, 64 + 2, 128, 48, 64);

                    }
                    else if (uk % 3 == 2) {
                        transparentimage(NULL, 4 * 48 + Ei * 16, 10 * 48 - 16 - Ej * 16 - 30, &ene1, 0xFF55FF, 64 - 48, 128, 48, 64);
                        transparentimage(NULL, 3 * 48 + Ei * 16, 9 * 48 - 16 - Ej * 16 - 30, &ene1, 0xFF55FF, 64 - 48, 128, 48, 64);

                    }
                    ei = 0;
                    ej = 0;
                    }
                    else {                       
                        if (ei <24) {
                            ei+=2;
                        }
                        if (ej < 6) {
                            ej++;
                        }
                        if (uK % 3 == 0) {
                            transparentimage(NULL, 4 * 48+ei*16, 10* 48 - 16-ej*16 - 30, &ene1, 0xFF55FF, 64 + 48 + 2, 0, 48, 64);
                            transparentimage(NULL, 3 * 48 + ei * 16, 9 * 48 - 16 - ej * 16 - 30, &ene1, 0xFF55FF, 64 + 48 + 2, 0, 48, 64);

                        }
                        else if (uK % 3 == 1) {
                            transparentimage(NULL, 4 * 48 + ei * 16, 10 * 48 - 16 - ej * 16 - 30, &ene1, 0xFF55FF, 64 + 2, 0, 48, 64);
                            transparentimage(NULL, 3 * 48 + ei * 16, 9 * 48 - 16 - ej * 16 - 30, &ene1, 0xFF55FF, 64 + 2, 0, 48, 64);

                        }
                        else if (uK % 3 == 2) {
                            transparentimage(NULL, 4 * 48 + ei * 16, 10* 48 - 16 - ej * 16 - 30, &ene1, 0xFF55FF, 64 - 48, 0, 48, 64);
                            transparentimage(NULL, 3 * 48 + ei * 16, 9 * 48 - 16 - ej * 16 - 30, &ene1, 0xFF55FF, 64 - 48, 0, 48, 64);
                        }
                        Ei = 24;
                        Ej = 6;
                    }
                }
                }
                else if (ep == 1) {                   
                    if (k==5&&ubs==15&&pa==0) {
                        transparentimage(NULL, 15 * 16, 11 * 48 - 16 - 30, &ene2, 0xFF55FF, 64 + 48 + 2, 128, 48, 64);
                        transparentimage(NULL, 528, 528 - 96, &ma3_0);
                        transparentimage(NULL, 13*48 ,11 * 48 -16 , &p1, 0xFF55FF, 64 +8, 64, 48, 64);
                    }      
                    if (k == 6) {
                        transparentimage(NULL, 528, 528 - 96, &ma3_0);
                    }
                    if (ubs <= 15&&k<=5&&EJ!=5) {
                    transparentimage(NULL, 528, 528 - 96, &ma1_0, 0xFF55FF);
                    }
                    transparentimage(NULL, 4 * 48 + uI* 16, 10 * 48 - 16 - 30, &ene1, 0xFF55FF, 64 + 48 * (uI % 2) + 2, 128, 48, 64);
                    transparentimage(NULL, 3 * 48 + uI * 16, 9 * 48 - 16 - 30, &ene1, 0xFF55FF, 64 + 48 * (uI % 2) + 2, 128, 48, 64);
                    if (po == 1) {
                        transparentimage(NULL, 25 * 48 + uI * 16, 9 * 48 - 16 - 30, &ene1, 0xFF55FF, 64 +48 * (uI % 2) +2, 128, 48, 64);
                        transparentimage(NULL, 24 * 48 + uI * 16, 10 * 48 - 16 - 30, &ene1, 0xFF55FF, 64 +48 * (uI % 2) +2, 128, 48, 64);
                    }
                    else {
                    transparentimage(NULL, 25 * 48, 9 * 48 - 16 - 30, &ene1, 0xFF55FF, 64 + 16, 64, 48, 64);
                    transparentimage(NULL, 24 * 48, 10 * 48 - 16 - 30, &ene1, 0xFF55FF, 64 + 16, 64, 48, 64);
                    }
                    if (ubs >= 30 && EJ == 3) {
                        transparentimage(NULL, 24 * 48, 11 * 48 - 16 - 10, &p2);
                        transparentimage(NULL, 25 * 48, 12 * 48 - 16 - 10, &p2);
                    }
                    else {
                    transparentimage(NULL, 24 * 48, 11 * 48 - 16 - 30, &ene1, 0xFF55FF, 64 + 16, 64, 48, 64);
                    transparentimage(NULL, 25 * 48, 12 * 48 - 16 - 30, &ene1, 0xFF55FF, 64 + 16, 64, 48, 64);                    
                    }
                    if (k == 4) {
                        pa = 0;
                    }
                    if (k == 5&&pa==1) {
                        transparentimage(NULL, ubs*16, 11 * 48 - 16 - 30, &ene2, 0xFF55FF, 64 + 48 + 2, 128, 48, 64);                     
                        if (ubs < 15) {
                               ubs++;
                        }
                        if (ubs == 15) {
                            if (EI == 0 && EJ == 0) {
                                mciSendString(L"play fir from 0", NULL, 0, NULL);
                                mciSendString(L"play sss from 0", NULL, 0, NULL);
                            }
                            transparentimageA(NULL,48*6+60, 10 * 48-60, &ef_1, EI*320, EJ*120, 320, 120);
                            EI++;                            
                            if (EI == 3) {
                                EJ++;
                                EI = 0;
                            }
                            if (EJ == 5) {
                                pa = 0;
                            }
                        }
                    }               
                    if (k == 6) {
                        transparentimage(NULL, 15 * 16 + uI * 16, 11 * 48 - 16 - 30, &ene2, 0xFF55FF, 64 + 48 * (uI % 2) + 2, 128, 48, 64);
                        if (13 * 48 + ubs * 16 < 1296) {
                            transparentimage(NULL, 13 * 48 + ubs * 16, 11 * 48 - 16, &p1, 0xFF55FF, 64 * (ubs % 3) + 8, 128, 48, 64);
                        }
                        else if (po != 1) {
                            po = 1;
                            uI = 0;
                            mciSendString(L"play woo from 0", NULL, 0, NULL);
                        }
                    }
                    if (pa == 1 && k == 6) {
                        if (ubs < 10 * 3) {
                            ubs+=3;
                            EI = 0; EJ = 0;
                        }
                        else if (ubs == 30&&EJ!=3) {
                            transparentimageA(NULL, 23 * 48+50 , 10 * 48 +10, &ef_2, EI * 80, EJ * 80, 80, 80);
                            if (EI == 0 && EJ == 0) {
                            mciSendString(L"play sword from 0", NULL, 0, NULL);
                            mciSendString(L"play blood from 0", NULL, 0, NULL);
                            }

                            EI++;
                            if (EI == 3) {
                                EJ++;
                                EI = 0;
                            }
                        }
                        else if (ubs >= 30 && EJ == 3&&po==0) {
                            ubs+=3;
                        }
                        else if (po == 1) {
                            uI+=2;
                            if (uI * 16 + 3 * 48 > 1296) {
                                pa = 0;
                            }
                        }
                    }
                }
            }
            else if ( root["mType"].asInt() == 3 && F % 30 == 0) {       
                 Ei++;
                putimage(0,0,&mmp);
                transparentimageA(NULL, 300, 100-10, &ef_1, ui * 300, uj * 120, 300, 120);
                transparentimageA(NULL, 0, 150, &ef_1, ui * 300, uj * 120, 300, 120);
                transparentimageA(NULL, 96+30, 150+96, &ef_1, ui * 300, uj * 120, 300, 120);
                transparentimageA(NULL, 96 + 30+48+10, 150 + 96+48+10, &ef_1, ui * 300, uj * 120, 300, 120);
                transparentimageA(NULL, 96 + 30, 150 + 96+96+20, &ef_1, ui * 300, uj * 120, 300, 120);
                transparentimageA(NULL, 80, 150 + 96 + 48 + 210, &ef_1, ui * 300, uj * 120, 300, 120);
                transparentimageA(NULL, 80+80, 150 + 96 + 48 + 210+80, &ef_1, ui * 300, uj * 120, 300, 120);
                transparentimageA(NULL, 8*48, 6*48, &ef_3, Ei * 120, Ej * 320, 120, 320);
                transparentimageA(NULL, 9 * 48, 0 * 48, &ef_3, Ei * 120, Ej * 320, 120, 320);
                if (time(NULL)-bom>2){                
                    if (ei == 0 && ej == 0) {
                    wstring vos = L"play boom from 0";
                    mciSendString(vos.c_str(), NULL, 0, NULL);
                    }
                transparentimageA(NULL, 1179, 175, &ef_2, ei * 80, ej * 80, 80, 80);
                transparentimageA(NULL, 1184, 608, &ef_2, ei * 80, ej * 80, 80, 80);
                transparentimageA(NULL, 1088, 512, &ef_2, ei * 80, ej * 80, 80, 80);
                    ei++;
                if (ei == 8) {
                    ei = 0; ej++;
                }
                if (ej == 2) {
                    ej = 0; bom = time(NULL);                

                }
                }     
                uj++;
                if (uj == 3) {
                    ui--;
                    uj = 0;
                }  
                if (ui == 0) {
                    ui = 5;;
                }
                if (Ei == 6) {
                    Ei = 0;
                    Ej++;
                }
                if (Ej == 5) {
                    Ej = 0;
                }
                
            }
            else if (root["mType"].asInt() == 4 && F % 30 == 0) {
                if (k < 5) {
                if (time(NULL) - bom > 2) {
                        wstring vos = L"play boom from 0";
                        mciSendString(vos.c_str(), NULL, 0, NULL);
                        bom = time(NULL);
                 }
                }
                putimage(0, 0, &mmp);
                transparentimage(NULL, 21 * 48, 9 * 48 - 16, &p3, 0xFF55FF, 64 + 8, 64, 48, 64);
                transparentimage(NULL, 21 * 48, 6* 48 - 16, &p3, 0xFF55FF, 64 + 8, 64, 48, 64);
                transparentimage(NULL, 21 * 48, 2 * 48 - 16, &p3, 0xFF55FF, 64 + 8, 64, 48, 64);
                if (k == 0) {
                transparentimage(NULL, 23 * 48, 5 * 48 - 16, &p1, 0xFF55FF, 64 + 16, 64, 48, 64);
                }
                else {
                    transparentimage(NULL, 23 * 48, 5 * 48 - 16, &p1, 0xFF55FF, 64 + 8, 192, 48, 64);
                }
                if (k == 1&& ubs==0) {
                    pa = 1;
                }
                if (k == 2 && ubs == 0) {
                    pa = 1;
                }
                 if (k == 4 && w >= s.size() - 1&&ubs!=-1&&pa==0) {
                     pa = 1;
                     ubs = 9;
                 }
                if (k == 5 && ubs == 0) {
                    pa = 1;
                }
                if (k == 16 && w >= s.size() - 1 && ubs != -1 && pa == 0) {
                    pa = 1;
                    ubs = 9;
                }
                if (k > 0&&k<5) {
                    if ((k == 1 && pa == 1)||(k==4&&pa==1)||ubs==-1) {
                 
                    }
                    else {
                    transparentimage(NULL, 23 * 48, 3 * 48 - 16, &p3, 0xFF55FF, 64 + 8, 0, 48, 64);
                    }
                }
                if (k > 1 && k < 5) {
                    if ((k == 2 && pa == 1) || (k == 4 && pa == 1)||ubs==-1) {

                    }
                    else{
                    transparentimage(NULL, 22 * 48, 3 * 48 - 16, &p3, 0xFF55FF, 64 + 8, 0, 48, 64);
                    }
                }
                if (k > 4&&k<17&&ubs!=-1) {
                    if (k == 5 && pa == 1) {

                    }
                    else if (k == 16 && pa == 1) {

                    }
                    else {
                    transparentimage(NULL, 23 * 48, 3 * 48 - 16, &p2, 0xFF55FF, 64 + 8, 0, 48, 64);
                    }
                }

                if (pa == 1 && k == 1 ) {
                    transparentimage(NULL,23*48, 16 * ubs - 16, &p3, 0xFF55FF, 64 * (ubs % 2) + 8, 0, 48, 64);
                    ubs++;
                    if (ubs == 9) {
                        pa = 0;
                    }
                }
                else if (pa == 1 && k == 2) {
                    transparentimage(NULL, 22 * 48, 16 * ubs - 16, &p3, 0xFF55FF, 64 * (ubs % 2) + 8, 0, 48, 64);
                    ubs++;
                    if (ubs == 9) {
                        pa = 0;
                    }
                }
                else if (pa == 1 && k == 4) {
                    transparentimage(NULL, 23 * 48, 16 * ubs - 16, &p3, 0xFF55FF, 64 * (ubs % 2) + 8, 64*3, 48, 64);
                    transparentimage(NULL, 22 * 48, 16 * ubs - 16, &p3, 0xFF55FF, 64 * (ubs % 2) + 8, 64*3, 48, 64);
                    ubs--;
                    if (ubs == 0) {
                        pa = 0;
                        ubs = -1;
                    }
                }
                else if (pa == 1 && k == 5) {
                    transparentimage(NULL, 23 * 48, 16 * ubs - 16, &p2, 0xFF55FF, 64 * (ubs % 2) + 8, 0, 48, 64);
                    ubs++;
                    if (ubs == 9) {
                        pa = 0;
                    }
                }
                else if (pa == 1 && k == 16) {
                    transparentimage(NULL, 23 * 48, 16 * ubs - 16, &p2, 0xFF55FF, 64 * (ubs % 2) + 8, 64 * 3, 48, 64);
                    ubs--;
                    if (ubs == 0) {
                        pa = 0;
                        ubs = -1;
                    }
                }            
}
            else if (root["mType"].asInt() == 5 && F % 30 == 0) {
            putimage(0, 0, &mmp);
            transparentimage(NULL, 21 * 48, 9 * 48 - 16, &p3, 0xFF55FF, 64 + 8, 64, 48, 64);
            transparentimage(NULL, 21 * 48, 6 * 48 - 16, &p3, 0xFF55FF, 64 + 8, 64, 48, 64);
            transparentimage(NULL, 21 * 48, 2 * 48 - 16, &p3, 0xFF55FF, 64 + 8, 64, 48, 64);
            if (k < 5&&pa!=1&&ubs>-1) {
                transparentimage(NULL, 48*14, 48 * 6, &ma2_0, 0xFF55FF);
                transparentimage(NULL, 48 * 18, 48 * 6, &ma1_0, 0xFF55FF);
                transparentimage(NULL, 48 * 18, 48 * 7, &ma1_0, 0xFF55FF);
                transparentimage(NULL, 48 * 16, 48 * 6, &ma1_0, 0xFF55FF);
                transparentimage(NULL, 48*16, 48*7, &ma1_0, 0xFF55FF);
            }
            if (k == 5 && pa != 1&&ei==0) {
                transparentimage(NULL, 48 * 18, 48 * 7, &ma3_0, 0xFF55FF);
            }
            if (k == 4 && w >= s.size() - 1&& ubs >-1 && pa == 0) {
                pa = 1;
                ubs = 16*3;
                wstring vos = L"play ma repeat";
                mciSendString(vos.c_str(), NULL, 0, NULL);
            }
            if (k == 5 && w >= s.size() - 1 && ei==0 && pa == 0) {
                pa = 1;
                ei = 0;
                ubs = 7 * 3;
                wstring vos = L"play ma repeat";
                mciSendString(vos.c_str(), NULL, 0, NULL);
            }
            if (pa == 1 && k == 4) {
                for (ui = 0; (ui*96 +ubs*16<18*48&&ui<10); ui++) {
                    if (ubs % 3 == 0&& ubs * 16 + ui * 96>-96) {
                         transparentimage(NULL, ubs*16+ui*96, 48 * 6, &ma1_0, 0xFF55FF);
                         transparentimage(NULL, ubs*16+ui*96, 48 * 7, &ma1_0, 0xFF55FF);
                    }
                    else if ((ubs % 3 == 1||ubs%3==-1) && ubs * 16 + ui * 96 > -96) {
                        transparentimage(NULL, ubs * 16 + ui * 96, 48 * 6, &ma1_1, 0xFF55FF);
                        transparentimage(NULL, ubs * 16 + ui * 96, 48 * 7, &ma1_1, 0xFF55FF);
                    }
                    else if ((ubs % 3 == 2 || ubs % 3 == -2) && ubs * 16 + ui * 96 > -96) {
                        transparentimage(NULL, ubs * 16 + ui * 96, 48 * 6, &ma1_2, 0xFF55FF);
                        transparentimage(NULL, ubs * 16 + ui * 96, 48 * 7, &ma1_2, 0xFF55FF);
                    }
                }
                if (ubs % 3 == 0) {
                    transparentimage(NULL, ubs * 16-96, 48 * 6, &ma2_0, 0xFF55FF);
                }
                else if (ubs % 3 == 1) {
                    transparentimage(NULL, ubs * 16 - 96, 48 * 6, &ma2_1, 0xFF55FF);
                }
                else if (ubs % 3 == 2) {
                    transparentimage(NULL, ubs * 16 - 96, 48 * 6, &ma2_2, 0xFF55FF);
                }
                ubs--;
                if (ubs == -20*3) {
                    ubs = -1;
                    pa = 0;
                    wstring vos = L"stop ma";
                    mciSendString(vos.c_str(), NULL, 0, NULL);
                }
            }
            if (pa == 1 && k == 5) {               
                if (ei == 15) {
                    if (ubs % 3 == 0) {
                        transparentimage(NULL, 48 * 13 , ubs*16, &ma4_0, 0xFF55FF);
                    }
                    else if (ubs % 3 == 1) {
                        transparentimage(NULL, 48 * 13, ubs * 16, &ma4_1, 0xFF55FF);
                    }
                    else if (ubs % 3 == 2) {
                        transparentimage(NULL, 48 * 13, ubs * 16, &ma4_2, 0xFF55FF);
                    }
                    ubs++;
                }
                if (ei <15) {
                    if (ei % 3 == 0) {
                        transparentimage(NULL, 48 * 18 - ei * 16, 48 * 7, &ma3_0, 0xFF55FF);
                    }
                    else if (ei % 3 == 1) {
                        transparentimage(NULL, 48 * 18 - ei * 16, 48 * 7, &ma3_1, 0xFF55FF);
                    }
                    else if (ei % 3 == 2) {
                        transparentimage(NULL, 48 * 18 - ei * 16, 48 * 7, &ma3_2, 0xFF55FF);
                    }
                    ei++;
                    }
                if (ubs >=3*13 -1) {
                    pa = 0;
                    wstring vos = L"close ma";
                    mciSendString(vos.c_str(), NULL, 0, NULL);
                }
            }
            }
            else if (root["mType"].asInt() == 6 && F % 30 == 0) {
            putimage(0, 0, &mmp);
            transparentimage(NULL, 3 * 48, 8 * 48 - 16, &p1, 0xFF55FF, 64 + 8, 128, 48, 64);
            if ((k > 1 || ei ==-1)&&k!=3&&k!=6&&k<9) {
                transparentimage(NULL, 3 * 48, 6 * 48 - 16, &p2, 0xFF55FF, 64 + 8, 0, 48, 64);
            }
            else if (k == 3&&pa==0) {
                transparentimage(NULL, 3 * 48, 6 * 48 - 16, &p2, 0xFF55FF, 64 + 8, 125, 48, 64);
            }
            else if (k == 6 && pa == 0) {
                transparentimage(NULL, 3 * 48, 6 * 48 - 16, &p2, 0xFF55FF, 64 + 8, 0, 48, 64);
            }
            else if (k == 9 && pa == 0&&ei!=-3) {
                transparentimage(NULL, 3 * 48, 6 * 48 - 16, &p2, 0xFF55FF, 64 + 8, 0, 48, 64);
            }
            if (k == 1 && pa == 0&&ei==0) {
                pa = 1;
            }
            if (k == 6 && w >= s.size() - 1 && ei == -1 && pa == 0) {
                pa = 1;            
                ei = 6;
            }
            if (k == 9 && w >= s.size() - 1 && ei == -2 && pa == 0) {
                ei = 0;
                pa = 1;
            }
            if (k == 1 && pa == 1) {
                transparentimage(NULL, 3 * 48, 4 * 48 - 16+ei*16, &p2, 0xFF55FF, 64 * (ei % 3) + 8, 0, 48, 64);
                ei++;
                if (ei == 6) {
                    pa = 0;
                    ei = -1;
                }
            }
            if (k == 6 && pa == 1&&ei!=-2) {
                transparentimage(NULL, 3 * 48, 4 * 48 - 16 + ei * 16, &p2, 0xFF55FF, 64 * (ei % 3) + 8, 192, 48, 64);
                ei--;
                if (ei == 0) {
                    ei = -2;
                    ts = time(NULL);
                }
            }
            else if (k == 6 && pa == 1 && ei == -2) {
                if (time(NULL) - ts > 3) {
                    transparentimage(NULL, 3 * 48, 4 * 48 - 16 + ej * 16, &p2, 0xFF55FF, 64 * (ej % 3) + 8, 0, 48, 64);
                    ej++;
                    if (ej == 6) {
                        ej = 0;
                        pa = 0;
                    }
                }
            }
            if (k == 9 && pa == 1) {
                transparentimage(NULL, 3 * 48, 6 * 48 - 16 - ei * 16, &p2, 0xFF55FF, 64 * (ei % 3) + 8, 192, 48, 64);
                ei++;
                if (ei == 6) {
                    pa = 0;
                    ei = -3;
                }
            }
            if (k > 4) {
            if (ubs % 35 >5) {
            transparentimage(NULL, 48 * 15, 48 *0, &can_0, 0xFF55FF);
            transparentimage(NULL, 48 * 15, 48 * 4, &can_0, 0xFF55FF);
            transparentimage(NULL, 48 * 15, 48 * 8, &can_0, 0xFF55FF);
            transparentimage(NULL, 48 * 11, 48 * 1, &can_0, 0xFF55FF);
            transparentimage(NULL, 48 * 11, 48 * 5, &can_0, 0xFF55FF);
            transparentimage(NULL, 48 * 11, 48 * 9, &can_0, 0xFF55FF);
            }
            else {
                if (time(NULL) - bom > 2) {
                    wstring vos = L"play boom from 0";
                    mciSendString(vos.c_str(), NULL, 0, NULL);
                    bom = time(NULL);
                }
                transparentimage(NULL, 48 * 15, 48 * 0, &can_1);
                transparentimage(NULL, 48 * 15, 48 * 4, &can_1);
                transparentimage(NULL, 48 * 15, 48 * 8, &can_1);
                transparentimage(NULL, 48 * 11, 48 * 1, &can_1);
                transparentimage(NULL, 48 * 11, 48 * 5, &can_1);
                transparentimage(NULL, 48 * 11, 48 * 9, &can_1);
            }
            ubs++;
            }
            else {
                transparentimage(NULL, 48 * 15, 48 * 0, &can_0, 0xFF55FF);
                transparentimage(NULL, 48 * 15, 48 * 4, &can_0, 0xFF55FF);
                transparentimage(NULL, 48 * 15, 48 * 8, &can_0, 0xFF55FF);
                transparentimage(NULL, 48 * 11, 48 * 1, &can_0, 0xFF55FF);
                transparentimage(NULL, 48 * 11, 48 * 5, &can_0, 0xFF55FF);
                transparentimage(NULL, 48 * 11, 48 * 9, &can_0, 0xFF55FF);
            }
            }
            else if (root["mType"].asInt() == 7 && F % 30 == 0) {
            putimage(0, 0, &mmp);
            if (k == 1 && w >= s.size() - 1 && ubs != -1 && pa == 0) {
                pa = 1;
            }
            if (k >= 0 & ubs == 0) {
                transparentimage(NULL, 10 * 48, 11 * 48 - 16, &p1, 0xFF55FF, 64 + 8, 128, 48, 64);
                transparentimage(NULL, 10 * 48, 10 * 48 - 16, &p2, 0xFF55FF, 64 + 8, 128, 48, 64);
            }
            if (pa == 1 && k == 1) {
                transparentimage(NULL, 10 * 48+ubs*16, 11 * 48 - 16, &p1, 0xFF55FF, 64*(ubs%3) + 8, 128, 48, 64);
                transparentimage(NULL, 10 * 48+ubs*16, 10 * 48 - 16, &p2, 0xFF55FF, 64*(ubs%3) + 8, 128, 48, 64);
                ubs++;
                if (ubs * 16 + 10 * 48 > 1296) {
                    pa = 0;
                }
            }
            }
            else if (root["mType"].asInt() == 8 && F % 30 == 0) {
            putimage(0, 0, &mmp);
            if (k == 0 && ubs == -1) {
                transparentimage(NULL, 13.5 * 48, 5 * 48 - 16 , &p3, 0xFF55FF, 64  + 8, 0, 48, 64);
                transparentimage(NULL, 12 * 48, 10 * 48 , &ene1, 0xFF55FF, 64+ 2, 192, 48, 64);
                transparentimage(NULL, 15 * 48, 10 * 48, &ene1, 0xFF55FF, 64  + 2, 192, 48, 64);
                transparentimage(NULL, 10* 48, 11 * 48 - 16, &ene4, 0xFF55FF, 64 + 2, 192, 48, 64);
                transparentimage(NULL, 17 * 48, 12 * 48 - 16, &ene4, 0xFF55FF, 64 + 2, 192, 48, 64);
                transparentimage(NULL, 13.5 * 48, 13 * 48 - 16, &ene2, 0xFF55FF, 64 + 2, 192, 48, 64);
            }
            else if(k>0) {
                transparentimage(NULL, 13.5 * 48, 5 * 48 - 16 , &p3, 0xFF55FF, 64 + 8, 0, 48, 64);
                if (k > 3) {
                    transparentimage(NULL, 10 * 48, 11 * 48 - 16, &ene4, 0xFF55FF, 64 + 2, 128, 48, 64);
                    transparentimage(NULL, 17 * 48, 12 * 48 - 16, &ene4, 0xFF55FF, 64 + 16 - 48, 64, 48, 64);
                }
                else if (k > 0) {
                transparentimage(NULL, 10 * 48, 11 * 48 - 16, &ene4, 0xFF55FF, 64 + 2, 192, 48, 64);
                transparentimage(NULL, 17 * 48, 12 * 48 - 16, &ene4, 0xFF55FF, 64 + 2, 192, 48, 64);
                }
                if (k < 2) {
                transparentimage(NULL, 13.5 * 48, 13 * 48 - 16, &ene2, 0xFF55FF, 64 + 2, 192, 48, 64);
              
                }
                else if (k == 2 && pa == 0 && EI == 4) {
                    transparentimage(NULL, 13.5 * 48 - 5, 8 * 48 - 16, &ene2, 0xFF55FF, 64 + 2, 192, 48, 64);  
                    transparentimageA(NULL, 13 * 48 - 35, 3 * 48 - 10, &ef_4, 3 * 120, 2 * 320, 120, 320);
                }
                else if (k > 2) {
                    transparentimage(NULL, 13.5 * 48-5, 8 * 48 - 16, &ene2, 0xFF55FF, 64 + 2, 192, 48, 64);
                }
                if (k > 2) {
                transparentimage(NULL, 13.5 * 48 , 5 * 48 - 16, &ef_3);
                }
                if (k == 1 && ubs == 12&&pa==0) {
                    transparentimage(NULL, 13 * 48, 6 * 48+16, &ene3);
                    transparentimage(NULL, 14*48, 6*48+16, &ene3);
                }
                else if (k > 1) {
                    transparentimage(NULL, 13*48, 6*48+16, &ene3);
                    transparentimage(NULL, 14 * 48, 6 * 48+16, &ene3);
                }
                if (k > 3) {
                    transparentimage(NULL, 13 * 48, 13 * 48 - 16, &p1, 0xFF55FF, 64 + 8, 192, 48, 64);
                    transparentimage(NULL, 14 * 48, 13 * 48 - 16, &p2, 0xFF55FF, 64 + 8, 192, 48, 64);
                }
            }
            if (k == 0 && pa == 0 && ubs == 0) {
                pa = 1;
            }          
            if (k == 1 && pa == 0 && ubs == 0) {
                mciSendString(L"play woo from 0", NULL, 0, NULL);
                pa = 1;

            }
            if (k == 2 && pa == 0 && ubs == 0) {
                pa = 1;
                EI = 0;
                EJ = 2;
            }
            if (k == 3 && pa == 0 &&EJ == 2&&EI==4) {
                mciSendString(L"play zun from 0", NULL, 0, NULL);
                pa = 1;
            }
            if (k == 0 && pa == 1) {
                transparentimage(NULL, 12 * 48+(ubs/(3*5))*(ubs-15)*6, 13 * 48 - 16 - ubs * 16, &p3, 0xFF55FF, 64 * (ubs % 3) + 8, 192, 48, 64);
                if (ubs >= 15) {
                    transparentimage(NULL, 12 * 48 , 13 * 48-16*(ubs-15), &ene1, 0xFF55FF, 64 + 48 * (ubs % 2) + 2, 192, 48, 64);
                    transparentimage(NULL, 15 * 48, 13 * 48-16*(ubs-15), &ene1, 0xFF55FF, 64 + 48 * (ubs % 2) + 2, 192, 48, 64);
                    transparentimage(NULL, 4 * 48+32*(ubs-15), 11 * 48-16, &ene4, 0xFF55FF, 64 + 48 * (ubs % 2) + 2, 128, 48, 64);
                    transparentimage(NULL, 23 * 48-(ubs-15) * 32, 12 * 48-16, &ene4, 0xFF55FF, 64 + 48 * (ubs % 2) +16-48, 64, 48, 64);
                }
                if(ubs<3*8){
                    ubs++;
                }
                else if(ubs==24) {
                    ubs = -1;
                    pa = 0;
                }
            }
            if (k == 1 && pa == 1) {  
                if (ubs < 12) {
                    ubs++;
                }               
                transparentimage(NULL, 12 * 48+(ubs/6)*(ubs-6)*8, 10 * 48 - 16 * ubs, &ene1, 0xFF55FF, 64 + 2 + 48 * (ubs % 2), 192, 48, 64);
                transparentimage(NULL, 15 * 48 - (ubs / 6) * (ubs - 6) * 8, 10 * 48-16*ubs, &ene1, 0xFF55FF, 64 + 2 + 48 * (ubs % 2), 192, 48, 64);
                if (ubs == 4 * 3) {
                    if (uI == 0 && uK == 0) {
                        mciSendString(L"play sword from 0", NULL, 0, NULL);
                        mciSendString(L"play zua from 0", NULL, 0, NULL);
                        mciSendString(L"play blood from 0", NULL, 0, NULL);
                    }
                    transparentimageA(NULL, 13 * 48 -40, 6 * 48 -10, &ef_1, uI * 120, uK * 120, 120, 120);
                    transparentimageA(NULL, 14 * 48 -40, 6 * 48-10 , &ef_1, uI * 120, uK * 120, 120, 120);
                    transparentimageA(NULL,13.5* 48 - 25, 5* 48 - 16 - 20, &ef_2, uI * 120, uK * 120, 120, 120);
                    uI++;
                    if (uI == 3) {
                        uK++;
                        uI = 0;
                    }
                    if (uK == 5) {
                        pa = 0;
                    }
                }
            }
            if (k == 2 && pa == 1) {
                transparentimage(NULL, 13.5 * 48-5, 13 * 48 - 16-ubs*16, &ene2, 0xFF55FF, 64 + 2 + 48 * (ubs % 2), 192, 48, 64);
                if (ubs < 3 * 5) {
                    ubs++;
                }
                if (ubs == 15) {
                    transparentimageA(NULL, 13 * 48 - 35, 3 * 48 - 10, &ef_4, EI * 120, EJ * 320, 120, 320);
                    if (EJ == 2 && EI == 0) {
                    mciSendString(L"play fir from 0", NULL, 0, NULL);
                    }
                    EJ--;
                    if (EJ == -1) {
                        EJ = 2;
                        EI++;
                    }
                    if (EI == 4) {
                        pa = 0;
                    }
                }
            }
            if (k == 3 && pa == 1) {
                transparentimageA(NULL, 13 * 48 - 35, 3 * 48 - 10, &ef_4, EI * 120, EJ * 320, 120, 320);
                EJ--;
                if (EJ == -1) {
                    EJ = 2;
                    EI++;
                }
                if (EI == 5) {
                    pa = 0;
                }
            }
            }
             else if (root["mType"].asInt() == 9 && F % 30 == 0) {
             putimage(0, 0, &mmp);
             transparentimage(NULL, 17 * 48, 5 * 48 - 16, &ene3);
             transparentimage(NULL, 12 * 48, 8 * 48 - 16, &ene3);
             if (k < 8) {
                 transparentimage(NULL,48*13+16,48*5,&p3);
                 if (k > 2) {
                     transparentimage(NULL, 13 * 48 + 16, 6 * 48 - 16, &p1, 0xFF55FF, 64 + 8, 192, 48, 64);
                 }
             }             
             if (k > 7) {
                 transparentimage(NULL, 13 * 48+16, 5 * 48 - 16, &p4);
             }
             if (k > 0 ) {
                 transparentimage(NULL, 9 * 48, 5 * 48 - 16, &ene3);
                 transparentimage(NULL, 15 * 48, 8 * 48 - 16, &ene3);             
                 transparentimage(NULL, 15 * 48, 5 * 48 - 16, &p2, 0xFF55FF, 64 + 8, 0, 48, 64);                 
             }
             if (k == 1) {
                 transparentimage(NULL, 12 * 48, 5 * 48 - 16, &p1, 0xFF55FF, 64 + 8, 0, 48, 64);
             }
             if (k == 2 && ubs == 0) {
                 transparentimage(NULL, 12 * 48, 5 * 48 - 16 , &p1, 0xFF55FF, 64  + 8, 0, 48, 64);
             }
             if (k == 2 && uk == 4) {
                 transparentimage(NULL, 12 * 48 + uk * 16, 6 * 48 - 16, &p1, 0xFF55FF, 64+ 8, 192, 48, 64);
             }
             if (k == 0 && ei == 5 && EJ == -1) {
                 transparentimage(NULL, 12 * 48, 5 * 48 - 16, &p1, 0xFF55FF, 64 + 8, 0, 48, 64);
                 transparentimage(NULL, 15 * 48, 5 * 48 - 16, &p2, 0xFF55FF, 64 + 8, 0, 48, 64);
                 transparentimage(NULL, 9 * 48, 5 * 48 - 16, &ene3);
                 transparentimage(NULL, 15 * 48, 8 * 48 - 16, &ene3);
             }
             if (k == 1 && ubs == 0) {
                 pa = 1;
             }
             if (k == 2 && s[w] == '^'&&ubs==0) {
                 pa = 1;
                 ubs = 0;
             }
             if (k == 0) {
                 transparentimage(NULL, 13 * 48+15, 8 * 48 , &ene2, 0xFF55FF, 64, 192, 48, 64);
                 transparentimage(NULL, 12 * 48, 10 * 48 - 16, &ene4, 0xFF55FF, 64, 192, 48, 64);
                 transparentimage(NULL, 15 * 48, 10 * 48 - 16, &ene4, 0xFF55FF, 64, 192, 48, 64);
             }
             if (k == 0 && pa == 0 && w == 0) {
                 pa = 1;
             }
             if (k == 0 && pa == 1) {
                 if (EJ == 4 && EI== 0) {
                     mciSendString(L"play shot from 0", NULL, 0, NULL);
                 }
                 transparentimage(NULL, 12 * 48, 5 * 48 - 16, &p1, 0xFF55FF, 64 + 8, 64, 48, 64);
                 transparentimage(NULL, 15 * 48, 5 * 48 - 16, &p2, 0xFF55FF, 64 + 8, 0, 48, 64);
                 transparentimage(NULL, 9 * 48, 5 * 48 - 16, &ene1, 0xFF55FF, 64  , 128, 48, 64);
                 transparentimage(NULL, 15 * 48, 8 * 48 - 16, &ene1, 0xFF55FF, 64 + 8, 192, 48, 64);
                 transparentimageA(NULL, 8 * 48 - 35, 4 * 48+10 , &ef_1, ei * 320, ej * 120, 320, 120);
                 transparentimageA(NULL, 15 * 48 - 35, 6 * 48 - 10, &ef_2, EI * 120, EJ * 300, 120, 300);
                 if (EJ >=0) {
                     EI++;
                 }
                 if (EI == 3) {
                     EI = 0;
                     EJ--;
                 }
                 if (ei < 5) {
                     ej++;
                 }
                 if (ej == 3) {
                     ej = 0;
                     ei++;
                 }
                 if (ei == 5 && EJ ==- 1) {
                     mciSendString(L"play woo from 1", NULL, 0, NULL);
                     pa = 0;
                 }
             }
             if (k == 1 && pa == 1) {
                 if (8 * 48 + ubs * 16 <= 13 * 48) {
                 transparentimage(NULL, 13 * 48 + 15, 8 * 48+ubs*16, &ene2, 0xFF55FF, 48*(ubs%3)+16, 0, 48, 64);
                 }
                 else {
                     pa = 0;
                 }
                 if (10 * 48 - 16 + ubs * 16 <= 13 * 48) {
                 transparentimage(NULL, 12 * 48, 10 * 48 - 16 + ubs * 16, &ene4, 0xFF55FF, 48 * (ubs % 3)+16, 0, 48, 64);
                 transparentimage(NULL, 15 * 48, 10 * 48 - 16 + ubs * 16, &ene4, 0xFF55FF, 48 * (ubs % 3)+16, 0, 48, 64);
                 }
                 ubs++;
             }
             if (k == 2 && pa == 1) {
                 if (ubs <= 3) {
                 transparentimage(NULL, 12 * 48, 5 * 48 - 16+ubs*16, &p1, 0xFF55FF, 64*(ubs%3) + 8, 0, 48, 64);
                 ubs++;
                 }
                 if (ubs == 4) {
                     transparentimage(NULL, 12 * 48+ uk * 16, 6 * 48 - 16 , &p1, 0xFF55FF, 64 * (uk % 3) + 8, 128, 48, 64);
                     uk++;
                     if (uk == 4) {
                         pa = 0;
                     }
                 }
             }
             }
              else if (root["mType"].asInt() == 10 && F % 30 == 0) {
              putimage(0, 0, 1296, 696, &mmp, 48, 48*16);
              }
            }
            if ((w < s.size() && w < Bu) && F % 30==0 && pa == 0) {
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
                    outtextxy(30 * W + 250, 716 + K * 50 + 5, path);
                    W++;
                }
                w++;
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
            if(w==s.size()&&isReady){
            transparentimage(NULL, 600, 910, &tri, 0xFF55FF);
            FlushBatchDraw();
            flushmessage(-1);
            if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000)&&w==s.size()&&pa==0) {
                        break;
            }
            if ((GetAsyncKeyState(VK_SPACE) & 0x8000 )&& w == s.size()&&pa==0) {

                        break;
            }
            if (GetAsyncKeyState(VK_RETURN) & 0x8000 && w == s.size() && pa == 0) {
                break;
            }
            }
            FlushBatchDraw(); 
            start_time = clock();
            for (; (clock() - start_time) < 2;);
              F++;
          }
        }
        if (root.isMember("flag")) {
            f[root["flag"].asInt()].check = 1;
        }
        if (root.isMember("stone")) {
            for (int uc = 0; uc < root["stone"]["id"].size(); uc++) {
                st[root["stone"]["id"][uc].asInt()].number += root["stone"]["number"][uc].asInt();
            }
        }
        if (root.isMember("m_flag_on")) {
            m_f[root["m_flag_on"].asInt()].check=0;
        }
        if (root.isMember("m_flag_off")) {
            m_f[root["m_flag_on"].asInt()].check = 1;
        }
        if (root.isMember("task_d")) {
            tk[root["task_d"].asInt()].state = 2;
        }
        if (root.isMember("task_u")) {
            tk[root["task_u"].asInt()].state = 1;
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
    }
    in.close();
}
void readmapeventjson(player* p, npc* n,m_flag *m_f,arms *ar,item *it,Map *m,t_equip *t_E,stone *st,task *tk,const char* filename,int &m_id,int s,int &sp_id) {
    EndBatchDraw();
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    IMAGE t_block, a1, tri, c1;
    RECT t = { 50,696 + 5,210,696 + 60 + 5 };
    bool bo=true;
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
                settextstyle(23, 0, _T("Taipei Sans TC Beta"));
                LPCTSTR path = n[root["talk"][k]["npc"].asInt()].name.c_str();
                drawtext(path, &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                IMAGE p1;
                if (root["talk"][k]["npc"].asInt() != 0) {
                    mm = std::to_wstring(root["talk"][k]["npc"].asInt());
                    mm = L"./Game/picture/t_npc" + mm + L"_" + std::to_wstring(root["talk"][k]["face"].asInt()) + L".png";
                    LPCTSTR path = mm.c_str();
                    loadimage(&p1, path, 0, 0, false);
                  transparentimage(NULL, 50, 746, &p1, 0xFF55FF);
                }
            }
            else {
                settextstyle(23, 0, _T("Taipei Sans TC Beta"));
                LPCTSTR path = p[root["talk"][k]["player"].asInt()].name.c_str();
                drawtext(path, &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                IMAGE p1;
                mm = std::to_wstring(root["talk"][k]["player"].asInt());
                mm = L"./Game/picture/player" + mm + L"_" + std::to_wstring(root["talk"][k]["face"].asInt()) + L".png";
                path = mm.c_str();
                loadimage(&p1, path, 0, 0, false);
                transparentimage(NULL, 50, 746, &p1, 0xFF55FF);
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
                    outtextxy(30 * W + 250, 716 + K * 50 + 5, path);
                    W++;
                    Sleep(30);
                }
            }
            transparentimage(NULL, 600, 910, &tri, 0xFF55FF);
            flushmessage(-1);
            while (1) {
                if (isReady) {
                if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
                    Sleep(1000);
                    break;
                }
                if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                    Sleep(1000);
                    break;
                }
                if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
                    Sleep(1000);
                    break;
                }
                }

            }
        }
      }
        else if (root.isMember("see")) {         
            for (j = 0; j < root["see"].size();j++) {  
                putimage(0, 696, &t_block);
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
                    else if (s[w] == '@') {
                        settextstyle(30, 0, _T("Taipei Sans TC Beta"),0,0,0,false, false,bo);
                        bo = !bo;
                    }
                    else {
                        tt += s[w];
                        LPCTSTR path = tt.c_str();
                        outtextxy(30 * W + 250, 716 + K * 50 + 5, path);
                        W++;
                        Sleep(50);
                    }
                }
                transparentimage(NULL, 600, 910, &tri, 0xFF55FF);
            flushmessage(EM_KEY);
            while (1) {
                if (isReady) {
                if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                    Sleep(1000);
                    break;
                }
                if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
                    Sleep(1000);
                    break;
                }
                if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
                    Sleep(1000);
                    break;
                }
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
        if (root.isMember("shop")) {
            sp_id = root["shop"].asInt();
        }
        if (root.isMember("stone")) {
            for (int uc = 0; uc < root["stone"]["id"].size(); uc++) {
                st[root["stone"]["id"][uc].asInt()].number += root["stone"]["number"][uc].asInt();
            }
        }
        if (root.isMember("task_d")) {
            tk[root["task_d"].asInt()].state = 2;
        }
        if (root.isMember("task_u")) {
            tk[root["task_u"].asInt()].state = 1;
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
void e_put(enemy *e,b_map *b_m, terrain(*te)[50],int esize,int b_mid) {
    IMAGE b1,b2,b3,b4,hpB;
    wstring mm;
    for (int I = 0; I < esize; I++) {
        if (e[I].x * 48 - b_m[b_mid].ox < 960 && e[I].y * 48 - b_m[b_mid].oy - 16 - 16 < 720&&(te[e[I].x][e[I].y].dark==0|| te[e[I].x][e[I].y].dark == 2 || te[e[I].x][e[I].y].dark == 3)) {
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
void e_bput(enemy *e,b_map *b_m, terrain(*te)[50], int id, int esize,int b_mid) {
    IMAGE b1, b2, b3, b4;
    wstring mm;
    for (int I = 0; I < esize; I++) {
        if (I != id) {
            if (e[I].x * 48 - b_m[b_mid].ox < 960 && e[I].y * 48 - b_m[b_mid].oy - 16 - 16 < 720 && (te[e[I].x][e[I].y].dark == 0 || te[e[I].x][e[I].y].dark == 2|| te[e[I].x][e[I].y].dark == 3)) {
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
            if (b_n[I].buff_check[10] != 0) {
                IMAGE ef;
                loadimage(&ef, L"./Game/picture/mask.png", 0, 0, false);
                transparentimage(NULL, b_n[I].x * 48-b_m[b_mid].ox, b_n[I].y * 48 - 16-b_m[b_mid].oy, &ef);
            }
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
void maps(player *p, int P_id,enemy *e,b_map *b_m, arms *ar,terrain(*te)[50],int b_mid) {
    b_m[b_mid].ox = (b_m[b_mid].cx-11)* (b_m[b_mid].cx > 11) * 48   - (9 - (b_m[b_mid].x - b_m[b_mid].cx)) * (b_m[b_mid].cx + 9 > b_m[b_mid].x) * 48;
    b_m[b_mid].oy = (b_m[b_mid].cy -7) * (b_m[b_mid].cy > 7) * 48 - (8 - (b_m[b_mid].y - b_m[b_mid].cy)) * (b_m[b_mid].cy + 8 > b_m[b_mid].y) * 48;
    putimage(0, 0, 960, 720, &mapP, b_m[b_mid].ox, b_m[b_mid].oy);
    IMAGE te2,io,te3,io1,te4;         
    loadimage(&te2, L"./Game/picture/fire.png", 0, 0, false);
    loadimage(&te3, L"./Game/picture/trap.png", 0, 0, false);
    loadimage(&te4, L"./Game/picture/dark.png", 0, 0, false);
    for (int I = 0; I < b_m[b_mid].esize; I++) {
        if (e[I].buff_check[13] == 1) {
            loadimage(&io, L"./Game/picture/io.png", 0, 0, false);
            for (int sx = -1; sx < 2;sx++) {
                for (int sy = -1; sy < 2; sy++) {
                    if (te[e[I].target_x+sx][e[I].target_y+sy].type == 0) {
                        transparentimage(NULL, 48 * (e[I].target_x + sx), 48 * (e[I].target_y + sy), &io);
                    }
                }
            }
        }
        if (e[I].buff_check[15] == 1) {
            loadimage(&io, L"./Game/picture/io.png", 0, 0, false);
            for (int sx = -5; sx <= 5; sx++) {
                for (int sy = -5; sy <= 5; sy++) {
                    if (te[e[I].x + sx][e[I].y + sy].type == 0) {
                        if (abs(sx) + abs(sy) < 6) {
                        transparentimage(NULL, 48 * (e[I].x + sx), 48 * (e[I].y + sy), &io);
                        }
                    }
                }
            }
        }
    }
    for (int I = 0; I < b_m[b_mid].psize; I++) {
        if (p[I].buff_check[22] > 0) {
            loadimage(&io, L"./Game/picture/sb.png", 0, 0, false);
            int range=0;
            if (p[I].arms_id_1 != -1) {
                if (p[I].arms_b_1 > 0) {
                range = ar[p[I].arms_id_1].range;
                if (p[I].s_check[13] > 0) {
                    if (ar[p[I].arms_id_1].type[0] == 'r') {
                        range++;
                    }
                }
                }
            }
            if (p[I].arms_id_2 != -1) {
                if (p[I].arms_b_2 > 0) {
                    int rangeb = ar[p[I].arms_id_2].range;
                    if (p[I].s_check[13] > 0) {
                        if (ar[p[I].arms_id_2].type[0] == 'r') {
                            rangeb++;
                        }
                    }
                    if (range < rangeb) {
                         range = rangeb;
                    }
                }
            }
            for (int sx = -range; sx <= range; sx++) {
                for (int sy = -range; sy <= range; sy++) {
                    if ((sx!=0||sy!=0)&&abs(sx)+abs(sy)<=range&&te[p[I].x + sx][p[I].y + sy].type==0) {
                        transparentimage(NULL, 48 * (p[I].x+sx)-b_m[b_mid].ox, 48 * (p[I].y + sy) - b_m[b_mid].oy, &io);
                    }
                }
            }
        }
        if (p[I].buff_check[28] > 0) {
            loadimage(&io1, L"./Game/picture/cover.png", 0, 0, false);
            int range = 0;
            if (p[I].arms_id_1 != -1) {
                if (p[I].arms_b_1 > 0) {
                    range = ar[p[I].arms_id_1].range;
                    if (p[I].s_check[13] > 0) {
                        if (ar[p[I].arms_id_1].type[0] == 'r') {
                            range++;
                        }
                    }
                }
            }
            if (p[I].arms_id_2 != -1) {
                if (p[I].arms_b_2 > 0) {
                    int rangeb = ar[p[I].arms_id_2].range;
                    if (p[I].s_check[13] > 0) {
                        if (ar[p[I].arms_id_2].type[0] == 'r') {
                            rangeb++;
                        }
                    }
                    if (range < rangeb) {
                        range = rangeb;
                    }
                }
            }
            for (int sx = -range; sx <= range; sx++) {
                for (int sy = -range; sy <= range; sy++) {
                    if ((sx != 0 || sy != 0) && abs(sx) + abs(sy) <= range && te[p[I].x + sx][p[I].y + sy].type == 0) {
                        transparentimage(NULL, 48 * (p[I].x + sx) - b_m[b_mid].ox, 48 * (p[I].y + sy) - b_m[b_mid].oy, &io1);
                    }
                }
            }
        }
    }
    for (int I = b_m[b_mid].ox/48; I < b_m[b_mid].ox / 48+20; I++) {
        for (int J = b_m[b_mid].oy / 48; J < b_m[b_mid].oy / 48+15; J++) {
            if (te[I][J].fire == 1) {
                transparentimage(NULL,48*I- b_m[b_mid].ox,48*J- b_m[b_mid].oy,&te2);
            }      
            if (te[I][J].trap == 2) {
                transparentimage(NULL, 48 * I -b_m[b_mid].ox, 48 * J- b_m[b_mid].oy, &te3);
            }
            if (te[I][J].dark == 1||te[I][J].dark==2) {
                transparentimage(NULL, 48 * I- b_m[b_mid].ox, 48 * J-b_m[b_mid].oy, &te4);
            }
        }
    }
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
    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
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
void p_attack(wofstream* wofs, player *p,enemy *e,enemy_type *e_t,b_npc *b_n,arms *ar,b_map *b_m,buff *bu, terrain(*te)[50], int b_mid,int P_id,int id,string &chose,int esize,int psize,int bu_id,int b_nid) {
    EndBatchDraw();
    int ax, ay,cost[50][50],X,Y,cho[50][50];
    int ar_id=-1,bullet=-1,a_chose;
    LPCTSTR path1;
    wstring mm,vos;
    TCHAR t[5];
    IMAGE ar1,hi,get,h_r;
    string mark;
    RECT rect;
        int sss = 1;
        ExMessage m, m1; string attack;        
        if (chose == "a" && p[P_id].act > 0&&(p[P_id].arms_id_1!=-1||p[P_id].arms_id_2!=-1)) {
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
                    mm+=  L"(" + std::to_wstring(p[P_id].arms_b_1) + L"/" + to_wstring(ar[p[P_id].arms_id_1].mbullet) + L")";
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
                      mm += L"(" + std::to_wstring(p[P_id].arms_b_2) + L"/" + to_wstring(ar[p[P_id].arms_id_2].mbullet) + L")";
                    }
                    outtextxy(210, 815 + 48 * i, mm.c_str());
                    mm = L"./Game/picture/arms" + to_wstring(p[P_id].arms_id_2) + L".png";
                    loadimage(&ar1, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 180, 815+48*i, &ar1, 0xFF55FF);
                }
            while (sss != 0)//選武器
            {
                peekmessage(&m, EM_MOUSE);
                if (m.lbutton) {
                    if (m.x >= 175 && m.x <=325 && m.y >= 799 && m.y <=847) {
                        attack = "a"; ar_id = p[P_id].arms_id_1; bullet = p[P_id].arms_b_1; a_chose = 0; sss = 0;
                    }
                    else if (m.x >= 175 && m.x <=325 && m.y >= 847 && m.y <= 895&&i==1) {
                        attack = "a"; ar_id = p[P_id].arms_id_2; bullet = p[P_id].arms_b_2; a_chose = 1; sss = 0;
                    }
                }
                else if (m.rbutton) {
                    sss = 0;
                }
                if (GetAsyncKeyState(0x31) & 0x8000) {
                    attack = "a"; ar_id = p[P_id].arms_id_1; bullet = p[P_id].arms_b_1; a_chose = 0; sss = 0;
                }
                else  if (GetAsyncKeyState(0x32) & 0x8000 && i == 1) {
                    attack = "a"; ar_id = p[P_id].arms_id_2; bullet = p[P_id].arms_b_2; a_chose = 1; sss = 0;
                }
            }
            int rx, ry, AC, ATK, dmg;

            if (attack == "a" && bullet > 0) {
                int range= ar[ar_id].range;
                if (ar[ar_id].type == "rr") {
                    range++;
                }
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
                maps(p, P_id, e, b_m,ar,te, b_mid);
                while (q.size() > 0) {
                    X = q.front().first;
                    Y = q.front().second;
                    if (te[X+1][Y].type!=1&&cost[X][Y]<range&&X+1<b_m[b_mid].x) {
                        cost[X+1][Y]=cost[X][Y]+1;
                        q.push({ X +1,Y  });
                    }
                    if (te[X-1][Y].type != 1 && cost[X][Y] < range&&X-1>=0) {
                        cost[X - 1][Y] = cost[X][Y] + 1;
                        q.push({ X - 1,Y });
                    }
                    if (te[X][Y+1].type != 1 && cost[X][Y] <range&&Y+1<b_m[b_mid].y) {
                        cost[X][Y+1] = cost[X][Y] + 1;
                        q.push({ X ,Y + 1 });
                    }
                    if (te[X][Y-1].type != 1 && cost[X][Y] <range&&Y-1>=0) {
                        cost[X][Y-1] = cost[X][Y] + 1;
                        q.push({ X ,Y - 1 });
                    }
                    if (te[X][Y].type != 1 && te[X][Y].npc != 1 && te[X][Y].player != 1&&cho[X][Y]!=1) {
                         transparentimage(NULL, 48 * X-b_m[b_mid].ox, 48 * Y-b_m[b_mid].oy, &mb);
                         cho[X][Y] = 1;
                    }
                    q.pop();
                }               
                e_put(e, b_m, te, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p,b_m, psize,b_mid);
                ui(p, e, b_m,P_id, esize, psize,b_mid,bu_id);
                EndBatchDraw();
                getimage(&get, 0, 0, 1296, 960);
                loadimage(&h_r, L"./Game/picture/H_R.png", 0, 0, false);
                sss = 1;
                BeginBatchDraw();
                int hx, hy,ha,hc,hv=0,hd,hcv=0,hrc,absa,wheel=0;
                hv = p[P_id].buff_check[1] + p[P_id].buff_check[2];
                HWND hwnd = GetForegroundWindow();
                RECT reCt;
                GetWindowRect(hwnd, &reCt);
                int left = reCt.left;
                int top = reCt.top;
                SetCursorPos(p[P_id].x * 48 - b_m[b_mid].ox + 24 + left, p[P_id].y * 48 - b_m[b_mid].oy + 48 + top);
                while (sss != 0)
                {
                    // 获取一条鼠标消息
                    m1 = getmessage(EM_MOUSE);                 
                  putimage(0, 0, &get);     
                        for (i = 0; i < 20; i++) {
                            for (j = 0; j < 15; j++) {
                                if (m1.x >= 48 * i && m1.x <= 48 * i + 48 && m1.y >= 48 * j && m1.y <= 48 * j + 48&&cho[i + b_m[b_mid].ox / 48][j + b_m[b_mid].oy / 48]==1&&m1.x<960&&m1.y<720) {
                                    for (int k = 0; k < esize; k++) {
                                        if (e[k].x == i+b_m[b_mid].ox/48 && e[k].y == j + b_m[b_mid].oy / 48) {
                                           hcv=PKTVS(p, e, b_m, ar, e_t,P_id, k, b_mid, ar_id, 0,ar[ar_id].Dmg);
                                           if (hcv == -2334) {
                                               return ;
                                           }
                                           if (hcv != -12334) {
                                               sss = 0;
                                           }
                                        }
                                    }
                                }
                            }
                        }

                    if (m1.rbutton) {
                            return;
                    }
                     FlushBatchDraw();
                }
                *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"將" << e[id].name << L"選為目標" << endl;
                ry = (p[P_id].y - e[id].y) * (p[P_id].y > e[id].y) + (p[P_id].y - e[id].y) * (p[P_id].y < e[id].y) * -1; rx = (p[P_id].x - e[id].x) * (p[P_id].x > e[id].x) + (p[P_id].x - e[id].x) * (p[P_id].x < e[id].x) * -1;
                if (rx + ry <= range) {
                    ay = p[P_id].y - e[id].y;ax = p[P_id].x - e[id].x;
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
                    while (1) {
                    AC =roll("",2) + (e[id].dex - 10) / 2;
                    if (e[id].buff_check[20] == 1) {
                        AC = 0;
                    }
                    if (e[id].buff_check[27] >0) {
                        AC /= 2;
                    }
                    if (ar[ar_id].type[0] == 'r' && p[P_id].buff_check[25] == 1) {
                        AC = 0;
                        p[P_id].buff_check[25] = 0;
                        p[P_id].buff_check[24] = 1;
                        p[P_id].buff_time[p[P_id].buff_Size] = 30 - p[P_id].dex / 2;
                        p[P_id].buff_id[p[P_id].buff_Size] = 24;
                        p[P_id].buff_Size++;
                    }
                    else if (p[P_id].buff_check[24] > 0) {
                        for (int I = 0; I < p[P_id].buff_Size; I++) {
                            if (p[P_id].buff_id[I] == 24) {
                                p[P_id].buff_time[I] = 30 - p[P_id].dex / 2;
                            }
                        }
                    }
                    if (ar[ar_id].type[0] == 'r') {
                     ATK = roll("", 2) + (p[P_id].dex - 10) / 2+roll(ar[ar_id].hit,1);
                    }
                    else if (ar[ar_id].type[0] == 'm') {
                        ATK = roll("", 2) + (p[P_id].str - 10) / 2 + roll(ar[ar_id].hit, 1);;
                    }
                    if (e[id].buff_check[19] > 0) {
                        mark = "1d" + to_string(p[P_id].cha / 2);
                        ATK += roll(mark, 1);
                    }
                    if (ATK > AC&&hcv>=0) {
                        break;
                    }
                    if (AC>=ATK&&hcv <= 0) {
                        break;
                    }
                    if (hcv == 0) {
                        break;
                    }
                    if (hcv > 0) {
                        hcv--;
                    }
                    if (hcv < 0) {
                        hcv++;
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
                                maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                                e_put(e, b_m, te, esize, b_mid);
                                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                p_put(p, b_m, psize, b_mid); 
                                transparentimageA(NULL, e[id].x * 48 - b_m[b_mid].ox-30, e[id].y * 48 - b_m[b_mid].oy-160, &ae, i * 120, j * 300, 120, 300);
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
                                maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                                e_put(e, b_m, te, esize, b_mid);
                                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                p_put(p, b_m, psize, b_mid);
                                transparentimageA(NULL, e[id].x * 48 - b_m[b_mid].ox-110, e[id].y * 48 - b_m[b_mid].oy-47, &ae, i * 320, j * 120, 320, 120);
                                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);                                
                                EndBatchDraw();
                                Sleep(5);
                            }
                        }
                    }
                    else if (ar_id == 8) {
                        BeginBatchDraw();
                        maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                        e_put(e, b_m, te, esize, b_mid);
                        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, psize, b_mid);
                        ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                        EndBatchDraw();
                        getimage(&get, 0, 0, 1296, 960);
                        loadimage(&ae, L"./Game/picture/杖1.png", 0, 0, false);
                        mciSendString(L"close se", NULL, 0, NULL);
                        mciSendString(L"open ./Game/Sound/SE/軽いパンチ2.mp3 alias se", NULL, 0, NULL);
                        vos = L"setaudio se volume to " + to_wstring(soundSize);
                        mciSendString(vos.c_str(), NULL, 0, NULL);
                        mciSendString(L"play se from 0", NULL, 0, NULL);
                        for (int ej = 0; ej < 4; ej++) {
                            for (int ei = 0; ei < 5; ei++) {
                                putimage(0, 0, &get);
                                transparentimageA(NULL, 48 * e[id].x - b_m[b_mid].ox - 10, e[id].y * 48 - b_m[b_mid].oy - 30, &ae, ei * 80, ej * 80, 80, 80);
                                FlushBatchDraw();
                                Sleep(40);
                            }
                        }
                    }
                    if (ATK > AC) {
                         BeginBatchDraw();
                        maps(p, P_id, e,b_m, ar, te,b_mid);/*地圖繪製*/
                        e_put(e,b_m, te, esize,b_mid);
                        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, psize, b_mid);
                        dmg = roll(ar[ar_id].dmg, 1);
                        if (ar[ar_id].type[0] == 'm') {
                            dmg += (p[P_id].str - 10) / 5;
                        }
                        if (p[P_id].s_check[15] == 1) {
                            if (abs(p[P_id].x-e[id].x) + abs(p[P_id].y-e[id].y) > 5) {
                                dmg += abs(p[P_id].x - e[id].x) + abs(p[P_id].y - e[id].y) - 5;
                            }
                        }
                        if (p[P_id].s_check[17] == 1&&e_t[e[id].type].species=="a") {
                            dmg *= 1.3;
                        }
                        if (p[P_id].buff_check[21] > 0) {
                            dmg += (p[P_id].wis - 5) / 2;
                        }      
                        eHP(e, b_m,id,b_mid, 0, dmg);
                        *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"用" << ar[ar_id].name << L"命中" <<  e[id].name<< L"造成" << dmg << L"點傷害(" << ATK << ">" << AC << ")" << endl;
                        if (p[P_id].buff_check[21] > 0) {
                            mm = L"+" + to_wstring((p[P_id].mhp - p[P_id].hp) / 5);
                        settextcolor(RGB(47, 255, 197));
                        settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                        outtextxy(p[P_id].x * 48 - b_m[b_mid].ox, p[P_id].y * 48 - b_m[b_mid].oy - 40, mm.c_str());
                        p[P_id].hp += (p[P_id].mhp - p[P_id].hp) / 5;
                        settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                        settextcolor(RGB(0, 0, 0));
                        }
                        EndBatchDraw();
                        if (ar[ar_id].type[0] == 'r') {
                          bullet--;
                        }
                        p[P_id].act--;
                        Sleep(1000);
                        if (e[id].hp <= 0) {
                            te[e[id].x][e[id].y].mA -= 10000;
                            te[e[id].x][e[id].y].mB -= 10000;
                            int x = e[id].x, y = e[id].y;
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
                            te[e[id].x][e[id].y].enemy = 0;
                            e[id].x = -1; e[id].y = -1;
                            e[id].speed = -1;
                            if (e[id].buff_check[20] > 0) {
                                p[0].buff_check[20]=0;
                                e[id].buff_check[20] = 0;
                            }
                        }
                    }
                    else {
                        BeginBatchDraw();
                        maps(p, P_id, e,b_m, ar, te,b_mid);/*地圖繪製*/
                        e_put(e, b_m, te, esize, b_mid);
                        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, psize, b_mid);
                        *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"用" << ar[ar_id].name << L"攻擊" << e[id].name << L"未能命中(" << ATK << "<=" << AC << ")" << endl;
                        settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                        settextcolor(RGB(255, 0, 0));
                        mm = L"miss";
                        outtextxy(e[id].x * 48 - b_m[b_mid].ox, e[id].y * 48 - b_m[b_mid].oy - 40, mm.c_str());
                        settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                        settextcolor(RGB(0, 0, 0));
                        if (p[P_id].buff_check[21] > 0) {
                            mm = L"+" + to_wstring((p[P_id].mhp - p[P_id].hp) / 5);
                            settextcolor(RGB(47, 255, 197));
                            settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                            outtextxy(p[P_id].x * 48 - b_m[b_mid].ox, p[P_id].y * 48 - b_m[b_mid].oy - 40, mm.c_str());
                            p[P_id].hp += (p[P_id].mhp - p[P_id].hp) / 5;
                            settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                            settextcolor(RGB(0, 0, 0));
                        }
                        EndBatchDraw();
                        if (ar[ar_id].type[0] == 'r') {
                            bullet--;
                        }
                        p[P_id].act--;
                        Sleep(1000);
                    }
                    if (a_chose == 0) {
                        p[P_id].arms_b_1=bullet;
                    }
                    else if (a_chose == 1) {
                        p[P_id].arms_b_2 = bullet;
                    }
                    /*掩護射擊處理*/
                    if (e[id].hp > 0) {                    
                    for (i = 0; i < b_m[b_mid].psize;i++) {
                        if (p[i].buff_check[28] > 0) {                      
                                    int range = 0;
                                    int ar_id = -1;
                                    if (p[i].arms_id_1 != -1&& ar[p[i].arms_id_1].type[0]!='m') {
                                        if (p[i].arms_b_1 > 0) {
                                            range = ar[p[i].arms_id_1].range;
                                            ar_id = p[i].arms_id_1;
                                        }
                                        if (p[i].buff_check[23] > 0 && ar[p[i].arms_id_2].type[1] == 'r') {
                                            range++;
                                        }
                                    }
                                    if (p[i].arms_id_2 != -1 && ar[p[i].arms_id_2].type[0] != 'm') {
                                        if (p[i].arms_b_2 > 0) {
                                            int rangeb = ar[p[i].arms_id_2].range;
                                            if (p[i].buff_check[23] > 0&&ar[p[i].arms_id_2].type[1]=='r') {
                                                rangeb++;
                                            }
                                            if (range < rangeb) {
                                                range = rangeb;
                                                ar_id = p[i].arms_id_2;
                                            }
                                        }
                                    }
                                    if (abs(p[i].x - e[id].x) + abs(p[i].y - e[id].y) <= range && ar_id != -1) {
                                        int AC = roll("", 2) + (e[id].dex - 10) / 2;
                                        if (e[id].buff_check[20] == 1) {
                                            AC = 0;
                                        }
                                        if (e[id].buff_check[27] > 0) {
                                            AC /= 2;
                                        }
                                         int ATK = roll("", 2) + (p[i].dex - 10) / 2 + roll(ar[ar_id].hit, 1);
                                        wstring vos;
                                        IMAGE ae;
                                        if (ar_id == 0 || ar_id == 5) {
                                            mciSendString(L"close se", NULL, 0, NULL);
                                            mciSendString(L"open ./Game/Sound/SE/拳銃2.mp3 alias se", NULL, 0, NULL);
                                            vos = L"setaudio se volume to " + to_wstring(soundSize);
                                            mciSendString(vos.c_str(), NULL, 0, NULL);
                                            mciSendString(L"play se from 0", NULL, 0, NULL);
                                            loadimage(&ae, L"./Game/picture/武器・弓1.png", 0, 0, false);
                                            BeginBatchDraw();
                                            for (int ej = 0; ej < 5; ej++) {
                                                for (int ei = 0; ei < 3; ei++) {
                                                    maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                                                    e_put(e, b_m, te, esize, b_mid);
                                                    b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                                    p_put(p, b_m, psize, b_mid);
                                                    transparentimageA(NULL, e[id].x * 48 - b_m[b_mid].ox - 30, e[id].y * 48 - b_m[b_mid].oy - 160, &ae, ei * 120, ej * 300, 120, 300);
                                                    ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                                                    FlushBatchDraw();
                                                    Sleep(5);
                                                }
                                            }
                                            EndBatchDraw();
                                        }
                                        /*ae*/
                                        if (ATK > AC) {
                                            int dmg = roll(ar[ar_id].dmg, 1);
                                            if (dmg >= e[id].buff_check[7] + e[id].buff_check[10]) {
                                                dmg -= e[id].buff_check[7] + e[id].buff_check[10];
                                            }
                                            else {
                                                dmg = 0;
                                            }
                                            if (p[i].buff_check[21] > 0) {
                                                dmg += (p[P_id].wis - 5) / 2;
                                            }
                                            eHP(e,b_m, id,b_mid, 0, dmg);
                                            *wofs << L"(T" << b_m[b_mid].time << L")" << p[i].name << L"用" << ar[ar_id].name << L"命中" << e[id].name << L"造成" << dmg << L"點傷害(" << ATK << ">" << AC << ")" << endl;
                                            EndBatchDraw();
                                                if (ar_id == p[i].arms_id_1 && p[i].arms_b_1 > 0) {
                                                    p[i].arms_b_1--;
                                                }
                                                else {
                                                    p[i].arms_b_2--;
                                                }
                                            Sleep(1000);
                                            if (e[id].hp <= 0) {
                                                te[e[id].x][e[id].y].mA -= 10000;
                                                te[e[id].x][e[id].y].mB -= 10000;
                                                int x = e[id].x, y = e[id].y;
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
                                                te[e[id].x][e[id].y].enemy = 0;
                                                e[id].x = -1; e[id].y = -1;
                                                e[id].speed = -1;
                                                if (e[id].buff_check[20] > 0) {
                                                    p[0].buff_check[20] = 0;
                                                    e[id].buff_check[20] = 0;
                                                }
                                                return;
                                            }
                                        }
                                        else {
                                            *wofs << L"(T" << b_m[b_mid].time << L")" << p[i].name << L"用" << ar[ar_id].name << L"攻擊" << e[id].name << L"未能命中(" << ATK << "<=" << AC << ")" << endl;
                                                if (ar_id == p[i].arms_id_1 && p[i].arms_b_1 > 0) {
                                                    p[i].arms_b_1--;
                                                }
                                                else {
                                                    p[i].arms_b_2--;
                                                }
                                            mm = L"miss";
                                            settextcolor(RGB(255, 0, 0));
                                            settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                                            outtextxy(e[id].x * 48 - b_m[b_mid].ox, e[id].y * 48 - b_m[b_mid].oy - 40, mm.c_str());
                                            settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                                            settextcolor(RGB(0, 0, 0));
                                            Sleep(1000);
                                        }
                                    }                            
                        }
                    }                    
                    }
                }
                else {
                }
            }
            else if (chose == "a" &&bullet==0) {
            mciSendString(L"close se", NULL, 0, NULL);
            mciSendString(L"open ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 alias se", NULL, 0, NULL);
            vos = L"setaudio se volume to " + to_wstring(soundSize);
            mciSendString(vos.c_str(), NULL, 0, NULL);
            mciSendString(L"play se from 0", NULL, 0, NULL);
            }
        }
        else if (chose == "a" ) {
        mciSendString(L"close se", NULL, 0, NULL);
        mciSendString(L"open ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 alias se", NULL, 0, NULL);
        vos = L"setaudio se volume to " + to_wstring(soundSize);
        mciSendString(vos.c_str(), NULL, 0, NULL);
        mciSendString(L"play se from 0", NULL, 0, NULL);
        }
    }
void p_item(wofstream* wofs, player *p,enemy *e,b_npc *b_n,arms *ar, item *it, t_equip *t_E,b_map *b_m, terrain(*te)[50],string &chose, int P_id, int i_id, int t_Eid,int b_mid,int bu_id) {
    wstring vos;
    if (chose == "i"&&p[P_id].t_id!=-1) {
    IMAGE p1,ar1,hi,get;
    wstring mm;        
    ExMessage m;
    RECT t,is={960,0,960+247,148};
    int cost[50][50], cho[50][50], X, Y;
    loadimage(&p1, L"./Game/picture/tbox.png", 0, 0, false);
    loadimage(&hi, L"./Game/picture/b_hint.png", 0, 0, false);
    vector<IMAGE> itemImages;
    int I, J,ry,rx,n,k;
    for (int i = 0; i < t_E[p[P_id].t_id].size; i++) {
        if (p[P_id].box_id[i] != -1 && p[P_id].box_size != 0) {
            mm = L"./Game/picture/item" + to_wstring(p[P_id].box_id[i]) + L".png";
            IMAGE p2;
            loadimage(&p2, mm.c_str(), 0, 0, false);
            itemImages.push_back(p2);
        }
    }
   IMAGE ib,ab,ao;
   loadimage(&ib, L"./Game/picture/ib.png", 0, 0, false);
   loadimage(&ab, L"./Game/picture/ab.png", 0, 0, false);
   loadimage(&ao, L"./Game/picture/io.png", 0, 0, false);
        getimage(&get, 0, 0, 1296, 960);
    while (1) {
        BeginBatchDraw();      
        putimage(0, 0, &get);
        for (i = 0; i < t_E[p[P_id].t_id].size; i++) {
            putimage(400 + i * 30, 720, &p1);
            if (p[P_id].box_id[i] != -1 && p[P_id].box_size[i] != 0) {
                transparentimage(NULL, 405 + i * 30, 725, &itemImages[i], 0xFF55FF);
                outtextxy(405 + i * 30, 700, to_wstring(p[P_id].box_size[i]).c_str());
            }
        }                 
                m = getmessage(EM_MOUSE|EM_KEY);
        for (i = 0; i<t_E[p[P_id].t_id].size; i++) {
            if (m.x >= 400 + i * 30 && m.x <= 430 + i * 30 && m.y >= 720 && m.y <= 750) {
                POINT pts[] = { {400 + i * 30,720 }, {400  + i * 30 + 30,720 }, {400  + i * 30 + 30,750 }, {400  + i * 30,750 } };
                polygon(pts, 4);
                drawtext(it[p[P_id].box_id[i]].story.c_str(), &is, DT_WORDBREAK);
                FlushBatchDraw();
            }
        }
        int vk=0;
        for (i = 0; i < t_E[p[P_id].t_id].size; i++) {
            vk = 0;
        if (GetAsyncKeyState(0x31) & 0x8000) {
            i = 0; vk = 1;
        }
        else if (GetAsyncKeyState(0x32) & 0x8000) {
            i = 1; vk = 1;
        }
        else if (GetAsyncKeyState(0x33) & 0x8000) {
            i = 2; vk = 1;
        }
           if ((m.x >= 400 + i * 30 && m.x <= 430 + i * 30 && m.y >= 720 && m.y <= 750)||vk==1) {
            if (m.lbutton||vk==1) {       
                if (p[P_id].box_id[i] != -1 && p[P_id].box_size[i] != 0) {
                    if (it[p[P_id].box_id[i]].type == 'b' || it[p[P_id].box_id[i]].type=='h') {
                        b_camera(b_m, p[P_id].x, p[P_id].y, b_mid);
                        maps(p, P_id, e, b_m, ar, te, b_mid);
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
                        e_put(e, b_m, te, b_m[b_mid].esize, b_mid);
                        b_nput(b_n, b_m, b_m[b_mid].nsize, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, b_m[b_mid].psize, b_mid);
                        ui(p, e, b_m, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, bu_id);
                        EndBatchDraw();
                        while (1) {
                            m = getmessage(EM_MOUSE);
                            if (m.lbutton) {
                                for (int I = 0; I < b_m[b_mid].psize; I++) {
                                    if (m.x >= 48 * p[I].x -b_m[b_mid].ox && m.x <= 48 * p[I].x + 48 - b_m[b_mid].ox && m.y >= 48 * p[I].y - b_m[b_mid].oy&& m.y <= 48 * p[I].y + 48 - b_m[b_mid].oy&&m.x<960&&m.y<720) {
                                        if (cho[p[I].x][p[I].y] == 1) {
                                        if (p[P_id].box_id[i] == 1|| p[P_id].box_id[i] == 5|| p[P_id].box_id[i] == 16) {
                                            if (I == P_id) {
                                                if (p[P_id].s_check[4] == 1) {
                                                    if (p[I].mhp / 10 + p[I].hp > p[I].mhp) {
                                                        p[I].hp = p[I].mhp;
                                                    }
                                                    else {
                                                        p[I].hp += p[I].mhp / 10;
                                                    }
                                                }
                                                p[P_id].buff_time[p[P_id].buff_Size]= it[p[P_id].box_id[i]].time;
                                                p[P_id].buff_id[p[P_id].buff_Size] = 4;
                                                p[P_id].buff_check[4] += 1;
                                                if (p[P_id].t_id == 0) {
                                                    p[P_id].buff_time[p[P_id].buff_Size] = p[P_id].buff_time[p[P_id].buff_Size]  *80/100;
                                                }         
                                                if (p[P_id].buff_time[p[P_id].buff_Size] < 1) {
                                                    p[P_id].buff_time[p[P_id].buff_Size] = 1;
                                                }
                                                p[P_id].buff_give[p[P_id].buff_Size] = "iw"+to_string(p[P_id].box_id[i]);
                                                p[P_id].buff_Size++;
                                                p[P_id].box_size[i]--;
                                            }
                                            else {
                                                if (p[P_id].s_check[4] == 1) {
                                                    if (p[I].mhp / 10 + p[I].hp > p[I].mhp) {
                                                        p[I].hp = p[I].mhp;
                                                    }
                                                    else {
                                                        p[I].hp += p[I].mhp / 10;
                                                    }
                                                }
                                                p[P_id].buff_time[p[P_id].buff_Size] = it[p[P_id].box_id[i]].time;
                                                p[I].buff_time[p[I].buff_Size] = it[p[P_id].box_id[i]].time;
                                                p[P_id].buff_id[p[P_id].buff_Size] = 4;
                                                p[I].buff_id[p[I].buff_Size] = 4;
                                                p[P_id].buff_check[4] += 1;
                                                p[I].buff_check[4] += 1;
                                                if (p[P_id].t_id == 0) {
                                                    p[P_id].buff_time[p[P_id].buff_Size] = p[P_id].buff_time[p[P_id].buff_Size] * 80 / 100;
                                                    p[I].buff_time[p[I].buff_Size] = p[I].buff_time[p[I].buff_Size] * 80 / 100;
                                                }
                                                if (p[P_id].buff_time[p[P_id].buff_Size] < 1) {
                                                    p[P_id].buff_time[p[P_id].buff_Size] = 1;
                                                    p[I].buff_time[p[I].buff_Size] = 1;
                                                }
                                                p[P_id].buff_give[p[P_id].buff_Size] = "w";
                                                p[I].buff_give[p[I].buff_Size] = "i"+to_string(p[P_id].box_id[i]);
                                                p[P_id].buff_Size++;
                                                p[I].buff_Size++;
                                                p[P_id].box_size[i]--;
                                            }
                                            chose = "e";
                                            return;
                                        }
                                        else if (p[P_id].box_id[i] == 0||p[P_id].box_id[i] == 3) {
                                            if (I == P_id) {
                                                IMAGE backa, armb;
                                                loadimage(&backa, L"./Game/picture/attackblock.png", 0, 0, false);
                                                loadimage(&armb, L"./Game/picture/armsblock.png", 0, 0, false);
                                                n = 0;
                                                if (p[P_id].arms_id_1 != -1) {
                                                    putimage(175, 799 + 48 * n, &armb);
                                                    mm = ar[p[P_id].arms_id_1].name;
                                                    if (ar[p[P_id].arms_id_1].type[0] == 'r') {
                                                        mm += L"(" + std::to_wstring(p[P_id].arms_b_1) + L"/" + to_wstring(ar[p[P_id].arms_id_1].mbullet) + L")";
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
                                                        mm += L"(" + std::to_wstring(p[P_id].arms_b_2) + L"/" + to_wstring(ar[p[P_id].arms_id_2].mbullet) + L")";
                                                    }
                                                    outtextxy(210, 815 + 48 * n, mm.c_str());
                                                    mm = L"./Game/picture/arms" + to_wstring(p[P_id].arms_id_2) + L".png";
                                                    loadimage(&ar1, mm.c_str(), 0, 0, false);
                                                    transparentimage(NULL, 180, 815 + 48 * n, &ar1, 0xFF55FF);
                                                }
                                                ExMessage im;                                               
                                                while (1) {
                                                    im = getmessage(EM_MOUSE);
                                                    if (im.lbutton) {
                                                        for (k = 0; k <= n; k++) {
                                                        if (im.x >= 175 && im.x <= im.x <= 325 && im.y >= 799 + 48 * k && im.y <= 847 + 48 * k) {
                                                            if (k == 0) {
                                                                if (ar[p[P_id].arms_id_1].type[0] == 'r') {
                                                                    if (ar[p[P_id].arms_id_1].type[1] == 'r'&& p[P_id].box_id[i] == 0) {
                                                                        p[P_id].buff_time[p[P_id].buff_Size] = ar[p[I].arms_id_1].time;
                                                                        p[P_id].buff_id[p[P_id].buff_Size] = 5;
                                                                        p[P_id].buff_check[5] += 1;                                                                        
                                                                        p[P_id].buff_give[p[P_id].buff_Size] = "iw0a";
                                                                        p[P_id].buff_Size++;
                                                                        p[P_id].box_size[i]--;
                                                                        chose = "e";
                                                                        return;
                                                                    }
                                                                    else if (ar[p[P_id].arms_id_1].type[1] == 'p' && p[P_id].box_id[i] == 3) {
                                                                        p[P_id].buff_time[p[P_id].buff_Size] = ar[p[I].arms_id_1].time;
                                                                        p[P_id].buff_id[p[P_id].buff_Size] = 5;
                                                                        p[P_id].buff_check[5] += 1;
                                                                        p[P_id].buff_give[p[P_id].buff_Size] = "iw3a";
                                                                        p[P_id].buff_Size++;
                                                                        p[P_id].box_size[i]--;
                                                                        chose = "e";
                                                                        return;
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
                                                                    mm = ar[p[P_id].arms_id_1].name + L"並非槍械";
                                                                    drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                                                                    Sleep(2000);
                                                                }
                                                            }
                                                            else if (k == 1) {
                                                                if (ar[p[P_id].arms_id_2].type[0] == 'r' ) {
                                                                    if (ar[p[P_id].arms_id_2].type[1] == 'r'&& p[P_id].box_id[i] == 0) {
                                                                        p[P_id].buff_time[p[P_id].buff_Size] = ar[p[I].arms_id_2].time;
                                                                        p[P_id].buff_id[p[P_id].buff_Size] = 5;
                                                                        p[P_id].buff_check[5] += 1;
                                                                        p[P_id].buff_give[p[P_id].buff_Size] = "iw0A";
                                                                        p[P_id].buff_Size++;
                                                                        p[P_id].box_size[i]--;
                                                                        chose = "e";
                                                                        return;
                                                                    }
                                                                    else if (ar[p[P_id].arms_id_2].type[1] == 'p' && p[P_id].box_id[i] == 3) {
                                                                        p[P_id].buff_time[p[P_id].buff_Size] = ar[p[I].arms_id_2].time;
                                                                        p[P_id].buff_id[p[P_id].buff_Size] = 5;
                                                                        p[P_id].buff_check[5] += 1;
                                                                        p[P_id].buff_give[p[P_id].buff_Size] = "iw3A";
                                                                        p[P_id].buff_Size++;
                                                                        p[P_id].box_size[i]--;
                                                                        chose = "e";
                                                                        return;
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
                                                    else if(im.rbutton) {
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                              
                                        }

                                    }
                                }
                                for (I = 0; I < b_m[b_mid].nsize; I++) {
                                    if (m.x >= 48 * b_n[I].x - b_m[b_mid].ox && m.x <= 48 * b_n[I].x + 48 - b_m[b_mid].ox && m.y >= 48 * b_n[I].y - b_m[b_mid].oy && m.y <= 48 * b_n[I].y + 48 - b_m[b_mid].oy&&m.x<960&&m.y<720) {
                                        if (cho[b_n[I].x][b_n[I].y] == 1) {
                                            if (p[P_id].box_id[i] == 1 || p[P_id].box_id[i] == 5 || p[P_id].box_id[i] == 16) {
                                                if (p[P_id].s_check[4] == 1) {
                                                    if (b_n[I].mhp / 10 + b_n[I].hp > b_n[I].mhp) {
                                                        b_n[I].hp = b_n[I].mhp;
                                                    }
                                                    else {
                                                        b_n[I].hp += b_n[I].mhp / 10;
                                                    }
                                                }
                                                p[P_id].buff_time[p[P_id].buff_Size] = it[p[P_id].box_id[i]].time;
                                                b_n[I].buff_time[b_n[I].buff_Size] = it[p[P_id].box_id[i]].time;
                                                p[P_id].buff_id[p[P_id].buff_Size] = 4;
                                                b_n[I].buff_id[b_n[I].buff_Size] = 4;
                                                p[P_id].buff_check[4] += 1;
                                                b_n[I].buff_check[4] += 1;
                                                if (p[P_id].t_id == 0) {
                                                    p[P_id].buff_time[p[P_id].buff_Size] = p[P_id].buff_time[p[P_id].buff_Size] * 80 / 100;
                                                    b_n[I].buff_time[b_n[I].buff_Size] = b_n[I].buff_time[b_n[I].buff_Size] * 80 / 100;
                                                }
                                                if (p[P_id].buff_time[p[P_id].buff_Size] < 1) {
                                                    p[P_id].buff_time[p[P_id].buff_Size] = 1;
                                                    b_n[I].buff_time[b_n[I].buff_Size] = 1;
                                                }
                                                p[P_id].buff_give[p[P_id].buff_Size] = "w";
                                                b_n[I].buff_give[b_n[I].buff_Size] = "i"+to_string(p[P_id].box_id[i]);
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
                    else if (it[p[P_id].box_id[i]].type = 'a') {
                    b_camera(b_m, p[P_id].x, p[P_id].y, b_mid);
                    maps(p, P_id, e, b_m, ar, te, b_mid);
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
                        if (te[X + 1][Y].type != 1 && cost[X][Y] <p[P_id].str/5+3 && X + 1 < b_m[b_mid].x) {
                            cost[X + 1][Y] = cost[X][Y] + 1;
                            q.push({ X + 1,Y });
                        }
                        if (te[X - 1][Y].type != 1 && cost[X][Y] < p[P_id].str / 5 + 3 && X - 1 >= 0) {
                            cost[X - 1][Y] = cost[X][Y] + 1;
                            q.push({ X - 1,Y });
                        }
                        if (te[X][Y + 1].type != 1 && cost[X][Y] < p[P_id].str / 5 + 3 && Y + 1 < b_m[b_mid].y) {
                            cost[X][Y + 1] = cost[X][Y] + 1;
                            q.push({ X ,Y + 1 });
                        }
                        if (te[X][Y - 1].type != 1 && cost[X][Y] < p[P_id].str / 5 + 3 && Y - 1 >= 0) {
                            cost[X][Y - 1] = cost[X][Y] + 1;
                            q.push({ X ,Y - 1 });
                        }
                        if (te[X][Y].type != 1 && cho[X][Y] != 1) {
                            transparentimage(NULL, 48 * X - b_m[b_mid].ox, 48 * Y - b_m[b_mid].oy, &ab);
                            cho[X][Y] = 1;
                        }
                        q.pop();
                    }
                    e_put(e, b_m, te, b_m[b_mid].esize, b_mid);
                    b_nput(b_n, b_m, b_m[b_mid].nsize, b_m[b_mid].nsize, b_mid);
                    p_put(p, b_m, b_m[b_mid].psize, b_mid);
                    ui(p, e, b_m, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, bu_id);
                    EndBatchDraw();
                    getimage(&get,0,0,1296,960);
                    while (1) {
                        m = getmessage(EM_MOUSE);
                        if (m.lbutton) {
                            if (cho[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48] == 1) {
                                if (p[P_id].box_id[i]==17) {
                                int x = m.x / 48 + b_m[b_mid].ox / 48,y= m.y / 48 + b_m[b_mid].oy / 48;
                                for (int I = -1; I <= 1; I++) {
                                    for (int J = -1; J <= 1; J++) {
                                        for (int pi=0; pi < b_m[b_mid].psize; pi++) {
                                            if (p[pi].x == x + I && p[pi].y == y + J) {
                                            int DMG = roll("2d3",1);
                                            pHP(p, b_m,pi,b_mid, 0, DMG);
                                            *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"用" << it[p[P_id].box_id[i]].Name << L"命中" << p[pi].name << L"造成" << DMG << L"點傷害" << endl;
                                            p[pi].buff_check[12] +=3 ;
                                            }
                                        }
                                        for (int ei = 0; ei < b_m[b_mid].esize; ei++) {
                                            if (e[ei].x == x + I && e[ei].y == y + J) {
                                                int DMG = roll("2d3", 1);
                                                eHP(e,b_m, ei,b_mid, 0, DMG);
                                                *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"用" << it[p[P_id].box_id[i]].Name << L"命中" << e[ei].name << L"造成" << DMG << L"點傷害" << endl;
                                                e[ei].buff_check[12] += 3;
                                            }
                                        }
                                        for (int ni = 0; ni < b_m[b_mid].nsize; ni++) {
                                            if (b_n[ni].x == x + I && b_n[ni].y == y + J) {
                                                int DMG = roll("2d3", 1);
                                                nHP(b_n, b_m,ni,b_mid,0, DMG);
                                                *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"用" << it[p[P_id].box_id[i]].Name << L"命中" << b_n[ni].name << L"造成" << DMG << L"點傷害" << endl;
                                                b_n[ni].buff_check[12] += 3;
                                            }
                                        }
                                        if (te[x+I][y+J].fire == 1) {
                                            te[x + I][y + J].fire_time += 60;
                                        }
                                        else if(te[x + I][y + J].type!=1) {
                                            te[x + I][y + J].fire = 1;
                                            te[x + I][y + J].fire_time = 60;
                                        }
                                    }
                                }
                                FlushBatchDraw();
                                Sleep(500);
                                chose = "";
                                p[P_id].box_size[i]--;
                                for (int I = 0; I < b_m[b_mid].x; I++) {
                                    for (int J = 0; J < b_m[b_mid].y; J++) {
                                        if (te[I][J].dark == 2 || te[I][J].dark == 3) {
                                            te[I][J].dark = 1;
                                        }
                                    }
                                }
                                for (int I = 0; I < b_m[b_mid].psize; I++) {
                                    for (int fI = -2; fI <= 2; fI++) {
                                        for (int fJ = -2; fJ <= 2; fJ++) {
                                            if (te[p[I].x + fI][p[I].y + fJ].dark == 1) {
                                                te[p[I].x + fI][p[I].y + fJ].dark = 2;
                                            }
                                        }
                                    }
                                }
                                for (int I = 0; I < b_m[b_mid].x; I++) {
                                    for (int J = 0; J < b_m[b_mid].y; J++) {
                                        if (te[I][J].fire == 1) {
                                for (int fi = -3; fi <= 3; fi++) {
                                    for (int fj = -3; fj <= 3; fj++) {
                                        if ((te[I + fi][J + fj].dark == 1|| te[I + fi][J + fj].dark == 2) && (abs(fi) + abs(fj)) <= 3) {
                                            te[I + fi][J + fj].dark = 3;
                                        }
                                    }
                                }
                                        }

                                    }
                                }

                                return;
                                }
                            }
                        }
                        else if (m.rbutton) {
                            break;
                        }
                        else if (m.message == WM_MOUSEMOVE) {
                            if (cho[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48] == 1) {
                                putimage(0, 0, &get);
                            for (int I = -1; I <= 1;I++) {
                                for (int J = -1; J <= 1; J++) {
                                    transparentimage(NULL, m.x / 48*48+I*48, m.y / 48 * 48 + J * 48, &ao);
                                }
                            }
                            EndBatchDraw();
                            }

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
    else if(chose=="i") {
    mciSendString(L"close se", NULL, 0, NULL);
    mciSendString(L"open ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3 alias se", NULL, 0, NULL);
    vos = L"setaudio se volume to " + to_wstring(soundSize);
    mciSendString(vos.c_str(), NULL, 0, NULL);
    mciSendString(L"play se from 0", NULL, 0, NULL);
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
void p_walk(wofstream *wofs,player *p,enemy *e,b_npc *b_n,b_map *b_m,arms *ar, terrain(*te)[50], string chose, int P_id, int id, int psize, int esize,int b_mid,int bu_id,int b_nid) {
    wstring vos;
    if (chose == "w" && p[P_id].move > 0) {
            b_camera(b_m, p[P_id].x,p[P_id].y, b_mid);
            int rx, ry, m, X, Y,dr[100][100],cost[100][100],box[3000]={0}, box_id = 0, p_move;
            BeginBatchDraw();
            maps(p, P_id, e, b_m, ar, te, b_mid);
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
            e_put(e, b_m, te, esize, b_mid);
            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
            p_put(p, b_m, psize, b_mid);
            ui(p, e, b_m, P_id, esize, psize, b_mid,bu_id);
            EndBatchDraw();
            HWND hwnd = GetForegroundWindow();
            RECT rect;
            GetWindowRect(hwnd, &rect);
            int left = rect.left;
            int top = rect.top; 
            SetCursorPos(p[P_id].x*48-b_m[b_mid].ox+24+left, p[P_id].y*48 - b_m[b_mid].oy+48+top);
            int sss = 1,mx=p[P_id].x*48,my= p[P_id].y * 48;
            p_move = p[P_id].move;
            BeginBatchDraw();
            setfillcolor(RGB(0, 156, 255));
            while (sss != 0)
            {
                ExMessage m;
                m = getmessage(EM_MOUSE);
                if (m.lbutton) {
                    for (j = 0; j < 20; j++) {
                        for (i = 0; i < 15; i++) {
                            if (m.x <= j * 48 + 48 && m.x >= j * 48 && m.y <= i * 48 + 48 && m.y >= i * 48 && dr[j + b_m[b_mid].ox / 48][i + b_m[b_mid].oy / 48] == 1&&m.x<960&&m.y<720) {
                                    int k, w = 0;
                                    int xbox = p[P_id].x, ybox = p[P_id].y;             
                                    for (w = 0; w < box_id; w++) {
                                        if (box[w] == 2) {
                                            if (p[P_id].move >= te[xbox][ybox + 1].mA) {
                                                p[P_id].move = p[P_id].move - te[xbox][ybox + 1].mA;
                                                if (te[xbox][ybox + 1].fire == 1) {
                                                    p[P_id].buff_check[12]++;
                                                }
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                        else if (box[w] == 4) {
                                            if (p[P_id].move >= te[xbox - 1][ybox].mA) {
                                                p[P_id].move =p[P_id].move - te[xbox - 1][ybox].mA;
                                                if (te[xbox-1][ybox].fire == 1) {
                                                    p[P_id].buff_check[12]++;
                                                }
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                        else if (box[w] == 6) {
                                            if (p[P_id].move >= te[xbox + 1][ybox].mA) {
                                                p[P_id].move = p[P_id].move - te[xbox + 1][ybox].mA;
                                                if (te[xbox+1][ybox].fire == 1) {
                                                    p[P_id].buff_check[12]++;
                                                }
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                        else if (box[w] == 8) {
                                            if (p[P_id].move >= te[xbox][ybox - 1].mA) {
                                                p[P_id].move = p[P_id].move - te[xbox][ybox - 1].mA;
                                                if (te[xbox][ybox - 1].fire == 1) {
                                                    p[P_id].buff_check[12]++;
                                                }
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                        for (int I = -2; I <= 2; I++) {
                                            for (int J = -2; J <= 2; J++) {
                                            if (te[xbox+I][ybox+J].dark == 2) {
                                                te[xbox+I][ybox+J].dark = 1;
                                            }
                                            }
                                        }
                                        if (box[w] == 2) {
                                            for (int I = -2; I <= 2; I++) {
                                                for (int J = -2; J <= 2; J++) {
                                                    if (te[xbox + I][ybox + J+1].dark == 1) {
                                                        te[xbox + I][ybox + J+1].dark = 2;
                                                    }
                                                }
                                            }
                                            for (k = 0; k <= 48; k += 16) {
                                                BeginBatchDraw();
                                                maps(p, P_id, e, b_m, ar, te, b_mid);
                                                e_put(e, b_m, te, esize, b_mid);
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
                                            for (int I = -2; I <= 2; I++) {
                                                for (int J = -2; J <= 2; J++) {
                                                    if (te[xbox + I-1][ybox + J].dark == 1) {
                                                        te[xbox + I-1][ybox + J].dark = 2;
                                                    }
                                                }
                                            }
                                            for (k = 0; k <= 48; k += 16) {

                                                BeginBatchDraw();
                                                maps(p, P_id, e, b_m, ar, te, b_mid);
                                                e_put(e, b_m, te, esize, b_mid);
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
                                            for (int I = -2; I <= 2; I++) {
                                                for (int J = -2; J <= 2; J++) {
                                                    if (te[xbox + I+1][ybox + J].dark == 1) {
                                                        te[xbox + I+1][ybox + J].dark = 2;
                                                    }
                                                }
                                            }
                                            for (k = 0; k <= 48; k += 16) {

                                                BeginBatchDraw();
                                                maps(p, P_id, e, b_m, ar, te,b_mid);
                                                e_put(e, b_m, te, esize, b_mid);
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
                                            for (int I = -2; I <= 2; I++) {
                                                for (int J = -2; J <= 2; J++) {
                                                    if (te[xbox + I][ybox + J-1].dark == 1) {
                                                        te[xbox + I][ybox + J-1].dark = 2;
                                                    }
                                                }
                                            }
                                            for (k = 0; k <= 48; k += 16) {
                                                BeginBatchDraw();
                                                maps(p, P_id, e, b_m, ar, te, b_mid);
                                                e_put(e, b_m, te, esize, b_mid);
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
                                        if (te[xbox][ybox].trap == 2) {
                                            p[P_id].move = 0;
                                            p[P_id].buff_check[27]++;
                                            te[xbox][ybox].trap = 0;
                                            *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"觸發狩獵陷阱" << endl;
                                            int dmg = 5;
                                            pHP(p, b_m, P_id, b_mid, 0, dmg);
                                            FlushBatchDraw();
                                            Sleep(500);
                                            if (p[P_id].hp <= 0 && p[P_id].buff_check[14] == 0) {
                                                p[P_id].buff_check[14] = 1;
                                                p[P_id].buff_time[p[P_id].buff_Size] = 15;
                                                p[P_id].buff_id[p[P_id].buff_Size] = 14;
                                                p[P_id].buff_Size++;
                                            }
                                        }
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
                                    *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"從(" << p[P_id].x << L"," << p[P_id].y << L")到(" << xbox << L"," << ybox << L")" << endl;
                                    p[P_id].x = xbox; p[P_id].y = ybox;
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
                }
                if (m.rbutton) {
                    sss = 0;
                }
                if (m.message== WM_MOUSEMOVE) {
                    if (te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].type != 1 && te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].player != 1 && te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].npc != 1 && te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].enemy != 1&&m.x<960&&m.y<720) {
                    if (m.x < mx - b_m[b_mid].ox) {
                        if (box[box_id - 1] ==6&&m.x/48+b_m[b_mid].ox /48== mx / 48 - 1 && m.y / 48 + b_m[b_mid].oy / 48 == my / 48) {
                            p_move += te[m.x / 48+b_m[b_mid].ox/48][m.y / 48 + b_m[b_mid].oy / 48].mA;
                            box_id--;
                            mx = m.x / 48 * 48+b_m[b_mid].ox;
                        }
                        else if(p_move>=te[m.x/48+b_m[b_mid].ox/48][m.y/48 + b_m[b_mid].oy / 48].mA && m.x / 48 + b_m[b_mid].ox / 48 == mx / 48 - 1 && m.y / 48 + b_m[b_mid].oy / 48 == my / 48) {
                            box[box_id] = 4;
                            box_id++;
                            mx = m.x / 48 * 48 + b_m[b_mid].ox;
                            p_move -= te[mx / 48][my / 48].mA;
                        }
                        q.push({ mx / 48,my / 48 });
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
                            if (cost[X + 1][Y] > cost[X][Y] + te[X + 1][Y].mA && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x) {
                                cost[X + 1][Y] = cost[X][Y] + te[X + 1][Y].mA;
                                q.push({ X + 1,Y });
                            }
                            if (cost[X - 1][Y] > cost[X][Y] + te[X - 1][Y].mA && te[X - 1][Y].type != 1 && X - 1 >= 0) {
                                cost[X - 1][Y] = cost[X][Y] + te[X - 1][Y].mA;
                                q.push({ X - 1,Y });
                            }
                            if (cost[X][Y + 1] > cost[X][Y] + te[X][Y + 1].mA && te[X][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y) {
                                cost[X][Y + 1] = cost[X][Y] + te[X][Y + 1].mA;
                                q.push({ X ,Y + 1 });
                            }
                            if (cost[X][Y - 1] > cost[X][Y] + te[X][Y - 1].mA && te[X][Y - 1].type != 1 && Y - 1 >= 0) {
                                cost[X][Y - 1] = cost[X][Y] + te[X][Y - 1].mA;
                                q.push({ X ,Y - 1 });
                            }
                            if (dr[X][Y] == 0 && te[X][Y].enemy == 0 && te[X][Y].npc == 0 && te[X][Y].player == 0 && cost[X][Y] <= p_move) {
                                dr[X][Y] = 1;
                            }
                            q.pop();
                        }
                        maps(p, P_id, e, b_m, ar, te, b_mid);
                        for (i = 0; i < b_m[b_mid].x; i++) {
                            for (j = 0; j < b_m[b_mid].y; j++) {
                                if (dr[i][j] == 1) {
                                    transparentimage(NULL, 48 * i - b_m[b_mid].ox, 48 * j - b_m[b_mid].oy, &mb);
                                }
                            }
                        }
                            int px=p[P_id].x*48, py=p[P_id].y*48;
                        for (i = 0; i < box_id; i++) {
                            if (box[i] == 2) {
                                 py+=48;
                                 POINT pts[] = { {px-b_m[b_mid].ox+20, py-b_m[b_mid].oy},{px  - b_m[b_mid].ox + 26, py  - b_m[b_mid].oy}, {px - b_m[b_mid].ox + 26, py  - b_m[b_mid].oy+30}, {px - b_m[b_mid].ox + 20, py  - b_m[b_mid].oy + 30} };
                                 solidpolygon(pts, 4);    
                                 POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 45} };
                                 solidpolygon(pts3, 3);
                            }
                            else if (box[i] == 4) {
                                px-=48;
                                POINT pts[] = { {px - b_m[b_mid].ox+18 , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox +18, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 20} };
                                solidpolygon(pts, 4);
                                POINT pts3[] = { {px - b_m[b_mid].ox + 18, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 18,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 3, py - b_m[b_mid].oy + 23} };
                                solidpolygon(pts3, 3);
                            }
                            else if (box[i] == 6) {
                                px+=48;
                                POINT pts[] = { {px - b_m[b_mid].ox , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox , py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 20} };
                                solidpolygon(pts, 4);
                                POINT pts3[] = { {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 30,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 45, py - b_m[b_mid].oy + 23} };
                                solidpolygon(pts3, 3);
                            }
                            else if (box[i] == 8) {
                                py-=48;
                                POINT pts[] = { {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy+18},{px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy+18}, {px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 48}, {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 48} };
                                solidpolygon(pts, 4);
                                POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 3} };
                                solidpolygon(pts3, 3);
                            }
                        }
                        e_put(e, b_m, te, esize, b_mid);
                        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, psize, b_mid);
                        ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                        FlushBatchDraw();
                    }
                    else if (m.y < my - b_m[b_mid].oy) {
                        if (box[box_id - 1] == 2 && m.x / 48 + b_m[b_mid].ox/48 == mx / 48&& m.y / 48 + b_m[b_mid].oy / 48 == my / 48-1) {
                            p_move += te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].mA;
                            box_id--;
                            my = m.y / 48 * 48 + b_m[b_mid].oy;
                        }
                        else if(p_move >= te[m.x / 48][m.y / 48].mA && m.x / 48 + b_m[b_mid].ox / 48 == mx / 48 && m.y / 48 + b_m[b_mid].oy / 48 == my / 48-1) {
                            box[box_id] = 8;
                            box_id++;
                            my = m.y / 48 * 48 + b_m[b_mid].oy;
                            p_move -= te[mx / 48][my / 48].mA;
                        }
                        q.push({ mx / 48,my / 48 });
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
                            if (cost[X + 1][Y] > cost[X][Y] + te[X + 1][Y].mA && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x) {
                                cost[X + 1][Y] = cost[X][Y] + te[X + 1][Y].mA;
                                q.push({ X + 1,Y });
                            }
                            if (cost[X - 1][Y] > cost[X][Y] + te[X - 1][Y].mA && te[X - 1][Y].type != 1 && X - 1 >= 0) {
                                cost[X - 1][Y] = cost[X][Y] + te[X - 1][Y].mA;
                                q.push({ X - 1,Y });
                            }
                            if (cost[X][Y + 1] > cost[X][Y] + te[X][Y + 1].mA && te[X][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y) {
                                cost[X][Y + 1] = cost[X][Y] + te[X][Y + 1].mA;
                                q.push({ X ,Y + 1 });
                            }
                            if (cost[X][Y - 1] > cost[X][Y] + te[X][Y - 1].mA && te[X][Y - 1].type != 1 && Y - 1 >= 0) {
                                cost[X][Y - 1] = cost[X][Y] + te[X][Y - 1].mA;
                                q.push({ X ,Y - 1 });
                            }
                            if (dr[X][Y] == 0 && te[X][Y].enemy == 0 && te[X][Y].npc == 0 && te[X][Y].player == 0 && cost[X][Y] <= p_move) {
                                dr[X][Y] = 1;
                            }
                            q.pop();
                        }
                        maps(p, P_id, e, b_m, ar, te, b_mid);
                        for (i = 0; i < b_m[b_mid].x; i++) {
                            for (j = 0; j < b_m[b_mid].y; j++) {
                                if (dr[i][j] == 1) {
                                    transparentimage(NULL, 48 * i - b_m[b_mid].ox, 48 * j - b_m[b_mid].oy, &mb);
                                }
                            }
                        }
                        int px = p[P_id].x * 48, py = p[P_id].y * 48;
                        for (i = 0; i < box_id; i++) {
                            if (box[i] == 2) {
                                py += 48;
                                POINT pts[] = { {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy},{px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy}, {px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 30} };
                                solidpolygon(pts, 4);
                                POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 45} };
                                solidpolygon(pts3, 3);
                            }
                            else if (box[i] == 4) {
                                px -= 48;
                                POINT pts[] = { {px - b_m[b_mid].ox + 18 , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox + 18, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 20} };
                                solidpolygon(pts, 4);
                                POINT pts3[] = { {px - b_m[b_mid].ox + 18, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 18,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 3, py - b_m[b_mid].oy + 23} };
                                solidpolygon(pts3, 3);
                            }
                            else if (box[i] == 6) {
                                px += 48;
                                POINT pts[] = { {px - b_m[b_mid].ox , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox , py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 20} };
                                solidpolygon(pts, 4);
                                POINT pts3[] = { {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 30,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 45, py - b_m[b_mid].oy + 23} };
                                solidpolygon(pts3, 3);
                            }
                            else if (box[i] == 8) {
                                py -= 48;
                                POINT pts[] = { {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 18},{px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 48}, {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 48} };
                                solidpolygon(pts, 4);
                                POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 3} };
                                solidpolygon(pts3, 3);
                            }
                        }
                        e_put(e, b_m, te, esize, b_mid);
                        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, psize, b_mid);
                        ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                        FlushBatchDraw();
                    }
                    else if (m.x > mx + 48 - b_m[b_mid].ox) {
                        if (box[box_id - 1] == 4 && m.x / 48 + b_m[b_mid].ox / 48 == mx / 48+1 && m.y / 48 + b_m[b_mid].oy / 48 == my / 48) {
                            p_move += te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].mA;
                            box_id--;
                            mx = m.x / 48 * 48 + b_m[b_mid].ox;
                        }
                        else if(p_move >= te[m.x / 48][m.y / 48].mA && m.x / 48 + b_m[b_mid].ox / 48 == mx / 48+1 && m.y / 48 + b_m[b_mid].oy / 48 == my / 48) {
                            box[box_id] = 6;
                            box_id++;
                            mx = m.x / 48 * 48+b_m[b_mid].ox;
                            p_move -= te[mx / 48][my / 48].mA;
                        }
                        q.push({ mx / 48,my / 48 });
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
                            if (cost[X + 1][Y] > cost[X][Y] + te[X + 1][Y].mA && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x) {
                                cost[X + 1][Y] = cost[X][Y] + te[X + 1][Y].mA;
                                q.push({ X + 1,Y });
                            }
                            if (cost[X - 1][Y] > cost[X][Y] + te[X - 1][Y].mA && te[X - 1][Y].type != 1 && X - 1 >= 0) {
                                cost[X - 1][Y] = cost[X][Y] + te[X - 1][Y].mA;
                                q.push({ X - 1,Y });
                            }
                            if (cost[X][Y + 1] > cost[X][Y] + te[X][Y + 1].mA && te[X][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y) {
                                cost[X][Y + 1] = cost[X][Y] + te[X][Y + 1].mA;
                                q.push({ X ,Y + 1 });
                            }
                            if (cost[X][Y - 1] > cost[X][Y] + te[X][Y - 1].mA && te[X][Y - 1].type != 1 && Y - 1 >= 0) {
                                cost[X][Y - 1] = cost[X][Y] + te[X][Y - 1].mA;
                                q.push({ X ,Y - 1 });
                            }
                            if (dr[X][Y] == 0 && te[X][Y].enemy == 0 && te[X][Y].npc == 0 && te[X][Y].player == 0 && cost[X][Y] <= p_move) {
                                dr[X][Y] = 1;
                            }
                            q.pop();
                        }
                        maps(p, P_id, e, b_m, ar, te, b_mid);
                        for (i = 0; i < b_m[b_mid].x; i++) {
                            for (j = 0; j < b_m[b_mid].y; j++) {
                                if (dr[i][j] == 1) {
                                    transparentimage(NULL, 48 * i - b_m[b_mid].ox, 48 * j - b_m[b_mid].oy, &mb);
                                }
                            }
                        }
                        int px = p[P_id].x * 48, py = p[P_id].y * 48;
                        for (i = 0; i < box_id; i++) {
                            if (box[i] == 2) {
                                py += 48;
                                POINT pts[] = { {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy},{px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy}, {px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 30} };
                                solidpolygon(pts, 4);
                                POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 45} };
                                solidpolygon(pts3, 3);
                            }
                            else if (box[i] == 4) {
                                px -= 48;
                                POINT pts[] = { {px - b_m[b_mid].ox + 18 , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox + 18, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 20} };
                                solidpolygon(pts, 4);
                                POINT pts3[] = { {px - b_m[b_mid].ox + 18, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 18,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 3, py - b_m[b_mid].oy + 23} };
                                solidpolygon(pts3, 3);
                            }
                            else if (box[i] == 6) {
                                px += 48;
                                POINT pts[] = { {px - b_m[b_mid].ox , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox , py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 20} };
                                solidpolygon(pts, 4);
                                POINT pts3[] = { {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 30,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 45, py - b_m[b_mid].oy + 23} };
                                solidpolygon(pts3, 3);
                            }
                            else if (box[i] == 8) {
                                py -= 48;
                                POINT pts[] = { {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 18},{px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 48}, {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 48} };
                                solidpolygon(pts, 4);
                                POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 3} };
                                solidpolygon(pts3, 3);
                            }
                        }
                        e_put(e, b_m, te, esize, b_mid);
                        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, psize, b_mid);
                        ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                        FlushBatchDraw();
                    }
                    else if (m.y > my + 48-b_m[b_mid].oy) {
                        if (box[box_id - 1] == 8 && m.x / 48 + b_m[b_mid].ox / 48 == mx / 48 && m.y / 48 + b_m[b_mid].oy / 48 == my / 48+1) {
                            p_move += te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].mA;
                            box_id--;
                            my = m.y / 48 * 48 + b_m[b_mid].oy;
                        }
                        else if(p_move >= te[m.x / 48][m.y / 48].mA && m.x / 48 + b_m[b_mid].ox / 48 == mx / 48 && m.y / 48 + b_m[b_mid].oy / 48 == my / 48+1) {
                            box[box_id] = 2;
                            box_id++;
                            my = m.y / 48 * 48 + b_m[b_mid].oy;
                            p_move -= te[mx / 48][my / 48].mA;
                        }
                        q.push({ mx / 48,my / 48 });
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
                            if (cost[X + 1][Y] > cost[X][Y] + te[X + 1][Y].mA && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x) {
                                cost[X + 1][Y] = cost[X][Y] + te[X + 1][Y].mA;
                                q.push({ X + 1,Y });
                            }
                            if (cost[X - 1][Y] > cost[X][Y] + te[X - 1][Y].mA && te[X - 1][Y].type != 1 && X - 1 >= 0) {
                                cost[X - 1][Y] = cost[X][Y] + te[X - 1][Y].mA;
                                q.push({ X - 1,Y });
                            }
                            if (cost[X][Y + 1] > cost[X][Y] + te[X][Y + 1].mA && te[X][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y) {
                                cost[X][Y + 1] = cost[X][Y] + te[X][Y + 1].mA;
                                q.push({ X ,Y + 1 });
                            }
                            if (cost[X][Y - 1] > cost[X][Y] + te[X][Y - 1].mA && te[X][Y - 1].type != 1 && Y - 1 >= 0) {
                                cost[X][Y - 1] = cost[X][Y] + te[X][Y - 1].mA;
                                q.push({ X ,Y - 1 });
                            }
                            if (dr[X][Y] == 0 && te[X][Y].enemy == 0 && te[X][Y].npc == 0 && te[X][Y].player == 0 && cost[X][Y] <= p_move) {
                                dr[X][Y] = 1;
                            }
                            q.pop();
                        }
                        maps(p, P_id, e, b_m, ar, te, b_mid);
                        for (i = 0; i < b_m[b_mid].x; i++) {
                            for (j = 0; j < b_m[b_mid].y; j++) {
                                if (dr[i][j] == 1) {
                                    transparentimage(NULL, 48 * i - b_m[b_mid].ox, 48 * j - b_m[b_mid].oy, &mb);
                                }
                            }
                        }
                        int px = p[P_id].x * 48, py = p[P_id].y * 48;
                        for (i = 0; i < box_id; i++) {
                            if (box[i] == 2) {
                                py += 48;
                                POINT pts[] = { {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy},{px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy}, {px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 30} };
                                solidpolygon(pts, 4);
                                POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 45} };
                                solidpolygon(pts3, 3);
                            }
                            else if (box[i] == 4) {
                                px -= 48;
                                POINT pts[] = { {px - b_m[b_mid].ox + 18 , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox + 18, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 20} };
                                solidpolygon(pts, 4);
                                POINT pts3[] = { {px - b_m[b_mid].ox + 18, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 18,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 3, py - b_m[b_mid].oy + 23} };
                                solidpolygon(pts3, 3);
                            }
                            else if (box[i] == 6) {
                                px += 48;
                                POINT pts[] = { {px - b_m[b_mid].ox , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox , py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 20} };
                                solidpolygon(pts, 4);
                                POINT pts3[] = { {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 30,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 45, py - b_m[b_mid].oy + 23} };
                                solidpolygon(pts3, 3);
                            }
                            else if (box[i] == 8) {
                                py -= 48;
                                POINT pts[] = { {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 18},{px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 48}, {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 48} };
                                solidpolygon(pts, 4);
                                POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 3} };
                                solidpolygon(pts3, 3);
                            }
                        }
                        e_put(e, b_m, te, esize, b_mid);
                        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, psize, b_mid);
                        ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                        FlushBatchDraw();
                    }
                    }
                    else if (m.x<p[P_id].x * 48 + 48 - b_m[b_mid].ox && m.x > p[P_id].x * 48 - b_m[b_mid].ox && m.y<p[P_id].y * 48 - b_m[b_mid].oy + 48 && m.y>p[P_id].y * 48 - b_m[b_mid].oy && m.x < 960 && m.y < 720&&box_id==1) {
                        box_id = 0;
                        box[0] = 0;
                        mx = p[P_id].x * 48, my = p[P_id].y * 48;
                        p_move = p[P_id].move;
                        q.push({ mx / 48,my / 48 });
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
                            if (cost[X + 1][Y] > cost[X][Y] + te[X + 1][Y].mA && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x) {
                                cost[X + 1][Y] = cost[X][Y] + te[X + 1][Y].mA;
                                q.push({ X + 1,Y });
                            }
                            if (cost[X - 1][Y] > cost[X][Y] + te[X - 1][Y].mA && te[X - 1][Y].type != 1 && X - 1 >= 0) {
                                cost[X - 1][Y] = cost[X][Y] + te[X - 1][Y].mA;
                                q.push({ X - 1,Y });
                            }
                            if (cost[X][Y + 1] > cost[X][Y] + te[X][Y + 1].mA && te[X][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y) {
                                cost[X][Y + 1] = cost[X][Y] + te[X][Y + 1].mA;
                                q.push({ X ,Y + 1 });
                            }
                            if (cost[X][Y - 1] > cost[X][Y] + te[X][Y - 1].mA && te[X][Y - 1].type != 1 && Y - 1 >= 0) {
                                cost[X][Y - 1] = cost[X][Y] + te[X][Y - 1].mA;
                                q.push({ X ,Y - 1 });
                            }
                            if (dr[X][Y] == 0 && te[X][Y].enemy == 0 && te[X][Y].npc == 0 && te[X][Y].player == 0 && cost[X][Y] <= p_move) {
                                dr[X][Y] = 1;
                            }
                            q.pop();
                        }
                        maps(p, P_id, e, b_m, ar, te, b_mid);
                        for (i = 0; i < b_m[b_mid].x; i++) {
                            for (j = 0; j < b_m[b_mid].y; j++) {
                                if (dr[i][j] == 1) {
                                    transparentimage(NULL, 48 * i - b_m[b_mid].ox, 48 * j - b_m[b_mid].oy, &mb);
                                }
                            }
                        }
                        e_put(e, b_m, te, esize, b_mid);
                        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, psize, b_mid);
                        ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                        FlushBatchDraw();
                    }
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
void e_attack(wofstream *wofs,arms *ar,armor *Ar,player *p,enemy *e,b_npc *b_n,b_map *b_m, terrain(*te)[50],enemy_type *e_t, int b_mid, int id, int P_id, int esize, int psize, int bu_id, int b_nid) {
    int rx, ry, ATK, AC, ax, ay,sb=0;
    wstring nn,vos,mm;
    IMAGE get;
    TCHAR t[5];
    RECT rect;
    ExMessage m1;
    if (e[id].hp > 0) {
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
            for (int I = 0; I < b_m[b_mid].psize; I++) {
                if (p[I].buff_check[22] > 0) {
                    int range = 0;
                    int ar_id = -1;
                    if (p[I].arms_id_1 != -1) {
                        if (p[I].arms_b_1 > 0) {                      
                            range = ar[p[I].arms_id_1].range;
                            if (p[I].s_check[13] > 0) { 
                                if (ar[p[I].arms_id_1].type[0] == 'r') {
                                    range++;
                                }
                            }     
                            ar_id = p[I].arms_id_1;
                        }
                    }
                    if (p[I].arms_id_2 != -1) {
                        if (p[I].arms_b_2 > 0) {
                            int rangeb = ar[p[I].arms_id_2].range;
                            if (ar[p[I].arms_id_2].type[0] == 'r') {
                                rangeb++;
                            }
                            if (range <rangeb ) {
                                range = rangeb;
                                ar_id = p[I].arms_id_2;
                            }
                        }
                    }
                    if (abs(p[I].x - e[id].x) + abs(p[I].y - e[id].y) <= range && ar_id != -1) {
                        sb = 1;
                        int AC = roll("", 2) + (e[id].dex - 10) / 2;
                        if (e[id].buff_check[20] == 1) {
                            AC = 0;
                        }
                        if (e[id].buff_check[27] > 0) {
                            AC /= 2;
                        }
                        int ATK;
                        if (ar[ar_id].type[0] == 'r') {
                            ATK = roll("", 2) + (p[I].dex - 10) / 2 + roll(ar[ar_id].hit, 1);
                        }
                        else if (ar[ar_id].type[0] == 'm') {
                            ATK = roll("", 2) + (p[I].str - 10) / 2 + roll(ar[ar_id].hit, 1);;
                        }
                        wstring vos;
                        IMAGE ae;
                        if (ar_id == 0 || ar_id == 5) {
                            mciSendString(L"close se", NULL, 0, NULL);
                            mciSendString(L"open ./Game/Sound/SE/拳銃2.mp3 alias se", NULL, 0, NULL);
                            vos = L"setaudio se volume to " + to_wstring(soundSize);
                            mciSendString(vos.c_str(), NULL, 0, NULL);
                            mciSendString(L"play se from 0", NULL, 0, NULL);
                            loadimage(&ae, L"./Game/picture/武器・弓1.png", 0, 0, false);
                            BeginBatchDraw();
                            for (j = 0; j < 5; j++) {
                                for (i = 0; i < 3; i++) {
                                    maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                                    e_put(e, b_m, te, esize, b_mid);
                                    b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                    p_put(p, b_m, psize, b_mid);
                                    transparentimageA(NULL, e[id].x * 48 - b_m[b_mid].ox - 30, e[id].y * 48 - b_m[b_mid].oy - 160, &ae, i * 120, j * 300, 120, 300);
                                    ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                                    FlushBatchDraw();
                                    Sleep(5);
                                }
                            }
                            EndBatchDraw();
                        }
                        else if (ar_id == 4 || ar_id == 6) {
                            mciSendString(L"close se", NULL, 0, NULL);
                            mciSendString(L"open ./Game/Sound/SE/剣の素振り3.mp3 alias se", NULL, 0, NULL);
                            vos = L"setaudio se volume to " + to_wstring(soundSize);
                            mciSendString(vos.c_str(), NULL, 0, NULL);
                            mciSendString(L"play se from 0", NULL, 0, NULL);
                            loadimage(&ae, L"./Game/picture/技・一閃.png", 0, 0, false);
                            BeginBatchDraw();
                            for (j = 0; j < 5; j++) {
                                for (i = 0; i < 3; i++) {
                                    maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                                    e_put(e, b_m, te, esize, b_mid);
                                    b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                    p_put(p, b_m, psize, b_mid);
                                    transparentimageA(NULL, e[id].x * 48 - b_m[b_mid].ox - 110, e[id].y * 48 - b_m[b_mid].oy - 47, &ae, i * 320, j * 120, 320, 120);
                                    ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                                    FlushBatchDraw();
                                    Sleep(5);
                                }
                            }
                            EndBatchDraw();
                        }
                        else if (ar_id == 8) {
                            BeginBatchDraw();
                            maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                            e_put(e, b_m, te,esize, b_mid);
                            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                            p_put(p, b_m, psize, b_mid);
                            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                            EndBatchDraw();
                            getimage(&get, 0, 0, 1296, 960);
                            loadimage(&ae, L"./Game/picture/杖1.png", 0, 0, false);
                            mciSendString(L"close se", NULL, 0, NULL);
                            mciSendString(L"open ./Game/Sound/SE/軽いパンチ2.mp3 alias se", NULL, 0, NULL);
                            vos = L"setaudio se volume to " + to_wstring(soundSize);
                            mciSendString(vos.c_str(), NULL, 0, NULL);
                            mciSendString(L"play se from 0", NULL, 0, NULL);
                            for (int ej = 0; ej < 4; ej++) {
                                for (int ei = 0; ei < 5; ei++) {
                                    putimage(0, 0, &get);
                                    transparentimageA(NULL, 48 * e[id].x - b_m[b_mid].ox - 10, e[id].y * 48 - b_m[b_mid].oy - 30, &ae, ei * 80, ej * 80, 80, 80);
                                    FlushBatchDraw();
                                    Sleep(40);
                                }
                            }
                        }
                        /*ae*/
                        if (ATK > AC) {
                            int dmg = roll(ar[ar_id].dmg, 1);
                            if (ar[ar_id].type[0] == 'm') {
                                dmg += (p[P_id].str - 10) / 5;
                            }
                            if (dmg >= e[id].buff_check[7] + e[id].buff_check[10]) {
                                dmg -= e[id].buff_check[7] + e[id].buff_check[10];
                            }
                            else {
                                dmg = 0;
                            }
                            if (p[I].buff_check[21] > 0) {
                                dmg += (p[P_id].wis - 5) / 2;
                            }
                            eHP(e, b_m, id, b_mid, 0, dmg);
                            *wofs << L"(T" << b_m[b_mid].time << L")" << p[I].name << L"用" << ar[ar_id].name << L"命中" << e[id].name << L"造成" << dmg << L"點傷害(" << ATK << ">" << AC << ")" << endl;
                            EndBatchDraw();
                            if (ar[ar_id].type[0] == 'r') {
                                if (ar_id == p[I].arms_id_1 && p[I].arms_b_1 > 0) {
                                    p[I].arms_b_1--;
                                }
                                else {
                                    p[I].arms_b_2--;
                                }
                            }
                            Sleep(500);
                            if (e[id].hp <= 0) {
                                te[e[id].x][e[id].y].mA -= 10000;
                                te[e[id].x][e[id].y].mB -= 10000;
                                int x = e[id].x, y = e[id].y;
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
                                te[e[id].x][e[id].y].enemy = 0;
                                e[id].x = -1; e[id].y = -1;
                                e[id].speed = -1;
                                if (e[id].buff_check[20] > 0) {
                                    p[0].buff_check[20] = 0;
                                    e[id].buff_check[20] = 0;
                                }
                                return;
                            }
                        }
                        else {
                            *wofs << L"(T" << b_m[b_mid].time << L")" << p[I].name << L"用" << ar[ar_id].name << L"攻擊" << e[id].name << L"未能命中(" << ATK << "<=" << AC << ")" << endl;
                            if (ar[ar_id].type[0] == 'r') {
                                if (ar_id == p[I].arms_id_1 && p[I].arms_b_1 > 0) {
                                    p[I].arms_b_1--;
                                }
                                else {
                                    p[I].arms_b_2--;
                                }
                            }
                            mm = L"miss";
                            settextcolor(RGB(255, 0, 0));
                            settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                            outtextxy(e[id].x * 48 - b_m[b_mid].ox, e[id].y * 48 - b_m[b_mid].oy - 40, mm.c_str());
                            settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                            settextcolor(RGB(0, 0, 0));
                            Sleep(500);
                        }
                        p[I].buff_check[22]--;
                    }
                }
            }
            int sss = 1;
            BeginBatchDraw();
            int hx, hy, ha, hc, hv, hd=0, hcv=0, hrc, absa, wheel = 0;
            hv =  p[P_id].buff_check[2];
            if (hv > 0) {
            int ar_id = e[id].baid;
                hcv=EKTVS(p, e, b_m, ar, e_t,Ar, P_id, id,b_mid, ar_id, 0,ar[ar_id].Dmg);
            }
            while (1) {
            if (ar[e[id].baid].type[0] == 'r') {
                ATK = roll("", 2) + roll(ar[e[id].baid].hit, 1)+(e[id].dex-10)/2;
            }
            else if (ar[e[id].baid].type[0] == 'm') {
                ATK = roll("", 2) + roll(ar[e[id].baid].hit, 1)+ (e[id].str - 10) / 2;
            }
            AC = roll("", 2) + (p[P_id].dex - 10) / 2+ Ar[p[P_id].armor_id].EDV;
            if (p[P_id].buff_check[27] == 1) {
                AC /= 2;
            }
            if (p[P_id].buff_check[14] == 1||p[P_id].buff_check[20]==1) {
                AC = 0;
            }
            if (hcv == 0) {
                break;
            }
            if (hcv < 0) {
                if (ATK > AC) {
                    break;
                }
                hcv++;
            }
            if (hcv > 0) {
                if (ATK <= AC) {
                    break;
                }
                hcv--;
            }            
            }
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
                        maps(p, P_id, e, b_m, ar, te,b_mid);/*地圖繪製*/
                        e_put(e, b_m, te, esize, b_mid);
                        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, psize, b_mid); 
                        transparentimageA(NULL, p[P_id].x * 48 - b_m[b_mid].ox - 25, p[P_id].y * 48 - 16 - b_m[b_mid].oy - 20, &ae, i * 120, j * 120, 120, 120);
                        ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);                       
                        EndBatchDraw();
                        Sleep(5);
                    }
                }
            }
            if (ATK > AC) {
                BeginBatchDraw();
                maps(p, P_id, e,b_m, ar, te,b_mid);/*地圖繪製*/
                e_put(e, b_m, te, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                int  DMG = roll(ar[e[id].baid].dmg, 1);           
                if (ar[e[id].baid].type[0] == 'm') {
                    DMG += (e[id].str - 10) / 5;
                }
                pHP(p,b_m, P_id,b_mid, 0, DMG);
                *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"用"<<ar[e[id].baid].name<<L"命中"<<p[P_id].name<<L"造成"<<DMG<<L"點傷害("<<ATK<<">"<<AC<<")" << endl;
                EndBatchDraw();
                if (e[id].baid == 7) {
                    p[P_id].buff_check[31]++;
                }
                IMAGE tri;
                loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                transparentimage(NULL, (e[id].x) * 48 - b_m[b_mid].ox, ((e[id].y - 1) * 48) - b_m[b_mid].oy-40, &tri);
                if (p[P_id].hp <= 0 && p[P_id].buff_check[14] == 0) {
                    p[P_id].buff_check[14] = 1;
                    p[P_id].buff_time[p[P_id].buff_Size] = 15;
                    p[P_id].buff_id[p[P_id].buff_Size] = 14;
                    p[P_id].buff_Size++;
                }
                else if (p[P_id].hp <= 0 && p[P_id].buff_check[14] == 1) {
                    *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"退出戰鬥" << endl;
                    if (p[P_id].buff_check[20] > 0) {
                        p[P_id].buff_check[20] = 0;
                        for (int lk = 0; lk < b_m[b_mid].esize; lk++) {
                            if (e[lk].buff_check[20] > 0) {
                                e[lk].buff_check[20] = 0;
                            }
                        }
                    }
                    te[p[P_id].x][p[P_id].y].mA -= 10000;
                    te[p[P_id].x][p[P_id].y].mB -= 10000;
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
                    te[p[P_id].x][p[P_id].y].player = 0;
                    p[P_id].x = -1; p[P_id].y = -1;                    
                }
                Sleep(1000);
            }
            else {         
                BeginBatchDraw();
                maps(p, P_id, e, b_m, ar, te,b_mid);/*地圖繪製*/
                e_put(e, b_m, te, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"用" <<ar[e[id].baid].name<<L"攻擊"<<p[P_id].name<<L"未能命中("<<ATK<<"<="<<AC<<")" << endl;
                nn = L"miss";
                settextcolor(RGB(255, 0, 0));
                settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                outtextxy(p[P_id].x * 48 - b_m[b_mid].ox, p[P_id].y * 48 - b_m[b_mid].oy - 40, nn.c_str());
                settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                settextcolor(RGB(0, 0, 0));
                EndBatchDraw();
                IMAGE tri;
                loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                transparentimage(NULL, (e[id].x) * 48-b_m[b_mid].ox, ((e[id].y - 1) * 48) - b_m[b_mid].oy, &tri);
                Sleep(1000);
            }
            if (ar[e[id].baid].type[0] == 'r') {
                e[id].b_num_1--;
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
            if (ar[e[id].baid].type[0] == 'r') {
                ATK = roll("", 2) + roll(ar[e[id].baid].hit, 1) + (e[id].dex - 10) / 2;
            }
            else if (ar[e[id].baid].type[0] == 'm') {
                ATK = roll("", 2) + roll(ar[e[id].baid].hit, 1) + (e[id].str - 10) / 2;
            }
            AC = roll("", 2) + (b_n[b_nid].dex - 10) / 2;
            if (b_n[b_nid].buff_check[9] > 0) {
                AC = 0;
            }
            if (b_n[b_nid].buff_check[27] > 0) {
                AC /= 2;
            }
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
                        maps(p, P_id, e, b_m, ar, te,b_mid);/*地圖繪製*/
                        e_put(e, b_m, te, esize, b_mid);
                        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, psize, b_mid);
                        transparentimageA(NULL, b_n[b_nid].x * 48 - b_m[b_mid].ox - 25, b_n[b_nid].y * 48 - 16 - b_m[b_mid].oy - 20, &ae, i * 120, j * 120, 120, 120);
                        ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                        EndBatchDraw();
                        Sleep(5);
                    }
                }
            }
            if (ATK > AC) {
                BeginBatchDraw();
                maps(p, P_id, e, b_m, ar, te,b_mid);/*地圖繪製*/
                e_put(e, b_m, te, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                int  DMG = roll(ar[e[id].baid].dmg, 1);
                if (ar[e[id].baid].type[0] == 'm') {
                    DMG += (e[id].str - 10) / 5;
                }
                nHP(b_n, b_m, b_nid, b_mid, 0, DMG);
                *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"用" << ar[e[id].baid].name << L"命中" << b_n[b_nid].name << L"造成" << DMG << L"點傷害(" << ATK << ">" << AC << ")" << endl;
                EndBatchDraw();
                IMAGE tri;
                loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                transparentimage(NULL, (e[id].x) * 48 - b_m[b_mid].ox, ((e[id].y - 1) * 48) - b_m[b_mid].oy, &tri);
                Sleep(500);
                if (b_n[b_nid].hp <= 0) {
                    te[b_n[b_nid].x][b_n[b_nid].y].mA -= 10000;
                    te[b_n[b_nid].x][b_n[b_nid].y].mB -= 10000;
                    int x = b_n[b_nid].x, y = b_n[b_nid].y;
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
                    te[b_n[b_nid].x][b_n[b_nid].y].npc = 0;
                    b_n[b_nid].x = -1; b_n[b_nid].y = -1;
                    b_n[b_nid].speed = -1;
                }
            }
            else {
                BeginBatchDraw();
                maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                e_put(e, b_m, te, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"用" << ar[e[id].baid].name << L"攻擊" << b_n[b_nid].name << L"未能命中(" << ATK << "<=" << AC << ")" << endl;
                nn = L"miss";
                settextcolor(RGB(255, 0, 0));
                settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                outtextxy(b_n[b_nid].x * 48 - b_m[b_mid].ox, b_n[b_nid].y * 48 - b_m[b_mid].oy-40, nn.c_str());
                settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                settextcolor(RGB(0, 0, 0));
                EndBatchDraw();
                IMAGE tri;
                loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                transparentimage(NULL, (e[id].x) * 48 - b_m[b_mid].ox, ((e[id].y - 1) * 48) - b_m[b_mid].oy, &tri);
                Sleep(500);
            }
        }
    }
    }
    }
void e_target(wofstream*wofs,enemy *e,player *p,b_map *b_m,b_npc *b_n, terrain(*te)[50],int &P_id,int id,int b_mid,int psize,int nsize,int &b_nid,int type) {
    /*type0:優先尋找友方NPC單位;type1:只找友方NPC;type2:只找我方角色*/
        int min = 1000;
        int box;
        e[id].target = -1;
        if (e[id].e_range == 0) {
        if (type == 0) {
        for (i = 0; i < psize; i++) {
            box = abs(e[id].x - p[i].x) + abs(e[id].y - p[i].y);
            if (box < min) {
                min = box;
                P_id = i;
                e[id].target = 0;
                e[id].target_id = P_id;
            }
        }
        for (i = 0; i < nsize; i++) {
            box = abs(e[id].x - b_n[i].x) + abs(e[id].y - b_n[i].y);
            if (box < min) {
                min = box;
                b_nid = i;
                e[id].target = 1;
                e[id].target_id = b_nid;
            }
        }
        }
        else if (type == 1) {
            for (i = 0; i < nsize; i++) {
                box = abs(e[id].x - b_n[i].x) + abs(e[id].y - b_n[i].y);
                if (box < min) {
                    min = box;
                    b_nid = i;
                    e[id].target = 1;
                    e[id].target_id = b_nid;
                }
            }
        }
        else if (type == 2) {
            for (i = 0; i < psize; i++) {
                box = abs(e[id].x - p[i].x) + abs(e[id].y - p[i].y);
                if (box < min) {
                    min = box;
                    P_id = i;
                    e[id].target = 0;
                    e[id].target_id = P_id;
                }
            }
        }
        if (e[id].target == 0) {
            *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name<<L"將"<<p[P_id].name<<L"選為目標" << endl;
        }
        else if (e[id].target == 1) {
            *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"將" << b_n[b_nid].name << L"選為目標" << endl;
        }
        }
        else if (e[id].e_range != 0) {
            if (type == 0) {
                for (int I = 0; I < b_m[b_mid].psize; I++) {
                    if (te[p[I].x][p[I].y].dark!=1&& te[p[I].x][p[I].y].dark != 2) {
                        box = abs(e[id].x - p[I].x) + abs(e[id].y - p[I].y);
                        if (box < min) {
                            min = box;
                            P_id = I;
                            e[id].target = 0;
                            e[id].target_id = P_id;
                            e[id].target_x = p[P_id].x;
                            e[id].target_y = p[P_id].y;
                            e[id].lastx = p[P_id].x;
                            e[id].lasty = p[P_id].y;
                        }
                    }
                }
                for (i = 0; i < nsize; i++) {
                    if (te[b_n[i].x][b_n[i].y].dark!=1&& te[b_n[i].x][b_n[i].y].dark != 2) {
                    box = abs(e[id].x - b_n[b_nid].x) + abs(e[id].y - b_n[b_nid].y);
                    if (box < min) {
                        min = box;
                        b_nid = i;
                        e[id].target = 1;
                        e[id].target_id = b_nid;
                    }
                    }
                }
            }
            else if (type == 1) {
                for (i = 0; i < nsize; i++) {
                    if (te[b_n[i].x][b_n[i].y].dark != 1 && te[b_n[i].x][b_n[i].y].dark != 2) {
                        box = abs(e[id].x - b_n[b_nid].x) + abs(e[id].y - b_n[b_nid].y);
                        if (box < min) {
                            min = box;
                            b_nid = i;
                            e[id].target = 1;
                            e[id].target_id = b_nid;
                            e[id].target_x = b_n[b_nid].x;
                            e[id].target_y = b_n[b_nid].y;
                            e[id].lastx = b_n[b_nid].x;
                            e[id].lasty = b_n[b_nid].y;
                        }
                    }
                }
            }
            else if (type == 2) {
                for (int I = 0; I < b_m[b_mid].psize; I++) {
                    if (te[p[I].x][p[I].y].dark != 1 && te[p[I].x][p[I].y].dark != 2) {
                        box = abs(e[id].x - p[I].x) + abs(e[id].y - p[I].y);
                        if (box < min) {
                            min = box;
                            P_id = I;
                            e[id].target = 0;
                            e[id].target_id = P_id;
                            e[id].target_x = p[P_id].x;
                            e[id].target_y = p[P_id].y;
                            e[id].lastx = p[P_id].x;
                            e[id].lasty = p[P_id].y;
                        }
                    }
                }
            }
            for (int I = 0; I < e[id].e_range; I++) {
                for (int J = 0; J < e[id].e_range; J++) {
                    if (abs(I) + abs(J) <= e[id].e_range) {
                        if (type == 0) {
                        for (int K = 0; K < b_m[b_mid].psize;K++) {
                            if (p[K].x == e[id].x + I && p[K].y == e[id].y + J) {
                                box = abs(e[id].x - p[K].x) + abs(e[id].y - p[K].y);
                                if (box < min) {
                                    min = box;
                                    P_id = K;
                                    e[id].target = 0;
                                    e[id].target_id = K;
                                    e[id].target_x = p[K].x;
                                    e[id].target_y = p[K].y;
                                    e[id].lastx = p[K].x;
                                    e[id].lasty = p[K].y;
                                }
                                
                            }
                        }
                        for (int W = 0; W < b_m[b_mid].nsize; W++) {
                            if (b_n[W].x == e[id].x + I && b_n[W].y == e[id].y + J) {
                                box = abs(e[id].x - b_n[W].x) + abs(e[id].y - b_n[W].y);
                                if (box < min) {
                                    min = box;
                                    b_nid = W;
                                    e[id].target = 1;
                                    e[id].target_id = W;
                                    e[id].target_x = b_n[W].x;
                                    e[id].target_y = b_n[W].y;
                                    e[id].lastx = b_n[W].x;
                                    e[id].lasty = b_n[W].y;
                                }
                            }
                        }
                        }
                        else if (type == 1) {
                            for (int W = 0; W < b_m[b_mid].nsize; W++) {
                                if (b_n[W].x == e[id].x + I && b_n[W].y == e[id].y + J) {
                                    box = abs(e[id].x - b_n[W].x) + abs(e[id].y - b_n[W].y);
                                    if (box < min) {
                                        min = box;
                                        b_nid = W;
                                        e[id].target = 1;
                                        e[id].target_id = W;
                                        e[id].target_x = b_n[W].x;
                                        e[id].target_y = b_n[W].y;
                                        e[id].lastx = b_n[W].x;
                                        e[id].lasty = b_n[W].y;
                                    }
                                }
                            }
                        }
                        else if (type == 2) {
                            for (int K = 0; K < b_m[b_mid].psize; K++) {
                                if (p[K].x == e[id].x + I && p[K].y == e[id].y + J) {
                                    box = abs(e[id].x - p[K].x) + abs(e[id].y - p[K].y);
                                    if (box < min) {
                                        min = box;
                                        P_id = K;
                                        e[id].target = 0;
                                        e[id].target_id = K;
                                        e[id].target_x = p[K].x;
                                        e[id].target_y = p[K].y;
                                        e[id].lastx = p[K].x;
                                        e[id].lasty = p[K].y;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (e[id].target == 0) {
                *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"將" << p[P_id].name << L"選為目標" << endl;
            }
            else if (e[id].target == 1) {
                *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"將" << b_n[b_nid].name << L"選為目標" << endl;
            }
        }
        if (e[id].target == -1) {
            if (e[id].lastx == -1 && e[id].lasty == -1||(e[id].x==e[id].lastx&&e[id].y==e[id].lasty)) {
            e[id].buff_check[32] = 1;
            e[id].buff_check[33] = 0;
            }
            else {
                e[id].buff_check[32] = 0;
                e[id].buff_check[33] = 1;
            }
        }
        else {
            e[id].buff_check[32] = 0; e[id].buff_check[33] = 0;
        }
}
void e_walk(wofstream *wofs,enemy* e, player* p,b_npc *b_n, b_map* b_m,arms *ar, terrain(*te)[50], int id, int P_id, int esize, int psize, int b_mid, int bu_id, int b_nid, int target,int type) {
    int box[1000], w = 0, k, xbox, ybox;
    int range,sb=0;        
    /*type:0 近戰，type:1 遠程*/
    IMAGE get;
    if (ar[e[id].baid].type[0] == 'r') {
        type = 1;
    }
    if (e[id].baid == 10) {
        if (e[id].b_num_1 == 0) {
            type = 0;
        }
    }
    if (e[id].target == -1) {
            if (e[id].lastx != -1 && e[id].lasty != -1 && (e[id].x != e[id].lastx || e[id].y != e[id].lasty)) {
            Bbfs(b_m, te, e[id].x, e[id].y, e[id].lastx, e[id].lasty, box, b_mid, e[id].Move);
            }
            else {
                return;
            }
        }
    else if (type == 0) {
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
    }
    else if (type == 1) {
     if (ar[e[id].baid].range < e[id].s_range) {
            range = e[id].s_range;
       }
     else {
            range = ar[e[id].baid].range;
     }
     if (target == 0) {
         if (abs(e[id].x - p[P_id].x) + abs(e[id].y - p[P_id].y) == range) {
             return;
         }
         else if (abs(e[id].x - p[P_id].x) + abs(e[id].y - p[P_id].y) < range) {
             r_bfs(b_m, te, e[id].x, e[id].y, e[id].target_x, e[id].target_y, box, e[id].move, b_mid, range, 1);
         }
         else {
             r_bfs(b_m, te, e[id].x, e[id].y, e[id].target_x, e[id].target_y, box, e[id].move, b_mid, range, 0);
         }
     }
     else if (target == 1) {
    if (abs(e[id].x - b_n[b_nid].x) + abs(e[id].y - b_n[b_nid].y) == range) {
        return;
    }
    else if (abs(e[id].x - b_n[b_nid].x) + abs(e[id].y - b_n[b_nid].y) < range) {
        r_bfs(b_m, te, e[id].x, e[id].y, e[id].target_x, e[id].target_y, box, e[id].move, b_mid, range, 1);
    }
    else {
        r_bfs(b_m, te, e[id].x, e[id].y, e[id].target_x, e[id].target_y, box, e[id].move, b_mid, range, 0);
    }
     }
    }    
    xbox = e[id].x; ybox = e[id].y;
    wstring mm;
    mm = std::to_wstring(e[id].type);
    mm = L"./Game/picture/enemy" + mm + L".png";
    IMAGE enemy1;
    loadimage(&enemy1, mm.c_str(), 0, 0, false);
    b_camera(b_m, xbox, ybox, b_mid);
    for (int I = 0; I < b_m[b_mid].psize; I++) {
        if (p[I].buff_check[22] > 0) {
            int range = 0;
            int ar_id = -1;
            if (p[I].arms_id_1 != -1) {
                if (p[I].arms_b_1 > 0) {
                    range = ar[p[I].arms_id_1].range;
                    ar_id = p[I].arms_id_1;
                }
            }
            if (p[I].arms_id_2 != -1) {
                if (p[I].arms_b_2 > 0) {
                    if (range < ar[p[I].arms_id_2].range) {
                        range = ar[p[I].arms_id_2].range;
                        ar_id = p[I].arms_id_2;
                    }
                }
            }
            if (abs(p[I].x - e[id].x) + abs(p[I].y - e[id].y) <= range&&ar_id!=-1) {
                sb = 1;
                int AC = roll("", 2) + (e[id].dex - 10) / 2;
                if (e[id].buff_check[20] == 1) {
                    AC = 0;
                }
                if (e[id].buff_check[27] > 0) {
                    AC /= 2;
                }
                int ATK;
                if (ar[ar_id].type[0] == 'r') {
                 ATK = roll("", 2) + (p[I].dex - 10) / 2 + roll(ar[ar_id].hit, 1);
                }
                else if (ar[ar_id].type[0] == 'm') {
                    ATK = roll("", 2) + (p[I].str - 10) / 2 + roll(ar[ar_id].hit, 1);;
                }
                wstring vos;
                IMAGE ae;
                if (ar_id == 0 || ar_id == 5) {
                    mciSendString(L"close se", NULL, 0, NULL);
                    mciSendString(L"open ./Game/Sound/SE/拳銃2.mp3 alias se", NULL, 0, NULL);
                    vos = L"setaudio se volume to " + to_wstring(soundSize);
                    mciSendString(vos.c_str(), NULL, 0, NULL);
                    mciSendString(L"play se from 0", NULL, 0, NULL);
                    loadimage(&ae, L"./Game/picture/武器・弓1.png", 0, 0, false);
                    BeginBatchDraw();
                    for (j = 0; j < 5; j++) {
                        for (i = 0; i < 3; i++) {
                            maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                            e_put(e, b_m, te, esize, b_mid);
                            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                            p_put(p, b_m, psize, b_mid);
                            transparentimageA(NULL, e[id].x * 48 - b_m[b_mid].ox - 30, e[id].y * 48 - b_m[b_mid].oy - 160, &ae, i * 120, j * 300, 120, 300);
                            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                            FlushBatchDraw();
                            Sleep(5);
                        }
                    }
                    EndBatchDraw();
                }
                else if (ar_id == 4 || ar_id == 6) {
                    mciSendString(L"close se", NULL, 0, NULL);
                    mciSendString(L"open ./Game/Sound/SE/剣の素振り3.mp3 alias se", NULL, 0, NULL);
                    vos = L"setaudio se volume to " + to_wstring(soundSize);
                    mciSendString(vos.c_str(), NULL, 0, NULL);
                    mciSendString(L"play se from 0", NULL, 0, NULL);
                    loadimage(&ae, L"./Game/picture/技・一閃.png", 0, 0, false);
                    BeginBatchDraw();
                    for (j = 0; j < 5; j++) {
                        for (i = 0; i < 3; i++) {
                            maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                            e_put(e, b_m, te, esize, b_mid);
                            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                            p_put(p, b_m, psize, b_mid);
                            transparentimageA(NULL, e[id].x * 48 - b_m[b_mid].ox - 110, e[id].y * 48 - b_m[b_mid].oy - 47, &ae, i * 320, j * 120, 320, 120);
                            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                            FlushBatchDraw();
                            Sleep(5);
                        }
                    }
                    EndBatchDraw();
                }
                else if (ar_id == 8) {
                    BeginBatchDraw();
                    maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                    e_put(e, b_m, te, esize, b_mid);
                    b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                    p_put(p, b_m, psize, b_mid);
                    ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                    EndBatchDraw();
                    getimage(&get, 0, 0, 1296, 960);
                    loadimage(&ae, L"./Game/picture/杖1.png", 0, 0, false);
                    mciSendString(L"close se", NULL, 0, NULL);
                    mciSendString(L"open ./Game/Sound/SE/軽いパンチ2.mp3 alias se", NULL, 0, NULL);
                    vos = L"setaudio se volume to " + to_wstring(soundSize);
                    mciSendString(vos.c_str(), NULL, 0, NULL);
                    mciSendString(L"play se from 0", NULL, 0, NULL);
                    for (int ej = 0; ej < 4; ej++) {
                        for (int ei = 0; ei < 5; ei++) {
                            putimage(0, 0, &get);
                            transparentimageA(NULL, 48 * e[id].x - b_m[b_mid].ox - 10, e[id].y * 48 - b_m[b_mid].oy - 30, &ae, ei * 80, ej * 80, 80, 80);
                            FlushBatchDraw();
                            Sleep(40);
                        }
                    }
                }
                /*ae*/
                if (ATK > AC) {
                int dmg = roll(ar[ar_id].dmg, 1);
                if (ar[ar_id].type[0] == 'm') {
                    dmg += (p[I].str - 10) / 5;
                }
                if (dmg >= e[id].buff_check[7] + e[id].buff_check[10]) {
                    dmg -= e[id].buff_check[7] + e[id].buff_check[10];
                }
                else {
                    dmg = 0;
                }
                if (p[I].buff_check[21] > 0) {
                    dmg += (p[P_id].wis - 5) / 2;
                }
                eHP(e, b_m, id, b_mid, 0, dmg);
                *wofs << L"(T" << b_m[b_mid].time << L")" << p[I].name << L"用" << ar[ar_id].name << L"命中" << e[id].name << L"造成" << dmg << L"點傷害(" << ATK << ">" << AC << ")" << endl;
                EndBatchDraw();                                    
                if (ar[ar_id].type[0] == 'r') {
                    if (ar_id == p[I].arms_id_1 && p[I].arms_b_1 > 0) {
                        p[I].arms_b_1--;
                    }
                    else {
                        p[I].arms_b_2--;
                    }
                }
                Sleep(1000);
                if (e[id].hp <= 0) {
                    te[e[id].x][e[id].y].mA -= 10000;
                    te[e[id].x][e[id].y].mB -= 10000;
                    int x = e[id].x, y = e[id].y;
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
                    te[e[id].x][e[id].y].enemy = 0;
                    e[id].x = -1; e[id].y = -1;
                    e[id].speed = -1;
                    if (e[id].buff_check[20] > 0) {
                        p[0].buff_check[20] = 0;
                        e[id].buff_check[20] = 0;
                    }
                    return;
                }
                }
                else {
                *wofs << L"(T" << b_m[b_mid].time << L")" << p[I].name << L"用" << ar[ar_id].name << L"攻擊" << e[id].name << L"未能命中(" << ATK << "<=" << AC << ")" << endl;
                if (ar[ar_id].type[0] == 'r') {
                    if (ar_id == p[I].arms_id_1 && p[I].arms_b_1 > 0) {
                        p[I].arms_b_1--;
                    }
                    else {
                        p[I].arms_b_2--;
                    }
                }
                mm = L"miss" ;
                settextcolor(RGB(255, 0, 0));
                settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                outtextxy(e[id].x * 48 - b_m[b_mid].ox, e[id].y * 48 - b_m[b_mid].oy - 40, mm.c_str());
                settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                settextcolor(RGB(0, 0, 0));
                Sleep(1000);
                }  
                p[I].buff_check[22]--;
            }
        }
    }
    while (1) {
        if (box[w] == 2) {
            if (e[id].move >= te[xbox][ybox+1].mB) {
                e[id].move = e[id].move - te[xbox][ybox+1].mB;
                if (te[xbox][ybox + 1].fire == 1) {
                    if (e[id].buff_check[11] > 0) {

                    }
                    else {
                    e[id].buff_check[12]++;
                    }
                }
            }
            else {
                break;
            }
        }
        else if (box[w] == 4) {
            if (e[id].move >= te[xbox-1][ybox].mB) {
                e[id].move = e[id].move - te[xbox-1][ybox].mB;
                if (te[xbox-1][ybox ].fire == 1) {
                    if (e[id].buff_check[11] > 0) {

                    }
                    else {
                        e[id].buff_check[12]++;
                    }
                }
            }
            else {
                break;
            }
        }
        else if (box[w] == 6) {
            if (e[id].move >= te[xbox+1][ybox].mB) {
                e[id].move = e[id].move - te[xbox+1][ybox].mB;
                if (te[xbox+1][ybox].fire == 1) {
                    if (e[id].buff_check[11] > 0) {

                    }
                    else {
                        e[id].buff_check[12]++;
                    }
                }
            }
            else {
                break;
            }
        }
        else if (box[w] == 8) {
            if (e[id].move >= te[xbox][ybox-1].mB) {
                e[id].move = e[id].move - te[xbox][ybox-1].mB;
                if (te[xbox][ybox - 1].fire == 1) {
                    if (e[id].buff_check[11] > 0) {

                    }
                    else {
                        e[id].buff_check[12]++;
                    }
                }
            }
            else {
                break;
            }
        }
        if (box[w] == 2) {

            for (k = 0; k <= 48; k += 16) {

                BeginBatchDraw();
                maps(p, P_id, e, b_m, ar, te,b_mid);
                e_bput(e, b_m,te, id, esize, b_mid);
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
                maps(p, P_id, e, b_m, ar, te, b_mid);
                e_bput(e, b_m, te, id, esize, b_mid);
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
                maps(p, P_id, e, b_m, ar, te,b_mid);
                e_bput(e, b_m, te, id, esize, b_mid);
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
                maps(p, P_id, e, b_m, ar, te,b_mid);
                e_bput(e, b_m, te, id, esize, b_mid);
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
        for (int I = 0; I < b_m[b_mid].psize; I++) {
            if (p[I].buff_check[22] > 0&&sb==0) {
                int range = 0;
                int ar_id = -1;
                if (p[I].arms_id_1 != -1) {
                    if (p[I].arms_b_1 > 0) {
                        range = ar[p[I].arms_id_1].range;
                        ar_id = p[I].arms_id_1;
                    }
                }
                if (p[I].arms_id_2 != -1) {
                    if (p[I].arms_b_2 > 0) {
                        if (range < ar[p[I].arms_id_2].range) {
                            range = ar[p[I].arms_id_2].range;
                            ar_id = p[I].arms_id_2;
                        }
                    }
                }
                if (abs(p[I].x - xbox) + abs(p[I].y - ybox) <= range && ar_id != -1) {
                    sb = 1;
                    int AC = roll("", 2) + (e[id].dex - 10) / 2;
                    if (e[id].buff_check[20] == 1) {
                        AC = 0;
                    }
                    if (e[id].buff_check[27] > 0) {
                        AC /= 2;
                    }
                    int ATK;
                    if (ar[ar_id].type[0] == 'r') {
                        ATK = roll("", 2) + (p[I].dex - 10) / 2 + roll(ar[ar_id].hit, 1);
                    }
                    else if (ar[ar_id].type[0] == 'm') {
                        ATK = roll("", 2) + (p[I].str - 10) / 2 + roll(ar[ar_id].hit, 1);;
                    }
                    wstring vos;
                    IMAGE ae;
                    getimage(&get, 0, 0, 1296, 960);
                    if (ar_id == 0 || ar_id == 5) {
                        mciSendString(L"close se", NULL, 0, NULL);
                        mciSendString(L"open ./Game/Sound/SE/拳銃2.mp3 alias se", NULL, 0, NULL);
                        vos = L"setaudio se volume to " + to_wstring(soundSize);
                        mciSendString(vos.c_str(), NULL, 0, NULL);
                        mciSendString(L"play se from 0", NULL, 0, NULL);
                        loadimage(&ae, L"./Game/picture/武器・弓1.png", 0, 0, false);
                        BeginBatchDraw();
                        for (j = 0; j < 5; j++) {
                            for (i = 0; i < 3; i++) {
                                putimage(0, 0, &get);
                                transparentimageA(NULL,xbox * 48 - b_m[b_mid].ox - 30, ybox * 48 - b_m[b_mid].oy - 160, &ae, i * 120, j * 300, 120, 300);
                                FlushBatchDraw();
                                Sleep(5);
                            }
                        }
                        EndBatchDraw();
                    }
                    else if (ar_id == 4 || ar_id == 6) {
                        mciSendString(L"close se", NULL, 0, NULL);
                        mciSendString(L"open ./Game/Sound/SE/剣の素振り3.mp3 alias se", NULL, 0, NULL);
                        vos = L"setaudio se volume to " + to_wstring(soundSize);
                        mciSendString(vos.c_str(), NULL, 0, NULL);
                        mciSendString(L"play se from 0", NULL, 0, NULL);
                        loadimage(&ae, L"./Game/picture/技・一閃.png", 0, 0, false);
                        BeginBatchDraw();
                        for (j = 0; j < 5; j++) {
                            for (i = 0; i < 3; i++) {
                                putimage(0, 0, &get);
                                transparentimageA(NULL,xbox * 48 - b_m[b_mid].ox - 110, ybox * 48 - b_m[b_mid].oy - 47, &ae, i * 320, j * 120, 320, 120);
                                FlushBatchDraw();
                                Sleep(5);
                            }
                        }
                        EndBatchDraw();
                    }
                    else if (ar_id == 8) {
                        BeginBatchDraw();
                        maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                        e_put(e, b_m, te, esize, b_mid);
                        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                        p_put(p, b_m, psize, b_mid);
                        ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                        EndBatchDraw();
                        getimage(&get, 0, 0, 1296, 960);
                        loadimage(&ae, L"./Game/picture/杖1.png", 0, 0, false);
                        mciSendString(L"close se", NULL, 0, NULL);
                        mciSendString(L"open ./Game/Sound/SE/軽いパンチ2.mp3 alias se", NULL, 0, NULL);
                        vos = L"setaudio se volume to " + to_wstring(soundSize);
                        mciSendString(vos.c_str(), NULL, 0, NULL);
                        mciSendString(L"play se from 0", NULL, 0, NULL);
                        for (int ej = 0; ej < 4; ej++) {
                            for (int ei = 0; ei < 5; ei++) {
                                putimage(0, 0, &get);
                                transparentimageA(NULL, 48 * e[id].x - b_m[b_mid].ox - 10, e[id].y * 48 - b_m[b_mid].oy - 30, &ae, ei * 80, ej * 80, 80, 80);
                                FlushBatchDraw();
                                Sleep(40);
                            }
                        }
                    }
                    /*ae*/
                    if (ATK > AC) {
                        int dmg = roll(ar[ar_id].dmg, 1);
                        if (ar[ar_id].type[0] == 'm') {
                            dmg += (p[I].str - 10) / 5;
                        }
                        if (dmg >= e[id].buff_check[7] + e[id].buff_check[10]) {
                            dmg -= e[id].buff_check[7] + e[id].buff_check[10];
                        }
                        else {
                            dmg = 0;
                        }
                        if (p[I].buff_check[21] > 0) {
                            dmg += (p[P_id].wis - 5) / 2;
                        }
                        eHP(e, b_m, id, b_mid,0,dmg );
                        *wofs << L"(T" << b_m[b_mid].time << L")" << p[I].name << L"用" << ar[ar_id].name << L"命中" << e[id].name << L"造成" << dmg << L"點傷害(" << ATK << ">" << AC << ")" << endl;
                        EndBatchDraw();
                        if (ar[ar_id].type[0] == 'r') {
                            if (ar_id == p[I].arms_id_1 && p[I].arms_b_1 > 0) {
                                p[I].arms_b_1--;
                            }
                            else {
                                p[I].arms_b_2--;
                            }
                        }
                        Sleep(1000);
                        if (e[id].hp <= 0) {
                            te[e[id].x][e[id].y].mA -= 10000;
                            te[e[id].x][e[id].y].mB -= 10000;
                            int x = e[id].x, y = e[id].y;
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
                            te[e[id].x][e[id].y].enemy = 0;
                            e[id].x = -1; e[id].y = -1;
                            e[id].speed = -1;
                            if (e[id].buff_check[20] > 0) {
                                p[0].buff_check[20] = 0;
                                e[id].buff_check[20] = 0;
                            }
                            return;
                        }
                    }
                    else {
                        *wofs << L"(T" << b_m[b_mid].time << L")" << p[I].name << L"用" << ar[ar_id].name << L"攻擊" << e[id].name << L"未能命中(" << ATK << "<=" << AC << ")" << endl;
                        if (ar[ar_id].type[0] == 'r') {
                            if (ar_id == p[I].arms_id_1 && p[I].arms_b_1 > 0) {
                                p[I].arms_b_1--;
                            }
                            else {
                                p[I].arms_b_2--;
                            }
                        }
                        mm = L"miss";
                        settextcolor(RGB(255, 0, 0));
                        settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                        outtextxy(xbox * 48 - b_m[b_mid].ox, ybox * 48 - b_m[b_mid].oy - 40, mm.c_str());
                        settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                        settextcolor(RGB(0, 0, 0));
                        Sleep(1000);
                    }
                    p[I].buff_check[22]--;
                }
            }
        }
        if (te[xbox][ybox].trap == 2) {
            e[id].move = 0;
            e[id].buff_check[27]++;
            te[xbox][ybox].trap = 0;
            int dmg = 5;
            eHP(e, b_m, id, b_mid, 0, dmg);
            *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << "觸發狩獵陷阱" << endl;
            FlushBatchDraw();
            Sleep(500);
            if (e[id].hp <= 0) {
                te[e[id].x][e[id].y].mA -= 10000;
                te[e[id].x][e[id].y].mB -= 10000;
                int x = e[id].x, y = e[id].y;
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
                te[e[id].x][e[id].y].enemy = 0;
                e[id].x = -1; e[id].y = -1;
                e[id].speed = -1;
                if (e[id].buff_check[20] > 0) {
                    p[0].buff_check[20] = 0;
                    e[id].buff_check[20] = 0;
                }
                return;
            }
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
        RECT t;
        wifs.open(b_m[b_mid].log_name, ios::in);
        BeginBatchDraw();
        putimage(960, 0, &text);
        while (getline(wifs, word)) {
            if (word[0] != '[') {
                if (J < b_m[b_mid].line - line+3 && J >= b_m[b_mid].line-line) {
                    t = {960,I*48,1296,I*48+48};
                    drawtext(word.c_str(), &t, DT_WORDBREAK);
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
    int flag1 = -1;
    int flag2 = -1;
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
    IMAGE g,ug,get,mb;
    TCHAR t[5];   
    loadimage(&ug, L"./Game/picture/bblock.png", 0, 0, false);
    getimage(&get, 0, 0, 1296, 960);
    while (sss != 0)
    {
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
        else if (GetAsyncKeyState(0x51) & 0x8000) {
            chose = "a";
            sss = 0;
        }
        else if (GetAsyncKeyState(0x57) & 0x8000) {
            chose = "w";
            sss = 0;
        }
        else if (GetAsyncKeyState(0x41) & 0x8000) {
            chose = "i";
            sss = 0;
        }
        else if (GetAsyncKeyState(0x53) & 0x8000) {
            chose = "k";
            sss = 0;
        }
        else if (GetAsyncKeyState(0x5A) & 0x8000) {
            chose = "s";
            sss = 0;
        }
        else if (GetAsyncKeyState(0x58) & 0x8000) {
            chose = "l";
            sss = 0;
        }
        else if (GetAsyncKeyState(0x45) & 0x8000) {
            chose = "e";
            sss = 0;
        }
        else {
            flushmessage(-1);
                m = getmessage(-1);
                if (m.message == WM_MOUSEMOVE) {                    
                    for (i = 0; i < esize; i++) {
                        if (m.x >= e[i].x * 48 - b_m[b_mid].ox && m.x <= e[i].x * 48 + 48 - b_m[b_mid].ox && m.y >= e[i].y * 48 - b_m[b_mid].oy && m.y <= e[i].y * 48 + 48 - b_m[b_mid].oy && Flag != i && m.x < 960 && m.y < 720) {
                            BeginBatchDraw();
                            maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
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
                                if (te[X + 1][Y].mB < 10000 && te[X + 1][Y].npc == 0 && te[X + 1][Y].player == 0 && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x && (abs(X + 1 - x) + abs(Y - y) <= MR) && road[X][Y] < e[i].Move && road[X][Y] >= 0) {
                                    if (cost[X + 1][Y] > cost[X][Y] + te[X][Y].mB) {
                                        q.push({ X + 1,Y });
                                        if (road[X][Y] + te[X + 1][Y].mB <= e[i].Move) {
                                            road[X + 1][Y] = road[X][Y] + te[X + 1][Y].mB;
                                            cost[X + 1][Y] = cost[X][Y] + te[X][Y].mB;
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
                                if (te[X - 1][Y].mB < 10000 && te[X - 1][Y].npc == 0 && te[X - 1][Y].player == 0 && te[X - 1][Y].type != 1 && X - 1 >= 0 && (abs(X - 1 - x) + abs(Y - y) <= MR) && road[X][Y] < e[i].Move && road[X][Y] >= 0) {
                                    if (cost[X - 1][Y] > cost[X][Y] + te[X][Y].mB) {
                                        q.push({ X - 1,Y });
                                        if (road[X][Y] + te[X - 1][Y].mB <= e[i].Move) {
                                            road[X - 1][Y] = road[X][Y] + te[X - 1][Y].mB;
                                            cost[X - 1][Y] = cost[X][Y] + te[X][Y].mB;
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
                                if (te[X][Y + 1].mB < 10000 && te[X][Y + 1].npc == 0 && te[X][Y + 1].player == 0 && te[X][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y && (abs(X - x) + abs(Y + 1 - y) <= MR) && road[X][Y] < e[i].Move && road[X][Y] >= 0) {
                                    if (cost[X][Y + 1] > cost[X][Y] + te[X][Y].mB) {
                                        q.push({ X ,Y + 1 });
                                        if (road[X][Y] + te[X][Y + 1].mB <= e[i].Move) {
                                            road[X][Y + 1] = road[X][Y] + te[X][Y + 1].mB;
                                            cost[X][Y + 1] = cost[X][Y] + te[X][Y].mB;
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
                                if (te[X][Y - 1].mB < 10000 && te[X][Y - 1].npc == 0 && te[X][Y - 1].player == 0 && te[X][Y - 1].type != 1 && Y - 1 >= 0 && (abs(X - x) + abs(Y - 1 - y) <= MR) && road[X][Y] < e[i].Move && road[X][Y] >= 0) {
                                    if (cost[X][Y - 1] > cost[X][Y] + te[X][Y].mB) {
                                        q.push({ X ,Y - 1 });
                                        if (road[X][Y] + te[X][Y - 1].mB <= e[i].Move) {
                                            road[X][Y - 1] = road[X][Y] + te[X][Y - 1].mB;
                                            cost[X][Y - 1] = cost[X][Y] + te[X][Y].mB;
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
                            road[6][10] = road[6][10];
                            cost[6][10] = cost[6][10];
                            move[6][10] = move[6][10];
                            e_put(e, b_m, te, b_m[b_mid].esize, b_mid);
                            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                            p_put(p, b_m, psize, b_mid);
                            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                            outtextxy(960, 0, e[i].name.c_str());
                            wstring mm = L"HP[" + to_wstring(e[i].hp) + L"/" + to_wstring(e[i].mhp) + L"]";
                            outtextxy(960, 30, mm.c_str());
                            loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);                            
                            transparentimage(NULL, (e[i].x) * 48 - b_m[b_mid].ox, ((e[i].y - 1) * 48) - b_m[b_mid].oy, &tri);
                            ue = 0;
                            for (int I = 0; I < bu_id; I++) {
                                if (e[i].buff_check[I] >= 1) {
                                    mm = L"./Game/picture/buff" + to_wstring(I) + L".png";
                                    loadimage(&g, mm.c_str(), 0, 0, false);
                                    putimage(960 + 30 * ue, 100, &g);
                                    _stprintf(t, _T("%d"), e[i].buff_check[I]);
                                    outtextxy(960 + 30 * ue, 130, t);
                                    ke[ue] = I;
                                    ue++;
                                }
                            }
                            FlushBatchDraw();
                            getimage(&get, 0, 0, 1296, 960);
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
                            maps(p, P_id, e, b_m, ar, te,b_mid);/*地圖繪製*/
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
                            e_put(e, b_m, te, b_m[b_mid].esize, b_mid);
                            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                            p_put(p, b_m, psize, b_mid);
                            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                            outtextxy(960, 0, b_n[i].name.c_str());
                            wstring mm = L"HP[" + to_wstring(b_n[i].hp) + L"/" + to_wstring(b_n[i].mhp) + L"]";
                            outtextxy(960, 30, mm.c_str());
                            loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                            transparentimage(NULL, (b_n[i].x) * 48 - b_m[b_mid].ox, ((b_n[i].y - 1) * 48) - b_m[b_mid].oy, &tri);  
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
                            FlushBatchDraw();
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
                            maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                            e_put(e, b_m, te, b_m[b_mid].esize, b_mid);
                            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                            p_put(p, b_m, psize, b_mid);
                            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                            outtextxy(960, 0, p[i].name.c_str());

                            wstring mm = L"HP[" + to_wstring(p[i].hp) + L"/" + to_wstring(p[i].mhp) + L"]";
                            outtextxy(960, 30, mm.c_str());

                            loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
                            transparentimage(NULL, (p[i].x) * 48 - b_m[b_mid].ox, ((p[i].y - 1) * 48) - b_m[b_mid].oy, &tri);
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
                            FlushBatchDraw();
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
                            wstring mm;
                            mm = bu[uk[i]].story;
                            if (p[P_id].buff_Size > 0) {
                                for (int bs = 0; bs < p[P_id].buff_Size; bs++) {
                                    if (p[P_id].buff_id[bs] == uk[i]) {
                                        mm += L"\n剩餘" + to_wstring(p[P_id].buff_time[bs]) + L"秒";
                                    }
                                }
                            }
                            drawtext(mm.c_str(), &r, DT_WORDBREAK);
                            FlushBatchDraw();
                        }
                    }
                    if (Flag != -1) {
                        for (i = 0; i < ue; i++) {
                        if (m.x > 960 + 30 * i && m.x < 960 + 30 * i + 20 && m.y>100 && m.y < 120) {
                            BeginBatchDraw();
                            putimage(0, 0, &get);
                            RECT r = { 1005 + 30 * i + 5, 100 + 5, 1005 + 30 * i + 190 + 5, 180 + 30 };
                            putimage(1005 + 30 * i, 100, &ug);
                            wstring mm;
                            mm = bu[ke[i]].name+L":"+ bu[ke[i]].story;
                            if (e[Flag].buff_Size > 0) {
                                for (int bs = 0; bs < e[Flag].buff_Size; bs++) {
                                    if (e[Flag].buff_id[bs] == ke[i]) {
                                        mm += L"\n剩餘" + to_wstring(e[Flag].buff_time[bs]) + L"秒";
                                        break;
                                    }
                                }
                            }
                            drawtext(mm.c_str(), &r, DT_WORDBREAK);
                            FlushBatchDraw();
                        }
                    }
                    }
                    else if (flag1 != -1) {
                        for (i = 0; i < un; i++) {
                            if (m.x>960+30*i&&m.x<960+30*i+20&&m.y>100&&m.y<120) {
                                BeginBatchDraw();
                                putimage(0, 0, &get);
                                RECT r = { 1005 + 30 * i+5, 100+5, 1005 + 30 * i + 190+5, 180 +30};
                                putimage(1005 + 30 * i, 100, &ug);
                                wstring mm;   
                                mm= bu[kn[i]].name + L":"+ bu[kn[i]].story ;
                                if (b_n[flag1].buff_Size > 0) {
                                    for (int bs=0; bs < b_n[flag1].buff_Size; bs++) {
                                        if (b_n[flag1].buff_id[bs] == kn[i]) {
                                            mm += L"\n剩餘" + to_wstring(b_n[flag1].buff_time[bs]) + L"秒";
                                        }
                                    }
                                }                                 
                                drawtext(mm.c_str(), &r, DT_WORDBREAK);
                                FlushBatchDraw();
                            }
                        }
                    }
                    else if (flag2 != -1) {
                        for (i = 0; i < up; i++) {
                            if (m.x > 960 + 30 * i && m.x < 960 + 30 * i + 20 && m.y>100 && m.y < 120) {
                                BeginBatchDraw();
                                putimage(0, 0, &get);
                                RECT r = { 1005 + 30 * i+5, 100+5, 1005 + 30 * i + 190+5, 180 +30};
                                putimage(1005 + 30 * i, 100, &ug);
                                wstring mm;
                                mm = bu[kp[i]].name + L":" + bu[kp[i]].story;
                                if (p[flag2].buff_Size > 0) {
                                    for (int bs = 0; bs < p[flag2].buff_Size; bs++) {
                                        if (p[flag2].buff_id[bs] == kp[i]) {
                                            mm += L"\n剩餘" + to_wstring(p[flag2].buff_time[bs]) + L"秒";
                                        }
                                    }
                                }
                                drawtext(mm.c_str(), &r, DT_WORDBREAK);
                                FlushBatchDraw();
                            }
                        }
                    }
                    b_m[b_mid].mx = m.x / 48+b_m[b_mid].ox/48;
                    b_m[b_mid].my = m.y / 48+b_m[b_mid].oy / 48;
                    if (m.x<960&&m.y<720) {
                        putimage(0, 0, &get);
                        loadimage(&mb, L"./Game/picture/mouse_block.png", 0, 0, false);
                        transparentimage(NULL, 48 * b_m[b_mid].mx-b_m[b_mid].ox, 48 * b_m[b_mid].my - b_m[b_mid].oy, &mb);
                        int ti = 0;
                        wstring tew=L"("+to_wstring(b_m[b_mid].mx)+L","+to_wstring(b_m[b_mid].my)+L")";
                        outtextxy(970, 570 + ti * 40, tew.c_str());
                        ti++;
                        if (te[b_m[b_mid].mx][b_m[b_mid].my].fire == 1) {
                           tew = L"燃燒("+ to_wstring(te[b_m[b_mid].mx][b_m[b_mid].my].fire_time)+L"秒)";
                            outtextxy(970,570+ti*40,tew.c_str());
                            ti++;
                        }
                        if (te[b_m[b_mid].mx][b_m[b_mid].my].type == 1) {
                           tew = L"不可通行";
                            outtextxy(970, 570 + ti * 40, tew.c_str());
                            ti++;
                        }
                        if (te[b_m[b_mid].mx][b_m[b_mid].my].type == 0) {
                            tew = L"可通行";
                            outtextxy(970, 570 + ti * 40, tew.c_str());
                            ti++;
                        }
                        if (te[b_m[b_mid].mx][b_m[b_mid].my].trap == 2) {
                            tew = L"陷阱";
                            outtextxy(970, 570 + ti * 40, tew.c_str());
                            ti++;
                        }
                        if (te[b_m[b_mid].mx][b_m[b_mid].my].dark == 1|| te[b_m[b_mid].mx][b_m[b_mid].my].dark == 2) {
                            tew = L"黑暗";
                            outtextxy(970, 570 + ti * 40, tew.c_str());
                            ti++;
                        }
                        FlushBatchDraw();
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
int battle_check(player *p,enemy *e,b_map *b_m,b_flag *b_f,int b_mid) {
    if (b_mid == 4) {
        if (e[2].hp<15) {
            return 1;
        }
    }
    else {
    for (i = 0; i < b_m[b_mid].esize; i++) {
        if (e[i].hp <= 0) {
        }
        else {
            return 0;
        }
    }
    return 1;
    }
    return 0;
}
void menu_json_save(player* p, arms* ar, item* it, stone* st, flag* f,Exit* EX,m_flag *m_f,t_equip *t_E,shop *sp,BOX *Box,task *tk,int P_id, int i_id, int ar_id, int st_id, int f_id, int psize, time_t first, time_t two, time_t three,string a,int m_id,int ex_id,int sk_id,int m_fid,int t_Eid,int sp_id,int b_id,int tk_id) {
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
        player["arms_b_1"].append(p[i].arms_b_1);
        player["arms_b_2"].append(p[i].arms_b_2);
        player["b_id_1"].append(p[i].b_id_1);
        player["b_id_2"].append(p[i].b_id_2);
        player["sp"].append(p[i].sp);
        player["EDV"].append(p[i].EDV);
        player["DEF"].append(p[i].DEF);
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
    root["flag"] = flag;
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
    Json::Value shop;
    for (i = 0; i < 2; i++) {
        string sj = "ar_number_" + to_string(i);
        for (j = 0; j < ar_id; j++) {
            shop[sj].append(sp[i].ar_number[j]);
        }
        sj = "i_number_" + to_string(i);
        for (j = 0; j < i_id; j++) {
            shop[sj].append(sp[i].i_number[j]);
        }
        sj = "t_E_number_" + to_string(i);
        for (j = 0; j < t_Eid; j++) {
            shop[sj].append(sp[i].t_E_number[j]);
        }
    }
    root["shop"] = shop;
    for (i = 0; i < b_id; i++) {
        root["box_state"].append(Box[i].state);
    }
    for (i = 0; i < tk_id; i++) {
        root["task_state"].append( tk[i].state);
    }
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
void menu_save(player* p, arms* ar, item* it, stone* st, flag* f,Exit* EX,m_flag *m_f,t_equip *t_E,shop *sp,BOX *Box,task *tk, Map *m,int P_id, int i_id, int ar_id, int st_id, int f_id, int psize, time_t first, time_t two, time_t three,int m_id,int ex_id,int sk_id,int m_fid,int t_Eid,int sp_id,int b_id,int tk_id) {
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    while (1) {
        flushmessage(EM_MOUSE);
        int u = 0;
        string a, b = ".txt", read = "";
        wstring vos,mm;
        int ssss = 1; j = 0;
        while (ssss != 0) {
            int sss = 1;
            IMAGE saveblock,sP;
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
                    mm = L"./Game/save/" + to_wstring(i + 1 + j * 14) + L".png";
                    loadimage(&sP, mm.c_str(), 108, 80, false);
                    putimage(458, 38+128*i, &sP);
                    outtextxy(265, 55 + 128 * i, m[root["m_id"].asInt()].name.c_str());
                    mm = to_wstring(hr)+L":"+to_wstring(min)+L":"+to_wstring(sec);
                    outtextxy(110, 60 + 128 * i, mm.c_str());
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
                    mm = L"./Game/save/" + to_wstring(i + 8 + j * 14) + L".png";
                    loadimage(&sP, mm.c_str(), 108, 80, false);
                    putimage(458+600, 38 + 128 * i, &sP);
                    outtextxy(265+600, 55 + 128 * i, m[root["m_id"].asInt()].name.c_str());
                    mm = to_wstring(hr) + L":" + to_wstring(min) + L":" + to_wstring(sec);
                    outtextxy(110+600, 60 + 128 * i, mm.c_str());
                }
            }
            FlushBatchDraw();
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
                        sss = 0; ssss = 0;  u = 1; EndBatchDraw(); return;
                    }
                }
            }
        }
        if (u == 0) {
            menu_json_save(p, ar, it, st, f,EX,m_f,t_E,sp,Box,tk,P_id, i_id, ar_id, st_id, f_id, psize, first, two, three, a,m_id,ex_id,sk_id,m_fid,t_Eid,sp_id,b_id,tk_id);
            wstring mm=L"./Game/save/";
            mm += string2wstring(a);
            mm += L".png";      
            saveimage(mm.c_str(),&save_image);
        }
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
void event(flag *f,b_flag *b_f,player *p,npc *n,m_flag *m_f,Map *m,BOX *Box,Exit *EX,enemy *e,e_npc *e_n,task *tk,shop *sp,item *it,stone *st,arms *ar,int &m_id,int &b_mid,int b_id) {
    i = 0;
    IMAGE p1,p2,p3;
    if (f[0].check == 0) {
        string filename;
        filename = "./Game/story/event" + to_string(0) + string(".json");
        const char* path = filename.c_str();
        readeventjson(p, n,f,m,Box,tk,m_f,it,st,path,m_id,b_id);
        f[0].check = 1;
    }
    else if (f[1].check ==0) {
        string filename;
        filename = "./Game/story/event" + to_string(1) + string(".json");
        const char* path = filename.c_str();
        readeventjson(p, n, f, m, Box, tk, m_f, it, st, path, m_id, b_id);
        f[1].check = 1;
    }
    else if (f[2].check == 0) {
        if (m_id == 2) {
            string filename;
            filename = "./Game/story/event" + to_string(2) + string(".json");
            const char* path = filename.c_str();
            readeventjson(p, n, f, m, Box, tk, m_f, it, st, path, m_id, b_id);
            f[2].check = 1;
        }
    }
     if (f[3].check == 0) {
        if (m_id == 2) {
            if (p[0].x == 5 && p[0].y == 14) {
                string filename;
                filename = "./Game/story/event" + to_string(3) + string(".json");
                const char* path = filename.c_str();
                readeventjson(p, n, f, m, Box, tk, m_f, it, st, path, m_id, b_id);
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
                readeventjson(p, n, f, m, Box, tk, m_f, it, st, path, m_id, b_id);
                f[4].check = 1;
            }
        }
    }
     if (f[5].check == 1) {
             b_mid = 1;
             f[5].check = 1;
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
         ar[8].number++;
         f[8].check = 0;
     }
     if (f[9].check == 0) {
         if (m_id == 4) {
             string filename;
             filename = "./Game/story/event" + to_string(6) + string(".json");
             const char* path = filename.c_str();
             readeventjson(p, n, f, m, Box, tk, m_f, it, st, path, m_id, b_id);
             f[9].check = 1;
         }
     }
     if (f[10].check == 0) {
         if (m_f[2].check == 1&&m_id==4) {
             ss[27][9] = 0;
             n[5].avatar = -1;
         }
     }
     if (f[11].check == 0) {
         if (p[0].x <= 27 && p[0].x >= 26 &&p[0].y==0&&m_f[2].check==0) {
             string filename;
             filename = "./Game/story/event" + to_string(7) + string(".json");
             const char* path = filename.c_str();
             readeventjson(p, n, f, m, Box, tk, m_f, it, st, path, m_id, b_id);
             f[11].check = 1;
         }
     }
     if (f[12].check ==0) {
         if (m_id == 5) {
             string filename;
             filename = "./Game/story/event" + to_string(5) + string(".json");
             const char* path = filename.c_str();
             readeventjson(p, n, f, m, Box, tk, m_f, it, st, path, m_id, b_id);
             filename = "./Game/story/event" + to_string(8) + string(".json");
             path = filename.c_str();
             readeventjson(p, n, f, m, Box, tk, m_f, it, st, path, m_id, b_id);
             f[12].check = 1;
         }
     }
     if (f[13].check == 0) {
         if (m_id == 5) {
             f[13].check = 1;
             b_mid = 2;
         }
     }
     if (f[14].check ==1) {
         n[6].avatar = 1;
         string filename;
         filename = "./Game/story/event" + to_string(9) + string(".json");
         readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);
         f[14].check = 0;
         if (m_f[7].check == 0) {
             m_f[7].check = 1;
             m_f[8].check = 0;
         }
     }
     if (f[15].check == 0) {
         if (sp[1].i_number[15] >= 10) {
             string filename;
             filename = "./Game/story/event" + to_string(10) + string(".json");
             readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);
             f[15].check = 1;
         }
     }
     if (f[16].check == 0) {
         if (m_id == 8 && p[0].x <= 2 && p[0].y == 0) {
             string filename;
             filename = "./Game/story/event" + to_string(11) + string(".json");
             readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);
             f[16].check = 1;
         }   
     }
     if (f[17].check == 0) {
         if (m_id == 8 && p[0].x <= 2 && p[0].y == 0) {
          IMAGE block;
         ExMessage em;
         loadimage(&block,L"./Game/picture/lose_chose.png", 0, 0, false);
         RECT rect={500+5,350+5,505+240,355+60};
         putimage(500, 350, &block);
         settextstyle(18, 0, _T("Taipei Sans TC Beta"));
         drawtext(L"前方有危險的氣息，是否繼續前進", &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
         FlushBatchDraw();
         while (1) {
             em = getmessage(EM_MOUSE);
             if (em.lbutton) {
             if (em.x>500+21&&em.x<500+21+72&&em.y>350+67&&em.y<350+67+25) {
                 f[17].check= 1;
                 f[18].check = 1;
                 break;
             }
             else if (em.x >500+149 && em.x< 500+149+72&& em.y>350+67 && em.y <350+67+25 ) {
                string filename = "./Game/story/event" + to_string(20) + string(".json");
                readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);
                p[0].y = 1;
                break;
             }
             }

         }        
         }
     }
     if (f[18].check == 1) {
          string filename;
         p[2].name = L"女騎士";
         filename = "./Game/story/event" + to_string(12) + string(".json");
         readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);
         mciSendString(L"close ma", NULL, 0, NULL);
         filename = "./Game/story/event" + to_string(13) + string(".json");
         readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);
         p[2].name = L"愛麗絲";
         filename = "./Game/story/event" + to_string(14) + string(".json");
         readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);
         filename = "./Game/story/event" + to_string(15) + string(".json");
         readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);
         filename = "./Game/story/event" + to_string(16) + string(".json");
         readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);         
        filename = "./Game/story/event" + to_string(17) + string(".json");
         readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);
         filename = "./Game/story/event" + to_string(18) + string(".json");
         readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);
         filename = "./Game/story/event" + to_string(19) + string(".json");
         readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);       
         b_mid = 4;
         f[18].check = 0;          
         }
     if (f[19].check ==1) {
         string filename;
         filename = "./Game/story/event" + to_string(21) + string(".json");
         readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);
         f[19].check = 0;
         f[20].check = 1;
     }
     if (f[20].check == 1) {
         string filename;
         filename = "./Game/story/event" + to_string(22) + string(".json");
         readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);
         m_id = 2;
         p[0].x = 22;
         p[0].y = 18;
         f[20].check = 0;
         m[2].nsize = 0;
         tk[5].state = 1;
         EX[1].state = 0;
         m_set(m, n, p, e_n, Box, m_id, b_id);
         f[21].check = 1;
     }
     if (f[21].check == 1) {
         if (m_id == 2 && p[0].x == 19 && p[0].y == 11) {
             string filename;
             /*             filename = "./Game/story/event" + to_string(23) + string(".json");
             readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);
             filename = "./Game/story/event" + to_string(24) + string(".json");
             readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);*/
             filename = "./Game/story/event" + to_string(99) + string(".json");
             readeventjson(p, n, f, m, Box, tk, m_f, it, st, filename.c_str(), m_id, b_id);
             p[2].box_id[0] =5;
             p[2].box_size[0] = 1;
             p[2].box_id[1] = 5;
             p[2].box_size[1] = 1;
             p[2].box_id[2] = 17;
             p[2].box_size[2] = 2;
             b_mid = 1;
             f[21].check = 0;
         }
     }
     if (m_f[1].check == 1) {
         EX[2].state = true;
     }
     if (m_f[2].check == 1) {
         EX[6].state = true;
     }
     if (m_f[7].check == 1&&m_f[8].check==1) {
         m_f[9].check = 0;
     }
}
void n_put(npc* n,Map *M,player *p,int m_id) {
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
            transparentimage(NULL, n[a].x * 48 - M[m_id].ox, n[a].y * 48 - 16 - M[m_id].oy, &t, 0xFF55FF, 64 + uk[n[a].avatar], 64 * (n[a].avatar - 1), 48, 64);
            k++;
            sum = "";
        }
        sum += M[m_id].npcid[k];
}
}
void m_put(player* p,Map *m,int m_id) {
    if (p[0].pose == 1) {
            transparentimage(NULL, p[0].x * 48-m[m_id].ox, p[0].y * 48 - 16 - m[m_id].oy,&player_image, 0xFF55FF, 64+8, 0, 48, 64);
    }
    else if (p[0].pose == 2) {
        transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48-16 - m[m_id].oy, &player_image, 0xFF55FF, 64+16, 64, 48, 64);
    }
    else if (p[0].pose == 3) {
        transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48-16 - m[m_id].oy, &player_image, 0xFF55FF, 64, 128,48, 64);
    }
    else if (p[0].pose == 4) {
        transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48-16 - m[m_id].oy, &player_image, 0xFF55FF, 64+8, 192, 48, 64);
    }
}
void box_put(BOX* box,int b_id,Map *m,int m_id) {
    IMAGE  p1,p2;
    wstring mm;
    for (i = 0; i < b_id; i++) {
        if (box[i].check == true) {           
            if (box[i].state == true) {  
                mm = L"./Game/picture/box" + to_wstring(box[i].type) + L"_off.png";
                loadimage(&p1,mm.c_str(), 0.0, false);
                transparentimage(NULL, 48*box[i].x-m[m_id].ox, 48 * box[i].y - m[m_id].oy, &p1, 0xFF55FF);
            }
            else if (box[i].state == false) { 
                mm = L"./Game/picture/box" + to_wstring(box[i].type) + L"_on.png";
                loadimage(&p2, mm.c_str(), 0.0, false);
                transparentimage(NULL, 48 * box[i].x - m[m_id].ox, 48 * box[i].y - m[m_id].oy, &p2, 0xFF55FF);
            }
        }
    }
}
void m_map(player* p, Map* m, BOX* Box, int m_id,int b_id, npc* n, string g) {
    m[m_id].ox = (p[0].x > 13) * (p[0].x - 13) * 48 - (14 - (m[m_id].x - p[0].x)) * (p[0].x + 14 > m[m_id].x) * 48;
    m[m_id].oy = (p[0].y > 10) * (p[0].y - 10) * 48 - (10 - (m[m_id].y - p[0].y)) * (p[0].y + 10 > m[m_id].y) * 48;
    if (g == "w") {
        if (p[0].pose == 1) {
            m[m_id].oy += (-48 + 16 * (Z + 1)) * (p[0].y + 10 <= m[m_id].y) * (p[0].y > 10);
            putimage(0, 0, 1296, 960, &mapP, m[m_id].ox, m[m_id].oy);
        }
        else if (p[0].pose == 2) {
            m[m_id].ox += (48 - 16 * (Z + 1)) * (p[0].x >= 13) * (p[0].x + 14 < m[m_id].x);
            putimage(0, 0, 1296, 960, &mapP, m[m_id].ox, m[m_id].oy);
        }
        else if (p[0].pose == 3) {
            m[m_id].ox += (-48 + 16 * (Z + 1)) * (p[0].x > 13) * (p[0].x + 14 <= m[m_id].x);
            putimage(0, 0, 1296, 960, &mapP, m[m_id].ox, m[m_id].oy);
        }
        else if (p[0].pose == 4) {
            m[m_id].oy += (48 - 16 * (Z + 1)) * (p[0].y >= 10) * (p[0].y + 10 < m[m_id].y);
            putimage(0, 0, 1296, 960, &mapP, m[m_id].ox, m[m_id].oy);
        }
        else {
            putimage(0, 0, 1296, 960, &mapP, m[m_id].ox, m[m_id].oy);
        }
    }
    else {
        putimage(0, 0, 1296, 960, &mapP, m[m_id].ox, m[m_id].oy);
    }    
    box_put(Box, b_id,m,m_id);
    for (int I = 0; I < m[m_id].nsize; I++) {
        n_put(n, m,p,m_id);
    }
}
string  m_act(player *p,Map *m,npc *n,int m_id) {
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
                        p[0].pose = 1; Z = 0;
                            return "w";
                    }
                    else {
                        p[0].pose = 1; return"";
                    }
                }
                else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
                    if (p[0].x > 0 && ss[p[0].x - 1][p[0].y] == 0) {
                        p[0].x--;
                        p[0].pose = 2; Z = 0;
                        return"w";
                    }
                    else {
                        p[0].pose = 2; return"";
                    }
                }
                else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
                    if (p[0].x < m[m_id].x - 1 && ss[p[0].x + 1][p[0].y] == 0) {
                        p[0].x++;
                        p[0].pose = 3; Z = 0;
                        return"w";
                    }
                    else {
                        p[0].pose = 3; return"";
                    }
                }
                else if (GetAsyncKeyState(VK_UP) & 0x8000) {
                    if (p[0].y > 0 && ss[p[0].x][p[0].y - 1] == 0) {
                        p[0].y--;
                        p[0].pose = 4; Z = 0;
                        return"w";
                    }
                    else {
                        p[0].pose = 4; return"";
                    }
                }            
        return"";
}
void m_set(Map* m, npc* n, player* p,e_npc *e_n ,BOX *Box,int m_id,int b_id) {

    int k, w, x, y;
    string number="",box="";   
    string filename;
        filename= "./Game/map/map"+to_string(m_id) + string(".json");
    const char* path = filename.c_str();
    readmapjson(m,m_id,path);
    wstring mm,vos;
    LPCTSTR Path;
    loadimage(&player_image, L"./Game/picture/p0.png", 0, 0, false);
    if (m[m_id].bgm != -1) {    
        mciSendString(L"close bgm", NULL, 0, NULL);
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
    IMAGE boxx,ep; 
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
                            e_n[i].move = 0; e_n[i].pose = 1; e_n[i].type = 1; e_n[i].see = 0; e_n[i].live = 1; ss[e_n[i].x][e_n[i].y] = 1; e_n[i].kind = stoi(number); 
                            mm = L"./Game/picture/enemy" + to_wstring(stoi(number)) + L".png";
                            loadimage(&ep, mm.c_str(), 0, 0, false);
                            enemyimage[i] = ep;
                            i++;
                            w--;
                            break;
                        }
                        else {
                            e_n[i].move = 0; e_n[i].pose = 1; e_n[i].type = 1; e_n[i].see = 0; e_n[i].live = 1; ss[e_n[i].x][e_n[i].y] = 1; e_n[i].kind = stoi(number); 
                            mm= L"./Game/picture/enemy" +to_wstring(stoi(number)) + L".png";                           
                            loadimage(&ep, mm.c_str(), 0, 0, false); 
                            enemyimage[i] = ep;
                            i++;
                            w--;
                            number = "";
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
    if (p[0].pose == 1) {
        if (Z == 0) {            
            transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y*48  - 16 - m[m_id].oy+ (-48 + 16 * (Z + 1)) , &player_image, 0xFF55FF, 8, 0, 48, 64);
        }
        else if (Z == 1) {
            transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48 - 16 - m[m_id].oy+ (-48 + 16 * (Z + 1)) , &player_image, 0xFF55FF, 128 + 8, 0, 48, 64);
        }
        else if (Z == 2) {
            transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48 - 16 - m[m_id].oy+ (-48 + 16 * (Z + 1)) , &player_image, 0xFF55FF, 64 + 8, 0, 48, 64);
            ss[p[0].x][p[0].y - 1] = 0;
            ss[p[0].x][p[0].y] = 1;
        }
        }
    else if (p[0].pose == 2) {

            if (Z == 0) {
                transparentimage(NULL, p[0].x*48  - m[m_id].ox+ (48 - 16 * (Z + 1)) , p[0].y * 48 - 16 - m[m_id].oy, &player_image, 0xFF55FF, 16, 64, 48, 64);
            }
            else if (Z == 1) {
                transparentimage(NULL, p[0].x * 48  - m[m_id].ox+ (48 - 16 * (Z + 1)) , p[0].y * 48 - 16 - m[m_id].oy, &player_image, 0xFF55FF, 128 + 16, 64, 48, 64);
            }
            else if (Z == 2) {
                transparentimage(NULL, p[0].x * 48  - m[m_id].ox+ (48 - 16 * (Z + 1)) , p[0].y * 48 - 16 - m[m_id].oy, &player_image, 0xFF55FF, 64 + 16, 64, 48, 64);
                ss[p[0].x + 1][p[0].y] = 0;
                ss[p[0].x][p[0].y] = 1;
            }

    }
    else if (p[0].pose == 3) {

            if (Z == 0) {
                transparentimage(NULL, p[0].x * 48  - m[m_id].ox+ (-48 + 16 * (Z + 1)), p[0].y * 48 - 16 - m[m_id].oy, &player_image, 0xFF55FF, 0, 128, 48, 64);
            }
            else if (Z == 1) {
                transparentimage(NULL, p[0].x * 48  - m[m_id].ox+ (-48 + 16 * (Z + 1)) , p[0].y * 48 - 16 - m[m_id].oy, &player_image, 0xFF55FF, 128 + 0, 128, 48, 64);
            }
            else if (Z == 2) {
                transparentimage(NULL, p[0].x * 48  - m[m_id].ox+ (-48 + 16 * (Z + 1)), p[0].y * 48 - 16 - m[m_id].oy, &player_image, 0xFF55FF, 64 + 0, 128, 48, 64);
                ss[p[0].x - 1][p[0].y] = 0;
                ss[p[0].x][p[0].y] = 1;
            }

        }
    else if (p[0].pose == 4) {
            if (Z == 0) {
                transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48 - 16 - m[m_id].oy+ (48 - 16 * (Z + 1)) , &player_image, 0xFF55FF, 8, 192, 48, 64);
            }
            else if (Z == 1) {
                transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48 - 16 - m[m_id].oy+ (48 - 16 * (Z + 1)) , &player_image, 0xFF55FF, 128 + 8, 192, 48, 64);
            }
            else if (Z == 2) {
                transparentimage(NULL, p[0].x * 48 - m[m_id].ox, p[0].y * 48  - 16 - m[m_id].oy+ (48 - 16 * (Z + 1)) , &player_image, 0xFF55FF, 64 + 8, 192, 48, 64);
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
        ExMessage m;
    BeginBatchDraw();
    while (1) {    
        if (T == true) {
            putimage(0, 0, &start1);
            putimage(493, 739, &startblock);
        }
        else {
            putimage(0, 0, &start2);
            putimage(493, 739, &loadblock);
        }
        m = getmessage(EM_MOUSE);
        if (m.lbutton) {
            if (m.x>300&&m.x<370&&m.y>730&&m.y<840) {
                if (T == true) { T = false; }
                else { T = true; }
                Sleep(100);
            }
            else if (m.x>930&&m.y>740&&m.x<1000&&m.y<840) {
                if (T == true) { T = false; }
                else { T = true; }
                Sleep(100);
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
        FlushBatchDraw();  
        Sleep(10);
        flushmessage(-1);
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
void menu_load(player* p, arms* ar, item* it, stone* st, flag* f,Exit* EX, m_flag* m_f,t_equip *t_E, shop* sp, BOX* Box,task *tk,Map *m,e_npc *e_n, npc *n,int &P_id, int i_id, int ar_id, int st_id, int f_id, int &psize, time_t first, time_t two, time_t &three,int &m_id,int ex_id,int sk_id,int m_fid,int t_Eid, int sp_id, int b_id,int tk_id) {
    settextcolor(WHITE);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    string a, b = ".txt", read = "";
    int ssss = 1; j = 0;
    wstring vos,mm;
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    while (ssss != 0) {
        int sss = 1;
        IMAGE saveblock,sP;
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
                mm = L"./Game/save/" + to_wstring(i + 1 + j * 14) + L".png";
                loadimage(&sP, mm.c_str(), 108, 80, false);
                putimage(458, 38 + 128 * i, &sP);
                outtextxy(265, 55 + 128 * i, m[root["m_id"].asInt()].name.c_str());
                mm = to_wstring(hr) + L":" + to_wstring(min) + L":" + to_wstring(sec);
                outtextxy(110, 60 + 128 * i, mm.c_str());
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
                mm = L"./Game/save/" + to_wstring(i + 8 + j * 14) + L".png";
                loadimage(&sP, mm.c_str(), 108, 80, false);
                putimage(458 + 600, 38 + 128 * i, &sP);
                outtextxy(265 + 600, 55 + 128 * i, m[root["m_id"].asInt()].name.c_str());
                mm = to_wstring(hr) + L":" + to_wstring(min) + L":" + to_wstring(sec);
                outtextxy(110 + 600, 60 + 128 * i, mm.c_str());
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
            p[i].arms_b_1 = root["player"]["arms_b_1"][i].asInt();
            p[i].arms_b_2= root["player"]["arms_b_2"][i].asInt();
            p[i].b_id_1 = root["player"]["b_id_1"][i].asInt();
            p[i].b_id_2 = root["player"]["b_id_2"][i].asInt();
            p[i].sp = root["player"]["sp"][i].asInt();
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
        /*商店*/
        for (i = 0; i < 2; i++) {
            string sj = "ar_number_" + to_string(i);            
            for (j = 0; j < ar_id; j++) {
                sp[i].ar_number[j] = root["shop"][sj][j].asInt();
            }
            sj = "i_number_" + to_string(i);
            for (j = 0; j < i_id; j++) {
                sp[i].i_number[j] = root["shop"][sj][j].asInt();
            }
            sj = "t_E_number_" + to_string(i);
            for (j = 0; j < t_Eid; j++) {
                sp[i].t_E_number[j] = root["shop"][sj][j].asInt();
            }
        }
        for (i = 0; i < b_id; i++) {
           Box[i].state=root["box_state"][i].asBool();
        }
        for (i = 0; i < tk_id; i++) {
            tk[i].state = root["task_state"][i].asInt();
        }
        P_id = root["P_id"].asInt();
        three = root["time"].asInt();
        m_id = root["m_id"].asInt();
        voiceSize = root["voiceSize"].asInt();
        soundSize = root["soundSize"].asInt();
        first = time(NULL);
        m_map(p, m, Box, m_id, b_id, n, ""); m_put(p, m, m_id); m_set(m, n, p, e_n, Box, m_id, b_id); m_put(p, m, m_id);
    }
}
void item_heal(player *p,item *it,int i_id,int psize) {
    IMAGE b,p1,ch;
    wstring mm;
    ExMessage em;
    loadimage(&b, L"./Game/picture/p_iblock.png", 0, 0, false);
    BeginBatchDraw();
    for (j = 0; j < 5; j++) {
        putimage(0, j * 180, &b);
    }
    for (j = 0; j < psize; j++) {       
        mm = L"./Game/picture/player" + to_wstring(j) + L"_1.png";
        loadimage(&p1, mm.c_str(), 0, 0, false);
        transparentimage(NULL, 18, j*180+18, &p1, 0xFF55FF);
        mm = L"HP " + to_wstring(p[j].hp) + L"/" + to_wstring(p[j].mhp);
        outtextxy(170, j * 180 + 20, mm.c_str());
    }
    EndBatchDraw();
    BeginBatchDraw();
    while (1) {
        em = getmessage(EM_MOUSE);
        for (j = 0; j < psize; j++) {
        if (em.x > 0 && em.x<400 && em.y>j * 180 && em.y < j * 180 + 180&&em.lbutton) {
            if (i == 1) {
                if (p[j].hp + 11 >= p[j].mhp) {
                    loadimage(&ch, L"./Game/picture/skill_chose.png", 0, 0, false);
                    putimage(500, 400, &ch);
                    RECT r = { 500,400, 750, 500 };
                    mm = L"道具部分效果浪費，是否使用?";
                    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                    drawtext(mm.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                    FlushBatchDraw();
                    while (1) {
                        em = getmessage(EM_MOUSE);
                        if (em.lbutton) {
                            if (em.x > 521 && em.x < 593 && em.y>467 && em.y < 491) {
                    p[j].hp = p[j].mhp;
                    it[i].number--;
                                break;
                            }
                            else if (em.x > 649 && em.x < 721 && em.y>467 && em.y < 491) {
                                break;
                            }
                        }
                    }
                }
                else {   p[j].hp += 11;
                                it[i].number--;                  
                }
               
            }
            else if (i == 5) {
                if (p[j].hp + 10 >=p[j].mhp) {                  
                    loadimage(&ch, L"./Game/picture/skill_chose.png", 0, 0, false);
                    putimage(500, 400, &ch);
                    RECT r = { 500,400, 750, 500 };
                    mm = L"道具部分效果浪費，是否使用?";
                    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                    drawtext(mm.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                    FlushBatchDraw();
                    while (1) {
                        em = getmessage(EM_MOUSE);
                        if (em.lbutton) {
                            if (em.x > 521 && em.x < 593 && em.y>467 && em.y < 491) {
                         p[j].hp = p[j].mhp;   
                         it[i].number--;
                                break;
                            }
                            else if (em.x > 649 && em.x < 721 && em.y>467 && em.y < 491) {
                                break;
                            }
                        }
                    }
                }
                else {  
                    p[j].hp += 10;
                    it[i].number--;
                }
            
            }
            else if (i == 10) {
                if (p[j].mhp / 5 + p[j].hp >= p[j].mhp) {
                    loadimage(&ch, L"./Game/picture/skill_chose.png", 0, 0, false);
                    putimage(500, 400, &ch);
                    RECT r = { 500,400, 750, 500 };
                    mm = L"道具部分效果浪費，是否使用?";
                    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                    drawtext(mm.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                    FlushBatchDraw();
                    while (1) {
                        em = getmessage(EM_MOUSE);
                        if (em.lbutton) {
                            if (em.x > 521 && em.x < 593 && em.y>467 && em.y < 491) {
                                p[j].hp = p[j].mhp;
                                it[i].number--;

                                break;
                            }
                            else if (em.x > 649 && em.x < 721 && em.y>467 && em.y < 491) {
                                break;
                            }
                        }
                    }
                }
                else {
                    p[j].hp += p[j].mhp / 5;
                    it[i].number--;
                }
            }
            else if (i == 11) {
                if (p[j].mhp / 4 + p[j].hp >= p[j].mhp) {
                    loadimage(&ch, L"./Game/picture/skill_chose.png", 0, 0, false);
                    putimage(500, 400, &ch);
                    RECT r = { 500,400, 750, 500 };
                    mm = L"道具部分效果浪費，是否使用?";
                    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                    drawtext(mm.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                    FlushBatchDraw();
                    while (1) {
                        em = getmessage(EM_MOUSE);
                        if (em.lbutton) {
                            if (em.x > 521 && em.x < 593 && em.y>467 && em.y < 491) {
                                p[j].hp = p[j].mhp;
                                it[i].number--;

                                break;
                            }
                            else if (em.x > 649 && em.x < 721 && em.y>467 && em.y < 491) {
                                break;
                            }
                        }
                    }
                }
                else {
                    p[j].hp += p[j].mhp / 4;
                    it[i].number--;
                }
            }
            else if (i == 13) {
            if (p[j].mhp *3/10 + p[j].hp >= p[j].mhp) {
                loadimage(&ch, L"./Game/picture/skill_chose.png", 0, 0, false);
                putimage(500, 400, &ch);
                RECT r = { 500,400, 750, 500 };
                mm = L"道具部分效果浪費，是否使用?";
                settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                drawtext(mm.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                FlushBatchDraw();
                while (1) {
                    em = getmessage(EM_MOUSE);
                    if (em.lbutton) {
                        if (em.x > 521 && em.x < 593 && em.y>467 && em.y < 491) {
                            p[j].hp = p[j].mhp;
                            it[i].number--;

                            break;
                        }
                        else if (em.x > 649 && em.x < 721 && em.y>467 && em.y < 491) {
                            break;
                        }
                    }
                }
            }
            else {
                p[j].hp += p[j].mhp *3/10;
                it[i].number--;
            }
            }
            else if (i == 14) {
                if (p[j].mhp / 10 + p[j].hp >= p[j].mhp) {
 
                    loadimage(&ch, L"./Game/picture/skill_chose.png", 0, 0, false);
                    putimage(500, 400, &ch);
                    RECT r = { 500,400, 750, 500 };
                    mm = L"道具部分效果浪費，是否使用?";
                    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                    drawtext(mm.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                    FlushBatchDraw();
                    while (1) {
                        em = getmessage(EM_MOUSE);
                        if (em.lbutton) {
                            if (em.x > 521 && em.x < 593 && em.y>467 && em.y < 491) {
                   p[j].hp = p[j].mhp;  
                    it[i].number--;
                                break;
                            }
                            else if (em.x > 649 && em.x < 721 && em.y>467 && em.y < 491) {
                                break;
                            }
                        }
                    }
                }
                else {      p[j].hp += p[j].mhp / 10;
                                it[i].number--;
                }
              
            }
            else if (i == 16) {
                if (p[j].mhp / 4 + p[j].hp >= p[j].mhp) {
                    loadimage(&ch, L"./Game/picture/skill_chose.png", 0, 0, false);
                    putimage(500, 400, &ch);
                    RECT r = { 500,400, 750, 500 };
                    mm = L"道具部分效果浪費，是否使用?";
                    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                    drawtext(mm.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                    FlushBatchDraw();
                    while (1) {
                        em = getmessage(EM_MOUSE);
                        if (em.lbutton) {
                            if (em.x > 521 && em.x < 593 && em.y>467 && em.y < 491) {
                    p[j].hp = p[j].mhp;       
                    it[i].number--;

                                break;
                            }
                            else if (em.x > 649 && em.x < 721 && em.y>467 && em.y < 491) {
                                break;
                            }
                        }
                    }
                }
                else {                                 
                    p[j].hp += p[j].mhp / 4;
                    it[i].number--;
                }
         
            }
            for (j = 0; j < 5; j++) {
                putimage(0, j * 180, &b);
            }
            for (j = 0; j < psize; j++) {
                mm = L"./Game/picture/player" + to_wstring(j) + L"_1.png";
                loadimage(&p1, mm.c_str(), 0, 0, false);
                transparentimage(NULL, 18, j * 180 + 18, &p1, 0xFF55FF);
                mm = L"HP " + to_wstring(p[j].hp) + L"/" + to_wstring(p[j].mhp);
                outtextxy(170, j * 180 + 20, mm.c_str());
            }
            EndBatchDraw();
            return;
        }
        
        }      
        if (em.rbutton) {
            break;
        }  
    }
}
void item_skill(player *p,skill *sk,int sk_id,int psize) {
    IMAGE b, p1,b1,text,get,ch;
    wstring mm;
    ExMessage em;    
    int x, y,up[12];
    loadimage(&b, L"./Game/picture/p_iblock.png", 0, 0, false);
    BeginBatchDraw();
    for (j = 0; j < 5; j++) {
        putimage(0, j * 180, &b);
    }
    for (j = 0; j < psize; j++) {
        mm = L"./Game/picture/player" + to_wstring(j) + L"_1.png";
        loadimage(&p1, mm.c_str(), 0, 0, false);
        transparentimage(NULL, 18, j * 180 + 18, &p1, 0xFF55FF);
        mm = L"HP " + to_wstring(p[j].hp) + L"/" + to_wstring(p[j].mhp);
        outtextxy(170, j * 180 + 20, mm.c_str());
    }
    EndBatchDraw();
    while (1) {
        flushmessage(-1);
        em = getmessage(EM_MOUSE);
        for (j = 0; j < psize; j++) {
            if (em.x > 0 && em.x<400 && em.y>j * 180 && em.y < j * 180 + 180 && em.lbutton) {
                if (j == 0) {                 
                   int  zp[12] = { 4,1,0,7,6,2,5,8,9,10,11,12 };
                   for (int I = 0; I < 12; I++) {
                       up[I] = zp[I];
                   }
                }
                else if (j == 1) {
                   int  zp[12] = { 16,3,17,7,-1,13,14,19,18,15,11,-1 };
                   for (int I = 0; I < 12; I++) {
                       up[I] = zp[I];
                   }
                }
                pair<int, int> z[12];
                RECT r;
                z[0].first = 234; z[0].second = 333; z[1].first = 1049; z[1].second = 453; z[2].first = 968; z[2].second = 732; z[3].first = 626; z[3].second = 901; z[4].first = 627; z[4].second = 60; z[5].first = 305; z[5].second = 747; z[6].first = 292; z[6].second = 363; z[7].first = 978; z[7].second = 455; z[8].first = 909; z[8].second = 696; z[9].first = 357; z[9].second = 713; z[10].first = 625; z[10].second = 832; z[11].first = 629; z[11].second = 133;
                mm = L"./Game/picture/skillB" + to_wstring(j) + L".png";
                loadimage(&b1, mm.c_str(), 0, 0, false);
                loadimage(&text, L"./Game/picture/skill_text.png", 0, 0, false);
                BeginBatchDraw();
                putimage(0, 0, &b1);
                for (i = 0; i < 12; i++) {    /*技能數量*/
                    if (p[j].s_check[up[i]] > 0&&up[i]!=-1) {
                        circle(z[i].first, z[i].second, 35);
                    }
                }
                EndBatchDraw();
                getimage(&get, 0, 0, 1296, 960);
                while (1) {
                    flushmessage(-1);
                    em = getmessage(EM_MOUSE);
                    BeginBatchDraw();
                    putimage(0, 0, &get);
                    mm = L"SP:" + to_wstring(p[j].sp);
                    outtextxy(1100, 50, mm.c_str());
                    for (i = 0; i < 12; i++) {/*技能數量*/
                        if (sqrt(pow(z[i].first - em.x, 2) + pow(z[i].second - em.y, 2)) < 28) {
                            if (em.lbutton&&up[i]!=-1) {
                                if (p[j].s_check[up[i]] == 0) {
                                    loadimage(&ch, L"./Game/picture/skill_chose.png", 0, 0, false);
                                    putimage(500, 400, &ch);
                                    r = { 500,400, 750, 500 };
                                    mm = L"是否要習得" + sk[up[i]].name + L"?(" + to_wstring(sk[up[i]].cost) + L"SP)";
                                    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                                    drawtext(mm.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                                    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                                    EndBatchDraw();
                                    while (1) {
                                        em = getmessage(EM_MOUSE);
                                        if (em.lbutton) {
                                            if (em.x > 521 && em.x < 593 && em.y>467 && em.y < 491) {
                                                if (p[j].sp >= sk[up[i]].cost) {
                                                    p[j].sp -= sk[up[i]].cost;
                                                    p[j].s_check[up[i]]++;
                                                    BeginBatchDraw();
                                                    putimage(500, 400, &text);
                                                    mm = L"已習得" + sk[up[i]].name;
                                                    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                                                    drawtext(mm.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                                                    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                                                    EndBatchDraw();
                                                    Sleep(2000);
                                                    BeginBatchDraw();
                                                    putimage(0, 0, &get);
                                                    for (i = 0; i < 12; i++) {/*技能數量*/
                                                        if (p[j].s_check[up[i]] > 0&&up[i]!=-1) {
                                                            circle(z[i].first, z[i].second, 35);
                                                        }
                                                    }
                                                    EndBatchDraw();
                                                    getimage(&get, 0, 0, 1296, 960);
                                                    break;
                                                }
                                                else {
                                                    BeginBatchDraw();
                                                    putimage(500, 400, &text);
                                                    mm = L"沒有足夠的SP習得" + sk[up[i]].name;
                                                    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                                                    drawtext(mm.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                                                    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                                                    EndBatchDraw();
                                                    Sleep(2000);
                                                    break;
                                                }
                                            }
                                            else if (em.x > 649 && em.x < 721 && em.y>467 && em.y < 491) {
                                                break;
                                            }
                                        }
                                    }
                                }
                                else  {
                                    putimage(500, 400, &text);
                                    r = { 500,400, 750, 500 };
                                    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                                    mm = L"此技能已習得過";
                                    drawtext(mm.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                                    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                                    EndBatchDraw();
                                    Sleep(2000);
                                }
                            }
                            else if(up[i]!=-1) {
                                if (z[i].first >= 1046) {
                                    x = 1046;
                                }
                                else {
                                    x = z[i].first + 28;
                                }
                                if (z[i].second >= 860) {
                                    y = 860;
                                }
                                else {
                                    y = z[i].second + 28;
                                }
                                putimage(x, y, &text);
                                r = { x,y, x + 250, y + 130 };
                                settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                                drawtext((sk[up[i]].name + L"\n" + sk[up[i]].story).c_str(), &r, DT_WORDBREAK);
                                settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                            }

                        }

                    }

                    if (em.rbutton) {
                        break;
                    }
                    EndBatchDraw();
                    Sleep(10);
                }
            }
        }
        if (em.rbutton) {
            break;
        }
    }
}
char menu_item(player *p,item *it,arms *ar,t_equip *t_E,stone *st,int i_id,int ar_id,int st_id,int t_Eid) {
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
    int kp = -1;
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
                    if (it[i].number != 0 && it[i].type == 'e') {
                        if (chose == j) {
                            kp = i;
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
                  if (it[i].number != 0&&it[i].type=='b') {
                                if (chose == j) {
                                   kp=i;
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
                            kp=i;
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
                for (i = 0; i < st_id; i++) {
                    if (st[i].number != 0) {
                        if (chose == j) {
                            return NULL;
                        }
                        J = j / 2;
                        wstring mm;
                        mm = std::to_wstring(i);
                        mm = L"./Game/picture/stone" + mm + L".png";
                        LPCTSTR path = mm.c_str();
                        loadimage(&p2, path, 0, 0, false);
                        if (j % 2 == 0) {
                            transparentimage(NULL, 420, 200 + J * 40, &p2, 0xFF55FF);
                            LPCTSTR path1 = st[i].name.c_str();
                            outtextxy(445, 200 + J * 40, path1);
                            outtextxy(780, 200 + J * 40, _T(":"));
                            mm = std::to_wstring(st[i].number);
                            path1 = mm.c_str();
                            outtextxy(790, 200 + J * 40, path1);
                            j++;

                        }
                        else if (j % 2 == 1) {
                            transparentimage(NULL, 834, 200 + J * 40, &p2, 0xFF55FF);
                            LPCTSTR path1 = st[i].name.c_str();
                            outtextxy(859, 200 + J * 40, path1);
                            outtextxy(1194, 200 + J * 40, _T(":"));
                            mm = std::to_wstring(st[i].number);
                            path1 = mm.c_str();
                            outtextxy(1204, 200 + J * 40, path1);
                            j++;
                        }
                        if (mc == j) {
                            LPCTSTR path1 = st[i].story.c_str();
                            drawtext(path1, &r, DT_WORDBREAK);
                        }
                    }
                }
                if (kp != -1) {
                    i = kp;
                    return it[i].type;
                }
            }
            else if (pchose == 2) {
                for (i = 0; i < i_id; i++) {
                  if (it[i].number != 0 && it[i].type == 'm') {
                    if (chose == j&&i==6) {
                        kp = i;
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
            if (kp != -1) {
                i = kp;
                return it[i].type;
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
        flushmessage(-1);
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
        if (p[I].arms_id_1 != -1) {
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
        }

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
        if (p[I].stone_id != -1) {
        mm = std::to_wstring(p[I].stone_id);
        mm = L"./Game/picture/stone" + mm + L".png";
        path1 = mm.c_str();
        loadimage(&p9, path1, 0, 0, false);
        transparentimage(NULL, 950, 335, &p9, 0xFF55FF);
        path1 = st[p[I].stone_id].name.c_str();
        outtextxy(1000, 333, path1);
        }
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
                    if(p[I].arms_id_1!=-1){
                    path1 = ar[p[I].arms_id_1].story.c_str();
                    }
                    else {
                        path = L"";
                    }
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
                    if (p[I].armor_id != -1) {
                        path1 = Ar[p[I].armor_id].story.c_str();
                    }
                    else {
                        path1 = L"";
                    }
                }
                else if (i == 3) {
                    if (p[I].stone_id != -1) {
                    path1 = st[p[I].stone_id].story.c_str();
                    }
                    else {
                        path1 = L"";
                    }
                }
                else if (i == 4) {
                    if (p[I].t_id != -1) {
                    path1 = t_E[p[I].t_id].story.c_str();
                    }
                    else {
                        path1 = L"";
                    }
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
        if (p[I].arms_id_1 != -1) {
        if (ar[p[I].arms_id_1].type[0] == 'r') {
            if (m.x >= 1215 && m.x <= 1245 && m.y >= 210 && m.y <= 240) {
                if (m.lbutton) {
                    d = 7;
                }
            }
        }
        }
        if (p[I].arms_id_2 != -1) {
        if (ar[p[I].arms_id_2].type[0] == 'r') {
            if (m.x >= 1215 && m.x <= 1245 && m.y >= 250 && m.y <= 280) {
                if (m.lbutton) {
                    d = 8;
                }
            }
        }
        }        
        int arm[100],sto[100];
        if (d == 1) {
            int J = 0, j = 1;
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
            mm = L"卸下";
            outtextxy(600, 570, mm.c_str());
            if (m.x > 420 && m.x < 830&& m.y>570  && m.y < 605 ) {
                   transparentimage(NULL, 415, 563, &p8);
                   if (m.lbutton) {
                       if (p[I].arms_id_1 != -1) {
                           ar[p[I].arms_id_1].number++;
                           p[I].arms_b_1 = 1;
                               p[I].b_id_1 = -1;
                           p[I].arms_id_1 = -1;
                       }
                   }
            }     
            for (i = 1; i < j; i++) {
                            if (m.x > 420+(i%2)*414 && m.x < 830+(i%2)*414 && m.y>570+(i/2)*40 && m.y < 605+(i/2)*40) {
                                transparentimage(NULL, 415 + (i % 2) * 414,563 + (i / 2) * 40, &p8);     
                                path1 = ar[arm[i]].story.c_str();
                                drawtext(path1, &r, DT_WORDBREAK);
                                if (m.lbutton) {
                                    ar[p[I].arms_id_1].number++;
                                    p[I].arms_b_1 = 1;
                                    if (ar[arm[i]].type[0] == 'r') {
                                        p[I].arms_b_1 = 0;
                                        p[I].b_id_1 = -1;
                                    }
                                    p[I].arms_id_1 = arm[i];
                                    ar[arm[i]].number--;
                                }
                            }
                       }
        }
        else if (d == 2) {
            int J = 0, j = 1;
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
            mm = L"卸下";
            outtextxy(600, 570, mm.c_str());
            if (m.x > 420 && m.x < 830 && m.y>570 && m.y < 605) {
                transparentimage(NULL, 415, 563, &p8);
                if (m.lbutton) {
                    if (p[I].arms_id_2 != -1) {
                        ar[p[I].arms_id_2].number++;
                        p[I].arms_b_2 = 1;
                        p[I].b_id_2 = -1;
                        p[I].arms_id_2 = -1;
                    }
                }
            }
            for (i = 1; i < j; i++) {
                if (m.x > 420 + (i % 2) * 414 && m.x < 830 + (i % 2) * 414 && m.y>570 + (i / 2) * 40 && m.y < 605 + (i / 2) * 40) {
                    transparentimage(NULL, 415 + (i % 2) * 414, 563 + (i / 2) * 40, &p8);
                    path1 = ar[arm[i]].story.c_str();
                    drawtext(path1, &r, DT_WORDBREAK);
                    if (m.lbutton) {
                        ar[p[I].arms_id_2].number++;
                        p[I].arms_b_2 = 1;
                        if (ar[arm[i]].type[0] == 'r') {
                            p[I].arms_b_2 = 0;
                            p[I].b_id_2 = -1;
                        }
                        p[I].arms_id_2 = arm[i];                        
                        ar[arm[i]].number--;
                    }
                }
            }
        }
        /*缺防具d==3*/
        else if (d == 4) {
            int J = 0, j = 1;
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
            mm = L"卸下";
            outtextxy(600, 570, mm.c_str());
            if (m.x > 420 && m.x < 830 && m.y>570 && m.y < 605) {
                transparentimage(NULL, 415, 563, &p8);
                if (m.lbutton) {
                    if (p[I].stone_id != -1) {
                        change(p, I, st[p[I].stone_id].type, "");
                        st[p[I].stone_id].number++;
                        p[I].stone_id = -1;
                    }
                }
            }
            for (i = 1; i < j; i++) {
                if (m.x > 420 + (i % 2) * 414 && m.x < 830 + (i % 2) * 414 && m.y>570 + (i / 2) * 40 && m.y < 605 + (i / 2) * 40) {
                    transparentimage(NULL, 415 + (i % 2) * 414, 563 + (i / 2) * 40, &p8);
                    path1 = st[sto[i]].story.c_str();
                    drawtext(path1, &r, DT_WORDBREAK);
                    if (m.lbutton) {
                        if (p[I].stone_id != -1) {
                        change(p,I,st[p[I].stone_id].type,st[sto[i]].type);
                        st[p[I].stone_id].number++;
                        p[I].stone_id = sto[i];
                        st[sto[i]].number--;
                        }
                        else {
                            change(p, I,"", st[sto[i]].type);
                            p[I].stone_id = sto[i];
                            st[sto[i]].number--;
                        }
                    }
                }
            }
        }
        else if (d == 5) {
            int J = 0, j = 1;
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
            mm = L"卸下";
            outtextxy(600, 570, mm.c_str());
            if (m.x > 420 && m.x < 830 && m.y>570 && m.y < 605) {
                transparentimage(NULL, 415, 563, &p8);
                if (m.lbutton) {
                    if (p[I].t_id != -1) {
                        for (int uk = 0; uk < t_E[p[I].t_id].size; uk++) {
                            it[p[I].box_id[uk]].number+= p[I].box_size[uk];
                            p[I].box_id[uk] = -1;
                            p[I].box_size[uk] = 0;
                        }
                        t_E[p[I].t_id].number++;
                        p[I].t_id = -1;
                    }
                }
            }
            for (i = 1; i < j; i++) {
                if (m.x > 420 + (i % 2) * 414 && m.x < 830 + (i % 2) * 414 && m.y>570 + (i / 2) * 40 && m.y < 605 + (i / 2) * 40) {
                    transparentimage(NULL, 415 + (i % 2) * 414, 563 + (i / 2) * 40, &p8);
                    path1 = t_E[sto[i]].story.c_str();
                    drawtext(path1, &r, DT_WORDBREAK);
                    if (m.lbutton) {
                        if (I != -1) {
                           t_E[p[I].t_id].number++;
                        }                     
                        for (int uk = 0; uk < t_E[p[I].t_id].size; uk++) {
                            it[p[I].box_id[uk]].number += p[I].box_size[uk];
                            p[I].box_id[uk] = -1;
                            p[I].box_size[uk] = 0;
                        }
                        p[I].t_id = sto[i];
                        t_E[sto[i]].number--;
                    }
                }
            }
        }
        else if (d == 6) {
        int J = 0, j = 1;
        POINT pts[] = { {955+q*30-5,420-5}, {955 -5+ q * 30+30,420-5}, {955-5 + q * 30+30,450-5}, {955 -5+ q * 30,450-5} };
        polygon(pts, 4);
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
        mm = L"卸下";
        outtextxy(600, 570, mm.c_str());
        if (m.x > 420 && m.x < 830 && m.y>570 && m.y < 605) {
            transparentimage(NULL, 415, 563, &p8);
            if (m.lbutton) {
                if (p[I].box_id[q] != -1) {
                    it[p[I].box_id[q]].number += p[I].box_size[q];
                    p[I].box_id[q] = -1;
                    p[I].box_size[q] = 0;
                }
            }
        }
        for (i = 1; i < j; i++) {
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
        int J = 0, j = 1;
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
            for (i = 0; i < i_id; i++) {
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
            
        }
        mm = L"清空";
        outtextxy(600, 570, mm.c_str());
        if (m.x > 420 && m.x < 830 && m.y>570 && m.y < 605) {
            transparentimage(NULL, 415, 563, &p8);
            if (m.lbutton) {
                if (p[I].b_id_1 != -1) {
                    p[I].b_id_1 = -1;
                    p[I].arms_b_1 = 0;
                }
            }
        }
        for (i = 1; i < j; i++) {
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
        int J = 0, j = 1;
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
            for (i = 0; i < i_id; i++) {
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
           
        }
        mm = L"清空";
        outtextxy(600, 570, mm.c_str());
        if (m.x > 420 && m.x < 830 && m.y>570 && m.y < 605) {
            transparentimage(NULL, 415, 563, &p8);
            if (m.lbutton) {
                if (p[I].b_id_2 != -1) {
                    p[I].b_id_2 = -1;
                    p[I].arms_b_2 = 0;
                }
            }
        }
        for (i = 1; i < j; i++) {
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
        Sleep(10);
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
void menu_task(task *tk,int tk_id) {
    IMAGE b1,tl,d,ud;
    ExMessage em;
        RECT r = {411,111,808+411,470};
    int J = 0,chosea=1,choseb=1,chosec=1,chose=-1;
    loadimage(&b1, L"./Game/picture/task_back.png", 0, 0, false);     
    loadimage(&tl, L"./Game/picture/equip_block.png", 0, 0, false);
    loadimage(&d, L"./Game/picture/done.png", 0, 0, false);
    loadimage(&ud, L"./Game/picture/undone.png", 0, 0, false);
    BeginBatchDraw();
    while (1) {
        flushmessage(-1);
        em = getmessage(EM_MOUSE);
        j = 0; J = 0;
        putimage(400, 0, &b1);
        if (chosea == 1) {
            rectangle(410, 10, 410+284, 97);
        for (i = 0; i < tk_id; i++) {
            if (tk[i].state==1&&tk[i].type==0) {
            J = j / 2;
            if (j % 2 == 0) {
                putimage(475, 600 + J * 40+5,&ud);
            outtextxy(500, 600+J*40, tk[i].name.c_str());
            }
            else {
                putimage(850, 600 + J * 40+5, &ud);
                outtextxy(875, 600 + J * 40, tk[i].name.c_str());
            }
            if (em.x >470+375*(j%2) && em.x <350 + 375 * (j % 2) + 470 && em.y>600 + J * 40-3 && em.y < 600 + J * 40 + 35-3) {
                transparentimage(NULL, 470 + 375 * (j % 2), 600 + J * 40-3, &tl);
                if (em.lbutton) {
                    chose = j;
                }
            }
            j++;
            }
            else if (chosec == 1) {
                if (tk[i].state == 2 && tk[i].type == 0) {
                    J = j / 2;
                    if (j % 2 == 0) {
                        putimage(475, 600 + J * 40 + 5, &d);
                        outtextxy(500, 600 + J * 40, tk[i].name.c_str());
                    }
                    else {
                        putimage(850, 600 + J * 40 + 5, &d);
                        outtextxy(875, 600 + J * 40, tk[i].name.c_str());
                    }
                    if (em.x > 470 + 375 * (j % 2) && em.x < 350 + 375 * (j % 2) + 470 && em.y>600 + J * 40 - 3 && em.y < 600 + J * 40 + 35 - 3) {
                        transparentimage(NULL, 470 + 375 * (j % 2), 600 + J * 40 - 3, &tl);
                        if (em.lbutton) {
                            chose = j;
                        }
                    }
                    j++;
                }
            }
        }
        }        
        if (choseb == 1) {
            rectangle(708+3, 7, 708 + 299-3, 95);
            for (i = 0; i < tk_id; i++) {
                if (tk[i].state == 1 && tk[i].type == 1) {
                    J = j / 2;
                    if (j % 2 == 0) {
                        putimage(475, 600 + J * 40 + 5, &ud);
                        outtextxy(500, 600 + J * 40, tk[i].name.c_str());
                    }
                    else {
                        putimage(850, 600 + J * 40 + 5, &ud);
                        outtextxy(875, 600 + J * 40, tk[i].name.c_str());
                    }
                    if (em.x > 470 + 375 * (j % 2) && em.x < 350 + 375 * (j % 2) + 470 && em.y>600 + J * 40 - 3 && em.y < 600 + J * 40 + 35 - 3) {
                        transparentimage(NULL, 470 + 375 * (j % 2), 600 + J * 40 - 3, &tl);
                        if (em.lbutton) {
                            chose = j;
                        }
                    }
                    j++;
                }
                else if (chosec == 1) {
                    if (tk[i].state == 2 && tk[i].type == 1) {
                        J = j / 2;
                        if (j % 2 == 0) {
                            putimage(475, 600 + J * 40 + 5, &d);
                            outtextxy(500, 600 + J * 40, tk[i].name.c_str());
                        }
                        else {
                            putimage(850, 600 + J * 40 + 5, &d);
                            outtextxy(875, 600 + J * 40, tk[i].name.c_str());
                        }
                        if (em.x > 470 + 375 * (j % 2) && em.x < 350 + 375 * (j % 2) + 470 && em.y>600 + J * 40 - 3 && em.y < 600 + J * 40 + 35 - 3) {
                            transparentimage(NULL, 470 + 375 * (j % 2), 600 + J * 40 - 3, &tl);
                            if (em.lbutton) {
                                chose = j;
                            }
                        }
                        j++;
                    }
                }
            }
        }
        if (chosec == 1) {

        }   
        if (chose != -1) {
        drawtext(tk[chose].story.c_str(), &r, DT_WORDBREAK);
        }
        FlushBatchDraw();
        Sleep(10);
        if (em.rbutton) {
            EndBatchDraw();
            return;
        }
    }
}
void menu(player* p,item *it,arms *ar,armor *Ar,stone *st,flag* f,enemy *e,Map *M,npc *n,e_npc *e_n,Exit* EX,BOX *Box,skill *sk, m_flag* m_f,t_equip *t_E,shop *sp,task *tk,int &P_id,int &f_id,int &i_id,int &ar_id,int &Ar_id,int &st_id,int b_id, int &psize,int &roundp, int &roundb, time_t &first, time_t &two, time_t &three,int &m_id,int ex_id,int sk_id,int m_fid,int t_Eid,int sp_id,int tk_id) {
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
    IMAGE p1, option,p2,p3,p4,p5,p6,p7,p8,p9;
    loadimage(&p1, L"./Game/picture/menu.png", 0, 0, false);
    loadimage(&option, L"./Game/picture/option.png", 0, 0, false);
    loadimage(&p2, L"./Game/picture/e_item.png", 0, 0, false);
    loadimage(&p3, L"./Game/picture/e_player.png", 0, 0, false);
    loadimage(&p4, L"./Game/picture/e_equip.png", 0, 0, false);
    loadimage(&p5, L"./Game/picture/e_skill.png", 0, 0, false);
    loadimage(&p6, L"./Game/picture/e_save.png", 0, 0, false);
    loadimage(&p7, L"./Game/picture/e_load.png", 0, 0, false);
    loadimage(&p8, L"./Game/picture/e_system.png", 0, 0, false);
    loadimage(&p9, L"./Game/picture/e_task.png", 0, 0, false);
    ExMessage m,m1;
    int u=0,c=0,g=-1;
    string chose = "_";
    while (1) {
        chose = "_";
        u = 0;
        BeginBatchDraw();
        putimage(0, 0, &p1);
        for (i = 0; i < 9; i++) {
            transparentimage(NULL, 0, 100 * i, &option, 0xFF55FF);
        }
        putimage(25, 20, &p2);
            putimage(0, 0, &p1);
            flushmessage(-1);
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
                        putimage(75, 20 + 100 * i, &p9);
                    }
                    else if (i == 5) {
                        putimage(75, 20 + 100 * i, &p6);
                    }
                    else if (i == 6) {
                        putimage(75, 20 + 100 * i, &p7);
                    }
                    else if (i == 7) {
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
                        putimage(25, 20 + 100 * i, &p9);

                    }
                    else if (i == 5) {
                        putimage(25, 20 + 100 * i, &p6);
                    }
                    else if (i == 6) {
                        putimage(25, 20 + 100 * i, &p7);
                    }
                    else if (i == 7) {
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
            while (chose[0] != 0) {
            chose = menu_item(p, it, ar,t_E,st,i_id,ar_id,st_id,t_Eid);
            if (chose=="h"||chose=="e") {
                item_heal(p,it,i_id,psize);
            }
            else if (chose == "m") {
                item_skill(p, sk, sk_id,psize);
            }
            }            
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
            menu_task(tk,tk_id);
            g = -1;
        }
        else if (g == 5) {
           menu_save(p, ar, it, st, f,EX,m_f,t_E,sp,Box,tk,M,P_id, i_id, ar_id, st_id,f_id, psize, first,two, three,m_id,ex_id,sk_id,m_fid,t_Eid,sp_id,b_id,tk_id);
           g = -1;
        }
        else if (g == 6) {
            menu_load(p, ar, it, st, f,EX, m_f,t_E,sp,Box,tk,M,e_n,n,P_id, i_id, ar_id, st_id, f_id, psize, first, two, three,m_id,ex_id,sk_id,m_fid,t_Eid,sp_id,b_id,tk_id);
            return;
        }
        else if (g == 7) {
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
    IMAGE image;

    for (k = 0; k < m[m_id].esize; k++) {
        if (e_n[k].see == 0&&e_n[k].live==1) {
            if (abs(e_n[k].x - p[0].x) + abs(e_n[k].y - p[0].y) <= 4) {
                /*mciSendString(L"close se", NULL, 0, NULL);
                mciSendString(L"open ./Game/Sound/SE/oylpu-opc5i.mp3 alias se", NULL, 0, NULL);
                vos = L"setaudio se volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play se from 0", NULL, 0, NULL);*/
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
        image =enemyimage[k];
        if (e_n[k].move == -1 && e_n[k].live == 1) {
            if (e_n[k].pose == 1) {
                if (e_n[k].y + 1 > m[m_id].y||ss[e_n[k].x][e_n[k].y+1]==1) {
                    transparentimage(NULL, e_n[k].x * 48-m[m_id].ox , e_n[k].y * 48 - m[m_id].oy - 16 , &image, 0xFF55FF, 64+8, 0, 48, 64);
                    e_n[k].type++;
                    if (e_n[k].type >= 3) {
                        e_n[k].type = 1;
                        e_n[k].move = 0;
                        e_n[k].see = 0;
                    }
                }
                else if (e_n[k].type == 1) {
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox , e_n[k].y * 48 + 16 * e_n[k].type - m[m_id].oy-16 , &image, 0xFF55FF, 64+8-48, 0, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 2) {
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox , e_n[k].y * 48 + 16 * e_n[k].type - m[m_id].oy - 16 , &image, 0xFF55FF, 64+8+48, 0, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 3) {
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 + 16 * e_n[k].type - m[m_id].oy - 16, &image, 0xFF55FF, 64+8, 0, 48, 64);
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
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &image, 0xFF55FF, 64+16, 64, 48, 64);
                    e_n[k].type++;
                    if (e_n[k].type >= 3) {
                        e_n[k].type = 1;
                        e_n[k].move = 0;
                        e_n[k].see = 0;
                    }
                }
                else if (e_n[k].type == 1) {
                    transparentimage(NULL, e_n[k].x * 48 - 16 * e_n[k].type - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16 , &image, 0xFF55FF, 64+16-48, 64, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 2) {
                    transparentimage(NULL, e_n[k].x * 48 - 16 * e_n[k].type - m[m_id].ox , e_n[k].y * 48 - m[m_id].oy - 16 , &image, 0xFF55FF, 64+16+48, 64, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 3) {
                    transparentimage(NULL, e_n[k].x * 48 - 16 * e_n[k].type - m[m_id].ox , e_n[k].y * 48 - m[m_id].oy - 16 , &image, 0xFF55FF, 64+16, 64, 48, 64);
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
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox , e_n[k].y * 48 - m[m_id].oy - 16 , &image, 0xFF55FF, 64, 128, 48, 64);
                    e_n[k].type++;
                    if (e_n[k].type >= 3) {
                        e_n[k].type = 1;
                        e_n[k].move = 0;
                        e_n[k].see = 0;
                    }
                }
                else if (e_n[k].type == 1) {
                    transparentimage(NULL, e_n[k].x * 48 + 16 * e_n[k].type - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16 , &image, 0xFF55FF, 64-48, 128, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 2) {
                    transparentimage(NULL, e_n[k].x * 48 + 16 * e_n[k].type - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16 , &image, 0xFF55FF, 64+48, 128, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 3) {
                    transparentimage(NULL, e_n[k].x * 48 + 16 * e_n[k].type - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16 , &image, 0xFF55FF, 64, 128, 48, 64);
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
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox , e_n[k].y * 48 - m[m_id].oy - 16 , &image, 0xFF55FF, 64+8, 192, 48, 64);
                    e_n[k].type++;
                    if (e_n[k].type >= 3) {
                        e_n[k].type = 1;
                        e_n[k].move = 0;
                        e_n[k].see = 0;
                    }
                }
                else if (e_n[k].type == 1) {
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox , e_n[k].y * 48 - 16 * e_n[k].type - m[m_id].oy - 16 , &image, 0xFF55FF,64+8-48, 192, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 2) {
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox , e_n[k].y * 48 - 16 * e_n[k].type - m[m_id].oy - 16 , &image, 0xFF55FF, 64+8+48, 192, 48, 64);
                    e_n[k].type++;
                }
                else if (e_n[k].type == 3) {
                    transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox , e_n[k].y * 48 - 16 * e_n[k].type - m[m_id].oy - 16 , &image, 0xFF55FF, 64+8, 192, 48, 64);
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
            if (e_n[k].pose == 1) {
                transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &image, 0xFF55FF, 64+8, 0, 48, 64);
            }
            else if (e_n[k].pose == 2) {
                transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &image, 0xFF55FF, 64+16, 64, 48, 64);

            }
            else if (e_n[k].pose == 3) {
                transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &image, 0xFF55FF, 64, 128, 48, 64);

            }
            else if (e_n[k].pose == 4) {
                transparentimage(NULL, e_n[k].x * 48 - m[m_id].ox, e_n[k].y * 48 - m[m_id].oy - 16, &image, 0xFF55FF, 64+8, 192, 48, 64);

            }
        }
    }
    if (FP % 10 == 0) {
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
void show(Map* m, player* p, enemy *e,e_npc *e_n,BOX *Box,int &m_id,int b_id, npc* n,flag *f,string g) {
    BeginBatchDraw();
    m_map(p,m, Box, m_id, b_id, n,g);
    if (g == "w") {
        m_walk(m, p,Box, m_id,b_id, n,g);
    }
    else { 
       m_put(p,m,m_id); 
    } 
    m_e_ai(e, p, m, e_n, m_id);
    if (f[21].check == 1) {
        IMAGE siy;
        loadimage(&siy, L"./Game/picture/siy.png", 0, 0, false);
        transparentimage(NULL, 0, 0, &siy);
    }
    FlushBatchDraw();
    /*       */
    tb = clock();
    if (tb - ta <35) {
        Sleep(35 - (tb - ta));
    }
    ta = clock();
    FP++;    

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
                te[i][j].npc = 0;
                te[i][j].enemy = 0;
                te[i][j].player = 0;
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
            if (root["layers"][k]["properties"][1]["value"].asBool() == true) {
                for (unsigned int w = 0; w < root["layers"][k]["data"].size(); w++) {
                    if (root["layers"][k]["data"][w].asInt64() != 0) {
                        te[w % b_m[b_mid].x][w / b_m[b_mid].x].dark = 1;
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
void battle_set(wofstream* wofs,enemy *e,enemy_type *e_t,player *p,b_map *b_m,b_flag *b_f,skill *sk,buff *bu,arms *ar,stone *st,b_npc *b_n,b_npc_type *b_nt,terrain (* te)[50], int& b_mid, int sk_id,int bu_id) {
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
                    box = "";
                    i--;
                    break;
                }
            }
        }
        else if (b_m[b_mid].e_set[i] == 'p') {
            i++;
            while (1) {
                if (b_m[b_mid].e_set[i] >= '0' && b_m[b_mid].e_set[i] <= '9') {
                    box += b_m[b_mid].e_set[i];
                    i++;
                }
                else {
                    e[k].pose = stoi(box);
                    box = "";
                    i--;
                    k++;
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
                    e[k].Move = e_t[id].Move; e[k].speed = e_t[id].speed; e[k].turn = 0; e[k].type = id; e[k].exp = e_t[id].exp;
                    e[k].s_range = e_t[id].s_range; e[k].e_range = e_t[id].e_range;
                    if (e[k].baid == 10) {
                        e[k].b_num_1 = 1;
                        e[k].b_num_2 = 1;
                    }
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
                    p[k].turn = 0;
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
        if (p[i].s_check[7] == 1) {
            if (p[i].stone_id == 0) {
                p[i].buff_check[17]=1;
                p[i].mhp += 2;
            }
            else if(p[i].stone_id == 1) {
                p[i].buff_check[18] = 1;
                p[i].speed += 2;
                p[i].dex++;
            }
            else if (p[i].stone_id == 2) {
                p[i].buff_check[10] += st[2].lv/2;
            }
        }
        if (p[i].s_check[11] == 1) {
            if (p[i].stone_id != -1) {
            p[i].buff_check[8] += st[p[i].stone_id].lv * 10;
            }
        }
        p_buff_check(wofs, "t", p, e, b_n,bu,ar, b_m,te, i, b_mid,bu_id);
    }
    b_m[b_mid].time_seed = time(NULL);
    srand(b_m[b_mid].time_seed);
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
void m_event_check(player* p, Map* m, m_flag* m_f, arms* ar, item* it,stone *st, BOX* Box,t_equip *t_E,task *tk, int m_id, int b_id, npc* n,int &sp_id) {
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
                        readmapeventjson(p, n, m_f, ar, it,m,t_E, st,tk,path, m_id, s,sp_id);
                    }
                }
            }
            else if (p[0].pose == 2) {
                if (m_f[s].check == 0) {
                    if (m_f[s].x == p[0].x - 1 && p[0].y == m_f[s].y) {
                        string filename;
                        filename = "./Game/story/m_event" + to_string(s) + string(".json");
                        const char* path = filename.c_str();
                        readmapeventjson(p, n, m_f, ar, it,m,t_E, st, tk, path, m_id, s, sp_id);
                    }
                }
            }
            else if (p[0].pose == 3) {
                if (m_f[s].check == 0) {
                    if (m_f[s].x == p[0].x + 1 && p[0].y == m_f[s].y) {
                        string filename;
                        filename = "./Game/story/m_event" + to_string(s) + string(".json");
                        const char* path = filename.c_str();
                        readmapeventjson(p, n, m_f, ar, it, m, t_E, st, tk, path, m_id, s, sp_id);
                    }
                }
            }
            else if (p[0].pose == 4) {
                if (m_f[s].check == 0) {
                    if (m_f[s].x == p[0].x && p[0].y - 1 == m_f[s].y) {
                        string filename;
                        filename = "./Game/story/m_event" + to_string(s) + string(".json");
                        const char* path = filename.c_str();
                        readmapeventjson(p, n, m_f, ar, it, m, t_E, st, tk, path, m_id, s, sp_id);
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
                            readmapeventjson(p, n, m_f, ar, it, m, t_E, st, tk, path, m_id, -1, sp_id);
                            Box[i].state = false;
                        }
                    }
                }
            }
        }
    }
}
void enemy_type_get(enemy *e,enemy_type *e_t, terrain(*te)[50],b_map *b_m,int b_mid,int id,int x,int y,int pose) {
    char b = 'A';
    wchar_t a;
    int k = b_m[b_mid].esize;
    e[k].x = x;
    e[k].y = y;
    e[k].baid = e_t[id].baid; e[k].str = e_t[id].str; e[k].dex = e_t[id].dex; e[k].con = e_t[id].con; e[k].INT = e_t[id].INT;
    e[k].wis = e_t[id].wis; e[k].cha = e_t[id].cha; e[k].lv = e_t[id].lv; e[k].mhp = e_t[id].mhp; e[k].hp = e_t[id].hp;
    e[k].Move = e_t[id].Move; e[k].speed = e_t[id].speed; e[k].turn = 0; e[k].type = id; e[k].exp = e_t[id].exp;
    e[k].s_range = e_t[id].s_range; e[k].e_range = e_t[id].e_range;
    e[k].pose = pose;
    if (e[k].baid == 10) {
        e[k].b_num_1 = 1;
        e[k].b_num_2 = 1;
    }
    b = b + k; mbtowc(&a, &b, 1); e[k].name = e_t[id].name + a;
    te[x][y].mA += 10000;
    te[x][y].mB+= 10000;
    te[x][y].enemy = 1;
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
    b_m[b_mid].esize++;
}
void b_event(flag *f,b_flag *b_f,player *p,npc *n,b_map *b_m,enemy_type *e_t,enemy *e,Map *m,BOX *Box,b_npc *b_n,task *tk,m_flag *m_f,item *it,stone *st,arms *ar, terrain(*te)[50],int b_mid,int P_id,int psize,int bu_id,int b_id,int b_nid,int buff_id) {
    int m_id,I;
    for (int fk = 0; fk < b_m[b_mid].fsize; fk++) {
        if (b_f[fk].check == 1) {
            if (b_f[fk].id == 0) {
                string filename;
                filename = "./Game/story/b_event" + to_string(0) + string(".json");
                const char* path = filename.c_str();
                readeventjson(p, n, f, m, Box, tk, m_f, it, st, path, m_id, b_id);
                b_f[fk].check = 0;
            }
            if (b_f[fk].id == 1) {
                if (b_m[b_mid].time >= 12) {
                    string filename;
                    filename = "./Game/story/b_event" + to_string(1) + string(".json");
                    const char* path = filename.c_str();
                    b_camera(b_m, 49, 44, b_mid);
                    enemy_type_get(e, e_t, te,b_m,b_mid, 1, 49, 44,2);
                    BeginBatchDraw();
                    maps(p, P_id, e, b_m, ar, te,b_mid);/*地圖繪製*/
                    e_put(e, b_m, te, b_m[b_mid].esize, b_mid);
                    b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                    p_put(p, b_m, psize, b_mid);
                    ui(p, e, b_m, P_id, b_m[b_mid].esize, psize, b_mid, bu_id);
                    EndBatchDraw();
                    readeventjson(p, n, f, m, Box, tk, m_f, it, st, path, m_id, b_id);
                    b_f[fk].check = 0;
                }
            }
            if (b_f[fk].id == 2) {
                if (P_id == 0) {
                    string filename;
                    filename = "./Game/story/b_event" + to_string(2) + string(".json");
                    const char* path = filename.c_str();
                    readeventjson(p, n, f, m, Box, tk, m_f, it, st, path, m_id, b_id);
                    b_f[fk].check = 0;
                    b_n[0].hp = 15;
                }
            }
            if (b_f[fk].id == 3) {
                if (b_m[b_mid].time <9) {
                    b_n[0].buff_check[9] = 1;
                    b_n[0].buff_check[10] = 5;
                    b_n[0].buff_check[8] = 200;
                    b_n[0].buff_time[0] = 300;             
                    b_n[0].buff_id[0] = 10;
                    b_n[0].buff_Size = 1;
                    e[2].buff_check[11] = 1;
                }
                else {
                string filename;
                filename = "./Game/story/b_event" + to_string(3) + string(".json");
                const char* path = filename.c_str();
                readeventjson(p, n, f, m, Box, tk, m_f, it, st, path, m_id, b_id);     
                BeginBatchDraw();
                maps(p, P_id, e, b_m, ar, te,b_mid);/*地圖繪製*/
                e_put(e, b_m, te, b_m[b_mid].esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, b_m[b_mid].psize, b_mid);
                ui(p, e, b_m, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id);
                FlushBatchDraw();
                b_f[fk].check = 0;
                }
            }
            if (b_f[fk].id == 4) {
                for (I = 0; I < b_m[b_mid].esize; I++) {
                    if (I != 2) {
                        if (e[I].hp > 0) {
                            break;
                        }
                    }
                }
                if (I == b_m[b_mid].esize||e[2].hp<e[2].mhp) {
                string filename;
                filename = "./Game/story/b_event" + to_string(4) + string(".json");
                const char* path = filename.c_str();
                readeventjson(p, n, f, m, Box, tk, m_f, it, st, path, m_id, b_id);
                    b_f[fk].check = 0;
                }
            }
            if (b_f[fk].id == 5) {
                b_f[fk].check = 0;
            }
            if (b_f[fk].id == 6) {
                for (int I = 0; I < b_m[b_mid].esize; I++) {
                    e[I].buff_check[32] = 1;
                }
                b_f[fk].check = 0;
            }
        }
    }

}
void END(wofstream *wofs,player* p, enemy* e,b_npc *b_n, string chose, b_map* b_m, b_flag* b_f,flag *f, npc* n, enemy_type* e_t,buff *bu,arms *ar,armor *Ar,Map *m,BOX *Box,task *tk,m_flag *m_f,item *it,stone *st, terrain(*te)[50], int b_mid, int& P_id, int& id, int& roundp, int& roundb,int& roundn,int psize,int bu_id,int b_id,int b_nid,int buff_id,int sk_id) {
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
                if (p[i].turn >= 100 && p[i].buff_check[4] == 0 && p[i].buff_check[5] == 0 && p[i].buff_check[14] == 0 && p[i].buff_check[26] == 0) {
                    if (p[pmax].turn < p[i].turn || pmax == i) {
                        pmax = i;
                    }
                }
            }
            for (i = 0; i < b_m[b_mid].esize; i++&&e[i].buff_check[13]==0 && e[i].buff_check[15] == 0) {
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
                if (e[i].buff_check[13] == 0&& e[i].buff_check[15] == 0) {
                id = bmax; e[bmax].turn -= 100;  roundp = 0; roundb = 1; roundn = 0; e[id].move = e[id].Move; e[id].ap = 1;
                e_buff_check(wofs, "r",Ar, p, e, b_n, bu, b_m, te, -1, b_mid, bu_id, id);
                te_check(wofs,"e", p, e, b_n, b_m, te, b_mid, -1,id, -1);
                break;
                }
            }
            else if (p[pmax].turn >= e[bmax].turn && p[pmax].turn >= 100) {
                if (p[pmax].buff_check[4] == 0 && p[pmax].buff_check[5] == 0 && p[pmax].buff_check[14] == 0) {
                P_id = pmax;
                p[pmax].turn -= 100;
                p[P_id].move = p[P_id].Move;
                p[P_id].act = p[P_id].Act;
                roundp = 1; roundb = 0; roundn=0;
                if (p[P_id].s_check[16] == 1) {
                    int los=0;
                    for (i = 0; i < b_m[b_mid].esize; i++) {
                        if (abs(p[P_id].x - e[i].x) + abs(p[P_id].y - e[i].y) <= 5&&los==0) {
                            p[P_id].move += 1;
                            los = 1;
                        }
                        if (abs(p[P_id].x - e[i].x) + abs(p[P_id].y - e[i].y) <= 2) {
                            p[P_id].move += 1;
                            break;
                        }
                    }
                }
                p_buff_check(wofs,"r", p, e, b_n, bu,ar, b_m,te, P_id, b_mid, bu_id);
                te_check(wofs,"p", p, e, b_n, b_m, te, b_mid, P_id, -1, -1);
                cd_check("p", p, e, b_n, b_m, b_mid, sk_id, P_id);
                break;
                }                
            }
            else if(b_n[nmax].turn>=100) {
                if (b_n[nmax].buff_check[4] == 0 && b_n[nmax].buff_check[5] == 0) {
                b_nid = nmax;
                b_n[nmax].turn -= 100;
                b_n[b_nid].move = b_n[b_nid].Move;
                roundp = 0; roundb = 0; roundn = 1;
                n_buff_check(wofs,"r", p, e, b_n, bu, b_m, te,P_id, b_mid, bu_id,b_nid);
                te_check(wofs,"n", p, e, b_n, b_m, te, b_mid, -1, -1, b_nid);
                break;
                }                
            }
            for (i = 0; i < b_m[b_mid].psize; i++) {
                if (p[i].speed>0&&p[i].buff_check[4] == 0 && p[i].buff_check[5] ==0 && p[i].buff_check[14] == 0 && p[i].buff_check[26] == 0) {
                     p[i].turn += p[i].speed;
                }                
            }
            for (i = 0; i < b_m[b_mid].esize; i++) {
                if (e[i].speed > 0&&e[i].buff_check[5]==0&& e[i].buff_check[13] == 0 && e[i].buff_check[15] == 0) {
                    e[i].turn += e[i].speed;
                }
            }
            for (i = 0; i < b_m[b_mid].nsize; i++) {
                if (b_n[i].speed>0&&b_n[i].buff_check[4]==0&&b_n[i].buff_check[5]==0) {
                    b_n[i].turn += b_n[i].speed;
                }
               
            }
            b_m[b_mid].time++;
            p_buff_check(wofs,"t", p, e, b_n, bu,ar, b_m,te, P_id, b_mid,bu_id);
            p_buff_check(wofs,"T", p, e, b_n, bu, ar,b_m,te, P_id, b_mid,bu_id);
            n_buff_check(wofs, "T", p, e, b_n, bu, b_m, te,P_id, b_mid, bu_id,b_nid);
            e_buff_check(wofs,"T",Ar, p, e, b_n, bu, b_m, te, P_id, b_mid, bu_id,-1);
            e_buff_check(wofs, "s", Ar, p, e, b_n, bu, b_m, te, P_id, b_mid, bu_id, -1);
            cd_check("t", p, e, b_n, b_m, b_mid,sk_id,-1);
            te_check(wofs,"t", p, e, b_n, b_m, te, b_mid, -1, -1, -1);
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
            b_event(f,b_f, p, n, b_m, e_t, e,m,Box,b_n, tk, m_f, it, st, ar, te,b_mid, P_id, psize,bu_id,b_id,b_nid,buff_id);
        }
    }
}
void buff_skill(player *p,enemy *e,skill *sk,int sk_id,int P_id) {
    
}
void p_skill(wofstream* wofs, string &chose,player *p,enemy *e,b_npc *b_n,skill *sk,arms *ar, t_equip* t_E,item *it,b_map *b_m, terrain(*te)[50], int sk_id,int P_id,int esize,int b_mid,int psize,int bu_id,int b_nid) {
    wstring vos;
    if (chose == "k") {
        int a[100];
    BeginBatchDraw();
    IMAGE m1,ae,get,hi,ab,ef,mb;
    RECT  is = { 960,0,960 + 247,148 };
    wstring mm;
    ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);    
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    j = 0;
    loadimage(&m1, L"./Game/picture/s_block.png", 0, 0, false);
    putimage(0, 0, &m1);
        for (i = 0; i < sk_id; i++) {
            if (p[P_id].s_check[i]==1&&sk[i].type[0] != 'b'&&sk[i].type[0]!='P' && sk[i].type[0] != 'S') {
                IMAGE s1;
                wstring ww;
                ww = L"./Game/picture/skill" + to_wstring(i) + L".png";
                loadimage(&s1,ww.c_str(), 0, 0, false);
                putimage(0, j * 50,&s1);
                mm = sk[i].name;
                if (p[P_id].r_cd[i] > 0) {
                    mm += L"(CD中:" + to_wstring(p[P_id].r_cd[i]) + L"回合)";
                }
                else if (p[P_id].s_cd[i] > 0) {
                    mm += L"(CD中:" + to_wstring(p[P_id].s_cd[i]) + L"秒)";
                }
                outtextxy(31, j * 50, mm.c_str());
                a[j] = i;
                j++;
            }
        }
        FlushBatchDraw();
        int uk = 1;
        getimage(&get, 0, 0, 1296, 960);
        settextstyle(18, 0, _T("Taipei Sans TC Beta"));
        while (uk) {
            ExMessage em;
            Sleep(10);
            if (peekmessage(&em, EM_MOUSE)) {
                flushmessage(-1);
                for (i = 0; i < j; i++) {
                    if (em.x >= 0 && em.x <= 400 && em.y >= i * 50 && em.y <= i * 50 + 50) {
                        putimage(0,0,&get);
                        POINT pts[] = { {0 ,i*50}, {400, i*50 }, {400,i*50+50 },{0,i*50+50 }  };
                        polygon(pts, 4);
                        drawtext(sk[a[i]].story.c_str(), &is, DT_WORDBREAK);
                        FlushBatchDraw();
                        if (em.lbutton) {
                            uk = 0;
                            break;
                        }                        
                    }
                }
                if (em.rbutton) {
                    return;
                }
            }
        }
        uk = a[i];
        if (uk == 1) {
            if (p[P_id].move>=3) {
                mciSendString(L"close se", NULL, 0, NULL);
                mciSendString(L"open ./Game/Sound/SE/maou_se_magical14.mp3 alias se", NULL, 0, NULL);
                vos = L"setaudio se volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play se from 0", NULL, 0, NULL);
                while (p[P_id].move >= 3) {
                    p[P_id].move -= 3;
                    p[P_id].buff_check[1]++;
                }
                maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                e_put(e, b_m, te, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                FlushBatchDraw();
                getimage(&get, 0, 0, 1296, 960);
                loadimage(&ae, L"./Game/picture/上昇.png",0,0,false);
                    for (j = 0; j < 3; j++) {
                        for (i = 0; i < 5; i++) {
                            putimage(0, 0, &get);
                            transparentimageA(NULL, p[P_id].x * 48 - b_m[b_mid].ox-12, p[P_id].y * 48 - b_m[b_mid].oy-18, &ae, i * 80, j * 80,80,80);
                            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                            FlushBatchDraw();
                            Sleep((j*5+i*3)*2);
                        }
                    }
                    *wofs <<L"(T"<<b_m[b_mid].time <<L")" << p[P_id].name << L"使用了" << sk[uk].name << endl;
            }
            else {
                loadimage(&hi, L"./Game/picture/b_hint.png", 0, 0, false);
                putimage(400, 300, &hi);
                RECT t = { 400,300,650,400 };
                mm = L"MP不足";
                drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                FlushBatchDraw();
                Sleep(1000);
            }
        }
        else if (uk == 2) {
            if (p[P_id].act > 0) {
                if (p[P_id].arms_id_1 == 8 || p[P_id].arms_id_2 == 8) {
                    loadimage(&ab, L"./Game/picture/ab.png", 0, 0, false);               
                    b_camera(b_m, p[P_id].x, p[P_id].y, b_mid);
                    maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                    for (i = -1; i <= 1; i++) {
                        for (j = -1; j <= 1; j++) {
                            if ((i != 0 || j != 0)&&te[p[P_id].x+i][p[P_id].y+j].type==0 && te[p[P_id].x + i][p[P_id].y + j].player==0 && te[p[P_id].x + i][p[P_id].y + j].npc == 0) {
                            transparentimage(NULL, 48 * (p[P_id].x+i) - b_m[b_mid].ox, 48 * (p[P_id].y+j)- b_m[b_mid].oy, &ab);
                            }
                        }
                    }
                    e_put(e, b_m, te, esize, b_mid);
                    b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                    p_put(p, b_m, psize, b_mid);
                    ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                    FlushBatchDraw();
                    ExMessage em;
                    while (1) {
                        flushmessage(-1);
                       em = getmessage(EM_MOUSE);
                        for (i = -1; i <= 1; i++) {
                            for (j = -1; j <= 1; j++) {
                                if (i != 0 || j != 0) {
                                    if (em.x > (p[P_id].x + i) * 48-b_m[b_mid].ox && em.x < (p[P_id].x + i) * 48 + 48 - b_m[b_mid].ox && em.y >(p[P_id].y + j) * 48 - b_m[b_mid].oy && em.y< (p[P_id].y + j) * 48 + 48 - b_m[b_mid].oy&&em.x<960&&em.y<720) {
                                        for (int I = 0; I < b_m[b_mid].esize; I++) {
                                            if (e[I].x == p[P_id].x + i && e[I].y == p[P_id].y + j&&em.lbutton) {
                                                int ATK = roll("", 2)+(p[P_id].str-10)/2+(p[P_id].dex-10)/2;
                                                int AC = roll("", 2)+(e[I].dex-10)/2;
                                                if (e[I].buff_check[27] == 1) {
                                                    AC /= 2;
                                                }
                                                if (e[I].buff_check[20] == 1) {
                                                    AC =0;
                                                }
                                                int dmg;
                                                if (ATK > AC) {
                                                     dmg = roll("1d4", 1) + (p[P_id].str + p[P_id].dex) / 5;
                                                     eHP(e, b_m, I, b_mid, 0, dmg);
                                                    e[I].buff_check[3]++;
                                                    e[I].buff_time[e[I].buff_Size] = 15;
                                                    e[I].buff_id[e[I].buff_Size] = 3;
                                                    e[I].buff_Size++;
                                                    *wofs <<L"(T"<<b_m[b_mid].time <<L")" << p[P_id].name << L"使用了" << sk[uk].name <<L"對"<<e[I].name << L"造成"<<dmg<<L"點傷害("<<ATK<<L">" <<AC<<L")" << endl;
                                                }
                                                else {
                                                    mm = L"miss";
                                                    settextcolor(RGB(255, 0, 0));
                                                    settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                                                    outtextxy(e[I].x * 48 - b_m[b_mid].ox, e[I].y * 48 - b_m[b_mid].oy - 40, mm.c_str());
                                                    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                                                    settextcolor(RGB(0, 0, 0));
                                                    *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"使用了" << sk[uk].name << L"未能命中" << e[I].name << L"(" << ATK << L"<=" << AC << L")"<<endl;
                                                }
                                                p[P_id].act--;
                                                FlushBatchDraw();
                                                getimage(&get, 0, 0, 1296, 960);
                                                loadimage(&ef, L"./Game/picture/杖1.png", 0, 0, false);
                                                mciSendString(L"close se", NULL, 0, NULL);
                                                mciSendString(L"open ./Game/Sound/SE/軽いパンチ2.mp3 alias se", NULL, 0, NULL);
                                                vos = L"setaudio se volume to " + to_wstring(soundSize);
                                                mciSendString(vos.c_str(), NULL, 0, NULL);
                                                mciSendString(L"play se from 0", NULL, 0, NULL);
                                                for (int ej = 0; ej < 4; ej++) {
                                                for (int ei = 0; ei < 5; ei++) {
                                                    putimage(0, 0, &get);
                                                    transparentimageA(NULL, 48 * e[I].x - b_m[b_mid].ox - 10, e[I].y * 48 - b_m[b_mid].oy - 30, &ef, ei * 80, ej * 80, 80, 80);
                                                    FlushBatchDraw();
                                                    Sleep(40);
                                                }
                                                }

                                                return;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if (em.rbutton) {
                            return;
                        }
                    }
                }
                else {
                    loadimage(&hi, L"./Game/picture/b_hint.png", 0, 0, false);
                    putimage(400, 300, &hi);
                    RECT t = { 400,300,650,400 };
                    mm = L"沒有攜帶杖類武器";
                    drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    FlushBatchDraw();
                    Sleep(1000);
                }
            }
            else {
                loadimage(&hi, L"./Game/picture/b_hint.png", 0, 0, false);
                putimage(400, 300, &hi);
                RECT t = { 400,300,650,400 };
                mm = L"AP不足";
                drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                FlushBatchDraw();
                Sleep(1000);
            }
        }
        else if (uk == 3) {
            settextstyle(18, 0, _T("Taipei Sans TC Beta"));
            IMAGE backa, armb,ar1,hi,p1;
            vector<IMAGE> itemImages;
            ExMessage im;
            RECT t;
            loadimage(&backa, L"./Game/picture/attackblock.png", 0, 0, false);
            loadimage(&armb, L"./Game/picture/armsblock.png", 0, 0, false);
            loadimage(&hi, L"./Game/picture/b_hint.png", 0, 0, false);
            loadimage(&p1, L"./Game/picture/tbox.png", 0, 0, false);
            int n = 0,k,arms_id;
            BeginBatchDraw();
            if (p[P_id].arms_id_1 != -1) {
                putimage(175, 799 + 48 * n, &armb);
                mm = ar[p[P_id].arms_id_1].name;
                if (ar[p[P_id].arms_id_1].type[0] == 'r') {
                    mm += L"(" + std::to_wstring(p[P_id].arms_b_1) + L"/" + to_wstring(ar[p[P_id].arms_id_1].mbullet) + L")";
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
                    mm += L"(" + std::to_wstring(p[P_id].arms_b_2) + L"/" + to_wstring(ar[p[P_id].arms_id_2].mbullet) + L")";
                }
                outtextxy(210, 815 + 48 * n, mm.c_str());
                mm = L"./Game/picture/arms" + to_wstring(p[P_id].arms_id_2) + L".png";
                loadimage(&ar1, mm.c_str(), 0, 0, false);
                transparentimage(NULL, 180, 815 + 48 * n, &ar1, 0xFF55FF);
            }
            EndBatchDraw();
            while (1) {
                im = getmessage(EM_MOUSE);
                if (im.lbutton) {
                    for (k = 0; k <= n; k++) {
                        if (im.x >= 175 && im.x <= im.x <= 325 && im.y >= 799 + 48 * k && im.y <= 847 + 48 * k) {
                            if (k == 0) {
                                arms_id = p[P_id].arms_id_1;
                            }
                            else if (k == 1) {
                                arms_id = p[P_id].arms_id_2;
                            } 
                            if (ar[arms_id].type[0] == 'r') { 
                                    
                                    if (ar[arms_id].type[1] == 'r') {
                                        for (j = 0; j < t_E[P_id].size; j++) {
                                            if (p[P_id].box_id[j] == 0) {
                                                j = t_E[P_id].size ;
                                            }
                                        }
                                    }
                                    else if (ar[arms_id].type[1] == 'p') {
                                        for (j = 0; j < t_E[P_id].size; j++) {
                                            if (p[P_id].box_id[j] == 3) {
                                                j = t_E[P_id].size+1;
                                            }
                                        }
                                    }
                                    if (j == t_E[P_id].size) {
                                        putimage(400, 300, &hi);
                                        t = { 400,300,650,400 };
                                        mm =L"沒有能裝填"+ ar[arms_id].name+L"的彈藥";
                                        drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                                        Sleep(2000);
                                        return;
                                    }
                                    BeginBatchDraw();
                                    for (i = 0; i < t_E[P_id].size; i++) {
                                         if (p[P_id].box_id[i] != -1 && p[P_id].box_size != 0) {
                                                mm = L"./Game/picture/item" + to_wstring(p[P_id].box_id[i]) + L".png";
                                                IMAGE p2;
                                                loadimage(&p2, mm.c_str(), 0, 0, false);
                                                itemImages.push_back(p2);
                                         }
                                    }
                                    for (i = 0; i < t_E[p[P_id].t_id].size; i++) {                                       
                                        putimage(400 + i * 30, 720, &p1);
                                        if (p[P_id].box_id[i] != -1 && p[P_id].box_size[i] != 0) {
                                            transparentimage(NULL, 405 + i * 30, 725, &itemImages[i], 0xFF55FF);
                                            outtextxy(405 + i * 30, 700, to_wstring(p[P_id].box_size[i]).c_str());
                                        }
                                    }
                                    EndBatchDraw();
                                    while (1) {
                                        im = getmessage(EM_MOUSE);
                                        for (i = 0; i < t_E[p[P_id].t_id].size; i++) {
                                            if (im.x >= 400 + i * 30 && im.x <= 430 + i * 30 && im.y >= 720 && im.y <= 750) {
                                                if (im.lbutton) {
                                                    if (j == t_E[P_id].size + 1) {
                                                        if (p[P_id].box_id[i] == 0) {
                                                            if (p[P_id].move > 2) {
                                                                p[P_id].move-=3;
                                                            p[P_id].buff_time[p[P_id].buff_Size] = 5*70/100;
                                                            p[P_id].buff_id[p[P_id].buff_Size] = 5;
                                                            p[P_id].buff_check[5] += 1;
                                                            if (k == 0) {   
                                                                p[P_id].buff_give[p[P_id].buff_Size] = "iw0a";
                                                            }
                                                            else if (k == 1) {
                                                                p[P_id].buff_give[p[P_id].buff_Size] = "iw0A";
                                                            }                                                         
                                                            p[P_id].buff_Size++;
                                                            p[P_id].box_size[i]--;
                                                            chose = "e";
                                                            return;
                                                            }
                                                            else {
                                                                putimage(400, 300, &hi);
                                                                t = { 400,300,650,400 };
                                                                mm = L"裝填"+ar[arms_id].name + L"需要3MP";
                                                                drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                                                                Sleep(2000);
                                                                return;
                                                            }
                                                        }
                                                        else {
                                                            putimage(400, 300, &hi);
                                                            t = { 400,300,650,400 };
                                                            mm = it[p[P_id].box_id[i]].Name + L"並非所需彈藥";
                                                            drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                                                            Sleep(2000);
                                                            return;
                                                        }
                                                    }
                                                    else if (j == t_E[P_id].size + 2) {
                                                        if (p[P_id].box_id[i] == 3) {
                                                            if (p[P_id].move > 0) {
                                                                p[P_id].move--;
                                                            p[P_id].buff_time[p[P_id].buff_Size] = 3*70/100;
                                                            p[P_id].buff_id[p[P_id].buff_Size] = 5;
                                                            p[P_id].buff_check[5] += 1;
                                                            if (k == 0) {
                                                                p[P_id].buff_give[p[P_id].buff_Size] = "iw3a";
                                                            }
                                                            else if (k == 1) {
                                                                p[P_id].buff_give[p[P_id].buff_Size] = "iw3A";
                                                            }
                                                            p[P_id].buff_Size++;
                                                            p[P_id].box_size[i]--;
                                                            chose = "e";
                                                            return;
                                                            }
                                                            else {
                                                                putimage(400, 300, &hi);
                                                                t = { 400,300,650,400 };
                                                                mm = L"裝填" + ar[arms_id].name + L"需要1MP";
                                                                drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                                                                Sleep(2000);
                                                                return;
                                                            }
                                                       }
                                                        else {
                                                            putimage(400, 300, &hi);
                                                            t = { 400,300,650,400 };
                                                            mm = it[p[P_id].box_id[i]].Name + L"並非所需彈藥";
                                                            drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                                                            Sleep(2000);
                                                            return;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        if (im.rbutton) {
                                            break;
                                        }
                                    }
                                }
                                else {
                                    putimage(400, 300, &hi);
                                    t = { 400,300,650,400 };
                                    mm = ar[arms_id].name + L"並非槍械";
                                    drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                                    Sleep(2000);
                                    return;
                             }
                        }
                    }
                }
                if (im.rbutton) {
                    break;
                }
            }
        }
        else if (uk == 5) {
        if (p[P_id].act > 0&&p[P_id].move>0) {
            loadimage(&ab, L"./Game/picture/ib.png", 0, 0, false);
            b_camera(b_m, p[P_id].x, p[P_id].y, b_mid);
            maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
            for (i = -1; i <= 1; i++) {
                for (j = -1; j <= 1; j++) {
                    if (abs(i) + abs(j) < 2 && (i != 0 || j != 0)&&te[p[P_id].x+i][p[P_id].y+j].enemy==0 && te[p[P_id].x + i][p[P_id].y + j].type==0) {
                        transparentimage(NULL, 48 * (p[P_id].x + i) - b_m[b_mid].ox, 48 * (p[P_id].y + j) - b_m[b_mid].oy, &ab);
                    }
                }
            }
            e_put(e, b_m, te, esize, b_mid);
            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
            p_put(p, b_m, psize, b_mid);
            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
            FlushBatchDraw();
            ExMessage em;
            while (1) {
                flushmessage(-1);
                em = getmessage(EM_MOUSE);
                if (em.lbutton) {
                    for (i = -1; i <= 1; i++) {
                        for (j = -1; j <= 1; j++) {
                            if (abs(i) + abs(j) < 2 && (i != 0 || j != 0) && te[p[P_id].x + i][p[P_id].y + j].enemy == 0 && te[p[P_id].x + i][p[P_id].y + j].type == 0) {
                                if (em.x > 48 * (p[P_id].x + i)-b_m[b_mid].ox && em.x < 48 * (p[P_id].x + i) + 48 - b_m[b_mid].ox && em.y>48 * (p[P_id].y + j) - b_m[b_mid].oy&& em.y < 48 * (p[P_id].y + j) + 48 - b_m[b_mid].oy&&em.x<960&&em.y<720) {
                                    for (int I = 0; I < b_m[b_mid].psize;I++) {
                                        if (p[I].x == p[P_id].x + i && p[I].y == p[P_id].y + j) {
                                            te[p[I].x][p[I].y].player = 0;
                                            te[p[I].x][p[I].y].mB -= 10000;
                                            te[p[I].x][p[I].y].mA -= 10000;
                                            te[p[I].x+1][p[I].y].mB -= 1;
                                            te[p[I].x - 1][p[I].y].mB -= 1;
                                            te[p[I].x ][p[I].y-1].mB -= 1;
                                            te[p[I].x + 1][p[I].y+1].mB -= 1;                                            
                                            p[I].x = -1; p[I].y = -1;
                                            p_walk(wofs, p, e, b_n, b_m, ar, te, "w", P_id, -1, b_m[b_mid].psize, b_m[b_mid].esize, b_mid, bu_id,-1);
                                            for (i = -1; i <= 1; i++) {
                                                for (j = -1; j <= 1; j++) {
                                                    if (abs(i) + abs(j) < 2 && (i != 0 || j != 0) && te[p[P_id].x + i][p[P_id].y + j].enemy == 0 && te[p[P_id].x + i][p[P_id].y + j].type == 0) {
                                                        transparentimage(NULL, 48 * (p[P_id].x + i) - b_m[b_mid].ox, 48 * (p[P_id].y + j) - b_m[b_mid].oy, &ab);
                                                    }
                                                }
                                            }
                                            FlushBatchDraw();
                                            while (1) {
                                                flushmessage(-1);
                                                em = getmessage(EM_MOUSE);
                                                if (em.lbutton) {
                                                    for (i = -1; i <= 1; i++) {
                                                        for (j = -1; j <= 1; j++) {
                                                            if (abs(i) + abs(j) < 2 && (i != 0 || j != 0) && te[p[P_id].x + i][p[P_id].y + j].enemy == 0 && te[p[P_id].x + i][p[P_id].y + j].type == 0) {
                                                                if (em.x > 48 * (p[P_id].x + i) - b_m[b_mid].ox && em.x < 48 * (p[P_id].x + i) + 48 - b_m[b_mid].ox && em.y>48 * (p[P_id].y + j) - b_m[b_mid].oy && em.y < 48 * (p[P_id].y + j) + 48 - b_m[b_mid].oy) {
                                                                 p[I].x = p[P_id].x + i;
                                                                p[I].y = p[P_id].y + j;
                                                                te[p[I].x][p[I].y].player = 1;
                                                                te[p[I].x][p[I].y].mB += 10000;
                                                                te[p[I].x][p[I].y].mA += 10000;
                                                                te[p[I].x + 1][p[I].y].mB += 1;
                                                                te[p[I].x - 1][p[I].y].mB += 1;
                                                                te[p[I].x][p[I].y - 1].mB += 1;
                                                                te[p[I].x + 1][p[I].y + 1].mB += 1;
                                                                p[P_id].act--;
                                                                return;                                                               
                                                                }

                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    for (int I = 0; I < b_m[b_mid].nsize; I++) {
                                        if (b_n[I].x == p[P_id].x + i && b_n[I].y == p[P_id].y + j) {
                                            te[b_n[I].x][b_n[I].y].player = 0;
                                            te[b_n[I].x][b_n[I].y].mB -= 10000;
                                            te[b_n[I].x][b_n[I].y].mA -= 10000;
                                            te[b_n[I].x + 1][b_n[I].y].mB -= 1;
                                            te[b_n[I].x - 1][b_n[I].y].mB -= 1;
                                            te[b_n[I].x][b_n[I].y - 1].mB -= 1;
                                            te[b_n[I].x + 1][b_n[I].y + 1].mB -= 1;
                                            b_n[I].x = -1; b_n[I].y = -1;
                                            p_walk(wofs, p, e, b_n, b_m, ar, te, "w", P_id, -1, b_m[b_mid].psize, b_m[b_mid].esize, b_mid, bu_id, -1);
                                            for (i = -1; i <= 1; i++) {
                                                for (j = -1; j <= 1; j++) {
                                                    if (abs(i) + abs(j) < 2 && (i != 0 || j != 0) && te[p[P_id].x + i][p[P_id].y + j].enemy == 0 && te[p[P_id].x + i][p[P_id].y + j].type == 0) {
                                                        transparentimage(NULL, 48 * (p[P_id].x + i) - b_m[b_mid].ox, 48 * (p[P_id].y + j) - b_m[b_mid].oy, &ab);
                                                    }
                                                }
                                            }
                                            FlushBatchDraw();
                                            while (1) {
                                                flushmessage(-1);
                                                em = getmessage(EM_MOUSE);
                                                if (em.lbutton) {
                                                    for (i = -1; i <= 1; i++) {
                                                        for (j = -1; j <= 1; j++) {
                                                            if (abs(i) + abs(j) < 2 && (i != 0 || j != 0) && te[p[P_id].x + i][p[P_id].y + j].enemy == 0 && te[p[P_id].x + i][p[P_id].y + j].type == 0) {
                                                                if (em.x > 48 * (p[P_id].x + i) - b_m[b_mid].ox && em.x < 48 * (p[P_id].x + i) + 48 - b_m[b_mid].ox && em.y>48 * (p[P_id].y + j) - b_m[b_mid].oy&& em.y < 48 * (p[P_id].y + j) + 48 - b_m[b_mid].oy) {
                                                                    b_n[I].x = p[P_id].x + i;
                                                                    b_n[I].y = p[P_id].y + j;
                                                                    te[b_n[I].x][b_n[I].y].player = 1;
                                                                    te[b_n[I].x][b_n[I].y].mB += 10000;
                                                                    te[b_n[I].x][b_n[I].y].mA += 10000;
                                                                    te[b_n[I].x + 1][b_n[I].y].mB += 1;
                                                                    te[b_n[I].x - 1][b_n[I].y].mB += 1;
                                                                    te[b_n[I].x][b_n[I].y - 1].mB += 1;
                                                                    te[b_n[I].x + 1][b_n[I].y + 1].mB += 1;
                                                                    p[P_id].act--;
                                                                    return;
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
                    }
                }
                if (em.rbutton) {
                    return;
                }
            }
         }
            else {
                     loadimage(&hi, L"./Game/picture/b_hint.png", 0, 0, false);
                     putimage(400, 300, &hi);
                     RECT t = { 400,300,650,400 };
                     mm = L"AP不足或MP不足";
                     drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                     FlushBatchDraw();
                     Sleep(1000);
            }
        }
        else if (uk == 6) {
          if (p[P_id].act > 0) {
              loadimage(&ab, L"./Game/picture/ib.png", 0, 0, false);
              b_camera(b_m, p[P_id].x, p[P_id].y, b_mid);
              maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
              for (int sx = -5; sx <= 5; sx++) {
                  for (int sy = -5; sy <= 5; sy++) {
                      if (abs(sx) + abs(sy) < 6&&te[p[P_id].x+sx][p[P_id].y+sy].enemy==0&& te[p[P_id].x + sx][p[P_id].y + sy].type == 0) {
                           transparentimage(NULL, 48 * (p[P_id].x + sx) - b_m[b_mid].ox, 48 * (p[P_id].y + sy) - b_m[b_mid].oy, &ab);
                      }
                  }
              }
              e_put(e, b_m, te, esize, b_mid);
              b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
              p_put(p, b_m, psize, b_mid);
              ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
              FlushBatchDraw();
              while (1) {
                  ExMessage em;
                  flushmessage(-1);
                  em = getmessage(EM_MOUSE);
                  if (em.lbutton) {
                      for (int sx = -5; sx <= 5; sx++) {
                          for (int sy = -5; sy <= 5; sy++) {
                              if (abs(sx) + abs(sy) < 6 && te[p[P_id].x + sx][p[P_id].y + sy].enemy == 0 && te[p[P_id].x + sx][p[P_id].y + sy].type == 0) {
                                  if (em.x > (p[P_id].x + sx) * 48-b_m[b_mid].ox && em.x< (p[P_id].x + sx) * 48 + 48 - b_m[b_mid].ox && em.y>(p[P_id].y + sy) * 48 - b_m[b_mid].oy&& em.y <(p[P_id].y + sy) * 48 + 48 - b_m[b_mid].oy&&em.x<960&&em.y<720) {
                                      for (i = 0; i < b_m[b_mid].psize;i++) {
                                          if (p[i].x==p[P_id].x+sx&&p[i].y==p[P_id].y+sy) {
                                              p[i].buff_check[16] = 5+p[i].wis/2;
                                              p[i].buff_check[7] = p[i].wis/7;
                                              p[i].buff_time[p[i].buff_Size] = 60;
                                              p[i].buff_id[p[i].buff_Size] = 7;
                                              p[i].buff_Size++;
                                              p[P_id].act--;
                                              return;
                                          }
                                      }
                                      for (i = 0; i < b_m[b_mid].nsize;i++) {
                                          if (b_n[i].x == p[P_id].x + sx && b_n[i].y == p[P_id].y + sy) {
                                              b_n[i].buff_check[16] = 5 + p[i].wis / 2;
                                              b_n[i].buff_check[7] = p[i].wis / 7;
                                              b_n[i].buff_time[b_n[i].buff_Size] = 60;
                                              b_n[i].buff_id[b_n[i].buff_Size] = 7;
                                              b_n[i].buff_Size++;
                                              p[P_id].act--;
                                              return;
                                          }
                                      }
                                  }
                              }
                          }
                      }
                  }
                  else if(em.rbutton) {
                      return;
                  }
              }
          }
        else {
            loadimage(&hi, L"./Game/picture/b_hint.png", 0, 0, false);
            putimage(400, 300, &hi);
            RECT t = { 400,300,650,400 };
            mm = L"AP不足";
            drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            FlushBatchDraw();
            Sleep(1000);
        }
        }
        else if (uk == 8) {
            if (p[P_id].act > 0) {
                p[P_id].buff_check[22]+=3;
                p[P_id].act--;
            }
            else {
                loadimage(&hi, L"./Game/picture/b_hint.png", 0, 0, false);
                putimage(400, 300, &hi);
                RECT t = { 400,300,650,400 };
                mm = L"AP不足";
                drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                FlushBatchDraw();
                Sleep(1000);
            }
        }
        else if (uk == 9) {
            if (p[P_id].r_cd[9] == 0) {
                loadimage(&ab, L"./Game/picture/ab.png", 0, 0, false);
                b_camera(b_m, p[P_id].x, p[P_id].y, b_mid);
                maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                for (i = b_m[b_mid].ox / 48; i < b_m[b_mid].ox / 48+20; i++) {
                    for (j = b_m[b_mid].oy/48; j < b_m[b_mid].oy / 48+15; j++) {
                        for (int k = 0; k < b_m[b_mid].esize; k++) {
                            if (e[k].x == i && e[k].y == j) {
                                transparentimage(NULL, 48 * (e[k].x) - b_m[b_mid].ox, 48 * (e[k].y) - b_m[b_mid].oy, &ab);
                            }
                        }
                    }
                }
                e_put(e, b_m, te, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                FlushBatchDraw();   
                ExMessage em;
                while (1) {
                    flushmessage(-1);
                    em = getmessage(EM_MOUSE);
                    for (i = b_m[b_mid].ox / 48; i < b_m[b_mid].ox / 48 + 20; i++) {
                        for (j = b_m[b_mid].oy / 48; j < b_m[b_mid].oy / 48 + 15; j++) {
                            for (int k = 0; k < b_m[b_mid].esize; k++) {
                                if (e[k].x == i && e[k].y == j&&em.lbutton) {
                                    if (em.x > e[k].x * 48-b_m[b_mid].ox && em.x<e[k].x * 48 + 48 - b_m[b_mid].ox&&em.y>e[k].y*48-b_m[b_mid].oy&&em.y< e[k].y * 48 +48- b_m[b_mid].oy&&em.x<960&&em.y<720) {
                                        e[k].buff_check[19]++;
                                        e[k].buff_time[e[k].buff_Size] = 10;
                                        e[k].buff_id[e[k].buff_Size] = 19;
                                        e[k].buff_Size++;
                                        p[P_id].r_cd[9] = 1;
                                        return;
                                    }
                                }
                            }
                        }
                    }
                    if (em.rbutton) {
                        return;
                    }
                }
            }
            else {
                loadimage(&hi, L"./Game/picture/b_hint.png", 0, 0, false);
                putimage(400, 300, &hi);
                RECT t = { 400,300,650,400 };
                mm = L"技能冷卻中";
                drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                FlushBatchDraw();
                Sleep(1000);
            }
        }
        else if (uk == 10) {
        if (p[P_id].s_cd[10] == 0) {
        loadimage(&ab, L"./Game/picture/ab.png", 0, 0, false);
        b_camera(b_m, p[P_id].x, p[P_id].y, b_mid);
        maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
        for (i = -1; i <= 1; i++) {
            for (j = -1; j <= 1; j++) {
                if (abs(i) + abs(j) < 2 && (i != 0 || j != 0) && te[p[P_id].x + i][p[P_id].y + j].player == 0 && te[p[P_id].x + i][p[P_id].y + j].npc== 0 && te[p[P_id].x + i][p[P_id].y + j].type == 0) {
                    transparentimage(NULL, 48 * (p[P_id].x + i) - b_m[b_mid].ox, 48 * (p[P_id].y + j) - b_m[b_mid].oy, &ab);
                }
            }
        }
        e_put(e, b_m, te, esize, b_mid);
        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
        p_put(p, b_m, psize, b_mid);
        ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
        FlushBatchDraw();
        while (1) {
            flushmessage(-1);
            ExMessage em;
            em = getmessage(EM_MOUSE);
            for (i = -1; i <= 1; i++) {
                for (j = -1; j <= 1; j++) {
                    if (abs(i) + abs(j) < 2 && (i != 0 || j != 0) && te[p[P_id].x + i][p[P_id].y + j].player == 0 && te[p[P_id].x + i][p[P_id].y + j].npc == 0 && te[p[P_id].x + i][p[P_id].y + j].type == 0) {
                        for (int I = 0; I < b_m[b_mid].esize; I++) {
                            if (em.x>e[I].x*48-b_m[b_mid].ox&&em.x< e[I].x * 48 - b_m[b_mid].ox+48&&em.y>e[I].y*48-b_m[b_mid].oy&&em.y< e[I].y * 48 - b_m[b_mid].oy+48&&em.x<960&&em.y<720&&em.lbutton) {
                                e[I].buff_check[20] = 1;
                                e[I].buff_time[e[I].buff_Size] = (p[P_id].str + p[P_id].dex) / 5 + 10;
                                e[I].buff_id[e[I].buff_Size] = 20;
                                e[I].buff_Size++;
                                p[P_id].buff_check[20] = 1;
                                p[P_id].buff_time[p[P_id].buff_Size] = (p[P_id].str + p[P_id].dex) / 5 + 10;
                                p[P_id].buff_id[p[P_id].buff_Size] = 20;
                                p[P_id].buff_Size++;
                                p[P_id].s_cd[10] = 30;
                                p[P_id].move = 0;
                                return;
                            }
                        }
                    }
                }
            }
            if (em.rbutton) {
                return;
            }
        }
        }
        else {
            loadimage(&hi, L"./Game/picture/b_hint.png", 0, 0, false);
            putimage(400, 300, &hi);
            RECT t = { 400,300,650,400 };
            mm = L"技能冷卻中";
            drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            FlushBatchDraw();
            Sleep(1000);
        }
        }
        else if (uk == 12) {
        if (p[P_id].act > 0) {
            loadimage(&ab, L"./Game/picture/ib.png", 0, 0, false);
            b_camera(b_m, p[P_id].x, p[P_id].y, b_mid);
            maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
            for (int sx = -5; sx <= 5; sx++) {
                for (int sy = -5; sy <= 5; sy++) {
                    if (abs(sx) + abs(sy) < 6 && te[p[P_id].x + sx][p[P_id].y + sy].enemy == 0 && te[p[P_id].x + sx][p[P_id].y + sy].type == 0) {
                        transparentimage(NULL, 48 * (p[P_id].x + sx) - b_m[b_mid].ox, 48 * (p[P_id].y + sy) - b_m[b_mid].oy, &ab);
                    }
                }
            }
            e_put(e, b_m, te, esize, b_mid);
            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
            p_put(p, b_m, psize, b_mid);
            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
            FlushBatchDraw();
            while (1) {
                ExMessage em;
                flushmessage(-1);
                em = getmessage(EM_MOUSE);
                if (em.lbutton) {
                    for (int sx = -5; sx <= 5; sx++) {
                        for (int sy = -5; sy <= 5; sy++) {
                            if (abs(sx) + abs(sy) < 6 && te[p[P_id].x + sx][p[P_id].y + sy].enemy == 0 && te[p[P_id].x + sx][p[P_id].y + sy].type == 0) {
                                if (em.x > (p[P_id].x + sx) * 48 - b_m[b_mid].ox && em.x< (p[P_id].x + sx) * 48 + 48 - b_m[b_mid].ox && em.y>(p[P_id].y + sy) * 48 - b_m[b_mid].oy && em.y < (p[P_id].y + sy) * 48 + 48 - b_m[b_mid].oy && em.x < 960 && em.y < 720) {
                                    for (i = 0; i < b_m[b_mid].psize; i++) {
                                        if (p[i].x == p[P_id].x + sx && p[i].y == p[P_id].y + sy) {
                                            p[i].buff_check[21] =1;
                                            p[i].buff_time[p[i].buff_Size] = 15 + p[i].wis;
                                            p[i].buff_id[p[i].buff_Size] = 21;
                                            p[i].buff_Size++;
                                            p[P_id].act--;
                                            return;
                                        }
                                    }
                                    for (i = 0; i < b_m[b_mid].nsize; i++) {
                                        if (b_n[i].x == p[P_id].x + sx && b_n[i].y == p[P_id].y + sy) {
                                            b_n[i].buff_check[22] = 1;
                                            b_n[i].buff_time[b_n[i].buff_Size] = 15 + p[i].wis;
                                            b_n[i].buff_id[b_n[i].buff_Size] = 21;
                                            b_n[i].buff_Size++;
                                            p[P_id].act--;
                                            return;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else if (em.rbutton) {
                    return;
                }
            }
        }
        else {
            loadimage(&hi, L"./Game/picture/b_hint.png", 0, 0, false);
            putimage(400, 300, &hi);
            RECT t = { 400,300,650,400 };
            mm = L"AP不足";
            drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            FlushBatchDraw();
            Sleep(1000);
        }
        }
        else if (uk == 18) {
        loadimage(&ab, L"./Game/picture/ib.png", 0, 0, false);
        b_camera(b_m, p[P_id].x, p[P_id].y, b_mid);
        maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
        for (i = -1; i <= 1; i++) {
            for (j = -1; j <= 1; j++) {
                if (abs(i) + abs(j) < 2 && (i != 0 || j != 0) && te[p[P_id].x + i][p[P_id].y + j].enemy == 0 && te[p[P_id].x + i][p[P_id].y + j].type == 0&& te[p[P_id].x + i][p[P_id].y + j].player==0&& te[p[P_id].x + i][p[P_id].y + j].npc==0&& te[p[P_id].x + i][p[P_id].y + j].trap==0) {
                    transparentimage(NULL, 48 * (p[P_id].x + i) - b_m[b_mid].ox, 48 * (p[P_id].y + j) - b_m[b_mid].oy, &ab);
                }
            }
        }
        e_put(e, b_m, te, esize, b_mid);
        b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
        p_put(p, b_m, psize, b_mid);
        ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
        FlushBatchDraw();
        ExMessage em;
        while (1) {
            flushmessage(-1);
            em = getmessage(EM_MOUSE);
            if (em.lbutton) {
                for (i = -1; i <= 1; i++) {
                    for (j = -1; j <= 1; j++) {
                        if (abs(i) + abs(j) < 2 && (i != 0 || j != 0) && te[p[P_id].x + i][p[P_id].y + j].enemy == 0 && te[p[P_id].x + i][p[P_id].y + j].type == 0 && te[p[P_id].x + i][p[P_id].y + j].player == 0 && te[p[P_id].x + i][p[P_id].y + j].npc == 0 && te[p[P_id].x + i][p[P_id].y + j].trap == 0) {
                            if (em.x > 48 * (p[P_id].x + i) - b_m[b_mid].ox && em.x < 48 * (p[P_id].x + i) + 48 - b_m[b_mid].ox && em.y>48 * (p[P_id].y + j) - b_m[b_mid].oy && em.y < 48 * (p[P_id].y + j) + 48 - b_m[b_mid].oy && em.x < 960 && em.y < 720) {
                                te[p[P_id].x + i][p[P_id].y + j].trap=1;
                                p[P_id].buff_check[26] = 1;
                                p[P_id].buff_time[p[P_id].buff_Size] = 10;
                                p[P_id].buff_id[p[P_id].buff_Size] = 26;
                                p[P_id].buff_Size++;
                                chose = "e";
                                return;
                            }
                        }
                    }
                }
            }
            else if (em.rbutton) {
                return;
            }
        }
        }
        else if (uk == 19) {
            if (p[P_id].act > 0) {
                p[P_id].buff_check[28] = 1;
                chose = "e";
                return;
            }
            else {
                loadimage(&hi, L"./Game/picture/b_hint.png", 0, 0, false);
                putimage(400, 300, &hi);
                RECT t = { 400,300,650,400 };
                mm = L"AP不足";
                drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                FlushBatchDraw();
                Sleep(1000);
            }
        }
        else if (uk == 20) {
            if (p[P_id].act > 0) {
                p[P_id].move += p[P_id].act * 4;
                p[P_id].act = 0;
            }
            else {
                loadimage(&hi, L"./Game/picture/b_hint.png", 0, 0, false);
                putimage(400, 300, &hi);
                RECT t = { 400,300,650,400 };
                mm = L"AP不足";
                drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                FlushBatchDraw();
                Sleep(1000);
            }
        }
        else if (uk == 21) {
            if (p[P_id].move > 0) {
                IMAGE p1, mb;
                queue<pair<int, int>> q;               
                wstring mm;
                LPCTSTR path;
                HWND hwnd = GetForegroundWindow();
                RECT rect;
                b_camera(b_m, p[P_id].x, p[P_id].y, b_mid);
                int rx, ry, m, X, Y, dr[100][100], cost[100][100], box[3000] = { 0 }, box_id = 0, p_move;
                BeginBatchDraw();
                maps(p, P_id, e, b_m, ar, te, b_mid);
                loadimage(&mb, L"./Game/picture/mb.png", 0, 0, false);
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
                    if (cost[X + 1][Y] > cost[X][Y] + te[X + 1][Y].mA && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x) {
                        cost[X + 1][Y] = cost[X][Y] + te[X + 1][Y].mA;
                        q.push({ X + 1,Y });
                    }
                    if (cost[X - 1][Y] > cost[X][Y] + te[X - 1][Y].mA && te[X - 1][Y].type != 1 && X - 1 >= 0) {
                        cost[X - 1][Y] = cost[X][Y] + te[X - 1][Y].mA;
                        q.push({ X - 1,Y });
                    }
                    if (cost[X][Y + 1] > cost[X][Y] + te[X][Y + 1].mA && te[X][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y) {
                        cost[X][Y + 1] = cost[X][Y] + te[X][Y + 1].mA;
                        q.push({ X ,Y + 1 });
                    }
                    if (cost[X][Y - 1] > cost[X][Y] + te[X][Y - 1].mA && te[X][Y - 1].type != 1 && Y - 1 >= 0) {
                        cost[X][Y - 1] = cost[X][Y] + te[X][Y - 1].mA;
                        q.push({ X ,Y - 1 });
                    }
                    if (dr[X][Y] == 0 && te[X][Y].enemy == 0 && te[X][Y].npc == 0 && te[X][Y].player == 0 && cost[X][Y] <= p[P_id].move) {
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
                mm = std::to_wstring(P_id);
                mm = L"./Game/picture/p" + mm + L".png";
                path = mm.c_str();
                loadimage(&p1, path, 0, 0, false);
                GetWindowRect(hwnd, &rect);
                int left = rect.left;
                int top = rect.top;
                SetCursorPos(p[P_id].x * 48 - b_m[b_mid].ox + 24 + left, p[P_id].y * 48 - b_m[b_mid].oy + 48 + top);
                int sss = 1, mx = p[P_id].x * 48, my = p[P_id].y * 48;
                p_move = p[P_id].move;
                e_put(e, b_m, te, esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, psize, b_mid);
                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                FlushBatchDraw();
                while (1) {
                    ExMessage m;
                    m = getmessage(EM_MOUSE);
                    if (m.lbutton) {
                        for (j = 0; j < 20; j++) {
                            for (i = 0; i < 15; i++) {
                                if (m.x <= j * 48 + 48 && m.x >= j * 48 && m.y <= i * 48 + 48 && m.y >= i * 48 && dr[j + b_m[b_mid].ox / 48][i + b_m[b_mid].oy / 48] == 1 && m.x < 960 && m.y < 720) {
                                    int k, w = 0;
                                    int xbox = p[P_id].x, ybox = p[P_id].y;
                                    for (w = 0; w < box_id; w++) {
                                        if (box[w] == 2) {
                                            if (p[P_id].move >= te[xbox][ybox + 1].mA) {
                                                p[P_id].move = p[P_id].move - te[xbox][ybox + 1].mA;
                                                if (te[xbox][ybox + 1].fire == 1) {
                                                    p[P_id].buff_check[12]++;
                                                }
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                        else if (box[w] == 4) {
                                            if (p[P_id].move >= te[xbox - 1][ybox].mA) {
                                                p[P_id].move = p[P_id].move - te[xbox - 1][ybox].mA;
                                                if (te[xbox - 1][ybox].fire == 1) {
                                                    p[P_id].buff_check[12]++;
                                                }
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                        else if (box[w] == 6) {
                                            if (p[P_id].move >= te[xbox + 1][ybox].mA) {
                                                p[P_id].move = p[P_id].move - te[xbox + 1][ybox].mA;
                                                if (te[xbox + 1][ybox].fire == 1) {
                                                    p[P_id].buff_check[12]++;
                                                }
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                        else if (box[w] == 8) {
                                            if (p[P_id].move >= te[xbox][ybox - 1].mA) {
                                                p[P_id].move = p[P_id].move - te[xbox][ybox - 1].mA;
                                                if (te[xbox][ybox - 1].fire == 1) {
                                                    p[P_id].buff_check[12]++;
                                                }
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                        for (int I = -2; I <= 2; I++) {
                                            for (int J = -2; J <= 2; J++) {
                                                if (te[xbox + I][ybox + J].dark == 2) {
                                                    te[xbox + I][ybox + J].dark = 1;
                                                }
                                            }
                                        }
                                        if (box[w] == 2) {
                                            for (int I = -2; I <= 2; I++) {
                                                for (int J = -2; J <= 2; J++) {
                                                    if (te[xbox + I][ybox + J+1].dark == 1) {
                                                        te[xbox + I][ybox + J+1].dark = 2;
                                                    }
                                                }
                                            }
                                            for (k = 0; k <= 48; k += 16) {
                                                BeginBatchDraw();
                                                maps(p, P_id, e, b_m, ar, te, b_mid);
                                                e_put(e, b_m, te, esize, b_mid);
                                                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                                p_bput(p, b_m, b_m[b_mid].psize, b_mid, P_id);
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
                                            for (int I = -2; I <= 2; I++) {
                                                for (int J = -2; J <= 2; J++) {
                                                    if (te[xbox + I-1][ybox + J].dark == 1) {
                                                        te[xbox + I-1][ybox + J].dark = 2;
                                                    }
                                                }
                                            }
                                            for (k = 0; k <= 48; k += 16) {
                                                BeginBatchDraw();
                                                maps(p, P_id, e, b_m, ar, te, b_mid);
                                                e_put(e, b_m, te, esize, b_mid);
                                                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                                p_bput(p, b_m, b_m[b_mid].psize, b_mid, P_id);
                                                if (xbox * 48 - k - b_m[b_mid].ox < 960 - k && ybox * 48 - 16 - b_m[b_mid].oy < 720) {
                                                    transparentimage(NULL, xbox * 48 - k - b_m[b_mid].ox, ybox * 48 - 16 - b_m[b_mid].oy, &p1, 0xFF55FF, 64 + 16, 64, 48, 64);
                                                }
                                                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                                                EndBatchDraw();
                                                Sleep(30);
                                                p[P_id].pose = 2;
                                            }
                                            xbox--;

                                        }
                                        else if (box[w] == 6) {
                                            for (int I = -2; I <= 2; I++) {
                                                for (int J = -2; J <= 2; J++) {
                                                    if (te[xbox + I+1][ybox + J].dark == 1) {
                                                        te[xbox + I+1][ybox + J].dark = 2;
                                                    }
                                                }
                                            }
                                            for (k = 0; k <= 48; k += 16) {
                                                BeginBatchDraw();
                                                maps(p, P_id, e, b_m, ar, te, b_mid);
                                                e_put(e, b_m, te, esize, b_mid);
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
                                            for (int I = -2; I <= 2; I++) {
                                                for (int J = -2; J <= 2; J++) {
                                                    if (te[xbox + I][ybox + J-1].dark == 1) {
                                                        te[xbox + I][ybox + J-1].dark = 2;
                                                    }
                                                }
                                            }
                                            for (k = 0; k <= 48; k += 16) {
                                                BeginBatchDraw();
                                                maps(p, P_id, e, b_m, ar, te, b_mid);
                                                e_put(e, b_m, te, esize, b_mid);
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
                                        if (te[xbox][ybox].trap == 2) {
                                            p[P_id].move = 0;
                                            p[P_id].buff_check[27]++;
                                            te[xbox][ybox].trap = 0;
                                            *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"觸發狩獵陷阱" << endl;
                                            int dmg = 5;
                                            pHP(p, b_m, P_id, b_mid, 0, dmg);
                                            FlushBatchDraw();
                                            Sleep(500);
                                            if (p[P_id].hp <= 0 && p[P_id].buff_check[14] == 0) {
                                                p[P_id].buff_check[14] = 1;
                                                p[P_id].buff_time[p[P_id].buff_Size] = 15;
                                                p[P_id].buff_id[p[P_id].buff_Size] = 14;
                                                p[P_id].buff_Size++;
                                            }
                                        }
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
                                    *wofs << L"(T" << b_m[b_mid].time << L")" << p[P_id].name << L"從(" << p[P_id].x << L"," << p[P_id].y << L")到(" << xbox << L"," << ybox << L")" << endl;
                                    p[P_id].x = xbox; p[P_id].y = ybox;
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
                                    p[P_id].move = 0;
                                    return;
                                }
                            }
                        }
                    }
                    if (m.rbutton) {
                        return;
                    }
                    if (m.message == WM_MOUSEMOVE) {
                        if (te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].type != 1 && te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].player != 1 && te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].npc != 1 && te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].enemy != 1 && m.x < 960 && m.y < 720) {                   
                            if (m.x < mx - b_m[b_mid].ox) {
                                if (box[box_id - 1] == 6 && m.x / 48 + b_m[b_mid].ox / 48 == mx / 48 - 1 && m.y / 48 + b_m[b_mid].oy / 48 == my / 48) {
                                    p_move += te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].mA;
                                    box_id--;
                                    mx = m.x / 48 * 48 + b_m[b_mid].ox;
                                }
                                else if (p_move >= te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].mA && m.x / 48 + b_m[b_mid].ox / 48 == mx / 48 - 1 && m.y / 48 + b_m[b_mid].oy / 48 == my / 48) {
                                    box[box_id] = 4;
                                    box_id++;
                                    mx = m.x / 48 * 48 + b_m[b_mid].ox;
                                    p_move -= te[mx / 48][my / 48].mA;
                                }
                                q.push({ mx / 48,my / 48 });
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
                                    if (cost[X + 1][Y] > cost[X][Y] + te[X + 1][Y].mA && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x) {
                                        cost[X + 1][Y] = cost[X][Y] + te[X + 1][Y].mA;
                                        q.push({ X + 1,Y });
                                    }
                                    if (cost[X - 1][Y] > cost[X][Y] + te[X - 1][Y].mA && te[X - 1][Y].type != 1 && X - 1 >= 0) {
                                        cost[X - 1][Y] = cost[X][Y] + te[X - 1][Y].mA;
                                        q.push({ X - 1,Y });
                                    }
                                    if (cost[X][Y + 1] > cost[X][Y] + te[X][Y + 1].mA && te[X][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y) {
                                        cost[X][Y + 1] = cost[X][Y] + te[X][Y + 1].mA;
                                        q.push({ X ,Y + 1 });
                                    }
                                    if (cost[X][Y - 1] > cost[X][Y] + te[X][Y - 1].mA && te[X][Y - 1].type != 1 && Y - 1 >= 0) {
                                        cost[X][Y - 1] = cost[X][Y] + te[X][Y - 1].mA;
                                        q.push({ X ,Y - 1 });
                                    }
                                    if (dr[X][Y] == 0 && te[X][Y].enemy == 0 && te[X][Y].npc == 0 && te[X][Y].player == 0 && cost[X][Y] <= p_move) {
                                        dr[X][Y] = 1;
                                    }
                                    q.pop();
                                }
                                b_camera(b_m, mx / 48, my / 48, b_mid);
                                maps(p, P_id, e, b_m, ar, te, b_mid);
                                for (i = 0; i < b_m[b_mid].x; i++) {
                                    for (j = 0; j < b_m[b_mid].y; j++) {
                                        if (dr[i][j] == 1) {
                                            transparentimage(NULL, 48 * i - b_m[b_mid].ox, 48 * j - b_m[b_mid].oy, &mb);
                                        }
                                    }
                                }
                                int px = p[P_id].x * 48, py = p[P_id].y * 48;
                                for (i = 0; i < box_id; i++) {
                                    if (box[i] == 2) {
                                        py += 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy},{px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy}, {px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 30} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 45} };
                                        solidpolygon(pts3, 3);
                                    }
                                    else if (box[i] == 4) {
                                        px -= 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox + 18 , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox + 18, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 20} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 18, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 18,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 3, py - b_m[b_mid].oy + 23} };
                                        solidpolygon(pts3, 3);
                                    }
                                    else if (box[i] == 6) {
                                        px += 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox , py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 20} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 30,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 45, py - b_m[b_mid].oy + 23} };
                                        solidpolygon(pts3, 3);
                                    }
                                    else if (box[i] == 8) {
                                        py -= 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 18},{px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 48}, {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 48} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 3} };
                                        solidpolygon(pts3, 3);
                                    }
                                }
                                e_put(e, b_m, te, esize, b_mid);
                                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                p_put(p, b_m, psize, b_mid);
                                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                                FlushBatchDraw();
                                SetCursorPos(mx - b_m[b_mid].ox + 24 + left, my - b_m[b_mid].oy + 48 + top);
                            }
                            else if (m.y < my - b_m[b_mid].oy) {
                                if (box[box_id - 1] == 2 && m.x / 48 + b_m[b_mid].ox / 48 == mx / 48 && m.y / 48 + b_m[b_mid].oy / 48 == my / 48 - 1) {
                                    p_move += te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].mA;
                                    box_id--;
                                    my = m.y / 48 * 48 + b_m[b_mid].oy;
                                }
                                else if (p_move >= te[m.x / 48][m.y / 48].mA && m.x / 48 + b_m[b_mid].ox / 48 == mx / 48 && m.y / 48 + b_m[b_mid].oy / 48 == my / 48 - 1) {
                                    box[box_id] = 8;
                                    box_id++;
                                    my = m.y / 48 * 48 + b_m[b_mid].oy;
                                    p_move -= te[mx / 48][my / 48].mA;
                                }
                                q.push({ mx / 48,my / 48 });
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
                                    if (cost[X + 1][Y] > cost[X][Y] + te[X + 1][Y].mA && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x) {
                                        cost[X + 1][Y] = cost[X][Y] + te[X + 1][Y].mA;
                                        q.push({ X + 1,Y });
                                    }
                                    if (cost[X - 1][Y] > cost[X][Y] + te[X - 1][Y].mA && te[X - 1][Y].type != 1 && X - 1 >= 0) {
                                        cost[X - 1][Y] = cost[X][Y] + te[X - 1][Y].mA;
                                        q.push({ X - 1,Y });
                                    }
                                    if (cost[X][Y + 1] > cost[X][Y] + te[X][Y + 1].mA && te[X][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y) {
                                        cost[X][Y + 1] = cost[X][Y] + te[X][Y + 1].mA;
                                        q.push({ X ,Y + 1 });
                                    }
                                    if (cost[X][Y - 1] > cost[X][Y] + te[X][Y - 1].mA && te[X][Y - 1].type != 1 && Y - 1 >= 0) {
                                        cost[X][Y - 1] = cost[X][Y] + te[X][Y - 1].mA;
                                        q.push({ X ,Y - 1 });
                                    }
                                    if (dr[X][Y] == 0 && te[X][Y].enemy == 0 && te[X][Y].npc == 0 && te[X][Y].player == 0 && cost[X][Y] <= p_move) {
                                        dr[X][Y] = 1;
                                    }
                                    q.pop();
                                }
                                b_camera(b_m, mx / 48, my / 48, b_mid);
                                maps(p, P_id, e, b_m, ar, te, b_mid);
                                for (i = 0; i < b_m[b_mid].x; i++) {
                                    for (j = 0; j < b_m[b_mid].y; j++) {
                                        if (dr[i][j] == 1) {
                                            transparentimage(NULL, 48 * i - b_m[b_mid].ox, 48 * j - b_m[b_mid].oy, &mb);
                                        }
                                    }
                                }
                                int px = p[P_id].x * 48, py = p[P_id].y * 48;
                                for (i = 0; i < box_id; i++) {
                                    if (box[i] == 2) {
                                        py += 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy},{px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy}, {px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 30} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 45} };
                                        solidpolygon(pts3, 3);
                                    }
                                    else if (box[i] == 4) {
                                        px -= 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox + 18 , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox + 18, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 20} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 18, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 18,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 3, py - b_m[b_mid].oy + 23} };
                                        solidpolygon(pts3, 3);
                                    }
                                    else if (box[i] == 6) {
                                        px += 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox , py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 20} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 30,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 45, py - b_m[b_mid].oy + 23} };
                                        solidpolygon(pts3, 3);
                                    }
                                    else if (box[i] == 8) {
                                        py -= 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 18},{px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 48}, {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 48} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 3} };
                                        solidpolygon(pts3, 3);
                                    }
                                }
                                e_put(e, b_m, te, esize, b_mid);
                                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                p_put(p, b_m, psize, b_mid);
                                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                                FlushBatchDraw();
                                SetCursorPos(mx - b_m[b_mid].ox + 24 + left, my - b_m[b_mid].oy + 48 + top);
                            }
                            else if (m.x > mx + 48 - b_m[b_mid].ox) {
                                if (box[box_id - 1] == 4 && m.x / 48 + b_m[b_mid].ox / 48 == mx / 48 + 1 && m.y / 48 + b_m[b_mid].oy / 48 == my / 48) {
                                    p_move += te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].mA;
                                    box_id--;
                                    mx = m.x / 48 * 48 + b_m[b_mid].ox;
                                }
                                else if (p_move >= te[m.x / 48][m.y / 48].mA && m.x / 48 + b_m[b_mid].ox / 48 == mx / 48 + 1 && m.y / 48 + b_m[b_mid].oy / 48 == my / 48) {
                                    box[box_id] = 6;
                                    box_id++;
                                    mx = m.x / 48 * 48 + b_m[b_mid].ox;
                                    p_move -= te[mx / 48][my / 48].mA;
                                }
                                q.push({ mx / 48,my / 48 });
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
                                    if (cost[X + 1][Y] > cost[X][Y] + te[X + 1][Y].mA && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x) {
                                        cost[X + 1][Y] = cost[X][Y] + te[X + 1][Y].mA;
                                        q.push({ X + 1,Y });
                                    }
                                    if (cost[X - 1][Y] > cost[X][Y] + te[X - 1][Y].mA && te[X - 1][Y].type != 1 && X - 1 >= 0) {
                                        cost[X - 1][Y] = cost[X][Y] + te[X - 1][Y].mA;
                                        q.push({ X - 1,Y });
                                    }
                                    if (cost[X][Y + 1] > cost[X][Y] + te[X][Y + 1].mA && te[X][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y) {
                                        cost[X][Y + 1] = cost[X][Y] + te[X][Y + 1].mA;
                                        q.push({ X ,Y + 1 });
                                    }
                                    if (cost[X][Y - 1] > cost[X][Y] + te[X][Y - 1].mA && te[X][Y - 1].type != 1 && Y - 1 >= 0) {
                                        cost[X][Y - 1] = cost[X][Y] + te[X][Y - 1].mA;
                                        q.push({ X ,Y - 1 });
                                    }
                                    if (dr[X][Y] == 0 && te[X][Y].enemy == 0 && te[X][Y].npc == 0 && te[X][Y].player == 0 && cost[X][Y] <= p_move) {
                                        dr[X][Y] = 1;
                                    }
                                    q.pop();
                                }
                                b_camera(b_m, mx / 48, my / 48, b_mid);
                                maps(p, P_id, e, b_m, ar, te, b_mid);
                                for (i = 0; i < b_m[b_mid].x; i++) {
                                    for (j = 0; j < b_m[b_mid].y; j++) {
                                        if (dr[i][j] == 1) {
                                            transparentimage(NULL, 48 * i - b_m[b_mid].ox, 48 * j - b_m[b_mid].oy, &mb);
                                        }
                                    }
                                }
                                int px = p[P_id].x * 48, py = p[P_id].y * 48;
                                for (i = 0; i < box_id; i++) {
                                    if (box[i] == 2) {
                                        py += 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy},{px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy}, {px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 30} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 45} };
                                        solidpolygon(pts3, 3);
                                    }
                                    else if (box[i] == 4) {
                                        px -= 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox + 18 , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox + 18, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 20} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 18, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 18,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 3, py - b_m[b_mid].oy + 23} };
                                        solidpolygon(pts3, 3);
                                    }
                                    else if (box[i] == 6) {
                                        px += 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox , py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 20} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 30,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 45, py - b_m[b_mid].oy + 23} };
                                        solidpolygon(pts3, 3);
                                    }
                                    else if (box[i] == 8) {
                                        py -= 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 18},{px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 48}, {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 48} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 3} };
                                        solidpolygon(pts3, 3);
                                    }
                                }
                                e_put(e, b_m, te, esize, b_mid);
                                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                p_put(p, b_m, psize, b_mid);
                                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                                FlushBatchDraw();
                                SetCursorPos(mx - b_m[b_mid].ox + 24 + left, my - b_m[b_mid].oy + 48 + top);
                            }
                            else if (m.y > my + 48 - b_m[b_mid].oy) {
                                if (box[box_id - 1] == 8 && m.x / 48 + b_m[b_mid].ox / 48 == mx / 48 && m.y / 48 + b_m[b_mid].oy / 48 == my / 48 + 1) {
                                    p_move += te[m.x / 48 + b_m[b_mid].ox / 48][m.y / 48 + b_m[b_mid].oy / 48].mA;
                                    box_id--;
                                    my = m.y / 48 * 48 + b_m[b_mid].oy;
                                }
                                else if (p_move >= te[m.x / 48][m.y / 48].mA && m.x / 48 + b_m[b_mid].ox / 48 == mx / 48 && m.y / 48 + b_m[b_mid].oy / 48 == my / 48 + 1) {
                                    box[box_id] = 2;
                                    box_id++;
                                    my = m.y / 48 * 48 + b_m[b_mid].oy;
                                    p_move -= te[mx / 48][my / 48].mA;
                                }
                                q.push({ mx / 48,my / 48 });
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
                                    if (cost[X + 1][Y] > cost[X][Y] + te[X + 1][Y].mA && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x) {
                                        cost[X + 1][Y] = cost[X][Y] + te[X + 1][Y].mA;
                                        q.push({ X + 1,Y });
                                    }
                                    if (cost[X - 1][Y] > cost[X][Y] + te[X - 1][Y].mA && te[X - 1][Y].type != 1 && X - 1 >= 0) {
                                        cost[X - 1][Y] = cost[X][Y] + te[X - 1][Y].mA;
                                        q.push({ X - 1,Y });
                                    }
                                    if (cost[X][Y + 1] > cost[X][Y] + te[X][Y + 1].mA && te[X][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y) {
                                        cost[X][Y + 1] = cost[X][Y] + te[X][Y + 1].mA;
                                        q.push({ X ,Y + 1 });
                                    }
                                    if (cost[X][Y - 1] > cost[X][Y] + te[X][Y - 1].mA && te[X][Y - 1].type != 1 && Y - 1 >= 0) {
                                        cost[X][Y - 1] = cost[X][Y] + te[X][Y - 1].mA;
                                        q.push({ X ,Y - 1 });
                                    }
                                    if (dr[X][Y] == 0 && te[X][Y].enemy == 0 && te[X][Y].npc == 0 && te[X][Y].player == 0 && cost[X][Y] <= p_move) {
                                        dr[X][Y] = 1;
                                    }
                                    q.pop();
                                }
                                b_camera(b_m, mx / 48, my / 48, b_mid);
                                maps(p, P_id, e, b_m, ar, te, b_mid);
                                for (i = 0; i < b_m[b_mid].x; i++) {
                                    for (j = 0; j < b_m[b_mid].y; j++) {
                                        if (dr[i][j] == 1) {
                                            transparentimage(NULL, 48 * i - b_m[b_mid].ox, 48 * j - b_m[b_mid].oy, &mb);
                                        }
                                    }
                                }
                                int px = p[P_id].x * 48, py = p[P_id].y * 48;
                                for (i = 0; i < box_id; i++) {
                                    if (box[i] == 2) {
                                        py += 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy},{px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy}, {px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 30} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 30}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 45} };
                                        solidpolygon(pts3, 3);
                                    }
                                    else if (box[i] == 4) {
                                        px -= 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox + 18 , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox + 18, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 48, py - b_m[b_mid].oy + 20} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 18, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 18,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 3, py - b_m[b_mid].oy + 23} };
                                        solidpolygon(pts3, 3);
                                    }
                                    else if (box[i] == 6) {
                                        px += 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox , py - b_m[b_mid].oy + 20},{px - b_m[b_mid].ox , py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 26}, {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 20} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 30, py - b_m[b_mid].oy + 14}, {px - b_m[b_mid].ox + 30,py - b_m[b_mid].oy + 32}, {px - b_m[b_mid].ox + 45, py - b_m[b_mid].oy + 23} };
                                        solidpolygon(pts3, 3);
                                    }
                                    else if (box[i] == 8) {
                                        py -= 48;
                                        POINT pts[] = { {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 18},{px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 26, py - b_m[b_mid].oy + 48}, {px - b_m[b_mid].ox + 20, py - b_m[b_mid].oy + 48} };
                                        solidpolygon(pts, 4);
                                        POINT pts3[] = { {px - b_m[b_mid].ox + 14, py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 32,py - b_m[b_mid].oy + 18}, {px - b_m[b_mid].ox + 23, py - b_m[b_mid].oy + 3} };
                                        solidpolygon(pts3, 3);
                                    }
                                }
                                e_put(e, b_m, te, esize, b_mid);
                                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                p_put(p, b_m, psize, b_mid);
                                ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                                FlushBatchDraw();
                            SetCursorPos(mx - b_m[b_mid].ox + 24 + left, my - b_m[b_mid].oy + 48 + top);
                            }
                        }
                        else if (m.x<p[P_id].x * 48 + 48 - b_m[b_mid].ox && m.x > p[P_id].x * 48 - b_m[b_mid].ox && m.y<p[P_id].y * 48 - b_m[b_mid].oy + 48 && m.y>p[P_id].y * 48 - b_m[b_mid].oy && m.x < 960 && m.y < 720 && box_id == 1) {
                            box_id = 0;
                            box[0] = 0;
                            mx = p[P_id].x * 48, my = p[P_id].y * 48;
                            p_move = p[P_id].move;
                            q.push({ mx / 48,my / 48 });
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
                                if (cost[X + 1][Y] > cost[X][Y] + te[X + 1][Y].mA && te[X + 1][Y].type != 1 && X + 1 < b_m[b_mid].x) {
                                    cost[X + 1][Y] = cost[X][Y] + te[X + 1][Y].mA;
                                    q.push({ X + 1,Y });
                                }
                                if (cost[X - 1][Y] > cost[X][Y] + te[X - 1][Y].mA && te[X - 1][Y].type != 1 && X - 1 >= 0) {
                                    cost[X - 1][Y] = cost[X][Y] + te[X - 1][Y].mA;
                                    q.push({ X - 1,Y });
                                }
                                if (cost[X][Y + 1] > cost[X][Y] + te[X][Y + 1].mA && te[X][Y + 1].type != 1 && Y + 1 < b_m[b_mid].y) {
                                    cost[X][Y + 1] = cost[X][Y] + te[X][Y + 1].mA;
                                    q.push({ X ,Y + 1 });
                                }
                                if (cost[X][Y - 1] > cost[X][Y] + te[X][Y - 1].mA && te[X][Y - 1].type != 1 && Y - 1 >= 0) {
                                    cost[X][Y - 1] = cost[X][Y] + te[X][Y - 1].mA;
                                    q.push({ X ,Y - 1 });
                                }
                                if (dr[X][Y] == 0 && te[X][Y].enemy == 0 && te[X][Y].npc == 0 && te[X][Y].player == 0 && cost[X][Y] <= p_move) {
                                    dr[X][Y] = 1;
                                }
                                q.pop();
                            }
                            b_camera(b_m, mx / 48, my / 48, b_mid);
                            maps(p, P_id, e, b_m, ar, te, b_mid);
                            for (i = 0; i < b_m[b_mid].x; i++) {
                                for (j = 0; j < b_m[b_mid].y; j++) {
                                    if (dr[i][j] == 1) {
                                        transparentimage(NULL, 48 * i - b_m[b_mid].ox, 48 * j - b_m[b_mid].oy, &mb);
                                    }
                                }
                            }
                            e_put(e, b_m, te, esize, b_mid);
                            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                            p_put(p, b_m, psize, b_mid);
                            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                            FlushBatchDraw();
                            SetCursorPos(mx - b_m[b_mid].ox + 24 + left, my - b_m[b_mid].oy + 48 + top);
                        }
                    }
                }
            }
            else {
                loadimage(&hi, L"./Game/picture/b_hint.png", 0, 0, false);
                putimage(400, 300, &hi);
                RECT t = { 400,300,650,400 };
                mm = L"MP不足";
                drawtext(mm.c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                FlushBatchDraw();
                Sleep(1000);
            }
        }
    }
}
void b_end_menu(player* p, enemy* e, enemy_type* e_t, item* it, b_map* b_m, int b_mid) {
    IMAGE p1, fa, im;
    wstring path;
    string number;
    int a[10] = { 0 }, pa, nn, J = 0;
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
        transparentimage(NULL, 20, 30 + 180 * i, &fa, 0xFF55FF);
        outtextxy(80, 12 + 180 * i, p[i].name.c_str());
        path = L"LV " + to_wstring(p[i].lv);
        outtextxy(165, 25 + 180 * i, path.c_str());
        path = L"EXP " + to_wstring(p[i].exp) + L"/" + to_wstring(int(p[i].lv * 100 + pow(p[i].lv, 3)));
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
                outtextxy(320, 65 + 180 * i, L"^");
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
                p[i].mhp += (p[i].conExp / 100) * 2;
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
            if (p[i].wisExp >= 100) {
                p[i].wis += p[i].wisExp / 100;
                p[i].wisExp -= p[i].wisExp / 100 * 100;
                outtextxy(320, 140 + 180 * i, L"^");
            }
            p[i].sp++;

            outtextxy(320, 155 + 180 * i, L"^");
        }
        path = L"STR " + to_wstring(p[i].str);
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
        path = L"SP " + to_wstring(p[i].sp);
        outtextxy(165, 155 + 180 * i, path.c_str());
    }
    for (i = 0; i < b_m[b_mid].esize; i++) {
        number = "";
        for (j = 0; j < e_t[e[i].type].drop.size(); j++) {
            if (e_t[e[i].type].drop[j] == '%') {
                pa = stoi(number);
                number = "";
            }
            else if (e_t[e[i].type].drop[j] == 'i') {
                int rd = rand()%100 + 1;
                nn = stoi(number);
                number = "";
                if (rd <= pa) {
                    it[nn].number++;
                    wstring mm;
                    mm = L"./Game/picture/item" + to_wstring(nn) + L".png";
                    loadimage(&im, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 500, 30 + J * 40, &im, 0xFF55FF);
                    LPCTSTR path1 = it[nn].Name.c_str();
                    outtextxy(530, 30 + J * 40, path1);
                    J++;
                }

            }                
            else {
                    number += e_t[e[i].type].drop[j];
            }
        }

    }        
    EndBatchDraw();
    Sleep(3000);
    ExMessage fq;
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    while (1) {
            if (peekmessage(&fq, EM_MOUSE)) {
                if (fq.lbutton) {
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
                if (te[xbox][ybox + 1].fire == 1) {
                    b_n[b_nid].buff_check[12]++;
                }
            }
            else {
                break;
            }
        }
        else if (box[w] == 4) {
            if (b_n[b_nid].move >= te[xbox - 1][ybox].mA) {
                b_n[b_nid].move = b_n[b_nid].move - te[xbox - 1][ybox].mA;
                if (te[xbox-1][ybox].fire == 1) {
                    b_n[b_nid].buff_check[12]++;
                }
            }
            else {
                break;
            }
        }
        else if (box[w] == 6) {
            if (b_n[b_nid].move >= te[xbox + 1][ybox].mA) {
                b_n[b_nid].move = b_n[b_nid].move - te[xbox + 1][ybox].mA;
                if (te[xbox+1][ybox ].fire == 1) {
                    b_n[b_nid].buff_check[12]++;
                }
            }
            else {
                break;
            }
        }
        else if (box[w] == 8) {
            if (b_n[b_nid].move >= te[xbox][ybox - 1].mA) {
                b_n[b_nid].move = b_n[b_nid].move - te[xbox][ybox - 1].mA;
                if (te[xbox][ybox - 1].fire == 1) {
                    b_n[b_nid].buff_check[12]++;
                }
            }
            else {
                break;
            }
        }
        if (box[w] == 2) {

            for (k = 0; k <= 48; k += 16) {

                BeginBatchDraw();
                maps(p, P_id, e, b_m, ar, te, b_mid);
                e_put(e, b_m, te, esize, b_mid);
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
                maps(p, P_id, e, b_m, ar, te, b_mid);
                e_put(e, b_m, te, esize, b_mid);
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
                maps(p, P_id, e, b_m, ar, te,b_mid);
                e_put(e, b_m, te, esize, b_mid);
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
                maps(p, P_id, e, b_m, ar, te, b_mid);
                e_put(e, b_m, te, esize, b_mid);
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
        if (te[xbox][ybox].trap == 2) {
            b_n[b_nid].move = 0;
            b_n[b_nid].buff_check[27]++;
            te[xbox][ybox].trap = 0;
            int dmg = 5;
            nHP(b_n, b_m, b_nid, b_mid, 0, dmg);
            *wofs << L"(T" << b_m[b_mid].time << L")" << b_n[b_nid].name << L"觸發狩獵陷阱" << endl;
            FlushBatchDraw();
            Sleep(500);
            if (b_n[b_nid].hp <= 0) {
                te[b_n[b_nid].x][b_n[b_nid].y].mA -= 10000;
                te[b_n[b_nid].x][b_n[b_nid].y].mB -= 10000;
                int x = b_n[b_nid].x, y = b_n[b_nid].y;
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
                te[b_n[b_nid].x][b_n[b_nid].y].npc = 0;
                b_n[b_nid].x = -1; b_n[b_nid].y = -1;
                b_n[b_nid].speed = -1;
                return;
            }
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
    wstring nn, vos,mm;
    TCHAR t[5];
    if (b_n[b_nid].hp > 0) {
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
        if (ar[b_n[b_nid].baid].type[0] == 'r') {
            ATK = roll("", 2) + roll(ar[b_n[b_nid].baid].hit, 1) + (b_n[b_nid].dex - 10) / 2;
        }
        else if (ar[b_n[b_nid].baid].type[0] == 'm') {
            ATK = roll("", 2) + roll(ar[b_n[b_nid].baid].hit, 1) + (b_n[b_nid].str - 10) / 2;
        }
        AC = roll("", 2) + (e[id].dex - 10) / 2;
        if (e[id].buff_check[20] == 1) {
            AC = 0;
        }
        if (e[id].buff_check[27] == 1) {
            AC /= 2;
        }
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
                    maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                    e_put(e, b_m, te, esize, b_mid);
                    b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                    p_put(p, b_m, psize, b_mid);
                    transparentimageA(NULL, e[id].x * 48 - b_m[b_mid].ox - 30, e[id].y * 48 - b_m[b_mid].oy - 160, &ae, i * 120, j * 300, 120, 300);
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
                    maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                    e_put(e, b_m, te, esize, b_mid);
                    b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                    p_put(p, b_m, psize, b_mid);
                    transparentimageA(NULL, e[id].x * 48 - b_m[b_mid].ox - 25, e[id].y * 48 - 16 - b_m[b_mid].oy - 20, &ae, i * 120, j * 120, 120, 120);
                    ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                    EndBatchDraw();
                    Sleep(5);
                }
            }
        }
        if (ATK > AC) {
            BeginBatchDraw();
            maps(p, P_id, e, b_m, ar, te,b_mid);/*地圖繪製*/
            e_put(e, b_m, te, esize, b_mid);
            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
            p_put(p, b_m, psize, b_mid);
            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
            int  DMG = roll(ar[b_n[b_nid].baid].dmg, 1);
            if (ar[b_n[b_nid].baid].type[0] == 'm') {
                DMG += (b_n[b_nid].str - 10) / 5;
            }
            if (b_n[b_nid].buff_check[21] > 0) {
                DMG += (b_n[b_nid].wis - 5) / 2;
            }
            eHP(e, b_m, id, b_mid, 0, DMG);
            if (b_n[b_nid].buff_check[21] > 0) {
                wstring mm = L"+" + to_wstring((b_n[b_nid].mhp - b_n[b_nid].hp) / 5);
                settextcolor(RGB(47, 255, 197));
                settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                outtextxy(b_n[b_nid].x * 48 - b_m[b_mid].ox, b_n[b_nid].y * 48 - b_m[b_mid].oy - 40, mm.c_str());
                b_n[b_nid].hp += (b_n[b_nid].mhp - b_n[b_nid].hp) / 5;
                settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                settextcolor(RGB(0, 0, 0));
            }
            *wofs << L"(T" << b_m[b_mid].time << L")" << b_n[b_nid].name << L"用" << ar[b_n[b_nid].baid].name << L"命中" << e[id].name << L"造成" << DMG << L"點傷害(" << ATK << ">" << AC << ")" << endl;
            EndBatchDraw();
            IMAGE tri;
            loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
            transparentimage(NULL, (b_n[b_nid].x) * 48 - b_m[b_mid].ox, ((b_n[b_nid].y - 1) * 48) - b_m[b_mid].oy, &tri);
            Sleep(1000);
            if (e[id].hp <= 0) {
                te[e[id].x][e[id].y].mA = 1;
                te[e[id].x][e[id].y].mB = 1;
                te[e[id].x][e[id].y].enemy = 0;
                e[id].x = -1; e[id].y = -1;
                e[id].speed = -1;
                if (e[id].buff_check[20] > 0) {
                    p[0].buff_check[20] = 0;
                    e[id].buff_check[20] = 0;
                }
            }
        }
        else {
            BeginBatchDraw();
            maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
            e_put(e, b_m, te, esize, b_mid);
            b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
            p_put(p, b_m, psize, b_mid);
            ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
            *wofs << L"(T" << b_m[b_mid].time << L")" << b_n[b_nid].name << L"用" << ar[b_n[b_nid].baid].name << L"攻擊" << e[id].name << L"未能命中(" << ATK << "<=" << AC << ")" << endl;
            nn = L"miss";
            settextcolor(RGB(255, 0, 0));
            settextstyle(25, 0, _T("Taipei Sans TC Beta"));
            outtextxy(e[id].x * 48 - b_m[b_mid].ox, e[id].y * 48 - b_m[b_mid].oy-40, nn.c_str());
            settextstyle(18, 0, _T("Taipei Sans TC Beta"));
            settextcolor(RGB(0, 0, 0));
            if (b_n[b_nid].buff_check[21] > 0) {
                wstring mm = L"+" + to_wstring((b_n[b_nid].mhp - b_n[b_nid].hp) / 5);
                settextcolor(RGB(47, 255, 197));
                settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                outtextxy(b_n[b_nid].x * 48 - b_m[b_mid].ox, b_n[b_nid].y * 48 - b_m[b_mid].oy - 40, mm.c_str());
                b_n[b_nid].hp += (b_n[b_nid].mhp - b_n[b_nid].hp) / 5;
                settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                settextcolor(RGB(0, 0, 0));
            }
            EndBatchDraw();
            IMAGE tri;
            loadimage(&tri, L"./Game/picture/tri.png", 0, 0, false);
            transparentimage(NULL, (b_n[b_nid].x) * 48 - b_m[b_mid].ox, ((b_n[b_nid].y - 1) * 48) - b_m[b_mid].oy, &tri);
            Sleep(1000);
        }
        /*掩護射擊處理*/
        if (e[id].hp > 0) {
            for (i = 0; i < b_m[b_mid].psize; i++) {
                if (p[i].buff_check[28] > 0) {
                    int range = 0;
                    int ar_id = -1;
                    if (p[i].arms_id_1 != -1 && ar[p[i].arms_id_1].type[0] != 'm') {
                        if (p[i].arms_b_1 > 0) {
                            range = ar[p[i].arms_id_1].range;
                            ar_id = p[i].arms_id_1;
                        }
                        if (p[i].buff_check[23] > 0 && ar[p[i].arms_id_2].type[1] == 'r') {
                            range++;
                        }
                    }
                    if (p[i].arms_id_2 != -1 && ar[p[i].arms_id_2].type[0] != 'm') {
                        if (p[i].arms_b_2 > 0) {
                            int rangeb = ar[p[i].arms_id_2].range;
                            if (p[i].buff_check[23] > 0 && ar[p[i].arms_id_2].type[1] == 'r') {
                                rangeb++;
                            }
                            if (range < rangeb) {
                                range = rangeb;
                                ar_id = p[i].arms_id_2;
                            }
                        }
                    }
                    if (abs(p[i].x - e[id].x) + abs(p[i].y - e[id].y) <= range && ar_id != -1) {
                        int AC = roll("", 2) + (e[id].dex - 10) / 2;
                        if (e[id].buff_check[20] == 1) {
                            AC = 0;
                        }
                        if (e[id].buff_check[27] > 0) {
                            AC /= 2;
                        }
                        int ATK = roll("", 2) + (p[i].dex - 10) / 2 + roll(ar[ar_id].hit, 1);
                        wstring vos;
                        IMAGE ae;
                        if (ar_id == 0 || ar_id == 5) {
                            mciSendString(L"close se", NULL, 0, NULL);
                            mciSendString(L"open ./Game/Sound/SE/拳銃2.mp3 alias se", NULL, 0, NULL);
                            vos = L"setaudio se volume to " + to_wstring(soundSize);
                            mciSendString(vos.c_str(), NULL, 0, NULL);
                            mciSendString(L"play se from 0", NULL, 0, NULL);
                            loadimage(&ae, L"./Game/picture/武器・弓1.png", 0, 0, false);
                            BeginBatchDraw();
                            for (j = 0; j < 5; j++) {
                                for (i = 0; i < 3; i++) {
                                    maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                                    e_put(e, b_m, te, esize, b_mid);
                                    b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                                    p_put(p, b_m, psize, b_mid);
                                    transparentimageA(NULL, e[id].x * 48 - b_m[b_mid].ox - 30, e[id].y * 48 - b_m[b_mid].oy - 160, &ae, i * 120, j * 300, 120, 300);
                                    ui(p, e, b_m, P_id, esize, psize, b_mid, bu_id);
                                    FlushBatchDraw();
                                    Sleep(5);
                                }
                            }
                            EndBatchDraw();
                        }
                        /*ae*/
                        if (ATK > AC) {
                            int dmg = roll(ar[ar_id].dmg, 1);
                            if (p[i].buff_check[21] > 0) {
                                dmg += (p[P_id].wis - 5) / 2;
                            }
                            eHP(e, b_m, id, b_mid, 0, dmg);
                            *wofs << L"(T" << b_m[b_mid].time << L")" << p[i].name << L"用" << ar[ar_id].name << L"命中" << e[id].name << L"造成" << dmg << L"點傷害(" << ATK << ">" << AC << ")" << endl;
                            EndBatchDraw();
                            if (ar_id == p[i].arms_id_1 && p[i].arms_b_1 > 0) {
                                p[i].arms_b_1--;
                            }
                            else {
                                p[i].arms_b_2--;
                            }
                            Sleep(1000);
                            if (e[id].hp <= 0) {
                                te[e[id].x][e[id].y].mA -= 10000;
                                te[e[id].x][e[id].y].mB -= 10000;
                                int x = e[id].x, y = e[id].y;
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
                                te[e[id].x][e[id].y].enemy = 0;
                                e[id].x = -1; e[id].y = -1;
                                e[id].speed = -1;
                                if (e[id].buff_check[20] > 0) {
                                    p[0].buff_check[20] = 0;
                                    e[id].buff_check[20] = 0;
                                }
                                return;
                            }
                        }
                        else {
                            *wofs << L"(T" << b_m[b_mid].time << L")" << p[i].name << L"用" << ar[ar_id].name << L"攻擊" << e[id].name << L"未能命中(" << ATK << "<=" << AC << ")" << endl;
                            if (ar_id == p[i].arms_id_1 && p[i].arms_b_1 > 0) {
                                p[i].arms_b_1--;
                            }
                            else {
                                p[i].arms_b_2--;
                            }
                            mm = L"miss";
                            settextcolor(RGB(255, 0, 0));
                            settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                            outtextxy(e[id].x * 48 - b_m[b_mid].ox, e[id].y * 48 - b_m[b_mid].oy - 40, mm.c_str());
                            settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                            settextcolor(RGB(0, 0, 0));
                            Sleep(1000);
                        }
                    }


                }
            }
        }
    }
    }

}
void trade(player *p,item *it,arms *ar,t_equip *t_E,shop *sp,int &sp_id,int i_id,int ar_id,int t_Eid) {
    IMAGE get,sp_block,ip,add,reduce;
    ExMessage em;
    wstring mm;
    int J = 0,sum=0,arc=0;
    clock_t time_a=0,time_b;
    RECT t={595,0,695,90},Text={0,100,643,267}, text{652,100,1296,262};
    int choseA = 1,choseB=1,choseC=1, chosea = 1, choseb = 1, chosec = 1,choseI=-1,chosei=-1,qua=0,Qua=0,F,itema[100]={0}, itemA[100]={0},armsA[100]={0},armsa[100]={0},t_EA[100]={0},t_Ea[100]={0},a[100]={0}, A[100] = {0},b[100],B[100];
    getimage(&get, 0, 0, 1296, 960);
    loadimage(&sp_block, L"./Game/picture/shop_block.png", 0, 0, false);
    loadimage(&add, L"./Game/picture/+.png", 0, 0, false);
    loadimage(&reduce, L"./Game/picture/-.png", 0, 0, false);
    while (1) {   
        j = 0;
        J = 0;
        F = 0;
        sum = 0;
        peekmessage(&em, -1);
        if (em.lbutton||arc==0) {
            arc = 1;
        if (em.x >0&& em.x<152 && em.y>0 && em.y<89&&(clock() -time_a)>400) {
            if (choseA == 1) {
                choseA = 0;
                time_a = clock();
            }
            else {
                choseA = 1;
                Sleep(500);
                time_a = clock();
            }
            choseI = -1;
        }
        if (em.x > 160 && em.x <160+168 && em.y>0 && em.y < 91 && (clock() - time_a)>400) {
            if (choseB == 1) {
                choseB = 0;
                time_a = clock();
            }
            else {
                choseB = 1;
                Sleep(500);
                time_a = clock();
            }
            choseI = -1;
        }
        if (em.x > 338 && em.x < 338+160 && em.y>0 && em.y < 89 && (clock() - time_a)>400) {
            if (choseC == 1) {
                choseC = 0;
                time_a = clock();
            }
            else {
                choseC = 1;
                Sleep(500);
                time_a = clock();
            }
            choseI = -1;
        }
        if (em.x > 800 && em.x < 960 && em.y>0 && em.y < 90 && (clock() - time_a)>400) {
            if (chosea == 1) {
                chosea = 0;
                time_a = clock();
            }
            else {
                chosea = 1;
                Sleep(500);
                time_a = clock();
            }
            chosei = -1;
        }
        if (em.x > 970 && em.x < 970+155 && em.y>0 && em.y < 90 && (clock() - time_a)>400) {
            if (choseb == 1) {
                choseb = 0;
                time_a = clock();
            }
            else {
                choseb = 1;
                Sleep(500);
                time_a = clock();
            }
            chosei = -1;
        }
        if (em.x > 1133 && em.x < 1133+162 && em.y>0 && em.y < 90 && (clock() - time_a)>400) {
            if (chosec == 1) {
                chosec = 0;
                time_a = clock();
            }
            else {
                chosec = 1;
                Sleep(500);
                time_a = clock();
            }
            chosei = -1;
        }
        if (em.x > 707 && em.x < 707 + 86 && em.y>0 && em.y < 90 && (clock() - time_a)>400) {
            sp_id = -1;
            break;
        }
        if (choseI != -1) {
            if (em.x>550 && em.x <570&& em.y>290+choseI*40 && em.y<310+choseI*40) {
                if (A[choseI] == 1) {
                    if (itemA[B[choseI]]>0) {
                        itemA[B[choseI]]--;
                    }
                }   
                else if (A[choseI] == 2) {
                    if (armsA[B[choseI]] > 0) {
                        armsA[B[choseI]]--;
                    }
                }
                else if (A[choseI] == 3) {
                    if (t_EA[B[choseI]] > 0) {
                        t_EA[B[choseI]]--;
                    }
                }             
            }
            else if (em.x>600 && em.x <620&& em.y>290+choseI*40 && em.y<310+choseI*40) {
                                       
                if (A[choseI] == 1) {
                    if (it[B[choseI]].number - itemA[B[choseI]] > 0) {
                        itemA[B[choseI]]++;
                    }
                }
                else  if (A[choseI] ==2) {
                    if (ar[B[choseI]].number - armsA[B[choseI]] > 0) {
                        armsA[B[choseI]]++;
                    }
                }
                else  if (A[choseI] == 3) {
                    if (t_E[B[choseI]].number - t_EA[B[choseI]] > 0) {
                        t_EA[B[choseI]]++;
                    }
                }
            }
        }
        if (chosei != -1) {
            if (em.x>1206&& em.x<1226 && em.y>290+chosei*40 &&em.y<310+chosei*40) {
                if (a[chosei] == 1) {
                    if (sp[sp_id].i_number[b[chosei]]-itema[b[chosei]] > 0) {
                        itema[b[chosei]]++;
                    }
                }
                else if (a[chosei] == 2) {
                    if (sp[sp_id].ar_number[b[chosei]]-armsa[b[chosei]] > 0) {
                        armsa[b[chosei]]++;
                    }
                }
                else if (a[chosei] == 3) {
                    if (sp[sp_id].t_E_number[b[chosei]]-t_Ea[b[chosei]] > 0) {
                        t_Ea[b[chosei]]++;
                    }
                }             
            }
            else if (em.x >1256&& em.x<1276 && em.y>290+chosei*40 && em.y<310+chosei*40) {
                if (a[chosei] == 1) {
                    if (itema[b[chosei]] > 0) {
                        itema[b[chosei]]--;
                    }
                }
                else if (a[chosei] == 2) {
                    if (armsa[b[chosei]] > 0) {
                        armsa[b[chosei]]--;
                    }
                }
                else if (a[chosei] == 3) {
                    if (t_Ea[b[chosei]] > 0) {
                        t_Ea[b[chosei]]--;
                    }
                }
            }
        }
        BeginBatchDraw();
        putimage(0, 0, &get);
        transparentimage(NULL, 0, 0, &sp_block);
        if (choseA == 1) {
            rectangle(10, 10, 142, 79);
            for (i = 0; i < i_id; i++) {
                if (it[i].number != 0 && it[i].type == 'e') {
                    mm = L"./Game/picture/item" + to_wstring(i) + L".png";
                    loadimage(&ip, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 15, 290 + j * 40, &ip, 0xFF55FF);
                    outtextxy(40, 290 + j * 40, it[i].Name.c_str());
                    outtextxy(375, 290 + j * 40, _T(":"));
                    outtextxy(385, 290 + j * 40, to_wstring(it[i].number-itemA[i]).c_str());
                    A[j] = 1;
                    B[j] = i;
                    j++;
                }
            }
            for (i = 0; i < i_id; i++) {
                if (it[i].number != 0 && it[i].type == 'b') {
                    mm = L"./Game/picture/item" + to_wstring(i) + L".png";
                    loadimage(&ip, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 15, 290 + j * 40, &ip, 0xFF55FF);
                    outtextxy(40, 290 + j * 40, it[i].Name.c_str());
                    outtextxy(375, 290 + j * 40, _T(":"));
                    outtextxy(385, 290 + j * 40, to_wstring(it[i].number - itemA[i]).c_str());
                    A[j] = 1;
                    B[j] = i;
                    j++;
                }
            }
            for (i = 0; i < i_id; i++) {
                if (it[i].number != 0 && it[i].type == 'h') {
                    mm = L"./Game/picture/item" + to_wstring(i) + L".png";
                    loadimage(&ip, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 15, 290 + j * 40, &ip, 0xFF55FF);
                    outtextxy(40, 290 + j * 40, it[i].Name.c_str());
                    outtextxy(375, 290 + j * 40, _T(":"));
                    outtextxy(385, 290 + j * 40, to_wstring(it[i].number - itemA[i]).c_str());
                    A[j] = 1;
                    B[j] = i;
                    j++;
                }
            }
        }
        if (choseB == 1) {
            rectangle(170, 10, 160 + 158, 81);
            for (i = 0; i < ar_id; i++) {
                if (ar[i].number != 0) {
                    mm = L"./Game/picture/arms" + to_wstring(i) + L".png";
                    loadimage(&ip, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 15, 290 + j * 40, &ip, 0xFF55FF);
                    outtextxy(40, 290 + j * 40, ar[i].name.c_str());
                    outtextxy(375, 290 + j * 40, _T(":"));
                    outtextxy(385, 290 + j * 40, to_wstring(ar[i].number - armsA[i]).c_str());
                    A[j] = 2;
                    B[j] = i;
                    j++;
                }
            }
            for (i = 0; i < t_Eid; i++) {
                if (t_E[i].number != 0) {
                    mm = L"./Game/picture/te" + to_wstring(i) + L".png";
                    loadimage(&ip, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 15, 290 + j * 40, &ip, 0xFF55FF);
                    outtextxy(40, 290 + j * 40, t_E[i].name.c_str());
                    outtextxy(375, 290 + j * 40, _T(":"));
                    outtextxy(385, 290 + j * 40, to_wstring(t_E[i].number - t_EA[i]).c_str());
                    A[j] = 3;
                    B[j] = i;
                    j++;
                }
            }
        }
        if (choseC == 1) {
            rectangle(348, 10, 338 + 150, 79);
            for (i = 0; i < i_id; i++) {
                if (it[i].number != 0 && it[i].type == 'm') {
                    mm = L"./Game/picture/item" + to_wstring(i) + L".png";
                    loadimage(&ip, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 15, 290 + j * 40, &ip, 0xFF55FF);
                    outtextxy(40, 290 + j * 40, it[i].Name.c_str());
                    outtextxy(375, 290 + j * 40, _T(":"));
                    outtextxy(385, 290 + j * 40, to_wstring(it[i].number - itemA[i]).c_str());
                    A[j] = 1;
                    B[j] = i;
                    j++;
                }
            }
        }
        if (chosea == 1) {
            rectangle(810, 10, 950, 80);
            for (i = 0; i < i_id; i++) {
                if (sp[sp_id].i_number[i] != 0 && it[i].type == 'e') {
                    mm = L"./Game/picture/item" + to_wstring(i) + L".png";
                    loadimage(&ip, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 15 + 655, 290 + J * 40, &ip, 0xFF55FF);
                    outtextxy(40 + 655, 290 + J * 40, it[i].Name.c_str());
                    outtextxy(375 + 655, 290 + J * 40, _T(":"));
                    outtextxy(385 + 655, 290 + J * 40, to_wstring(sp[sp_id].i_number[i]-itema[i]).c_str());
                    a[J] = 1;
                    b[J] = i;
                    J++;
                }
            }
            for (i = 0; i < i_id; i++) {
                if (sp[sp_id].i_number[i] != 0 && it[i].type == 'b') {
                    mm = L"./Game/picture/item" + to_wstring(i) + L".png";
                    loadimage(&ip, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 15 + 655, 290 + J * 40, &ip, 0xFF55FF);
                    outtextxy(40 + 655, 290 + J * 40, it[i].Name.c_str());
                    outtextxy(375 + 655, 290 + J * 40, _T(":"));
                    outtextxy(385 + 655, 290 + J * 40, to_wstring(sp[sp_id].i_number[i] - itema[i]).c_str());
                    a[J] = 1;
                    b[J] = i;
                    J++;
                }
            }
            for (i = 0; i < i_id; i++) {
                if (sp[sp_id].i_number[i] != 0 && it[i].type == 'h') {
                    mm = L"./Game/picture/item" + to_wstring(i) + L".png";
                    loadimage(&ip, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 15 + 655, 290 + J * 40, &ip, 0xFF55FF);
                    outtextxy(40 + 655, 290 + J * 40, it[i].Name.c_str());
                    outtextxy(375 + 655, 290 + J * 40, _T(":"));
                    outtextxy(385 + 655, 290 + J * 40, to_wstring(sp[sp_id].i_number[i] - itema[i]).c_str());
                    a[J] = 1;
                    b[J] = i;
                    J++;
                }
            }
        }
        if (choseb == 1) {
            rectangle(980, 10, 970 + 145, 80);
            for (i = 0; i < ar_id; i++) {
                if (sp[sp_id].ar_number[i] != 0) {
                    mm = L"./Game/picture/arms" + to_wstring(i) + L".png";
                    loadimage(&ip, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 15 + 655, 290 + J * 40, &ip, 0xFF55FF);
                    outtextxy(40 + 655, 290 + J * 40, ar[i].name.c_str());
                    outtextxy(375 + 655, 290 + J * 40, _T(":"));
                    outtextxy(385 + 655, 290 + J * 40, to_wstring(sp[sp_id].ar_number[i] - armsa[i]).c_str());
                    a[J] = 2;
                    b[J] = i;
                    J++;
                }
            }
            for (i = 0; i < t_Eid; i++) {
                if (sp[sp_id].t_E_number[i] != 0) {
                    mm = L"./Game/picture/te" + to_wstring(i) + L".png";
                    loadimage(&ip, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 15 + 655, 290 + J * 40, &ip, 0xFF55FF);
                    outtextxy(40 + 655, 290 + J * 40, t_E[i].name.c_str());
                    outtextxy(375 + 655, 290 + J * 40, _T(":"));
                    outtextxy(385 + 655, 290 + J * 40, to_wstring(sp[sp_id].t_E_number[i] - t_Ea[i]).c_str());
                    a[J] = 3;
                    b[J] = i;
                    J++;
                }
            }
        }
        if (chosec == 1) {
            rectangle(1143, 10, 1133 + 152, 80);
            for (i = 0; i < i_id; i++) {
                if (sp[sp_id].i_number[i] != 0 && it[i].type == 'm') {
                    mm = L"./Game/picture/item" + to_wstring(i) + L".png";
                    loadimage(&ip, mm.c_str(), 0, 0, false);
                    transparentimage(NULL, 15 + 655, 290 + J * 40, &ip, 0xFF55FF);
                    outtextxy(40 + 655, 290 + J * 40, it[i].Name.c_str());
                    outtextxy(375 + 655, 290 + J * 40, _T(":"));
                    outtextxy(385 + 655, 290 + J * 40, to_wstring(sp[sp_id].i_number[i] - itema[i]).c_str());
                    a[J] = 1;
                    b[J] = i;
                    J++;
                }
            }
        }    
        if (em.x < 640 && em.y>275) {
        for (i = 0; i < j; i++) {
            if (em.x >0&& em.x<640&&em.y>275+i*40&&em.y<315+i*40) {
                choseI = i;
            }
        }
        }
        else if (em.x>655&&em.y > 275) {
            for (i = 0; i < J; i++) {
                if (em.x > 655 && em.x < 1296 && em.y>275 + i * 40 && em.y <315 + i * 40) {
                    chosei = i;
                }
            }
        }
        if (choseI != -1) {
            rectangle(10, 285+choseI*40, 630, 325+choseI*40);
            transparentimage(NULL, 550, 290 + choseI * 40, &add, 0xFF55FF);
            transparentimage(NULL, 600, 290 + choseI * 40, &reduce, 0xFF55FF);
        }
        if (chosei != -1) {
            rectangle(665, 285 + chosei * 40, 1286, 325 + chosei * 40);
            transparentimage(NULL, 1206, 290 + chosei * 40, &add, 0xFF55FF);
            transparentimage(NULL, 1256, 290 + chosei * 40, &reduce, 0xFF55FF);
        }
        for (i = 0; i < i_id; i++) {         
            if (sp[sp_id].type == "a") {
                if (i == 15) {
                    sum += itemA[i] * it[i].value*2;
                }
                else { 
                    sum += itemA[i] * it[i].value;                 
                }
            }
            else if (sp[sp_id].type == "") {
                sum += itemA[i] * it[i].value;
            }       
            if (sp[sp_id].type == "a") {
                if (i == 15) {
                sum -= itema[i] * it[i].value*2;
                }
                else {
                    sum -= itema[i] * it[i].value * 1.1;
                }
            }
            else {
                sum -= itema[i] * it[i].value*1.1;
            }
        }
        for (i = 0; i < ar_id; i++) {
            if (sp[sp_id].type == "a") {
                sum += armsA[i] * ar[i].value;
            }
            else  {
                sum += armsA[i] * ar[i].value;
            }
           
            if (sp[sp_id].type == "") {
             sum -= armsa[i] * ar[i].value*1.1;
            }
            else {
                sum -= armsa[i] * ar[i].value * 1.1;
            }
           
        }
        for (i = 0; i < t_Eid; i++) {
            if (sp[sp_id].type == "a") {
            sum += t_EA[i]*t_E[i].value;
            }
            else  {
                sum += t_EA[i] * t_E[i].value;
            }
            if (sp[sp_id].type == "") {
               sum -= t_Ea[i] * t_E[i].value * 1.1;
            }
            else {
                sum -= t_Ea[i] * t_E[i].value * 1.1;
            }
         
        }            
        if (A[choseI] == 1) {
            drawtext(it[B[choseI]].story.c_str(), &Text, DT_WORDBREAK);
        }
        else if (A[choseI] == 2) {
            drawtext(ar[B[choseI]].story.c_str(), &Text, DT_WORDBREAK);
        }
        else if (A[choseI] == 3) {
            drawtext(t_E[B[choseI]].story.c_str(), &Text, DT_WORDBREAK);
        }
        if (a[chosei] == 1) {
            drawtext(it[b[chosei]].story.c_str(), &text, DT_WORDBREAK);
        }
        else if (a[chosei] == 2) {
            drawtext(ar[b[chosei]].story.c_str(), &text, DT_WORDBREAK);
        }
        else if (a[chosei] == 3) {
            drawtext(t_E[b[chosei]].story.c_str(), &text, DT_WORDBREAK);
        }
        if (em.x > 505 && em.x < 587 && em.y>0 && em.y < 90 && (clock() - time_a)>400) {
            if (sum >= 0) {
                for (i = 0; i < i_id; i++) {
                    it[i].number -= itemA[i];
                    sp[sp_id].i_number[i] += itemA[i];
                    sp[sp_id].i_number[i] -= itema[i];
                    it[i].number += itema[i];
                }
                for (i = 0; i < ar_id; i++) {
                    ar[i].number -= armsA[i];
                    sp[sp_id].ar_number[i] += armsA[i];
                    sp[sp_id].ar_number[i] -= armsa[i];
                    ar[i].number += armsa[i];
                }
                for (i = 0; i < t_Eid; i++) {
                    t_E[i].number -= t_EA[i];
                    sp[sp_id].t_E_number[i] += t_EA[i];
                    sp[sp_id].t_E_number[i] -= t_Ea[i];
                    t_E[i].number += t_Ea[i];
                }
                sp_id = -1;
                break;
            }
        }
        drawtext(to_wstring(sum).c_str(), &t, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        EndBatchDraw();
        }  
        flushmessage(-1);
        em = getmessage(EM_MOUSE);
    }
}
void e_battle_ai(wofstream* wofs, enemy *e,player *p,e_npc *e_n,b_npc *b_n,b_map*b_m, enemy_type *e_t,arms *ar,armor *Ar,terrain(*te)[50],b_flag *b_f,int id,int P_id,int b_nid,int b_mid,int buff_id) {
    int type=0,ro=0;
    wstring mm;
    if (e[id].type == 0) {
        e_target(wofs, e, p, b_m, b_n,te, P_id, id, b_mid, b_m[b_mid].psize, b_m[b_mid].nsize, b_nid,0);
        e_walk(wofs, e, p, b_n, b_m,ar, te, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id, b_nid, e[id].target,0);
        e_attack(wofs, ar, Ar,p, e, b_n, b_m,te, e_t,b_mid, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, buff_id, b_nid);
    }
    else if (e[id].type == 1) {
        e_target(wofs, e, p, b_m, b_n, te, P_id, id, b_mid, b_m[b_mid].psize, b_m[b_mid].nsize, b_nid, 0);
        if (e[id].b_num_1 > 0) {
        e_walk(wofs, e, p, b_n, b_m, ar, te, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id, b_nid, e[id].target, 0);       
        e_attack(wofs, ar, Ar, p, e, b_n, b_m, te, e_t, b_mid, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, buff_id, b_nid);
        }
        else if (e[id].b_num_2 > 0) {
            e_walk(wofs, e, p, b_n, b_m, ar, te, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id, b_nid, e[id].target, 0);
            if (e[id].target == -1) {
                for (int I = -2; I <= 2; I++) {
                    for (int J = -2; J <= 2; J++) {
                        if (abs(I) != abs(J)) {
                            for (int K = 0; K < b_m[b_mid].psize; K++) {
                                if (p[K].x == e[id].x + I && p[K].y == e[id].y + J) {
                                    P_id = K;
                                    e[id].target = 0;
                                    I = 3;
                                    J = 3;
                                    K = b_m[b_mid].psize;
                                }
                            }
                            for (int K = 0; K < b_m[b_mid].nsize; K++) {
                                if (b_n[K].x == e[id].x + I && b_n[K].y == e[id].y + J) {
                                    b_nid = K;
                                    e[id].target = 1;
                                    I = 3;
                                    J = 3;
                                    K = b_m[b_mid].nsize;
                                }
                            }
                        }
                    }
                }
            }
            if (e[id].target == 0) {
            for (int I = -2; I <= 2;I++) {
                for (int J = -2; J <= 2; J++) {
                    if (abs(I) != abs(J)) {
                        if (e[id].x + I == p[P_id].x && e[id].y + J == p[P_id].y) {
                            skill_show(L"霰彈射擊");
                            if (I == 2||(I==1&&J==0)) {
                                e[id].pose = 3;
                                for (int K = 0; K < b_m[b_mid].psize; K++) {
                                    if ((p[K].x == e[id].x + 2 && (p[K].y==e[id].y+1|| p[K].y == e[id].y|| p[K].y == e[id].y - 1))||(p[K].x==e[id].x+1&&p[K].y==e[id].y)) {
                                        int DMG = roll("2d4", 1);
                                        pHP(p, b_m, K, b_mid, 0, DMG);
                                    }
                                }
                                for (int K = 0; K < b_m[b_mid].nsize; K++) {
                                    if ((b_n[K].x == e[id].x + 2 && (b_n[K].y == e[id].y + 1 || b_n[K].y == e[id].y || b_n[K].y == e[id].y - 1)) || (b_n[K].x == e[id].x + 1 && b_n[K].y == e[id].y)) {
                                        int DMG = roll("2d4", 1);
                                        nHP(b_n, b_m, K, b_mid, 0, DMG);
                                    }
                                }
                            }
                            else if (I == -2||(I==-1&&J==0)) {
                                e[id].pose = 2;
                                for (int K = 0; K < b_m[b_mid].psize; K++) {
                                    if ((p[K].x == e[id].x - 2 && (p[K].y == e[id].y + 1 || p[K].y == e[id].y || p[K].y == e[id].y - 1)) || (p[K].x == e[id].x - 1 && p[K].y == e[id].y)) {
                                        int DMG = roll("2d4", 1);
                                        pHP(p, b_m, K, b_mid, 0, DMG);
                                    }
                                }
                                for (int K = 0; K < b_m[b_mid].nsize; K++) {
                                    if ((b_n[K].x == e[id].x - 2 && (b_n[K].y == e[id].y + 1 || b_n[K].y == e[id].y || b_n[K].y == e[id].y - 1)) || (b_n[K].x == e[id].x - 1 && b_n[K].y == e[id].y)) {
                                        int DMG = roll("2d4", 1);
                                        nHP(b_n, b_m, K, b_mid, 0, DMG);
                                    }
                                }
                            }
                            else if (J == 2||(I==0&&J==1)) {
                                e[id].pose = 1;
                                for (int K = 0; K < b_m[b_mid].psize; K++) {
                                    if ((p[K].y == e[id].y + 2 && (p[K].x == e[id].x + 1 || p[K].x == e[id].x || p[K].x == e[id].x - 1)) || (p[K].y == e[id].y + 1 && p[K].x == e[id].x)) {
                                        int DMG = roll("2d4", 1);
                                        pHP(p, b_m, K, b_mid, 0, DMG);
                                    }
                                }
                                for (int K = 0; K < b_m[b_mid].nsize; K++) {
                                    if ((b_n[K].y == e[id].y + 2 && (b_n[K].x == e[id].x + 1 || b_n[K].x == e[id].x || b_n[K].x == e[id].x - 1)) || (b_n[K].y == e[id].y + 1 && b_n[K].x == e[id].x)) {
                                        int DMG = roll("2d4", 1);
                                        nHP(b_n, b_m, K, b_mid, 0, DMG);
                                    }
                                }
                            }
                            else if (J == -2||(I==0&&J==-1)) {
                                e[id].pose = 4;
                                for (int K = 0; K < b_m[b_mid].psize; K++) {
                                    if ((p[K].y == e[id].y - 2 && (p[K].x == e[id].x + 1 || p[K].x == e[id].x || p[K].x == e[id].x - 1)) || (p[K].y == e[id].y - 1 && p[K].x == e[id].x)) {
                                        int DMG = roll("2d4", 1);
                                        pHP(p, b_m, K, b_mid, 0, DMG);
                                    }
                                }
                                for (int K = 0; K < b_m[b_mid].nsize; K++) {
                                    if ((b_n[K].y == e[id].y - 2 && (b_n[K].x == e[id].x + 1 || b_n[K].x == e[id].x || b_n[K].x == e[id].x - 1)) || (b_n[K].y == e[id].y - 1 && b_n[K].x == e[id].x)) {
                                        int DMG = roll("2d4", 1);
                                        nHP(b_n, b_m, K, b_mid, 0, DMG);
                                    }
                                }
                            }
                            FlushBatchDraw();
                            Sleep(1000);
                            e[id].b_num_2--;
                        }
                    }
                }
            }
            }
            else if (e[id].target == 1) {
                for (int I = -2; I <= 2; I++) {
                    for (int J = -2; J <= 2; J++) {
                        if (abs(I) != abs(J)) {
                            if (e[id].x + I == b_n[b_mid].x && e[id].y + J == b_n[b_nid].y) {
                                skill_show(L"霰彈射擊");
                                if (I == 2 || (I == 1 && J == 0)) {
                                    e[id].pose = 3;
                                    for (int K = 0; K < b_m[b_mid].psize; K++) {
                                        if ((p[K].x == e[id].x + 2 && (p[K].y == e[id].y + 1 || p[K].y == e[id].y || p[K].y == e[id].y - 1)) || (p[K].x == e[id].x + 1 && p[K].y == e[id].y)) {
                                            int DMG = roll("2d4", 1);
                                            pHP(p, b_m, K, b_mid, 0, DMG);
                                        }
                                    }
                                    for (int K = 0; K < b_m[b_mid].nsize; K++) {
                                        if ((b_n[K].x == e[id].x + 2 && (b_n[K].y == e[id].y + 1 || b_n[K].y == e[id].y || b_n[K].y == e[id].y - 1)) || (b_n[K].x == e[id].x + 1 && b_n[K].y == e[id].y)) {
                                            int DMG = roll("2d4", 1);
                                            nHP(b_n, b_m, K, b_mid, 0, DMG);
                                        }
                                    }
                                }
                                else if (I == -2 || (I == -1 && J == 0)) {
                                    e[id].pose = 2;
                                    for (int K = 0; K < b_m[b_mid].psize; K++) {
                                        if ((p[K].x == e[id].x - 2 && (p[K].y == e[id].y + 1 || p[K].y == e[id].y || p[K].y == e[id].y - 1)) || (p[K].x == e[id].x - 1 && p[K].y == e[id].y)) {
                                            int DMG = roll("2d4", 1);
                                            pHP(p, b_m, K, b_mid, 0, DMG);
                                        }
                                    }
                                    for (int K = 0; K < b_m[b_mid].nsize; K++) {
                                        if ((b_n[K].x == e[id].x - 2 && (b_n[K].y == e[id].y + 1 || b_n[K].y == e[id].y || b_n[K].y == e[id].y - 1)) || (b_n[K].x == e[id].x - 1 && b_n[K].y == e[id].y)) {
                                            int DMG = roll("2d4", 1);
                                            nHP(b_n, b_m, K, b_mid, 0, DMG);
                                        }
                                    }
                                }
                                else if (J == 2 || (I == 0 && J == 1)) {
                                    e[id].pose = 1;
                                    for (int K = 0; K < b_m[b_mid].psize; K++) {
                                        if ((p[K].y == e[id].y + 2 && (p[K].x == e[id].x + 1 || p[K].x == e[id].x || p[K].x == e[id].x - 1)) || (p[K].y == e[id].y + 1 && p[K].x == e[id].x)) {
                                            int DMG = roll("2d4", 1);
                                            pHP(p, b_m, K, b_mid, 0, DMG);
                                        }
                                    }
                                    for (int K = 0; K < b_m[b_mid].nsize; K++) {
                                        if ((b_n[K].y == e[id].y + 2 && (b_n[K].x == e[id].x + 1 || b_n[K].x == e[id].x || b_n[K].x == e[id].x - 1)) || (b_n[K].y == e[id].y + 1 && b_n[K].x == e[id].x)) {
                                            int DMG = roll("2d4", 1);
                                            nHP(b_n, b_m, K, b_mid, 0, DMG);
                                        }
                                    }
                                }
                                else if (J == -2 || (I == 0 && J == -1)) {
                                    e[id].pose = 4;
                                    for (int K = 0; K < b_m[b_mid].psize; K++) {
                                        if ((p[K].y == e[id].y - 2 && (p[K].x == e[id].x + 1 || p[K].x == e[id].x || p[K].x == e[id].x - 1)) || (p[K].y == e[id].y - 1 && p[K].x == e[id].x)) {
                                            int DMG = roll("2d4", 1);
                                            pHP(p, b_m, K, b_mid, 0, DMG);
                                        }
                                    }
                                    for (int K = 0; K < b_m[b_mid].nsize; K++) {
                                        if ((b_n[K].y == e[id].y - 2 && (b_n[K].x == e[id].x + 1 || b_n[K].x == e[id].x || b_n[K].x == e[id].x - 1)) || (b_n[K].y == e[id].y - 1 && b_n[K].x == e[id].x)) {
                                            int DMG = roll("2d4", 1);
                                            nHP(b_n, b_m, K, b_mid, 0, DMG);
                                        }
                                    }
                                }
                                FlushBatchDraw();
                                Sleep(1000);
                                e[id].b_num_2--;
                            }
                        }
                    }
                }
            }

        }
        else {
            if (e[id].target == -1) {
                e_walk(wofs, e, p, b_n, b_m, ar, te, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id, b_nid, e[id].target, 0);      
                skill_show(L"裝彈");
                e[id].buff_check[5] = 1;
                e[id].buff_id[e[id].buff_Size] = 5;
                e[id].buff_time[e[id].buff_Size] = 5;
                e[id].buff_Size++;
            }
            else{
                e_walk(wofs, e, p, b_n, b_m, ar, te, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id, b_nid, e[id].target, 0);
                if (e[id].target==0) {
                    if (abs(e[id].x - p[P_id].x) + abs(e[id].y - p[P_id].y) == 1) {
                    skill_show(L"刺刀突擊");
                    *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name<<L"對"<<p[P_id].name << L"發起刺刀突擊";
                    if (p[P_id].buff_check[2] > 0) {
                       ro=EKTVS(p, e, b_m, ar, e_t, Ar, P_id, id, b_mid, NULL,1,L"1d4");        
                    }
                    if (retrye(wofs, p, e, ar, Ar, P_id, id, ro, 1) > 0) {
                    int dmg = roll("1d4", 1);
                    pHP(p, b_m, P_id, b_mid, 0, dmg);
                    }
                    else {
                        LOGFONT f;
                        settextstyle(25, 0, _T("Taipei Sans TC Beta"));
                        settextcolor(RGB(255, 0, 0));
                        mm = L"miss";
                        outtextxy(e[id].x * 48 - b_m[b_mid].ox, e[id].y * 48 - b_m[b_mid].oy - 40, mm.c_str());
                        settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                        settextcolor(RGB(0, 0, 0));
                    }
                    }
                }
                else if (e[id].target == 1) {
                    if (abs(e[id].x - b_n[b_nid].x) + abs(e[id].y - b_n[b_nid].y) == 1) {
                        skill_show(L"刺刀突擊");               
                        int dmg = roll("1d4", 1);
                        nHP(b_n, b_m, b_nid, b_mid, 0, dmg);
                    }
                }
                else {
                    skill_show(L"裝彈");
                    e[id].buff_check[5] = 1;
                    e[id].buff_id[e[id].buff_Size] = 5;
                    e[id].buff_time[ e[id].buff_Size] = 5;
                    e[id].buff_Size++;
                }
                FlushBatchDraw();
                    Sleep(1000);
            }
        }
    }
    else if (e[id].type == 2) {
        e_target(wofs, e, p, b_m, b_n, te, P_id, id, b_mid, b_m[b_mid].psize, b_m[b_mid].nsize, b_nid, 0);
        e_walk(wofs, e, p, b_n, b_m, ar, te, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id, b_nid, e[id].target, 0);
        e_attack(wofs, ar, Ar, p, e, b_n, b_m, te, e_t, b_mid, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, buff_id, b_nid);
    }
    else if (e[id].type == 3) {
        for (i = 0; i < b_m[b_mid].fsize; i++) {
            if (b_f[i].id == 4 && b_f[i].check == 0) {
                type = 1;
            }
        }
        if (type == 0) {
        if (e[id].cd[0] == 0) {
            e_target(wofs, e, p, b_m, b_n, te, P_id, id, b_mid, b_m[b_mid].psize, b_m[b_mid].nsize, b_nid,1);
            if (e[id].target == 0) {
            e[id].target_x = p[P_id].x;
            e[id].target_y = p[P_id].y;
            }
            else if (e[id].target == 1) {
                e[id].target_x = b_n[b_nid].x;
                e[id].target_y = b_n[b_nid].y;
            }
            e_walk(wofs, e, p, b_n, b_m,ar, te, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id, b_nid, e[id].target,1);            
            if (abs(e[id].target_x - e[id].x) + abs(e[id].target_y - e[id].y) < 5) {
            e[id].buff_check[13] = 1;
            e[id].buff_time[e[id].buff_Size] = 8;
            e[id].buff_id[e[id].buff_Size] = 13;
            e[id].buff_Size++;            
            e[id].cd[0] = 30;
            }
        }
        else {
        e_target(wofs, e, p, b_m, b_n, te, P_id, id, b_mid, b_m[b_mid].psize, b_m[b_mid].nsize, b_nid,1);
        e_walk(wofs, e, p, b_n, b_m, ar,te, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id, b_nid, e[id].target,0);
        e_attack(wofs, ar, Ar, p, e, b_n, b_m,te, e_t, b_mid, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, buff_id, b_nid);
        }
        }
        else if(type==1) {
            if (e[id].cd[1] == 0 && e[id].hp < 25) {
                e_target(wofs, e, p, b_m, b_n, te, P_id, id, b_mid, b_m[b_mid].psize, b_m[b_mid].nsize, b_nid, 2);
                e_walk(wofs, e, p, b_n, b_m, ar, te, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id, b_nid, e[id].target, 0);
                if (abs(p[P_id].x - e[id].x) + abs(p[P_id].y - e[id].y) <= 5) {
                    e[id].buff_check[15] = 1;
                    e[id].buff_time[e[id].buff_Size] = 6;
                    e[id].buff_id[e[id].buff_Size] = 15;
                    e[id].buff_Size++;
                    e[id].cd[1] = 20;
                }
            }
            else if (e[id].cd[0] == 0) {
                e_target(wofs, e, p, b_m, b_n, te, P_id, id, b_mid, b_m[b_mid].psize, b_m[b_mid].nsize, b_nid,2);
                e[id].target_x = p[P_id].x;
                e[id].target_y = p[P_id].y;
                e_walk(wofs, e, p, b_n, b_m, ar, te, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id, b_nid, e[id].target, 1);
                if (abs(e[id].target_x - e[id].x) + abs(e[id].target_y - e[id].y) < 5) {
                    e[id].buff_check[13] = 1;
                    e[id].buff_time[e[id].buff_Size] = 8;
                    e[id].buff_id[e[id].buff_Size] = 13;
                    e[id].buff_Size++;
                    e[id].cd[0] = 30;
                }
            }            
            else {
                e_target(wofs, e, p, b_m, b_n, te, P_id, id, b_mid, b_m[b_mid].psize, b_m[b_mid].nsize, b_nid,2);
                e_walk(wofs, e, p, b_n, b_m, ar, te, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id, b_nid, e[id].target, 0);
                e_attack(wofs, ar, Ar, p, e, b_n, b_m, te, e_t, b_mid, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, buff_id, b_nid);
            }
        }
    }
    else if (e[id].type==4) {
        if (e[id].cd[0] == 0) {
            if (e[id].hp > 0) {
            if (te[0][11].mB < 500) {
            enemy_type_get(e, e_t, te,b_m,b_mid, 0, 0, 11,3);  
            e[id].cd[0] = 30;
            wstring sPath = L"open ./Game/Sound/SE/eSe0.mp3 alias woo1";
            mciSendString(sPath.c_str(), NULL, 0, NULL);
            wstring vos = L"setaudio woo1 volume to " + to_wstring(soundSize);
            mciSendString(vos.c_str(), NULL, 0, NULL);
            mciSendString(L"play woo1 from 0", NULL, 0, NULL);
            e_put(e, b_m, te, b_m[b_mid].esize, b_mid);
            *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"呼喚了" << e[b_m[b_mid].esize - 1].name << endl;
            skill_show(L"狼群呼喚");
            }
            else if (te[19][12].mB < 500) {
                enemy_type_get(e, e_t,te, b_m,b_mid, 0, 19, 12,2);
                e[id].cd[0] = 30;
                wstring sPath = L"open ./Game/Sound/SE/eSe0.mp3 alias woo2";
                mciSendString(sPath.c_str(), NULL, 0, NULL);
                wstring vos = L"setaudio woo2 volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play woo2 from 0", NULL, 0, NULL);
                e_put(e, b_m, te, b_m[b_mid].esize, b_mid);
                *wofs << L"(T" << b_m[b_mid].time << L")" << e[id].name << L"呼喚了" << e[b_m[b_mid].esize - 1].name << endl;
                skill_show(L"狼群呼喚");
            }
            }
        }
        else {
            e_target(wofs, e, p, b_m, b_n, te, P_id, id, b_mid, b_m[b_mid].psize, b_m[b_mid].nsize, b_nid,0);
            e_walk(wofs, e, p, b_n, b_m,ar, te, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id, b_nid, e[id].target,0);
            e_attack(wofs, ar, Ar, p, e, b_n, b_m,te, e_t, b_mid, id, P_id, b_m[b_mid].esize, b_m[b_mid].psize, buff_id, b_nid);
        }
    }
}
int lose_check(wofstream* wofs, enemy* e, player* p, e_npc* e_n, b_npc* b_n, b_map* b_m, enemy_type* e_t, terrain(*te)[50], b_flag* b_f,int b_mid ) {
    if (b_mid == 4) {
        if (b_n[0].hp <= 0) {
            return 1;
        }
        for (i = 0; i < b_m[b_mid].psize; i++) {
            if (p[i].hp > 0) {
                break;
            }
        }
        if (i == b_m[b_mid].psize) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
    for (i = 0; i < b_m[b_mid].psize; i++) {
        if (p[i].hp > 0) {
            break;
        }
    }
    if (i == b_m[b_mid].psize) {
        return 1;
    }
    else {
        return 0;
    }
    }

}
int main() {
    HWND hwnd = initgraph(1296, 960);   
    g_OriginWndProc = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
    SetWindowPos(hwnd, HWND_TOP, 312, 30, 1296, 1000, 0);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Taipei Sans TC Beta"));
    SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    HCURSOR hcur = (HCURSOR)LoadImage(NULL, _T("./Game/picture/mouse.cur"), IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
    SetClassLongPtr(hwnd, GCLP_HCURSOR, (long)hcur);
    SetWindowText(hwnd, L"隕星傳奇");
    bc_l = FindWindow(NULL,L"隕星傳奇");
    SystemParametersInfo(SPI_GETMOUSESPEED, 0, &oms, 0);
    thread Back_l(back_listen);
    int  variable = 1, id = 0, P_id = 0, m_id = 1, psize = 1, load = 0, n_id = 1, i_id = 18, ar_id = 11, Ar_id = 3, st_id = 3, f_id = 22, b_mid, ex_id = 14, b_id = 9, sk_id = 22, buff_id = 34, m_fid = 11, b_nid = 0, t_Eid = 3, sp_id = -1,tk_id=6;
    /*變數數量*/    
    wofstream wofs;
    player p[3];
    enemy_type e_t[7];
    enemy e[10];
    arms  ar[11];
    armor Ar[3];
    item  it[18];
    Map   m[10];
    npc  n[100];
    stone st[3];
    flag f[100];
    e_npc e_n[100];
    b_map b_m[5];
    Exit EX[14];
    m_flag m_f[100];
    BOX Box[10];
    b_flag b_f[100];
    skill sk[100];
    buff bu[100];
    b_npc b_n[10];
    b_npc_type b_nt[10];
    terrain te[50][50];
    t_equip t_E[3];
    shop sp[2];
    task tk[10];
    int ix, iy, abox1[1];
    int  t, roundp = 0, roundb = 0, roundn = 0;
    string  chose = "l";
    wstring vos;
    string a, b = ".txt", read = "";
    time_t first = 0, two = 0, three = 0;
    IMAGE get;
    if (variable == 1) {
    m[2].psize = 1; m[2].nsize = 1; m[2].esize = 0; m[2].exitsize = 3; m[2].box_size = 1; m[2].exit_set = "1_2_3_"; m[2].npcid = "3-"; m[2].mevent_size = 1; m[2].mevent_set = "1_"; m[2].box_set = "1n"; m[2].name = L"艾倫家";
    m[1].psize = 1; m[1].nsize = 0; m[1].esize = 0; m[1].exitsize = 1; m[1].box_size = 1; m[1].exit_set = "0_"; m[1].mevent_size = 1; m[1].mevent_set = "0_"; m[1].box_set = "0n"; m[1].name = L"艾倫臥室";
    m[0].x = 27; m[0].y = 20; m[0].psize = 1; m[0].nsize = 1; m[0].esize = 3; m[0].npcid = "1-";  m[0].b_set = "b0b0b0z"; m[0].e_set = "x10y13zx5y5zx17y17z"; m[0].block = "x2y4";
    m[3].psize = 1; m[3].nsize = 0; m[3].esize = 0; m[3].exitsize = 1; m[3].exit_set = "5_"; m[3].name = L"艾倫家飯廳";
    m[4].psize = 1; m[4].nsize = 1; m[4].npcid = "5-"; m[4].esize = 0; m[4].exitsize = 4; m[4].exit_set = "4_6_8_12_"; m[4].mevent_size = 2; m[4].mevent_set = "2_3_"; m[4].name = L"星隕村";
    m[5].nsize = 0; m[5].esize = 2; m[5].b_set = "b0zb0z"; m[5].e_set = "x12y9k0x15y16k0"; m[5].exitsize = 2; m[5].exit_set = "7_10_"; m[5].mevent_size = 1; m[5].mevent_set = "10_"; m[5].name = L"夏洛特家外";
    m[6].nsize = 1; m[6].npcid = "7-"; m[6].esize = 0; m[6].exitsize = 1; m[6].exit_set = "9_"; m[6].mevent_size = 1; m[6].mevent_set = "4_"; m[6].name = L"星隕村雜貨鋪";
    m[7].nsize = 0; m[7].esize = 0; m[7].exitsize = 1; m[7].exit_set = ""; m[7].mevent_size = 1; m[7].mevent_set = "";
    m[8].nsize = 0; m[8].esize = 4; m[8].b_set = "b3zb3zb3zb3z"; m[8].e_set = "x41y42k2x25y26k0x14y10k2x52y10k0"; m[8].exitsize = 1; m[8].exit_set = "11_"; m[8].mevent_size = 2; m[8].mevent_set = "5_6_"; m[8].box_set = "2n3n4n5n6n7n8n"; m[8].name = L"南部森林外圍";
    m[9].nsize = 1; m[9].npcid = "8-"; m[9].esize = 0; m[9].exitsize = 1; m[9].exit_set = "13_"; m[9].mevent_size = 3; m[9].mevent_set = "7_8_9_"; m[9].name = L"星落村村長家";
    b_m[0].esize = 1; b_m[0].e_set = "e0x6y4p1"; b_m[0].p_set = "x18y14x19y14"; b_m[0].cx = 0; b_m[0].cy = 0; b_m[0].fsize = 0; b_m[0].nsize = 0;
    b_m[1].esize = 1; b_m[1].e_set = "e1x20y24p4"; b_m[1].p_set = "x27y27x26y27x27y26"; b_m[1].cx = 27; b_m[1].cy = 26; b_m[1].fsize = 3; b_m[1].f_set = "0n1n6n"; b_m[1].type = 1;
    b_m[2].esize = 2; b_m[2].e_set = "e0x5y4p1e0x6y3p1"; b_m[2].p_set = "x18y13x19y13"; b_m[2].cx = 0; b_m[2].cy = 0; b_m[2].fsize = 1; b_m[2].f_set = "2n"; b_m[2].nsize = 1; b_m[2].n_set = "n0x16y10";
    b_m[3].esize = 1; b_m[3].e_set = "e2x3y1p1"; b_m[3].p_set = "x3y12x4y12"; b_m[3].cx = 0; b_m[3].cy = 0; b_m[3].fsize = 0; b_m[3].nsize = 0;
    b_m[4].esize = 3; b_m[4].e_set = "e4x6y11p1e4x13y12p1e3x10y7p1"; b_m[4].p_set = "x9y14x10y14"; b_m[4].cx = 0; b_m[4].cy = 0; b_m[4].fsize = 2;  b_m[4].f_set = "3n4n"; b_m[4].nsize = 1; b_m[4].n_set = "n1x9y5"; b_m[4].lc = L"1.我方全員被擊敗\n2.愛麗絲被擊敗"; b_m[4].vc = L"1.狼王HP低於30%";
    p[0].name = L"艾倫"; p[0].story = L"見習醫生"; p[0].lv = 1; p[0].mhp = 10; p[0].hp = 10; p[0].dex = 10; p[0].Move = 6; p[0].isize = 1; p[0].asize = 1; p[0].x = 10; p[0].y = 10; p[0].speed = 10; p[0].turn = 0; p[0].abox = 0; p[0].pose = 1; p[0].str = 10; p[0].INT = 10; p[0].con = 10; p[0].cha = 10; p[0].wis = 10; p[0].arms_id_1 = 4; p[0].arms_id_2 = -1; p[0].armor_id = 1; p[0].stone_id = -1; p[0].s_check[4] = 1; p[0].state = 1; p[0].exp = 0; p[0].dexUp = 50; p[0].strUp = 40; p[0].intUp = 60; p[0].conUp = 20; p[0].chaUp = 60; p[0].wisUp = 20; p[0].act = 1; p[0].Act = 1; p[0].arms_b_1 = 1; p[0].arms_b_2 = 0; p[0].b_id_1 = -1; p[0].b_id_2 = -1; p[0].DEF = 1;
    p[1].name = L"夏洛特"; p[1].story = L"獵人"; p[1].lv = 1; p[1].mhp = 10; p[1].hp = 10; p[1].dex = 10; p[1].Move = 6; p[1].isize = 1; p[1].asize = 1; p[1].x = 10; p[1].y = 10; p[1].speed = 10; p[1].turn = 0; p[1].abox = 0; p[1].pose = 1; p[1].str = 10; p[1].INT = 10; p[1].con = 10; p[1].cha = 10; p[1].wis = 10; p[1].arms_id_1 = 0; p[1].arms_id_2 = 6; p[1].armor_id = 0; p[1].stone_id = -1; p[1].s_check[3] = 1; p[1].exp = 0; p[1].dexUp = 60; p[1].strUp = 30; p[1].intUp = 20; p[1].conUp = 20; p[1].chaUp = 60; p[1].wisUp = 60; p[1].act = 1; p[1].Act = 1; p[1].t_id = 1; p[1].arms_b_1 = 5; p[1].arms_b_2 = 1; p[1].b_id_1 = 0; p[1].b_id_2 = -1; p[1].DEF = 1; p[1].EDV = 1;
    p[2].name = L"愛麗絲"; p[2].story = L"騎士冠軍"; p[2].lv = 5; p[2].mhp = 20; p[2].hp = 20; p[2].dex = 13; p[2].Move = 6; p[2].isize = 1; p[2].asize = 1; p[2].x = 10; p[2].y = 10; p[2].speed = 12; p[2].turn = 0; p[2].abox = 0; p[2].pose = 1; p[2].str = 15; p[2].INT = 14; p[2].con = 14; p[2].cha = 13; p[2].wis = 12; p[2].arms_id_1 = 2; p[2].arms_id_2 = -1; p[2].armor_id = 2; p[2].stone_id = 2; p[2].exp = 0; p[2].dexUp = 30; p[2].strUp = 80; p[2].intUp = 50; p[2].conUp = 70; p[2].chaUp = 30; p[2].wisUp = 20; p[2].act = 1; p[2].Act = 1; p[2].t_id = 1; p[2].arms_b_1 = 1; p[2].arms_b_2 = 1; p[2].b_id_1 = -1; p[2].b_id_2 = -1; p[2].s_check[7] = 1; p[2].s_check[11] = 1; p[2].s_check[20] = 1; p[2].s_check[21] = 1; p[2].EDV = -2; p[2].DEF = 3;
    e_t[0].name = L"野狼"; e_t[0].story = L"團體行動的動物 隨著數量增加危險性也會大幅上升"; e_t[0].baid = 1; e_t[0].str = 12; e_t[0].dex = 13; e_t[0].con = 12; e_t[0].INT = 3; e_t[0].wis = 14; e_t[0].cha = 7; e_t[0].lv = 1; e_t[0].mhp = 11; e_t[0].hp = 11; e_t[0].Move = 7; e_t[0].speed = 12; e_t[0].exp = 105; e_t[0].drop = "100%15i"; e_t[0].species = "a";
    e_t[1].name = L"帝國動員兵"; e_t[1].story = L"帝國戰時動員的士兵，只接受過基礎的軍事訓練"; e_t[1].baid = 10; e_t[1].str = 11; e_t[1].dex = 10; e_t[1].con = 11; e_t[1].INT = 10; e_t[1].wis = 10; e_t[1].cha = 10; e_t[1].lv = 1; e_t[1].mhp = 10; e_t[1].hp = 10; e_t[1].Move = 5; e_t[1].speed = 11; e_t[1].exp = 150; e_t[1].drop = "50%0i"; e_t[1].species = "h"; e_t[1].e_range = 5;
    e_t[2].name = L"野蜂"; e_t[2].story = L"領地意識極強，一但靠近就會使用帶有毒液的尾針攻擊"; e_t[2].baid = 7; e_t[2].str = 3; e_t[2].dex = 15; e_t[2].con = 10; e_t[2].INT = 1; e_t[1].wis = 7; e_t[2].cha = 1; e_t[2].lv = 1; e_t[2].mhp = 5; e_t[2].hp = 5; e_t[2].Move = 5; e_t[2].speed = 15; e_t[2].exp = 50; e_t[2].drop = "20%16i"; e_t[2].species = "a"; e_t[2].e_range = 8;
    e_t[3].name = L"狼王"; e_t[3].story = L"統御南部森林的王者，過去誤食星隕礦而覺醒了操控火焰的能力，也獲得了普通野狼所沒有的智力"; e_t[3].baid = 1; e_t[3].str = 13; e_t[3].dex = 14; e_t[3].con = 13; e_t[3].INT = 15; e_t[3].wis = 13; e_t[3].cha = 11; e_t[3].lv = 1; e_t[3].mhp = 50; e_t[3].hp = 50; e_t[3].Move = 8; e_t[3].speed = 13; e_t[3].exp = 500; e_t[3].drop = ""; e_t[3].s_range = 4; e_t[3].species = "a";
    e_t[4].name = L"老狼"; e_t[4].story = L"經歷自然的選擇存活下來的野狼，體力雖然衰退卻也獲得了生存的智慧"; e_t[4].baid = 1; e_t[4].str = 10; e_t[4].dex = 11; e_t[4].con = 10; e_t[4].INT = 6; e_t[4].wis = 14; e_t[4].cha = 7; e_t[4].lv = 1; e_t[4].mhp = 15; e_t[4].hp = 15; e_t[4].Move = 6; e_t[4].speed = 11; e_t[4].exp = 150; e_t[4].drop = "100%15i"; e_t[4].species = "a";
    e_t[5].name = L"帝國軍隊長";
    e_t[6].name = L"帝國裝甲車\"阿斯提\""; e_t[6].story = L"帝國大戰後開發的新式裝甲車，運用了星隕石作為動力來源，必要時能夠激活星隕護盾";
    b_nt[0].name = L"戴恩"; b_nt[0].story = L"夏洛特的父親，是位老練的獵手，曾在巴蘭斯獨立戰爭中擔任狙擊手"; b_nt[0].lv = 15; b_nt[0].hp = 15; b_nt[0].mhp = 20; b_nt[0].Move = 5; b_nt[0].speed = 15; b_nt[0].pose = 2; b_nt[0].str = 15; b_nt[0].dex = 20; b_nt[0].con = 15; b_nt[0].INT = 10; b_nt[0].wis = 12; b_nt[0].cha = 9; b_nt[0].baid = 0;
    b_nt[1].name = L"昏迷的愛麗絲"; b_nt[1].story = L"因為超載使用大地之心而昏迷的愛麗絲"; b_nt[1].lv = 5; b_nt[1].hp = 15; b_nt[1].mhp = 15; b_nt[1].Move = 0; b_nt[1].speed = 0; b_nt[1].pose = 1; b_nt[1].str = 12; b_nt[1].dex = 10; b_nt[1].con = 12; b_nt[1].INT = 10; b_nt[1].wis = 11; b_nt[1].cha = 12; b_nt[1].baid = 2;
    ar[0].name = L"巡林者"; ar[0].dmg = "1d12"; ar[0].Dmg = L"1d12"; ar[0].range = 5; ar[0].story = L"巴蘭斯獵戶所喜愛的獵槍，能精準射殺大型獵物，是帝國在大戰後遺留的產物\n傷害:1d12\n裝填時間:1S"; ar[0].number = 0; ar[0].bullet = 1; ar[0].mbullet = 1; ar[0].type = "rr"; ar[0].time = 1;
    ar[1].name = L"爪子"; ar[1].dmg = "1d4+2"; ar[1].Dmg = L"1d4+2"; ar[1].hit = "1d2+2"; ar[1].range = 1; ar[1].number = 0; ar[1].bullet = 1; ar[1].mbullet = 1; ar[1].type = "m";
    ar[2].name = L"長劍"; ar[2].dmg = "1d8"; ar[2].Dmg = L"1d8"; ar[2].hit = "1d2"; ar[2].range = 1; ar[2].story = L"巴蘭斯騎士所使用的制式長劍\n傷害:1d8\n命中加值:1d2"; ar[2].number = 0; ar[2].bullet = 1; ar[2].mbullet = 1; ar[2].type = "m";
    ar[3].name = L"石中劍"; ar[3].dmg = "1d8"; ar[3].Dmg = L"1d8"; ar[3].hit = "1d2"; ar[3].range = 1; ar[3].story = L"傳說只有神選之人才能使用的聖劍\n傷害:1d8\n命中加值:1d2+3"; ar[3].number = 0; ar[3].bullet = 1; ar[3].mbullet = 1; ar[3].type = "m";
    ar[4].name = L"短刀"; ar[4].dmg = "1d4"; ar[4].Dmg = L"1d4"; ar[4].hit = "1d2+1"; ar[4].range = 1; ar[4].story = L"生活中常見的工具，作為武器用來近戰或投擲都很實用\n傷害:1d4\n命中加值:1d2+1"; ar[4].number = 0; ar[4].bullet = 1; ar[4].mbullet = 1; ar[4].type = "m";
    ar[5].name = L"10式手槍"; ar[5].dmg = "2d4"; ar[5].Dmg = L"2d4"; ar[5].hit = "1d2"; ar[5].range = 3; ar[5].story = L"大戰後期配發給共和軍官使用的手槍，全稱是諾曼10式輪轉手槍，槍身刻有守護二字\n傷害:2d4\n命中加值:1d2 裝填時間:3S"; ar[5].number = 0; ar[5].bullet = 6; ar[5].mbullet = 6; ar[5].type = "rp"; ar[5].time = 3;
    ar[6].name = L"獵刀"; ar[6].dmg = "1d5"; ar[6].Dmg = L"1d5"; ar[6].hit = "1d2"; ar[6].range = 1; ar[6].story = L"用來給獵物剝皮和切肉的刀具\n傷害:1d5\n命中加值:1d2"; ar[6].number = 0; ar[6].bullet = 1; ar[6].mbullet = 1; ar[6].type = "m";
    ar[7].name = L"尾針"; ar[7].dmg = "1d2+1"; ar[7].Dmg = L"1d2+1"; ar[7].range = 1; ar[7].story = L"昆蟲防衛自身的武器，帶毒"; ar[7].number = 0; ar[7].bullet = 1; ar[7].mbullet = 1; ar[7].type = "m";
    ar[8].name = L"蛇杖"; ar[8].dmg = "1d4"; ar[8].Dmg = L"1d4"; ar[8].hit = "1d2"; ar[8].range = 1; ar[8].story = L"醫者遠行常攜帶的手杖，杖身刻有著代表醫者的蛇\n傷害:1d4\n命中加值:1d2"; ar[8].number = 0; ar[8].bullet = 1; ar[8].mbullet = 1; ar[8].type = "m";
    ar[9].name = L"短嚎"; ar[9].dmg = "3d4"; ar[9].Dmg = L"3d3"; ar[9].hit = "3d3"; ar[9].story = L"帝國士官配發的防衛手槍，能全自動射擊";
    ar[10].name = L"無畏二型"; ar[10].dmg = "1d8+2"; ar[10].Dmg = L"1d8+2"; ar[10].range = 5; ar[10].story = L"源自三十年戰爭中期帝國製械廠開始量產的新式複合槍，無畏二字來自查里三世在試射後的賜名，帝國在大戰後總結了前線的反饋，所改進的二代版本\n傷害:1d8+2(步槍);2d4(霰彈)\n裝填時間:5S"; ar[10].type = "rrs";
    Ar[0].name = L"獵手服"; Ar[0].story = L"與森林近似的顏色能讓獵手不容易被發現，特殊的設計使穿著者能靈巧的移動\n防禦加值:1，閃避加值:1"; Ar[0].DEF = 1; Ar[0].EDV = 1;
    Ar[1].name = L"粗布襯衫"; Ar[1].story = L"便宜實惠耐穿，一天著裝的好選擇\n防禦加值:1"; Ar[1].DEF = 1; Ar[1].EDV = 0;
    Ar[2].name = L"半身甲"; Ar[2].story = L"保護身體要害免於刀劍威脅，但在抵禦火器的防護效果不佳，且對穿戴者的體力要求較高\n防禦加值:3，閃避減值:2\n穿戴要求:13 STR以上"; Ar[2].DEF = 3; Ar[2].EDV = -2;
    it[0].Name = L"步槍子彈"; it[0].name = "步槍子彈"; it[0].number = 0; it[0].type = 'b'; it[0].story = L"可以裝填步槍跟機槍的子彈\n傷害加值:2\n命中加值:2"; it[0].range = 0; it[0].value = 200;
    it[1].Name = L"繃帶"; it[1].name = "繃帶"; it[1].number = 0; it[1].type = 'h'; it[1].story = L"保護傷處與止血，用酒精消毒過的布匹能避免傷口惡化\n使用10秒後回復5HP並獲得兩層生命回復"; it[1].range = 1; it[1].value = 100; it[1].time = 10;
    it[2].Name = L"通常彈2"; it[2].name = "通常彈"; it[2].number = 0; it[2].type = 'b'; it[2].story = L"";
    it[3].Name = L"手槍子彈"; it[3].name = "手槍子彈"; it[3].number = 0; it[3].type = 'b'; it[3].story = L"可以裝填手槍和衝鋒槍的子彈\n傷害加值:1"; it[3].range = 0;
    it[4].Name = L"藥物清單"; it[4].name = "藥物清單"; it[4].number = 0; it[4].type = 'm'; it[4].story = L"父親紀錄需要藥物的內容，大部分都是與外傷處理的藥物有關";
    it[5].Name = L"百三聖水"; it[5].name = "百三聖水"; it[5].number = 0; it[5].type = 'h'; it[5].story = L"由白教生產的聖水藥劑，3%的聖水濃度能提供足夠回復輕傷的生命力\n使用1秒後回復10HP"; it[5].range = 1; it[5].value = 300; it[5].time = 1;
    it[6].Name = L"白之聖典"; it[6].name = "白之聖典"; it[6].number = 1; it[6].type = 'm'; it[6].story = L"艾倫已逝母親的遺物，古樸的書頁紀錄著白教的教義，但與教會的許多觀點有所分歧，因為一些緣故，似乎有未知力量能激發自身的潛能";
    it[7].Name = L"巴平"; it[7].name = "巴平"; it[7].number = 0; it[7].type = 'm'; it[7].story = L"巴蘭斯建國後鑄造的銀幣，巴蘭斯國的貨幣基本單位"; it[7].value = 1000;
    it[8].Name = L"阿根"; it[8].name = "阿根"; it[8].number = 5; it[8].type = 'm'; it[8].story = L"巴蘭斯建國後鑄造的金幣，1巴平可以兌換10阿根"; it[8].value = 100;
    it[9].Name = L"古彭"; it[9].name = "古彭"; it[9].number = 20; it[9].type = 'm'; it[9].story = L"巴蘭斯建國後鑄造的銅幣，1阿根可以兌換10古彭"; it[9].value = 10;
    it[10].Name = L"蘋果"; it[10].name = "蘋果"; it[10].number = 0; it[10].type = 'e'; it[10].story = L"遍布整片大陸的水果，民間有著\"一天一蘋果，醫生遠離我\"的俗諺\n食物 為單體回復20%HP\n儲存上限:10"; it[10].value = 20;
    it[11].Name = L"柳橙"; it[11].name = "柳橙"; it[11].number = 0; it[11].type = 'e'; it[11].story = L"能治療敗血症的水果，製成的柳橙汁也有良效\n食物 為單體回復25%HP\n儲存上限:10"; it[11].value = 25;
    it[12].Name = L"馬鈴薯"; it[12].name = "馬鈴薯"; it[12].number = 0; it[12].type = 'm'; it[12].story = L"易於保存以及種植，使之成為多數家庭中一天的主食\n食材"; it[12].value = 30;
    it[13].Name = L"豌豆罐頭"; it[13].name = "豌豆罐頭"; it[13].number = 0; it[13].type = 'e'; it[13].story = L"運用了最新的儲存技術，可以保存很久的罐頭食品，存放了豌豆\n食物 為單位回復30%生命"; it[13].value = 150;
    it[14].Name = L"小紅莓"; it[14].name = "小紅莓"; it[14].number = 0; it[14].type = 'e'; it[14].story = L"自然給予旅人的恩賜，郊外總能發現其存在\n食物 為單體回復10%HP\n儲存上限:25"; it[14].value = 5;
    it[15].Name = L"狼牙"; it[15].name = "狼牙"; it[15].number = 0; it[15].type = 'm'; it[15].story = L"通常會加工成飾品，此外也能作為神眷使儀式用的材料"; it[15].value = 50;
    it[16].Name = L"蜂蜜"; it[16].name = "蜂蜜"; it[16].number = 0; it[16].type = 'h'; it[16].story = L"具有藥用價值，能促進傷口癒合\n使用1秒後回復25%hp且獲得1AP"; it[16].value = 500; it[16].range = 0; it[16].time = 1;
    it[17].Name = L"燃燒瓶"; it[17].name = "燃燒瓶"; it[17].number = 0; it[17].type = 'a'; it[17].story = L"裝有易燃液體的玻璃瓶，製作簡單且對抗裝甲單位有奇效\n投擲物 對目標範圍內的單位造成2d3點傷害以及3層燃燒，同時點燃地面"; it[17].value = 200;
    st[0].name = L"長青石"; st[0].number = 0; st[0].story = L"從星隕礦採掘出的伴生礦 沾染了生命的氣息\nHP+2"; st[0].type = "H+2"; st[0].lv = 1;
    st[1].name = L"烈風石"; st[1].number = 0; st[1].story = L"從星隕礦採掘出的伴生礦 沾染了狂風的氣息\n速度+2"; st[1].type = "S+2"; st[1].lv = 1;
    st[2].name = L"大地之心"; st[2].number = 0; st[2].story = L"巴蘭斯的鎮國之寶，由頂級的星隕石製成，只有契合的神眷使才能使用\n限定愛麗絲使用\n全屬性+2"; st[2].type="A+2"; st[2].lv = 2;
    t_E[0].name = L"簡易醫療箱"; t_E[0].story = L"父親為艾倫準備的醫療箱，能很好的存放藥物與器材\n戰鬥中能攜帶的藥物數量增加 進行治療時所需的時間減少20%"; t_E[0].size = 3; t_E[0].number = 0;
    t_E[1].name = L"彈藥袋"; t_E[1].story = L"便於攜帶彈藥與裝填的布袋\n減少裝填時間1秒"; t_E[1].size = 3; t_E[1].number = 0;
    t_E[2].name = L""; t_E[2].story = L""; t_E[2].size = 3; t_E[2].number = 0;
    n[0].name = L"";
    n[1].name = L"朱利安"; n[1].x = 3; n[1].y = 6; n[1].avatar = 1;
    n[2].name = L"???";
    n[3].name = L"愛德華"; n[3].x = 23; n[3].y = 12; n[3].avatar = 1;
    n[4].name = L"士兵"; n[4].x = 13; n[4].y = 26; n[4].avatar = 3;
    n[5].name = L"夏洛特"; n[5].x = 27; n[5].y = 9; n[5].avatar = 1;
    n[6].name = L"戴恩"; n[6].x = 19; n[6].y = 13; n[6].avatar = 2;
    n[7].name = L"雜貨店店長"; n[7].x = 14; n[7].y = 12; n[7].avatar = 1;
    n[8].name = L"星落村村長"; n[8].x = 6; n[8].y = 14; n[8].avatar = 3;
    n[9].name = L"巴蘭斯軍官";
    n[10].name = L"路易斯";
    n[11].name = L"偵查兵";
    n[12].name = L"巴蘭斯騎士";
    n[13].name = L"全員";
    n[14].name = L"米切爾";
    n[15].name = L"帝國傳令兵";
    n[16].name = L"帝國炮兵";
    n[17].name = L"狼王";
    EX[0].x = 14; EX[0].y = 13; EX[0].gx = 6; EX[0].gy = 13; EX[0].Mid = 2; EX[0].state = true;  EX[0].cx = 14; EX[0].cy = 13;
    EX[1].x = 19; EX[1].y = 11; EX[1].gx = 13; EX[1].gy = 12; EX[1].Mid = 3; EX[1].state = true; EX[1].cx = 19; EX[1].cy = 11;
    EX[2].x = 19; EX[2].y = 19; EX[2].gx = 12; EX[2].gy = 25; EX[2].Mid = 4; EX[2].state = false;  EX[2].cx = 19; EX[2].cy = 19;
    EX[3].x = 6; EX[3].y = 11; EX[3].gx = 14; EX[3].gy = 12; EX[3].Mid = 1; EX[3].state = true; EX[3].cx = 6; EX[3].cy = 11;
    EX[4].x = 12; EX[4].y = 23; EX[4].gx = 19; EX[4].gy = 18; EX[4].Mid = 2; EX[4].state = true; EX[4].cx = 12; EX[4].cy = 23;
    EX[5].x = 13; EX[5].y = 13; EX[5].gx = 19; EX[5].gy = 13; EX[5].Mid = 2; EX[5].state = true; EX[5].cx = 13; EX[5].cy = 13;
    EX[6].x = 26; EX[6].y = 0; EX[6].gx = 23; EX[6].gy = 18; EX[6].Mid = 5; EX[6].state = false; EX[6].cx = 27; EX[6].cy = 0;
    EX[7].x = 22; EX[7].y = 19; EX[7].gx = 26; EX[7].gy = 1; EX[7].Mid = 4; EX[7].state = true; EX[7].cx = 23; EX[7].cy = 19;
    EX[8].x = 41; EX[8].y = 42; EX[8].gx = 15; EX[8].gy = 18; EX[8].Mid = 6; EX[8].state = true; EX[8].cx = 41; EX[8].cy = 42;
    EX[9].x = 15; EX[9].y = 19; EX[9].gx = 41; EX[9].gy = 44; EX[9].Mid = 4; EX[9].state = true; EX[9].cx = 15; EX[9].cy = 19;
    EX[10].x = 2; EX[10].y = 0; EX[10].gx = 58; EX[10].gy = 46; EX[10].Mid = 8; EX[10].state = true; EX[10].cx = 3; EX[10].cy = 0;
    EX[11].x = 58; EX[11].y = 47; EX[11].gx = 3; EX[11].gy = 1; EX[11].Mid = 5; EX[11].state = true; EX[11].cx = 59; EX[11].cy = 47;
    EX[12].x = 42; EX[12].y =25; EX[12].gx = 14; EX[12].gy = 18; EX[12].Mid = 9; EX[12].state = true; EX[12].cx = 42; EX[12].cy = 25;
    EX[13].x = 14; EX[13].y = 19; EX[13].gx = 42; EX[13].gy = 27; EX[13].Mid = 4; EX[13].state = true; EX[13].cx = 14; EX[13].cy = 19;
    m_f[0].x = 14; m_f[0].y = 9; m_f[0].check = 0;
    m_f[1].x = 23; m_f[1].y = 12; m_f[1].check = 0;
    m_f[2].x = 27; m_f[2].y = 9; m_f[2].check = 0;
    m_f[3].x = 24; m_f[3].y = 25; m_f[3].check = 0;
    m_f[4].x = 14; m_f[4].y = 14; m_f[4].check = 0;
    m_f[5].x = 25; m_f[5].y = 22; m_f[5].check = 0;
    m_f[6].x = 51; m_f[6].y = 7; m_f[6].check = 0;
    m_f[7].x=6; m_f[7].y=14; m_f[7].check = 0;
    m_f[8].x = 6; m_f[8].y = 14; m_f[8].check = 1;
    m_f[9].x = 6; m_f[9].y = 14; m_f[9].check = 1;
    m_f[10].x = 26; m_f[10].y = 3; m_f[10].check = 0;
    Box[0].state = true; Box[0].x = 16; Box[0].y = 9; Box[0].type = 0;
    Box[1].state = true; Box[1].x = 15; Box[1].y = 12; Box[1].type = 0;
    Box[2].state = true; Box[2].x = 59; Box[2].y = 45; Box[2].type = 1;
    Box[3].state = true; Box[3].x = 18; Box[3].y = 45; Box[3].type = 1;
    Box[4].state = true; Box[4].x = 40; Box[4].y = 37; Box[4].type = 1;
    Box[5].state = true; Box[5].x = 20; Box[5].y = 14; Box[5].type = 1;
    Box[6].state = true; Box[6].x = 33; Box[6].y = 26; Box[6].type = 1;
    Box[7].state = true; Box[7].x =48 ; Box[7].y = 19; Box[7].type =1 ;
    Box[8].state = true; Box[8].x = 43; Box[8].y =0 ; Box[8].type = 2;
    /*Box[].state = true; Box[].x = ; Box[].y = ; Box[].type = ;*/
    sk[0].name = L"戰場敏銳"; sk[0].story = L"抓準敵人攻勢的破綻\n被動:每30秒為我方全員取得一層戰術優勢"; sk[0].type = "b0"; sk[0].cost = 3;
    sk[1].name = L"瞄準"; sk[1].story = L"進行射擊前的精準估算 效果:每3點移動力換取一層攻擊優勢"; sk[1].type = "B"; sk[1].cost = 3;
    sk[2].name = L"巴頓術"; sk[2].story = L"一種運用杖技、腿法與柔術的綜合護身術，攜帶杖類武器方可使用\n效果:消耗1AP，近戰，命中計算時加入靈巧，命中時使對手獲得15秒的一層失衡並造成1d4+(STR+DEX)/5點傷害"; sk[2].type = "A"; sk[2].cost = 3;
    sk[3].name = L"快速裝填"; sk[3].story = L"長期的訓練所掌握的裝填技巧\n效果:裝填裝備中槍械並減少裝填時間30%，根據裝填槍種不同扣除所需MP"; sk[3].type = "B"; sk[3].cost = 3;
    sk[4].name = L"傷勢評估"; sk[4].story = L"快速排查傷患生命狀態，給予初步治療\n被動:治療前先回復10%目標最大生命"; sk[4].type = "P"; sk[4].cost = 3;
    sk[5].name = L"傷患轉移"; sk[5].story = L"將傷患轉移到更適合治療的地形\n效果:消耗1AP，支付所需MP轉移單位與自身到指定位置"; sk[5].type = "A"; sk[5].cost = 2;
    sk[6].name = L"白之加護"; sk[6].story = L"為一名友方單位注入生命之力\n效果:消耗1AP，給予5+(WIS/2)的護盾，以及60秒的(WIS/7)層加護"; sk[6].type = "B"; sk[6].cost = 3;
    sk[7].name = L"星隕石激活"; sk[7].story = L"使星隕石系統的核心能量激活\n被動:根據星隕石種類獲得額外的屬性與能力"; sk[7].type = "S"; sk[7].cost = 3;
    sk[8].name = L"哨戒"; sk[8].story = L"效果:消耗1AP，獲得三層哨戒狀態，每當武器範圍內有敵人進入或是移動以及攻擊，進行一次攻擊判定"; sk[8].cost = 2;
    sk[9].name = L"集火號令"; sk[9].story = L"效果:一回合可以標記一個敵方目標使對該目標的命中判定獲得一個獎勵骰，獎勵骰的面數取決於標記者魅力/2"; sk[9].cost = 2; sk[9].type = "A";
    sk[10].name = L"戰鎖"; sk[10].story = L"運用近戰技巧封鎖一名敵人單位的行動，使敵我雙方進入無法移動與無法閃避的狀態\n效果:直到10+(STR+DEX)/5秒後，或是雙方中有單位死亡"; sk[10].cost = 2;
    sk[11].name = L"核心護盾"; sk[11].story = L"藉由星隕石能量搭建星隕護盾，星隕石的常見運用方法\n被動:根據星隕石等級獲得LV*10的星隕護盾"; sk[11].cost = 2; sk[11].type = "S";
    sk[12].name = L"白之利刃"; sk[12].story = L"消耗1AP，為一名友軍的武器注入生命之力\n效果:普攻時最終傷害獲得(持有者WIS-5)/2點加值，單位普攻時回復20%已損失生命，持續15+WIS秒"; sk[12].cost = 2;
    sk[13].name = L"步槍掌握"; sk[13].story = L"熟悉了步槍的運作，能射擊更遠的目標\n被動:使用步槍時，射程+1"; sk[13].cost = 3; sk[13].type = "b23";
    sk[14].name = L"猝不及防"; sk[14].story = L"通過隱匿手段避免進入敵方視線，實現難以防備的攻擊\n被動:進入戰鬥後第一次攻擊必定命中，必須使用槍械，當(30-DEX/2)秒內未主動攻擊，將可以再觸發本技能"; sk[14].cost =2 ; sk[14].type = "b25";
    sk[15].name = L"遠射"; sk[15].story = L"讓敵方難以察覺的遠程攻擊\n被動:攻擊距離超過5格的敵人，敵人的迴避判定減少雙方DEX差值/2，並增加(雙方距離-5)點傷害"; sk[15].cost =2 ; sk[15].type = "P";
    sk[16].name = L"迴避偵查"; sk[16].story = L"快速脫離戰場正面的技巧，前往更有利的位置\n被動:回合開始時有敵人在5格距離內，MP+1，兩格內再MP+1"; sk[16].cost =3 ; sk[16].type = "P";
    sk[17].name = L"野性洞察"; sk[17].story = L"狩獵的經驗使得對野獸的習性相當熟悉，能更好的找到獵物的破綻\n被動:對非人類動物單位攻擊判定獲得優勢，最終傷害提升30%"; sk[17].cost =3 ; sk[17].type = "P";
    sk[18].name = L"狩獵陷阱"; sk[18].story = L"對獵人而言，陷阱的設置也是一門重要的技術\n效果:花費10秒的時間設置一個簡易狩獵陷阱，觸發陷阱的敵人中斷當前移動並會受到5點傷害以及獲得60秒足傷"; sk[18].cost = 2; sk[18].type = "A";
    sk[19].name = L"掩護射擊"; sk[19].story = L"在不影響友方攻擊的前提，為友軍進行掩護\n效果:消耗1AP並結束當前回合，直到下回合開始前，當我方單位在掩護者武器範圍內攻擊敵方單位未擊殺時，掩護者追加一次攻擊，持有槍械且裝有彈藥才能生效"; sk[19].cost =2; sk[19].type = "A";
    sk[20].name = L"強行軍"; sk[20].story = L"管理並分配體能，專注於移動的技巧\n效果:支付所有AP，每支付一點AP本回合MP+4"; sk[20].cost = 0; sk[20].type = "B";
    sk[21].name = L"衝鋒"; sk[21].story = L"衝擊敵陣，將速度的力量達到極致\n效果:支付所有MP，規劃衝鋒路徑，對路徑周圍所有敵人進行普攻\n越遠的敵人追加傷害越高，每兩格追加一點傷害\n需要攜帶近戰武器才可發動";
    /*sk[].cost = ;*/
    bu[0].name = L"戰場敏銳"; bu[0].story = L"每30秒為我方全員取得一層戰術優勢";
    bu[1].name = L"攻擊優勢"; bu[1].story = L"進攻前可消耗N層，當進攻失敗時將可進行N次重新判定";
    bu[2].name = L"戰術優勢"; bu[2].story = L"當進攻或閃躲前可消耗N層，當失敗時可進行N次重新判定";
    bu[3].name = L"失衡"; bu[3].story = L"移動力減半、回合開始時獲得一層劣勢";
    bu[4].name = L"治療中"; bu[4].story = L"治療時不能行動以及暫停累積行動條";
    bu[5].name = L"裝填中"; bu[5].story = L"裝填時不能行動以及暫停累積行動條";
    bu[6].name = L"生命回復"; bu[6].story = L"單位行動開始時，消耗一層回復3HP";
    bu[7].name = L"純白加護"; bu[7].story = L"每一層可以減少一點所受的生命傷害";
    bu[8].name = L"星隕護盾"; bu[8].story = L"當單位受到傷害時自動扣除相同護盾減少傷害，但攻擊者持有星隕護盾且近戰攻擊時則無法減免";
    bu[9].name = L"昏迷"; bu[9].story = L"無法行動以及閃避";
    bu[10].name = L"大地加護"; bu[10].story = L"每一層減少一點所受的生命傷害";
    bu[11].name = L"浴火"; bu[11].story = L"於燃燒地形，免疫燃燒地形傷害以及DEBUFF並獲得STR+2，DEX+1，回合開始時若在燃燒地形上回復最大生命5%";
    bu[12].name = L"燒灼"; bu[12].story = L"回合開始時造成MHP*5%*層數的直接傷害(最少為1)，並減少一層燒灼";    
    bu[13].name = L"蓄力中-火球"; bu[13].story = L"對3*3範圍內的單位造成2d4的範圍傷害以及附加一層燒灼，並燃燒地面60秒，射程4";
    bu[14].name = L"力竭"; bu[14].story = L"無法閃避以及行動，當再次受到敵人攻擊或倒數結束時將會退出戰鬥，當HP回到1以上將消除";
    bu[15].name = L"蓄力中-炎浪"; bu[15].story = L"對範圍內的單位造成3d3加上燒灼層數*2的傷害，並燃燒地面90秒";
    bu[16].name = L"神眷護盾"; bu[16].story = L"當單位受到傷害時自動扣除相同護盾減少傷害";
    bu[17].name = L"長青石:激活"; bu[17].story = L"每30秒回復2HP，MHP+2";
    bu[18].name = L"烈風石:激活"; bu[18].story = L"DEX+1，速度+2";
    bu[19].name = L"集火目標"; bu[19].story = L"命中判定時，對方獲得一個特殊獎勵骰";
    bu[20].name = L"封鎖中"; bu[20].story = L"無法移動與閃避";
    bu[21].name = L"白之利刃"; bu[21].story = L"普攻時最終傷害獲得(持有者WIS-5)/2點加值，單位攻擊時回復20%已損失生命";
    bu[22].name = L"哨戒"; bu[22].story = L"當敵人進入或是在武器射程內進行移動跟行動前，先進行一次攻擊，優先使用射程較遠的武器，槍械會消耗彈藥";
    bu[23].name = L"步槍掌握"; bu[23].story = L"步槍射程+1";
    bu[24].name = L"猝不及防:冷卻中"; bu[24].story = L"持續時間內不進行普攻，冷卻完將能獲得一層猝不及防";
    bu[25].name = L"猝不及防"; bu[25].story = L"普攻必定命中，必須使用槍械";
    bu[26].name = L"陷阱布置中"; bu[26].story = L"無法行動";
    bu[27].name = L"足傷"; bu[27].story = L"每回合MP-5，閃避值減半";
    bu[28].name = L"掩護射擊"; bu[28].story = L"本單位回合開始前，當我方單位在本單位武器範圍內攻擊敵方單位未擊殺時，本單位追加一次攻擊，持有槍械且裝有彈藥才能生效";
    bu[29].name = L"戰術劣勢"; bu[29].story = L"當進攻或閃躲成功時自動移除一層劣勢 進行一次重新判定 直到失敗或劣勢耗盡";
    bu[30].name = L"進攻劣勢"; bu[30].story = L"當進攻成功時自動移除一層劣勢 進行一次重新判定 直到失敗或劣勢耗盡";
    bu[31].name = L"中毒-蜂毒"; bu[31].story = L"回合開始時進行體質鑑定，成功則移除一層否則將受到3點直接傷害";
    bu[32].name = L"駐守"; bu[32].story = L"單位尚未發現敵人，將在原地駐守直到發現敵人";
    bu[33].name = L"索敵中"; bu[33].story = L"單位發現敵人出現，將前往最後發現敵人蹤跡的位置";
    /*    bu[].name = L""; bu[].story = L"";*/
    sp[0].i_number[10] = 10;   sp[0].i_number[11] = 10;   sp[0].i_number[12] = 10;   sp[0].i_number[13] = 10; sp[0].i_number[16] = 3;
    sp[1].i_number[7] = 10; sp[1].i_number[8] = 10; sp[1].i_number[9] = 10; sp[1].type = "a";
    tk[0].name = L"序章-主線-集合"; tk[0].story = L"與青梅竹馬夏洛特約定在村門口集合，在集合點與之對話\n夏洛特有著特別的粉髮很容易認出來"; tk[0].state = 1; tk[0].type = 0;
    tk[1].name = L"序章-主線-父親的提醒"; tk[1].story = L"艾倫父親-愛德華似乎有什麼事要與艾倫說，去找他問問吧"; tk[1].state = 0; tk[1].type = 0;
    tk[2].name = L"序章-主線-與戴恩會合"; tk[2].story = L"出村前往夏洛特家，先與夏洛特的父親-戴恩會合"; tk[2].state = 0; tk[2].type = 0;
    tk[3].name = L"序章-主線-前往大河鎮"; tk[3].story = L"沿著林中小徑，穿越森林前往大河鎮，要小心沿途的野獸"; tk[3].state = 0; tk[3].type = 0;
    tk[4].name = L"序章-支線-狩獵狼群"; tk[4].story = L"森林中的狼群嚴重危害到星落村居民的安全，村長為尋求能人清除狼群，願意給出賞金來獎勵，可以用狼牙作為證明來兌換賞金"; tk[4].state = 0; tk[4].type = 1;
    tk[5].name = L"序章-主線-聚餐"; tk[5].story = L"前往飯廳與夏洛特一家一起享用晚餐"; tk[5].state = 0; tk[5].type = 0;
}
/*開始畫面*/
    while (1) {
        int s = start();
        if (s == 1) {
            first = time(NULL);
            p[0].state = 1;
            event(f, b_f,p, n,m_f,m,Box, EX,e,e_n,tk,sp, it,st,ar,m_id,b_mid,b_id);
            /*事件在這裡event(f,p,n);*/
            break;
        }
        else if (s == 0) {
            menu_load(p, ar, it, st, f,EX, m_f,t_E,sp,Box,tk,m,e_n,n,P_id, i_id, ar_id, st_id, f_id, psize, first, two, three, m_id, ex_id,sk_id,m_fid,t_Eid,sp_id,b_id,tk_id);
            if (three>0 ) {
                first = time(NULL);
                break;
            }
        }
    }
    FP = 0;
        b_mid = -1;
        
        m_set(m, n, p, e_n, Box, m_id, b_id);m_map(p,m, Box, m_id, b_id, n,""); m_put(p, m, m_id);;
            string g;
            Z = 2;

    while (1) {
/*一般模式*/
        while (1) {
            if (isReady) {
                event(f, b_f,p, n, m_f, m, Box,EX,e,e_n, tk,sp, it, st,ar, m_id, b_mid, b_id);
                if (sp_id != -1) {
                    trade(p, it,ar,t_E, sp, sp_id, i_id,ar_id,t_Eid);
                }
                if (b_mid == -1) {
                    b_mid = b_check(p, e_n, m, m_id);
                }
                if (Z == 2) {
                    m_check(p, m, EX, Box, m_id,b_id, n, e_n);
                    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
                    flushmessage(-1);
                }
                /*!=-1*/
                if (b_mid !=-1) {
                    m[m_id].px = p[0].x;
                    m[m_id].py = p[0].y;
                    battle_set(&wofs,e, e_t, p, b_m, b_f,sk,bu,ar,st,b_n,b_nt,te, b_mid, sk_id,buff_id);
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
                    IMAGE s_l;
                    RECT lt={208,417,208+750,417+210},vt={0,70,752,70+217};
                    loadimage(&s_l , L"./Game/picture/win_lose.png", 0, 0, false);
                    BeginBatchDraw();
                    maps(p, P_id, e, b_m, ar, te, b_mid);/*地圖繪製*/
                    e_put(e, b_m, te, b_m[b_mid].esize, b_mid);
                    b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                    p_put(p, b_m, b_m[b_mid].psize, b_mid);
                    ui(p, e, b_m, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id);                    
                    FlushBatchDraw();
                    getimage(&get, 0, 0, 1296, 960);
                    transparentimage(NULL, 0, 0, &s_l);
                    settextcolor(WHITE);
                    setbkmode(TRANSPARENT);
                    settextstyle(40, 0, _T("Taipei Sans TC Beta"));
                    if (b_m[b_mid].vc != L"") {
                    drawtext(b_m[b_mid].vc.c_str(), &vt, DT_WORDBREAK | DT_LEFT);
                    }
                    else {
                        wstring mvc = L"擊敗敵方全員";
                        drawtext(mvc.c_str(), &vt, DT_WORDBREAK | DT_LEFT);
                    }
                    if (b_m[b_mid].lc != L"") {
                    drawtext(b_m[b_mid].lc.c_str(), &lt, DT_WORDBREAK | DT_RIGHT);
                    }
                    else {
                        wstring mlc = L"我方全員被擊敗";
                        drawtext(mlc.c_str(), &lt, DT_WORDBREAK | DT_RIGHT);
                    }
                    FlushBatchDraw();
                    Sleep(1000);
                    flushmessage(-1);
                    ExMessage em;
                    while (1) {
                        flushmessage(-1);
                        em = getmessage(EM_MOUSE);
                        if (em.lbutton) {
                            break;
                        }
                    }
                    putimage(0, 0, &get);
                    FlushBatchDraw();
                    settextcolor(BLACK);
                    setbkmode(TRANSPARENT);
                    settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                    b_event(f,b_f,p,n,b_m,e_t,e,m,Box,b_n, tk, m_f, it, st, ar, te,b_mid,P_id, b_m[b_mid].psize, buff_id,b_id,b_nid,buff_id);
                    END(&wofs,p, e,b_n, "e",b_m,b_f,f,n,e_t,bu,ar,Ar,m,Box, tk, m_f,it, st,te,b_mid, P_id, id, roundp, roundb, roundn, b_m[b_mid].psize, buff_id,b_id,b_nid,buff_id,sk_id);
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
                    m_event_check(p,m,m_f,ar,it, st,Box,t_E, tk,m_id,b_id,n,sp_id);
                }
                if (g == "esc") {
                    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
                    flushmessage(-1);
                    Sleep(500);
                    menu(p, it, ar,Ar, st, f, e,m,n,e_n, EX,Box,sk,m_f,t_E,sp,tk,P_id, f_id, i_id, ar_id, Ar_id,st_id,b_id, m[m_id].psize, roundp, roundb, first, two, three, m_id, ex_id, sk_id,m_fid,t_Eid,sp_id,tk_id);
                    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));                    
                    flushmessage(-1);
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
                }
                show(m, p, e, e_n,Box, m_id,b_id, n,f, g);
                getimage(&save_image, 0, 0, 1296, 960);
             }
            else {
                Sleep(500);
            }

            }
/*戰鬥模式*/
        mciSendString(L"open ./Game/Sound/SE/魔王魂_戦闘18.mp3", NULL, 0, NULL);
        mciSendString(L"open ./Game/Sound/SE/魔王魂_効果音_ワンポイント33.mp3", NULL, 0, NULL);
        mciSendString(L"open ./Game/Sound/SE/決定ボタンを押す9.mp3 alias click", NULL, 0, NULL);
        while (1) {
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            if (battle_check(p, e, b_m, b_f,b_mid) == 1) {           
                for (i = 0; i < b_m[b_mid].psize; i++) {
                    p[i].buff_Size = 0;
                    if (p[i].buff_check[17] >= 1) {
                        p[i].mhp -= 2;
                        if (p[i].hp > p[i].mhp) {
                            p[i].hp = p[i].mhp;
                        }
                    }
                    if (p[i].buff_check[18] >= 1) {
                        p[i].dex--;
                        p[i].speed -= 2;
                    }
                    for (j = 0; j < buff_id; j++) {
                        p[i].buff_check[j] = 0;
                    }
                }
                for (i = 0; i < b_m[b_mid].nsize; i++) {
                    for (j = 0; j < buff_id; j++) {
                        b_n[i].buff_check[j] = 0;
                    }
                    b_n[i].buff_Size = 0;
                }
                for (i = 0; i < b_m[b_mid].esize; i++) {
                    for (j = 0; j < buff_id; j++) {
                        e[i].buff_check[j] = 0;
                    }
                    e[i].buff_Size = 0;
                }    
                b_end_menu(p, e,e_t, it,b_m, b_mid);
                battle2map(p, n, e_n, m, m_id); m_map(p, m, Box, m_id, b_id, n, g); m_put(p, m, m_id); b_mid = -1; break;
            }
            if (lose_check(&wofs, e, p, e_n, b_n, b_m, e_t, te, b_f, b_mid) == 1) {
                IMAGE ch;
                BeginBatchDraw();
                loadimage(&ch, L"./Game/picture/lose_chose.png", 0, 0, false);
                putimage(500-48*3, 400-48, &ch);
                RECT r = { 500 - 48 * 3,400 - 48, 750 - 48 * 3, 500 - 48 };
                wstring mm = L"是否要讀檔";
                settextstyle(18, 0, _T("Taipei Sans TC Beta"));
                drawtext(mm.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                settextstyle(30, 0, _T("Taipei Sans TC Beta"));
                EndBatchDraw();
                ExMessage em;
                while (1) {
                    flushmessage(-1);
                    em = getmessage(EM_MOUSE);
                    if (em.lbutton) {
                        if (em.x > 521 - 48 * 3 && em.x < 593 - 48 * 3 && em.y>467 - 48 && em.y < 491 - 48) {
                            menu_load(p, ar, it, st, f, EX, m_f, t_E, sp, Box, tk,m,e_n,n,P_id, i_id, ar_id, st_id, f_id, psize, first, two, three, m_id, ex_id, sk_id, m_fid, t_Eid, sp_id, b_id, tk_id);
                        }
                        else if (em.x > 649 - 48 * 3 && em.x < 721 - 48 * 3 && em.y>467 - 48 && em.y < 491 - 48) {
                            return 0;
                        }
                    }
                }
                break;
            }
            b_event(f,b_f, p, n, b_m, e_t, e,m,Box,b_n, tk, m_f, it, st, ar, te,b_mid,P_id, b_m[b_mid].psize,buff_id,b_id,b_nid,buff_id);
            settextstyle(18, 0, _T("Taipei Sans TC Beta"));
            if (roundp == 1) {
                b_camera(b_m, p[P_id].x, p[P_id].y, b_mid);
                while (chose=="") {
                BeginBatchDraw();                
                maps(p, P_id, e,b_m, ar, te,b_mid);/*地圖繪製*/
                e_put(e, b_m, te, b_m[b_mid].esize, b_mid);
                b_nput(b_n, b_m, b_nid, b_m[b_mid].nsize, b_mid);
                p_put(p, b_m, b_m[b_mid].psize, b_mid);
                ui(p, e, b_m, P_id, b_m[b_mid].esize, b_m[b_mid].psize, b_mid, buff_id);
                FlushBatchDraw();
                start_time = clock();
                for (; (clock() - start_time) < 50;);
                flushmessage(-1);
                acts(p, e,b_n, b_m, ar,bu,te,chose, b_mid, P_id, b_m[b_mid].esize, id, b_m[b_mid].psize,buff_id,b_nid);/*選項*/                
                }
                mciSendString(L"close click", NULL, 0, NULL);
                mciSendString(L"open ./Game/Sound/SE/決定ボタンを押す9.mp3 alias click", NULL, 0, NULL);
                vos = L"setaudio click volume to " + to_wstring(soundSize);
                mciSendString(vos.c_str(), NULL, 0, NULL);
                mciSendString(L"play click", NULL, 0, NULL);                
                Save(p, e, ar, it, st, f, P_id, i_id, ar_id, st_id, f_id, b_m[b_mid].time, b_m[b_mid].psize, roundp, roundb, first, two, three, chose);
                Load(p, e, ar, it, st, f, i_id, ar_id, st_id, f_id, P_id, b_m[b_mid].time, b_m[b_mid].psize, b_m[b_mid].esize, roundp, roundb, first, two, three, chose);
                p_attack(&wofs,p, e,e_t,b_n, ar, b_m,bu,te,b_mid,P_id, id, chose, b_m[b_mid].esize, b_m[b_mid].psize, buff_id,b_nid);/*攻擊*/
                p_item(&wofs,p,e,b_n, ar, it, t_E,b_m,te,chose, P_id,i_id,t_Eid,b_mid,buff_id);/*物品*/
                p_walk(&wofs,p, e,b_n,b_m, ar, te, chose, P_id, id, b_m[b_mid].psize, b_m[b_mid].esize,b_mid, buff_id,b_nid);/*移動*/
                p_skill(&wofs,chose,p,e,b_n,sk,ar,t_E,it,b_m,te,sk_id,P_id,b_m[b_mid].esize,b_mid,b_m[b_mid].psize,buff_id,b_nid);
                END(&wofs, p, e,b_n, chose, b_m, b_f, f, n, e_t, bu, ar,Ar,m, Box, tk, m_f, it, st,te, b_mid, P_id, id, roundp, roundb, roundn, b_m[b_mid].psize, buff_id, b_id,b_nid, buff_id, sk_id);/*結束行動*/
                chose = "";
                /*存檔*/
                /*讀檔*/
            }
            else if (roundb == 1) {
                e_battle_ai(&wofs,e,p,e_n,b_n,b_m,e_t,ar, Ar,te,b_f,id,P_id,b_nid,b_mid,buff_id);
                END(&wofs, p, e,b_n, "e", b_m, b_f, f, n, e_t, bu, ar,Ar,m, Box, tk, m_f, it, st, te,b_mid, P_id, id, roundp, roundb,roundn, b_m[b_mid].psize, buff_id, b_id,b_nid, buff_id, sk_id);/*結束行動*/
            }
            else if (roundn == 1) {
                n_target(b_n, b_m, e, b_mid, b_nid, id);
                n_walk(&wofs, b_n, b_m, e, p, ar, te, b_mid, b_nid, id, b_m[b_mid].psize, b_m[b_mid].esize, buff_id, P_id);
                n_attack(&wofs, b_n, b_m, e, p, ar, te, b_mid, b_nid, id, b_m[b_mid].psize, b_m[b_mid].esize, buff_id, P_id);
                END(&wofs, p, e, b_n, "e", b_m, b_f, f, n, e_t, bu, ar,Ar,m, Box, tk, m_f, it, st,te, b_mid, P_id, id, roundp, roundb, roundn, b_m[b_mid].psize, buff_id, b_id, b_nid, buff_id, sk_id);/*結束行動*/
            }
        }
    }
}