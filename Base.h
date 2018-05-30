#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include <interface.h>

#include <metahook.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <VGUI\ISurface.h>

#include <math.h>
#include <vector.h>
#include <gl/gl.h>
#include <const.h>
#include <triangleapi.h>
#include <cl_entity.h>
#include <ref_params.h>
#include <cvardef.h>
#include <com_model.h>

#include "exportfuncs.h"
#include "engfuncs.h"
#include "r_efx.h"
#include"ITextureRender.h"
#include "event_api.h"
#include"parsemsg.h"

#include "Game_UI.h"
#include <io.h>
#include<api.h>

#include <r_studioint.h>
#include <pm_movevars.h>