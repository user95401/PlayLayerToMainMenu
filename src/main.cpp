#include "mod_utils.hpp"
#include "hooks.hpp"
using namespace cocos2d::extension;
using namespace cocos2d;
using namespace gd;
#include "SimpleIni.h"
CSimpleIni cfg;
bool forcecursorgv;
bool MusicStarted;
MenuGameLayer* (__thiscall* MenuGameLayer_create)();//0x18e6d0
CCLayer* __fastcall MenuGameLayer_create_H() {
    MusicStarted = false;
    auto level = GameLevelManager::sharedState()->getMainLevel(rand() % std::stoi(cfg.GetValue("PlayLayerToMainMenu", "levelsCount")), false);
    level->m_sLevelName = "isPreviewThing6380";
    return PlayLayer::create(level);
    //MenuGameLayer_create();
}
PlayLayer* (__thiscall* PlayLayer_create)(GJGameLevel*);//0x1fb6d0
PlayLayer* __fastcall PlayLayer_create_H(GJGameLevel* level) {
    forcecursorgv = (level->m_sLevelName == std::string("isPreviewThing6380"));
    if (forcecursorgv) GameManager::sharedState()->setGameVariable("0024", true);
    else GameManager::sharedState()->setGameVariable("0024", GameManager::sharedState()->getGameVariable("56738926750"));
    //CCMessageBox(std::to_string(forcecursorgv).c_str(), "forcecursorgv");
    //CCMessageBox(std::to_string(GameManager::sharedState()->getGameVariable("0024")).c_str(), "0024");
    //CCMessageBox(std::to_string(GameManager::sharedState()->getGameVariable("56738926750")).c_str(), "56738926750");
    PlayLayer* _PlayLayer = PlayLayer_create(level);
    return _PlayLayer;
}
inline void(__thiscall* PlayLayer_levelComplete)(PlayLayer*);//0x1fd3d0
void __fastcall PlayLayer_levelComplete_H(PlayLayer* self) {
    if (self->m_level->m_sLevelName == "isPreviewThing6380") return self->resetLevel();
    PlayLayer_levelComplete(self);
}
inline void(__thiscall* PlayLayer_startMusic)(PlayLayer*);//0x20C8F0
void __fastcall PlayLayer_startMusic_H(PlayLayer* self) {
    if (self->m_level->m_sLevelName == "isPreviewThing6380" && !cfg.GetBoolValue("PlayLayerToMainMenu", "playLevelSong"))
        if (MusicStarted) return;
        else {
            MusicStarted = true;
            return GameManager::sharedState()->fadeInMusic("menuLoop.mp3");
        }
    MusicStarted = false;
    PlayLayer_startMusic(self);
}
inline void(__thiscall* PlayLayer_updateProgressbar)(PlayLayer*);//0x208020
void __fastcall PlayLayer_updateProgressbar_H(PlayLayer* self) {
    PlayLayer_updateProgressbar(self);
    if (self->m_level->m_sLevelName == "isPreviewThing6380") {
        if (geodeInstalled) {
            cfg.LoadFile("geode/config/PlayLayerToMainMenu.ini");
        }
        else {
            std::filesystem::create_directories("config");
            cfg.LoadFile("config/PlayLayerToMainMenu.ini");
        }
        self->m_uiLayer->setPosition({ 9999, 9999 });
        self->m_isTestMode = true;
        self->m_isPlaybackMode = true;
        //self->m_percentLabel->setString(to_string(self->m_pPlayer1->m_position.y).c_str());
        if (self->m_pPlayer1->m_position.y > 2700.f) self->m_pPlayer1->flipGravity(false, false);
        if (cfg.GetBoolValue("PlayLayerToMainMenu", "hidePlayer1")) self->m_pPlayer1->m_isHidden = true;
    }
}
inline void(__thiscall* MoreOptionsLayer_addToggle)(MoreOptionsLayer*, const char*, const char*, const char*);
void __fastcall MoreOptionsLayer_addToggle_H(MoreOptionsLayer* self, void*, const char* name, const char* key, const char* info) {
    if (std::string(key) == "0024") {//showCursor In-Game
        key = "56738926750";
    }
    MoreOptionsLayer_addToggle(self, name, key, info);
}
inline CCLabelBMFont* (__cdecl* CCLabelBMFont_create)(const char*, const char*);
CCLabelBMFont* CCLabelBMFont_create_H(const char* str, const char* fntFile) {
    if (std::string(str) == "Settings for PlayLayerToMainMenu") ShellExecute(NULL, ("open"), (CCFileUtils::sharedFileUtils()->fullPathForFilename("geode/config/PlayLayerToMainMenu.ini", 0).c_str()), NULL, NULL, 1);
    return CCLabelBMFont_create(str, fntFile);
}
DWORD WINAPI thread_func(void* hModule) {
    MH_SafeInitialize();
    HOOK(base + 0x18e6d0, MenuGameLayer_create);
    HOOK(base + 0x1fb6d0, PlayLayer_create);
    HOOK(base + 0x20C8F0, PlayLayer_startMusic);
    HOOK(base + 0x1fd3d0, PlayLayer_levelComplete);
    HOOK(base + 0x208020, PlayLayer_updateProgressbar);
    HOOK(base + 0x1DF6B0, MoreOptionsLayer_addToggle);
    CC_HOOK("?create@CCLabelBMFont@cocos2d@@SAPAV12@PBD0@Z", CCLabelBMFont_create);
    MH_EnableHook(MH_ALL_HOOKS);

    if (!bool(CCFileUtils::sharedFileUtils()->isFileExist("geode/config/PlayLayerToMainMenu.ini") || CCFileUtils::sharedFileUtils()->isFileExist("config/PlayLayerToMainMenu.ini")))
        if (geodeInstalled) CCMessageBox("Config created at geode/config/PlayLayerToMainMenu.ini", "PlayLayerToMainMenu");
        else             CCMessageBox("Config created at config/PlayLayerToMainMenu.ini", "PlayLayerToMainMenu");

    if (geodeInstalled) {
        cfg.LoadFile("geode/config/PlayLayerToMainMenu.ini");
    }
    else {
        std::filesystem::create_directories("config");
        cfg.LoadFile("config/PlayLayerToMainMenu.ini");
    }

    if (!(cfg.KeyExists("PlayLayerToMainMenu", "levelsCount")))
        cfg.SetValue("PlayLayerToMainMenu", "levelsCount", "22", "; range from 0 to the maximum number of levels to be loaded");
    if (!(cfg.KeyExists("PlayLayerToMainMenu", "playLevelSong")))
        cfg.SetBoolValue("PlayLayerToMainMenu", "playLevelSong", false, "; play level song instead menuLoop");
    if (!(cfg.KeyExists("PlayLayerToMainMenu", "hidePlayer1")))
        cfg.SetBoolValue("PlayLayerToMainMenu", "hidePlayer1", false, "; hide player really");

    SI_Error err;
    if (geodeInstalled) err = cfg.SaveFile("geode/config/PlayLayerToMainMenu.ini");
    else err = cfg.SaveFile("config/PlayLayerToMainMenu.ini");

    return 0;
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, thread_func, hModule, 0, 0);
    return TRUE;
}
