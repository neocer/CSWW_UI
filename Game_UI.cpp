
#include"metahook.h"
#include"parsemsg.h"
#include "Game_UI.h"
#include<api.h>
#include <windows.h>
#include "mmsystem.h"
#include<io.h>
#pragma comment(lib,"winmm.lib")
#define DEATHFADE  0x080


using namespace std;
char g_szKillSounds[14][64] =
{
	".//cstrike//sound//MultiKill//killrace_01.wav",
	".//cstrike//sound//MultiKill//killrace_02.wav",
	".//cstrike//sound//MultiKill//killrace_03.wav",
	".//cstrike//sound//MultiKill//killrace_04.wav",
	".//cstrike//sound//MultiKill//killrace_05.wav",
	".//cstrike//sound//MultiKill//killrace_06.wav",
	".//cstrike//sound//MultiKill//firstblood.wav",
	".//cstrike//sound//MultiKill//niceshot.wav",
	".//cstrike//sound//MultiKill//goodjob.wav",
	".//cstrike//sound//MultiKill//grenadekill.wav",
	".//cstrike//sound//MultiKill//wallshot.wav",
	".//cstrike//sound//MultiKill//headshot.wav",
	".//cstrike//sound//MultiKill//revenge.wav",
	".//cstrike//sound//MultiKill//assist.wav"
};


wchar_t Radio1text[8][64] =
{
	L"无线电消息",
	L"1.掩护我！",
	L"2.拿下这个地方！",
	L"3.一定要守住这里",
	L"4.重新组队进攻",
	L"5.跟我来！",
	L"6.准备冲锋，火力支援",
	L"0.退出"
};

wchar_t Radio2text[8][64] =
{
	L"无线电消息",
	L"1.都冲上去！",
	L"2.全队撤退！",
	L"3.全对集中火力进攻！",
	L"4.必须拿下这个地区",
	L"5.前面一定守住！",
	L"6.通报你的情况！",
	L"0.退出"
};
wchar_t Radio3text[11][64] =
{
	L"无线电消息",
	L"1.收到！",
	L"2.注意，前面有敌人！",
	L"3.无需要支援！",
	L"4.周围安全！",
	L"5.我守在这里！",
	L"6.通报你的情况",
	L"7.注意，炸弹马上爆炸！",
	L"8.做不到！",
	L"9.敌人已被击毙",
	L"0.退出"
};


pfnUserMsgHook pmTeamInfo;
pfnUserMsgHook pmScoreInfo;
pfnUserMsgHook pmTeamScore;
pfnUserMsgHook pmHealth;
pfnUserMsgHook pmBattery;
pfnUserMsgHook pmWeaponList;
pfnUserMsgHook pmCurWeapon;
pfnUserMsgHook pmAmmoX;
pfnUserMsgHook pmHideWeapon;
pfnUserMsgHook pmCrosshair;
pfnUserMsgHook pmSayText;
pfnUserMsgHook pmBarTime;
pfnUserMsgHook pmBarTime2;

pfnUserMsgHook pmSetFOV;
pfnUserMsgHook pmRoundTime;
pfnUserMsgHook pmStatusIcon;
pfnUserMsgHook pmSendAudio;

extern Loadpic gloadpic;

extern DrawTGA gDrawTGA[CHANNEL];
extern DrawTEXT gDrawTEXT[CHANNEL];
extern bool showscore;
extern WeaponShow gWpnshow;
extern texture_api_t *gpTexFuncs;
extern fonts_api_t *gpFontFuncs;
extern SCREENINFO gScreen;
extern PlayerInfo g_iPlayers[33];
struct deathmsg gDeathMsg[8];
texcache_t *ScoreBroad = NULL;
extern texcache_t *backgroudpic;
extern texcache_t *ct_bar;
extern texcache_t *t_bar;
extern texcache_t *me_bar;
extern texcache_t *Deathicon[32];
extern texcache_t *killertga;
extern texcache_t *killer_bar;
extern texcache_t *Wpn_bg;
extern texcache_t *Now_wpn;

extern texcache_t *awp;
extern texcache_t *C4show;
extern texcache_t *WpnFX_bg[7];

extern texcache_t *WpnFX[7];
extern texcache_t *BAG;

extern struct Killicon KILLICON;
extern texcache_t *Miklltex[MAXKILL];
extern texcache_t *specialtex[4];
extern texcache_t *Crosshair;
extern texcache_t *firstkill[2];
extern texcache_t *assiste;

extern texcache_t *fuchou;
extern texcache_t *headshoot;
extern int CHRGB[4];
struct Message gMeaasge[MAXMSG];
extern NoticeShow *gNoticeShow;

bool Crosshairboolean = false;
string killsound;
int c4state = 0;
int gRadio;

int buystate = 0;

texcache_t *Wpn_now;
string lastwpn;
int GetWpnSlot(int iWpn);
char model[64];

extern Playershow * gPlayershow;

int tscore;
int ctscore;
int killerid = 0;
int ammo1;
int ammo2;
int Roundtime;
clock_t clockRtime;

bool mzj = false;

int Msgxpoint = 0;

clock_t weapontime;
struct SBstyle gScoreBroad;

int CTkillscore = 0, Tkillscore = 0;

bool is_playershow = false;

clock_t killyoutime = (clock_t)-1.0;


wstring GETradio(const string a);
int MsgFunc_TeamInfo(const char *pszName, int iSize, void *pbuf);
int MsgFunc_ScoreInfo(const char *pszName, int iSize, void *pbuf);
int MsgFunc_TeamScore(const char *pszName, int iSize, void *pbuf);
int MsgFunc_WeaponList(const char *pszName, int iSize, void *pbuf);
int MsgFunc_Health(const char *pszName, int iSize, void *pbuf);
int MsgFunc_Battery(const char *pszName, int iSize, void *pbuf);
int MsgFunc_CurWeapon(const char *pszName, int iSize, void *pbuf);
int MsgFunc_Crosshair(const char *pszName, int iSize, void *pbuf);
int MsgFunc_AmmoX(const char *pszName, int iSize, void *pbuf);
int MsgFunc_HideWeapon(const char *pszName, int iSize, void *pbuf);
int MsgFunc_SayText(const char *pszName, int iSize, void *pbuf);

int MsgFunc_BarTime(const char *pszName, int iSize, void *pbuf);
int MsgFunc_BarTime2(const char *pszName, int iSize, void *pbuf);
int MsgFunc_SetFOV(const char *pszName, int iSize, void *pbuf);
int MsgFunc_RoundTime(const char *pszName, int iSize, void *pbuf);
int MsgFunc_StatusIcon(const char *pszName, int iSize, void *pbuf);

int MsgFunc_SendAudio(const char *pszName, int iSize, void *pbuf);
std::wstring StringToWstring(const std::string str);
std::string WstringToString(const std::wstring str);

void Fun_showSB() {
	showscore = true;
} 



void Fun_hideSB() {
	showscore = false;


}

bool fun_DrawTGA(char *filename, int r, int g, int b, int a, double x, double y, int center, int wide, int height, int mod, double time, int channel)
{
	if (channel < CHANNEL && channel >=0)
	{
		strcpy(gDrawTGA[channel].filename, filename);

		gDrawTGA[channel].tex = gpTexFuncs->LoadTGA(filename);
		gDrawTGA[channel].x = x;
		gDrawTGA[channel].y = y;
		gDrawTGA[channel].red = r;
		gDrawTGA[channel].green = g;
		gDrawTGA[channel].blue = b;
		gDrawTGA[channel].alpha = a;
		gDrawTGA[channel].holdtime = time;
		gDrawTGA[channel].mode = mod == 0? RenderNormal : RenderAdditive;
		gDrawTGA[channel].wide = wide;
		gDrawTGA[channel].tall = height;
		gDrawTGA[channel].starttime = clock();
		if (center == 1)
		{

			gDrawTGA[channel].x = x - gDrawTGA[channel].tex->wide/2;
			gDrawTGA[channel].y = y - gDrawTGA[channel].tex->tall / 2;
		}

		gDrawTGA[channel].id = 0;
		gDrawTGA[channel].XYZ = false;

		return true;
	}
	else 
		return false;
}
bool fun_DrawTEXT(wstring text,const char font[], int size, int r, int g,  int b, int a, int x, int y ,int center, int mode, int fade, double holdtime, int channel)
{
	if (channel < CHANNEL && channel >= 0)
	{	


		gDrawTEXT[channel].text = text;
		strcpy(gDrawTEXT[channel].font, font);

		gDrawTEXT[channel].size = size;
		gDrawTEXT[channel].red = r;
		gDrawTEXT[channel].green = g;
		gDrawTEXT[channel].blue = b;
		gDrawTEXT[channel].alpha = a;
		gDrawTEXT[channel].mode = mode;
		gDrawTEXT[channel].fade = fade;
		gDrawTEXT[channel].holdtime = holdtime;
		gDrawTEXT[channel].starttime = clock();
		gDrawTEXT[channel].center = center;
		gDrawTEXT[channel].hfont = gpFontFuncs->CreateFont();
		gDrawTEXT[channel].x = x;
		gDrawTEXT[channel].y = y;
		gDrawTEXT[channel].id = 0;
		gDrawTEXT[channel].XYZ = false;
		return true;
	}
	else
		return false;



}


bool fun_DelTEXT(int channel) {
	if (channel < CHANNEL && channel >= 0)
	{
		gDrawTEXT[channel].text = L"";
	}
	else
		return false;

}
bool fun_DelTGA(int channel) {
	if (channel < CHANNEL  && channel >= 0 )
	{
		gDrawTGA[channel].filename[0] = '\0';
		return true;
	}
	else
		return false;
}



int EngFunc_HookUserMsg(char *szMsgName, pfnUserMsgHook pfn)
{
	if (!strcmp(szMsgName, "TeamInfo"))
	{
		pmTeamInfo = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_TeamInfo);
	}
	else if (!strcmp(szMsgName, "ScoreInfo"))
	{
		pmScoreInfo = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_ScoreInfo);
	}

	else if (!strcmp(szMsgName, "TeamScore"))
	{
		pmTeamScore = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_TeamScore);
	}
	else if (!strcmp(szMsgName, "Health"))
	{
		pmHealth = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_Health);
	}
	else if (!strcmp(szMsgName, "Battery"))
	{
		pmBattery = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_Battery);
	}
	if (!strcmp(szMsgName, "WeaponList"))
	{
		pmWeaponList = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_WeaponList);
	}
	if (!strcmp(szMsgName, "CurWeapon"))
	{
		pmCurWeapon = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_CurWeapon);
	}
	if (!strcmp(szMsgName, "Crosshair"))
	{
		pmCrosshair = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_Crosshair);
	}
	if (!strcmp(szMsgName, "AmmoX"))
	{
		pmAmmoX = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_AmmoX);
	}
	if (!strcmp(szMsgName, "HideWeapon"))
	{
		pmHideWeapon = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_HideWeapon);
	}
	if (!strcmp(szMsgName, "SayText"))
	{
		pmSayText = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_SayText);
	}
	if (!strcmp(szMsgName, "BarTime"))
	{
		pmBarTime = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_BarTime);
	}
	if (!strcmp(szMsgName, "BarTime2"))
	{
		pmBarTime2 = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_BarTime2);
	}
	if (!strcmp(szMsgName, "SetFOV"))
	{
		pmSetFOV = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_SetFOV);
	}
	if (!strcmp(szMsgName, "RoundTime"))
	{
		pmRoundTime = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_RoundTime);

	}
	if (!strcmp(szMsgName, "StatusIcon"))
	{
		pmStatusIcon = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_StatusIcon);

	}
	if (!strcmp(szMsgName, "SendAudio"))
	{
		pmSendAudio = pfn;
		return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_SendAudio);

	}
	
	return gEngfuncs.pfnHookUserMsg(szMsgName, pfn);
}







int MsgFunc_TeamInfo(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	int id = READ_BYTE();
	char szTeamName[32];
	strcpy(szTeamName, READ_STRING());
	int iTeam;
	if (!strcmp(szTeamName, "TERRORIST")) iTeam = 1;
	else if (!strcmp(szTeamName, "CT")) iTeam = 2;
	else iTeam = 0;
	if (0 < id && id < 33) g_iPlayers[id].team = iTeam;
	
	return pmTeamInfo(pszName, iSize, pbuf);
}

