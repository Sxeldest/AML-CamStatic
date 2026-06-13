#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>

MYMODCFG(net.dexsocy.mymod.guid, AML Mod Template, 1.0, Dexsociety)

uintptr_t pGameLibrary = 0;
ConfigEntry* pCfgMyBestEntry;

extern "C" void OnModLoad()
{
    logger->SetTag("Mod Template");

    pGameLibrary = aml->GetLib("libGTASA.so");
    if(pGameLibrary)
    {
        logger->Info("MyGame mod is loaded!");
    }
    else
    {
        logger->Error("MyGame mod is not loaded :(");
        return; // Do not load our mod?
    }

    pCfgMyBestEntry = cfg->Bind("mySetting", "DefaultValue is 0?", "MyUniqueSection");
    pCfgMyBestEntry->SetString("DefaultValue is unchanged");
    pCfgMyBestEntry->SetInt(1);
    pCfgMyBestEntry->Reset();
    delete pCfgMyBestEntry; // Clean-up memory

    bool bEnabled = cfg->Bind("Enable", true)->GetBool();
    delete Config::pLastEntry; // Clean-up of the latest ConfigEntry*

    cfg->Save(); // Will only save if something was changed
}