#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <cmath>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "shell32.lib")

#include "Nexus.h"
#include "imgui/imgui.h"
#include "RotationTypes.h"
#include "KeybindDetector.h"
#include "RotationEngine.h"

static AddonAPI_t* globalNexusAddonInterface = nullptr;
static KeybindDetector globalKeybindDetector;
static RotationEngine globalRotationEngine;
static std::chrono::steady_clock::time_point globalLastFrameTimestamp;

static bool globalShowMasterControlPanel = false;
static bool globalShowRibbonHud = true;
static bool globalShowActionBarHighlighter = false;
static bool globalShowAllActionBarSlotHighlights = false;
static bool globalShowBuildInspectorWindow = false;
static bool globalShowPacingMetricsWindow = false;
static bool globalShowMistakeHistoryWindow = false;

static float globalWeaponBarHorizontalOffset = 6.0f;
static float globalWeaponBarVerticalOffset = -21.0f;
static float globalUtilityBarHorizontalOffset = 3.0f;
static float globalUtilityBarVerticalOffset = -21.0f;
static float globalProfessionBarHorizontalOffset = 0.0f;
static float globalProfessionBarVerticalOffset = 0.0f;

static bool globalAutoDetectLivePing = true;
static double globalLiveSmoothedPingMilliseconds = 50.0;
static std::chrono::steady_clock::time_point globalLastPingSampleTime;
static float globalActionBarSlotSize = 48.0f;
static float globalActionBarSlotSpacing = 7.0f;
static float globalProfessionSlotSize = 37.0f;
static float globalProfessionSlotSpacing = 1.0f;

struct ProfessionBarAlignmentProfile
{
    float horizontalOffset = 0.0f;
    float verticalOffset = 0.0f;
    float slotBoxDimension = 37.0f;
    float slotSpacingDimension = 1.0f;
};

static std::map<std::string, ProfessionBarAlignmentProfile> globalProfessionAlignmentProfiles;

static std::string ResolveProfessionNameFromIdentifier(uint32_t professionIdentifier)
{
    if (professionIdentifier == 1)
    {
        return "Guardian";
    }

    if (professionIdentifier == 2)
    {
        return "Warrior";
    }

    if (professionIdentifier == 3)
    {
        return "Engineer";
    }

    if (professionIdentifier == 4)
    {
        return "Ranger";
    }

    if (professionIdentifier == 5)
    {
        return "Thief";
    }

    if (professionIdentifier == 6)
    {
        return "Elementalist";
    }

    if (professionIdentifier == 7)
    {
        return "Mesmer";
    }

    if (professionIdentifier == 8)
    {
        return "Necromancer";
    }

    if (professionIdentifier == 9)
    {
        return "Revenant";
    }

    return "Default";
}

static std::string GetActiveProfessionProfileKey()
{
    const BuildProfile* activeProfile = globalRotationEngine.GetActiveProfilePointer();
    if (activeProfile != nullptr)
    {
        if (!activeProfile->specializationName.empty())
        {
            return activeProfile->specializationName;
        }

        if (!activeProfile->professionName.empty())
        {
            return activeProfile->professionName;
        }
    }

    uint32_t specializationIdentifier = globalRotationEngine.GetDetectedSpecializationNumber();
    if (specializationIdentifier == 62)
    {
        return "Firebrand";
    }

    if (specializationIdentifier == 66)
    {
        return "Virtuoso";
    }

    if (specializationIdentifier == 40)
    {
        return "Chronomancer";
    }

    if (specializationIdentifier == 67)
    {
        return "Catalyst";
    }

    if (specializationIdentifier == 60)
    {
        return "Scourge";
    }

    if (specializationIdentifier == 70)
    {
        return "Mechanist";
    }

    uint32_t professionIdentifier = globalRotationEngine.GetDetectedProfessionNumber();
    if (professionIdentifier != 0)
    {
        return ResolveProfessionNameFromIdentifier(professionIdentifier);
    }

    return "Default";
}

static ProfessionBarAlignmentProfile& GetProfessionAlignmentProfile(const std::string& profileKey)
{
    std::string effectiveKey = profileKey.empty() ? "Default" : profileKey;
    auto existingIterator = globalProfessionAlignmentProfiles.find(effectiveKey);
    if (existingIterator != globalProfessionAlignmentProfiles.end())
    {
        return existingIterator->second;
    }

    ProfessionBarAlignmentProfile defaultProfile;
    defaultProfile.horizontalOffset = globalProfessionBarHorizontalOffset;
    defaultProfile.verticalOffset = globalProfessionBarVerticalOffset;
    defaultProfile.slotBoxDimension = globalProfessionSlotSize;
    defaultProfile.slotSpacingDimension = globalProfessionSlotSpacing;

    globalProfessionAlignmentProfiles[effectiveKey] = defaultProfile;
    return globalProfessionAlignmentProfiles[effectiveKey];
}

static void UpdateLiveNetworkPing()
{
    if (!globalAutoDetectLivePing)
    {
        return;
    }

    auto currentTimePoint = std::chrono::steady_clock::now();
    auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTimePoint - globalLastPingSampleTime).count();
    if (elapsedMilliseconds < 2500)
    {
        return;
    }

    globalLastPingSampleTime = currentTimePoint;

    DWORD tableSize = 0;
    GetExtendedTcpTable(nullptr, &tableSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
    if (tableSize == 0)
    {
        return;
    }

    std::vector<uint8_t> buffer(tableSize);
    if (GetExtendedTcpTable(buffer.data(), &tableSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) != NO_ERROR)
    {
        return;
    }

    const MIB_TCPTABLE_OWNER_PID* tcpTable = reinterpret_cast<const MIB_TCPTABLE_OWNER_PID*>(buffer.data());
    DWORD currentProcessIdentifier = GetCurrentProcessId();
    uint32_t targetServerAddress = 0;
    uint16_t targetServerPort = 0;

    for (DWORD entryIndex = 0; entryIndex < tcpTable->dwNumEntries; entryIndex = entryIndex + 1)
    {
        const MIB_TCPROW_OWNER_PID& tcpRow = tcpTable->table[entryIndex];
        if (tcpRow.dwOwningPid == currentProcessIdentifier && tcpRow.dwState == MIB_TCP_STATE_ESTAB)
        {
            uint16_t remotePort = ntohs(static_cast<u_short>(tcpRow.dwRemotePort));
            if (remotePort == 6112)
            {
                targetServerAddress = tcpRow.dwRemoteAddr;
                targetServerPort = static_cast<uint16_t>(tcpRow.dwRemotePort);
                break;
            }
        }
    }

    if (targetServerAddress == 0)
    {
        return;
    }

    SOCKET pingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (pingSocket == INVALID_SOCKET)
    {
        return;
    }

    u_long nonBlockingMode = 1;
    ioctlsocket(pingSocket, FIONBIO, &nonBlockingMode);

    sockaddr_in serverSocketAddress;
    memset(&serverSocketAddress, 0, sizeof(serverSocketAddress));
    serverSocketAddress.sin_family = AF_INET;
    serverSocketAddress.sin_addr.s_addr = targetServerAddress;
    serverSocketAddress.sin_port = targetServerPort;

    auto connectStartTimePoint = std::chrono::steady_clock::now();
    connect(pingSocket, reinterpret_cast<sockaddr*>(&serverSocketAddress), sizeof(serverSocketAddress));

    fd_set writeSet;
    FD_ZERO(&writeSet);
    FD_SET(pingSocket, &writeSet);

    timeval timeoutStructure;
    timeoutStructure.tv_sec = 0;
    timeoutStructure.tv_usec = 800000;

    int selectResult = select(0, nullptr, &writeSet, nullptr, &timeoutStructure);
    if (selectResult > 0)
    {
        auto connectEndTimePoint = std::chrono::steady_clock::now();
        double measuredPingMilliseconds = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(connectEndTimePoint - connectStartTimePoint).count()) / 1000.0;

        if (measuredPingMilliseconds > 0.0 && measuredPingMilliseconds < 1000.0)
        {
            if (globalLiveSmoothedPingMilliseconds <= 0.0)
            {
                globalLiveSmoothedPingMilliseconds = measuredPingMilliseconds;
            }
            else
            {
                globalLiveSmoothedPingMilliseconds = (globalLiveSmoothedPingMilliseconds * 0.6) + (measuredPingMilliseconds * 0.4);
            }

            globalRotationEngine.SetNetworkLatencyMilliseconds(globalLiveSmoothedPingMilliseconds);
        }
    }

    closesocket(pingSocket);
}

static float globalRibbonHudHorizontalOffset = 0.0f;
static float globalRibbonHudVerticalOffset = 0.0f;
static bool globalResetRibbonHudPositionRequested = false;

static std::string globalSlotWaitingForBinding = "";
static char globalBuildSearchBuffer[128] = "";

typedef void* (*MemoryAllocationFunction)(size_t allocationSize, void* userData);
typedef void (*MemoryDeallocationFunction)(void* memoryPointer, void* userData);

static void AdvanceFrameTimer()
{
    auto currentFrameTimestamp = std::chrono::steady_clock::now();
    auto elapsedNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(currentFrameTimestamp - globalLastFrameTimestamp).count();
    float deltaSeconds = static_cast<float>(elapsedNanoseconds) / 1000000000.0f;

    globalLastFrameTimestamp = currentFrameTimestamp;

    if (deltaSeconds > 0.0f && deltaSeconds < 1.0f)
    {
        globalRotationEngine.UpdatePerFrame(deltaSeconds);
    }
}

static Texture_t* RetrieveSkillTexture(uint32_t skillIdentifier, SkillSlotCategory slotCategory, const std::string& skillName = "")
{
    if (globalNexusAddonInterface == nullptr)
    {
        return nullptr;
    }

    const char* gameDirectory = globalNexusAddonInterface->Paths_GetGameDirectory();
    if (gameDirectory == nullptr)
    {
        return nullptr;
    }

    std::string imageDirectory = std::string(gameDirectory) + "\\addons\\Gw2RotationCoach\\img\\";

    if (skillIdentifier == 9999 || slotCategory == SkillSlotCategory::WeaponSwap)
    {
        std::string swapIdentifier = "GW2_SKILL_9999";
        Texture_t* existingSwapTexture = globalNexusAddonInterface->Textures_Get(swapIdentifier.c_str());
        if (existingSwapTexture != nullptr)
        {
            return existingSwapTexture;
        }

        std::string swapPath = imageDirectory + "9999.png";
        return globalNexusAddonInterface->Textures_GetOrCreateFromFile(swapIdentifier.c_str(), swapPath.c_str());
    }

    if (skillIdentifier == 60975 || skillName.find("Stow Tome") != std::string::npos)
    {
        std::string stowIdentifier = "GW2_SKILL_60975";
        Texture_t* existingStowTexture = globalNexusAddonInterface->Textures_Get(stowIdentifier.c_str());
        if (existingStowTexture != nullptr)
        {
            return existingStowTexture;
        }

        std::string stowPath = imageDirectory + "60975.png";
        return globalNexusAddonInterface->Textures_GetOrCreateFromFile(stowIdentifier.c_str(), stowPath.c_str());
    }

    if (skillIdentifier > 0)
    {
        std::string textureIdentifier = "GW2_SKILL_" + std::to_string(skillIdentifier);
        Texture_t* existingTexture = globalNexusAddonInterface->Textures_Get(textureIdentifier.c_str());

        if (existingTexture != nullptr)
        {
            return existingTexture;
        }

        std::string mappedIconFileName = globalRotationEngine.GetCatalogReference().GetSkillIconFileName(skillIdentifier);
        if (!mappedIconFileName.empty())
        {
            std::string filePath = imageDirectory + mappedIconFileName;
            Texture_t* mappedTexture = globalNexusAddonInterface->Textures_GetOrCreateFromFile(textureIdentifier.c_str(), filePath.c_str());
            if (mappedTexture != nullptr)
            {
                return mappedTexture;
            }
        }

        std::string imageFilePath = imageDirectory + std::to_string(skillIdentifier) + ".png";
        Texture_t* loadedTexture = globalNexusAddonInterface->Textures_GetOrCreateFromFile(textureIdentifier.c_str(), imageFilePath.c_str());

        if (loadedTexture != nullptr)
        {
            return loadedTexture;
        }
    }

    if (!skillName.empty())
    {
        std::string nameTextureIdentifier = "GW2_SKILLNAME_" + skillName;
        Texture_t* existingNameTexture = globalNexusAddonInterface->Textures_Get(nameTextureIdentifier.c_str());

        if (existingNameTexture != nullptr)
        {
            return existingNameTexture;
        }

        std::string mappedByName = globalRotationEngine.GetCatalogReference().FindSkillIconByName(skillName);
        if (!mappedByName.empty())
        {
            std::string nameFilePath = imageDirectory + mappedByName;
            Texture_t* nameTexture = globalNexusAddonInterface->Textures_GetOrCreateFromFile(nameTextureIdentifier.c_str(), nameFilePath.c_str());
            if (nameTexture != nullptr)
            {
                return nameTexture;
            }
        }
    }

    std::string slotFileName = "";

    if (slotCategory == SkillSlotCategory::WeaponSkillOne)
    {
        slotFileName = "1.png";
    }
    else if (slotCategory == SkillSlotCategory::WeaponSkillTwo)
    {
        slotFileName = "2.png";
    }
    else if (slotCategory == SkillSlotCategory::WeaponSkillThree)
    {
        slotFileName = "3.png";
    }
    else if (slotCategory == SkillSlotCategory::WeaponSkillFour)
    {
        slotFileName = "4.png";
    }
    else if (slotCategory == SkillSlotCategory::WeaponSkillFive)
    {
        slotFileName = "5.png";
    }
    else if (slotCategory == SkillSlotCategory::HealingSkill)
    {
        slotFileName = "6.png";
    }
    else if (slotCategory == SkillSlotCategory::UtilitySkillOne)
    {
        slotFileName = "7.png";
    }
    else if (slotCategory == SkillSlotCategory::UtilitySkillTwo)
    {
        slotFileName = "8.png";
    }
    else if (slotCategory == SkillSlotCategory::UtilitySkillThree)
    {
        slotFileName = "9.png";
    }
    else if (slotCategory == SkillSlotCategory::EliteSkill)
    {
        slotFileName = "0.png";
    }
    else if (slotCategory == SkillSlotCategory::WeaponSwap)
    {
        slotFileName = "9999.png";
    }
    else if (slotCategory == SkillSlotCategory::DodgeAction)
    {
        slotFileName = "Dodge.png";
    }

    if (!slotFileName.empty())
    {
        std::string slotTextureIdentifier = "GW2_SLOT_ICON_" + slotFileName;
        Texture_t* slotTexture = globalNexusAddonInterface->Textures_Get(slotTextureIdentifier.c_str());

        if (slotTexture != nullptr)
        {
            return slotTexture;
        }

        std::string slotPath = imageDirectory + slotFileName;
        return globalNexusAddonInterface->Textures_GetOrCreateFromFile(slotTextureIdentifier.c_str(), slotPath.c_str());
    }

    return nullptr;
}

