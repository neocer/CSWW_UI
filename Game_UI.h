#pragma once
#include<ITextureRender.h>
#include <time.h>
#include <string>

using  std::wstring;
#define KILLHOLDTIME 6
#define CHANNEL 64
#define WPNSHOWFX 0.7
#define MAXKILL 6
#define MAXMSG 10
#define Msgtime 10
#define KILLICONHOLETIME 1
#define WPNJG 5


#define CSW_P228		1
#define CSW_SCOUT		3
#define CSW_HEGRENADE	4
#define CSW_XM1014		5
#define CSW_C4			6
#define CSW_MAC10		7
#define CSW_AUG			8
#define CSW_SMOKEGRENADE	9
#define CSW_ELITE		10
#define CSW_FIVESEVEN	11
#define CSW_UMP45		12
#define CSW_SG550		13
#define CSW_GALI		14
#define CSW_GALIL		14
#define CSW_FAMAS		15
#define CSW_USP			16
#define CSW_GLOCK18		17
#define CSW_AWP			18
#define CSW_MP5NAVY		19
#define CSW_M249		20
#define CSW_M3			21
#define CSW_M4A1		22
#define CSW_TMP			23
#define CSW_G3SG1		24
#define CSW_FLASHBANG		25
#define CSW_DEAGLE		26
#define CSW_SG552		27
#define CSW_AK47		28
#define CSW_KNIFE		29
#define CSW_P90			30
#define CSW_VEST		31
#define CSW_VESTHELM	32

const int WPN_BIT_1 = (1 << CSW_SCOUT) | (1 << CSW_XM1014) | (1 << CSW_MAC10) | (1 << CSW_AUG) | (1 << CSW_UMP45) | (1 << CSW_SG550) | (1 << CSW_GALIL) | (1 << CSW_FAMAS) | (1 << CSW_AWP) | (1 << CSW_MP5NAVY) | (1 << CSW_M249) | (1 << CSW_M3) | (1 << CSW_M4A1) | (1 << CSW_TMP) | (1 << CSW_G3SG1) | (1 << CSW_SG552) | (1 << CSW_AK47) | (1 << CSW_P90);
const int WPN_BIT_2 = (1 << CSW_P228) | (1 << CSW_ELITE) | (1 << CSW_FIVESEVEN) | (1 << CSW_USP) | (1 << CSW_GLOCK18) | (1 << CSW_DEAGLE);
const int WPN_BIT_3 = (1 << CSW_KNIFE);
const int WPN_BIT_4 = (1 << CSW_HEGRENADE);

const int WPN_BIT_5 = (1 << CSW_C4);
const int WPN_BIT_42 = (1 << CSW_FLASHBANG);
const int WPN_BIT_43 = (1 << CSW_SMOKEGRENADE);
using namespace std;
struct DrawTGA {
	char filename[64];
	int red = 255;
	int green = 255;
	int blue = 255;
	int alpha = 255;
	int mode = 0;
	int x = 0, y = 0;
	int wide, tall;
	double holdtime = -1;
	clock_t starttime;
	texcache_t *tex;
	int id;
	int body;
	bool XYZ;
	float POINT[3];

};



struct DrawTEXT {
	wstring text;
	char font[64];
	int red = 255;
	int green = 255;
	int blue = 255;
	int alpha = 255;
	int mode;
	int fade;
	int size;
	int x = 0, y = 0;
	int hfont;
	int center;

	double holdtime = (clock_t)-1;
	clock_t starttime;

	int id;
	int body;
	bool XYZ;
	float POINT[3];

};
class NoticeShow {

	texcache_t *NoticeBG;
	int Tip;

public:

	wstring Tiptext;
	clock_t CreatTime = -1;
	int showtime = 0;
	NoticeShow();
	void DrawNotice();
	void IntTip(const wchar_t text[],int showTime);

};
struct myplayer {
	wstring name;
	int id;
	int font;

};

struct deathmsg {
	struct myplayer player1;
	struct myplayer player2;
	struct myplayer player3;

