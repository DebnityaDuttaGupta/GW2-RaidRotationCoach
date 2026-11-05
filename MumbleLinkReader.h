#ifndef MUMBLE_LINK_READER_HEADER
#define MUMBLE_LINK_READER_HEADER

#include <windows.h>
#include <string>
#include <cstdint>

struct MumbleLinkedMemory
{
    uint32_t versionNumber;
    uint32_t updateTickNumber;
    float avatarPosition[3];
    float avatarFrontVector[3];
    float avatarTopVector[3];
    wchar_t avatarName[256];
    float cameraPosition[3];
    float cameraFrontVector[3];
    float cameraTopVector[3];
    wchar_t characterIdentityJson[256];
    uint32_t contextLength;
    uint8_t contextData[256];
    wchar_t descriptionText[2048];
};

class MumbleLinkReader
{
public:
    MumbleLinkReader();
    ~MumbleLinkReader();

    bool Initialize();

    void Shutdown();

    bool QueryCharacterIdentity(uint32_t& detectedProfessionNumber, uint32_t& detectedSpecializationNumber);

    bool IsInActiveGameplay();

    uint32_t GetCurrentMapIdentifier();

    uint32_t GetCurrentMapType();

    bool IsInCompetitiveGameMode();

    bool IsInSpecialForcesTrainingArea();

private:
    HANDLE fileMappingHandle;
    const MumbleLinkedMemory* linkedMemoryPointer;

    bool ParseProfessionAndSpecialization(const std::wstring& jsonString, uint32_t& outProfession, uint32_t& outSpecialization);
};

#endif