static std::string FormatDisplayPrompt(const RotationStep& rotationStep)
{
    if (rotationStep.slotCategory == SkillSlotCategory::WeaponSwap)
    {
        return "[`] " + rotationStep.skillName;
    }

    SkillSlotCategory effectiveSlot = globalRotationEngine.GetEffectiveSlotCategory(rotationStep);
    std::string slotName = globalKeybindDetector.GetSlotNameFromCategory(effectiveSlot);
    std::string boundKeyText = globalKeybindDetector.GetFormattedKeybindText(slotName, rotationStep.defaultKeybindName);

    return "[" + boundKeyText + "] " + rotationStep.skillName;
}

static std::string FormatKeyOnlyPrompt(const RotationStep& rotationStep)
{
    if (rotationStep.slotCategory == SkillSlotCategory::WeaponSwap)
    {
        return globalKeybindDetector.GetFormattedKeybindText("WeaponSwap", "`");
    }

    SkillSlotCategory effectiveSlot = globalRotationEngine.GetEffectiveSlotCategory(rotationStep);
    std::string slotName = globalKeybindDetector.GetSlotNameFromCategory(effectiveSlot);
    return globalKeybindDetector.GetFormattedKeybindText(slotName, rotationStep.defaultKeybindName);
}

struct ActionSlotBox
{
    float coordinateX;
    float coordinateY;
    float boxWidth;
    float boxHeight;
    std::string slotLabel;
    SkillSlotCategory category;
};

static std::vector<ActionSlotBox> CalculateAllActionSlotBoxes(float displayWidth, float displayHeight)
{
    float slotBoxWidth = globalActionBarSlotSize;
    float slotBoxHeight = globalActionBarSlotSize;
    float slotSpacing = globalActionBarSlotSpacing;

    float weaponBaseY = (displayHeight - 52.0f) + globalWeaponBarVerticalOffset;
    float utilityBaseY = (displayHeight - 52.0f) + globalUtilityBarVerticalOffset;

    float weaponCenterX = (displayWidth * 0.5f) + globalWeaponBarHorizontalOffset;
    float utilityCenterX = (displayWidth * 0.5f) + globalUtilityBarHorizontalOffset;

    std::vector<ActionSlotBox> resultBoxes;

    float healthGlobeRadius = slotBoxWidth * 1.15f;
    float weaponSkillFiveRightCoordinateX = weaponCenterX - healthGlobeRadius;
    float weaponSkillOneLeftCoordinateX = weaponSkillFiveRightCoordinateX - (5.0f * slotBoxWidth) - (4.0f * slotSpacing);
    float weaponSwapDimension = slotBoxWidth * 0.72f;
    float weaponSwapCoordinateX = weaponSkillOneLeftCoordinateX - weaponSwapDimension - 6.0f;
    float weaponSwapCoordinateY = weaponBaseY + ((slotBoxHeight - weaponSwapDimension) * 0.5f);

    resultBoxes.push_back({weaponSwapCoordinateX, weaponSwapCoordinateY, weaponSwapDimension, weaponSwapDimension, "`", SkillSlotCategory::WeaponSwap});
    resultBoxes.push_back({weaponSkillOneLeftCoordinateX, weaponBaseY, slotBoxWidth, slotBoxHeight, "1", SkillSlotCategory::WeaponSkillOne});
    resultBoxes.push_back({weaponSkillOneLeftCoordinateX + 1.0f * (slotBoxWidth + slotSpacing), weaponBaseY, slotBoxWidth, slotBoxHeight, "2", SkillSlotCategory::WeaponSkillTwo});
    resultBoxes.push_back({weaponSkillOneLeftCoordinateX + 2.0f * (slotBoxWidth + slotSpacing), weaponBaseY, slotBoxWidth, slotBoxHeight, "3", SkillSlotCategory::WeaponSkillThree});
    resultBoxes.push_back({weaponSkillOneLeftCoordinateX + 3.0f * (slotBoxWidth + slotSpacing), weaponBaseY, slotBoxWidth, slotBoxHeight, "4", SkillSlotCategory::WeaponSkillFour});
    resultBoxes.push_back({weaponSkillOneLeftCoordinateX + 4.0f * (slotBoxWidth + slotSpacing), weaponBaseY, slotBoxWidth, slotBoxHeight, "5", SkillSlotCategory::WeaponSkillFive});

    float utilityLeftCoordinateX = utilityCenterX + healthGlobeRadius;
    resultBoxes.push_back({utilityLeftCoordinateX, utilityBaseY, slotBoxWidth, slotBoxHeight, "6", SkillSlotCategory::HealingSkill});
    resultBoxes.push_back({utilityLeftCoordinateX + 1.0f * (slotBoxWidth + slotSpacing), utilityBaseY, slotBoxWidth, slotBoxHeight, "7", SkillSlotCategory::UtilitySkillOne});
    resultBoxes.push_back({utilityLeftCoordinateX + 2.0f * (slotBoxWidth + slotSpacing), utilityBaseY, slotBoxWidth, slotBoxHeight, "8", SkillSlotCategory::UtilitySkillTwo});
    resultBoxes.push_back({utilityLeftCoordinateX + 3.0f * (slotBoxWidth + slotSpacing), utilityBaseY, slotBoxWidth, slotBoxHeight, "9", SkillSlotCategory::UtilitySkillThree});
    resultBoxes.push_back({utilityLeftCoordinateX + 4.0f * (slotBoxWidth + slotSpacing), utilityBaseY, slotBoxWidth, slotBoxHeight, "0", SkillSlotCategory::EliteSkill});

    const BuildProfile* activeProfile = globalRotationEngine.GetActiveProfilePointer();
    std::string professionName = (activeProfile != nullptr) ? activeProfile->professionName : "";
    std::string specializationName = (activeProfile != nullptr) ? activeProfile->specializationName : "";
    uint32_t professionIdentifier = (activeProfile != nullptr) ? activeProfile->professionIdentifierNumber : globalRotationEngine.GetDetectedProfessionNumber();
    uint32_t specializationIdentifier = (activeProfile != nullptr) ? activeProfile->specializationIdentifierNumber : globalRotationEngine.GetDetectedSpecializationNumber();

    if (professionName.empty() && professionIdentifier != 0)
    {
        professionName = ResolveProfessionNameFromIdentifier(professionIdentifier);
    }

    std::string activeProfileKey = specializationName.empty() ? (professionName.empty() ? "Default" : professionName) : specializationName;
    ProfessionBarAlignmentProfile& alignmentProfile = GetProfessionAlignmentProfile(activeProfileKey);

    float professionSlotSize = alignmentProfile.slotBoxDimension;
    float professionSpacing = alignmentProfile.slotSpacingDimension;
    float standardProfessionBaseY = (displayHeight - 52.0f) - professionSlotSize - 6.0f + alignmentProfile.verticalOffset;
    float standardProfessionLeftBaseX = (displayWidth * 0.5f) - healthGlobeRadius - (5.0f * slotBoxWidth) - (4.0f * slotSpacing);

    if (specializationName == "Firebrand" || specializationIdentifier == 62)
    {
        float firebrandStartX = standardProfessionLeftBaseX + 6.0f + alignmentProfile.horizontalOffset;

        resultBoxes.push_back({firebrandStartX, standardProfessionBaseY, professionSlotSize, professionSlotSize, "F1", SkillSlotCategory::ProfessionSkillOne});
        resultBoxes.push_back({firebrandStartX + 1.0f * (professionSlotSize + professionSpacing), standardProfessionBaseY, professionSlotSize, professionSlotSize, "F2", SkillSlotCategory::ProfessionSkillTwo});
        resultBoxes.push_back({firebrandStartX + 2.0f * (professionSlotSize + professionSpacing), standardProfessionBaseY, professionSlotSize, professionSlotSize, "F3", SkillSlotCategory::ProfessionSkillThree});
    }
    else if (professionName == "Guardian" || professionIdentifier == 1)
    {
        float guardianStartX = standardProfessionLeftBaseX + (0.5f * (slotBoxWidth + slotSpacing)) + alignmentProfile.horizontalOffset;

        resultBoxes.push_back({guardianStartX, standardProfessionBaseY, professionSlotSize, professionSlotSize, "F1", SkillSlotCategory::ProfessionSkillOne});
        resultBoxes.push_back({guardianStartX + 1.0f * (professionSlotSize + professionSpacing), standardProfessionBaseY, professionSlotSize, professionSlotSize, "F2", SkillSlotCategory::ProfessionSkillTwo});
        resultBoxes.push_back({guardianStartX + 2.0f * (professionSlotSize + professionSpacing), standardProfessionBaseY, professionSlotSize, professionSlotSize, "F3", SkillSlotCategory::ProfessionSkillThree});
    }
    else if (professionName == "Warrior" || professionIdentifier == 2)
    {
        if (specializationName == "Berserker" || specializationName == "Spellbreaker" || specializationName == "Bladesworn" ||
            specializationIdentifier == 18 || specializationIdentifier == 61 || specializationIdentifier == 68)
        {
            float warriorStartX = standardProfessionLeftBaseX + (1.0f * (slotBoxWidth + slotSpacing)) + alignmentProfile.horizontalOffset;

            resultBoxes.push_back({warriorStartX, standardProfessionBaseY, professionSlotSize, professionSlotSize, "F1", SkillSlotCategory::ProfessionSkillOne});
            resultBoxes.push_back({warriorStartX + 1.0f * (professionSlotSize + professionSpacing), standardProfessionBaseY, professionSlotSize, professionSlotSize, "F2", SkillSlotCategory::ProfessionSkillTwo});
        }
        else
        {
            float warriorStartX = standardProfessionLeftBaseX + (1.5f * (slotBoxWidth + slotSpacing)) + alignmentProfile.horizontalOffset;

            resultBoxes.push_back({warriorStartX, standardProfessionBaseY, professionSlotSize, professionSlotSize, "F1", SkillSlotCategory::ProfessionSkillOne});
        }
    }
    else if (professionName == "Engineer" || professionIdentifier == 3)
    {
        if (specializationName == "Mechanist" || specializationIdentifier == 70)
        {
            float mechanistStartX = standardProfessionLeftBaseX + (0.5f * (slotBoxWidth + slotSpacing)) + alignmentProfile.horizontalOffset;

            resultBoxes.push_back({mechanistStartX, standardProfessionBaseY, professionSlotSize, professionSlotSize, "F1", SkillSlotCategory::ProfessionSkillOne});
            resultBoxes.push_back({mechanistStartX + 1.0f * (professionSlotSize + professionSpacing), standardProfessionBaseY, professionSlotSize, professionSlotSize, "F2", SkillSlotCategory::ProfessionSkillTwo});
            resultBoxes.push_back({mechanistStartX + 2.0f * (professionSlotSize + professionSpacing), standardProfessionBaseY, professionSlotSize, professionSlotSize, "F3", SkillSlotCategory::ProfessionSkillThree});
        }
        else
        {
            float engineerStartX = standardProfessionLeftBaseX + 4.0f + alignmentProfile.horizontalOffset;

            for (uint32_t slotIndex = 0; slotIndex < 5; slotIndex = slotIndex + 1)
            {
                float slotCoordinateX = engineerStartX + (static_cast<float>(slotIndex) * (professionSlotSize + professionSpacing));
                SkillSlotCategory slotCategory = static_cast<SkillSlotCategory>(static_cast<uint32_t>(SkillSlotCategory::ProfessionSkillOne) + slotIndex);
                std::string slotLabel = "F" + std::to_string(slotIndex + 1);

                resultBoxes.push_back({slotCoordinateX, standardProfessionBaseY, professionSlotSize, professionSlotSize, slotLabel, slotCategory});
            }
        }
    }
    else if (professionName == "Elementalist" || professionIdentifier == 6)
    {
        float elementalistStartX = standardProfessionLeftBaseX + 4.0f + alignmentProfile.horizontalOffset;
        SkillSlotCategory activeAttunement = globalRotationEngine.GetActiveAttunementCategory();

        for (uint32_t slotIndex = 0; slotIndex < 4; slotIndex = slotIndex + 1)
        {
            SkillSlotCategory category = static_cast<SkillSlotCategory>(static_cast<uint32_t>(SkillSlotCategory::ProfessionSkillOne) + slotIndex);
            std::string slotLabel = "F" + std::to_string(slotIndex + 1);

            float currentSlotX = elementalistStartX + (static_cast<float>(slotIndex) * (professionSlotSize + professionSpacing));
            float currentSlotY = standardProfessionBaseY;
            float currentSlotDimension = professionSlotSize;

            if (category == activeAttunement)
            {
                currentSlotDimension = professionSlotSize * 1.25f;
                currentSlotY = standardProfessionBaseY - (professionSlotSize * 0.20f);
                currentSlotX = currentSlotX - ((currentSlotDimension - professionSlotSize) * 0.5f);
            }

            resultBoxes.push_back({currentSlotX, currentSlotY, currentSlotDimension, currentSlotDimension, slotLabel, category});
        }

        if (specializationName == "Catalyst" || specializationIdentifier == 67)
        {
            float catalystEnergyGaugeGap = professionSlotSize * 0.65f;
            float catalystJadeSphereStartX = elementalistStartX + (4.0f * (professionSlotSize + professionSpacing)) + catalystEnergyGaugeGap;
            resultBoxes.push_back({catalystJadeSphereStartX, standardProfessionBaseY, professionSlotSize, professionSlotSize, "F5", SkillSlotCategory::ProfessionSkillFive});
        }
    }
    else if (professionName == "Mesmer" || professionIdentifier == 7)
    {
        uint32_t shatterCount = (specializationName == "Virtuoso" || specializationName == "Chronomancer" || specializationIdentifier == 66 || specializationIdentifier == 40) ? 5 : 4;

        if (specializationName == "Virtuoso" || specializationIdentifier == 66)
        {
            float virtuosoSlotDimension = 36.0f;
            if (alignmentProfile.slotBoxDimension > 0.0f)
            {
                virtuosoSlotDimension = alignmentProfile.slotBoxDimension;
            }

            float virtuosoSpacing = 4.0f;
            if (alignmentProfile.slotSpacingDimension > 0.0f)
            {
                virtuosoSpacing = alignmentProfile.slotSpacingDimension;
            }

            float virtuosoStartX = standardProfessionLeftBaseX + (1.0f * (slotBoxWidth + slotSpacing)) + 8.0f + alignmentProfile.horizontalOffset;
            float virtuosoBaseY = (displayHeight - 52.0f) - virtuosoSlotDimension - 6.0f + alignmentProfile.verticalOffset;

            for (uint32_t slotIndex = 0; slotIndex < 5; slotIndex = slotIndex + 1)
            {
                float slotCoordinateX = virtuosoStartX + (static_cast<float>(slotIndex) * (virtuosoSlotDimension + virtuosoSpacing));
                SkillSlotCategory slotCategory = static_cast<SkillSlotCategory>(static_cast<uint32_t>(SkillSlotCategory::ProfessionSkillOne) + slotIndex);
                std::string slotLabel = "F" + std::to_string(slotIndex + 1);

                resultBoxes.push_back({slotCoordinateX, virtuosoBaseY, virtuosoSlotDimension, virtuosoSlotDimension, slotLabel, slotCategory});
            }
        }
        else
        {
            float totalProfessionGroupWidth = (static_cast<float>(shatterCount) * professionSlotSize) + (static_cast<float>(shatterCount - 1) * professionSpacing);
            float weaponBarTotalWidth = (5.0f * slotBoxWidth) + (4.0f * slotSpacing);
            float mesmerStartX = (standardProfessionLeftBaseX + weaponBarTotalWidth) - totalProfessionGroupWidth - 12.0f + alignmentProfile.horizontalOffset;

            for (uint32_t slotIndex = 0; slotIndex < shatterCount; slotIndex = slotIndex + 1)
            {
                float slotCoordinateX = mesmerStartX + (static_cast<float>(slotIndex) * (professionSlotSize + professionSpacing));
                SkillSlotCategory slotCategory = static_cast<SkillSlotCategory>(static_cast<uint32_t>(SkillSlotCategory::ProfessionSkillOne) + slotIndex);
                std::string slotLabel = "F" + std::to_string(slotIndex + 1);

                resultBoxes.push_back({slotCoordinateX, standardProfessionBaseY, professionSlotSize, professionSlotSize, slotLabel, slotCategory});
            }
        }
    }
    else if (professionName == "Necromancer" || professionIdentifier == 8)
    {
        if (specializationName == "Scourge" || specializationIdentifier == 60)
        {
            float scourgeStartX = standardProfessionLeftBaseX + 4.0f + alignmentProfile.horizontalOffset;

            for (uint32_t slotIndex = 0; slotIndex < 5; slotIndex = slotIndex + 1)
            {
                float slotCoordinateX = scourgeStartX + (static_cast<float>(slotIndex) * (professionSlotSize + professionSpacing));
                SkillSlotCategory slotCategory = static_cast<SkillSlotCategory>(static_cast<uint32_t>(SkillSlotCategory::ProfessionSkillOne) + slotIndex);
                std::string slotLabel = "F" + std::to_string(slotIndex + 1);

                resultBoxes.push_back({slotCoordinateX, standardProfessionBaseY, professionSlotSize, professionSlotSize, slotLabel, slotCategory});
            }
        }
        else
        {
            float shroudCoordinateX = standardProfessionLeftBaseX + (1.5f * (slotBoxWidth + slotSpacing)) + alignmentProfile.horizontalOffset;

            resultBoxes.push_back({shroudCoordinateX, standardProfessionBaseY, professionSlotSize, professionSlotSize, "F1", SkillSlotCategory::ProfessionSkillOne});
        }
    }
    else if (professionName == "Ranger" || professionIdentifier == 4)
    {
        float rangerStartX = standardProfessionLeftBaseX - 4.0f + alignmentProfile.horizontalOffset;

        resultBoxes.push_back({rangerStartX, standardProfessionBaseY, professionSlotSize, professionSlotSize, "F1", SkillSlotCategory::ProfessionSkillOne});
        resultBoxes.push_back({rangerStartX + 1.0f * (professionSlotSize + professionSpacing), standardProfessionBaseY, professionSlotSize, professionSlotSize, "F2", SkillSlotCategory::ProfessionSkillTwo});
        resultBoxes.push_back({rangerStartX + 2.0f * (professionSlotSize + professionSpacing), standardProfessionBaseY, professionSlotSize, professionSlotSize, "F3", SkillSlotCategory::ProfessionSkillThree});
        resultBoxes.push_back({rangerStartX + 3.0f * (professionSlotSize + professionSpacing), standardProfessionBaseY, professionSlotSize, professionSlotSize, "F4", SkillSlotCategory::ProfessionSkillFour});

        if (specializationName != "Untamed" && specializationIdentifier != 72)
        {
            resultBoxes.push_back({rangerStartX + 4.0f * (professionSlotSize + professionSpacing), standardProfessionBaseY, professionSlotSize, professionSlotSize, "F5", SkillSlotCategory::ProfessionSkillFive});
        }
    }
    else if (professionName == "Thief" || professionIdentifier == 5)
    {
        if (specializationName == "Specter" || specializationIdentifier == 71)
        {
            float thiefStartX = standardProfessionLeftBaseX + (1.0f * (slotBoxWidth + slotSpacing)) + alignmentProfile.horizontalOffset;

            resultBoxes.push_back({thiefStartX, standardProfessionBaseY, professionSlotSize, professionSlotSize, "F1", SkillSlotCategory::ProfessionSkillOne});
            resultBoxes.push_back({thiefStartX + 1.0f * (professionSlotSize + professionSpacing), standardProfessionBaseY, professionSlotSize, professionSlotSize, "F2", SkillSlotCategory::ProfessionSkillTwo});
        }
        else
        {
            float thiefStartX = standardProfessionLeftBaseX + (1.5f * (slotBoxWidth + slotSpacing)) + alignmentProfile.horizontalOffset;

            resultBoxes.push_back({thiefStartX, standardProfessionBaseY, professionSlotSize, professionSlotSize, "F1", SkillSlotCategory::ProfessionSkillOne});
        }
    }
    else if (professionName == "Revenant" || professionIdentifier == 9)
    {
        float revenantStartX = standardProfessionLeftBaseX + (1.0f * (slotBoxWidth + slotSpacing)) + alignmentProfile.horizontalOffset;

        resultBoxes.push_back({revenantStartX, standardProfessionBaseY, professionSlotSize, professionSlotSize, "F1", SkillSlotCategory::ProfessionSkillOne});
        resultBoxes.push_back({revenantStartX + 1.0f * (professionSlotSize + professionSpacing), standardProfessionBaseY, professionSlotSize, professionSlotSize, "F2", SkillSlotCategory::ProfessionSkillTwo});
    }
    else
    {
        float defaultStartX = standardProfessionLeftBaseX + 4.0f + alignmentProfile.horizontalOffset;

        for (uint32_t slotIndex = 0; slotIndex < 5; slotIndex = slotIndex + 1)
        {
            float slotCoordinateX = defaultStartX + (static_cast<float>(slotIndex) * (professionSlotSize + professionSpacing));
            SkillSlotCategory slotCategory = static_cast<SkillSlotCategory>(static_cast<uint32_t>(SkillSlotCategory::ProfessionSkillOne) + slotIndex);
            std::string slotLabel = "F" + std::to_string(slotIndex + 1);

            resultBoxes.push_back({slotCoordinateX, standardProfessionBaseY, professionSlotSize, professionSlotSize, slotLabel, slotCategory});
        }
    }

    return resultBoxes;
}