	string weapon;
	bool headshoot;
	clock_t time =(clock_t) -1.0;
	texcache_t *wpntex = NULL;
	texcache_t *headtex = NULL;
	texcache_t *ditu = NULL;
	texcache_t *firstkilltex = NULL;
	texcache_t *mKILL = NULL;
	int mkill;
	int firstkill = 0;


};


struct PlayerInfo
{
	bool dead = true;

	int team;
	int flags;
	int death;
	int assist;

	vec3_t origin;
	
};


typedef struct DrawTGA DrawTGA;
typedef struct DrawTEXT DrawTEXT;




bool fun_DrawTGA(char *filename, int r, int g, int b, int a, double x, double y, int center, int wide, int height, int mod, double time, int channel);
bool fun_DelTGA(int channel);
bool fun_DelTEXT(int channel);
int EngFunc_HookUserMsg(char *szMsgName, pfnUserMsgHook pfn);
int Msg_RoundState(const char *pszName, int iSize, void *pbuf);
int Msg_UpdataAmmo(const char *pszName, int iSize, void *pbuf);

int Msg_DTmsg(const char *pszName, int iSize, void *pbuf);
int Msg_Massist(const char *pszName, int iSize, void *pbuf);
int Msg_CHstyle(const char *pszName, int iSize, void *pbuf);

int Msg_DRTGA(const char *pszName, int iSize, void *pbuf);

int Msg_DTTGA(const char *pszName, int iSize, void *pbuf);

int Msg_DRTGAP(const char *pszName, int iSize, void *pbuf);
int Msg_DRTGAX(const char *pszName, int iSize, void *pbuf);
int Msg_WRadio(const char *pszName, int iSize, void *pbuf);
int Msg_Cnotice(const char *pszName, int iSize, void *pbuf);
int Msg_Bartime(const char *pszName, int iSize, void *pbuf);

int Msg_SETSB(const char *pszName, int iSize, void *pbuf);

int Msg_WWPrit(const char *pszName, int iSize, void *pbuf);

void Draw版权();
int Msg_Playerspawn(const char *pszName, int iSize, void *pbuf);
void DrawDeathMsg(clock_t * nowtime);
void DrawScoredBroad();
void Fun_showSB();
void Fun_hideSB();
void DrawKiller(clock_t *nowtime);

void Draw_Tabscorebroad();
void DrawPlayershow(); 
void DrawWpnshow(clock_t *nowtime);
void Fun_slot4();
void DrawAWP();
void DrawC4();
void DrawKillicon(clock_t *nowtime);
void DrawCrosshair();
void Draw_Radio();

void DrawWpnfx(clock_t *nowtime);

void DrawMessage(clock_t *nowtime);
void DrawLoadingBar(clock_t *nowtime);

#define VectorCopy(a, b) { (b)[0] = (a)[0]; (b)[1] = (a)[1]; (b)[2] = (a)[2]; }
struct SBstyle {
	wchar_t ModoName[64] = L"普通模式";
	wchar_t Tname[64] = L"TR";
	wchar_t CTname[64] = L"CT";
	int type = 0;
	int score = 13;

	bool isshow = true;

};

class WeaponShow {
private:
	

public:
	int Nowweapon;
	string List_weapon[7];
	int ammo1[6];
	int ammo2[6];
	void GetweaponList(const char *weapon, int wpnid);
	void Drawweaponshow(clock_t * nowtime);
	string GetNowweapon(int id);
	void wpnclear(int id);
};

int GetWpnSlot(int iWpn);

struct Message {
	int id;
	wstring message;
	int team;
	clock_t time = (clock_t)-1.0;
};


class Loadpic {
public:
	int alltime;
	clock_t time = (clock_t)-1;
	int loadd = 0;
	wstring text;
	texcache_t *loadbg;
	texcache_t *load;
	texcache_t *loadx;

};


struct Killicon
{
	int Mkill;
	int specialkill;
	int headshoot;
	int firstkill;
	bool fuchou;
	bool assista;
	clock_t nowtime;


};



class Playershow {
	texcache_t *playerbackground;
	texcache_t *HP_line;
	texcache_t *AC_line;

public:
	int MaxHealth;
	int Health = 0;
	int Batter = 0;
	char model[64];
	bool CanDraw = true;

	Playershow();

	void DrawPlayerShow();
};