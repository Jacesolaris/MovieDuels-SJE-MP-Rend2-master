/*
===========================================================================
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2015,MovieDuels contributors

This file is part of the MovieDuels source code.

MovieDuels is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/

/*
=======================================================================

USER INTERFACE SABER LOADING & DISPLAY CODE

=======================================================================
*/

#include "ui_local.h"
#include "ui_shared.h"

void WP_SaberLoadParms();
qboolean WP_SaberParseParm(const char* saber_name, const char* parmname, char* saberData);
saber_colors_t TranslateSaberColor(const char* name);
const char* SaberColorToString(saber_colors_t color);
saber_styles_t TranslateSaberStyle(const char* name);
saberType_t TranslateSaberType(const char* name);

qboolean	ui_saber_parms_parsed = qfalse;

static qhandle_t redSaberGlowShader;
static qhandle_t redSaberCoreShader;
static qhandle_t orangeSaberGlowShader;
static qhandle_t orangeSaberCoreShader;
static qhandle_t yellowSaberGlowShader;
static qhandle_t yellowSaberCoreShader;
static qhandle_t greenSaberGlowShader;
static qhandle_t greenSaberCoreShader;
static qhandle_t blueSaberGlowShader;
static qhandle_t blueSaberCoreShader;
static qhandle_t purpleSaberGlowShader;
static qhandle_t purpleSaberCoreShader;
static qhandle_t unstableRedSaberGlowCoreShader;

static qhandle_t unstableRedSaberCoreShader; //rend2 version
static qhandle_t unstableRedSaberCoreShader2; //vanilla version

static qhandle_t blackSaberGlowShader;
static qhandle_t blackSaberCoreShader;
static qhandle_t rgbSaberGlowShader;
static qhandle_t rgbSaberCoreShader;
static qhandle_t limeSaberGlowShader;
static qhandle_t limeSaberCoreShader;

//Episode I Sabers
static qhandle_t ep1SaberCoreShader;
static qhandle_t ep1BlackSaberCoreShader;
static qhandle_t redEp1GlowShader;
static qhandle_t orangeEp1GlowShader;
static qhandle_t yellowEp1GlowShader;
static qhandle_t greenEp1GlowShader;
static qhandle_t blueEp1GlowShader;
static qhandle_t purpleEp1GlowShader;

//Episode II Sabers
static qhandle_t ep2SaberCoreShader;
static qhandle_t ep2blackSaberCoreShader;
static qhandle_t redEp2GlowShader;
static qhandle_t orangeEp2GlowShader;
static qhandle_t yellowEp2GlowShader;
static qhandle_t greenEp2GlowShader;
static qhandle_t blueEp2GlowShader;
static qhandle_t purpleEp2GlowShader;

//Episode III Sabers
static qhandle_t ep3SaberCoreShader;
static qhandle_t ep3redSaberCoreShader;
static qhandle_t ep3orangeSaberCoreShader;
static qhandle_t ep3yellowSaberCoreShader;
static qhandle_t ep3greenSaberCoreShader;
static qhandle_t ep3blueSaberCoreShader;
static qhandle_t ep3purpleSaberCoreShader;
static qhandle_t ep3blackSaberCoreShader;
static qhandle_t redEp3GlowShader;
static qhandle_t orangeEp3GlowShader;
static qhandle_t yellowEp3GlowShader;
static qhandle_t greenEp3GlowShader;
static qhandle_t blueEp3GlowShader;
static qhandle_t purpleEp3GlowShader;

//SFX Sabers
static qhandle_t sfxSaberBladeShader;
static qhandle_t sfxblackSaberBladeShader;

//Original Trilogy Sabers
static qhandle_t otSaberCoreShader;
static qhandle_t otBlackSaberCoreShader;
static qhandle_t redOTGlowShader;
static qhandle_t orangeOTGlowShader;
static qhandle_t yellowOTGlowShader;
static qhandle_t greenOTGlowShader;
static qhandle_t blueOTGlowShader;
static qhandle_t purpleOTGlowShader;
static qhandle_t rgbTFASaberCoreShader;

//Episode VI Sabers
static qhandle_t ep6SaberCoreShader;
static qhandle_t ep6BlackSaberCoreShader;
static qhandle_t redEp6GlowShader;
static qhandle_t orangeEp6GlowShader;
static qhandle_t yellowEp6GlowShader;
static qhandle_t greenEp6GlowShader;
static qhandle_t blueEp6GlowShader;
static qhandle_t purpleEp6GlowShader;

//Episode VII Sabers
static qhandle_t ep7SaberCoreShader;
static qhandle_t ep7redSaberCoreShader;
static qhandle_t ep7orangeSaberCoreShader;
static qhandle_t ep7yellowSaberCoreShader;
static qhandle_t ep7greenSaberCoreShader;
static qhandle_t ep7blueSaberCoreShader;
static qhandle_t ep7purpleSaberCoreShader;
static qhandle_t ep7blackSaberCoreShader;
static qhandle_t redEp7GlowShader;
static qhandle_t orangeEp7GlowShader;
static qhandle_t yellowEp7GlowShader;
static qhandle_t greenEp7GlowShader;
static qhandle_t blueEp7GlowShader;
static qhandle_t purpleEp7GlowShader;

//rebels

//Episode I Sabers
static qhandle_t rebelsSaberCoreShader;
static qhandle_t rebelsredSaberCoreShader;
static qhandle_t rebelsorangeSaberCoreShader;
static qhandle_t rebelsyellowSaberCoreShader;
static qhandle_t rebelsgreenSaberCoreShader;
static qhandle_t rebelsblueSaberCoreShader;
static qhandle_t rebelspurpleSaberCoreShader;
static qhandle_t rebelsblackSaberCoreShader;
static qhandle_t rgbRebelsGlowShader;
static qhandle_t redRebelsGlowShader;
static qhandle_t orangeRebelsGlowShader;
static qhandle_t yellowRebelsGlowShader;
static qhandle_t greenRebelsGlowShader;
static qhandle_t blueRebelsGlowShader;
static qhandle_t purpleRebelsGlowShader;

//Clone Wars Sabers
static qhandle_t cwSaberCoreShader;
static qhandle_t cwredSaberCoreShader;
static qhandle_t cworangeSaberCoreShader;
static qhandle_t cwyellowSaberCoreShader;
static qhandle_t cwgreenSaberCoreShader;
static qhandle_t cwblueSaberCoreShader;
static qhandle_t cwpurpleSaberCoreShader;
static qhandle_t cwblackSaberCoreShader;
static qhandle_t rgbCWGlowShader;
static qhandle_t redCWGlowShader;
static qhandle_t orangeCWGlowShader;
static qhandle_t yellowCWGlowShader;
static qhandle_t greenCWGlowShader;
static qhandle_t blueCWGlowShader;
static qhandle_t purpleCWGlowShader;

//Maul Sabers
static qhandle_t maulSaberCoreShader;
static qhandle_t maulredSaberCoreShader;
static qhandle_t maulorangeSaberCoreShader;
static qhandle_t maulyellowSaberCoreShader;
static qhandle_t maulgreenSaberCoreShader;
static qhandle_t maulblueSaberCoreShader;
static qhandle_t maulpurpleSaberCoreShader;
static qhandle_t maulblackSaberCoreShader;
static qhandle_t rgbMaulGlowShader;
static qhandle_t redMaulGlowShader;
static qhandle_t orangeMaulGlowShader;
static qhandle_t yellowMaulGlowShader;
static qhandle_t greenMaulGlowShader;
static qhandle_t blueMaulGlowShader;
static qhandle_t purpleMaulGlowShader;

