int Initialize(struct cl_enginefuncs_s *pEnginefuncs, int iVersion);
void HUD_Init(void);
int HUD_Redraw(float time, int intermission);
int HUD_VidInit(void);
int HUD_UpdateClientData(client_data_t *pcldata, float flTime);
void V_CalcRefdef(struct ref_params_s *pParams);