void DrawDeathMsg(clock_t * nowtime) 
{
	int wide = 0, tall = 0;
	int x, y = 0  ;
	int play1wide = 0, play1tall = 0;
	int play2wide = 0, play2tall = 0;
	int picturewide = 0, picturetall = 0;
	int x1, y1, tall1, wide1;

	wchar_t AttacterGroup[128];

	for (int i = 0; i < 8 ; i++)
	{
		if (gDeathMsg[i].time  <= 0)
			continue;
		if (* nowtime - gDeathMsg[i].time >= KILLHOLDTIME * 1000)
			break;			//为啥break？ 这个都超时了，后面的还能不超时？

		//开始画被干的名字！！！
		y = y + tall + (i==0 ? 30 : 18);

		if (gDeathMsg[i].player3.id == 0)
			swprintf(AttacterGroup, L"%ls", gDeathMsg[i].player1.name.c_str(), gDeathMsg[i].player3.name.c_str());
		else
		swprintf(AttacterGroup, L"%ls + %ls", gDeathMsg[i].player1.name.c_str(), gDeathMsg[i].player3.name.c_str());

		gpFontFuncs->GetTextSize(gDeathMsg[i].player2.font, gDeathMsg[i].player2.name.c_str(), &play2wide, &play2tall);
		gpFontFuncs->GetTextSize(gDeathMsg[i].player1.font, AttacterGroup, &play1wide, &play1tall);
		x1 = gScreen.iWidth - play2wide - 40 - 100  - 30- play1wide - (gDeathMsg[i].headshoot == true ? 35 : 0) - (gDeathMsg[i].mkill > 1? 60:0)- (gDeathMsg[i].firstkill != 0 ? 100 : 0);
		wide1 = play2wide + 100 + 50 + play1wide + (gDeathMsg[i].headshoot ==true ?35 : 0)+ (gDeathMsg[i].mkill > 1 ? 60 : 0)+ (gDeathMsg[i].firstkill != 0 ?100:0);
		tall1 = gDeathMsg[i].wpntex->tall * 0.8 ;

		y1 = y + play2tall / 2 - gDeathMsg[i].wpntex->tall / 2 + gDeathMsg[i].wpntex->tall * 0.1;

		gpTexFuncs->RenderMode(RenderNormal);
		gpTexFuncs->DrawSetTexture(gDeathMsg[i].ditu);
		gpTexFuncs->DrawSetColor(255, 255, 255, *nowtime - gDeathMsg[i].time < (KILLHOLDTIME * 1000 - 2550)*1.5 ? 255 : ((KILLHOLDTIME * 1000) - (*nowtime - gDeathMsg[i].time)) / 8);
		gpTexFuncs->DisableScissor();
		gpTexFuncs->DrawTexturedRect(x1, y1, wide1, tall1);



		gpFontFuncs->AddGlyphSetToFont(gDeathMsg[i].player2.font, "黑体", 15, 600, 0, 0, 0x010+ 0x020, 0x0, 0xFFFF);
		
		x = gScreen.iWidth - play2wide  - 40 ;
	
		gpFontFuncs->DrawSetTextFont(gDeathMsg[i].player2.font);
		if (g_iPlayers[gDeathMsg[i].player2.id].team == 1)
			gpFontFuncs->DrawSetTextColor(176, 143, 87, *nowtime - gDeathMsg[i].time < (KILLHOLDTIME * 1000 - 2550)*1.5 ? 255 : ((KILLHOLDTIME * 1000) - (*nowtime - gDeathMsg[i].time)) / 5);
		else
			gpFontFuncs->DrawSetTextColor(92, 145, 180, *nowtime - gDeathMsg[i].time < (KILLHOLDTIME * 1000 - 2550)*1.5 ? 255 : ((KILLHOLDTIME * 1000) - (*nowtime - gDeathMsg[i].time)) / 5);

		gpFontFuncs->DrawSetTextPos(x, y);
		gpFontFuncs->DrawPrintText(gDeathMsg[i].player2.name.c_str());
		tall = play2tall;
		wide = play2wide;

		x = x - 100;
		gpTexFuncs->RenderMode(RenderNormal);
		gpTexFuncs->DrawSetTexture(  gDeathMsg [i] . wpntex);
		gpTexFuncs->DrawSetColor(255,255,255, *nowtime - gDeathMsg[i].time < (KILLHOLDTIME * 1000 - 2550)*1.5 ? 255 : ((KILLHOLDTIME * 1000) - (*nowtime - gDeathMsg[i].time)) / 5);
		gpTexFuncs->DisableScissor();
		gpTexFuncs->DrawTexturedRect(x, y + tall / 2 - gDeathMsg[i].wpntex->tall / 2, gDeathMsg[i].wpntex->wide, gDeathMsg[i].wpntex->tall);



		gpFontFuncs->AddGlyphSetToFont(gDeathMsg[i].player1.font, "黑体", 15, 600, 0, 0, 0x010 + 0x020, 0x0, 0xFFFF);
		tall = play1tall;
		wide = play1wide;
		x = x - wide - 10;
		gpFontFuncs->DrawSetTextFont(gDeathMsg[i].player1.font);
		if (g_iPlayers[gDeathMsg[i].player1.id].team == 1)
			gpFontFuncs->DrawSetTextColor(176, 143, 87, *nowtime - gDeathMsg[i].time < (KILLHOLDTIME * 1000 - 2550)*1.5 ? 255 : ((KILLHOLDTIME * 1000) - (*nowtime - gDeathMsg[i].time)) / 5);
		else
			gpFontFuncs->DrawSetTextColor(92, 145, 180, *nowtime - gDeathMsg[i].time < (KILLHOLDTIME * 1000 - 2550)*1.5 ? 255 : ((KILLHOLDTIME * 1000) - (*nowtime - gDeathMsg[i].time)) / 5);
		
		gpFontFuncs->DrawSetTextPos(x, y);
		gpFontFuncs->DrawPrintText(AttacterGroup);


		if (gDeathMsg[i].headshoot == true)
		{
			x = x - 40;
			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(gDeathMsg[i].headtex);
			gpTexFuncs->DrawSetColor(255, 255, 255, *nowtime - gDeathMsg[i].time < (KILLHOLDTIME * 1000 - 2550)*1.5 ? 255 : ((KILLHOLDTIME * 1000) - (*nowtime - gDeathMsg[i].time)) / 5);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(x, y+ tall/2- gDeathMsg[i].headtex->tall /2 , gDeathMsg[i].headtex->wide, gDeathMsg[i].headtex->tall);
		}
		if (gDeathMsg[i].mkill > 1)
		{
			x = x - 60;
			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(gDeathMsg[i].mKILL);
			gpTexFuncs->DrawSetColor(255, 255, 255, *nowtime - gDeathMsg[i].time < (KILLHOLDTIME * 1000 - 2550)*1.5 ? 255 : ((KILLHOLDTIME * 1000) - (*nowtime - gDeathMsg[i].time)) / 5);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(x, y + tall / 2 - gDeathMsg[i].mKILL->tall / 2, gDeathMsg[i].mKILL->wide, gDeathMsg[i].mKILL->tall);
		}
		if (gDeathMsg[i].firstkill !=0 )
		{
			x = x - 100;
			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(gDeathMsg[i].firstkilltex);
			gpTexFuncs->DrawSetColor(255, 255, 255, *nowtime - gDeathMsg[i].time < (KILLHOLDTIME * 1000 - 2550)*1.5 ? 255 : ((KILLHOLDTIME * 1000) - (*nowtime - gDeathMsg[i].time)) / 5);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(x, y + tall / 2 - gDeathMsg[i].firstkilltex->tall / 2, gDeathMsg[i].firstkilltex->wide, gDeathMsg[i].firstkilltex->tall);
		}

		
	}


}

int Msg_DTmsg(const char *pszName, int iSize, void *pbuf) {

	BEGIN_READ(pbuf, iSize);
	int id = gEngfuncs.GetLocalPlayer()->index;
	char szWeapon[64];
	int iAttacker = READ_BYTE();				//攻击者
	int iVictim = READ_BYTE();					//被攻击者
	int iHeadshot = READ_BYTE();				//是否爆头
	int iSpecialKill = READ_BYTE();
	strcpy(szWeapon, READ_STRING());
	int iAssister = READ_BYTE();
	int Mkill = READ_BYTE();
	int firstkill = READ_BYTE();
	int fouchou = READ_BYTE();
	//gEngfuncs.Con_Printf("fouchou:%d\n", fouchou);

	clock_t nowtime = clock();
	hud_player_info_t hPlayer;
	wchar_t AttackerszText[128];
	wchar_t VictimszText[128];
	wchar_t AssisterszText[128];
	char wpnpath[128];
	char wpnpath2[128];
	gEngfuncs.pfnGetPlayerInfo(iAttacker, &hPlayer);
	swprintf(AttackerszText, UTF8ToUnicode(hPlayer.name));
	gEngfuncs.pfnGetPlayerInfo(iVictim, &hPlayer);
	swprintf(VictimszText, UTF8ToUnicode(hPlayer.name));
	if (iAssister == 0)
	{
		AssisterszText[0] = L'\0';
	}
	else
	{

		gEngfuncs.pfnGetPlayerInfo(iAssister, &hPlayer);
		swprintf(AssisterszText, UTF8ToUnicode(hPlayer.name));
	}
	g_iPlayers[iVictim].dead = true;


	
	for (int i = 6; i >= 0; i--)
	{
		gDeathMsg[i + 1] = gDeathMsg[i];
	}
	if (Mkill > 6)Mkill = 6;

	if (Mkill > 1)
	{
		char Mkilltxt[64];
		sprintf(Mkilltxt, "WW_TGA/deathmsg/SHOT_MULTIKILL%d.tga", Mkill);

		gDeathMsg[0].mKILL = gpTexFuncs->LoadTGA(Mkilltxt);

	} 
	if (firstkill == 1)
	{

		gDeathMsg[0].firstkilltex = gpTexFuncs->LoadTGA("WW_TGA/deathmsg/SHOT_FIRSTKILL.tga");
	}
	else if (firstkill == 2)
	{
		gDeathMsg[0].firstkilltex = gpTexFuncs->LoadTGA("WW_TGA/deathmsg/SHOT_LASTKILL.tga");
	}
	if (iAttacker == gEngfuncs.GetLocalPlayer()->index || iVictim == gEngfuncs.GetLocalPlayer()->index || iAssister == gEngfuncs.GetLocalPlayer()->index)

		gDeathMsg[0].ditu = gpTexFuncs->LoadTGA("WW_TGA/suv_notice_me.tga");
	else
		gDeathMsg[0].ditu = gpTexFuncs->LoadTGA("WW_TGA/suv_notice.tga");
	gDeathMsg[0].firstkill = ( iAttacker!=iVictim ? firstkill :0);
	gDeathMsg[0].mkill = Mkill;
	gDeathMsg[0].player1.name = AttackerszText;
	gDeathMsg[0].player2.name = VictimszText;
	gDeathMsg[0].player3.name = AssisterszText;
	gDeathMsg[0].headshoot = (iHeadshot == 1 ? true : false);
	gDeathMsg[0].weapon = szWeapon;
	gDeathMsg[0].time = nowtime;
	gDeathMsg[0].player1.id = iAttacker;
	gDeathMsg[0].player2.id = iVictim;
	gDeathMsg[0].player3.id = iAssister;
	gDeathMsg[0].player1.font = gpFontFuncs->CreateFont();
	gDeathMsg[0].player2.font = gpFontFuncs->CreateFont();
	
	if (iAttacker == gEngfuncs.GetLocalPlayer()->index)
	{
		KILLICON.firstkill = gDeathMsg[0].firstkill;
		KILLICON.specialkill = iSpecialKill;
		KILLICON.headshoot = iHeadshot;
		KILLICON.Mkill = Mkill;
		KILLICON.nowtime = nowtime;
		KILLICON.fuchou = (fouchou == 0 ? false : true);
		if (iHeadshot == 1 && iSpecialKill == 3)
		{
			KILLICON.specialkill = 4;

		}
		KILLICON.assista = false;
		if (gDeathMsg[0].firstkill > 0)
		{
			PlaySound(g_szKillSounds[gDeathMsg[0].firstkill +5 ], NULL, SND_FILENAME | SND_ASYNC);
		}
		else if (iHeadshot == 1 && iSpecialKill!=3)
		{
			PlaySound(g_szKillSounds[11], NULL, SND_FILENAME | SND_ASYNC);
		}
		else if (iSpecialKill > 0)
		{
			PlaySound(g_szKillSounds[7+iSpecialKill], NULL, SND_FILENAME | SND_ASYNC);
		}
		else {

			PlaySound(g_szKillSounds[Mkill-1], NULL, SND_FILENAME | SND_ASYNC);
		
		}

	}
	if (iAssister == gEngfuncs.GetLocalPlayer()->index)
	{
		KILLICON.assista = true;

		PlaySound(g_szKillSounds[13], NULL, SND_FILENAME | SND_ASYNC);
		KILLICON.nowtime = nowtime;
	}
	sprintf(wpnpath, "WW_TGA/deathmsg/%s.tga", gDeathMsg->weapon.c_str());
	sprintf(wpnpath2, ".//cstrike//WW_TGA//deathmsg//%s.tga", gDeathMsg->weapon.c_str());
	if (_access(wpnpath2, 0) != -1) {
		gDeathMsg[0].wpntex = gpTexFuncs->LoadTGA(wpnpath);
	}
	else
		gDeathMsg[0].wpntex = gpTexFuncs->LoadTGA("WW_TGA/deathmsg/kill.tga");


	if (gDeathMsg[0].headshoot == true && iSpecialKill != 3)
	{
		gDeathMsg[0].headtex = gpTexFuncs->LoadTGA("WW_TGA/deathmsg/headshot.tga");

		gDeathMsg[0].headshoot = true;
	}
	else if (gDeathMsg[0].headshoot == true && iSpecialKill == 3)
	{

		gDeathMsg[0].headtex = gpTexFuncs->LoadTGA("WW_TGA/deathmsg/SHOT_WALL_HEAD.tga");

		gDeathMsg[0].headshoot = true;
	}
	else if (iSpecialKill == 3)
	{
		gDeathMsg[0].headtex = gpTexFuncs->LoadTGA("WW_TGA/deathmsg/SHOT_WALL.tga");

		gDeathMsg[0].headshoot = true;
	
	}
	if (id == iVictim)
	{
		//fun_DelTGA(3);
		killyoutime = clock();
		killerid = iAttacker;
		is_playershow = false;


	}
	
	if (id == iAttacker && killerid == iVictim)
	{
		killerid = 0;
	}

	return 1;



}


