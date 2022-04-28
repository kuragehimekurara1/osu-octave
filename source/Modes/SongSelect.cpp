#include "SongSelect.h"
#include "DataStorage/I18n.h"

OOUInt SongSelect::mCurrentPage = 0;
OOUInt SongSelect::mCountPages  = 0;
OOUInt SongSelect::mSongListSize  = 0;
bool SongSelect::shouldHandlePageUpdate = false;

OOUInt SongSelect::mExpandEntryIndex = -1;
bool SongSelect::shouldHandleEntryExpand = false;
bool SongSelect::shouldExpandRandomEntry = false;

pSprite* SongSelect::btn_sort_all = nullptr;
pText* SongSelect::btn_sort_all_label = nullptr;
pSprite* SongSelect::btn_sort_a_e = nullptr;
pText* SongSelect::btn_sort_a_e_label = nullptr;
pSprite* SongSelect::btn_sort_f_j = nullptr;
pText* SongSelect::btn_sort_f_j_label = nullptr;
pSprite* SongSelect::btn_sort_k_o = nullptr;
pText* SongSelect::btn_sort_k_o_label = nullptr;
pSprite* SongSelect::btn_sort_p_t = nullptr;
pText* SongSelect::btn_sort_p_t_label = nullptr;
pSprite* SongSelect::btn_sort_u_z = nullptr;
pText* SongSelect::btn_sort_u_z_label = nullptr;
pSprite* SongSelect::loadingScreenBG = nullptr;
pText* SongSelect::loadingScreenLabel = nullptr;

SpriteManager* SongSelect::mSpriteManager = nullptr;

void OnBetmapEntryPlayClick(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_MENUHIT);
    SongSelect::LoadingScreenShow();
    InputHelper::vitaUseBackTouch = Settings::get_bool("vitaUseBackTouch"); // VITA: Reset back touch usage flag
    BeatmapManager::Load(self->Tag);
    ChangeModeOnFrameEnd(MODE_PLAYER);
    //SongSelect::LoadingScreenHide();
}

void OnBetmapEntryExpandClick(pDrawable* self, OOInt x, OOInt y) {
    int index = self->TagInt;

    SongSelect::ExpandEntry(index);
}

void OnBtnQuitClick(pDrawable* self, OOInt x, OOInt y) {
    SDL_Event sdlevent;
    sdlevent.type = SDL_QUIT;
    SDL_PushEvent(&sdlevent);
}

void OnBtnSettingsClick(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_MENUCLICK);
    ChangeModeOnFrameEnd(MODE_SETTINGS);
}

void OnBtnAboutClick(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_MENUCLICK);
    ChangeModeOnFrameEnd(MODE_ABOUT);
}

void OnBtnRandomClick(pDrawable* self, OOInt x, OOInt y) {
    SongSelect::PageRand();
}

void OnBtnArrowLeftClick(pDrawable* self, OOInt x, OOInt y) {
    SongSelect::PagePrev();
}

void OnBtnArrowRightClick(pDrawable* self, OOInt x, OOInt y) {
    SongSelect::PageNext();
}

void OnBtnSortAllClick(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_CLICK_SHORT_CONFIRM);
    SongSelect::ApplyFilter(FILTER_NONE);
}

void OnBtnSortAEClick(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_CLICK_SHORT_CONFIRM);
    SongSelect::ApplyFilter(FILTER_ALPHA_A_E);
}

void OnBtnSortFJClick(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_CLICK_SHORT_CONFIRM);
    SongSelect::ApplyFilter(FILTER_ALPHA_F_J);
}

void OnBtnSortKOClick(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_CLICK_SHORT_CONFIRM);
    SongSelect::ApplyFilter(FILTER_ALPHA_K_O);
}

void OnBtnSortPTClick(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_CLICK_SHORT_CONFIRM);
    SongSelect::ApplyFilter(FILTER_ALPHA_P_T);
}

void OnBtnSortUZClick(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_CLICK_SHORT_CONFIRM);
    SongSelect::ApplyFilter(FILTER_ALPHA_U_Z);
}