static void RenderActionBarSlotHighlighter()
{
    ImVec2 displayDimension = ImGui::GetIO().DisplaySize;

    std::vector<ActionSlotBox> slotBoxes = CalculateAllActionSlotBoxes(displayDimension.x, displayDimension.y);

    const RotationStep* currentStep = globalRotationEngine.GetCurrentStepPointer();
    SkillSlotCategory activeCategory = currentStep != nullptr ? globalRotationEngine.GetEffectiveSlotCategory(*currentStep) : SkillSlotCategory::WeaponSkillOne;
    bool hasActiveStep = (currentStep != nullptr);
    bool redFlashActive = globalRotationEngine.IsRedFlashActive();

    ImDrawList* foregroundDrawList = ImGui::GetForegroundDrawList();

    for (const auto& slotBox : slotBoxes)
    {
        bool isTargetSlot = hasActiveStep && (slotBox.category == activeCategory);

        if (isTargetSlot)
        {
            ImU32 outerGlowColor = redFlashActive ? IM_COL32(255, 40, 40, 85) : IM_COL32(255, 215, 0, 80);
            ImU32 outlineColor = redFlashActive ? IM_COL32(255, 30, 30, 255) : IM_COL32(255, 215, 0, 255);

            ImVec2 outerGlowMin(slotBox.coordinateX - 3.0f, slotBox.coordinateY - 3.0f);
            ImVec2 outerGlowMax(slotBox.coordinateX + slotBox.boxWidth + 3.0f, slotBox.coordinateY + slotBox.boxHeight + 3.0f);
            foregroundDrawList->AddRect(outerGlowMin, outerGlowMax, outerGlowColor, 5.0f, 0, 2.0f);

            ImVec2 boxMinCoordinate(slotBox.coordinateX - 1.5f, slotBox.coordinateY - 1.5f);
            ImVec2 boxMaxCoordinate(slotBox.coordinateX + slotBox.boxWidth + 1.5f, slotBox.coordinateY + slotBox.boxHeight + 1.5f);
            foregroundDrawList->AddRect(boxMinCoordinate, boxMaxCoordinate, outlineColor, 3.5f, 0, 2.5f);

            if (globalRotationEngine.IsChannelingActive())
            {
                float castProgressFraction = globalRotationEngine.GetCastProgressPercentage();
                float remainingCastSeconds = globalRotationEngine.GetActiveCastRemainingSeconds();

                ImVec2 miniBarMin(slotBox.coordinateX - 2.0f, slotBox.coordinateY - 14.0f);
                ImVec2 miniBarMax(slotBox.coordinateX + slotBox.boxWidth + 2.0f, slotBox.coordinateY - 6.0f);
                foregroundDrawList->AddRectFilled(miniBarMin, miniBarMax, IM_COL32(15, 20, 25, 220), 2.0f);

                float miniBarFillWidth = (slotBox.boxWidth + 4.0f) * castProgressFraction;
                ImVec2 miniBarFillMax(miniBarMin.x + miniBarFillWidth, miniBarMax.y);
                foregroundDrawList->AddRectFilled(miniBarMin, miniBarFillMax, IM_COL32(0, 230, 255, 255), 2.0f);
                foregroundDrawList->AddRect(miniBarMin, miniBarMax, IM_COL32(255, 255, 255, 180), 2.0f, 0, 1.0f);

                char durationTextBuffer[32];
                snprintf(durationTextBuffer, sizeof(durationTextBuffer), "%.1fs", remainingCastSeconds);
                ImVec2 durationTextDimension = ImGui::CalcTextSize(durationTextBuffer);
                ImVec2 durationTextPosition(slotBox.coordinateX + ((slotBox.boxWidth - durationTextDimension.x) * 0.5f), slotBox.coordinateY - 28.0f);
                foregroundDrawList->AddText(durationTextPosition, IM_COL32(0, 240, 255, 255), durationTextBuffer);
            }
        }
        else if (globalShowAllActionBarSlotHighlights)
        {
            ImU32 secondaryOutlineColor = IM_COL32(0, 210, 255, 200);

            ImVec2 boxMinCoordinate(slotBox.coordinateX, slotBox.coordinateY);
            ImVec2 boxMaxCoordinate(slotBox.coordinateX + slotBox.boxWidth, slotBox.coordinateY + slotBox.boxHeight);

            foregroundDrawList->AddRect(boxMinCoordinate, boxMaxCoordinate, secondaryOutlineColor, 4.0f, 0, 1.5f);

            ImVec2 labelPosition(slotBox.coordinateX + 5.0f, slotBox.coordinateY + 4.0f);
            foregroundDrawList->AddText(labelPosition, IM_COL32(255, 255, 255, 220), slotBox.slotLabel.c_str());
        }
    }

    if (globalShowAllActionBarSlotHighlights)
    {
        float weaponBarMinCoordinateX = 99999.0f;
        float weaponBarMaxCoordinateX = -99999.0f;
        float utilityBarMinCoordinateX = 99999.0f;
        float utilityBarMaxCoordinateX = -99999.0f;
        float professionBarMinCoordinateX = 99999.0f;
        float professionBarMaxCoordinateX = -99999.0f;
        float weaponBarBaseCoordinateY = 0.0f;
        float utilityBarBaseCoordinateY = 0.0f;
        float professionBarBaseCoordinateY = 0.0f;

        for (const auto& slotBox : slotBoxes)
        {
            if (slotBox.category >= SkillSlotCategory::WeaponSkillOne && slotBox.category <= SkillSlotCategory::WeaponSkillFive)
            {
                if (slotBox.coordinateX < weaponBarMinCoordinateX)
                {
                    weaponBarMinCoordinateX = slotBox.coordinateX;
                }
                if (slotBox.coordinateX + slotBox.boxWidth > weaponBarMaxCoordinateX)
                {
                    weaponBarMaxCoordinateX = slotBox.coordinateX + slotBox.boxWidth;
                }
                weaponBarBaseCoordinateY = slotBox.coordinateY;
            }
            else if (slotBox.category >= SkillSlotCategory::HealingSkill && slotBox.category <= SkillSlotCategory::EliteSkill)
            {
                if (slotBox.coordinateX < utilityBarMinCoordinateX)
                {
                    utilityBarMinCoordinateX = slotBox.coordinateX;
                }
                if (slotBox.coordinateX + slotBox.boxWidth > utilityBarMaxCoordinateX)
                {
                    utilityBarMaxCoordinateX = slotBox.coordinateX + slotBox.boxWidth;
                }
                utilityBarBaseCoordinateY = slotBox.coordinateY;
            }
            else if (slotBox.category >= SkillSlotCategory::ProfessionSkillOne && slotBox.category <= SkillSlotCategory::ProfessionSkillFive)
            {
                if (slotBox.coordinateX < professionBarMinCoordinateX)
                {
                    professionBarMinCoordinateX = slotBox.coordinateX;
                }
                if (slotBox.coordinateX + slotBox.boxWidth > professionBarMaxCoordinateX)
                {
                    professionBarMaxCoordinateX = slotBox.coordinateX + slotBox.boxWidth;
                }
                professionBarBaseCoordinateY = slotBox.coordinateY;
            }
        }

        if (weaponBarMinCoordinateX < weaponBarMaxCoordinateX)
        {
            float weaponHandleCoordinateX = (weaponBarMinCoordinateX + weaponBarMaxCoordinateX) * 0.5f - 75.0f;
            float weaponHandleCoordinateY = weaponBarBaseCoordinateY - 32.0f;
            ImGui::SetNextWindowPos(ImVec2(weaponHandleCoordinateX, weaponHandleCoordinateY));
            if (ImGui::Begin("##WeaponBarDragHandle", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground))
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.45f, 0.85f, 0.85f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.6f, 1.0f, 0.95f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05f, 0.35f, 0.75f, 1.0f));
                ImGui::Button(":: Drag Weapon Bar ::");
                if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    ImVec2 mouseMovementDelta = ImGui::GetIO().MouseDelta;
                    globalWeaponBarHorizontalOffset = globalWeaponBarHorizontalOffset + mouseMovementDelta.x;
                    globalWeaponBarVerticalOffset = globalWeaponBarVerticalOffset + mouseMovementDelta.y;
                }
                ImGui::PopStyleColor(3);
            }
            ImGui::End();
        }

        if (utilityBarMinCoordinateX < utilityBarMaxCoordinateX)
        {
            float utilityHandleCoordinateX = (utilityBarMinCoordinateX + utilityBarMaxCoordinateX) * 0.5f - 70.0f;
            float utilityHandleCoordinateY = utilityBarBaseCoordinateY - 32.0f;
            ImGui::SetNextWindowPos(ImVec2(utilityHandleCoordinateX, utilityHandleCoordinateY));
            if (ImGui::Begin("##UtilityBarDragHandle", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground))
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.65f, 0.35f, 0.85f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.8f, 0.45f, 0.95f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05f, 0.5f, 0.25f, 1.0f));
                ImGui::Button(":: Drag Utility Bar ::");
                if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    ImVec2 mouseMovementDelta = ImGui::GetIO().MouseDelta;
                    globalUtilityBarHorizontalOffset = globalUtilityBarHorizontalOffset + mouseMovementDelta.x;
                    globalUtilityBarVerticalOffset = globalUtilityBarVerticalOffset + mouseMovementDelta.y;
                }
                ImGui::PopStyleColor(3);
            }
            ImGui::End();
        }

        if (professionBarMinCoordinateX < professionBarMaxCoordinateX)
        {
            std::string activeProfileKey = GetActiveProfessionProfileKey();
            ProfessionBarAlignmentProfile& activeProfessionProfile = GetProfessionAlignmentProfile(activeProfileKey);

            char professionButtonLabel[64];
            snprintf(professionButtonLabel, sizeof(professionButtonLabel), ":: Drag %s Bar ::", activeProfileKey.c_str());

            float professionHandleCoordinateX = (professionBarMinCoordinateX + professionBarMaxCoordinateX) * 0.5f - 75.0f;
            float professionHandleCoordinateY = professionBarBaseCoordinateY - 32.0f;
            ImGui::SetNextWindowPos(ImVec2(professionHandleCoordinateX, professionHandleCoordinateY));
            if (ImGui::Begin("##ProfessionBarDragHandle", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground))
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65f, 0.2f, 0.75f, 0.85f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.9f, 0.95f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.15f, 0.6f, 1.0f));
                ImGui::Button(professionButtonLabel);
                if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    ImVec2 mouseMovementDelta = ImGui::GetIO().MouseDelta;
                    activeProfessionProfile.horizontalOffset = activeProfessionProfile.horizontalOffset + mouseMovementDelta.x;
                    activeProfessionProfile.verticalOffset = activeProfessionProfile.verticalOffset + mouseMovementDelta.y;
                }
                ImGui::PopStyleColor(3);
            }
            ImGui::End();
        }
    }
}