int Msg_UpdataAmmo(const char *pszName, int iSize, void *pbuf) {


	BEGIN_READ(pbuf, iSize);

	ammo1 = READ_SHORT();
	ammo2 = READ_SHORT();

	return 1;

}
int Msg_RoundState(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);

	int state = READ_BYTE();
	int winteam = READ_BYTE();


	if (state == 0)			//开局
	{


		gEngfuncs.Con_Printf("===============================");
		gEngfuncs.Con_Printf("=                             =");
		gEngfuncs.Con_Printf("=                             =");
		gEngfuncs.Con_Printf("=                             =");
		gEngfuncs.Con_Printf("=     反恐精英：战争世界      =");
		gEngfuncs.Con_Printf("=                             =");
		gEngfuncs.Con_Printf("=                             =");
		gEngfuncs.Con_Printf("=                             =");
		gEngfuncs.Con_Printf("===============================");



		for (int i = 0; i < 8; i++)
		{
			CTkillscore = 0;
			Tkillscore = 0;
			gDeathMsg[i].time = (clock_t)-1;
			//gNoticeShow->IntTip(L"测试创建提示API！", 5);

		}
	}
	else if (state == 1)	//结束
	{
		if (winteam == g_iPlayers[gEngfuncs.GetLocalPlayer()->index].team) {
			//fun_DrawTGA("WW_TGA/win.tga",255,255,255,255,0.5,0.5,-1,-1,true,0,3.0,0);
		}
		else {
			//fun_DrawTGA("WW_TGA/lose.tga", 255, 255, 255, 255, 0.5, 0.5, -1, -1, true, 0, 3.0, 0);
		}
	}
	return 1;
}


void Draw版权() {
	static int 版权 = 0;
	if(版权==0)
		版权 = gpFontFuncs->CreateFont();

	int wide = 0, tall = 0;
	int x=0, y = 0;
	gpFontFuncs->AddGlyphSetToFont(版权, "黑体", 14, 600, 0, 0, DEATHFADE, 0x0, 0xFFFF);
	gpFontFuncs->GetTextSize(版权, L"开发者:零点创意工作室", &wide, &tall);
	x = x - wide - 10;
	gpFontFuncs->DrawSetTextFont(版权);
	gpFontFuncs->DrawSetTextColor(200, 200, 200,255);
	gpFontFuncs->DrawSetTextPos(gScreen.iWidth - wide - 3,3 );
	gpFontFuncs->DrawPrintText(L"开发者:零点创意工作室");
}


int Msg_Playerspawn(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	int id = READ_BYTE();
	g_iPlayers[id].dead = false;
	gPlayershow->MaxHealth = READ_SHORT();
	if (id == gEngfuncs.GetLocalPlayer()->index)
	{
		
		is_playershow = true;
	}
	
		
	return 1;
}




void DrawScoredBroad() {
	if (gScoreBroad.isshow == false)
		return;

	if (ScoreBroad == NULL)
	{
		ScoreBroad = gpTexFuncs->LoadTGA("WW_TGA/scoreborad.tga");
	}
	gpTexFuncs->RenderMode(RenderNormal);
	gpTexFuncs->DrawSetTexture(ScoreBroad);
	gpTexFuncs->DrawSetColor(255, 255, 255,255);
	gpTexFuncs->DisableScissor();
	gpTexFuncs->DrawTexturedRect(gScreen.iWidth/2- ScoreBroad->wide/2, 10 , ScoreBroad->wide, ScoreBroad->tall);
	static int CTnum = 0,Tnum = 0;
	if (CTnum == 0)
		CTnum = gpFontFuncs->CreateFont();
	if (Tnum == 0)
		Tnum = gpFontFuncs->CreateFont();
	int tall, wide;
	wchar_t ctsc[20];
	wchar_t tsc[20];
	if (gScoreBroad.type == 0)
	{

		swprintf(ctsc, L"%02d", ctscore);
		swprintf(tsc, L"%02d", tscore);
	}
	else
	{
		swprintf(ctsc, L"%002d", CTkillscore);
		swprintf(tsc, L"%002d", Tkillscore);

	}
	gpFontFuncs->AddGlyphSetToFont(CTnum, "时尚中黑简体", 47, 0, 0, 0, 0, 0x0, 0xFFFF);
	gpFontFuncs->GetTextSize(CTnum, ctsc , &wide, &tall);
	gpFontFuncs->DrawSetTextFont(CTnum);



	switch (g_iPlayers[gEngfuncs.GetLocalPlayer()->index].team)
	{
	case 1:
		gpFontFuncs->DrawSetTextColor(200, 200, 200, 255);
		break;
	case 2:
		gpFontFuncs->DrawSetTextColor(234, 148, 65, 255);
		break;

	default:
		gpFontFuncs->DrawSetTextColor(200, 200, 200, 255);
		break;
	}
	gpFontFuncs->DrawSetTextPos(gScreen.iWidth/2 - wide - 60 , 10);
	gpFontFuncs->DrawPrintText(ctsc);


	gpFontFuncs->AddGlyphSetToFont(Tnum, "时尚中黑简体", 47, 0, 0, 0, 0, 0x0, 0xFFFF);
	gpFontFuncs->DrawSetTextFont(Tnum);
	switch (g_iPlayers[gEngfuncs.GetLocalPlayer()->index].team)
	{
	case 1:
		gpFontFuncs->DrawSetTextColor(234, 148, 65, 255);
		break;
	case 2:
		
		gpFontFuncs->DrawSetTextColor(200, 200, 200, 255);
		break;

	default:
		gpFontFuncs->DrawSetTextColor(200, 200, 200, 255);
		break;
	}
	gpFontFuncs->DrawSetTextPos(gScreen.iWidth / 2  + 60, 10);
	gpFontFuncs->DrawPrintText(tsc);
	static int  time_f = 0;
	if (time_f == 0)
	{
		time_f = gpFontFuncs->CreateFont();
	}
	
	if (clock() - clockRtime >= 1000)
	{

		clockRtime = clock();
		if(Roundtime > 0)
			Roundtime--;
	}

	wchar_t TIME[20] , MBscore[20];
	swprintf(TIME,L"%02d:%02d",(int)Roundtime/60,(int)Roundtime%60);
	gpFontFuncs->AddGlyphSetToFont(time_f, "时尚中黑简体", 25, 600, 0, 0, 0x020, 0x0, 0xFFFF);
	gpFontFuncs->GetTextSize(time_f, TIME , &wide, &tall);
	gpFontFuncs->DrawSetTextFont(time_f);
	gpFontFuncs->DrawSetTextColor(220, 220, 220, 255);
	gpFontFuncs->DrawSetTextPos(gScreen.iWidth/2 - wide/2, 28);
	gpFontFuncs->DrawPrintText(TIME);

	int ct = 0, t = 0;
	for (int i = 0; i < 32; i++)
	{
		if (g_iPlayers[i + 1].dead == false)
		{
			if (g_iPlayers[i + 1].team == 1)
				t++;
			else if (g_iPlayers[i + 1].team == 2)
				ct++;

		}
	}
	static int  CT_P = 0, T_P=0 ,mode = 0 , mubiao = 0;
	if (CT_P == 0)
	{
		CT_P = gpFontFuncs->CreateFont();
	}
	if (T_P == 0)
	{
		T_P = gpFontFuncs->CreateFont();
	}
	if (mode == 0)
	{
		mode = gpFontFuncs->CreateFont();
	}
	if (mubiao == 0)
	{
		mubiao = gpFontFuncs->CreateFont();
	}
	swprintf(TIME, L"%ls×%d", gScoreBroad.CTname, ct);
	gpFontFuncs->AddGlyphSetToFont(CT_P, "黑体", 15, 800, 0, 0, 0x020, 0x0, 0xFFFF);
	gpFontFuncs->GetTextSize(CT_P, TIME, &wide, &tall);
	gpFontFuncs->DrawSetTextFont(CT_P);
	gpFontFuncs->DrawSetTextColor(220, 220, 220, 255);
	gpFontFuncs->DrawSetTextPos(gScreen.iWidth / 2 - wide - 60 , 55);
	gpFontFuncs->DrawPrintText(TIME);


	swprintf(TIME, L"%ls×%d", gScoreBroad.Tname, t);
	gpFontFuncs->AddGlyphSetToFont(T_P, "黑体", 15, 800, 0, 0, 0x020, 0x0, 0xFFFF);
	gpFontFuncs->DrawSetTextFont(T_P);
	gpFontFuncs->DrawSetTextColor(220, 220, 220, 255);
	gpFontFuncs->DrawSetTextPos(gScreen.iWidth / 2 +  60, 55);
	gpFontFuncs->DrawPrintText(TIME);


	if (gScoreBroad.type == 0)
	{
		swprintf(MBscore, L"%dRound", gScoreBroad.score);
	}
	else {
		swprintf(MBscore, L"%dKill", gScoreBroad.score);
	
	}


	//分数上限
	gpFontFuncs->GetTextSize(mubiao, MBscore,& wide,& tall);
	gpFontFuncs->AddGlyphSetToFont(mubiao, "黑体", 15, 800, 0, 0, 0x020, 0x0, 0xFFFF);
	gpFontFuncs->DrawSetTextFont(mubiao);
	gpFontFuncs->DrawSetTextColor(220, 220, 220, 255);
	gpFontFuncs->DrawSetTextPos(gScreen.iWidth / 2 - wide / 2 , 55);
	gpFontFuncs->DrawPrintText(MBscore);


	//模式名称
	gpFontFuncs->GetTextSize(mubiao, gScoreBroad.ModoName, &wide, &tall);
	gpFontFuncs->AddGlyphSetToFont(mubiao, "黑体", 13, 800, 0, 0, 0x020, 0x0, 0xFFFF);
	gpFontFuncs->DrawSetTextFont(mubiao);
	gpFontFuncs->DrawSetTextColor(220, 220, 220, 255);
	gpFontFuncs->DrawSetTextPos(gScreen.iWidth / 2 - wide / 2, 13);
	gpFontFuncs->DrawPrintText(gScoreBroad.ModoName);


}
int MsgFunc_ScoreInfo(const char *pszName, int iSize, void *pbuf) {
	BEGIN_READ(pbuf, iSize);
	int id = READ_BYTE();
	int score = READ_SHORT();
	g_iPlayers[id].flags = score < 0 ? 0:score;
	g_iPlayers[id].death = READ_SHORT();

	READ_SHORT();
	READ_SHORT();
	Tkillscore = 0;
	CTkillscore = 0;

	for (int i = 1; i < 33; i++)
	{
		cl_entity_s *pEntity = gEngfuncs.GetEntityByIndex(i);
		if (!pEntity) continue;
		if (g_iPlayers[i].team == 1)
			Tkillscore += g_iPlayers[i].flags;
		else if (g_iPlayers[i].team == 2)
		{
			CTkillscore += g_iPlayers[i].flags;
		}
	}


	return pmScoreInfo(pszName, iSize, pbuf);
}