SongSelect::SongSelect() {
    InputHelper::vitaUseBackTouch = false; // VITA: Disable back touch for menu.

    AudioManager::Engine().PlayBGM();

    GraphicsManager::Graphics().LoadTexturesForMode(MODE_SONGSELECT);
    PreviewBuffer::GetInstance().Update(-1, 0, mEntriesPerPage);

    delete mSpriteManager;
    mSpriteManager = new SpriteManager();

    auto* bg = new pSprite(TX_MENU_BG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255);
    mSpriteManager->Add(bg);

    mLogo = new Logo(176, 145);
    mLogo->AddToSpriteManager(mSpriteManager);

    auto* btn_about = new pSprite(TX_BUTTON_BIG, 37, 281, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_about->OnClick = OnBtnAboutClick;
    btn_about->Clickable = true;
    mSpriteManager->Add(btn_about);
    auto* btn_about_label = new pText(I18n::get("btn_about"), FONT_PRIMARY, 175, 308, Skins::get_options().FontColor_MenuButton);
    btn_about_label->Z = -0.02f;
    btn_about_label->Origin = ORIGIN_CENTER;
    mSpriteManager->Add(btn_about_label);

    auto* btn_settings = new pSprite(TX_BUTTON_BIG, 37, 349, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_settings->OnClick = OnBtnSettingsClick;
    btn_settings->Clickable = true;
    mSpriteManager->Add(btn_settings);
    auto* btn_settings_label = new pText(I18n::get("btn_settings"), FONT_PRIMARY, 175, 376, Skins::get_options().FontColor_MenuButton);
    btn_settings_label->Z = -0.02f;
    btn_settings_label->Origin = ORIGIN_CENTER;
    mSpriteManager->Add(btn_settings_label);

    auto* btn_quit = new pSprite(TX_BUTTON_BIG, 37, 418, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_quit->OnClick = OnBtnQuitClick;
    btn_quit->Clickable = true;
    mSpriteManager->Add(btn_quit);
    auto* btn_quit_label = new pText(I18n::get("btn_quit"), FONT_PRIMARY, 175, 445, Skins::get_options().FontColor_MenuButton);
    btn_quit_label->Z = -0.02f;
    btn_quit_label->Origin = ORIGIN_CENTER;
    mSpriteManager->Add(btn_quit_label);

    auto* btn_random = new pSprite(TX_BUTTON_MED, 400, 478, 207, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_random->OnClick = OnBtnRandomClick;
    btn_random->Clickable = true;
    mSpriteManager->Add(btn_random);
    auto* btn_random_label = new pText(I18n::get("btn_random"), FONT_PRIMARY, 503, 498, Skins::get_options().FontColor_MenuButton);
    btn_random_label->Z = -0.02f;
    btn_random_label->Origin = ORIGIN_CENTER;
    mSpriteManager->Add(btn_random_label);

    auto* btn_arrow_left = new pSprite(TX_BUTTON_ARROW, (780-78), 478, 78, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_arrow_left->Angle = 180;
    btn_arrow_left->OnClick = OnBtnArrowLeftClick;
    btn_arrow_left->Clickable = true;
    btn_arrow_left->ExtendedClickableArea = true;
    mSpriteManager->Add(btn_arrow_left);

    auto* btn_arrow_right = new pSprite(TX_BUTTON_ARROW, 841, 478, 78, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_arrow_right->OnClick = OnBtnArrowRightClick;
    btn_arrow_right->Clickable = true;
    btn_arrow_right->ExtendedClickableArea = true;
    mSpriteManager->Add(btn_arrow_right);

    btn_sort_all = new pSprite(TX_BUTTON_SM_ACTIVE, 400, 25, 78, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_all->OnClick = OnBtnSortAllClick;
    btn_sort_all->Clickable = true;
    btn_sort_all->ExtendedClickableArea = true;
    mSpriteManager->Add(btn_sort_all);
    btn_sort_all_label = new pText(I18n::get("filter_all"), FONT_PRIMARY, 439, 45, Skins::get_options().FontColor_SortingButtonActive);
    btn_sort_all_label->Z = -0.02f;
    btn_sort_all_label->Origin = ORIGIN_CENTER;
    mSpriteManager->Add(btn_sort_all_label);

    btn_sort_a_e = new pSprite(TX_BUTTON_XS, 536, 25, 67, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_a_e->OnClick = OnBtnSortAEClick;
    btn_sort_a_e->Clickable = true;
    btn_sort_a_e->ExtendedClickableArea = true;
    mSpriteManager->Add(btn_sort_a_e);
    btn_sort_a_e_label = new pText(I18n::get("filter_a_e"), FONT_PRIMARY, 569, 45, Skins::get_options().FontColor_SortingButton);
    btn_sort_a_e_label->Z = -0.02f;
    btn_sort_a_e_label->Origin = ORIGIN_CENTER;
    mSpriteManager->Add(btn_sort_a_e_label);

    btn_sort_f_j = new pSprite(TX_BUTTON_XS, 615, 25, 67, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_f_j->OnClick = OnBtnSortFJClick;
    btn_sort_f_j->Clickable = true;
    btn_sort_f_j->ExtendedClickableArea = true;
    mSpriteManager->Add(btn_sort_f_j);
    btn_sort_f_j_label = new pText(I18n::get("filter_f_j"), FONT_PRIMARY, 648, 45, Skins::get_options().FontColor_SortingButton);
    btn_sort_f_j_label->Z = -0.02f;
    btn_sort_f_j_label->Origin = ORIGIN_CENTER;
    mSpriteManager->Add(btn_sort_f_j_label);

    btn_sort_k_o = new pSprite(TX_BUTTON_XS, 694, 25, 67, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_k_o->OnClick = OnBtnSortKOClick;
    btn_sort_k_o->Clickable = true;
    btn_sort_k_o->ExtendedClickableArea = true;
    mSpriteManager->Add(btn_sort_k_o);
    btn_sort_k_o_label = new pText(I18n::get("filter_k_o"), FONT_PRIMARY, 727, 45, Skins::get_options().FontColor_SortingButton);
    btn_sort_k_o_label->Z = -0.02f;
    btn_sort_k_o_label->Origin = ORIGIN_CENTER;
    mSpriteManager->Add(btn_sort_k_o_label);

    btn_sort_p_t = new pSprite(TX_BUTTON_XS, 772, 25, 67, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_p_t->OnClick = OnBtnSortPTClick;
    btn_sort_p_t->Clickable = true;
    btn_sort_p_t->ExtendedClickableArea = true;
    mSpriteManager->Add(btn_sort_p_t);
    btn_sort_p_t_label = new pText(I18n::get("filter_p_t"), FONT_PRIMARY, 806, 45, Skins::get_options().FontColor_SortingButton);
    btn_sort_p_t_label->Z = -0.02f;
    btn_sort_p_t_label->Origin = ORIGIN_CENTER;
    mSpriteManager->Add(btn_sort_p_t_label);

    btn_sort_u_z = new pSprite(TX_BUTTON_XS, 852, 25, 67, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_u_z->OnClick = OnBtnSortUZClick;
    btn_sort_u_z->Clickable = true;
    btn_sort_u_z->ExtendedClickableArea = true;
    mSpriteManager->Add(btn_sort_u_z);
    btn_sort_u_z_label = new pText(I18n::get("filter_u_z"), FONT_PRIMARY, 885, 45, Skins::get_options().FontColor_SortingButton);
    btn_sort_u_z_label->Z = -0.02f;
    btn_sort_u_z_label->Origin = ORIGIN_CENTER;
    mSpriteManager->Add(btn_sort_u_z_label);

    loadingScreenBG = new pSprite(TX_SONGLESECT_LOADING_BG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 0, -100.f);
    loadingScreenLabel = new pText(I18n::get("loading"), FONT_NUMBERING, 480, 274, Skins::get_options().FontColor_LoadingScreen);
    loadingScreenLabel->Origin = ORIGIN_CENTER;
    loadingScreenLabel->Z = -150.f;
    loadingScreenLabel->Alpha = 0;
    mSpriteManager->Add(loadingScreenBG);
    mSpriteManager->Add(loadingScreenLabel);

    ApplyFilter(Settings::get_beatmapfilter("activeFilter"), false);

    mSongListSize = BeatmapManager::SongCount();
    mCurrentPage = Settings::get_int("page");
    mCountPages  = ceil(((float)mSongListSize/(float)mEntriesPerPage));

    UpdateSonglist(true);
}

void SongSelect::ApplyFilter(BeatmapFilter f, bool resetPage) {
    BeatmapManager::Filter(f);
    if (resetPage) {
        mCurrentPage = 0;
        Settings::set_int("page", (OOInt)mCurrentPage);
    }

    if (PreviewBuffer::GetInstance().lastAppliedFilter != f) {
        PreviewBuffer::GetInstance().Pics_ResetBuffer();
        PreviewBuffer::GetInstance().Update(-1, 0, mEntriesPerPage);
        PreviewBuffer::GetInstance().lastAppliedFilter = f;
    }

    shouldHandlePageUpdate = true;
    mSongListSize = BeatmapManager::SongCount();
    mCountPages  = ceil(((float)mSongListSize/(float)mEntriesPerPage));
    Settings::set_beatmapfilter("activeFilter", f);

    SDL_Color font_color_normal = Skins::get_options().FontColor_SortingButton;
    SDL_Color font_color_active = Skins::get_options().FontColor_SortingButtonActive;

    btn_sort_all->Texture = TX_BUTTON_SM;
    btn_sort_all_label->Color = font_color_normal;
    btn_sort_a_e->Texture = TX_BUTTON_XS;
    btn_sort_a_e_label->Color = font_color_normal;
    btn_sort_f_j->Texture = TX_BUTTON_XS;
    btn_sort_f_j_label->Color = font_color_normal;
    btn_sort_k_o->Texture = TX_BUTTON_XS;
    btn_sort_k_o_label->Color = font_color_normal;
    btn_sort_p_t->Texture = TX_BUTTON_XS;
    btn_sort_p_t_label->Color = font_color_normal;
    btn_sort_u_z->Texture = TX_BUTTON_XS;
    btn_sort_u_z_label->Color = font_color_normal;

    switch (f) {
        case FILTER_NONE:
            btn_sort_all->Texture = TX_BUTTON_SM_ACTIVE;
            btn_sort_all_label->Color = font_color_active;
            break;
        case FILTER_ALPHA_A_E:
            btn_sort_a_e->Texture = TX_BUTTON_XS_ACTIVE;
            btn_sort_a_e_label->Color = font_color_active;
            break;
        case FILTER_ALPHA_F_J:
            btn_sort_f_j->Texture = TX_BUTTON_XS_ACTIVE;
            btn_sort_f_j_label->Color = font_color_active;
            break;
        case FILTER_ALPHA_K_O:
            btn_sort_k_o->Texture = TX_BUTTON_XS_ACTIVE;
            btn_sort_k_o_label->Color = font_color_active;
            break;
        case FILTER_ALPHA_P_T:
            btn_sort_p_t->Texture = TX_BUTTON_XS_ACTIVE;
            btn_sort_p_t_label->Color = font_color_active;
            break;
        case FILTER_ALPHA_U_Z:
            btn_sort_u_z->Texture = TX_BUTTON_XS_ACTIVE;
            btn_sort_u_z_label->Color = font_color_active;
            break;
    }
}

void SongSelect::UpdateSonglist(bool initial)
{
    OOUInt spritesToRemoveCount = mSpritesPerBeatmapEntry * mEntriesDisplayed;
    if (mEntryExpanded) spritesToRemoveCount += (mSpritesPerExpandedBeatmapEntry - mSpritesPerBeatmapEntry);
    if (!initial) spritesToRemoveCount++; // to account for current_page_label, unless it's initial update.
    for (int i = 0; i < spritesToRemoveCount; ++i) {
        mSpriteManager->RemoveLast();
    }
    mEntryExpanded = false;
    mEntriesDisplayed = 0;

    if (shouldHandleEntryExpand && mExpandEntryIndex == -1) {
        shouldHandleEntryExpand = false;
    }

    OOUInt beatmap_list_offset = mCurrentPage * mEntriesPerPage;
    OOUInt entries_on_page = mEntriesPerPage;

    if (entries_on_page >= mSongListSize) {
        entries_on_page = mSongListSize;
        beatmap_list_offset = 0;
        mCurrentPage = 0;
    }

    if (entries_on_page != 0 && (beatmap_list_offset + (entries_on_page-1) >= mSongListSize)) {
        entries_on_page = mSongListSize - beatmap_list_offset;
    }

    // Skip drawing first or last entry if we need to expand an entry and there's not enough free space
    bool skipFirstEntry = false;
    bool skipLastEntry = false;

    if (shouldHandleEntryExpand && entries_on_page == mEntriesPerPage) {
        // First entry. Hide if:
        if ((mExpandEntryIndex == (beatmap_list_offset+entries_on_page-1)) // entry to expand is the last...
            || (mExpandEntryIndex == (beatmap_list_offset+entries_on_page-2))) // or second to last entry.
        {
            skipFirstEntry = true;
        }

        // Last entry. Hide if:
        if ((mExpandEntryIndex == beatmap_list_offset) // entry to expand is the first...
            || mExpandEntryIndex == beatmap_list_offset+1) // or second entry.
        {
            skipLastEntry = true;
        }
    }

    for (OOUInt i = 0; i < entries_on_page; i++) {
        if ((i == 0 && skipFirstEntry) || (i == (entries_on_page-1) && skipLastEntry)) {
            continue;
        }

        bool isExpanded = (shouldHandleEntryExpand && (mExpandEntryIndex == (i+beatmap_list_offset)));
        TextureType texid;

        switch (i) {
            case 0: {
                texid = TX_BEATMAP_ENTRY_PIC_0;
                break;
            }
            case 1: {
                texid = TX_BEATMAP_ENTRY_PIC_1;
                break;
            }
            case 2: {
                texid = TX_BEATMAP_ENTRY_PIC_2;
                break;
            }
            case 3: {
                texid = TX_BEATMAP_ENTRY_PIC_3;
                break;
            }
            default: {
                texid = TX_BEATMAP_ENTRY_PIC_0;
                fprintf(stderr, "[FATAL]: EntriesPerPage settings is higher than the amount of available texture buffers.\n");
                break;
            }
        }

        OOInt extra_y_offset = 0;
        if (skipFirstEntry) {
            // if we hid the first entry, shift everything 94 pixels up
            extra_y_offset = -94;
        }
        if (shouldHandleEntryExpand && mExpandEntryIndex < (i+beatmap_list_offset)) {
            // if expanded entry was before this one, add extra Y offset
            extra_y_offset += 94;
        }

        BeatmapEntry map = BeatmapManager::Beatmaps()[i+beatmap_list_offset];
        Highscore score;
        Scores::get_highscore(map.Checksum, score);
        TextureType score_tex;

        if (strcmp(score.grade, "UN") == 0) {
            score_tex = TX_RANKING_UNK_SM;
        } else if (strcmp(score.grade, "SS") == 0) {
            score_tex = TX_RANKING_SS_SM;
        } else if (strcmp(score.grade, "S") == 0) {
            score_tex = TX_RANKING_S_SM;
        } else if (strcmp(score.grade, "A") == 0) {
            score_tex = TX_RANKING_A_SM;
        } else if (strcmp(score.grade, "B") == 0) {
            score_tex = TX_RANKING_B_SM;
        } else if (strcmp(score.grade, "C") == 0) {
            score_tex = TX_RANKING_C_SM;
        } else if (strcmp(score.grade, "D") == 0) {
            score_tex = TX_RANKING_D_SM;
        }

        GraphicsManager::Graphics().LoadBeatmapPicTexture(texid, PreviewBuffer::GetInstance().GetTexture(i+beatmap_list_offset));

        if (isExpanded) {
            auto* bg = new pSprite(TX_BEATMAP_ENTRY_EXPANDED_BG, 351, 91 + ((OOInt)i * 94) + extra_y_offset, 609, 174, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.03f);

            bg->OnClick = OnBetmapEntryExpandClick;
            bg->Clickable = true;
            bg->TagInt = (OOInt)(i+beatmap_list_offset);

            mSpriteManager->Add(bg);
        } else {
            auto* bg = new pSprite(TX_BEATMAP_ENTRY_BG, 351, 91 + ((OOInt)i * 94) + extra_y_offset, 609, 80, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.03f);

            bg->OnClick = OnBetmapEntryExpandClick;
            bg->Clickable = true;
            bg->TagInt = (OOInt)(i+beatmap_list_offset);

            mSpriteManager->Add(bg);
        }

        if (isExpanded) {
            auto* play = new pSprite(TX_BUTTON_PLAY, 831, 200 + ((OOInt)i * 94) + extra_y_offset, 78, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.04f);

            play->OnClick = OnBetmapEntryPlayClick;
            play->Clickable = true;
            play->Tag = map.Checksum;
            play->ExtendedClickableArea = true;

            mSpriteManager->Add(play);
        }

        if (isExpanded) {
            auto* pic = new pSprite(texid, 360, 118 + ((OOInt)i * 94) + extra_y_offset, 120, 120, ORIGIN_TOPLEFT, FIELD_SCREEN,
                                    SDL_Color(), 255, -0.04f);
            mSpriteManager->Add(pic);
        } else {
            auto* pic = new pSprite(texid, 400, 91 + ((OOInt)i * 94) + extra_y_offset, 80, 80, ORIGIN_TOPLEFT, FIELD_SCREEN,
                                    SDL_Color(), 255, -0.04f);
            mSpriteManager->Add(pic);
        }

        SDL_Color font_color = Skins::get_options().FontColor_BeatmapDescription;

        if (isExpanded) {
            auto* mapTitle = new pText(map.Title, FONT_PRIMARY_SMALLER_BOLD, 494, 117 + ((OOInt)i * 94) + extra_y_offset, font_color);
            mapTitle->Z = -0.05f;

            auto* mapArtist = new pText(map.Artist, FONT_PRIMARY_SMALL, 494, 137 + ((OOInt)i * 94) + extra_y_offset, font_color);
            mapArtist->Z = -0.05f;

            auto* mapVersion = new pText(map.Version, FONT_PRIMARY_SMALL, 494, 154 + ((OOInt)i * 94) + extra_y_offset, font_color);
            mapVersion->Z = -0.05f;

            auto* highscoreTitle = new pText(I18n::get("personal_best"), FONT_PRIMARY_SMALLER_BOLD, 494, 184 + ((OOInt)i * 94) + extra_y_offset, font_color);
            highscoreTitle->Z = -0.06f;
            auto* highscoreScoreTitle = new pText(I18n::get("score"), FONT_PRIMARY_SMALL_BOLD, 494, 204 + ((OOInt)i * 94) + extra_y_offset, font_color);
            highscoreScoreTitle->Z = -0.06f;

            char highscoreScore_text[32];
            sprintf(highscoreScore_text, "%i (%ix)", score.score, score.combo);
            auto* highscoreScore = new pText(highscoreScore_text, FONT_PRIMARY_SMALL, 544, 204 + ((OOInt)i * 94) + extra_y_offset, font_color);
            highscoreScore->Z = -0.06f;
            auto* highscoreAccuracyTitle = new pText(I18n::get("accuracy"), FONT_PRIMARY_SMALL_BOLD, 494, 222 + ((OOInt)i * 94) + extra_y_offset, font_color);
            highscoreAccuracyTitle->Z = -0.06f;
            char highscoreAccuracy_text[32];
            sprintf(highscoreAccuracy_text, "%.3f%%", score.accuracy);
            auto* highscoreAccuracy = new pText(highscoreAccuracy_text, FONT_PRIMARY_SMALL, 570, 222 + ((OOInt)i * 94) + extra_y_offset, font_color);
            highscoreAccuracy->Z = -0.06f;

            mSpriteManager->Add(mapTitle);
            mSpriteManager->Add(mapArtist);
            mSpriteManager->Add(mapVersion);
            mSpriteManager->Add(highscoreTitle);
            mSpriteManager->Add(highscoreScoreTitle);
            mSpriteManager->Add(highscoreScore);
            mSpriteManager->Add(highscoreAccuracyTitle);
            mSpriteManager->Add(highscoreAccuracy);
        } else {
            auto* mapTitle = new pText(map.Title, FONT_PRIMARY_SMALLER_BOLD, 494, 106 + ((OOInt)i * 94) + extra_y_offset, font_color);
            mapTitle->Z = -0.05f;

            auto* mapArtist = new pText(map.Artist, FONT_PRIMARY_SMALL, 494, 126 + ((OOInt)i * 94) + extra_y_offset, font_color);
            mapArtist->Z = -0.05f;

            auto* mapVersion = new pText(map.Version, FONT_PRIMARY_SMALL, 494, 143 + ((OOInt)i * 94) + extra_y_offset, font_color);
            mapVersion->Z = -0.05f;

            mSpriteManager->Add(mapTitle);
            mSpriteManager->Add(mapArtist);
            mSpriteManager->Add(mapVersion);
        }

        if (isExpanded) {
            auto* ranking = new pSprite(score_tex, 869, 122 + ((OOInt)i * 94) + extra_y_offset, 40, 23,
                                        ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.04f);
            mSpriteManager->Add(ranking);

            auto* stars = new pSprite(TX_STARS, 712, 161 + ((OOInt)i * 94) + extra_y_offset, 196, 16, ORIGIN_TOPLEFT,
                                      FIELD_SCREEN, SDL_Color(), 255, -0.04f);
            auto* starsFill = new pSprite(TX_STARS_FILL, 712, 161 + ((OOInt)i * 94) + extra_y_offset, 196, 16, ORIGIN_TOPLEFT,
                                      FIELD_SCREEN, SDL_Color(), 255, -0.05f);

            delete starsFill->UV;
            starsFill->UV = new SDL_Rect({0,0,(int)floor(196.f*map.starRating/10.f),16});

            char stars_text[32];
            sprintf(stars_text, "%.2f", map.starRating);
            auto* starsText = new pText(stars_text, FONT_PRIMARY_SMALL, 707, 161 + ((OOInt)i * 94) + extra_y_offset, font_color);
            starsText->Z = -0.06f;
            starsText->Origin = ORIGIN_TOPRIGHT;

            mSpriteManager->Add(stars);
            mSpriteManager->Add(starsFill);
            mSpriteManager->Add(starsText);
        } else {
            auto* ranking = new pSprite(score_tex, 869, 104 + ((OOInt)i * 94) + extra_y_offset, 40, 23,
                                        ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.04f);
            mSpriteManager->Add(ranking);

            auto* stars = new pSprite(TX_STARS, 712, 141 + ((OOInt)i * 94) + extra_y_offset, 196, 16, ORIGIN_TOPLEFT,
                                      FIELD_SCREEN, SDL_Color(), 255, -0.04f);
            auto* starsFill = new pSprite(TX_STARS_FILL, 712, 141 + ((OOInt)i * 94) + extra_y_offset, 196, 16, ORIGIN_TOPLEFT,
                                          FIELD_SCREEN, SDL_Color(), 255, -0.05f);
            delete starsFill->UV;
            starsFill->UV = new SDL_Rect({0,0,(int)floor(196.f*map.starRating/10.f),16});
            mSpriteManager->Add(stars);
            mSpriteManager->Add(starsFill);
        }

        mEntriesDisplayed++;
    }

    auto* current_page_label = new pText(std::to_string(mCurrentPage+1)+"/"+std::to_string(mCountPages), FONT_PRIMARY, 811, 498, Skins::get_options().FontColor_Pagination);
    current_page_label->Z = -0.02f;
    current_page_label->Origin = ORIGIN_CENTER;
    mSpriteManager->Add(current_page_label);

    if (shouldHandleEntryExpand) mEntryExpanded = true;
}

void SongSelect::reloadPreviews() const {
    OOUInt beatmap_list_offset = mCurrentPage * mEntriesPerPage;
    OOUInt entries_on_page = mEntriesPerPage;
    if (beatmap_list_offset + (entries_on_page-1) >= mSongListSize) {
        entries_on_page = mSongListSize - (beatmap_list_offset);
    }

    for (int i = 0; i < entries_on_page; i++) {
        TextureType texid;

        switch (i) {
            case 0: {
                texid = TX_BEATMAP_ENTRY_PIC_0;
                break;
            }
            case 1: {
                texid = TX_BEATMAP_ENTRY_PIC_1;
                break;
            }
            case 2: {
                texid = TX_BEATMAP_ENTRY_PIC_2;
                break;
            }
            case 3: {
                texid = TX_BEATMAP_ENTRY_PIC_3;
                break;
            }
            default: {
                fprintf(stderr, "[FATAL]: EntriesPerPage settings is higher than the amount of available texture buffers.\n");
                continue;
            }
        }

        GraphicsManager::Graphics().LoadBeatmapPicTexture(texid, PreviewBuffer::GetInstance().GetTexture(i+beatmap_list_offset));
    }
}

void SongSelect::HandleInput() {}

void SongSelect::Update()
{
    if (shouldHandlePageUpdate) {
        UpdateSonglist();
        shouldHandlePageUpdate = false;
        shouldHandleEntryExpand = false;
    }

    reloadPreviews();
    mSpriteManager->Draw();
    mLogo->Update();

    if (shouldExpandRandomEntry) {
        OOUInt countEntriesOnPage = (mCurrentPage < (mCountPages-1)) ? mEntriesPerPage : (mSongListSize - (mEntriesPerPage * (mCountPages - 1)));
        ExpandEntry(MathHelper::Random(mCurrentPage*mEntriesPerPage,(mCurrentPage*mEntriesPerPage)+countEntriesOnPage-1));
        shouldExpandRandomEntry = false;
    }
}

void SongSelect::LoadingScreenShow() {
    loadingScreenBG->Alpha = 255;
    loadingScreenLabel->Alpha = 255;
    Redraw();
}

void SongSelect::Redraw() {
    GraphicsManager::Clear();
    mSpriteManager->Draw(true);
    GraphicsManager::Present();
}