static void RenderMasterControlPanel()
{
    ImGui::SetNextWindowSize(ImVec2(640, 580), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Rotation Coach - Master Control", &globalShowMasterControlPanel, ImGuiWindowFlags_NoCollapse))
    {
        const BuildProfile* headerActiveProfile = globalRotationEngine.GetActiveProfilePointer();
        if (headerActiveProfile != nullptr)
        {
            ProfessionColorRgba themeColor = globalRotationEngine.GetCatalogReference().GetProfessionThemeColor(headerActiveProfile->professionName);
            ImGui::TextColored(ImVec4(themeColor.red, themeColor.green, themeColor.blue, 1.0f), "%s (%s)", headerActiveProfile->professionName.c_str(), headerActiveProfile->specializationName.c_str());
            ImGui::SameLine();

            const std::vector<BuildProfile>& allProfiles = globalRotationEngine.GetCatalogReference().GetAllProfiles();
            ImGui::SetNextItemWidth(300.0f);
            if (ImGui::BeginCombo("##MasterHeaderActiveProfileCombo", headerActiveProfile->buildDisplayName.c_str()))
            {
                for (uint32_t profileIndex = 0; profileIndex < allProfiles.size(); profileIndex = profileIndex + 1)
                {
                    const BuildProfile& candidateProfile = allProfiles[profileIndex];
                    bool matchesSpecialization = (candidateProfile.professionIdentifierNumber == headerActiveProfile->professionIdentifierNumber) &&
                                                  (headerActiveProfile->specializationIdentifierNumber == 0 || candidateProfile.specializationIdentifierNumber == headerActiveProfile->specializationIdentifierNumber);

                    if (matchesSpecialization)
                    {
                        bool isSelected = (profileIndex == globalRotationEngine.GetActiveProfileIndex());
                        if (ImGui::Selectable(candidateProfile.buildDisplayName.c_str(), isSelected))
                        {
                            globalRotationEngine.LoadBuildProfileByIndex(profileIndex);
                        }

                        if (isSelected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                }

                ImGui::Separator();

                for (uint32_t profileIndex = 0; profileIndex < allProfiles.size(); profileIndex = profileIndex + 1)
                {
                    const BuildProfile& candidateProfile = allProfiles[profileIndex];
                    bool matchesSpecialization = (candidateProfile.professionIdentifierNumber == headerActiveProfile->professionIdentifierNumber) &&
                                                  (headerActiveProfile->specializationIdentifierNumber == 0 || candidateProfile.specializationIdentifierNumber == headerActiveProfile->specializationIdentifierNumber);

                    if (!matchesSpecialization)
                    {
                        bool isSelected = (profileIndex == globalRotationEngine.GetActiveProfileIndex());
                        std::string otherProfileLabel = candidateProfile.buildDisplayName + " (" + candidateProfile.professionName + ")";
                        if (ImGui::Selectable(otherProfileLabel.c_str(), isSelected))
                        {
                            globalRotationEngine.LoadBuildProfileByIndex(profileIndex);
                        }

                        if (isSelected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                }

                ImGui::EndCombo();
            }

            ImGui::SameLine();
            uint32_t activeWeaponSetIndex = globalRotationEngine.GetActiveWeaponSetIndex();
            std::string weaponSetButtonLabel = (activeWeaponSetIndex == 0) ? "Set 1 (Swap: `)" : "Set 2 (Swap: `)";
            if (ImGui::SmallButton(weaponSetButtonLabel.c_str()))
            {
                globalRotationEngine.ToggleActiveWeaponSet();
            }
        }

        ImGui::Spacing();

        if (ImGui::BeginTabBar("MasterControlTabBar", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Display & HUD Style"))
            {
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.35f, 1.0f), "Visual Presentation Mode:");

                PromptPresentationStyle presentationStyle = globalRotationEngine.GetPresentationStyle();
                int presentationStyleSelection = static_cast<int>(presentationStyle);

                if (ImGui::RadioButton("1. Transparent Simple Text (Floating keys and arrows only)", presentationStyleSelection == 0))
                {
                    globalRotationEngine.SetPresentationStyle(PromptPresentationStyle::SimpleTextRibbon);
                    globalShowRibbonHud = true;
                }

                if (ImGui::RadioButton("2. Skill Icon Images & Arrows (In-game textures)", presentationStyleSelection == 1))
                {
                    globalRotationEngine.SetPresentationStyle(PromptPresentationStyle::SkillIconsRibbon);
                    globalShowRibbonHud = true;
                }

                if (ImGui::RadioButton("3. Action Bar Slot Highlighter (Direct in-game slot overlay)", presentationStyleSelection == 2))
                {
                    globalRotationEngine.SetPresentationStyle(PromptPresentationStyle::ActionBarSlotHighlighter);
                    globalShowActionBarHighlighter = true;
                }

                ImGui::Spacing();
                ImGui::Separator();

                if (presentationStyle == PromptPresentationStyle::ActionBarSlotHighlighter)
                {
                    ImGui::TextColored(ImVec4(0.4f, 0.85f, 1.0f, 1.0f), "Action Bar Alignment (Global Offsets):");

                    ImGui::Text("UI Scale Preset:");
                    ImGui::SameLine();
                    if (ImGui::SmallButton("Small / Compact (36px)"))
                    {
                        globalActionBarSlotSize = 36.0f;
                        globalActionBarSlotSpacing = 4.0f;
                    }
                    ImGui::SameLine();
                    if (ImGui::SmallButton("Normal (48px)"))
                    {
                        globalActionBarSlotSize = 48.0f;
                        globalActionBarSlotSpacing = 7.0f;
                    }
                    ImGui::SameLine();
                    if (ImGui::SmallButton("Large (54px)"))
                    {
                        globalActionBarSlotSize = 54.0f;
                        globalActionBarSlotSpacing = 8.0f;
                    }

                    ImGui::SliderFloat("Weapon Bar Horizontal Offset", &globalWeaponBarHorizontalOffset, -300.0f, 300.0f, "%.0f px");
                    ImGui::SliderFloat("Weapon Bar Vertical Offset", &globalWeaponBarVerticalOffset, -200.0f, 100.0f, "%.0f px");
                    ImGui::SliderFloat("Utility Bar Horizontal Offset", &globalUtilityBarHorizontalOffset, -300.0f, 300.0f, "%.0f px");
                    ImGui::SliderFloat("Utility Bar Vertical Offset", &globalUtilityBarVerticalOffset, -200.0f, 100.0f, "%.0f px");
                    ImGui::SliderFloat("Slot Box Size", &globalActionBarSlotSize, 30.0f, 65.0f, "%.0f px");
                    ImGui::SliderFloat("Slot Box Spacing", &globalActionBarSlotSpacing, 0.0f, 20.0f, "%.0f px");

                    ImGui::Spacing();

                    std::string activeProfileKey = GetActiveProfessionProfileKey();
                    ProfessionBarAlignmentProfile& activeProfessionProfile = GetProfessionAlignmentProfile(activeProfileKey);

                    std::string professionSectionTitle = "Profession Mechanic Bar - " + activeProfileKey + " (Developer Controls)";
                    if (ImGui::CollapsingHeader(professionSectionTitle.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        ImGui::SliderFloat("Profession Horizontal Offset", &activeProfessionProfile.horizontalOffset, -300.0f, 300.0f, "%.0f px");
                        ImGui::SliderFloat("Profession Vertical Offset", &activeProfessionProfile.verticalOffset, -200.0f, 200.0f, "%.0f px");
                        ImGui::SliderFloat("Profession Slot Box Size", &activeProfessionProfile.slotBoxDimension, 20.0f, 60.0f, "%.0f px");
                        ImGui::SliderFloat("Profession Slot Spacing", &activeProfessionProfile.slotSpacingDimension, 0.0f, 20.0f, "%.0f px");

                        if (ImGui::Button("Reset Active Class to Defaults"))
                        {
                            activeProfessionProfile.horizontalOffset = 0.0f;
                            activeProfessionProfile.verticalOffset = 0.0f;
                            activeProfessionProfile.slotBoxDimension = 37.0f;
                            activeProfessionProfile.slotSpacingDimension = 1.0f;
                        }
                    }

                    ImGui::Spacing();
                    ImGui::Separator();
                }

                ImGui::TextColored(ImVec4(0.4f, 0.85f, 1.0f, 1.0f), "Floating HUD Position (Mode 1 & 2):");

                if (ImGui::Button("Reset HUD Position to Center of Screen"))
                {
                    globalResetRibbonHudPositionRequested = true;
                    globalRibbonHudHorizontalOffset = 0.0f;
                    globalRibbonHudVerticalOffset = 0.0f;
                }

                ImGui::SliderFloat("HUD Horizontal Offset", &globalRibbonHudHorizontalOffset, -800.0f, 800.0f, "%.0f px");
                ImGui::SliderFloat("HUD Vertical Offset", &globalRibbonHudVerticalOffset, -600.0f, 600.0f, "%.0f px");

                ImGui::Spacing();
                ImGui::Separator();

                ImGui::TextColored(ImVec4(0.4f, 0.85f, 1.0f, 1.0f), "Independent Floating Windows:");

                ImGui::Checkbox("Rotation HUD Ribbon", &globalShowRibbonHud);
                ImGui::Checkbox("Action Bar Skill Slot Highlighter (Direct In-Game Bar Overlay)", &globalShowActionBarHighlighter);
                ImGui::Checkbox("Highlight All Action Bar Slots (Diagnostic / Alignment Grid)", &globalShowAllActionBarSlotHighlights);
                ImGui::Checkbox("Build & Gear Inspector Window", &globalShowBuildInspectorWindow);
                ImGui::Checkbox("Detailed Pacing & Benchmark Window", &globalShowPacingMetricsWindow);
                ImGui::Checkbox("Mistake History Log Window", &globalShowMistakeHistoryWindow);

                ImGui::Spacing();
                if (ImGui::Button("Show All Windows (One-Click Display)"))
                {
                    globalShowRibbonHud = true;
                    globalShowActionBarHighlighter = true;
                    globalShowAllActionBarSlotHighlights = true;
                    globalShowMasterControlPanel = true;
                    globalShowBuildInspectorWindow = true;
                    globalShowPacingMetricsWindow = true;
                    globalShowMistakeHistoryWindow = true;
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Skills & Keybinds"))
            {
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.6f, 1.0f), "Action Bar Slots, Active Skills & Keybindings:");
                ImGui::TextDisabled("Unified setup showing each slot, its active skill, primary key, and secondary key:");

                if (!globalSlotWaitingForBinding.empty())
                {
                    ImGui::Spacing();
                    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), ">>> Press any Key or Mouse Button to bind to [%s] (or click Cancel) <<<", globalSlotWaitingForBinding.c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("Cancel"))
                    {
                        globalSlotWaitingForBinding = "";
                    }
                }

                struct SlotRowDefinition
                {
                    std::string slotKey;
                    std::string slotLabel;
                    SkillSlotCategory category;
                };

                const std::vector<SlotRowDefinition> slotRowDefinitions = {
                    {"WeaponSkill1", "Weapon Skill 1", SkillSlotCategory::WeaponSkillOne},
                    {"WeaponSkill2", "Weapon Skill 2", SkillSlotCategory::WeaponSkillTwo},
                    {"WeaponSkill3", "Weapon Skill 3", SkillSlotCategory::WeaponSkillThree},
                    {"WeaponSkill4", "Weapon Skill 4", SkillSlotCategory::WeaponSkillFour},
                    {"WeaponSkill5", "Weapon Skill 5", SkillSlotCategory::WeaponSkillFive},
                    {"HealingSkill", "Heal Skill", SkillSlotCategory::HealingSkill},
                    {"UtilitySkill1", "Utility Skill 1", SkillSlotCategory::UtilitySkillOne},
                    {"UtilitySkill2", "Utility Skill 2", SkillSlotCategory::UtilitySkillTwo},
                    {"UtilitySkill3", "Utility Skill 3", SkillSlotCategory::UtilitySkillThree},
                    {"EliteSkill", "Elite Skill", SkillSlotCategory::EliteSkill},
                    {"ProfessionSkill1", "Profession 1 (F1)", SkillSlotCategory::ProfessionSkillOne},
                    {"ProfessionSkill2", "Profession 2 (F2)", SkillSlotCategory::ProfessionSkillTwo},
                    {"ProfessionSkill3", "Profession 3 (F3)", SkillSlotCategory::ProfessionSkillThree},
                    {"ProfessionSkill4", "Profession 4 (F4)", SkillSlotCategory::ProfessionSkillFour},
                    {"ProfessionSkill5", "Profession 5 (F5)", SkillSlotCategory::ProfessionSkillFive},
                    {"WeaponSwap", "Weapon Swap (`)", SkillSlotCategory::WeaponSwap},
                    {"Dodge", "Dodge Action (V)", SkillSlotCategory::DodgeAction},
                    {"SpecialAction", "Special Action (N)", SkillSlotCategory::SpecialAction}
                };

                const BuildProfile* activeProfile = globalRotationEngine.GetActiveProfilePointer();

                if (ImGui::BeginTable("UnifiedSkillsAndKeybindsTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp))
                {
                    ImGui::TableSetupColumn("Action Slot", ImGuiTableColumnFlags_WidthFixed, 140.0f);
                    ImGui::TableSetupColumn("Active Skill in Rotation", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Primary Key", ImGuiTableColumnFlags_WidthFixed, 90.0f);
                    ImGui::TableSetupColumn("Secondary Key", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableSetupColumn("Quick Attach", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                    ImGui::TableHeadersRow();

                    for (const auto& rowDefinition : slotRowDefinitions)
                    {
                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%s", rowDefinition.slotLabel.c_str());

                        ImGui::TableSetColumnIndex(1);
                        std::string activeSkillName = "-";
                        uint32_t currentWeaponSet = globalRotationEngine.GetActiveWeaponSetIndex();

                        if (activeProfile != nullptr)
                        {
                            bool isWeaponSlot = (rowDefinition.category >= SkillSlotCategory::WeaponSkillOne && rowDefinition.category <= SkillSlotCategory::WeaponSkillFive);

                            for (const auto& step : activeProfile->sequenceSteps)
                            {
                                SkillSlotCategory effectiveCategory = globalRotationEngine.GetEffectiveSlotCategory(step);
                                if (effectiveCategory == rowDefinition.category)
                                {
                                    if (isWeaponSlot)
                                    {
                                        uint32_t stepSet = globalRotationEngine.DetermineStepWeaponSet(step);
                                        if (stepSet == currentWeaponSet || stepSet == 2 || stepSet == 3)
                                        {
                                            activeSkillName = step.skillName;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        activeSkillName = step.skillName;
                                        break;
                                    }
                                }
                            }

                            if (activeSkillName == "-")
                            {
                                if (rowDefinition.category == SkillSlotCategory::WeaponSkillOne)
                                {
                                    activeSkillName = "Flying Cutter";
                                }
                                else if (rowDefinition.category == SkillSlotCategory::WeaponSkillTwo)
                                {
                                    activeSkillName = "Bladecall";
                                }
                                else if (rowDefinition.category == SkillSlotCategory::WeaponSkillThree)
                                {
                                    activeSkillName = "Unstable Bladestorm";
                                }
                                else if (rowDefinition.category == SkillSlotCategory::WeaponSkillFour)
                                {
                                    activeSkillName = (currentWeaponSet == 0) ? "Illusionary Riposte" : "Phantasmal Warden";
                                }
                                else if (rowDefinition.category == SkillSlotCategory::WeaponSkillFive)
                                {
                                    activeSkillName = (currentWeaponSet == 0) ? "Phantasmal Swordsman" : "Temporal Curtain";
                                }
                            }

                            if (activeProfile->specializationIdentifierNumber == 66)
                            {
                                if (rowDefinition.category == SkillSlotCategory::ProfessionSkillFour)
                                {
                                    activeSkillName = "Bladeturn Requiem";
                                }
                                else if (rowDefinition.category == SkillSlotCategory::ProfessionSkillFive)
                                {
                                    activeSkillName = "Bladesong Distortion";
                                }
                            }

                            if (activeProfile->professionIdentifierNumber == 6)
                            {
                                if (rowDefinition.category == SkillSlotCategory::ProfessionSkillOne)
                                {
                                    activeSkillName = "Fire Attunement";
                                }
                                else if (rowDefinition.category == SkillSlotCategory::ProfessionSkillTwo)
                                {
                                    activeSkillName = "Water Attunement";
                                }
                                else if (rowDefinition.category == SkillSlotCategory::ProfessionSkillThree)
                                {
                                    activeSkillName = "Air Attunement";
                                }
                                else if (rowDefinition.category == SkillSlotCategory::ProfessionSkillFour)
                                {
                                    activeSkillName = "Earth Attunement";
                                }
                                else if (rowDefinition.category == SkillSlotCategory::ProfessionSkillFive)
                                {
                                    activeSkillName = "Deploy Jade Sphere";
                                }
                                else if (rowDefinition.category == SkillSlotCategory::WeaponSwap)
                                {
                                    activeSkillName = "No Weapon Swap (Attunements F1-F4)";
                                }
                            }
                        }
                        ImGui::Text("%s", activeSkillName.c_str());

                        ImGui::TableSetColumnIndex(2);
                        std::string primaryKey = globalKeybindDetector.GetPrimaryBindingForSlot(rowDefinition.slotKey);
                        ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.35f, 1.0f), "[%s]", primaryKey.c_str());

                        ImGui::TableSetColumnIndex(3);
                        std::string secondaryKey = globalKeybindDetector.GetSecondaryBindingForSlot(rowDefinition.slotKey);
                        if (secondaryKey != "-")
                        {
                            ImGui::TextColored(ImVec4(0.4f, 0.9f, 1.0f, 1.0f), "[%s]", secondaryKey.c_str());
                        }
                        else
                        {
                            ImGui::TextDisabled("-");
                        }

                        ImGui::TableSetColumnIndex(4);
                        ImGui::PushID(rowDefinition.slotKey.c_str());

                        if (globalSlotWaitingForBinding == rowDefinition.slotKey)
                        {
                            ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "[Press Key]");
                        }
                        else
                        {
                            if (ImGui::SmallButton("Bind"))
                            {
                                globalSlotWaitingForBinding = rowDefinition.slotKey;
                            }
                        }

                        ImGui::SameLine();
                        if (ImGui::SmallButton("+M5"))
                        {
                            globalKeybindDetector.SetSecondaryBindingForSlot(rowDefinition.slotKey, "Mouse5");
                        }
                        ImGui::SameLine();
                        if (ImGui::SmallButton("+M4"))
                        {
                            globalKeybindDetector.SetSecondaryBindingForSlot(rowDefinition.slotKey, "Mouse4");
                        }
                        ImGui::SameLine();
                        if (ImGui::SmallButton("+M3"))
                        {
                            globalKeybindDetector.SetSecondaryBindingForSlot(rowDefinition.slotKey, "MiddleMouse");
                        }
                        ImGui::SameLine();
                        if (ImGui::SmallButton("Clr"))
                        {
                            globalKeybindDetector.ClearBindingsForSlot(rowDefinition.slotKey);
                        }

                        ImGui::PopID();
                    }

                    ImGui::EndTable();
                }

                ImGui::Spacing();
                if (ImGui::Button("Reset to Recommended Dual-Keybindings"))
                {
                    globalKeybindDetector.ResetToDefaultKeybindings();
                }
                ImGui::SameLine();
                if (ImGui::Button("Import from GW2 InputBinds (Testing.xml)"))
                {
                    const char* userProfileDirectory = std::getenv("USERPROFILE");
                    if (userProfileDirectory != nullptr)
                    {
                        std::string xmlPath = std::string(userProfileDirectory) + "\\Documents\\Guild Wars 2\\InputBinds\\Testing.xml";
                        globalKeybindDetector.LoadFromGameBindsXmlFile(xmlPath);
                    }
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Build & Benchmark"))
            {
                ImGui::Spacing();

                bool autoDetectionState = globalRotationEngine.IsAutoDetectionEnabled();
                if (ImGui::Checkbox("Auto-Detect Character & Build (MumbleLink)", &autoDetectionState))
                {
                    globalRotationEngine.SetAutoDetectionEnabled(autoDetectionState);
                }

                bool autoSkipCooldownState = globalRotationEngine.IsAutoSkipCooldownSkillsEnabled();
                if (ImGui::Checkbox("Auto-Skip Skills on Cooldown", &autoSkipCooldownState))
                {
                    globalRotationEngine.SetAutoSkipCooldownSkillsEnabled(autoSkipCooldownState);
                }

                const BuildProfile* activeProfile = globalRotationEngine.GetActiveProfilePointer();
                if (activeProfile != nullptr)
                {
                    ProfessionColorRgba themeColor = globalRotationEngine.GetCatalogReference().GetProfessionThemeColor(activeProfile->professionName);
                    ImGui::TextColored(ImVec4(themeColor.red, themeColor.green, themeColor.blue, 1.0f), "Detected Class: %s (%s)", activeProfile->professionName.c_str(), activeProfile->specializationName.c_str());

                    const std::vector<BuildProfile>& allProfiles = globalRotationEngine.GetCatalogReference().GetAllProfiles();
                    if (ImGui::BeginCombo("Active Profile", activeProfile->buildDisplayName.c_str()))
                    {
                        for (uint32_t profileIndex = 0; profileIndex < allProfiles.size(); profileIndex = profileIndex + 1)
                        {
                            const BuildProfile& candidateProfile = allProfiles[profileIndex];
                            bool matchesSpecialization = (candidateProfile.professionIdentifierNumber == activeProfile->professionIdentifierNumber) &&
                                                          (activeProfile->specializationIdentifierNumber == 0 || candidateProfile.specializationIdentifierNumber == activeProfile->specializationIdentifierNumber);

                            if (matchesSpecialization)
                            {
                                bool isSelected = (profileIndex == globalRotationEngine.GetActiveProfileIndex());
                                if (ImGui::Selectable(candidateProfile.buildDisplayName.c_str(), isSelected))
                                {
                                    globalRotationEngine.LoadBuildProfileByIndex(profileIndex);
                                }

                                if (isSelected)
                                {
                                    ImGui::SetItemDefaultFocus();
                                }
                            }
                        }

                        ImGui::Separator();

                        for (uint32_t profileIndex = 0; profileIndex < allProfiles.size(); profileIndex = profileIndex + 1)
                        {
                            const BuildProfile& candidateProfile = allProfiles[profileIndex];
                            bool matchesSpecialization = (candidateProfile.professionIdentifierNumber == activeProfile->professionIdentifierNumber) &&
                                                          (activeProfile->specializationIdentifierNumber == 0 || candidateProfile.specializationIdentifierNumber == activeProfile->specializationIdentifierNumber);

                            if (!matchesSpecialization)
                            {
                                bool isSelected = (profileIndex == globalRotationEngine.GetActiveProfileIndex());
                                std::string otherProfileLabel = candidateProfile.buildDisplayName + " (" + candidateProfile.professionName + ")";
                                if (ImGui::Selectable(otherProfileLabel.c_str(), isSelected))
                                {
                                    globalRotationEngine.LoadBuildProfileByIndex(profileIndex);
                                }

                                if (isSelected)
                                {
                                    ImGui::SetItemDefaultFocus();
                                }
                            }
                        }

                        ImGui::EndCombo();
                    }
                }

                ImGui::Spacing();
                ImGui::Separator();

                uint32_t activeWeaponIndex = globalRotationEngine.GetActiveWeaponSetIndex();

                std::string primaryWeaponName = "First Weapon Set";
                std::string secondaryWeaponName = "Second Weapon Set";

                if (activeProfile != nullptr)
                {
                    if (!activeProfile->equipment.primaryWeaponSetName.empty())
                    {
                        primaryWeaponName = "First Weapon Set (" + activeProfile->equipment.primaryWeaponSetName + ")";
                    }
                    if (!activeProfile->equipment.secondaryWeaponSetName.empty())
                    {
                        secondaryWeaponName = "Second Weapon Set (" + activeProfile->equipment.secondaryWeaponSetName + ")";
                    }
                }

                ImGui::Text("Current Weapon Set: %s", activeWeaponIndex == 0 ? primaryWeaponName.c_str() : secondaryWeaponName.c_str());
                ImGui::SameLine();
                if (ImGui::SmallButton("Toggle Weapon Swap (`)"))
                {
                    globalRotationEngine.ToggleActiveWeaponSet();
                }

                ImGui::SameLine();
                if (ImGui::SmallButton("Reset Rotation Sequence (Alt+Shift+R)"))
                {
                    globalRotationEngine.ResetRotationProgress();
                }

                ImGui::Spacing();
                ImGui::Separator();

                ImGui::Checkbox("Auto-Detect Live Game Ping", &globalAutoDetectLivePing);
                if (globalAutoDetectLivePing)
                {
                    ImGui::Text("Network Ping: %.0f ms (Auto-Detected)", globalRotationEngine.GetNetworkLatencyMilliseconds());
                }
                else
                {
                    float latencySliderValue = static_cast<float>(globalRotationEngine.GetNetworkLatencyMilliseconds());
                    if (ImGui::SliderFloat("Network Latency (Ping in ms)", &latencySliderValue, 10.0f, 350.0f, "%.0f ms"))
                    {
                        globalRotationEngine.SetNetworkLatencyMilliseconds(static_cast<double>(latencySliderValue));
                    }
                }

                ImGui::Spacing();

                if (ImGui::Button(globalShowBuildInspectorWindow ? "Close Build & Gear Showcase" : "Open Build & Gear Showcase"))
                {
                    globalShowBuildInspectorWindow = !globalShowBuildInspectorWindow;
                }

                ImGui::SameLine();
                if (ImGui::Button(globalShowPacingMetricsWindow ? "Close Pacing Metrics" : "Open Pacing Metrics"))
                {
                    globalShowPacingMetricsWindow = !globalShowPacingMetricsWindow;
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

static void RenderCompactRibbonHud()
{
    PromptPresentationStyle presentationStyle = globalRotationEngine.GetPresentationStyle();

    if (presentationStyle == PromptPresentationStyle::ActionBarSlotHighlighter)
    {
        return;
    }

    ImGuiWindowFlags ribbonWindowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;

    ImVec2 displayDimension = ImGui::GetIO().DisplaySize;
    float defaultPositionX = ((displayDimension.x - 380.0f) * 0.5f) + globalRibbonHudHorizontalOffset;
    float defaultPositionY = (displayDimension.y - 240.0f) + globalRibbonHudVerticalOffset;

    if (globalResetRibbonHudPositionRequested)
    {
        ImGui::SetNextWindowPos(ImVec2(defaultPositionX, defaultPositionY), ImGuiCond_Always);
        globalResetRibbonHudPositionRequested = false;
    }
    else
    {
        ImGui::SetNextWindowPos(ImVec2(defaultPositionX, defaultPositionY), ImGuiCond_FirstUseEver);
    }

    bool redFlashActive = globalRotationEngine.IsRedFlashActive();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.06f, 0.08f, 0.12f, 0.94f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.25f, 0.32f, 0.42f, 0.75f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.5f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 6.0f));

    if (ImGui::Begin("Rotation Ribbon", &globalShowRibbonHud, ribbonWindowFlags))
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.2f, 0.26f, 0.85f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.28f, 0.38f, 0.48f, 0.95f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.75f, 0.85f, 0.95f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

        ImGui::Button(":::\n:::", ImVec2(22.0f, 48.0f));

        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            ImVec2 dragDelta = ImGui::GetIO().MouseDelta;
            ImVec2 currentWindowPosition = ImGui::GetWindowPos();
            ImGui::SetWindowPos(ImVec2(currentWindowPosition.x + dragDelta.x, currentWindowPosition.y + dragDelta.y));
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();

        const RotationStep* currentStep = globalRotationEngine.GetCurrentStepPointer();

        if (presentationStyle == PromptPresentationStyle::SimpleTextRibbon)
        {
            if (currentStep != nullptr)
            {
                std::string currentKeyText = FormatKeyOnlyPrompt(*currentStep);

                ImVec4 activeBoxBorderColor = redFlashActive ? ImVec4(1.0f, 0.2f, 0.2f, 1.0f) : ImVec4(1.0f, 0.85f, 0.2f, 1.0f);
                ImVec4 activeBoxBackgroundColor = redFlashActive ? ImVec4(0.4f, 0.05f, 0.05f, 0.85f) : ImVec4(0.08f, 0.12f, 0.16f, 0.85f);
                ImVec4 activeTextColor = redFlashActive ? ImVec4(1.0f, 0.3f, 0.3f, 1.0f) : ImVec4(1.0f, 1.0f, 0.4f, 1.0f);

                ImGui::PushStyleColor(ImGuiCol_Border, activeBoxBorderColor);
                ImGui::PushStyleColor(ImGuiCol_Button, activeBoxBackgroundColor);
                ImGui::PushStyleColor(ImGuiCol_Text, activeTextColor);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.5f);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

                ImGui::Button(currentKeyText.c_str(), ImVec2(48.0f, 48.0f));

                ImGui::PopStyleVar(2);
                ImGui::PopStyleColor(3);
            }

            for (uint32_t lookaheadIndex = 1; lookaheadIndex <= 4; lookaheadIndex = lookaheadIndex + 1)
            {
                const RotationStep* upcomingStep = globalRotationEngine.GetLookaheadStepPointer(lookaheadIndex);

                if (upcomingStep != nullptr)
                {
                    ImGui::SameLine();
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 14.0f);
                    ImGui::TextColored(ImVec4(0.6f, 0.65f, 0.7f, 0.9f), "->");

                    std::string upcomingKeyText = FormatKeyOnlyPrompt(*upcomingStep);

                    ImGui::SameLine();
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 14.0f);

                    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.45f, 0.5f, 0.6f));
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.06f, 0.08f, 0.1f, 0.65f));
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.8f, 0.85f, 0.85f));
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.5f);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

                    ImGui::Button(upcomingKeyText.c_str(), ImVec2(44.0f, 44.0f));

                    ImGui::PopStyleVar(2);
                    ImGui::PopStyleColor(3);
                }
            }
        }
        else if (presentationStyle == PromptPresentationStyle::SkillIconsRibbon)
        {
            float iconRowPositionY = ImGui::GetCursorPosY();

            if (currentStep != nullptr)
            {
                SkillSlotCategory currentSlot = globalRotationEngine.GetEffectiveSlotCategory(*currentStep);
                Texture_t* skillTexture = RetrieveSkillTexture(currentStep->skillIdentifier, currentSlot, currentStep->skillName);
                std::string currentKeyText = FormatKeyOnlyPrompt(*currentStep);

                ImVec4 activeBorderColor = redFlashActive ? ImVec4(1.0f, 0.2f, 0.2f, 1.0f) : ImVec4(1.0f, 0.85f, 0.2f, 1.0f);

                ImGui::SetCursorPosY(iconRowPositionY);
                ImGui::BeginGroup();

                ImGui::PushStyleColor(ImGuiCol_Border, activeBorderColor);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.5f);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

                if (skillTexture != nullptr && skillTexture->Resource != nullptr)
                {
                    ImGui::Image(skillTexture->Resource, ImVec2(48.0f, 48.0f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), activeBorderColor);
                }
                else
                {
                    ImGui::Button(currentKeyText.c_str(), ImVec2(48.0f, 48.0f));
                }

                ImGui::PopStyleVar(2);
                ImGui::PopStyleColor();

                ImVec2 badgeTextDimension = ImGui::CalcTextSize(currentKeyText.c_str());
                float offsetBadgeX = (48.0f - badgeTextDimension.x) * 0.5f;

                if (offsetBadgeX > 0.0f)
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetBadgeX);
                }

                if (redFlashActive)
                {
                    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "[%s]", currentKeyText.c_str());
                }
                else
                {
                    ImGui::TextColored(ImVec4(1.0f, 0.95f, 0.3f, 1.0f), "[%s]", currentKeyText.c_str());
                }

                ImGui::EndGroup();
            }

            for (uint32_t lookaheadIndex = 1; lookaheadIndex <= 4; lookaheadIndex = lookaheadIndex + 1)
            {
                const RotationStep* upcomingStep = globalRotationEngine.GetLookaheadStepPointer(lookaheadIndex);

                if (upcomingStep != nullptr)
                {
                    ImGui::SameLine();
                    ImGui::SetCursorPosY(iconRowPositionY + 16.0f);
                    ImGui::TextColored(ImVec4(0.6f, 0.65f, 0.7f, 0.9f), "->");

                    SkillSlotCategory upcomingSlot = globalRotationEngine.GetEffectiveSlotCategory(*upcomingStep);
                    Texture_t* upcomingTexture = RetrieveSkillTexture(upcomingStep->skillIdentifier, upcomingSlot, upcomingStep->skillName);
                    std::string upcomingKeyText = FormatKeyOnlyPrompt(*upcomingStep);

                    ImGui::SameLine();
                    ImGui::SetCursorPosY(iconRowPositionY);
                    ImGui::BeginGroup();

                    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.45f, 0.5f, 0.6f));
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.5f);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

                    if (upcomingTexture != nullptr && upcomingTexture->Resource != nullptr)
                    {
                        ImGui::Image(upcomingTexture->Resource, ImVec2(44.0f, 44.0f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.85f, 0.85f, 0.85f, 0.85f), ImVec4(0.4f, 0.45f, 0.5f, 0.6f));
                    }
                    else
                    {
                        ImGui::Button(upcomingKeyText.c_str(), ImVec2(44.0f, 44.0f));
                    }

                    ImGui::PopStyleVar(2);
                    ImGui::PopStyleColor();

                    ImVec2 upcomingBadgeDimension = ImGui::CalcTextSize(upcomingKeyText.c_str());
                    float offsetUpcomingX = (44.0f - upcomingBadgeDimension.x) * 0.5f;

                    if (offsetUpcomingX > 0.0f)
                    {
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetUpcomingX);
                    }

                    ImGui::TextDisabled("[%s]", upcomingKeyText.c_str());

                    ImGui::EndGroup();
                }
            }
        }

        if (globalRotationEngine.IsChannelingActive())
        {
            ImGui::Spacing();

            float castProgressFraction = globalRotationEngine.GetCastProgressPercentage();
            float remainingCastSeconds = globalRotationEngine.GetActiveCastRemainingSeconds();
            char castStatusMessageBuffer[128];
            snprintf(castStatusMessageBuffer, sizeof(castStatusMessageBuffer), "Casting: %s (%.1fs)", globalRotationEngine.GetActiveCastingSkillName().c_str(), remainingCastSeconds);

            ImGui::ProgressBar(castProgressFraction, ImVec2(240.0f, 14.0f), castStatusMessageBuffer);
        }
    }
    ImGui::End();

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);
}