void Draw_Tabscorebroad() {


	if (showscore == true)
	{
		
			int pictall;

			static int line[32];
			static int linescore[32];
			wstring scoretext[32];

			wstring scoretext2[32];
			wchar_t tmps[128];
			wchar_t tmp[128];
			int iid[32];
			int tmpnumb;


			hud_player_info_t hPlayer;
			wchar_t name[64], other[128];
			wchar_t score[64];
			int x = gScreen.iWidth/2-backgroudpic->wide/2, y = 100;
			int tall, wide;
			bool link[32];

			for (int i = 0; i < 32; i++)
			{
				iid[i] = i + 1;
			}
			int team;
			for (int i = 0; i<32; i++)
			{
				for (int j = i + 1; j < 32; j++)
				{
					if (g_iPlayers[iid[i]].flags < g_iPlayers[iid[j]].flags || g_iPlayers[iid[i]].flags == g_iPlayers[iid[j]].flags && g_iPlayers[iid[i]].death > g_iPlayers[iid[j]].death)
					{
						tmpnumb = iid[i];
						iid[i] = iid[j];
						iid[j] = tmpnumb;


					}
				}
			}

			for (int i = 0; i < 32; i++)
			{
			
				if (line[i] == 0)
					line[i] = gpFontFuncs->CreateFont();
				if (linescore[i] == 0)
					linescore[i] = gpFontFuncs->CreateFont();
				gEngfuncs.pfnGetPlayerInfo(i+1, &hPlayer);
				swprintf(name, UTF8ToUnicode(hPlayer.name));

				if (!lstrlenW(name))
				{
					link[i] = false;
					continue;
				}
				link[i] = true;
				swprintf(score, L"%d/%d/%d", g_iPlayers[i + 1].flags, g_iPlayers[i + 1].assist, g_iPlayers[i + 1].death);
				swprintf(tmps, L"%-10ls%4d", score,(int)hPlayer.ping);
				swprintf(tmp, L"%ls", name);
				scoretext[i] = tmp;
				scoretext2[i] = tmps;

				gpFontFuncs->GetTextSize(line[i], scoretext[i].c_str(), &wide, &tall);
			
				y = y + 5 + tall;

			}
			

			pictall = y - 100  +  t_bar->tall + ct_bar->tall    +2*5+ 40;

			y = gScreen.iHeight / 2 - pictall/2;

			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(backgroudpic);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(x,y, backgroudpic->wide, pictall);

			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(t_bar);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(x, y, t_bar->wide, t_bar->tall);


			x = x;
			y = y + t_bar->tall + 5;
			for (int i = 0; i < 32; i++) {
				if (link[iid[i] - 1] == false) continue;
				if (g_iPlayers[iid[i]].team == 2) continue;
				if (iid[i] == gEngfuncs.GetLocalPlayer()->index)
				{
					gpTexFuncs->RenderMode(RenderNormal);
					gpTexFuncs->DrawSetTexture(me_bar);
					gpTexFuncs->DrawSetColor(255, 255, 255, 255);
					gpTexFuncs->DisableScissor();
					gpTexFuncs->DrawTexturedRect(x+(backgroudpic->wide - me_bar->wide)/2, y-( me_bar->tall - tall)/2, me_bar->wide, me_bar->tall);
				}
				if (iid[i] == killerid)
				{
					gpTexFuncs->RenderMode(RenderNormal);
					gpTexFuncs->DrawSetTexture(killer_bar);
					gpTexFuncs->DrawSetColor(255, 255, 255, 255);
					gpTexFuncs->DisableScissor();
					gpTexFuncs->DrawTexturedRect(x + (backgroudpic->wide - killer_bar->wide) / 2, y - (killer_bar->tall - tall) / 2, killer_bar->wide, killer_bar->tall);
				}

				if (g_iPlayers[iid[i]].dead == true)
				{
					gpTexFuncs->RenderMode(RenderNormal);
					gpTexFuncs->DrawSetTexture(Deathicon[iid[i]-1]);
					gpTexFuncs->DrawSetColor(255, 255, 255, 255);
					gpTexFuncs->DisableScissor();
					gpTexFuncs->DrawTexturedRect(x+ 37, y - (Deathicon[iid[i]-1]->tall - tall) / 2, Deathicon[iid[i] - 1]->wide, Deathicon[iid[i] - 1]->tall);
				}

				gpFontFuncs->GetTextSize(line[iid[i]-1], scoretext[iid[i]-1].c_str(), &wide, &tall);
				gpFontFuncs->AddGlyphSetToFont(line[iid[i]-1], "宋体", 14, 600, 0, 0, 0, 0x0, 0xFFFF);
				gpFontFuncs->DrawSetTextFont(line[iid[i] - 1 ]);
				gpFontFuncs->DrawSetTextColor(176, 143, 87, 255);
				gpFontFuncs->DrawSetTextPos(x+139, y);
				gpFontFuncs->DrawPrintText(scoretext[iid[i] - 1].c_str());

				gpFontFuncs->GetTextSize(linescore[iid[i] - 1], scoretext2[iid[i] - 1].c_str(), &wide, &tall);
				gpFontFuncs->AddGlyphSetToFont(linescore[iid[i] - 1], "宋体", 14, 600, 0, 0, 0, 0x0, 0xFFFF);
				gpFontFuncs->DrawSetTextFont(linescore[iid[i] - 1]);
				gpFontFuncs->DrawSetTextColor(176, 143, 87, 255);
				gpFontFuncs->DrawSetTextPos(x + 350, y);
				gpFontFuncs->DrawPrintText(scoretext2[iid[i] - 1].c_str());


				y = y + 5 + tall;
			}

			y = y - 5 + 20 ;
			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(ct_bar);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(x, y-3, ct_bar->wide, ct_bar->tall);
			
			y = y + ct_bar->tall + 5;

			for (int i = 0; i < 32; i++) {
				if (link[iid[i]-1] == false) continue;
				if (g_iPlayers[iid[i]].team ==1) continue;
				if (iid[i] == gEngfuncs.GetLocalPlayer()->index)
				{
					gpTexFuncs->RenderMode(RenderNormal);
					gpTexFuncs->DrawSetTexture(me_bar);
					gpTexFuncs->DrawSetColor(255, 255, 255, 255);
					gpTexFuncs->DisableScissor();
					gpTexFuncs->DrawTexturedRect(x + (backgroudpic->wide - killer_bar->wide) / 2, y - (killer_bar->tall - tall) / 2, killer_bar->wide, killer_bar->tall);
				}

				if (iid[i] == killerid)
				{
					gpTexFuncs->RenderMode(RenderNormal);
					gpTexFuncs->DrawSetTexture(killer_bar);
					gpTexFuncs->DrawSetColor(255, 255, 255, 255);
					gpTexFuncs->DisableScissor();
					gpTexFuncs->DrawTexturedRect(x + (backgroudpic->wide - me_bar->wide) / 2, y - (me_bar->tall - tall) / 2, me_bar->wide, me_bar->tall);
				}



				if (g_iPlayers[iid[i]].dead == true)
				{
					gpTexFuncs->RenderMode(RenderNormal);
					gpTexFuncs->DrawSetTexture(Deathicon[iid[i] -1 ]);
					gpTexFuncs->DrawSetColor(255, 255, 255, 255);
					gpTexFuncs->DisableScissor();
					gpTexFuncs->DrawTexturedRect(x+37, y - (Deathicon[iid[i] - 1 ]->tall - tall) / 2, Deathicon[iid[i] - 1 ]->wide, Deathicon[iid[i] - 1 ]->tall);
				}
				gpFontFuncs->GetTextSize(line[iid[i]- 1 ], scoretext[iid[i] - 1].c_str(), &wide, &tall);
				gpFontFuncs->AddGlyphSetToFont(line[iid[i]- 1 ], "宋体", 14, 600, 0, 0, 0, 0x0, 0xFFFF);
				gpFontFuncs->DrawSetTextFont(line[iid[i]- 1 ]);
				gpFontFuncs->DrawSetTextColor(92, 145, 180, 255);
				gpFontFuncs->DrawSetTextPos(x+ 139, y);
				gpFontFuncs->DrawPrintText(scoretext[iid[i] - 1].c_str());

				gpFontFuncs->GetTextSize(linescore[iid[i] - 1], scoretext2[iid[i] - 1].c_str(), &wide, &tall);
				gpFontFuncs->AddGlyphSetToFont(linescore[iid[i] - 1], "宋体", 14, 600, 0, 0, 0, 0x0, 0xFFFF);
				gpFontFuncs->DrawSetTextFont(linescore[iid[i] - 1]);
				gpFontFuncs->DrawSetTextColor(92, 145, 180, 255);
				gpFontFuncs->DrawSetTextPos(x + 350, y);
				gpFontFuncs->DrawPrintText(scoretext2[iid[i] - 1].c_str());


				y = y + 5 + tall;
			}

	}
}


int MsgFunc_TeamScore(const char *pszName, int iSize, void *pbuf) {

	char team[64];
	int score;
	BEGIN_READ(pbuf, iSize);
	strcpy(team, READ_STRING());
	score = READ_BYTE();

	if (!strcmp(team, "TERRORIST"))
	{
		tscore = score;
	}
	else if(!strcmp(team, "CT")){
		ctscore = score;
	}

	
	return pmTeamScore(pszName, iSize, pbuf);

}


void DrawKiller(clock_t *nowtime) {
	if (*nowtime - killyoutime < 5000)
	{
		cl_entity_t *pEntity = NULL;
		if (killerid == gEngfuncs.GetLocalPlayer()->index)
			return;

		pEntity = gEngfuncs.GetEntityByIndex(killerid);

		if (!pEntity)
			return;
		if (g_iPlayers[killerid].dead)
			return;


		vec3_t fOrigin;
		VectorCopy(pEntity->curstate.origin, fOrigin);

		fOrigin[2] += 50;
		float vecScreen[2];
		if (!gEngfuncs.pTriAPI->WorldToScreen(fOrigin, vecScreen))
		{
			int iScreenWidth, iScreenHeight;
			g_pMetaHookAPI->GetVideoMode(&iScreenWidth, &iScreenHeight, NULL, NULL);
			vecScreen[0] = (1.0f + (vecScreen[0])) * iScreenWidth * 0.5f;
			vecScreen[1] = (1.0f - (vecScreen[1])) * iScreenHeight * 0.5f;




			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(killertga);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(vecScreen[0] - (killertga->wide / 2), vecScreen[1], killertga->wide, killertga->tall);
		}


	}


}



int MsgFunc_Health(const char *pszName, int iSize, void *pbuf) {
	
	BEGIN_READ(pbuf, iSize);
	gPlayershow->Health = READ_BYTE();
	

	return pmHealth(pszName, iSize, pbuf);
}


int MsgFunc_Battery(const char *pszName, int iSize, void *pbuf) {

	BEGIN_READ(pbuf, iSize);
	gPlayershow->Batter = READ_BYTE();
	return pmBattery(pszName, iSize, pbuf);
}
void Playershow::DrawPlayerShow()
{
	if (CanDraw == false)
		return;
	if (g_iPlayers[gEngfuncs.GetLocalPlayer()->index].dead == true)
		return;
	static texcache_t *player;

	char modelpic[64];
	hud_player_info_t hPlayer;
	gEngfuncs.pfnGetPlayerInfo(gEngfuncs.GetLocalPlayer()->index, &hPlayer);
	cl_entity_t *pEntity = NULL;


	pEntity = gEngfuncs.GetEntityByIndex(gEngfuncs.GetLocalPlayer()->index);


	if (strcmp(hPlayer.model, this->model))
	{
		sprintf(modelpic, "WW_TGA/%s.tga", hPlayer.model);
		strcpy(this->model, hPlayer.model);
		player = gpTexFuncs->LoadTGA(modelpic);
	}





	gpTexFuncs->RenderMode(RenderNormal);
	gpTexFuncs->DrawSetTexture(player);
	gpTexFuncs->DrawSetColor(255, 255, 255, 255);
	gpTexFuncs->DisableScissor();
	gpTexFuncs->DrawTexturedRect(10, gScreen.iHeight - player->tall - 10, player->wide, player->tall);

	Msgxpoint = gScreen.iHeight - player->tall - 10;
	gpTexFuncs->RenderMode(RenderNormal);
	gpTexFuncs->DrawSetTexture(this->playerbackground);
	gpTexFuncs->DrawSetColor(255, 255, 255, 255);
	gpTexFuncs->DisableScissor();
	gpTexFuncs->DrawTexturedRect(70, gScreen.iHeight - this->playerbackground->tall - 10, this->playerbackground->wide, this->playerbackground->tall);


	gpTexFuncs->RenderMode(RenderNormal);
	gpTexFuncs->DrawSetTexture(this->HP_line);
	gpTexFuncs->DrawSetColor(255, 255, 255, 255);
	gpTexFuncs->DisableScissor();
	gpTexFuncs->DrawTexturedRect(107, gScreen.iHeight - this->playerbackground->tall - 10, this->Health == 0 ? 0 : 244 * ((this->Health *1.0) / this->MaxHealth), 24);
	if (this->Batter > 0)
	{
		gpTexFuncs->RenderMode(RenderNormal);
		gpTexFuncs->DrawSetTexture(this->AC_line);
		gpTexFuncs->DrawSetColor(255, 255, 255, 255);
		gpTexFuncs->DisableScissor();
		gpTexFuncs->DrawTexturedRect(107, gScreen.iHeight - this->playerbackground->tall - 10 + 30, 244 * ((this->Batter * 1.0) / 100), 7);

	}

	static int HPfont = 0;
	if (HPfont == 0)
	{
		HPfont = gpFontFuncs->CreateFont();

	}
	int wide, tall;
	wchar_t HP[10];
	swprintf(HP, L"%03d", this->Health);
	gpFontFuncs->AddGlyphSetToFont(HPfont, "时尚中黑简体", 30, 0, 0, 0, 0x010, 0x0, 0xFFFF);
	gpFontFuncs->GetTextSize(HPfont, HP, &wide, &tall);
	gpFontFuncs->DrawSetTextFont(HPfont);
	gpFontFuncs->DrawSetTextColor(255, 255, 255, 255);
	gpFontFuncs->DrawSetTextPos(117, gScreen.iHeight - playerbackground->tall - 10 + 12 - tall / 2);
	gpFontFuncs->DrawPrintText(HP);


}