void UI_CacheSaberGlowGraphics(void)
{
	redSaberGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/red_glow");
	redSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/red_line");
	orangeSaberGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/orange_glow");
	orangeSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/orange_line");
	yellowSaberGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/yellow_glow");
	yellowSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/yellow_line");
	greenSaberGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/green_glow");
	greenSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/green_line");
	blueSaberGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/blue_glow");
	blueSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/blue_line");
	purpleSaberGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/purple_glow");
	purpleSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/purple_line");

	unstableRedSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/unstable_line"); //rend2 version
	unstableRedSaberCoreShader2 = trap->R_RegisterShaderNoMip("gfx/effects/TFASabers/blade_TFA"); //vanilla version

	unstableRedSaberGlowCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/red_unstable_line");

	blackSaberGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/black_glow");
	blackSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/black_line");
	rgbSaberGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/rgb_glow");
	rgbSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/rgb_line");
	limeSaberGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/lime_glow");
	limeSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers/lime_line");

	//Episode I Sabers
	ep1SaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep1/rgb_line");
	ep1BlackSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep1/black_line");
	redEp1GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep1/red_glow");
	orangeEp1GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep1/orange_glow");
	yellowEp1GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep1/yellow_glow");
	greenEp1GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep1/green_glow");
	blueEp1GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep1/blue_glow");
	purpleEp1GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep1/purple_glow");
	//Episode II Sabers
	ep2SaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep2/rgb_line");
	ep2blackSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep2/black_line");
	redEp2GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep2/red_glow");
	orangeEp2GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep2/orange_glow");
	yellowEp2GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep2/yellow_glow");
	greenEp2GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep2/green_glow");
	blueEp2GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep2/blue_glow");
	purpleEp2GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep2/purple_glow");
	//Episode III Sabers
	ep3SaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep3/rgb_line");
	ep3redSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep3/red_line");
	ep3orangeSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep3/orange_line");
	ep3yellowSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep3/yellow_line");
	ep3greenSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep3/green_line");
	ep3blueSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep3/blue_line");
	ep3purpleSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep3/purple_line");
	ep3blackSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep3/black_line");
	redEp3GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep3/red_glow");
	orangeEp3GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep3/orange_glow");
	yellowEp3GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep3/yellow_glow");
	greenEp3GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep3/green_glow");
	blueEp3GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep3/blue_glow");
	purpleEp3GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ep3/purple_glow");
	//SFX Sabers
	sfxSaberBladeShader = trap->R_RegisterShaderNoMip("SFX_Sabers/saber_blade");
	sfxblackSaberBladeShader = trap->R_RegisterShaderNoMip("SFX_Sabers/saber_blade_black");
	//Original Trilogy Sabers
	otSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ot/ot_saberCore");
	otBlackSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ot/ot_saberBlackCore");
	redOTGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ot/ot_redGlow");
	orangeOTGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ot/ot_orangeGlow");
	yellowOTGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ot/ot_yellowGlow");
	greenOTGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ot/ot_greenGlow");
	blueOTGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ot/ot_blueGlow");
	purpleOTGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_ot/ot_purpleGlow");
	//Episode VI Sabers
	ep6SaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rotj/rgb_line");
	ep6BlackSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rotj/black_line");
	redEp6GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rotj/red_glow");
	orangeEp6GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rotj/orange_glow");
	yellowEp6GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rotj/yellow_glow");
	greenEp6GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rotj/green_glow");
	blueEp6GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rotj/blue_glow");
	purpleEp6GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rotj/purple_glow");
	//Episode VII Sabers
	ep7SaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/rgb_line");
	ep7redSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/red_line");
	ep7orangeSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/orange_line");
	ep7yellowSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/yellow_line");
	ep7greenSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/green_line");
	ep7blueSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/blue_line");
	ep7purpleSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/purple_line");
	ep7blackSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/black_line");

	//rgbTFASaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/TFASabers/blade_TFA");
	rgbTFASaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/unstable_line");

	redEp7GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/red_glow");
	orangeEp7GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/orange_glow");
	yellowEp7GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/yellow_glow");
	greenEp7GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/green_glow");
	blueEp7GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/blue_glow");
	purpleEp7GlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tfa/purple_glow");

	//rebels I Sabers
	rebelsSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rebels/rgb_line");
	rebelsredSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rebels/red_line");
	rebelsorangeSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rebels/orange_line");
	rebelsyellowSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rebels/yellow_line");
	rebelsgreenSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rebels/green_line");
	rebelsblueSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rebels/blue_line");
	rebelspurpleSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rebels/purple_line");
	rebelsblackSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rebels/black_line");
	rgbRebelsGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rebels/rgb_glow");
	redRebelsGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rebels/red_glow");
	orangeRebelsGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rebels/orange_glow");
	yellowRebelsGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rebels/yellow_glow");
	greenRebelsGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rebels/green_glow");
	blueRebelsGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rebels/blue_glow");
	purpleRebelsGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_rebels/purple_glow");

	//Clone Wars Sabers
	cwSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tcw/rgb_line");
	cwredSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tcw/red_line");
	cworangeSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tcw/orange_line");
	cwyellowSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tcw/yellow_line");
	cwgreenSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tcw/green_line");
	cwblueSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tcw/blue_line");
	cwpurpleSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tcw/purple_line");
	cwblackSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tcw/black_line");
	rgbCWGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tcw/rgb_glow");
	redCWGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tcw/red_glow");
	orangeCWGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tcw/orange_glow");
	yellowCWGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tcw/yellow_glow");
	greenCWGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tcw/green_glow");
	blueCWGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tcw/blue_glow");
	purpleCWGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_tcw/purple_glow");

	//Maul Sabers
	maulSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_maul/rgb_line");
	maulredSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_maul/red_line");
	maulorangeSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_maul/orange_line");
	maulyellowSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_maul/yellow_line");
	maulgreenSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_maul/green_line");
	maulblueSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_maul/blue_line");
	maulpurpleSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_maul/purple_line");
	maulblackSaberCoreShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_maul/black_line");
	rgbMaulGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_maul/rgb_glow");
	redMaulGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_maul/red_glow");
	orangeMaulGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_maul/orange_glow");
	yellowMaulGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_maul/yellow_glow");
	greenMaulGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_maul/green_glow");
	blueMaulGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_maul/blue_glow");
	purpleMaulGlowShader = trap->R_RegisterShaderNoMip("gfx/effects/sabers_maul/purple_glow");
}

qboolean UI_SaberModelForSaber(const char* saber_name, char* saberModel)
{
	return WP_SaberParseParm(saber_name, "saberModel", saberModel);
}

qboolean UI_SaberSkinForSaber(const char* saber_name, char* saberSkin)
{
	return WP_SaberParseParm(saber_name, "customSkin", saberSkin);
}

qboolean UI_SaberTypeForSaber(const char* saber_name, char* saberType)
{
	return WP_SaberParseParm(saber_name, "saberType", saberType);
}

static int UI_saber_numBladesForSaber(const char* saber_name)
{
	char	numBladesString[8] = { 0 };
	WP_SaberParseParm(saber_name, "numBlades", numBladesString);
	int numBlades = atoi(numBladesString);
	if (numBlades < 1)
	{
		numBlades = 1;
	}
	else if (numBlades > 8)
	{
		numBlades = 8;
	}
	return numBlades;
}

static qboolean UI_SaberShouldDrawBlade(const char* saber_name, int blade_num)
{
	int bladeStyle2Start = 0, noBlade = 0;
	char	bladeStyle2StartString[8] = { 0 };
	char	noBladeString[8] = { 0 };
	WP_SaberParseParm(saber_name, "bladeStyle2Start", bladeStyle2StartString);
	if (bladeStyle2StartString[0])
	{
		bladeStyle2Start = atoi(bladeStyle2StartString);
	}
	if (bladeStyle2Start
		&& blade_num >= bladeStyle2Start)
	{//use second blade style
		WP_SaberParseParm(saber_name, "noBlade2", noBladeString);
		if (noBladeString[0])
		{
			noBlade = atoi(noBladeString);
		}
	}
	else
	{//use first blade style
		WP_SaberParseParm(saber_name, "noBlade", noBladeString);
		if (noBladeString[0])
		{
			noBlade = atoi(noBladeString);
		}
	}
	return noBlade == 0;
}

static qboolean UI_IsSaberTwoHanded(const char* saber_name)
{
	char	twoHandedString[8] = { 0 };
	WP_SaberParseParm(saber_name, "twoHanded", twoHandedString);
	if (!twoHandedString[0])
	{//not defined defaults to "no"
		return qfalse;
	}
	const int twoHanded = atoi(twoHandedString);
	return twoHanded != 0;
}

static float UI_SaberBladeLengthForSaber(const char* saber_name, int blade_num)
{
	char	lengthString[8] = { 0 };
	float	length = 40.0f;
	WP_SaberParseParm(saber_name, "saberLength", lengthString);
	if (lengthString[0])
	{
		length = atof(lengthString);
		if (length < 0.0f)
		{
			length = 0.0f;
		}
	}

	WP_SaberParseParm(saber_name, va("saberLength%d", blade_num + 1), lengthString);
	if (lengthString[0])
	{
		length = atof(lengthString);
		if (length < 0.0f)
		{
			length = 0.0f;
		}
	}

	return length;
}

static float UI_SaberBladeRadiusForSaber(const char* saber_name, int blade_num)
{
	char	radiusString[8] = { 0 };
	float	radius = 3.0f;
	WP_SaberParseParm(saber_name, "saberRadius", radiusString);
	if (radiusString[0])
	{
		radius = atof(radiusString);
		if (radius < 0.0f)
		{
			radius = 0.0f;
		}
	}

	WP_SaberParseParm(saber_name, va("saberRadius%d", blade_num + 1), radiusString);
	if (radiusString[0])
	{
		radius = atof(radiusString);
		if (radius < 0.0f)
		{
			radius = 0.0f;
		}
	}

	return radius;
}

qboolean UI_SaberProperNameForSaber(const char* saber_name, char* saberProperName)
{
	char	stringedSaberName[1024];
	const qboolean ret = WP_SaberParseParm(saber_name, "name", stringedSaberName);
	// if it's a stringed reference translate it
	if (ret && stringedSaberName[0] == '@')
	{
		trap->SE_GetStringTextString(&stringedSaberName[1], saberProperName, 1024);
	}
	else
	{
		// no stringed so just use it as it
		strcpy(saberProperName, stringedSaberName);
	}

	return ret;
}

static qboolean UI_SaberValidForPlayerInMP(const char* saber_name)
{
	char allowed[8] = { 0 };

	// Read "notInMP" from saber file
	if (WP_SaberParseParm(saber_name, "notInMP", allowed) == qfalse)
	{
		// Flag missing → NOT allowed
		return qfalse;
	}

	if (allowed[0] == '\0')
	{
		// Empty value → NOT allowed
		return qfalse;
	}

	// Only "0" means allowed
	if (atoi(allowed) == 0)
	{
		return qtrue;
	}

	// Any other value (including "1") → NOT allowed
	return qfalse;
}

void UI_SaberLoadParms(void)
{
	ui_saber_parms_parsed = qtrue;
	UI_CacheSaberGlowGraphics();

	WP_SaberLoadParms();
}

static void RGB_LerpColor(vec3_t from, vec3_t to, float frac, vec3_t out)
{
	vec3_t diff;

	VectorSubtract(to, from, diff);

	VectorCopy(from, out);

	for (int i = 0; i < 3; i++)
	{
		out[i] += diff[i] * frac;
	}
}