static uint32_t CalculateBuildRoleSortPriority(BuildRoleCategory roleCategory, DamageTypeCategory damageCategory, const std::string& buildDisplayName, const std::string& profileIdentifier)

{

    std::string combinedLowerText = buildDisplayName + " " + profileIdentifier;

    for (char& character : combinedLowerText)

    {

        character = static_cast<char>(tolower(character));

    }



    if (roleCategory == BuildRoleCategory::HealSupport || combinedLowerText.find("heal") != std::string::npos)

    {

        if (combinedLowerText.find("quick") != std::string::npos)

        {

            return 10;

        }



        return 9;

    }



    if (roleCategory == BuildRoleCategory::HybridSupportDamage || combinedLowerText.find("celestial") != std::string::npos || combinedLowerText.find("hybrid") != std::string::npos)

    {

        if (combinedLowerText.find("quick") != std::string::npos)

        {

            return 8;

        }



        return 7;

    }



    if (roleCategory == BuildRoleCategory::BoonAlacrity || combinedLowerText.find("alac") != std::string::npos)

    {

        if (damageCategory == DamageTypeCategory::PowerDamage || combinedLowerText.find("power") != std::string::npos)

        {

            return 3;

        }



        return 5;

    }



    if (roleCategory == BuildRoleCategory::BoonQuickness || combinedLowerText.find("quick") != std::string::npos)

    {

        if (damageCategory == DamageTypeCategory::PowerDamage || combinedLowerText.find("power") != std::string::npos)

        {

            return 4;

        }



        return 6;

    }



    if (damageCategory == DamageTypeCategory::PowerDamage || combinedLowerText.find("power") != std::string::npos)

    {

        return 1;

    }



    return 2;

}