Playershow::Playershow() {
	this->playerbackground = gpTexFuncs->LoadTGA("WW_TGA/HPAC.tga");
	this->HP_line = gpTexFuncs->LoadTGA("WW_TGA/hp.tga");
	this->AC_line = gpTexFuncs->LoadTGA("WW_TGA/ac.tga");

}


/*


void DrawPlayershow() {
	if (is_playershow == true  && g_iHealth != 0) {
	
		static texcache_t *player;

		char modelpic[64];
		hud_player_info_t hPlayer;
		gEngfuncs.pfnGetPlayerInfo(gEngfuncs.GetLocalPlayer()->index, &hPlayer);
		//gEngfuncs.Con_Printf("model :%s \n", hPlayer.model);

		cl_entity_t *pEntity = NULL;


		pEntity = gEngfuncs.GetEntityByIndex(gEngfuncs.GetLocalPlayer()->index);


		if (strcmp(hPlayer.model, model))
		{
			sprintf(modelpic, "WW_TGA/%s.tga", hPlayer.model);
			strcpy(model, hPlayer.model);
			player = gpTexFuncs->LoadTGA(modelpic);
		}





		gpTexFuncs->RenderMode(RenderNormal);
		gpTexFuncs->DrawSetTexture(player);
		gpTexFuncs->DrawSetColor(255, 255, 255, 255);
		gpTexFuncs->DisableScissor();
		gpTexFuncs->DrawTexturedRect(10, gScreen.iHeight - player->tall - 10, player->wide, player->tall);

		Msgxpoint = gScreen.iHeight - player->tall - 10;
		gpTexFuncs->RenderMode(RenderNormal);
		gpTexFuncs->DrawSetTexture(playerbackground);
		gpTexFuncs->DrawSetColor(255, 255, 255, 255);
		gpTexFuncs->DisableScissor();
		gpTexFuncs->DrawTexturedRect(70, gScreen.iHeight - playerbackground->tall - 10, playerbackground->wide, playerbackground->tall);


		gpTexFuncs->RenderMode(RenderNormal);
		gpTexFuncs->DrawSetTexture(HP_line);
		gpTexFuncs->DrawSetColor(255, 255, 255, 255);
		gpTexFuncs->DisableScissor();
		gpTexFuncs->DrawTexturedRect(107, gScreen.iHeight - playerbackground->tall - 10, g_iHealth == 0 ? 0 : 244 * ((g_iHealth*1.0) / full_Health), 24);
		if (g_iBattery > 0 && full_Battery > 0)
		{
			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(AC_line);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(107, gScreen.iHeight - playerbackground->tall - 10 + 30, 244 * ((g_iBattery * 1.0) / full_Battery), 7);

		}

		static int HPfont = 0;
		if (HPfont == 0)
		{
			HPfont = gpFontFuncs->CreateFont();

		}
		int wide, tall;
		wchar_t HP[10];
		swprintf(HP, L"%03d", g_iHealth);
		gpFontFuncs->AddGlyphSetToFont(HPfont, "时尚中黑简体", 30, 0, 0, 0, 0x010, 0x0, 0xFFFF);
		gpFontFuncs->GetTextSize(HPfont, HP, &wide, &tall);
		gpFontFuncs->DrawSetTextFont(HPfont);
		gpFontFuncs->DrawSetTextColor(255, 255, 255, 255);
		gpFontFuncs->DrawSetTextPos(117, gScreen.iHeight - playerbackground->tall - 10 + 12 - tall / 2);
		gpFontFuncs->DrawPrintText(HP);



	}

}


*/
int MsgFunc_WeaponList(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	char szName[32], iSlot, iID;
	sprintf(szName, "%s", READ_STRING()); // name
	READ_CHAR(); // ammotype
	READ_BYTE(); //max1
	READ_CHAR(); // ammotype2
	READ_BYTE(); // max2
	iSlot = READ_CHAR(); //slot
	READ_CHAR(); // slotpos
	iID = READ_CHAR(); // id
	int flag = READ_BYTE(); // flags
	if (GetWpnSlot(iID) <7  && GetWpnSlot(iID) >=0)
	{
		
		gWpnshow.GetweaponList(szName, GetWpnSlot(iID));
		weapontime = clock();
	}
	else gEngfuncs.Con_Printf("[SWFX]:MsgWeaponList:Bad slot %d name %s \n", iSlot, szName);
	return pmWeaponList(pszName, iSize, pbuf);
}


int MsgFunc_CurWeapon(const char *pszName, int iSize, void *pbuf)
{
	
	
	
	
	
	BEGIN_READ(pbuf, iSize);
	int iActive = READ_BYTE();
	int iWpn = READ_BYTE();
	READ_BYTE();
	
	if(GetWpnSlot(iWpn) >6 || GetWpnSlot(iWpn) < 0) return pmCurWeapon(pszName, iSize, pbuf);
	if (GetWpnSlot(iWpn) != gWpnshow.Nowweapon && iActive )
	{
		int slot = GetWpnSlot(iWpn);
		if (slot != -1)
		{

			
			gWpnshow.Nowweapon = slot;

			
			weapontime = clock();

		}

		
	}
	return pmCurWeapon(pszName, iSize, pbuf);
}

int MsgFunc_AmmoX(const char *pszName, int iSize, void *pbuf)
{
	return pmAmmoX(pszName, iSize, pbuf);
}

int MsgFunc_Crosshair(const char *pszName, int iSize, void *pbuf) {

	return pmCrosshair(pszName, iSize, pbuf);
}

int MsgFunc_HideWeapon(const char *pszName, int iSize, void *pbuf) {
	BEGIN_READ(pbuf, iSize);
	int g_iHideWeapon = READ_BYTE();
	if (g_iHideWeapon & (1 << 6)) {
		Crosshairboolean = false;
	}
	else {
		Crosshairboolean = true;

		//fun_DrawTGA("WW_TGA/front_sight.tga", 0, 255, 0, 255, 0.5, 0.5, -1, -1, true, 0, -3.0, 3);
	}

	return pmHideWeapon(pszName, iSize, pbuf);
}

int MsgFunc_SayText(const char *pszName, int iSize, void *pbuf) {
	BEGIN_READ(pbuf, iSize);

	string Message[3];
	int ID = READ_BYTE();
	for (int i = 0; i < 3; i++)
		Message[i] = READ_STRING();
	for (int i = MAXMSG - 2; i >=0  ; i--)
	{

		gMeaasge[i + 1] = gMeaasge[i];
	}

	gMeaasge[0].id = ID;
	//gMeaasge[0].message = Message[2];
	gMeaasge[0].time = clock();

	//gEngfuncs.Con_Printf("state:%s\n", gMeaasge[0].message.c_str());
	if (!strcmp(Message[0].c_str(), "#Cstrike_Chat_CT"))
	{
		gMeaasge[0].team = 2;

	}
	else if (!strcmp(Message[0].c_str(), "#Cstrike_Chat_All"))
	{
		gMeaasge[0].team = 0;
	}
	if (!strcmp(Message[0].c_str(), "#Cstrike_Chat_T"))
	{
		gMeaasge[0].team = 1;
	}

	wchar_t Msg[128];
	wchar_t name[128];
	wchar_t message[128];
	hud_player_info_t hPlayer;
	gEngfuncs.pfnGetPlayerInfo(gMeaasge[0].id, &hPlayer);
	swprintf(name, UTF8ToUnicode(hPlayer.name));

	if (g_iPlayers[gMeaasge[0].id].dead == false)
	{
		swprintf(Msg, L"%ls：%ls", name, UTF8ToUnicode(Message[2].c_str()));

	}
	else
		swprintf(Msg, L"%ls(死亡)：%ls", name, UTF8ToUnicode(Message[2].c_str()));

	if (gMeaasge[0].team == 0)
	{
		swprintf(message, L"[全部]%ls", Msg);
	}
	else
	{
		swprintf(message, L"[队伍]%ls", Msg);
	}
	gMeaasge[0].message = message;



	clock_t time = clock();

	
	return pmSayText(pszName, iSize, pbuf);
}
void WeaponShow::GetweaponList(const char *weapon, int wpnid)
{

	if (wpnid > 6 || wpnid < 0) return;
	this->List_weapon[wpnid] = strlen(weapon) >7 ? weapon + 7:weapon;
	if (strlen(weapon) > 7)
		weapontime = clock();
	char wpntga[64];
	sprintf(wpntga, "WW_TGA/weapons/%s.tga", this->List_weapon[wpnid].c_str());
	WpnFX[wpnid] = gpTexFuncs->LoadTGA(wpntga);


}

void WeaponShow::Drawweaponshow(clock_t *a) {

	
}
 string WeaponShow::GetNowweapon(int id) {


	return this->List_weapon[this->Nowweapon];
}


 void WeaponShow::wpnclear(int id) {
 
	 if (id > 6 || id < 0) return;
		this->List_weapon[id] = "";
 
 }
void DrawWpnshow(clock_t *nowtime) {
	if (is_playershow == true) {
		

		static int fontammo1 = 0;
		static int fontammo2 = 0;

		if(gWpnshow.Nowweapon <2 )
		{


			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(Wpn_bg);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(gScreen.iWidth - 300 - 30, gScreen.iHeight - 39, 300, 29);


			if (fontammo1 == 0)
				fontammo1 = gpFontFuncs->CreateFont();
			if (fontammo2 == 0)
				fontammo2 = gpFontFuncs->CreateFont();
			WCHAR charammo1[10];
			WCHAR charammo2[10];
			
			swprintf_s(charammo1, L"%03d", ammo1);
			swprintf_s(charammo2, L"%03d", ammo2);
			
			int tall, wide;
			int tall2, wide2;
			gpFontFuncs->AddGlyphSetToFont(fontammo1, "时尚中黑简体", 50, 0, 0, 0, 0x010, 0x0, 0xFFFF);
			gpFontFuncs->GetTextSize(fontammo1, charammo1, &wide, &tall);
			gpFontFuncs->DrawSetTextFont(fontammo1);
			gpFontFuncs->DrawSetTextColor(255, 255, 255, 255);
			gpFontFuncs->DrawSetTextPos(gScreen.iWidth - 300 - 20, gScreen.iHeight - 39 + 29 - tall);
			gpFontFuncs->DrawPrintText(charammo1);


			gpFontFuncs->AddGlyphSetToFont(fontammo2, "时尚中黑简体", 35, 0, 0, 0, 0x010, 0x0, 0xFFFF);
			gpFontFuncs->GetTextSize(fontammo2, charammo1, &wide2, &tall2);
			gpFontFuncs->DrawSetTextFont(fontammo2);
			gpFontFuncs->DrawSetTextColor(255, 255, 255, 255);
			gpFontFuncs->DrawSetTextPos(gScreen.iWidth - 300 - 20 + wide + 5, gScreen.iHeight - 39 +29 - tall2);
			gpFontFuncs->DrawPrintText(charammo2);

		}
		else if (gWpnshow.Nowweapon == 3 || (gWpnshow.Nowweapon == 5 || (gWpnshow.Nowweapon == 6)))
		{
			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(Wpn_bg);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(gScreen.iWidth - 300 - 30, gScreen.iHeight - 39, 300, 29);


			
			if (fontammo2 == 0)
				fontammo2 = gpFontFuncs->CreateFont();

			WCHAR charammo2[10];

			swprintf_s(charammo2, L"%d", ammo2);

			int tall, wide;
			int tall2, wide2;
			gpFontFuncs->AddGlyphSetToFont(fontammo1, "时尚中黑简体", 50, 0, 0, 0, 0x010, 0x0, 0xFFFF);
			gpFontFuncs->GetTextSize(fontammo1, charammo2, &wide, &tall);
			gpFontFuncs->DrawSetTextFont(fontammo1);
			gpFontFuncs->DrawSetTextColor(255, 255, 255, 255);
			gpFontFuncs->DrawSetTextPos(gScreen.iWidth - 300 - 20 + 60 , gScreen.iHeight - 39 + 29 - tall);
			gpFontFuncs->DrawPrintText(charammo2);



		}

		if (strcmp(lastwpn.c_str(), gWpnshow.GetNowweapon(gWpnshow.Nowweapon).c_str()))
		{
			lastwpn = gWpnshow.GetNowweapon(gWpnshow.Nowweapon);
			char wpnshow[64];

			sprintf(wpnshow, "WW_TGA/weapons/%s.tga", gWpnshow.GetNowweapon(gWpnshow.Nowweapon).c_str());
			Now_wpn = gpTexFuncs->LoadTGA(wpnshow);
		}

		gpTexFuncs->RenderMode(RenderNormal);
		gpTexFuncs->DrawSetTexture(Now_wpn);
		gpTexFuncs->DrawSetColor(255, 255, 255, 255);
		gpTexFuncs->DisableScissor();
		gpTexFuncs->DrawTexturedRect(gScreen.iWidth - Now_wpn->wide, gScreen.iHeight - 10 - Now_wpn->tall - 15, Now_wpn->wide, Now_wpn->tall);

	}



	

	 
}

