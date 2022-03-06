#ifndef __GFXINFO_H__
#define __GFXINFO_H__

typedef enum {
	TX_PLAY_CIRCLE,
	TX_PLAY_CIRCLEOVERLAY,
	TX_PLAY_CIRCLEAPPROACH,
	TX_PLAY_SPINNER,
	TX_PLAY_SPINNERBG,
	TX_PLAY_SPINNERBARS,
	TX_PLAY_SLIDERB0,
	TX_PLAY_SLIDERB1,
	TX_PLAY_SLIDERB2,
	TX_PLAY_SLIDERB3,
	TX_PLAY_SLIDERB4,
	TX_PLAY_SLIDERB5,
	TX_PLAY_SLIDERB6,
	TX_PLAY_SLIDERB7,
	TX_PLAY_SLIDERB8,
	TX_PLAY_SLIDERB9,
	TX_PLAY_DISC,
	TX_PLAY_SLIDERFOLLOW,
	TX_PLAY_SLIDERTICK,
	TX_PLAY_SLIDERREVERSE,
	
	TX_PLAY_HIT0,
	TX_PLAY_HIT50,
	TX_PLAY_HIT100,
	TX_PLAY_HIT100K,
	TX_PLAY_HIT300,
	TX_PLAY_HIT300K,
	TX_PLAY_HIT300G,
	TX_PLAY_SLIDER10,
	TX_PLAY_SLIDER30,
	
	TX_PLAY_SCOREBAR,
	TX_PLAY_SCOREBAR_BAR,
	TX_PLAY_SCOREBAR_KI,
	TX_PLAY_SCOREBAR_KIDANGER,
	TX_PLAY_SCOREBAR_KIDANGER2,

    TX_RANKING_A_LG,
    TX_RANKING_B_LG,
    TX_RANKING_C_LG,
    TX_RANKING_D_LG,
    TX_RANKING_S_LG,
    TX_RANKING_SS_LG,

    TX_RANKING_F_LG,

    TX_GAMEOVER_BG,
    TX_PAUSE_BG,
	
	TX_WHITE,

    TX_CURRENT_BG,
    TX_PLAY_DARKEN_OVERLAY,

    // UI TEXTURES:
    TX_WELCOME_BG,
    TX_SONGSELECT_BG,
    TX_BUTTON_BIG,
    TX_BUTTON_MED,
    TX_BUTTON_SM,
    TX_BUTTON_SM_ACTIVE,
    TX_BUTTON_XS,
    TX_BUTTON_ARROW,
    TX_BUTTON_PLAY,
    TX_BEATMAP_ENTRY_BG,
    TX_BEATMAP_ENTRY_EXPANDED_BG,

    TX_BEATMAP_ENTRY_PREVIEW,
    TX_BEATMAP_ENTRY_PIC_0,
    TX_BEATMAP_ENTRY_PIC_1,
    TX_BEATMAP_ENTRY_PIC_2,
    TX_BEATMAP_ENTRY_PIC_3,
    TX_RANKING_UNK_SM,
    TX_RANKING_A_SM,
    TX_RANKING_B_SM,
    TX_RANKING_C_SM,
    TX_RANKING_D_SM,
    TX_RANKING_S_SM,
    TX_RANKING_SS_SM,
    TX_STARS,
    TX_STARS_FILL
} TextureType;

#endif