static int getint(char** buf)
{
	const double temp = strtod(*buf, buf);
	return (int)temp;
}

void ParseRGBSaber(char* str, vec3_t c)
{
	char* p = str;

	for (int i = 0; i < 3; i++)
	{
		c[i] = getint(&p);
		p++;
	}
}

vec3_t  ScriptedColors[10][2] = { 0 };
int		ScriptedTimes[10][2] = { 0 };
int		ScriptedNum[2] = { 0 };
int		ScriptedActualNum[2] = { 0 };
int		ScriptedStartTime[2] = { 0 };
int		ScriptedEndTime[2] = { 0 };

void UI_ParseScriptedSaber(char* script, int snum)
{
	int n = 0;
	char* p = script;

	const int l = strlen(p);
	p++;

	while (p[0] && p - script < l && n < 10)
	{
		ParseRGBSaber(p, ScriptedColors[n][snum]);
		while (p[0] != ':')
			p++;
		p++;

		ScriptedTimes[n][snum] = getint(&p);

		p++;
		n++;
	}
	ScriptedNum[snum] = n;
}

static void rgb_adjust_scipted_saber_color(vec3_t color, const int n)
{
	int actual;
	const int time = uiInfo.uiDC.realTime;

	if (!ScriptedStartTime[n])
	{
		ScriptedActualNum[n] = 0;
		ScriptedStartTime[n] = time;
		ScriptedEndTime[n] = time + ScriptedTimes[0][n];
	}
	else if (ScriptedEndTime[n] < time)
	{
		ScriptedActualNum[n] = (ScriptedActualNum[n] + 1) % ScriptedNum[n];
		actual = ScriptedActualNum[n];
		ScriptedStartTime[n] = time;
		ScriptedEndTime[n] = time + ScriptedTimes[actual][n];
	}

	actual = ScriptedActualNum[n];

	const float frac = (float)(time - ScriptedStartTime[n]) / (float)(ScriptedEndTime[n] - ScriptedStartTime[n]);

	if (actual + 1 != ScriptedNum[n])
		RGB_LerpColor(ScriptedColors[actual][n], ScriptedColors[actual + 1][n], frac, color);
	else
		RGB_LerpColor(ScriptedColors[actual][n], ScriptedColors[0][n], frac, color);

	for (int i = 0; i < 3; i++)
		color[i] /= 255;
}

#define PIMP_MIN_INTESITY 120

static void RGB_RandomRGB(vec3_t c)
{
	int i;
	for (i = 0; i < 3; i++)
		c[i] = 0;

	while (c[0] + c[1] + c[2] < PIMP_MIN_INTESITY)
		for (i = 0; i < 3; i++)
			c[i] = rand() % 255;
}

int PimpStartTime[2];
int PimpEndTime[2];
vec3_t PimpColorFrom[2];
vec3_t PimpColorTo[2];

static void RGB_AdjustPimpSaberColor(vec3_t color, int n)
{
	int time;

	if (!PimpStartTime[n])
	{
		PimpStartTime[n] = uiInfo.uiDC.realTime;
		RGB_RandomRGB(PimpColorFrom[n]);
		RGB_RandomRGB(PimpColorTo[n]);
		time = 250 + rand() % 250;
		PimpEndTime[n] = uiInfo.uiDC.realTime + time;
	}
	else if (PimpEndTime[n] < uiInfo.uiDC.realTime)
	{
		VectorCopy(PimpColorTo[n], PimpColorFrom[n]);
		RGB_RandomRGB(PimpColorTo[n]);
		time = 250 + rand() % 250;
		PimpStartTime[n] = uiInfo.uiDC.realTime;
		PimpEndTime[n] = uiInfo.uiDC.realTime + time;
	}

	const float frac = (float)(uiInfo.uiDC.realTime - PimpStartTime[n]) / (float)(PimpEndTime[n] - PimpStartTime[n]);

	RGB_LerpColor(PimpColorFrom[n], PimpColorTo[n], frac, color);

	for (int i = 0; i < 3; i++)
		color[i] /= 255;
}