void DrawWpnfx(clock_t *nowtime)
{
	if (!(is_playershow == true))
		return;

	clock_t C_time = *nowtime - weapontime;
	if (C_time > (WPNSHOWFX * 1000)) C_time = WPNSHOWFX * 1000;
	int spointX[5];
	for (int i = 0; i < 5; i++)
	{
		spointX[i] = gScreen.iWidth -10 - WpnFX_bg[0]->wide - (WpnFX_bg[0]->wide * 0.5) * (1-(C_time*1.0 / (WPNSHOWFX * 1000)));

	}
	
	spointX[gWpnshow.Nowweapon>4?3: gWpnshow.Nowweapon] = gScreen.iWidth - 10 - WpnFX_bg[0]->wide - (WpnFX_bg[0]->wide * 0.5) *  (C_time*1.0 / (WPNSHOWFX * 1000));
	int spointY = gScreen.iHeight / 2 - (WpnFX_bg[0]->tall * 5 + 4 * WPNJG ) /2;
	if(gWpnshow.List_weapon[4].c_str()[0] =='\0') 
		spointY += (WpnFX_bg[0]->tall + WPNJG);
	if (*nowtime - weapontime < ((WPNSHOWFX + 0.5 )* 1000))
	{


		for (int i = 0; i < 5; i++)
		{
			if (gWpnshow.List_weapon[4].c_str()[0] == '\0' && i == 4) break;

			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(WpnFX_bg[i]);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(spointX[i], spointY, WpnFX_bg[i]->wide, WpnFX_bg[i]->tall);

			if (i != 3)
			{
				gpTexFuncs->RenderMode(RenderNormal);
				gpTexFuncs->DrawSetTexture(WpnFX[i]);
				gpTexFuncs->DrawSetColor(255, 255, 255, 255);
				gpTexFuncs->DisableScissor();
				gpTexFuncs->DrawTexturedRect(spointX[i], spointY, WpnFX[i]->wide, WpnFX[i]->tall);



			}
			if (i == 3)
			{	

				int mount = 0;
				if (gWpnshow.List_weapon[3].c_str()[0] != '\0')
					mount++;
				if (gWpnshow.List_weapon[5].c_str()[0] != '\0')
					mount++;
				if (gWpnshow.List_weapon[6].c_str()[0] != '\0')
					mount++;
				string hewpn[3];
				
				if (mount != 0)
				{
					if (mount == 1)
					{
						int index = 0;
						if (gWpnshow.List_weapon[3].c_str()[0] != '\0')
							index = 3;
						else if (gWpnshow.List_weapon[5].c_str()[0] != '\0')
							index = 5;
						else if (gWpnshow.List_weapon[6].c_str()[0] != '\0')
							index = 6;
						else continue;
						gpTexFuncs->RenderMode(RenderNormal);
						gpTexFuncs->DrawSetTexture(WpnFX[index]);
						gpTexFuncs->DrawSetColor(255, 255, 255, 255);
						gpTexFuncs->DisableScissor();
						gpTexFuncs->DrawTexturedRect(spointX[3], spointY, WpnFX[index]->wide, WpnFX[index]->tall);



					}
					else if (mount == 2)
					{
						int index[2];
						if (gWpnshow.List_weapon[3].c_str()[0] != '\0')
						{
							index[0] = 3;
							if (gWpnshow.List_weapon[5].c_str()[0] != '\0')
								index[1] = 5;
							else if (gWpnshow.List_weapon[6].c_str()[0] != '\0')
								index[1] = 6;
						}	
						else if (gWpnshow.List_weapon[5].c_str()[0] != '\0') 
						{
						
							index[0] = 5;
							index[1] = 6;
						}
						else continue;
						gpTexFuncs->RenderMode(RenderNormal);
						gpTexFuncs->DrawSetTexture(WpnFX[index[0]]);
						gpTexFuncs->DrawSetColor(255, 255, 255, 255);
						gpTexFuncs->DisableScissor();
						gpTexFuncs->DrawTexturedRect(spointX[3]-25, spointY, WpnFX[index[0]]->wide, WpnFX[index[0]]->tall);


						gpTexFuncs->RenderMode(RenderNormal);
						gpTexFuncs->DrawSetTexture(WpnFX[index[1]]);
						gpTexFuncs->DrawSetColor(255, 255, 255, 255);
						gpTexFuncs->DisableScissor();
						gpTexFuncs->DrawTexturedRect(spointX[3]+25, spointY, WpnFX[index[1]]->wide, WpnFX[index[1]]->tall);
					
					
					}
					else if (mount == 3)
					{

						gpTexFuncs->RenderMode(RenderNormal);
						gpTexFuncs->DrawSetTexture(WpnFX[3]);
						gpTexFuncs->DrawSetColor(255, 255, 255, 255);
						gpTexFuncs->DisableScissor();
						gpTexFuncs->DrawTexturedRect(spointX[3] - 50, spointY, WpnFX[3]->wide, WpnFX[3]->tall);


						gpTexFuncs->RenderMode(RenderNormal);
						gpTexFuncs->DrawSetTexture(WpnFX[5]);
						gpTexFuncs->DrawSetColor(255, 255, 255, 255);
						gpTexFuncs->DisableScissor();
						gpTexFuncs->DrawTexturedRect(spointX[3], spointY, WpnFX[5]->wide, WpnFX[5]->tall);


						gpTexFuncs->RenderMode(RenderNormal);
						gpTexFuncs->DrawSetTexture(WpnFX[6]);
						gpTexFuncs->DrawSetColor(255, 255, 255, 255);
						gpTexFuncs->DisableScissor();
						gpTexFuncs->DrawTexturedRect(spointX[3] + 50, spointY, WpnFX[6]->wide, WpnFX[6]->tall);


					}



				}

			}





			spointY += (WpnFX_bg[i]->tall + WPNJG);

		}
		
		
		

	}


}
int GetWpnSlot(int iWpn)
{
	 if (WPN_BIT_1 & (1 << iWpn)) return 0;
	 else if (WPN_BIT_2 & (1 << iWpn)) return 1;
	 else  if (WPN_BIT_3 & (1 << iWpn)) return 2;
	 else  if (WPN_BIT_4 & (1 << iWpn)) return 3;
	 else if (WPN_BIT_5 & (1 << iWpn)) return 4;
	 else  if (WPN_BIT_42 & (1 << iWpn)) return 5;
	 else  if (WPN_BIT_43 & (1 << iWpn)) return 6;
	 else return -1;
}


void Fun_slot4()
{
	if (gWpnshow.List_weapon[5].c_str()[0] == '\0' && gWpnshow.List_weapon[3].c_str()[0] == '\0' && gWpnshow.List_weapon[6].c_str()[0] == '\0')
	{
		return;
	}
	if (gRadio != 0)
	{
		gEngfuncs.pfnServerCmd("menuselect 4");
		return;
	}
	int slot;

	if (gWpnshow.Nowweapon == 3 )
	{
		
		if (gWpnshow.List_weapon[5].c_str()[0] != '\0')
		{
			slot = 5;
		}
		else if(gWpnshow.List_weapon[6].c_str()[0] != '\0')
		{
		
			slot = 6;

		}
		else return;
		char connd[64];
	
		sprintf(connd, "weapon_%s", gWpnshow.List_weapon[slot].c_str());
		gEngfuncs.pfnServerCmd(connd);
	}
	else if (gWpnshow.Nowweapon == 5)
	{
		if (gWpnshow.List_weapon[6].c_str()[0] != '\0')
		{
			slot = 6;
		}
		else if (gWpnshow.List_weapon[3].c_str()[0] != '\0')
		{
			slot = 3;
		}
		else return;
		char connd[64];
		sprintf(connd, "weapon_%s", gWpnshow.List_weapon[slot].c_str());
		gEngfuncs.pfnServerCmd(connd);
	}

	else if (gWpnshow.Nowweapon == 6)
	{
		if (gWpnshow.List_weapon[3].c_str()[0] != '\0')
		{
			slot = 3;
		}
		else if (gWpnshow.List_weapon[5].c_str()[0] != '\0')
		{

			slot = 5;

		}
		else return;
		char connd[64];
		sprintf(connd, "weapon_%s", gWpnshow.List_weapon[slot].c_str());
		gEngfuncs.pfnServerCmd(connd);
	}
	
	else {

		int slot;
		if (gWpnshow.List_weapon[3].c_str()[0] != '\0')
			slot = 3;
		else if (gWpnshow.List_weapon[5].c_str()[0] != '\0')
			slot = 5;
		else if (gWpnshow.List_weapon[6].c_str()[0] != '\0')
			slot = 6;
		else 
			return;
		char connd[64];
		sprintf(connd, "weapon_%s", gWpnshow.List_weapon[slot].c_str());
		gEngfuncs.pfnServerCmd(connd);
	
	}
	return;
}


void DrawMessage(clock_t *nowtime) {

	static int Message[MAXMSG];
	int tall,wide;

	if (Message[0] == 0)
		Message[0] = gpFontFuncs->CreateFont();

	gpFontFuncs->AddGlyphSetToFont(Message[0], "黑体", 14, 600, 0, 0, 0x080, 0x0, 0xFFFF);
	gpFontFuncs->GetTextSize(Message[0], L"aaaa", &wide, &tall);


	int xpoint = 10;
	int ypoint =( Msgxpoint == 0 ? gScreen.iHeight - 20 : Msgxpoint - 20 ) - tall *MAXMSG - 1 *( MAXMSG-1);

	for (int i = 0; i < MAXMSG; i++)
	{

		if (*nowtime - gMeaasge[i].time > Msgtime * 1000)
			break;
		if (gMeaasge[i].time < 0) continue;
		
		
		if (gMeaasge[i].team != g_iPlayers[gEngfuncs.GetLocalPlayer()->index].team && gMeaasge[i].team!= 0 )
			continue;	

		if (Message[i] == 0)
			Message[i] = gpFontFuncs->CreateFont();


		gpFontFuncs->AddGlyphSetToFont(Message[i], "黑体", 14, 600, 0, 0, 0x080 , 0x0, 0xFFFF);
		gpFontFuncs->DrawSetTextFont(Message[i]);
		if(gMeaasge[i].team == 0 && g_iPlayers[gMeaasge[i].id].team != g_iPlayers[gEngfuncs.GetLocalPlayer()->index].team)
			gpFontFuncs->DrawSetTextColor(255, 50, 50, 255);
		else 
			gpFontFuncs->DrawSetTextColor(180, 180, 180, 255);

		gpFontFuncs->DrawSetTextPos(xpoint, ypoint);

		gpFontFuncs->DrawPrintText(gMeaasge[i].message.c_str());
		ypoint = ypoint + 1 + tall;



	}




}


int MsgFunc_BarTime(const char *pszName, int iSize, void *pbuf) {


	BEGIN_READ(pbuf, iSize);

	int time = READ_SHORT();


	gloadpic.alltime = time;
	gloadpic.time = clock();
	gloadpic.text = L"";

	//gEngfuncs.Con_Printf("//////////////////////////\n=====%d=====\n", time);
	return pmBarTime(pszName, iSize, pbuf);
}
int MsgFunc_BarTime2(const char *pszName, int iSize, void *pbuf){

	BEGIN_READ(pbuf, iSize);

	int time = READ_SHORT();
	int load = READ_SHORT();
	gloadpic.time = load;
	gloadpic.alltime = time;

	gloadpic.text = L"";
	return pmBarTime2(pszName, iSize, pbuf);
}