static const char* GetRoleCategorySectionTitle(uint32_t priorityTier)

{

    switch (priorityTier)

    {

    case 1:

        return "Power DPS";

    case 2:

        return "Condition DPS";

    case 3:

        return "Power Alacrity";

    case 4:

        return "Power Quickness";

    case 5:

        return "Condition Alacrity";

    case 6:

        return "Condition Quickness";

    case 7:

        return "Hybrid Alacrity";

    case 8:

        return "Hybrid Quickness";

    case 9:

        return "Heal Alacrity";

    case 10:

        return "Heal Quickness";

    default:

        return "Other Builds";

    }

}



static void RenderBuildInspectorWindow()

{

    ImGui::SetNextWindowSize(ImVec2(680, 580), ImGuiCond_FirstUseEver);



    if (ImGui::Begin("Build & Gear Showcase", &globalShowBuildInspectorWindow))

    {

        const RotationCatalog& catalog = globalRotationEngine.GetCatalogReference();

        const auto& allProfiles = catalog.GetAllProfiles();

        uint32_t activeIndex = globalRotationEngine.GetActiveProfileIndex();



        const BuildProfile* activeProfile = globalRotationEngine.GetActiveProfilePointer();

        std::string activeProfession = activeProfile != nullptr ? activeProfile->professionName : "";

        std::string activeSpecialization = activeProfile != nullptr ? activeProfile->specializationName : "";



        ImGui::Text("Search Benchmark Build:");

        ImGui::InputText("##BuildSearchFilter", globalBuildSearchBuffer, sizeof(globalBuildSearchBuffer));



        std::string searchFilterString = globalBuildSearchBuffer;

        std::transform(searchFilterString.begin(), searchFilterString.end(), searchFilterString.begin(), ::tolower);



        ImGui::Spacing();

        ImGui::Text("Select Build by Profession & Specialization:");

        ImGui::Separator();



        std::map<std::string, std::map<std::string, std::map<uint32_t, std::vector<uint32_t>>>> professionHierarchy;



        for (uint32_t profileIndex = 0; profileIndex < allProfiles.size(); profileIndex = profileIndex + 1)

        {

            const auto& profile = allProfiles[profileIndex];



            if (!searchFilterString.empty())

            {

                std::string lowercaseDisplayName = profile.buildDisplayName;

                std::transform(lowercaseDisplayName.begin(), lowercaseDisplayName.end(), lowercaseDisplayName.begin(), ::tolower);



                std::string lowercaseSpecName = profile.specializationName;

                std::transform(lowercaseSpecName.begin(), lowercaseSpecName.end(), lowercaseSpecName.begin(), ::tolower);



                std::string lowercaseProfName = profile.professionName;

                std::transform(lowercaseProfName.begin(), lowercaseProfName.end(), lowercaseProfName.begin(), ::tolower);



                if (lowercaseDisplayName.find(searchFilterString) == std::string::npos &&

                    lowercaseSpecName.find(searchFilterString) == std::string::npos &&

                    lowercaseProfName.find(searchFilterString) == std::string::npos)

                {

                    continue;

                }

            }



            uint32_t rolePriority = CalculateBuildRoleSortPriority(profile.roleCategory, profile.damageCategory, profile.buildDisplayName, profile.profileIdentifier);



            professionHierarchy[profile.professionName][profile.specializationName][rolePriority].push_back(profileIndex);

        }



        if (ImGui::BeginChild("BuildScrollingList", ImVec2(0, 320), true))

        {

            if (allProfiles.empty())

            {

                ImGui::Spacing();

                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "No benchmark build files were found.");

                ImGui::Spacing();

                ImGui::TextWrapped("The addon assets folder is missing or not located in the expected directory.");

                ImGui::Spacing();

                ImGui::TextWrapped("Please extract the 'Gw2RotationCoach' assets folder into your Guild Wars 2 addons directory:");

                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "%s", catalog.GetStandaloneAddonDirectory().c_str());

                ImGui::Spacing();

                if (ImGui::Button("Open Addons Folder in File Explorer", ImVec2(280, 28)))

                {

                    ShellExecuteA(nullptr, "open", catalog.GetStandaloneAddonDirectory().c_str(), nullptr, nullptr, SW_SHOW);

                }

            }

            for (const auto& professionEntry : professionHierarchy)

            {

                const std::string& currentProfession = professionEntry.first;

                ProfessionColorRgba classColor = catalog.GetProfessionThemeColor(currentProfession);

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(classColor.red, classColor.green, classColor.blue, 1.0f));



                bool openProfessionByDefault = (!searchFilterString.empty()) || (currentProfession == activeProfession);

                ImGuiTreeNodeFlags professionFlags = openProfessionByDefault ? (ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed) : ImGuiTreeNodeFlags_Framed;



                bool professionExpanded = ImGui::TreeNodeEx(currentProfession.c_str(), professionFlags);

                ImGui::PopStyleColor();



                if (professionExpanded)

                {

                    for (const auto& specializationEntry : professionEntry.second)

                    {

                        const std::string& currentSpecialization = specializationEntry.first;



                        bool openSpecializationByDefault = (!searchFilterString.empty()) || (currentSpecialization == activeSpecialization);

                        ImGuiTreeNodeFlags specializationFlags = openSpecializationByDefault ? ImGuiTreeNodeFlags_DefaultOpen : 0;



                        if (ImGui::TreeNodeEx(currentSpecialization.c_str(), specializationFlags))

                        {

                            for (const auto& roleGroupEntry : specializationEntry.second)

                            {

                                uint32_t rolePriority = roleGroupEntry.first;

                                const char* roleSectionTitle = GetRoleCategorySectionTitle(rolePriority);



                                if (ImGui::TreeNodeEx(roleSectionTitle, ImGuiTreeNodeFlags_DefaultOpen))

                                {

                                    for (uint32_t profileIndex : roleGroupEntry.second)

                                    {

                                        const auto& profile = allProfiles[profileIndex];

                                        bool isSelected = (profileIndex == activeIndex);



                                        std::string itemLabel = profile.buildDisplayName;

                                        if (!profile.equipment.primaryWeaponSetName.empty() && profile.equipment.primaryWeaponSetName != "First Weapon Set")
                                        {
                                            if (!profile.equipment.secondaryWeaponSetName.empty() &&
                                                profile.equipment.secondaryWeaponSetName != "Second Weapon Set" &&
                                                profile.equipment.secondaryWeaponSetName.find("None") == std::string::npos)
                                            {
                                                itemLabel = itemLabel + " (" + profile.equipment.primaryWeaponSetName + " / " + profile.equipment.secondaryWeaponSetName + ")";
                                            }
                                            else
                                            {
                                                itemLabel = itemLabel + " (" + profile.equipment.primaryWeaponSetName + ")";
                                            }
                                        }

                                        if (ImGui::Selectable(itemLabel.c_str(), isSelected))

                                        {

                                            globalRotationEngine.LoadBuildProfileByIndex(profileIndex);

                                        }

                                    }



                                    ImGui::TreePop();

                                }

                            }



                            ImGui::TreePop();

                        }

                    }



                    ImGui::TreePop();

                }

            }

        }

        ImGui::EndChild();



        ImGui::Spacing();

        ImGui::Separator();



        if (activeProfile != nullptr)
        {
            ImGui::TextColored(ImVec4(0.2f, 0.9f, 1.0f, 1.0f), "Active Build: %s", activeProfile->buildDisplayName.c_str());
            ImGui::Text("Profession: %s   |   Specialization: %s", activeProfile->professionName.c_str(), activeProfile->specializationName.c_str());

            if (!activeProfile->chatTemplateCode.empty())
            {
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.35f, 1.0f), "Build Template Chat Code (In-Game Chat Link):");

                char chatBuffer[256];
                strncpy_s(chatBuffer, activeProfile->chatTemplateCode.c_str(), sizeof(chatBuffer) - 1);
                ImGui::SetNextItemWidth(360);
                ImGui::InputText("##ChatCodeDisplay", chatBuffer, sizeof(chatBuffer), ImGuiInputTextFlags_ReadOnly);
                ImGui::SameLine();
                if (ImGui::Button("Copy Build Template"))
                {
                    ImGui::SetClipboardText(activeProfile->chatTemplateCode.c_str());
                }
            }

            if (!activeProfile->snowCrowsGuideUrl.empty())
            {
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(0.4f, 0.85f, 1.0f, 1.0f), "Snow Crows Guide URL:");

                char urlBuffer[256];
                strncpy_s(urlBuffer, activeProfile->snowCrowsGuideUrl.c_str(), sizeof(urlBuffer) - 1);
                ImGui::SetNextItemWidth(360);
                ImGui::InputText("##SnowCrowsUrlDisplay", urlBuffer, sizeof(urlBuffer), ImGuiInputTextFlags_ReadOnly);
                ImGui::SameLine();
                if (ImGui::Button("Copy Guide URL"))
                {
                    ImGui::SetClipboardText(activeProfile->snowCrowsGuideUrl.c_str());
                }
            }

            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "Recommended Equipment Loadout:");

            ImGui::BulletText("First Weapon Set: %s", activeProfile->equipment.primaryWeaponSetName.c_str());
            ImGui::BulletText("Second Weapon Set: %s", activeProfile->equipment.secondaryWeaponSetName.c_str());
            ImGui::BulletText("Armor Attributes: %s", activeProfile->equipment.armorAttributeName.c_str());
            ImGui::BulletText("Upgrades / Runes: %s", activeProfile->equipment.upgradeRuneName.c_str());
            ImGui::BulletText("Weapon Sigils: %s", activeProfile->equipment.weaponSigilNames.c_str());
            ImGui::BulletText("Relic: %s", activeProfile->equipment.equipmentRelicName.c_str());
            ImGui::BulletText("Food Consumable: %s", activeProfile->equipment.foodConsumableName.c_str());
            ImGui::BulletText("Utility Consumable: %s", activeProfile->equipment.utilityConsumableName.c_str());
        }
    }
    ImGui::End();
}