static void UI_DoSaber(vec3_t origin, vec3_t dir, float length, float lengthMax, float radius, saber_colors_t color, int snum)
{
	vec3_t		mid, rgb = { 1,1,1 };
	qhandle_t	blade = 0, glow = 0;
	refEntity_t saber;
	float radiusmult;

	if (length < 0.5f)
	{
		// if the thing is so short, just forget even adding me.
		return;
	}

	// Find the midpoint of the saber for lighting purposes
	VectorMA(origin, length * 0.5f, dir, mid);

	switch (color)
	{
	case SABER_RED:
		glow = redSaberGlowShader;
		blade = redSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeSaberGlowShader;
		blade = orangeSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowSaberGlowShader;
		blade = yellowSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenSaberGlowShader;
		blade = greenSaberCoreShader;
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueSaberGlowShader;
		blade = blueSaberCoreShader;
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleSaberGlowShader;
		blade = purpleSaberCoreShader;
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = rgbSaberCoreShader;
		break;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		blade = rgbSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 1.0f);
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = blackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = rgbSaberCoreShader;
		break;
	}

	memset(&saber, 0, sizeof(refEntity_t));

	saber.saberLength = length;

	if (length < lengthMax)
	{
		radiusmult = 1.0 + 2.0 / length;		// Note this creates a curve, and length cannot be < 0.5.
	}
	else
	{
		radiusmult = 1.0;
	}

	float effectradius = (radius * 1.6f + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * cg_SFXSabersGlowSize.value;

	float coreradius = (radius * 0.4f + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * cg_SFXSabersCoreSize.value;

	// Main glow
	{
		float effectalpha = 0.8f;

		if (length - effectradius / 2.0f > 0.0f)
		{
			saber.radius = effectradius;
			saber.saberLength = length - saber.radius / 2.0f;
			VectorCopy(origin, saber.origin);
			VectorCopy(dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (snum == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			trap->R_AddRefEntityToScene(&saber);
		}
	}

	// Hot core
	VectorMA(origin, length, dir, saber.origin);
	VectorMA(origin, -1.0f, dir, saber.oldorigin);

	saber.customShader = blade;
	saber.reType = RT_LINE;
	saber.radius = coreradius;
	saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
	saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

	trap->R_AddRefEntityToScene(&saber);

	if (color != SABER_RGB)
	{
		return;
	}

	saber.customShader = rgbSaberCoreShader;
	saber.reType = RT_LINE;
	saber.radius = coreradius;
	saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
	saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

	trap->R_AddRefEntityToScene(&saber);
}

static void UI_DoRebelsSaber(vec3_t origin, vec3_t dir, float length, float lengthMax, float radius, saber_colors_t color, int snum)
{
	vec3_t	    mid, rgb = { 1,1,1 };
	qhandle_t	glow = 0, blade = 0;
	refEntity_t saber;
	float	    radiusmult;

	if (length < 0.5f)
	{
		return;
	}

	// Find the midpoint of the saber for lighting purposes
	VectorMA(origin, length * 0.5f, dir, mid);

	switch (color)
	{
	case SABER_RED:
		glow = redRebelsGlowShader;
		blade = rebelsredSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeRebelsGlowShader;
		blade = rebelsorangeSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowRebelsGlowShader;
		blade = rebelsyellowSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenRebelsGlowShader;
		blade = rebelsgreenSaberCoreShader;
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueRebelsGlowShader;
		blade = rebelsblueSaberCoreShader;
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleRebelsGlowShader;
		blade = rebelspurpleSaberCoreShader;
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_RGB:
		glow = rgbRebelsGlowShader;
		blade = rebelsSaberCoreShader;
		break;
	case SABER_WHITE:
		glow = rgbRebelsGlowShader;
		blade = rebelsSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 1.0f);
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = rebelsblackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = rgbSaberCoreShader;
		break;
	}

	memset(&saber, 0, sizeof(refEntity_t));

	// Saber glow is it's own ref type because it uses a ton of sprites, otherwise it would eat up too many
	//	refEnts to do each glow blob individually
	saber.saberLength = length;

	if (length < lengthMax)
	{
		radiusmult = 0.5 + length / lengthMax / 2;
	}
	else
	{
		radiusmult = 1.0;
	}

	float effectradius = (radius * 1.6f + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * cg_SFXSabersGlowSizeRebels.value;

	float coreradius = (radius * 0.4f + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * cg_SFXSabersCoreSizeRebels.value;

	effectradius *= 0.50f;
	coreradius *= 0.85f;

	// Main glow
	{
		float effectalpha = 0.8f;

		const float glowLength = length - effectradius * 0.5f;

		if (glowLength > 0.0f)
		{
			saber.radius = effectradius;
			saber.saberLength = glowLength;

			// Move glow slightly so its closer to the core
			VectorMA(origin, effectradius * 0.25f, dir, saber.origin);
			VectorCopy(dir, saber.axis[0]);

			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;

			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (snum == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			trap->R_AddRefEntityToScene(&saber);
		}
	}

	// Hot core
	VectorMA(origin, length, dir, saber.origin);
	VectorMA(origin, -1.0f, dir, saber.oldorigin);

	saber.customShader = blade;
	saber.reType = RT_LINE;
	saber.radius = coreradius;
	saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
	saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

	trap->R_AddRefEntityToScene(&saber);

	if (color != SABER_RGB)
	{
		return;
	}

	saber.customShader = rebelsSaberCoreShader;
	saber.reType = RT_LINE;
	saber.radius = coreradius;
	saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
	saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

	trap->R_AddRefEntityToScene(&saber);
}

static void UI_DoSFXSaber(vec3_t origin, vec3_t dir, float length, float lengthMax, float radius, saber_colors_t color, int snum)
{
	vec3_t	    mid, rgb = { 1,1,1 };
	qhandle_t	glow = 0, blade = 0;
	refEntity_t saber;
	float	    radiusmult;

	if (length < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_ORANGE:
		glow = orangeSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_YELLOW:
		glow = yellowSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_GREEN:
		glow = greenSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_BLUE:
		glow = blueSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_PURPLE:
		glow = purpleSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = sfxblackSaberBladeShader;
		break;
	case SABER_UNSTABLE_RED:
		glow = redSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = sfxSaberBladeShader;
		break;
	}

	// Find the midpoint of the saber for lighting purposes
	VectorMA(origin, length * 0.5f, dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (length < lengthMax)
	{
		radiusmult = 0.5 + length / lengthMax / 2;		// Note this creates a curve, and length cannot be < 0.5.
	}
	else
	{
		radiusmult = 1.0;
	}

	float effectradius = (radius * 1.6 + crandoms() * 0.1f) * radiusmult * cg_SFXSabersGlowSizeSFX.value;
	float coreradius = (radius * 0.4 + crandoms() * 0.1f) * radiusmult * cg_SFXSabersCoreSizeSFX.value;

	effectradius *= 0.75;
	coreradius *= 1.25;

	{
		float angle_scale = 1.0f;
		if (length - effectradius * angle_scale / 2 > 0)
		{
			float effectalpha = 0.8f;
			saber.radius = effectradius * angle_scale;
			saber.saberLength = length - saber.radius / 2;
			VectorCopy(origin, saber.origin);
			VectorCopy(dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (snum == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			trap->R_AddRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(origin, length, dir, saber.origin);
		VectorMA(origin, -1, dir, saber.oldorigin);
		saber.customShader = blade;
		saber.reType = RT_LINE;
		saber.radius = coreradius;
		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		trap->R_AddRefEntityToScene(&saber);
		trap->R_AddRefEntityToScene(&saber);
	}
}

static void UI_DoEp1Saber(vec3_t origin, vec3_t dir, float length, float lengthMax, float radius, saber_colors_t color, int snum)
{
	vec3_t mid, rgb = { 1, 1, 1 };
	float radiusmult;

	qhandle_t glow;
	qhandle_t blade;
	refEntity_t saber;

	if (length < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redEp1GlowShader;
		blade = ep1SaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeEp1GlowShader;
		blade = ep1SaberCoreShader;
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowEp1GlowShader;
		blade = ep1SaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenEp1GlowShader;
		blade = ep1SaberCoreShader;
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueEp1GlowShader;
		blade = ep1SaberCoreShader;
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleEp1GlowShader;
		blade = ep1SaberCoreShader;
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = ep1SaberCoreShader;
		break;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		blade = ep1SaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 1.0f);
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = ep1BlackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redEp1GlowShader;
		blade = unstableRedSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = ep1SaberCoreShader;
		break;
	}

	VectorMA(origin, length * 0.5f, dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (length < lengthMax)
	{
		radiusmult = 0.5 + length / lengthMax / 2;
	}
	else
	{
		radiusmult = 1.0;
	}

	float effectradius = (radius * 1.6 + crandoms() * 0.1f) * radiusmult * cg_SFXSabersGlowSizeEP1.value;
	float coreradius = (radius * 0.4 + crandoms() * 0.1f) * radiusmult * cg_SFXSabersCoreSizeEP1.value;

	effectradius *= 0.75f;
	coreradius *= 1.4f;

	{
		float angle_scale = 1.0f;
		if (length - effectradius * angle_scale / 2 > 0)
		{
			float effectalpha = 0.8f;
			saber.radius = effectradius * angle_scale;
			saber.saberLength = length - saber.radius / 2;
			VectorCopy(origin, saber.origin);
			VectorCopy(dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (snum == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			trap->R_AddRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(origin, length, dir, saber.origin);
		VectorMA(origin, -1, dir, saber.oldorigin);

		saber.customShader = blade;

		saber.reType = RT_LINE;

		saber.radius = coreradius;

		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		trap->R_AddRefEntityToScene(&saber);
		trap->R_AddRefEntityToScene(&saber);
	}
}

static void UI_DoEp2Saber(vec3_t origin, vec3_t dir, float length, float lengthMax, float radius, saber_colors_t color, int snum)
{
	vec3_t	    mid, rgb = { 1,1,1 };
	qhandle_t	glow = 0, blade = 0;
	refEntity_t saber;
	float	    radiusmult;

	if (length < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redEp2GlowShader;
		blade = ep2SaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeEp2GlowShader;
		blade = ep2SaberCoreShader;
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowEp2GlowShader;
		blade = ep2SaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenEp2GlowShader;
		blade = ep2SaberCoreShader;
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueEp2GlowShader;
		blade = ep2SaberCoreShader;
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleEp2GlowShader;
		blade = ep2SaberCoreShader;
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = ep2SaberCoreShader;
		break;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		blade = ep2SaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 1.0f);
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = ep2blackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redEp2GlowShader;
		blade = unstableRedSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = ep2SaberCoreShader;
		break;
	}

	// Find the midpoint of the saber for lighting purposes
	VectorMA(origin, length * 0.5f, dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (length < lengthMax)
	{
		radiusmult = 0.5 + length / lengthMax / 2;		// Note this creates a curve, and length cannot be < 0.5.
	}
	else
	{
		radiusmult = 1.0;
	}

	float effectradius = (radius * 1.6 + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * cg_SFXSabersGlowSizeEP2.value;
	float coreradius = (radius * 0.4 + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * cg_SFXSabersCoreSizeEP2.value;

	effectradius *= 0.75f;
	coreradius *= 1.3f;

	{
		float angle_scale = 1.0f;
		if (length - effectradius * angle_scale / 2 > 0)
		{
			float effectalpha = 0.8f;
			saber.radius = effectradius * angle_scale;
			saber.saberLength = length - saber.radius / 2;
			VectorCopy(origin, saber.origin);
			VectorCopy(dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (snum == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			trap->R_AddRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(origin, length, dir, saber.origin);
		VectorMA(origin, -1, dir, saber.oldorigin);

		saber.customShader = blade;

		saber.reType = RT_LINE;

		saber.radius = coreradius;

		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		trap->R_AddRefEntityToScene(&saber);
		trap->R_AddRefEntityToScene(&saber);
	}
}

static void UI_DoEp3Saber(vec3_t origin, vec3_t dir, float length, float lengthMax, float radius, saber_colors_t color, int snum)
{
	vec3_t	    mid, rgb = { 1,1,1 };
	qhandle_t	glow = 0, blade = 0;
	refEntity_t saber;
	float	    radiusmult;

	if (length < 0.5f)
	{
		return;
	}

	// Find the midpoint of the saber for lighting purposes
	VectorMA(origin, length * 0.5f, dir, mid);

	switch (color)
	{
	case SABER_RED:
		glow = redEp3GlowShader;
		blade = ep3redSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeEp3GlowShader;
		blade = ep3orangeSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowEp3GlowShader;
		blade = ep3yellowSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenEp3GlowShader;
		blade = ep3greenSaberCoreShader;
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueEp3GlowShader;
		blade = ep3blueSaberCoreShader;
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleEp3GlowShader;
		blade = ep3purpleSaberCoreShader;
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = ep3SaberCoreShader;
		break;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		blade = ep3SaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 1.0f);
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = ep3blackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redEp3GlowShader;
		blade = unstableRedSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = ep3SaberCoreShader;
		break;
	}

	memset(&saber, 0, sizeof(refEntity_t));

	if (length < lengthMax)
	{
		radiusmult = 0.5 + length / lengthMax / 2;		// Note this creates a curve, and length cannot be < 0.5.
	}
	else
	{
		radiusmult = 1.0;
	}

	float effectradius = (radius * 1.6 + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * cg_SFXSabersGlowSizeEP3.value;
	float coreradius = (radius * 0.4 + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * cg_SFXSabersCoreSizeEP3.value;

	effectradius *= 0.75f;
	coreradius *= 0.85f;

	{
		float angle_scale = 1.0f;
		if (length - effectradius * angle_scale / 2 > 0)
		{
			float effectalpha = 0.8f;
			saber.radius = effectradius * angle_scale;
			saber.saberLength = length - saber.radius / 2;
			VectorCopy(origin, saber.origin);
			VectorCopy(dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (snum == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			trap->R_AddRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(origin, length, dir, saber.origin);
		VectorMA(origin, -1, dir, saber.oldorigin);

		saber.customShader = blade;

		saber.reType = RT_LINE;

		saber.radius = coreradius;

		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		trap->R_AddRefEntityToScene(&saber);
		trap->R_AddRefEntityToScene(&saber);
	}
}

static void UI_DoOTSaber(vec3_t origin, vec3_t dir, float length, float lengthMax, float radius, saber_colors_t color, int snum)
{
	vec3_t	    mid, rgb = { 1,1,1 };
	qhandle_t	glow = 0, blade = 0;
	refEntity_t saber;
	float	    radiusmult;

	if (length < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redOTGlowShader;
		blade = otSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeOTGlowShader;
		blade = otSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowOTGlowShader;
		blade = otSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenOTGlowShader;
		blade = otSaberCoreShader;
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueOTGlowShader;
		blade = otSaberCoreShader;
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleOTGlowShader;
		blade = otSaberCoreShader;
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = otSaberCoreShader;
		break;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		blade = otSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 1.0f);
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = otBlackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redOTGlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = otSaberCoreShader;
		break;
	}

	VectorMA(origin, length * 0.5f, dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (length < lengthMax)
	{
		radiusmult = 0.5 + length / lengthMax / 2;
	}
	else
	{
		radiusmult = 1.0;
	}

	const float effectradius = (radius * 1.6 + flrand(0.3f, 1.8f)) * radiusmult * cg_SFXSabersGlowSizeOT.value;
	float coreradius = (radius * 0.4 + flrand(0.1f, 1.0f)) * radiusmult * cg_SFXSabersCoreSizeOT.value;

	coreradius *= 0.5f;

	{
		float angle_scale = 1.0f;
		if (length - effectradius * angle_scale / 2 > 0)
		{
			float effectalpha = 0.8f;
			saber.radius = effectradius * angle_scale;
			saber.saberLength = length - saber.radius / 2;
			VectorCopy(origin, saber.origin);
			VectorCopy(dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (snum == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			trap->R_AddRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(origin, length, dir, saber.origin);
		VectorMA(origin, -1, dir, saber.oldorigin);
		saber.customShader = blade;
		saber.reType = RT_LINE;
		saber.radius = coreradius;
		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		trap->R_AddRefEntityToScene(&saber);
		trap->R_AddRefEntityToScene(&saber);
	}
}

static void UI_DoRotJSaber(vec3_t origin, vec3_t dir, float length, float lengthMax, float radius, saber_colors_t color, int snum)
{
	vec3_t	    mid, rgb = { 1,1,1 };
	qhandle_t	glow = 0, blade = 0;
	refEntity_t saber;
	float	    radiusmult;

	if (length < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redEp6GlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_ORANGE:
		glow = orangeEp6GlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_YELLOW:
		glow = yellowEp6GlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_GREEN:
		glow = greenEp6GlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_BLUE:
		glow = blueEp6GlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_PURPLE:
		glow = purpleEp6GlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		blade = ep6SaberCoreShader;
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = ep6BlackSaberCoreShader;
		break;
	case SABER_UNSTABLE_RED:
		glow = redEp6GlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = ep6SaberCoreShader;
		break;
	}

	// Find the midpoint of the saber for lighting purposes
	VectorMA(origin, length * 0.5f, dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (length < lengthMax)
	{
		radiusmult = 0.5 + length / lengthMax / 2;
	}
	else
	{
		radiusmult = 1.0;
	}

	float effectradius = (radius * 1.6 + crandoms() * 0.1f) * radiusmult * cg_SFXSabersGlowSizeROTJ.value;
	float coreradius = (radius * 0.4 + crandoms() * 0.1f) * radiusmult * cg_SFXSabersCoreSizeROTJ.value;

	effectradius *= 0.75f;
	coreradius *= 0.7f;

	{
		float angle_scale = 1.0f;
		if (length - effectradius * angle_scale / 2 > 0)
		{
			float effectalpha = 0.8f;
			saber.radius = effectradius * angle_scale;
			saber.saberLength = length - saber.radius / 2;
			VectorCopy(origin, saber.origin);
			VectorCopy(dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (snum == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			trap->R_AddRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(origin, length, dir, saber.origin);
		VectorMA(origin, -1, dir, saber.oldorigin);

		saber.customShader = blade;

		saber.reType = RT_LINE;

		saber.radius = coreradius;

		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		trap->R_AddRefEntityToScene(&saber);
		trap->R_AddRefEntityToScene(&saber);
	}
}

static void UI_DoTFASaber(vec3_t origin, vec3_t dir, float length, float lengthMax, float radius, saber_colors_t color, int snum)
{
	vec3_t	    mid;
	qhandle_t	glow = 0, blade = 0;
	refEntity_t saber;
	float	    radiusmult;

	if (length < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redEp7GlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = ep7redSaberCoreShader;
		}
		else
		{
			blade = ep3redSaberCoreShader;
		}
		break;
	case SABER_ORANGE:
		glow = orangeEp7GlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = ep7orangeSaberCoreShader;
		}
		else
		{
			blade = ep3orangeSaberCoreShader;
		}
		break;
	case SABER_YELLOW:
		glow = yellowEp7GlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = ep7yellowSaberCoreShader;
		}
		else
		{
			blade = ep3yellowSaberCoreShader;
		}
		break;
	case SABER_GREEN:
		glow = greenEp7GlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = ep7greenSaberCoreShader;
		}
		else
		{
			blade = ep3greenSaberCoreShader;
		}
		break;
	case SABER_BLUE:
		glow = blueEp7GlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = ep7blueSaberCoreShader;
		}
		else
		{
			blade = ep3blueSaberCoreShader;
		}
		break;
	case SABER_PURPLE:
		glow = purpleEp7GlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = ep7purpleSaberCoreShader;
		}
		else
		{
			blade = ep3purpleSaberCoreShader;
		}
		break;
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = ep7SaberCoreShader;
		}
		else
		{
			blade = ep3SaberCoreShader;
		}
		break;
	case SABER_WHITE:
		glow = rgbSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = ep7SaberCoreShader;
		}
		else
		{
			blade = ep3SaberCoreShader;
		}
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = ep7blackSaberCoreShader;
		}
		else
		{
			blade = ep3blackSaberCoreShader;
		}
		break;
	case SABER_UNSTABLE_RED:
		glow = redEp7GlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader;
		}
		else
		{
			blade = rgbTFASaberCoreShader;
		}
		break;
	default:
		glow = rgbSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = ep7SaberCoreShader;
		}
		else
		{
			blade = ep3SaberCoreShader;
		}
		break;
	}

	// Find the midpoint of the saber for lighting purposes
	VectorMA(origin, length * 0.5f, dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (length < lengthMax)
	{
		radiusmult = 0.5 + length / lengthMax / 2;
	}
	else
	{
		radiusmult = 1.0;
	}

	float effectradius = (radius * 1.6 + crandoms() * 0.1f) * radiusmult * cg_SFXSabersGlowSizeTFA.value;
	const float coreradius = (radius * 0.4 + crandoms() * 0.1f) * radiusmult * cg_SFXSabersCoreSizeTFA.value;

	effectradius *= 0.8f;

	{
		float angle_scale = 1.0f;
		if (length - effectradius * angle_scale / 2 > 0)
		{
			float effectalpha = 0.8f;
			saber.radius = effectradius * angle_scale;
			saber.saberLength = length - saber.radius / 2;
			VectorCopy(origin, saber.origin);
			VectorCopy(dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (snum == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			trap->R_AddRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(origin, length, dir, saber.origin);
		VectorMA(origin, -1, dir, saber.oldorigin);

		saber.customShader = blade;

		saber.reType = RT_LINE;

		saber.radius = coreradius;

		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		trap->R_AddRefEntityToScene(&saber);
		trap->R_AddRefEntityToScene(&saber);
	}
}

static void UI_DoUnstableSaber(vec3_t origin, vec3_t dir, float length, float lengthMax, float radius, saber_colors_t color, int snum)
{
	vec3_t	    mid, rgb = { 1,1,1 };
	qhandle_t	glow = 0, blade = 0;
	refEntity_t saber;
	float	    radiusmult;

	if (length < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_WHITE:
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = blackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	default:
		glow = rgbSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		break;
	}

	// Find the midpoint of the saber for lighting purposes
	VectorMA(origin, length * 0.5f, dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (length < lengthMax)
	{
		radiusmult = 0.5 + length / lengthMax / 2;
	}
	else
	{
		radiusmult = 1.0;
	}

	{
		float angle_scale = 1.0f;
		float effectradius = (radius * 1.6f + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * cg_SFXSabersGlowSizeUSB.value;
		float coreradius = (radius * 0.4f + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * cg_SFXSabersCoreSizeUSB.value;

		effectradius *= 0.75f;

		if (length - effectradius * angle_scale / 2 > 0)
		{
			float effectalpha = 0.8f;
			saber.radius = effectradius * angle_scale;
			saber.saberLength = length - saber.radius / 2;
			VectorCopy(origin, saber.origin);
			VectorCopy(dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (snum == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			trap->R_AddRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(origin, length, dir, saber.origin);
		VectorMA(origin, -1, dir, saber.oldorigin);

		saber.customShader = blade;

		saber.reType = RT_LINE;

		saber.radius = coreradius;

		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		trap->R_AddRefEntityToScene(&saber);
		trap->R_AddRefEntityToScene(&saber);
	}
}

static void UI_DoSaberUnstable(vec3_t origin, vec3_t dir, float length, float lengthMax, float radius, saber_colors_t color, int snum)
{
	vec3_t	    mid, rgb = { 1,1,1 };
	qhandle_t	glow = 0, blade = 0;
	refEntity_t saber;
	float	    radiusmult;

	if (length < 0.5f)
	{
		return;
	}

	switch (color)
	{
	case SABER_RED:
		glow = redSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_WHITE:
	case SABER_RGB:
		glow = rgbSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = blackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	default:
		glow = rgbSaberGlowShader;
		if (com_rend2.integer == 1) //rend2 is on
		{
			blade = unstableRedSaberCoreShader; //rend2 only
		}
		else
		{
			blade = unstableRedSaberCoreShader2; //vanilla or Rend2
		}
		break;
	}

	// Find the midpoint of the saber for lighting purposes
	VectorMA(origin, length * 0.5f, dir, mid);

	memset(&saber, 0, sizeof(refEntity_t));

	if (length < lengthMax)
	{
		radiusmult = 0.5 + length / lengthMax / 2;
	}
	else
	{
		radiusmult = 1.0;
	}
	const float effectradius = (radius * 1.6f + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult;
	const float coreradius = (radius * 0.4f + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult;

	{
		float angle_scale = 1.0f;
		if (length - effectradius * angle_scale / 2 > 0)
		{
			float effectalpha = 0.8f;
			saber.radius = effectradius * angle_scale;
			saber.saberLength = length - saber.radius / 2;
			VectorCopy(origin, saber.origin);
			VectorCopy(dir, saber.axis[0]);
			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;
			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (snum == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			trap->R_AddRefEntityToScene(&saber);
		}

		// Do the hot core
		VectorMA(origin, length, dir, saber.origin);
		VectorMA(origin, -1, dir, saber.oldorigin);
		saber.customShader = blade;
		saber.reType = RT_LINE;
		saber.radius = coreradius;
		saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
		saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

		trap->R_AddRefEntityToScene(&saber);
		trap->R_AddRefEntityToScene(&saber);
	}
}

static void UI_DoCWSaber(vec3_t origin, vec3_t dir, float length, float lengthMax, float radius, saber_colors_t color, int snum)
{
	vec3_t		mid, rgb = { 1,1,1 };
	qhandle_t	blade = 0, glow = 0;
	refEntity_t saber;
	float radiusmult;

	if (length < 0.5f)
	{
		// if the thing is so short, just forget even adding me.
		return;
	}

	// Find the midpoint of the saber for lighting purposes
	VectorMA(origin, length * 0.5f, dir, mid);

	switch (color)
	{
	case SABER_RED:
		glow = redCWGlowShader;
		blade = cwredSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeCWGlowShader;
		blade = cworangeSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowCWGlowShader;
		blade = cwyellowSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenCWGlowShader;
		blade = cwgreenSaberCoreShader;
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueCWGlowShader;
		blade = cwblueSaberCoreShader;
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleCWGlowShader;
		blade = cwpurpleSaberCoreShader;
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_RGB:
		glow = rgbCWGlowShader;
		blade = cwSaberCoreShader;
		break;
	case SABER_WHITE:
		glow = rgbCWGlowShader;
		blade = cwSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 1.0f);
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = cwblackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = rgbSaberCoreShader;
		break;
	}

	memset(&saber, 0, sizeof(refEntity_t));

	saber.saberLength = length;

	if (length < lengthMax)
	{
		radiusmult = 1.0 + 2.0 / length;		// Note this creates a curve, and length cannot be < 0.5.
	}
	else
	{
		radiusmult = 1.0;
	}

	float effectradius = (radius * 1.6f + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * cg_SFXSabersGlowSizeCW.value;

	float coreradius = (radius * 0.4f + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * cg_SFXSabersCoreSizeCW.value;

	effectradius *= 0.42f;
	coreradius *= 0.85f;

	// Main glow
	{
		float effectalpha = 0.8f;

		const float glowLength = length - effectradius * 0.5f;

		if (glowLength > 0.0f)
		{
			saber.radius = effectradius;
			saber.saberLength = glowLength;

			// Move glow slightly so its closer to the core
			VectorMA(origin, effectradius * 0.25f, dir, saber.origin);
			VectorCopy(dir, saber.axis[0]);

			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;

			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (snum == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}

			trap->R_AddRefEntityToScene(&saber);
		}
	}

	// Hot core
	VectorMA(origin, length, dir, saber.origin);
	VectorMA(origin, -1.0f, dir, saber.oldorigin);

	saber.customShader = blade;
	saber.reType = RT_LINE;
	saber.radius = coreradius;
	saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
	saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

	trap->R_AddRefEntityToScene(&saber);

	if (color != SABER_RGB)
	{
		return;
	}

	saber.customShader = cwSaberCoreShader;
	saber.reType = RT_LINE;
	saber.radius = coreradius;
	saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
	saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

	trap->R_AddRefEntityToScene(&saber);
}

static void UI_DoMaulSaber(vec3_t origin, vec3_t dir, float length, float lengthMax, float radius, saber_colors_t color, int snum)
{
	vec3_t		mid, rgb = { 1,1,1 };
	qhandle_t	blade = 0, glow = 0;
	refEntity_t saber;
	float radiusmult;

	if (length < 0.5f)
	{
		// if the thing is so short, just forget even adding me.
		return;
	}

	// Find the midpoint of the saber for lighting purposes
	VectorMA(origin, length * 0.5f, dir, mid);

	switch (color)
	{
	case SABER_RED:
		glow = redMaulGlowShader;
		blade = maulredSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.2f, 0.2f);
		break;
	case SABER_ORANGE:
		glow = orangeMaulGlowShader;
		blade = maulorangeSaberCoreShader;
		VectorSet(rgb, 1.0f, 0.5f, 0.1f);
		break;
	case SABER_YELLOW:
		glow = yellowMaulGlowShader;
		blade = maulyellowSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 0.2f);
		break;
	case SABER_GREEN:
		glow = greenMaulGlowShader;
		blade = maulgreenSaberCoreShader;
		VectorSet(rgb, 0.2f, 1.0f, 0.2f);
		break;
	case SABER_BLUE:
		glow = blueMaulGlowShader;
		blade = maulblueSaberCoreShader;
		VectorSet(rgb, 0.2f, 0.4f, 1.0f);
		break;
	case SABER_PURPLE:
		glow = purpleMaulGlowShader;
		blade = maulpurpleSaberCoreShader;
		VectorSet(rgb, 0.9f, 0.2f, 1.0f);
		break;
	case SABER_RGB:
		glow = rgbMaulGlowShader;
		blade = maulSaberCoreShader;
		break;
	case SABER_WHITE:
		glow = rgbMaulGlowShader;
		blade = maulSaberCoreShader;
		VectorSet(rgb, 1.0f, 1.0f, 1.0f);
		break;
	case SABER_BLACK:
		glow = blackSaberGlowShader;
		blade = maulblackSaberCoreShader;
		VectorSet(rgb, .0f, .0f, .0f);
		break;
	case SABER_UNSTABLE_RED:
		glow = redSaberGlowShader;
		blade = unstableRedSaberCoreShader;
		break;
	default:
		glow = rgbSaberGlowShader;
		blade = rgbSaberCoreShader;
		break;
	}

	memset(&saber, 0, sizeof(refEntity_t));

	saber.saberLength = length;

	if (length < lengthMax)
	{
		radiusmult = 1.0 + 2.0 / length;		// Note this creates a curve, and length cannot be < 0.5.
	}
	else
	{
		radiusmult = 1.0;
	}

	float effectradius = (radius * 1.6f + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * cg_SFXSabersGlowSizeMaul.value;

	float coreradius = (radius * 0.4f + Q_flrand(-1.0f, 1.0f) * 0.1f) * radiusmult * cg_SFXSabersCoreSizeMaul.value;

	effectradius *= 0.42f;
	coreradius *= 0.85f;

	// Main glow
	{
		float effectalpha = 0.8f;

		const float glowLength = length - effectradius * 0.5f;

		if (glowLength > 0.0f)
		{
			saber.radius = effectradius;
			saber.saberLength = glowLength;

			// Move glow slightly so its closer to the core
			VectorMA(origin, effectradius * 0.25f, dir, saber.origin);
			VectorCopy(dir, saber.axis[0]);

			saber.reType = RT_SABER_GLOW;
			saber.customShader = glow;

			saber.shaderRGBA[0] = 0xff * effectalpha;
			saber.shaderRGBA[1] = 0xff * effectalpha;
			saber.shaderRGBA[2] = 0xff * effectalpha;
			saber.shaderRGBA[3] = 0xff * effectalpha;

			if (color >= SABER_RGB)
			{
				if (snum == 0)
				{
					saber.shaderRGBA[0] = ui_rgb_saber_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber_blue.integer * effectalpha;
				}
				else
				{
					saber.shaderRGBA[0] = ui_rgb_saber2_red.integer * effectalpha;
					saber.shaderRGBA[1] = ui_rgb_saber2_green.integer * effectalpha;
					saber.shaderRGBA[2] = ui_rgb_saber2_blue.integer * effectalpha;
				}
			}
			trap->R_AddRefEntityToScene(&saber);
		}
	}

	// Hot core
	VectorMA(origin, length, dir, saber.origin);
	VectorMA(origin, -1.0f, dir, saber.oldorigin);

	saber.customShader = blade;
	saber.reType = RT_LINE;
	saber.radius = coreradius;
	saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
	saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

	trap->R_AddRefEntityToScene(&saber);

	if (color != SABER_RGB)
	{
		return;
	}

	saber.customShader = maulSaberCoreShader;
	saber.reType = RT_LINE;
	saber.radius = coreradius;
	saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
	saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

	trap->R_AddRefEntityToScene(&saber);
}

static void UI_SaberDrawBlade(itemDef_t* item, const char* saber_name, int saberModel, saberType_t saberType, vec3_t origin, vec3_t angles, int blade_num)
{
	char bladeColorString[MAX_QPATH];
	vec3_t	bladeOrigin = { 0 };
	matrix3_t	axis;
	mdxaBone_t	boltMatrix;
	qboolean tagHack = qfalse;
	int snum;

	memset(axis, 0, sizeof axis);

	if (item->flags & ITF_ISSABER && saberModel < 2)
	{
		snum = 0;
		trap->Cvar_VariableStringBuffer("ui_saber_color", bladeColorString, sizeof bladeColorString);
	}
	else
	{
		snum = 1;
		trap->Cvar_VariableStringBuffer("ui_saber2_color", bladeColorString, sizeof bladeColorString);
	}

	if (!trap->G2API_HasGhoul2ModelOnIndex(&item->ghoul2, saberModel))
	{//invalid index!
		return;
	}

	const saber_colors_t bladeColor = TranslateSaberColor(bladeColorString);

	const float bladeLength = UI_SaberBladeLengthForSaber(saber_name, blade_num);
	const float bladeRadius = UI_SaberBladeRadiusForSaber(saber_name, blade_num);

	const char* tagName = va("*blade%d", blade_num + 1);
	int bolt = trap->G2API_AddBolt(item->ghoul2, saberModel, tagName);

	if (bolt == -1)
	{
		tagHack = qtrue;
		//hmm, just fall back to the most basic tag (this will also make it work with pre-JKA saber models
		bolt = trap->G2API_AddBolt(item->ghoul2, saberModel, "*flash");
		if (bolt == -1)
		{//no tag_flash either?!!
			bolt = 0;
		}
	}

	trap->G2API_GetBoltMatrix(item->ghoul2, saberModel, bolt, &boltMatrix, angles, origin, uiInfo.uiDC.realTime, NULL, vec3_origin);//NULL was cgs.model_draw

	// work the matrix axis stuff into the original axis and origins used.
	BG_GiveMeVectorFromMatrix(&boltMatrix, ORIGIN, bladeOrigin);
	BG_GiveMeVectorFromMatrix(&boltMatrix, NEGATIVE_Y, axis[0]);//front (was NEGATIVE_Y, but the md3->glm exporter screws up this tag somethin' awful)
	//		...changed this back to NEGATIVE_Y
	BG_GiveMeVectorFromMatrix(&boltMatrix, NEGATIVE_X, axis[1]);//right ... and changed this to NEGATIVE_X
	BG_GiveMeVectorFromMatrix(&boltMatrix, POSITIVE_Z, axis[2]);//up

	// Where do I get scale from?

	if (tagHack)
	{
		const float scale = 1.0f;
		switch (saberType)
		{
		case SABER_SINGLE:
		case SABER_SINGLE_CLASSIC:
		case SABER_UNSTABLE:
		case SABER_THIN:
		case SABER_SFX:
		case SABER_CUSTOMSFX:
		case SABER_YODA:
		case SABER_DOOKU:
		case SABER_BACKHAND:
		case SABER_PALP:
		case SABER_ANAKIN:
		case SABER_GRIE:
		case SABER_GRIE4:
		case SABER_OBIWAN:
		case SABER_ASBACKHAND:
		case SABER_WINDU:
		case SABER_VADER:
		case SABER_KENOBI:
		case SABER_REY:
			VectorMA(bladeOrigin, scale, axis[0], bladeOrigin);
			break;
		case SABER_DAGGER:
		case SABER_LANCE:
			break;
		case SABER_STAFF:
		case SABER_STAFF_UNSTABLE:
		case SABER_STAFF_THIN:
		case SABER_STAFF_SFX:
		case SABER_STAFF_MAUL:
		case SABER_ELECTROSTAFF:
			if (blade_num == 0)
			{
				VectorMA(bladeOrigin, 12 * scale, axis[0], bladeOrigin);
			}
			if (blade_num == 1)
			{
				VectorScale(axis[0], -1, axis[0]);
				VectorMA(bladeOrigin, 12 * scale, axis[0], bladeOrigin);
			}
			break;
		case SABER_BROAD:
			if (blade_num == 0)
			{
				VectorMA(bladeOrigin, -1 * scale, axis[1], bladeOrigin);
			}
			else if (blade_num == 1)
			{
				VectorMA(bladeOrigin, 1 * scale, axis[1], bladeOrigin);
			}
			break;
		case SABER_PRONG:
			if (blade_num == 0)
			{
				VectorMA(bladeOrigin, -3 * scale, axis[1], bladeOrigin);
			}
			else if (blade_num == 1)
			{
				VectorMA(bladeOrigin, 3 * scale, axis[1], bladeOrigin);
			}
			break;
		case SABER_ARC:
			VectorSubtract(axis[1], axis[2], axis[1]);
			VectorNormalize(axis[1]);
			switch (blade_num)
			{
			case 0:
				VectorMA(bladeOrigin, 8 * scale, axis[0], bladeOrigin);
				VectorScale(axis[0], 0.75f, axis[0]);
				VectorScale(axis[1], 0.25f, axis[1]);
				VectorAdd(axis[0], axis[1], axis[0]);
				break;
			case 1:
				VectorScale(axis[0], 0.25f, axis[0]);
				VectorScale(axis[1], 0.75f, axis[1]);
				VectorAdd(axis[0], axis[1], axis[0]);
				break;
			case 2:
				VectorMA(bladeOrigin, -8 * scale, axis[0], bladeOrigin);
				VectorScale(axis[0], -0.25f, axis[0]);
				VectorScale(axis[1], 0.75f, axis[1]);
				VectorAdd(axis[0], axis[1], axis[0]);
				break;
			case 3:
				VectorMA(bladeOrigin, -16 * scale, axis[0], bladeOrigin);
				VectorScale(axis[0], -0.75f, axis[0]);
				VectorScale(axis[1], 0.25f, axis[1]);
				VectorAdd(axis[0], axis[1], axis[0]);
				break;
			default:;
			}
			break;
		case SABER_SAI:
			if (blade_num == 1)
			{
				VectorMA(bladeOrigin, -3 * scale, axis[1], bladeOrigin);
			}
			else if (blade_num == 2)
			{
				VectorMA(bladeOrigin, 3 * scale, axis[1], bladeOrigin);
			}
			break;
		case SABER_CLAW:
			switch (blade_num)
			{
			case 0:
				VectorMA(bladeOrigin, 2 * scale, axis[0], bladeOrigin);
				VectorMA(bladeOrigin, 2 * scale, axis[2], bladeOrigin);
				break;
			case 1:
				VectorMA(bladeOrigin, 2 * scale, axis[0], bladeOrigin);
				VectorMA(bladeOrigin, 2 * scale, axis[2], bladeOrigin);
				VectorMA(bladeOrigin, 2 * scale, axis[1], bladeOrigin);
				break;
			case 2:
				VectorMA(bladeOrigin, 2 * scale, axis[0], bladeOrigin);
				VectorMA(bladeOrigin, 2 * scale, axis[2], bladeOrigin);
				VectorMA(bladeOrigin, -2 * scale, axis[1], bladeOrigin);
				break;
			default:;
			}
			break;
		case SABER_STAR:
			switch (blade_num)
			{
			case 0:
				VectorMA(bladeOrigin, 8 * scale, axis[0], bladeOrigin);
				break;
			case 1:
				VectorScale(axis[0], 0.33f, axis[0]);
				VectorScale(axis[2], 0.67f, axis[2]);
				VectorAdd(axis[0], axis[2], axis[0]);
				VectorMA(bladeOrigin, 8 * scale, axis[0], bladeOrigin);
				break;
			case 2:
				VectorScale(axis[0], -0.33f, axis[0]);
				VectorScale(axis[2], 0.67f, axis[2]);
				VectorAdd(axis[0], axis[2], axis[0]);
				VectorMA(bladeOrigin, 8 * scale, axis[0], bladeOrigin);
				break;
			case 3:
				VectorScale(axis[0], -1, axis[0]);
				VectorMA(bladeOrigin, 8 * scale, axis[0], bladeOrigin);
				break;
			case 4:
				VectorScale(axis[0], -0.33f, axis[0]);
				VectorScale(axis[2], -0.67f, axis[2]);
				VectorAdd(axis[0], axis[2], axis[0]);
				VectorMA(bladeOrigin, 8 * scale, axis[0], bladeOrigin);
				break;
			case 5:
				VectorScale(axis[0], 0.33f, axis[0]);
				VectorScale(axis[2], -0.67f, axis[2]);
				VectorAdd(axis[0], axis[2], axis[0]);
				VectorMA(bladeOrigin, 8 * scale, axis[0], bladeOrigin);
				break;
			default:;
			}
			break;
		case SABER_TRIDENT:
			switch (blade_num)
			{
			case 0:
				VectorMA(bladeOrigin, 24 * scale, axis[0], bladeOrigin);
				break;
			case 1:
				VectorMA(bladeOrigin, -6 * scale, axis[1], bladeOrigin);
				VectorMA(bladeOrigin, 24 * scale, axis[0], bladeOrigin);
				break;
			case 2:
				VectorMA(bladeOrigin, 6 * scale, axis[1], bladeOrigin);
				VectorMA(bladeOrigin, 24 * scale, axis[0], bladeOrigin);
				break;
			case 3:
				VectorMA(bladeOrigin, -32 * scale, axis[0], bladeOrigin);
				VectorScale(axis[0], -1, axis[0]);
				break;
			default:;
			}
			break;
		case SABER_SITH_SWORD:
			//no blade
			break;
		default:
			break;
		}
	}
	if (saberType == SABER_SITH_SWORD)
	{//draw no blade
		return;
	}

	if (cg_SFXSabers.integer < 1)
	{// Draw the Raven blade.
		if (saberType == SABER_UNSTABLE ||
			saberType == SABER_STAFF_UNSTABLE ||
			saberType == SABER_ELECTROSTAFF)
		{
			UI_DoSaberUnstable(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
		}
		else
		{
			UI_DoSaber(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
		}
	}
	else if (saberType != SABER_SITH_SWORD)
	{
		switch (cg_SFXSabers.integer)
		{
		case 1:
			if (saberType == SABER_UNSTABLE ||
				saberType == SABER_STAFF_UNSTABLE ||
				saberType == SABER_ELECTROSTAFF)
			{
				UI_DoSaberUnstable(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			else
			{
				UI_DoEp1Saber(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			break;
		case 2:
			if (saberType == SABER_UNSTABLE ||
				saberType == SABER_STAFF_UNSTABLE ||
				saberType == SABER_ELECTROSTAFF)
			{
				UI_DoSaberUnstable(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			else
			{
				UI_DoEp2Saber(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			break;
		case 3:
			if (saberType == SABER_UNSTABLE ||
				saberType == SABER_STAFF_UNSTABLE ||
				saberType == SABER_ELECTROSTAFF)
			{
				UI_DoSaberUnstable(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			else
			{
				UI_DoEp3Saber(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			break;
		case 4:
			if (saberType == SABER_UNSTABLE ||
				saberType == SABER_STAFF_UNSTABLE ||
				saberType == SABER_ELECTROSTAFF)
			{
				UI_DoSaberUnstable(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			else
			{
				UI_DoSFXSaber(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			break;
		case 5:
			if (saberType == SABER_UNSTABLE ||
				saberType == SABER_STAFF_UNSTABLE ||
				saberType == SABER_ELECTROSTAFF)
			{
				UI_DoSaberUnstable(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			else
			{
				UI_DoOTSaber(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			break;
		case 6:
			if (saberType == SABER_UNSTABLE ||
				saberType == SABER_STAFF_UNSTABLE ||
				saberType == SABER_ELECTROSTAFF)
			{
				UI_DoSaberUnstable(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			else
			{
				UI_DoRotJSaber(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			break;
		case 7:
			if (saberType == SABER_UNSTABLE ||
				saberType == SABER_STAFF_UNSTABLE ||
				saberType == SABER_ELECTROSTAFF)
			{
				UI_DoSaberUnstable(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			else
			{
				UI_DoTFASaber(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			break;
		case 8:
			if (saberType == SABER_UNSTABLE ||
				saberType == SABER_STAFF_UNSTABLE ||
				saberType == SABER_ELECTROSTAFF)
			{
				UI_DoSaberUnstable(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			else
			{
				UI_DoUnstableSaber(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			break;
		case 9:
			if (saberType == SABER_UNSTABLE ||
				saberType == SABER_STAFF_UNSTABLE ||
				saberType == SABER_ELECTROSTAFF)
			{
				UI_DoSaberUnstable(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			else
			{
				UI_DoRebelsSaber(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			break;
		case 10:
			if (saberType == SABER_UNSTABLE ||
				saberType == SABER_STAFF_UNSTABLE ||
				saberType == SABER_ELECTROSTAFF)
			{
				UI_DoSaberUnstable(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			else
			{
				UI_DoCWSaber(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			break;
		case 11:
			if (saberType == SABER_UNSTABLE ||
				saberType == SABER_STAFF_UNSTABLE ||
				saberType == SABER_ELECTROSTAFF)
			{
				UI_DoSaberUnstable(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			else
			{
				UI_DoMaulSaber(bladeOrigin, axis[0], bladeLength, bladeLength, bladeRadius, bladeColor, snum);
			}
			break;
		default:;
		}
	}
}

static void UI_GetSaberForMenu(char* saber, int saber_num)
{
	char saberTypeString[MAX_QPATH] = { 0 };
	saberType_t saberType = SABER_NONE;

	if (saber_num == 0)
	{
		trap->Cvar_VariableStringBuffer("ui_saber", saber, MAX_QPATH);
		if (!UI_SaberValidForPlayerInMP(saber))
		{
			trap->Cvar_Set("ui_saber", DEFAULT_SABER);
			trap->Cvar_VariableStringBuffer("ui_saber", saber, MAX_QPATH);
		}
	}
	else
	{
		trap->Cvar_VariableStringBuffer("ui_saber2", saber, MAX_QPATH);
		if (!UI_SaberValidForPlayerInMP(saber))
		{
			trap->Cvar_Set("ui_saber2", DEFAULT_SABER);
			trap->Cvar_VariableStringBuffer("ui_saber2", saber, MAX_QPATH);
		}
	}
	//read this from the sabers.cfg
	UI_SaberTypeForSaber(saber, saberTypeString);
	if (saberTypeString[0])
	{
		saberType = TranslateSaberType(saberTypeString);
	}

	switch (uiInfo.movesTitleIndex)
	{
	case 0://MD_ACROBATICS:
		break;
	case 1://MD_SINGLE_FAST:
	case 2://MD_SINGLE_MEDIUM:
	case 3://MD_SINGLE_STRONG:
		if (saberType != SABER_SINGLE)
		{
			Q_strncpyz(saber, "saber_1", MAX_QPATH);
		}
		break;
	case 4://MD_DUAL_SABERS:
		if (saberType != SABER_SINGLE)
		{
			Q_strncpyz(saber, "saber_1", MAX_QPATH);
		}
		break;
	case 5://MD_SABER_STAFF:
		if (saberType == SABER_SINGLE || saberType == SABER_NONE)
		{
			Q_strncpyz(saber, "staff_1", MAX_QPATH);
		}
		break;
	default:;
	}
}

void UI_SaberDrawBlades(itemDef_t* item, vec3_t origin, vec3_t angles)
{
	//NOTE: only allows one saber type in view at a time
	int saberModel;
	int	numSabers = 1;

	if (item->flags & ITF_ISCHARACTER//hacked sabermoves sabers in character's hand
		&& uiInfo.movesTitleIndex == 4 /*MD_DUAL_SABERS*/)
	{
		numSabers = 2;
	}

	for (int saber_num = 0; saber_num < numSabers; saber_num++)
	{
		char saber[MAX_QPATH];
		if (item->flags & ITF_ISCHARACTER)//hacked sabermoves sabers in character's hand
		{
			UI_GetSaberForMenu(saber, saber_num);
			saberModel = saber_num + 1;
		}
		else if (item->flags & ITF_ISSABER)
		{
			trap->Cvar_VariableStringBuffer("ui_saber", saber, sizeof saber);
			if (!UI_SaberValidForPlayerInMP(saber))
			{
				trap->Cvar_Set("ui_saber", DEFAULT_SABER);
				trap->Cvar_VariableStringBuffer("ui_saber", saber, sizeof saber);
			}
			saberModel = 0;
		}
		else if (item->flags & ITF_ISSABER2)
		{
			trap->Cvar_VariableStringBuffer("ui_saber2", saber, sizeof saber);
			if (!UI_SaberValidForPlayerInMP(saber))
			{
				trap->Cvar_Set("ui_saber2", DEFAULT_SABER);
				trap->Cvar_VariableStringBuffer("ui_saber2", saber, sizeof saber);
			}
			saberModel = 0;
		}
		else
		{
			return;
		}
		if (saber[0])
		{
			const int numBlades = UI_saber_numBladesForSaber(saber);
			if (numBlades)
			{//okay, here we go, time to draw each blade...
				char	saberTypeString[MAX_QPATH] = { 0 };
				UI_SaberTypeForSaber(saber, saberTypeString);
				const saberType_t saberType = TranslateSaberType(saberTypeString);
				for (int curBlade = 0; curBlade < numBlades; curBlade++)
				{
					if (UI_SaberShouldDrawBlade(saber, curBlade))
					{
						UI_SaberDrawBlade(item, saber, saberModel, saberType, origin, angles, curBlade);
					}
				}
			}
		}
	}
}

void UI_SaberAttachToChar(itemDef_t* item)
{
	int	numSabers = 1;

	if (trap->G2API_HasGhoul2ModelOnIndex(&item->ghoul2, 2))
	{//remove any extra models
		trap->G2API_RemoveGhoul2Model(&item->ghoul2, 2);
	}
	if (trap->G2API_HasGhoul2ModelOnIndex(&item->ghoul2, 1))
	{//remove any extra models
		trap->G2API_RemoveGhoul2Model(&item->ghoul2, 1);
	}

	if (uiInfo.movesTitleIndex == 4 /*MD_DUAL_SABERS*/)
	{
		numSabers = 2;
	}

	for (int saber_num = 0; saber_num < numSabers; saber_num++)
	{
		//bolt sabers
		char modelPath[MAX_QPATH];
		char saber[MAX_QPATH];

		UI_GetSaberForMenu(saber, saber_num);

		if (UI_SaberModelForSaber(saber, modelPath))
		{//successfully found a model
			const int g2Saber = trap->G2API_InitGhoul2Model(&item->ghoul2, modelPath, 0, 0, 0, 0, 0); //add the model
			if (g2Saber)
			{
				char skin_path[MAX_QPATH];
				int boltNum;
				//get the customSkin, if any
				if (UI_SaberSkinForSaber(saber, skin_path))
				{
					const int g2skin = trap->R_RegisterSkin(skin_path);
					trap->G2API_SetSkin(item->ghoul2, g2Saber, 0, g2skin);//this is going to set the surfs on/off matching the skin file
				}
				else
				{
					trap->G2API_SetSkin(item->ghoul2, g2Saber, 0, 0);//turn off custom skin
				}
				if (saber_num == 0)
				{
					boltNum = trap->G2API_AddBolt(item->ghoul2, 0, "*r_hand");
				}
				else
				{
					boltNum = trap->G2API_AddBolt(item->ghoul2, 0, "*l_hand");
				}
				trap->G2API_AttachG2Model(item->ghoul2, g2Saber, item->ghoul2, boltNum, 0);
			}
		}
	}
}