void DrawLoadingBar(clock_t *nowtime) {
	if (gloadpic.alltime == 0)
		return;
	if (*nowtime - gloadpic.time > (gloadpic.alltime *1000))
		return;
	static int tipfont = 0;
	int wide, tall;

	if (gloadpic.text != L"" && tipfont ==0)
	{
		tipfont = gpFontFuncs->CreateFont();
	}
	gloadpic.loadd = ((*nowtime - gloadpic.time) / (gloadpic.alltime * 1000.0)) * 100;

	gpTexFuncs->RenderMode(RenderNormal);
	gpTexFuncs->DrawSetTexture(gloadpic.loadbg);
	gpTexFuncs->DrawSetColor(255, 255, 255, 255);
	gpTexFuncs->DisableScissor();
	gpTexFuncs->DrawTexturedRect(gScreen.iWidth / 2 - gloadpic.loadbg->wide / 2 , (gScreen.iHeight / 4)*3 - gloadpic.loadbg->tall, gloadpic.loadbg->wide, gloadpic.loadbg->tall);

	gpTexFuncs->RenderMode(RenderNormal);
	gpTexFuncs->DrawSetTexture(gloadpic.load);
	gpTexFuncs->DrawSetColor(255, 255, 255, 255);
	gpTexFuncs->DisableScissor();
	gpTexFuncs->DrawTexturedRect(gScreen.iWidth / 2 - gloadpic.loadbg->wide / 2 + 7 , (gScreen.iHeight / 4) * 3 - gloadpic.loadbg->tall + 5 , 306 *(gloadpic.loadd/100.0), gloadpic.load->tall);

	gpFontFuncs->AddGlyphSetToFont(tipfont, "黑体", 14, 600, 0, 0, 0, 0x0, 0xFFFF);
	gpFontFuncs->GetTextSize(tipfont, gloadpic.text.c_str(), &wide, &tall);
	gpFontFuncs->DrawSetTextFont(tipfont);
	gpFontFuncs->DrawSetTextColor(255, 255, 255, 255);
	gpFontFuncs->DrawSetTextPos(gScreen.iWidth / 2 - (wide / 2), (gScreen.iHeight / 4) * 3 - gloadpic.loadbg->tall/2 - tall/2);
	gpFontFuncs->DrawPrintText(gloadpic.text.c_str());

}


int MsgFunc_SetFOV(const char *pszName, int iSize, void *pbuf) {
	BEGIN_READ(pbuf, iSize);

	int fov = READ_BYTE();
	// gEngfuncs.Con_Printf("OV:%d\n", fov);
	if (fov < 55)
	{
		Crosshairboolean = false;
		mzj = true;



	}
	else {
		mzj = false;
		Crosshairboolean = true;
		//fun_DrawTGA("WW_TGA/front_sight.tga", 0, 255, 0, 255, 0.5, 0.5, -1, -1, true, 0, -3.0, 3);
	}
	return pmSetFOV(pszName, iSize, pbuf);
}

int MsgFunc_RoundTime(const char *pszName, int iSize, void *pbuf) {
	BEGIN_READ(pbuf, iSize);

	int Time = READ_SHORT();
	Roundtime = Time;
	clockRtime = clock();
	gEngfuncs.Con_Printf("TIME：%d\n", Time);
	return pmRoundTime(pszName, iSize, pbuf);
}


int MsgFunc_StatusIcon(const char *pszName, int iSize, void *pbuf) {

	BEGIN_READ(pbuf, iSize);
	char szZONE[64];
	int zoneid;
	int color[3];
	zoneid = READ_BYTE();
	strcpy(szZONE, READ_STRING());

	color[0] = READ_BYTE();
	color[1] = READ_BYTE();
	color[2] = READ_BYTE();
	if (!strcmp(szZONE, "buyzone"))
	{
		if(zoneid == 1)
			buystate = 1;
		else 
			buystate = 0;
	}
	if (!strcmp(szZONE, "c4"))
	{
		if (zoneid == 1)
		{
			c4state = 1;
		}
		else if (zoneid == 2)
		{
			c4state = 2;
		}
		else 
			c4state = 0;
	}

	
	//gEngfuncs.Con_Printf("state:%d || %s\n",zoneid,szZONE);

	return pmStatusIcon(pszName, iSize, pbuf);
}
void DrawAWP() {
	if (!(is_playershow == true))
	
	if (mzj == true)
	{

		gpTexFuncs->RenderMode(RenderNormal);
		gpTexFuncs->DrawSetTexture(awp);
		gpTexFuncs->DrawSetColor(255, 255, 255, 255);
		gpTexFuncs->DisableScissor();
		gpTexFuncs->DrawTexturedRect(gScreen.iWidth / 2 - (awp->wide*(gScreen.iHeight / (awp->tall*1.0))) / 2,0, awp->wide*(gScreen.iHeight / (awp->tall*1.0)), gScreen.iHeight);

		
	}


}

void DrawC4() {

	if (is_playershow == true)
	{
		
		if(c4state == 1)
		{
			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(C4show);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(10, gScreen.iHeight / 2 - C4show->tall*0.7 - BAG->tall / 2 - 10, C4show->wide *0.7, C4show->tall *0.7);
		}
		if (c4state == 2)
		{
			static int state = 0;
			static clock_t time = clock();

			if (clock() - time > 300)
			{
				time = clock();
				state = !state;
			}

			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(C4show);
			if(state == 0)
				gpTexFuncs->DrawSetColor(255, 100, 100, 255);
			else
				gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(10, gScreen.iHeight / 2 - C4show->tall*0.7 - BAG->tall / 2 - 10 , C4show->wide *0.7, C4show->tall *0.7);
		}
		if (buystate == 1)
		{

			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(BAG);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(10, gScreen.iHeight / 2 - BAG->tall/2, BAG->wide, BAG->tall);
		}



	}



	
}


int Msg_Massist(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	int id = READ_BYTE();
	int attss = READ_SHORT();

	g_iPlayers[id].assist = attss;

	return 1;

}



void DrawKillicon(clock_t *nowtime) {
	if (*nowtime - KILLICON.nowtime <= KILLICONHOLETIME * 1000)
	{
		if (KILLICON.assista == true)
		{
			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(assiste);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(gScreen.iWidth / 2 - assiste->wide / 2, (gScreen.iHeight / 4) * 3 - assiste->tall / 2, assiste->wide, assiste->tall);


			return;

		}
		if (KILLICON.headshoot == 1)
		{
			
			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(headshoot);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(gScreen.iWidth / 2 - headshoot->wide / 2, (gScreen.iHeight / 4) * 3 - headshoot->tall / 2, headshoot->wide, headshoot->tall);

		}
		else if (KILLICON.specialkill > 0)
		{
			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(specialtex[KILLICON.specialkill-1 ]);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(gScreen.iWidth / 2 - specialtex[KILLICON.specialkill - 1]->wide / 2, (gScreen.iHeight / 4) * 3 - specialtex[KILLICON.specialkill - 1]->tall / 2, specialtex[KILLICON.specialkill - 1]->wide, specialtex[KILLICON.specialkill - 1]->tall);

		}
		else
		{
				gpTexFuncs->RenderMode(RenderNormal);
				gpTexFuncs->DrawSetTexture(Miklltex[KILLICON.Mkill - 1]);
				gpTexFuncs->DrawSetColor(255, 255, 255, 255);
				gpTexFuncs->DisableScissor();
				gpTexFuncs->DrawTexturedRect(gScreen.iWidth / 2 - Miklltex[KILLICON.Mkill]->wide / 2, (gScreen.iHeight / 4) * 3 - Miklltex[KILLICON.Mkill]->tall / 2, Miklltex[KILLICON.Mkill]->wide, Miklltex[KILLICON.Mkill]->tall);

		}
		if (KILLICON.fuchou == true)
		{
			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(fuchou);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(gScreen.iWidth / 2 - fuchou->wide / 2, (gScreen.iHeight / 4) - fuchou->tall / 2, fuchou->wide, fuchou->tall);
		}
		else if (KILLICON.firstkill == 1)
		{
			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(firstkill[0]);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(gScreen.iWidth / 2 - firstkill[0]->wide / 2, (gScreen.iHeight / 4) - firstkill[0]->tall / 2, firstkill[0]->wide, firstkill[0]->tall);
		}
		else if (KILLICON.firstkill == 2)
		{
			gpTexFuncs->RenderMode(RenderNormal);
			gpTexFuncs->DrawSetTexture(firstkill[1]);
			gpTexFuncs->DrawSetColor(255, 255, 255, 255);
			gpTexFuncs->DisableScissor();
			gpTexFuncs->DrawTexturedRect(gScreen.iWidth / 2 - firstkill[1]->wide / 2, (gScreen.iHeight / 4) - firstkill[1]->tall / 2, firstkill[1]->wide, firstkill[1]->tall);
		}
		
	}





}
int Msg_CHstyle(const char *pszName, int iSize, void *pbuf) {
	BEGIN_READ(pbuf, iSize);
	int R = READ_BYTE();
	int G = READ_BYTE();
	int B = READ_BYTE();
	int A = READ_BYTE();
	if (R >= 0 && R <= 255 && G >= 0 && G <= 255 && B >= 0 && B <= 255 && A >= 0 && A <= 255)
	{
		CHRGB[0] = R;
		CHRGB[1] = G;
		CHRGB[2] = B;
		CHRGB[3] = A;

		//gEngfuncs.Con_Printf("Crosshair is changed color (%d,%d,%d,%d)", R, G, B, A);
	}
	return 1;
}
void DrawCrosshair() 
{
	if (Crosshairboolean == false) 
		return;

	gpTexFuncs->RenderMode(RenderNormal);
	gpTexFuncs->DrawSetTexture(Crosshair);
	gpTexFuncs->DrawSetColor(CHRGB[0], CHRGB[1], CHRGB[2], CHRGB[3]);
	gpTexFuncs->DisableScissor();
	gpTexFuncs->DrawTexturedRect(gScreen.iWidth / 2 - Crosshair->wide / 2, gScreen.iHeight / 2 - Crosshair->tall / 2, Crosshair->wide, Crosshair->tall);

}

int Msg_DRTGA(const char *pszName, int iSize, void *pbuf) {
	BEGIN_READ(pbuf, iSize);
	char szTGA[64];
	strcpy(szTGA, READ_STRING());
	int Red = READ_BYTE();
	int Green = READ_BYTE();
	int Blue = READ_BYTE();
	int Alpha = READ_BYTE();
	float X = READ_LONG()/10000.0;
	float Y = READ_LONG() / 10000.0;
	int center = READ_BYTE();
	float Holdtime = READ_LONG()/10000.0;
	int channel = READ_SHORT();


	int xpoint = (int)(X*gScreen.iWidth);
	int ypoint = (int)(X*gScreen.iHeight);

	fun_DrawTGA(szTGA, Red, Green, Blue, Alpha, xpoint, ypoint, center, -1, -1, 0, Holdtime, channel);

	return 1;
}


int Msg_DTTGA(const char *pszName, int iSize, void *pbuf) {
	BEGIN_READ(pbuf, iSize);
	int channel = READ_SHORT();
	fun_DelTGA(channel);

	return 1;
}

int Msg_DRTGAX(const char *pszName, int iSize, void *pbuf) {


	BEGIN_READ(pbuf, iSize);
	char szTGA[64];
	strcpy(szTGA, READ_STRING());
	int Red = READ_BYTE();
	int Green = READ_BYTE();
	int Blue = READ_BYTE();
	int Alpha = READ_BYTE();
	float X = READ_COORD();
	float Y = READ_COORD();
	float Z = READ_COORD();
	int Holdtime = READ_LONG();
	int channel = READ_SHORT();

	gDrawTGA[channel].XYZ = true;
	strcpy(gDrawTGA[channel].filename, szTGA);
	gDrawTGA[channel].tex = gpTexFuncs->LoadTGA(szTGA);
	gDrawTGA[channel].red = Red;
	gDrawTGA[channel].green = Green;
	gDrawTGA[channel].blue = Blue;
	gDrawTGA[channel].alpha = Alpha;
	gDrawTGA[channel].holdtime = Holdtime / 10000.0;
	gDrawTGA[channel].starttime = clock();
	gDrawTGA[channel].id = 0;
	gDrawTGA[channel].POINT[0] = X;
	gDrawTGA[channel].POINT[1] = Y;
	gDrawTGA[channel].POINT[2] = Z;
	gDrawTGA[channel].wide = -1;
	gDrawTGA[channel].tall = -1;
	return 1;
}

