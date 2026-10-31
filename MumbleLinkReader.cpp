#include "MumbleLinkReader.h"

MumbleLinkReader::MumbleLinkReader()
{
    fileMappingHandle = nullptr;
    linkedMemoryPointer = nullptr;
}

MumbleLinkReader::~MumbleLinkReader()
{
    Shutdown();
}

bool MumbleLinkReader::Initialize()
{
    if (linkedMemoryPointer != nullptr)
    {
        return true;
    }

    fileMappingHandle = OpenFileMappingA(FILE_MAP_READ, FALSE, "MumbleLink");

    if (fileMappingHandle == nullptr)
    {
        return false;
    }

    linkedMemoryPointer = static_cast<const MumbleLinkedMemory*>(MapViewOfFile(fileMappingHandle, FILE_MAP_READ, 0, 0, sizeof(MumbleLinkedMemory)));

    if (linkedMemoryPointer == nullptr)
    {
        CloseHandle(fileMappingHandle);
        fileMappingHandle = nullptr;
        return false;
    }

    return true;
}

void MumbleLinkReader::Shutdown()
{
    if (linkedMemoryPointer != nullptr)
    {
        UnmapViewOfFile(linkedMemoryPointer);
        linkedMemoryPointer = nullptr;
    }

    if (fileMappingHandle != nullptr)
    {
        CloseHandle(fileMappingHandle);
        fileMappingHandle = nullptr;
    }
}

bool MumbleLinkReader::QueryCharacterIdentity(uint32_t& detectedProfessionNumber, uint32_t& detectedSpecializationNumber)
{
    if (linkedMemoryPointer == nullptr)
    {
        if (!Initialize())
        {
            return false;
        }
    }

    std::wstring identityWideString(linkedMemoryPointer->characterIdentityJson);

    if (identityWideString.empty())
    {
        return false;
    }

    return ParseProfessionAndSpecialization(identityWideString, detectedProfessionNumber, detectedSpecializationNumber);
}

bool MumbleLinkReader::IsInActiveGameplay()
{
    if (linkedMemoryPointer == nullptr)
    {
        if (!Initialize())
        {
            return false;
        }
    }

    if (linkedMemoryPointer->contextLength == 0)
    {
        return false;
    }

    if (linkedMemoryPointer->characterIdentityJson[0] == L'\0')
    {
        return false;
    }

    if (linkedMemoryPointer->contextLength >= 32)
    {
        const uint32_t* integerData = reinterpret_cast<const uint32_t*>(linkedMemoryPointer->contextData + 28);
        uint32_t currentMapIdentifier = integerData[0];

        if (currentMapIdentifier == 0)
        {
            return false;
        }
    }

    return true;
}

bool MumbleLinkReader::ParseProfessionAndSpecialization(const std::wstring& jsonString, uint32_t& outProfession, uint32_t& outSpecialization)
{
    size_t professionKeyPosition = jsonString.find(L"\"profession\":");

    if (professionKeyPosition == std::wstring::npos)
    {
        professionKeyPosition = jsonString.find(L"\"profession\": ");

        if (professionKeyPosition == std::wstring::npos)
        {
            return false;
        }
    }

    size_t colonPosition = jsonString.find(L':', professionKeyPosition);

    if (colonPosition == std::wstring::npos)
    {
        return false;
    }

    size_t commaPosition = jsonString.find(L',', colonPosition);

    if (commaPosition == std::wstring::npos)
    {
        return false;
    }

    std::wstring professionSubString = jsonString.substr(colonPosition + 1, commaPosition - (colonPosition + 1));
    outProfession = static_cast<uint32_t>(std::stoul(professionSubString));

    size_t specializationKeyPosition = jsonString.find(L"\"spec\":");

    if (specializationKeyPosition == std::wstring::npos)
    {
        specializationKeyPosition = jsonString.find(L"\"spec\": ");

        if (specializationKeyPosition == std::wstring::npos)
        {
            return false;
        }
    }

    size_t specColonPosition = jsonString.find(L':', specializationKeyPosition);

    if (specColonPosition == std::wstring::npos)
    {
        return false;
    }

    size_t specEndPosition = jsonString.find_first_of(L",}", specColonPosition);

    if (specEndPosition == std::wstring::npos)
    {
        return false;
    }

    std::wstring specializationSubString = jsonString.substr(specColonPosition + 1, specEndPosition - (specColonPosition + 1));
    outSpecialization = static_cast<uint32_t>(std::stoul(specializationSubString));

    return true;
}
