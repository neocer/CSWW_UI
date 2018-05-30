
#include "Base.h"
#include <imm.h>  
#pragma comment (lib ,"imm32.lib")  

WeaponShow gWpnshow;
qboolean g_iHudVidInitalized;
DrawTEXT gDrawTEXT[CHANNEL];
DrawTGA gDrawTGA[CHANNEL];
cl_enginefunc_t gEngfuncs;
texture_api_t *gpTexFuncs;
fonts_api_t *gpFontFuncs;
SCREENINFO gScreen;
texcache_t *backgroudpic;
texcache_t *ct_bar;
texcache_t *t_bar;
texcache_t *me_bar;
texcache_t *killer_bar;
texcache_t *Deathicon[32];
texcache_t *killertga;
PlayerInfo g_iPlayers[33];
texcache_t *Now_wpn;
texcache_t *Wpn_bg;
texcache_t *awp;
texcache_t *WpnFX_bg[7];
Loadpic gloadpic;
texcache_t *WpnFX[7];
texcache_t *C4show;
texcache_t *BAG;

texcache_t *Miklltex[MAXKILL];
texcache_t *specialtex[4];

texcache_t *firstkill[2];
texcache_t *assiste;
texcache_t *fuchou;
texcache_t *headshoot;
texcache_t *Crosshair;
NoticeShow *gNoticeShow;
Playershow * gPlayershow;

int CHRGB[4] = { 0,255,0,255 };

HIMC g_hIMC = NULL;

struct Killicon KILLICON;

bool showscore = false;

extern int tscore;
extern int ctscore;

int Initialize(struct cl_enginefuncs_s *pEnginefuncs, int iVersion)
{

	memcpy(&gEngfuncs, pEnginefuncs, sizeof(gEngfuncs));
	pEnginefuncs->pfnHookUserMsg = &EngFunc_HookUserMsg;
	
	g_hIMC = ImmAssociateContext(NULL, NULL);
	return gExportfuncs.Initialize(pEnginefuncs, iVersion);;
}

void HUD_Init(void)
{
	CreateInterfaceFn textureRenderCreateInterface = Sys_GetFactory((HINTERFACEMODULE)GetModuleHandleA("TextureRender.dll"));
	ITextureRender *pStudio = (ITextureRender *)textureRenderCreateInterface(METAHOOK_TEXTURERENDER_VERSION, NULL);
	gpTexFuncs = pStudio->GetTextureAPI();
	gpFontFuncs = pStudio->GetFontsAPI();
	gEngfuncs.pfnAddCommand("+showscores", Fun_showSB);
	gEngfuncs.pfnAddCommand("slot4", Fun_slot4);
	gEngfuncs.pfnAddCommand("-showscores", Fun_hideSB);
	gScreen.iSize = sizeof(gScreen);
	gEngfuncs.pfnGetScreenInfo(&gScreen);
	for (int i = 0; i < 32; i++)
	{
		g_iPlayers[i + 1].dead = true;

	}
	gExportfuncs.HUD_Init();
	return ;

}

void V_CalcRefdef(struct ref_params_s *pParams)
{
	gExportfuncs.V_CalcRefdef(pParams);
}