int Msg_DRTGAP(const char *pszName, int iSize, void *pbuf) {

	BEGIN_READ(pbuf, iSize);
	char szTGA[64];
	strcpy(szTGA, READ_STRING());
	int Red = READ_BYTE();
	int Green = READ_BYTE();
	int Blue = READ_BYTE();
	int Alpha = READ_BYTE();
	int id = READ_BYTE();
	int position = READ_BYTE();
	int Holdtime = READ_LONG();
	int channel = READ_SHORT();


	strcpy(gDrawTGA[channel].filename, szTGA);
	gDrawTGA[channel].tex = gpTexFuncs->LoadTGA(szTGA);
	gDrawTGA[channel].red = Red;
	gDrawTGA[channel].green = Green;
	gDrawTGA[channel].blue = Blue;
	gDrawTGA[channel].alpha = Alpha;
	gDrawTGA[channel].holdtime = Holdtime/10000.0;
	gDrawTGA[channel].mode = RenderNormal;
	gDrawTGA[channel].wide = -1;
	gDrawTGA[channel].tall = -1;
	gDrawTGA[channel].starttime = clock();
	gDrawTGA[channel].id = id;
	gDrawTGA[channel].body = position;
	gDrawTGA[channel].XYZ = false;
	//gEngfuncs.Con_Printf("%d;%d,%d ,%f,%d,,,tex:%d--%d\n", Red, id, position, gDrawTGA[channel].holdtime, channel, gDrawTGA[channel].tex->wide, gDrawTGA[channel].tex->tall);
	return 1;
}



int Msg_SETSB(const char *pszName, int iSize, void *pbuf) {


	BEGIN_READ(pbuf, iSize);
	
	int show = READ_BYTE();
	if (show == 0)
	{
		gScoreBroad.isshow = false;
		return 1;
	}
	
	gScoreBroad.isshow = true;
	int type = READ_BYTE();
	int score = READ_SHORT();
	gScoreBroad.type = (type == 0 ? 0 : 1);


	swprintf(gScoreBroad.ModoName, L"%s", UTF8ToUnicode(READ_STRING()));
	swprintf(gScoreBroad.Tname, L"%s", UTF8ToUnicode(READ_STRING()));
	swprintf(gScoreBroad.CTname, L"%s", UTF8ToUnicode(READ_STRING()));
	gScoreBroad.score = score;

}

int MsgFunc_SendAudio(const char *pszName, int iSize, void *pbuf) {

	BEGIN_READ(pbuf, iSize);

	string Message;
	int ID = READ_BYTE();
	Message = READ_STRING();
	//gEngfuncs.Con_Printf("AUDIO:%d:%s\n", ID, UTF8ToUnicode(Message.c_str()));

	for (int i = MAXMSG - 2; i >= 0; i--)
	{

		gMeaasge[i + 1] = gMeaasge[i];
	}

	gMeaasge[0].id = ID;
	gMeaasge[0].time = clock();

	gMeaasge[0].team = g_iPlayers[ID].team;
	wchar_t name[128];
	wchar_t message[128];
	wchar_t Msg[128];
	hud_player_info_t hPlayer;
	//gEngfuncs.Con_Printf("=====%s====\n", Message.c_str());
	gEngfuncs.pfnGetPlayerInfo(gMeaasge[0].id, &hPlayer);
	swprintf(name, UTF8ToUnicode(hPlayer.name));


	swprintf(Msg, L"[无线电消息]%ls：%ls", name, GETradio(Message).c_str());
	gMeaasge[0].message = Msg;


	clock_t time = clock();
	return pmSendAudio(pszName, iSize, pbuf);
}


int Msg_WWPrit(const char *pszName, int iSize, void *pbuf) {

	BEGIN_READ(pbuf, iSize);

	string Message;
	Message = READ_STRING();
	//gEngfuncs.Con_Printf("AUDIO:%d:%s\n", 0, Message.c_str());
	int ID = 1;
	for (int i = MAXMSG - 2; i >= 0; i--)
	{

		gMeaasge[i + 1] = gMeaasge[i];
	}

	gMeaasge[0].id = ID;
	gMeaasge[0].time = clock();
	gMeaasge[0].team = 0;
	gMeaasge[0].message = UTF8ToUnicode(Message.c_str());
	return  0;

}
wstring GETradio(const string a)
{
	wstring rString = StringToWstring(a);
	if (a == "%!MRAD_FIREINHOLE")
		rString = L"小心手雷！";
	else if(a == "%!MRAD_COVERME")
		rString = L"掩护我！";
	else if (a == "%!MRAD_TAKEPOINT")
		rString = L"你守住这个位置！";
	else if (a == "%!MRAD_POSITION")
		rString = L"各单位保持现在的位置！";
	else if (a == "%!MRAD_FOLLOWME")
		rString = L"跟我来！";
	else if (a == "%!MRAD_HITASSIST") 
		rString = L"需要火力支援";



	
	else if (a == "%!MRAD_GO")
		rString = L"Go,Go,Go！";
	else if (a == "%!MRAD_FALLBACK")
		rString = L"撤退！";
	else if (a == "%!MRAD_STICKTOG")
		rString = L"保持队型！";
	else if (a == "%!MRAD_STORMFRONT")
		rString = L"全队正面快速突进！";
	else if (a == "%!MRAD_REPORTIN")
		rString = L"准备好了吗？";


	else if (a == "%!MRAD_AFFIRM")
		rString = L"收到！";
	else if (a == "%!MRAD_ENEMYSPOT")
		rString = L"发现敌人";
	else if (a == "%!MRAD_BACKUP")
		rString = L"我需要支援！";
	else if (a == "%!MRAD_INPOS")
		rString = L"我已就位！";
	else if (a == "%!MRAD_REPRTINGIN")
		rString = L"汇报周围情况！";

	else if (a == "%!MRAD_BLOW")
		rString = L"C4即将爆炸！";
	else if (a == "%!MRAD_NEGATIVE")
		rString = L"做不到！";
	else if (a == "%!MRAD_ENEMYDOWN")
		rString = L"敌人已被击毙！";
	else if (a == "%!MRAD_CLEAR")
		rString = L"周围安全！";
	else if (a == "%!MRAD_GETINPOS")
		rString = L"全队就位，等我冲出去时掩护我！";
	else if (a == "%!MRAD_REGROUP")
		rString = L"重新组队进攻！";
	else if (a == "%!MRAD_ROGER")
		rString = L"收到！";
	return rString;



}






int Msg_WRadio(const char *pszName, int iSize, void *pbuf) {
	BEGIN_READ(pbuf, iSize);
	gRadio = READ_BYTE();


	return 1;
}



void Draw_Radio() {
	if (gRadio == 0)
		return;
	static int Radiowz[11];
	int static tall= 0, wide;
	int static titletall = 0;

	for (int i = 0; i < 11; i++)
	{
		if (Radiowz[i] == 0)
			Radiowz[i] = gpFontFuncs->CreateFont();
		if (i != 0)
		{

			gpFontFuncs->AddGlyphSetToFont(Radiowz[i], "宋体", 12, 600, 0, 0, 0x080, 0x0, 0xFFFF);
			gpFontFuncs->GetTextSize(Radiowz[i],L"你好", &tall, &wide);
		}
		else
		{
			
			gpFontFuncs->AddGlyphSetToFont(Radiowz[0], "宋体", 14, 600, 0, 0, 0x080,0x0, 0xFFFF);
			gpFontFuncs->GetTextSize(Radiowz[0],L"你好", &titletall, &wide);
		}
	}


	if (gRadio == 1)
	{
		int Y = gScreen.iHeight/2-(titletall + 10 + 7 * tall + 5 * 1 + 5)/2;

		for (int i = 0; i < 8; i++)
		{
			if (Radiowz[i] == 0)
				Radiowz[i] = gpFontFuncs->CreateFont();
			gpFontFuncs->DrawSetTextFont(Radiowz[i]);
			gpFontFuncs->DrawSetTextColor(176, 143, 87, 255);
			gpFontFuncs->DrawSetTextPos(gScreen.iWidth - 200, Y);
			gpFontFuncs->DrawPrintText(Radio1text[i]);
			if (i == 0)
			{
				Y =Y+ titletall + 10;
			}
			else if (i == 6)
				Y = Y + tall + 5;
			else
				Y = Y + tall;
		}
	}

	if (gRadio == 2)
	{
		int Y = gScreen.iHeight / 2 - (titletall + 10 + 7 * tall + 5 * 1 + 5) / 2;

		for (int i = 0; i < 8; i++)
		{
			if (Radiowz[i] == 0)
				Radiowz[i] = gpFontFuncs->CreateFont();
			gpFontFuncs->DrawSetTextFont(Radiowz[i]);
			gpFontFuncs->DrawSetTextColor(176, 143, 87, 255);
			gpFontFuncs->DrawSetTextPos(gScreen.iWidth - 200, Y);
			gpFontFuncs->DrawPrintText(Radio2text[i]);
			if (i == 0)
			{
				Y = Y + titletall + 10;
			}
			else if (i == 6)
				Y = Y + tall + 5;
			else
				Y = Y + tall;
		}
	}
	if (gRadio == 3)
	{
		int Y = gScreen.iHeight / 2 - (titletall + 10 + 10 * tall + 8 * 1 + 5) / 2;

		for (int i = 0; i < 11; i++)
		{
			if (Radiowz[i] == 0)
				Radiowz[i] = gpFontFuncs->CreateFont();
			gpFontFuncs->DrawSetTextFont(Radiowz[i]);
			gpFontFuncs->DrawSetTextColor(176, 143, 87, 255);
			gpFontFuncs->DrawSetTextPos(gScreen.iWidth - 200, Y);
			gpFontFuncs->DrawPrintText(Radio3text[i]);
			if (i == 0)
			{
				Y = Y + titletall + 10;
			}
			else if (i == 9)
				Y = Y + tall + 5;
			else
				Y = Y + tall ;
		}
	}
}


NoticeShow::NoticeShow() {
	NoticeBG = gpTexFuncs->LoadTGA("WW_TGA/suv_notice.tga");
	Tip = gpFontFuncs->CreateFont();
	gpFontFuncs->AddGlyphSetToFont(Tip, "黑体", 20, 600, 0, 0, 0, 0x0, 0xFFFF);
}

void NoticeShow::DrawNotice() {
	if (this->CreatTime <= 0)
		return;


	int tall, wide;
	gpTexFuncs->RenderMode(RenderNormal);
	gpTexFuncs->DrawSetTexture(this->NoticeBG);
	gpTexFuncs->DrawSetColor(255, 255, 255, 255);
	gpTexFuncs->DisableScissor();
	gpTexFuncs->DrawTexturedRect(gScreen.iWidth / 2 - this->NoticeBG->wide / 2, gScreen.iHeight / 2 - this->NoticeBG->tall / 2, this->NoticeBG->wide, this->NoticeBG->tall);

	gpFontFuncs->GetTextSize(Tip, Tiptext.c_str(), &wide, &tall);
	gpFontFuncs->DrawSetTextFont(Tip);
	gpFontFuncs->DrawSetTextColor(255, 255, 255, 255);
	gpFontFuncs->DrawSetTextPos(gScreen.iWidth / 2 - (wide / 2), gScreen.iHeight / 2 - tall /2);
	gpFontFuncs->DrawPrintText(Tiptext.c_str());
	if (clock() - CreatTime > showtime * 1000)
		CreatTime = -1;
}



void NoticeShow:: IntTip(const wchar_t text[], int showTime) {
	this->CreatTime = clock();
	this->Tiptext = text;
	this->showtime = showTime;

}

std::wstring StringToWstring(const std::string str)
{// string转wstring
	unsigned len = str.size() * 2;// 预留字节数
	setlocale(LC_CTYPE, "");     //必须调用此函数
	wchar_t *p = new wchar_t[len];// 申请一段内存存放转换后的字符串
	mbstowcs(p, str.c_str(), len);// 转换
	std::wstring str1(p);
	delete[] p;// 释放申请的内存
	return str1;
}

std::string WstringToString(const std::wstring str)
{// wstring转string
	unsigned len = str.size() * 4;
	setlocale(LC_CTYPE, "");
	char *p = new char[len];
	wcstombs(p, str.c_str(), len);
	std::string str1(p);
	delete[] p;
	return str1;
}


int Msg_Cnotice(const char *pszName, int iSize, void *pbuf) {


	BEGIN_READ(pbuf, iSize);

	string Message;
	int time;
	Message = READ_STRING();
	time = READ_SHORT();
	gNoticeShow->IntTip(UTF8ToUnicode(Message.c_str()), time);
	//gEngfuncs.Con_Printf("NOTICE===================\n");
	return 1;
}

int Msg_Bartime(const char *pszName, int iSize, void *pbuf) {

	BEGIN_READ(pbuf, iSize); 
	string tiptext;
	int time;
	tiptext = READ_STRING();

	//gEngfuncs.Con_Printf("KHD:%s", tiptext.c_str());
	time = READ_SHORT();
	gloadpic.time = clock();
	gloadpic.alltime = time;
	gloadpic.text = UTF8ToUnicode(tiptext.c_str());
	return 1;
}