static void RenderPacingMetricsWindow()
{
    ImGui::SetNextWindowSize(ImVec2(440, 280), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Benchmark Pacing Metrics", &globalShowPacingMetricsWindow))
    {
        BenchmarkEvaluation evaluation = globalRotationEngine.CalculateBenchmarkEvaluation();

        ImGui::Text("Network Ping (Latency): %.0f ms%s", globalRotationEngine.GetNetworkLatencyMilliseconds(), globalAutoDetectLivePing ? " (Auto-Detected)" : "");
        ImGui::Separator();

        ImGui::Text("85%% Benchmark Target Gap: %.0f ms", evaluation.benchmarkEightyFivePercentMilliseconds);
        ImGui::Text("95%% Benchmark Target Gap: %.0f ms", evaluation.benchmarkNinetyFivePercentMilliseconds);

        ImGui::Spacing();
        ImGui::Separator();

        if (evaluation.currentAverageGapMilliseconds > 0.0)
        {
            ImGui::Text("Your Average Cast Gap: %.0f ms (%.0f%% Optimal Speed)", evaluation.currentAverageGapMilliseconds, evaluation.executionSpeedPercentage);

            if (evaluation.deficitPercentageNinetyFive > 0.0)
            {
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.2f, 1.0f), "Slower than 95%% Target: +%.1f%% slower", evaluation.deficitPercentageNinetyFive);
            }
            else
            {
                ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.4f, 1.0f), "95%% Benchmark Status: Matching / Beating Target!");
            }

            if (evaluation.deficitPercentageEightyFive > 0.0)
            {
                ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "Slower than 85%% Target: +%.1f%% slower", evaluation.deficitPercentageEightyFive);
            }
            else
            {
                ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.4f, 1.0f), "85%% Benchmark Status: Matching / Beating Target!");
            }
        }
        else
        {
            ImGui::TextDisabled("Begin executing rotation skills to collect cast intervals.");
        }

        ImGui::Spacing();
        ImGui::Text("Successful Casts Recorded: %u", evaluation.successfulCastCount);
        ImGui::Text("Miscasts / Interrupts: %u", evaluation.mistakeCount);
    }
    ImGui::End();
}

static void RenderMistakeHistoryWindow()
{
    ImGui::SetNextWindowSize(ImVec2(460, 280), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Rotation Mistake Breakdown", &globalShowMistakeHistoryWindow))
    {
        const auto& mistakes = globalRotationEngine.GetMistakeHistory();

        if (mistakes.empty())
        {
            ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.4f, 1.0f), "Clean execution! Zero miscasts recorded.");
        }
        else
        {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.3f, 1.0f), "Recorded %zu Miscast(s):", mistakes.size());
            ImGui::Separator();

            for (size_t mistakeIndex = 0; mistakeIndex < mistakes.size(); mistakeIndex = mistakeIndex + 1)
            {
                const auto& mistake = mistakes[mistakeIndex];
                ImGui::BulletText("Step #%u: Expected [%s] but pressed [%s]",
                                  mistake.stepIndex + 1,
                                  mistake.expectedSkillName.c_str(),
                                  mistake.actualInputName.c_str());
            }
        }
    }
    ImGui::End();
}

static void RenderCoachUserInterface()
{
    AdvanceFrameTimer();

    UpdateLiveNetworkPing();

    if (globalNexusAddonInterface != nullptr)
    {
        NexusLinkData_t* nexusLinkDataPointer = static_cast<NexusLinkData_t*>(globalNexusAddonInterface->DataLink_Get(DL_NEXUS_LINK));

        if (nexusLinkDataPointer != nullptr)
        {
            if (!nexusLinkDataPointer->IsGameplay)
            {
                return;
            }
        }
        else if (!globalRotationEngine.IsInActiveGameplay())
        {
            return;
        }
    }
    else if (!globalRotationEngine.IsInActiveGameplay())
    {
        return;
    }

    if (globalShowActionBarHighlighter || globalShowAllActionBarSlotHighlights)
    {
        RenderActionBarSlotHighlighter();
    }

    if (globalShowRibbonHud)
    {
        RenderCompactRibbonHud();
    }

    if (globalShowMasterControlPanel)
    {
        RenderMasterControlPanel();
    }

    if (globalShowBuildInspectorWindow)
    {
        RenderBuildInspectorWindow();
    }

    if (globalShowPacingMetricsWindow)
    {
        RenderPacingMetricsWindow();
    }

    if (globalShowMistakeHistoryWindow)
    {
        RenderMistakeHistoryWindow();
    }
}