int HUD_VidInit(void)
{
	gEngfuncs.pfnHookUserMsg("RoundState", Msg_RoundState);
	gEngfuncs.pfnHookUserMsg("LSpawn", Msg_Playerspawn);
	gEngfuncs.pfnHookUserMsg("UDammo", Msg_UpdataAmmo);
	gEngfuncs.pfnHookUserMsg("WDTmsg", Msg_DTmsg);
	gEngfuncs.pfnHookUserMsg("Massist", Msg_Massist);
	gEngfuncs.pfnHookUserMsg("CHstyle", Msg_CHstyle);
	gEngfuncs.pfnHookUserMsg("WWPrit", Msg_WWPrit);
	gEngfuncs.pfnHookUserMsg("SETSB", Msg_SETSB);
	gEngfuncs.pfnHookUserMsg("WRadio", Msg_WRadio);
	gEngfuncs.pfnHookUserMsg("LBar", Msg_Bartime);

	gEngfuncs.pfnHookUserMsg("DRTGA", Msg_DRTGA);
	gEngfuncs.pfnHookUserMsg("DLTTGA", Msg_DTTGA);
	gEngfuncs.pfnHookUserMsg("DRTGAX", Msg_DRTGAX);
	gEngfuncs.pfnHookUserMsg("DRTGAP", Msg_DRTGAP);



	gEngfuncs.pfnHookUserMsg("Cnotice", Msg_Cnotice);
	gNoticeShow = new NoticeShow();
	gPlayershow = new Playershow();
	tscore = 0;
	ctscore = 0;
	backgroudpic =  gpTexFuncs->LoadTGA("WW_TGA/SB_background.tga");
	ct_bar = gpTexFuncs->LoadTGA("WW_TGA/ct_bar.tga");
	t_bar = gpTexFuncs->LoadTGA("WW_TGA/t_bar.tga");
	me_bar = gpTexFuncs->LoadTGA("WW_TGA/me_sb.tga");
	killertga = gpTexFuncs->LoadTGA("WW_TGA/killer.tga");

	killer_bar = gpTexFuncs->LoadTGA("WW_TGA/REVENGE_SCORE.tga");
	awp = gpTexFuncs->LoadTGA("WW_TGA/MZJ.tga");
	gloadpic.loadbg = gpTexFuncs->LoadTGA("WW_TGA/loadbg.tga");
	gloadpic.load = gpTexFuncs->LoadTGA("WW_TGA/load.tga");
	C4show = gpTexFuncs->LoadTGA("WW_TGA/C4.tga");
	Wpn_bg = gpTexFuncs->LoadTGA("WW_TGA/Wpn_BackBoard.tga"); 
	BAG = gpTexFuncs->LoadTGA("WW_TGA/bag.tga");
	char szKILL[64];

	for (int i = 0; i < MAXKILL; i++)
	{
		sprintf(szKILL, "WW_TGA/KILLICON/%dkill.tga", i + 1);

		Miklltex[i] = gpTexFuncs->LoadTGA(szKILL);
	}
	specialtex[0]=gpTexFuncs->LoadTGA("WW_TGA/KILLICON/knifekill.tga");
	specialtex[1] = gpTexFuncs->LoadTGA("WW_TGA/KILLICON/hekill.tga");
	specialtex[2] = gpTexFuncs->LoadTGA("WW_TGA/KILLICON/wallkill.tga");
	specialtex[3] = gpTexFuncs->LoadTGA("WW_TGA/KILLICON/wall_headkill.tga");
	firstkill[0] = gpTexFuncs->LoadTGA("WW_TGA/KILLICON/firstblood.tga");
	firstkill[1] = gpTexFuncs->LoadTGA("WW_TGA/KILLICON/final.tga");
	assiste = gpTexFuncs->LoadTGA("WW_TGA/KILLICON/assist.tga");
	fuchou = gpTexFuncs->LoadTGA("WW_TGA/KILLICON/fuchou.tga");
	Crosshair = gpTexFuncs->LoadTGA("WW_TGA/front_sight.tga");
	headshoot = gpTexFuncs->LoadTGA("WW_TGA/KILLICON/headshoot.tga");
	for (int i = 0; i < 7; i++)
		WpnFX_bg[i] = gpTexFuncs->LoadTGA("WW_TGA/Wpn_BackBoard.tga");
	for (int i = 0; i < 32; i++)
	{

		Deathicon[i] = gpTexFuncs->LoadTGA("WW_TGA/Deathicon.tga");
		g_iPlayers[i + 1].dead = true;
	}

	int result = gExportfuncs.HUD_VidInit();

	g_iHudVidInitalized = true;



	return result;
}
int HUD_Redraw(float time, int intermission)
{

	clock_t nowtime = clock();
	Draw∞Ê»®();
	for (int i=0; i < CHANNEL; i++)
	{
		if (strlen(gDrawTGA[i].filename)>0)
		{
			if (gDrawTGA[i].id == 0 && gDrawTGA[i].XYZ ==false)
			{
				gpTexFuncs->RenderMode(gDrawTGA[i].mode);
				gpTexFuncs->DrawSetTexture(gDrawTGA[i].tex);
				gpTexFuncs->DrawSetColor(gDrawTGA[i].red, gDrawTGA[i].green, gDrawTGA[i].blue, gDrawTGA[i].alpha);
				gpTexFuncs->DisableScissor();
				gpTexFuncs->DrawTexturedRect(gDrawTGA[i].x, gDrawTGA[i].y, gDrawTGA[i].wide > 0 ? gDrawTGA[i].wide : gDrawTGA[i].tex->wide, gDrawTGA[i].tall > 0 ? gDrawTGA[i].tall : gDrawTGA[i].tex->tall);
			}
			else 
				if (gDrawTGA[i].id > 0 && gDrawTGA[i].id < 33)
				{

					cl_entity_t *pEntity = NULL;
					pEntity = gEngfuncs.GetEntityByIndex(gDrawTGA[i].id);
					if (!pEntity)
						continue;
					vec3_t fOrigin;
					VectorCopy(pEntity->curstate.origin, fOrigin);

					if (gDrawTGA[i].body == 1)
					{
						fOrigin[2] += 50;
					}

					float vecScreen[2];
					if (!gEngfuncs.pTriAPI->WorldToScreen(fOrigin, vecScreen))
					{

						vecScreen[0] = (1.0f + (vecScreen[0])) * gScreen.iWidth * 0.5f;
						vecScreen[1] = (1.0f - (vecScreen[1])) * gScreen.iHeight * 0.5f;
						//gEngfuncs.Con_Printf("%d -- - %d \n", vecScreen[0], vecScreen[1]);
						gpTexFuncs->RenderMode(RenderNormal);
						gpTexFuncs->DrawSetTexture(gDrawTGA[i].tex);
						gpTexFuncs->DrawSetColor(gDrawTGA[i].red, gDrawTGA[i].green, gDrawTGA[i].blue, gDrawTGA[i].alpha);
						gpTexFuncs->DisableScissor();
						gpTexFuncs->DrawTexturedRect(vecScreen[0] - gDrawTGA[i].tex->wide / 2, vecScreen[1], gDrawTGA[i].tex->wide, gDrawTGA[i].tex->tall);
					}
					
				}
				else if (gDrawTGA[i].XYZ == true)
				{
					float vecScreen[2];
					if (!gEngfuncs.pTriAPI->WorldToScreen(gDrawTGA[i].POINT, vecScreen))
					{

						vecScreen[0] = (1.0f + (vecScreen[0])) * gScreen.iWidth * 0.5f;
						vecScreen[1] = (1.0f - (vecScreen[1])) * gScreen.iHeight * 0.5f;
						gpTexFuncs->RenderMode(RenderNormal);
						gpTexFuncs->DrawSetTexture(gDrawTGA[i].tex);
						gpTexFuncs->DrawSetColor(gDrawTGA[i].red, gDrawTGA[i].green, gDrawTGA[i].blue, gDrawTGA[i].alpha);
						gpTexFuncs->DisableScissor();
						gpTexFuncs->DrawTexturedRect(vecScreen[0] - gDrawTGA[i].tex->wide / 2, vecScreen[1], gDrawTGA[i].tex->wide, gDrawTGA[i].tex->tall);
					}
				}
			if ((double)(nowtime  - gDrawTGA[i].starttime ) >= gDrawTGA[i].holdtime *1000 && gDrawTGA[i].holdtime > 0)
			{
				gDrawTGA[i].filename[0] = '\0';
			}
		}
	}
	
	
	DrawDeathMsg(&nowtime);
	DrawScoredBroad();
	

	DrawKiller(&nowtime);
	DrawWpnfx(&nowtime);
	DrawMessage(&nowtime);
	DrawAWP();
	DrawC4();
	DrawKillicon(&nowtime);

	DrawWpnshow(&nowtime);
	gPlayershow->DrawPlayerShow();
	DrawCrosshair();
	DrawLoadingBar(&nowtime);
	Draw_Tabscorebroad();
	Draw_Radio();
	gNoticeShow->DrawNotice();


	return 1;
}
int HUD_UpdateClientData(client_data_t *pcldata, float flTime)
{
	bool wpn[7] = {false,false ,false,false ,false ,false ,false };
	if (pcldata->iWeaponBits & (1 << 31))
	{
			for (int i = 1; i < 31; i++)
			{

				if (i == 2) continue;
				if (pcldata->iWeaponBits & (1 << i))
				{
					if (GetWpnSlot(i) == -1) continue;
					wpn[GetWpnSlot(i)] = true;
				}
		
			}
	}
	 
	for (int i = 0; i < 7; i++)
	{
		if (wpn[i] == false)
				gWpnshow.GetweaponList("", i);
		

	}

	if (wpn[4] == true && gWpnshow.List_weapon[4].c_str()[0] =='\0')
		gWpnshow.GetweaponList("weapon_c4", 4);

	return gExportfuncs.HUD_UpdateClientData(pcldata, flTime);
}






/*
int Msg_DRTEXT(const char *pszName, int iSize, void *pbuf) {






return 1;
}
*/