static void RenderCoachOptionsUserInterface()
{
    ImGui::Text("Rotation Coach Configuration");
    ImGui::Separator();

    ImGui::Checkbox("Master Control Panel Window", &globalShowMasterControlPanel);
    ImGui::Checkbox("Rotation HUD Ribbon", &globalShowRibbonHud);
    ImGui::Checkbox("Build & Gear Inspector Window", &globalShowBuildInspectorWindow);
    ImGui::Checkbox("Detailed Pacing & Benchmark Window", &globalShowPacingMetricsWindow);
    ImGui::Checkbox("Mistake History Log Window", &globalShowMistakeHistoryWindow);

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::TextColored(ImVec4(0.4f, 0.85f, 1.0f, 1.0f), "Addon Shortcuts & Hotkeys:");
    ImGui::BulletText("Toggle Master Control Panel: Alt + Shift + T");
    ImGui::BulletText("Reset Rotation to Beginning: Alt + Shift + R");

    ImGui::Spacing();

    if (ImGui::Button("Reset Rotation Sequence (Start Over)"))
    {
        globalRotationEngine.ResetRotationProgress();
    }

    ImGui::SameLine();

    if (ImGui::Button("Open Master Control Panel"))
    {
        globalShowMasterControlPanel = true;
    }

    ImGui::Spacing();
    ImGui::Separator();

    bool autoDetect = globalRotationEngine.IsAutoDetectionEnabled();
    if (ImGui::Checkbox("Auto-Detect Character & Build (MumbleLink)", &autoDetect))
    {
        globalRotationEngine.SetAutoDetectionEnabled(autoDetect);
    }

    const BuildProfile* optionsActiveProfile = globalRotationEngine.GetActiveProfilePointer();
    if (optionsActiveProfile != nullptr)
    {
        ImGui::Text("Active Profile:");
        ImGui::SameLine();

        const std::vector<BuildProfile>& allProfiles = globalRotationEngine.GetCatalogReference().GetAllProfiles();
        ImGui::SetNextItemWidth(260.0f);
        if (ImGui::BeginCombo("##OptionsActiveProfileCombo", optionsActiveProfile->buildDisplayName.c_str()))
        {
            for (uint32_t profileIndex = 0; profileIndex < allProfiles.size(); profileIndex = profileIndex + 1)
            {
                const BuildProfile& candidateProfile = allProfiles[profileIndex];
                bool matchesSpecialization = (candidateProfile.professionIdentifierNumber == optionsActiveProfile->professionIdentifierNumber) &&
                                              (optionsActiveProfile->specializationIdentifierNumber == 0 || candidateProfile.specializationIdentifierNumber == optionsActiveProfile->specializationIdentifierNumber);

                if (matchesSpecialization)
                {
                    bool isSelected = (profileIndex == globalRotationEngine.GetActiveProfileIndex());
                    if (ImGui::Selectable(candidateProfile.buildDisplayName.c_str(), isSelected))
                    {
                        globalRotationEngine.LoadBuildProfileByIndex(profileIndex);
                    }

                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
            }

            ImGui::Separator();

            for (uint32_t profileIndex = 0; profileIndex < allProfiles.size(); profileIndex = profileIndex + 1)
            {
                const BuildProfile& candidateProfile = allProfiles[profileIndex];
                bool matchesSpecialization = (candidateProfile.professionIdentifierNumber == optionsActiveProfile->professionIdentifierNumber) &&
                                              (optionsActiveProfile->specializationIdentifierNumber == 0 || candidateProfile.specializationIdentifierNumber == optionsActiveProfile->specializationIdentifierNumber);

                if (!matchesSpecialization)
                {
                    bool isSelected = (profileIndex == globalRotationEngine.GetActiveProfileIndex());
                    std::string otherProfileLabel = candidateProfile.buildDisplayName + " (" + candidateProfile.professionName + ")";
                    if (ImGui::Selectable(otherProfileLabel.c_str(), isSelected))
                    {
                        globalRotationEngine.LoadBuildProfileByIndex(profileIndex);
                    }

                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
            }

            ImGui::EndCombo();
        }
    }

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.4f, 0.85f, 1.0f, 1.0f), "Action Bar UI Scale Preset:");
    if (ImGui::SmallButton("Small / Compact (36px)"))
    {
        globalActionBarSlotSize = 36.0f;
        globalActionBarSlotSpacing = 4.0f;
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("Normal (48px)"))
    {
        globalActionBarSlotSize = 48.0f;
        globalActionBarSlotSpacing = 7.0f;
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("Large (54px)"))
    {
        globalActionBarSlotSize = 54.0f;
        globalActionBarSlotSpacing = 8.0f;
    }

    ImGui::Spacing();

    ImGui::Checkbox("Auto-Detect Live Game Ping", &globalAutoDetectLivePing);
    if (globalAutoDetectLivePing)
    {
        ImGui::Text("Network Ping: %.0f ms (Auto-Detected)", globalRotationEngine.GetNetworkLatencyMilliseconds());
    }
    else
    {
        float latencyValue = static_cast<float>(globalRotationEngine.GetNetworkLatencyMilliseconds());
        if (ImGui::SliderFloat("Network Latency (Ping in ms)", &latencyValue, 10.0f, 350.0f, "%.0f ms"))
        {
            globalRotationEngine.SetNetworkLatencyMilliseconds(static_cast<double>(latencyValue));
        }
    }
}

static UINT CoachWindowProcedure(HWND windowHandle, UINT messageIdentifier, WPARAM wordParameter, LPARAM longParameter)
{
    std::string detectedKeyName = "";

    if (messageIdentifier == WM_KEYDOWN || messageIdentifier == WM_SYSKEYDOWN)
    {
        uint32_t virtualKeyCode = static_cast<uint32_t>(wordParameter);

        if (virtualKeyCode == 'T' && (GetKeyState(VK_MENU) & 0x8000) && (GetKeyState(VK_SHIFT) & 0x8000))
        {
            globalShowMasterControlPanel = !globalShowMasterControlPanel;
        }
        else if (virtualKeyCode == 'R' && (GetKeyState(VK_MENU) & 0x8000) && (GetKeyState(VK_SHIFT) & 0x8000))
        {
            globalRotationEngine.ResetRotationProgress();
        }

        detectedKeyName = globalKeybindDetector.TranslateVirtualKeyToString(virtualKeyCode);
    }
    else if (messageIdentifier == WM_XBUTTONDOWN)
    {
        WORD xButton = HIWORD(wordParameter);

        if (xButton == XBUTTON1)
        {
            detectedKeyName = "Mouse4";
        }
        else if (xButton == XBUTTON2)
        {
            detectedKeyName = "Mouse5";
        }
    }
    else if (messageIdentifier == WM_MBUTTONDOWN)
    {
        detectedKeyName = "MiddleMouse";
    }

    if (!detectedKeyName.empty())
    {
        if (!globalSlotWaitingForBinding.empty())
        {
            globalKeybindDetector.SetPrimaryBindingForSlot(globalSlotWaitingForBinding, detectedKeyName);
            globalSlotWaitingForBinding = "";
        }
        else
        {
            globalRotationEngine.ProcessDetectedInput(detectedKeyName, globalKeybindDetector);
        }
    }

    return messageIdentifier;
}

static void OnCombatEvent(void* eventArguments)
{
    if (eventArguments == nullptr)
    {
        return;
    }

    uint32_t* rawData = static_cast<uint32_t*>(eventArguments);
    uint32_t skillIdentifier = rawData[6];
    uint32_t durationMilliseconds = 400;

    globalRotationEngine.ProcessSkillActivation(skillIdentifier, durationMilliseconds);
}

static void AddonLoad(AddonAPI_t* addonApplicationInterface)
{
    globalNexusAddonInterface = addonApplicationInterface;

    globalLastFrameTimestamp = std::chrono::steady_clock::now();

    if (globalNexusAddonInterface != nullptr)
    {
        const char* userProfileDirectory = std::getenv("USERPROFILE");
        if (userProfileDirectory != nullptr)
        {
            std::string userBindsXmlPath = std::string(userProfileDirectory) + "\\Documents\\Guild Wars 2\\InputBinds\\Testing.xml";
            globalKeybindDetector.LoadFromGameBindsXmlFile(userBindsXmlPath);
        }

        const char* gameDirectory = globalNexusAddonInterface->Paths_GetGameDirectory();
        if (gameDirectory != nullptr)
        {
            std::string xmlPath = std::string(gameDirectory) + "\\addons\\Nexus\\GameBinds.xml";
            globalKeybindDetector.LoadFromGameBindsXmlFile(xmlPath);

            std::string resolvedAddonDirectory;
            const char* nexusAddonDirectory = globalNexusAddonInterface->Paths_GetAddonDirectory("Gw2RotationCoach");
            if (nexusAddonDirectory != nullptr)
            {
                std::string candidateBenchPath = std::string(nexusAddonDirectory) + "\\bench";
                DWORD benchAttributes = GetFileAttributesA(candidateBenchPath.c_str());
                if (benchAttributes != INVALID_FILE_ATTRIBUTES && (benchAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {
                    resolvedAddonDirectory = nexusAddonDirectory;
                }
            }

            if (resolvedAddonDirectory.empty() && gameDirectory != nullptr)
            {
                std::string fallbackAddonPath = std::string(gameDirectory) + "\\addons\\Gw2RotationCoach";
                std::string candidateBenchPath = fallbackAddonPath + "\\bench";
                DWORD benchAttributes = GetFileAttributesA(candidateBenchPath.c_str());
                if (benchAttributes != INVALID_FILE_ATTRIBUTES && (benchAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {
                    resolvedAddonDirectory = fallbackAddonPath;
                }
            }

            if (resolvedAddonDirectory.empty())
            {
                char moduleFilePath[MAX_PATH] = {0};
                HMODULE moduleHandle = GetModuleHandleA("Gw2RotationCoach.dll");
                if (moduleHandle != nullptr && GetModuleFileNameA(moduleHandle, moduleFilePath, MAX_PATH) > 0)
                {
                    std::string modulePathString(moduleFilePath);
                    size_t lastBackslash = modulePathString.find_last_of("\\/");
                    if (lastBackslash != std::string::npos)
                    {
                        std::string moduleDirectory = modulePathString.substr(0, lastBackslash);
                        std::string candidatePathOne = moduleDirectory + "\\Gw2RotationCoach";
                        DWORD benchAttributes = GetFileAttributesA((candidatePathOne + "\\bench").c_str());
                        if (benchAttributes != INVALID_FILE_ATTRIBUTES && (benchAttributes & FILE_ATTRIBUTE_DIRECTORY))
                        {
                            resolvedAddonDirectory = candidatePathOne;
                        }
                        else
                        {
                            DWORD directBenchAttributes = GetFileAttributesA((moduleDirectory + "\\bench").c_str());
                            if (directBenchAttributes != INVALID_FILE_ATTRIBUTES && (directBenchAttributes & FILE_ATTRIBUTE_DIRECTORY))
                            {
                                resolvedAddonDirectory = moduleDirectory;
                            }
                        }
                    }
                }
            }

            if (resolvedAddonDirectory.empty())
            {
                resolvedAddonDirectory = (nexusAddonDirectory != nullptr) ? nexusAddonDirectory : (std::string(gameDirectory) + "\\addons\\Gw2RotationCoach");
            }

            globalRotationEngine.GetMutableCatalogReference().SetStandaloneAddonDirectory(resolvedAddonDirectory);
            std::string standaloneBenchPath = resolvedAddonDirectory + "\\bench";
            globalRotationEngine.LoadBenchmarks(standaloneBenchPath);

            std::string configurationFilePath = resolvedAddonDirectory + "\\Gw2RotationCoach.ini";
            DWORD fileAttributes = GetFileAttributesA(configurationFilePath.c_str());
            if (fileAttributes == INVALID_FILE_ATTRIBUTES)
            {
                globalShowMasterControlPanel = true;
                HANDLE createdFileHandle = CreateFileA(configurationFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
                if (createdFileHandle != INVALID_HANDLE_VALUE)
                {
                    CloseHandle(createdFileHandle);
                }
            }
        }

        if (globalNexusAddonInterface->ImguiContext != nullptr)
        {
            ImGui::SetCurrentContext(static_cast<ImGuiContext*>(globalNexusAddonInterface->ImguiContext));

            if (globalNexusAddonInterface->ImguiMalloc != nullptr && globalNexusAddonInterface->ImguiFree != nullptr)
            {
                auto allocationFunction = reinterpret_cast<MemoryAllocationFunction>(globalNexusAddonInterface->ImguiMalloc);
                auto deallocationFunction = reinterpret_cast<MemoryDeallocationFunction>(globalNexusAddonInterface->ImguiFree);
                ImGui::SetAllocatorFunctions(allocationFunction, deallocationFunction, nullptr);
            }
        }

        globalNexusAddonInterface->GUI_Register(RT_Render, RenderCoachUserInterface);
        globalNexusAddonInterface->GUI_Register(RT_OptionsRender, RenderCoachOptionsUserInterface);

        globalNexusAddonInterface->GUI_RegisterCloseOnEscape("Rotation Coach - Master Control", &globalShowMasterControlPanel);
        globalNexusAddonInterface->GUI_RegisterCloseOnEscape("Rotation Ribbon", &globalShowRibbonHud);
        globalNexusAddonInterface->GUI_RegisterCloseOnEscape("Build & Gear Showcase", &globalShowBuildInspectorWindow);
        globalNexusAddonInterface->GUI_RegisterCloseOnEscape("Benchmark Pacing Metrics", &globalShowPacingMetricsWindow);
        globalNexusAddonInterface->GUI_RegisterCloseOnEscape("Rotation Mistake Breakdown", &globalShowMistakeHistoryWindow);

        globalNexusAddonInterface->WndProc_Register(CoachWindowProcedure);

        globalNexusAddonInterface->Events_Subscribe("EV_ARCDPS_COMBATEVENT", OnCombatEvent);
    }
}

static void AddonUnload()
{
    if (globalNexusAddonInterface != nullptr)
    {
        globalNexusAddonInterface->Events_Unsubscribe("EV_ARCDPS_COMBATEVENT", OnCombatEvent);

        globalNexusAddonInterface->WndProc_Deregister(CoachWindowProcedure);

        globalNexusAddonInterface->GUI_DeregisterCloseOnEscape("Rotation Mistake Breakdown");
        globalNexusAddonInterface->GUI_DeregisterCloseOnEscape("Benchmark Pacing Metrics");
        globalNexusAddonInterface->GUI_DeregisterCloseOnEscape("Build & Gear Showcase");
        globalNexusAddonInterface->GUI_DeregisterCloseOnEscape("Rotation Ribbon");
        globalNexusAddonInterface->GUI_DeregisterCloseOnEscape("Rotation Coach - Master Control");

        globalNexusAddonInterface->GUI_Deregister(RenderCoachOptionsUserInterface);
        globalNexusAddonInterface->GUI_Deregister(RenderCoachUserInterface);
    }

    globalNexusAddonInterface = nullptr;
}

static AddonDefinition_t coachAddonDefinition = {
    0x524F5443,
    NEXUS_API_VERSION,
    "Rotation Coach",
    {0, 526, 0, 0},
    "Oneleaf525",
    "Real-time rotation trainer, benchmark pacing analyzer, and mistake detector",
    AddonLoad,
    AddonUnload,
    AF_None,
    UP_GitHub,
    "https://github.com/DebnityaDuttaGupta/GW2-RaidRotationCoach"
};

extern "C" __declspec(dllexport) AddonDefinition_t* GetAddonDef()
{
    return &coachAddonDefinition;
}
