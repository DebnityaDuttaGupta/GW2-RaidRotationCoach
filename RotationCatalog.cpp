#include "RotationCatalog.h"
#include <filesystem>
#include <fstream>
#include <sstream>

RotationCatalog::RotationCatalog()
{
    PopulateDefaultSkillMappings();

    standaloneAddonDirectoryPath = "E:\\SteamLibrary\\steamapps\\common\\Guild Wars 2\\addons\\Gw2RotationCoach";

    std::string skillsFilePath = standaloneAddonDirectoryPath + "\\skills\\gw2_skills_en.json";
    if (std::filesystem::exists(skillsFilePath))
    {
        LoadMasterSkillDatabase(skillsFilePath);
    }

    std::string standaloneBenchPath = standaloneAddonDirectoryPath + "\\bench";
    if (std::filesystem::exists(standaloneBenchPath))
    {
        LoadBenchmarkDirectory(standaloneBenchPath);
    }
}

void RotationCatalog::PopulateDefaultSkillMappings()
{
    skillIdentifierToIconFileNameMap[2479383] = "2479383.png";
    skillIdentifierToIconFileNameMap[62616] = "2479383.png";
    skillIdentifierToIconFileNameMap[2479382] = "2479382.png";
    skillIdentifierToIconFileNameMap[62568] = "2479383.png";
    skillIdentifierToIconFileNameMap[2479379] = "2479379.png";
    skillIdentifierToIconFileNameMap[2479391] = "2479391.png";
    skillIdentifierToIconFileNameMap[103723] = "103723.png";
    skillIdentifierToIconFileNameMap[10214] = "103723.png";
    skillIdentifierToIconFileNameMap[2479389] = "2479389.png";
    skillIdentifierToIconFileNameMap[2479385] = "2479385.png";
    skillIdentifierToIconFileNameMap[103768] = "103768.png";
    skillIdentifierToIconFileNameMap[10267] = "103768.png";
    skillIdentifierToIconFileNameMap[699528] = "699528.png";
    skillIdentifierToIconFileNameMap[10211] = "699528.png";
    skillIdentifierToIconFileNameMap[103756] = "103756.png";
    skillIdentifierToIconFileNameMap[2479384] = "2479384.png";
    skillIdentifierToIconFileNameMap[2479381] = "2479381.png";
    skillIdentifierToIconFileNameMap[103284] = "103284.png";

    skillIdentifierToIconFileNameMap[40915] = "1770490.png";
    skillIdentifierToIconFileNameMap[41258] = "1770490.png";
    skillIdentifierToIconFileNameMap[40963] = "1770463.png";
    skillIdentifierToIconFileNameMap[41259] = "1770463.png";
    skillIdentifierToIconFileNameMap[9093] = "103268.png";
    skillIdentifierToIconFileNameMap[9094] = "103636.png";
    skillIdentifierToIconFileNameMap[68648] = "2779164.png";
    skillIdentifierToIconFileNameMap[45210] = "2779164.png";
    skillIdentifierToIconFileNameMap[1770485] = "1770485.png";
    skillIdentifierToIconFileNameMap[45211] = "1770485.png";
    skillIdentifierToIconFileNameMap[1770486] = "1770486.png";
    skillIdentifierToIconFileNameMap[45212] = "1770486.png";
    skillIdentifierToIconFileNameMap[9152] = "103702.png";
    skillIdentifierToIconFileNameMap[60975] = "60975.png";
    skillIdentifierToIconFileNameMap[9999] = "9999.png";
    skillIdentifierToIconFileNameMap[9110] = "103655.png";
    skillIdentifierToIconFileNameMap[9115] = "103671.png";
    skillIdentifierToIconFileNameMap[9118] = "103638.png";
    skillIdentifierToIconFileNameMap[9122] = "103662.png";
    skillIdentifierToIconFileNameMap[9119] = "103643.png";

    skillNameToIconFileNameMap["Bladesong Sorrow"] = "2479383.png";
    skillNameToIconFileNameMap["Bladesong Sorrow (F2)"] = "2479383.png";
    skillNameToIconFileNameMap["Bladesong Harmony"] = "2479382.png";
    skillNameToIconFileNameMap["Bladesong Harmony (F1)"] = "2479382.png";
    skillNameToIconFileNameMap["Bladecall"] = "2479379.png";
    skillNameToIconFileNameMap["Bladecall (Dagger 2)"] = "2479379.png";
    skillNameToIconFileNameMap["Unstable Bladestorm"] = "2479391.png";
    skillNameToIconFileNameMap["Unstable Bladestorm (Dagger 3)"] = "2479391.png";
    skillNameToIconFileNameMap["Unrelenting Bladestorm"] = "2479391.png";
    skillNameToIconFileNameMap["Unrelenting Bladestorm (Dagger 3)"] = "2479391.png";
    skillNameToIconFileNameMap["Flying Cutter"] = "2479385.png";
    skillNameToIconFileNameMap["Flying Cutter (Dagger 1)"] = "2479385.png";
    skillNameToIconFileNameMap["Phantasmal Swordsman"] = "103723.png";
    skillNameToIconFileNameMap["Phantasmal Swordsman (Sword 5)"] = "103723.png";
    skillNameToIconFileNameMap["Thousand Cuts"] = "2479389.png";
    skillNameToIconFileNameMap["Phantasmal Warden"] = "103768.png";
    skillNameToIconFileNameMap["Phantasmal Warden (Focus 4)"] = "103768.png";
    skillNameToIconFileNameMap["Signet of the Ether"] = "699528.png";
    skillNameToIconFileNameMap["Signet of Illusions"] = "103756.png";
    skillNameToIconFileNameMap["Bladeturn Requiem"] = "2479384.png";
    skillNameToIconFileNameMap["Bladeturn Requiem (F4)"] = "2479384.png";
    skillNameToIconFileNameMap["Bladesong Dissonance"] = "2479381.png";
    skillNameToIconFileNameMap["Bladesong Dissonance (F3)"] = "2479381.png";
    skillNameToIconFileNameMap["Bladesong Distortion"] = "103284.png";
    skillNameToIconFileNameMap["Bladesong Distortion (F5)"] = "103284.png";

    skillNameToIconFileNameMap["Mantra of Potence"] = "1770490.png";
    skillNameToIconFileNameMap["Mantra of Solace"] = "1770463.png";
    skillNameToIconFileNameMap["Symbol of Swiftness"] = "103268.png";
    skillNameToIconFileNameMap["Symbol of Swiftness (Staff 3)"] = "103268.png";
    skillNameToIconFileNameMap["Empower"] = "103636.png";
    skillNameToIconFileNameMap["Empower (Staff 4)"] = "103636.png";
    skillNameToIconFileNameMap["Tome of Resolve"] = "2779164.png";
    skillNameToIconFileNameMap["Tome of Resolve (F2)"] = "2779164.png";
    skillNameToIconFileNameMap["Tome of Courage (F3)"] = "2779164.png";
    skillNameToIconFileNameMap["Chapter 3: Valiant Bulwark"] = "1770485.png";
    skillNameToIconFileNameMap["Chapter 4: Stalwart Stand"] = "1770486.png";
    skillNameToIconFileNameMap["Feel My Wrath!"] = "103702.png";
    skillNameToIconFileNameMap["\"Feel My Wrath!\""] = "103702.png";
    skillNameToIconFileNameMap["Stow Tome (F2)"] = "60975.png";
    skillNameToIconFileNameMap["Stow Tome (F3)"] = "60975.png";
    skillNameToIconFileNameMap["Stow Tome"] = "60975.png";
    skillNameToIconFileNameMap["Weapon Swap"] = "9999.png";
    skillNameToIconFileNameMap["Symbol of Resolution"] = "103655.png";
    skillNameToIconFileNameMap["Shield of Judgment"] = "103671.png";
    skillNameToIconFileNameMap["Symbol of Blades (Sword 2)"] = "103638.png";
    skillNameToIconFileNameMap["Ray of Judgment (Focus 4)"] = "103662.png";
    skillNameToIconFileNameMap["Zealot's Defense (Sword 3)"] = "103643.png";

    skillNameToSlotCategoryMap["Bladesong Harmony"] = SkillSlotCategory::ProfessionSkillOne;
    skillNameToSlotCategoryMap["Bladesong Sorrow"] = SkillSlotCategory::ProfessionSkillTwo;
    skillNameToSlotCategoryMap["Bladesong Dissonance"] = SkillSlotCategory::ProfessionSkillThree;
    skillNameToSlotCategoryMap["Bladeturn Requiem"] = SkillSlotCategory::ProfessionSkillFour;
    skillNameToSlotCategoryMap["Bladesong Distortion"] = SkillSlotCategory::ProfessionSkillFive;
    skillNameToSlotCategoryMap["Flying Cutter"] = SkillSlotCategory::WeaponSkillOne;
    skillNameToSlotCategoryMap["Bladecall"] = SkillSlotCategory::WeaponSkillTwo;
    skillNameToSlotCategoryMap["Unstable Bladestorm"] = SkillSlotCategory::WeaponSkillThree;
    skillNameToSlotCategoryMap["Unrelenting Bladestorm"] = SkillSlotCategory::WeaponSkillThree;
    skillNameToSlotCategoryMap["Illusionary Riposte"] = SkillSlotCategory::WeaponSkillFour;
    skillNameToSlotCategoryMap["Phantasmal Warden"] = SkillSlotCategory::WeaponSkillFour;
    skillNameToSlotCategoryMap["Phantasmal Swordsman"] = SkillSlotCategory::WeaponSkillFive;
    skillNameToSlotCategoryMap["Temporal Curtain"] = SkillSlotCategory::WeaponSkillFive;
    skillNameToSlotCategoryMap["Into the Void"] = SkillSlotCategory::WeaponSkillFive;
    skillNameToSlotCategoryMap["Spatial Surge"] = SkillSlotCategory::WeaponSkillOne;
    skillNameToSlotCategoryMap["Mirror Blade"] = SkillSlotCategory::WeaponSkillTwo;
    skillNameToSlotCategoryMap["Mind Stab"] = SkillSlotCategory::WeaponSkillThree;
    skillNameToSlotCategoryMap["Phantasmal Berserker"] = SkillSlotCategory::WeaponSkillFour;
    skillNameToSlotCategoryMap["Illusionary Wave"] = SkillSlotCategory::WeaponSkillFive;
    skillNameToSlotCategoryMap["Power Spike"] = SkillSlotCategory::UtilitySkillOne;
    skillNameToSlotCategoryMap["Mantra of Pain"] = SkillSlotCategory::UtilitySkillOne;
    skillNameToSlotCategoryMap["Rain of Swords"] = SkillSlotCategory::UtilitySkillTwo;
    skillNameToSlotCategoryMap["Phantasmal Disenchanter"] = SkillSlotCategory::UtilitySkillThree;
    skillNameToSlotCategoryMap["Power Lock"] = SkillSlotCategory::UtilitySkillThree;
    skillNameToSlotCategoryMap["Mantra of Distraction"] = SkillSlotCategory::UtilitySkillThree;
    skillNameToSlotCategoryMap["Mantra of Recovery"] = SkillSlotCategory::HealingSkill;
    skillNameToSlotCategoryMap["Signet of the Ether"] = SkillSlotCategory::HealingSkill;
    skillNameToSlotCategoryMap["Signet of Illusions"] = SkillSlotCategory::UtilitySkillOne;
    skillNameToSlotCategoryMap["Thousand Cuts"] = SkillSlotCategory::EliteSkill;

    skillNameToSlotCategoryMap["Tome of Justice"] = SkillSlotCategory::ProfessionSkillOne;
    skillNameToSlotCategoryMap["Tome of Resolve"] = SkillSlotCategory::ProfessionSkillTwo;
    skillNameToSlotCategoryMap["Tome of Courage"] = SkillSlotCategory::ProfessionSkillThree;
    skillNameToSlotCategoryMap["Symbol of Blades"] = SkillSlotCategory::WeaponSkillTwo;
    skillNameToSlotCategoryMap["Zealot's Defense"] = SkillSlotCategory::WeaponSkillThree;
    skillNameToSlotCategoryMap["Symbol of Swiftness"] = SkillSlotCategory::WeaponSkillThree;
    skillNameToSlotCategoryMap["Ray of Judgment"] = SkillSlotCategory::WeaponSkillFour;
    skillNameToSlotCategoryMap["Empower"] = SkillSlotCategory::WeaponSkillFour;
    skillNameToSlotCategoryMap["Symbol of Resolution"] = SkillSlotCategory::WeaponSkillFour;
    skillNameToSlotCategoryMap["Shield of Judgment"] = SkillSlotCategory::WeaponSkillFive;

    skillNameToSlotCategoryMap["Fire Attunement"] = SkillSlotCategory::ProfessionSkillOne;
    skillNameToSlotCategoryMap["Water Attunement"] = SkillSlotCategory::ProfessionSkillTwo;
    skillNameToSlotCategoryMap["Air Attunement"] = SkillSlotCategory::ProfessionSkillThree;
    skillNameToSlotCategoryMap["Earth Attunement"] = SkillSlotCategory::ProfessionSkillFour;
    skillNameToSlotCategoryMap["Deploy Jade Sphere"] = SkillSlotCategory::ProfessionSkillFive;
    skillNameToSlotCategoryMap["Deploy Jade Sphere (F5)"] = SkillSlotCategory::ProfessionSkillFive;
}

uint32_t RotationCatalog::GetProfileCount() const
{
    return static_cast<uint32_t>(availableBuildProfiles.size());
}

const BuildProfile* RotationCatalog::GetProfilePointerByIndex(uint32_t requestedProfileIndex) const
{
    if (requestedProfileIndex < availableBuildProfiles.size())
    {
        return &availableBuildProfiles[requestedProfileIndex];
    }

    return nullptr;
}

const BuildProfile* RotationCatalog::FindProfileByIdentifier(const std::string& requestedProfileIdentifier) const
{
    for (const auto& profileReference : availableBuildProfiles)
    {
        if (profileReference.profileIdentifier == requestedProfileIdentifier)
        {
            return &profileReference;
        }
    }

    return nullptr;
}

std::string RotationCatalog::GetSkillIconFileName(uint32_t skillIdentifier) const
{
    auto iterator = skillIdentifierToIconFileNameMap.find(skillIdentifier);

    if (iterator != skillIdentifierToIconFileNameMap.end())
    {
        return iterator->second;
    }

    return "";
}

std::string RotationCatalog::FindSkillIconByName(const std::string& skillName) const
{
    if (skillName.empty())
    {
        return "";
    }

    auto exactIterator = skillNameToIconFileNameMap.find(skillName);
    if (exactIterator != skillNameToIconFileNameMap.end())
    {
        return exactIterator->second;
    }

    std::string cleanedSkillName = skillName;
    size_t parenthesisPosition = cleanedSkillName.find('(');
    if (parenthesisPosition != std::string::npos)
    {
        cleanedSkillName = cleanedSkillName.substr(0, parenthesisPosition);
        while (!cleanedSkillName.empty() && cleanedSkillName.back() == ' ')
        {
            cleanedSkillName.pop_back();
        }
    }

    auto cleanedExactIterator = skillNameToIconFileNameMap.find(cleanedSkillName);
    if (cleanedExactIterator != skillNameToIconFileNameMap.end())
    {
        return cleanedExactIterator->second;
    }

    std::string lowercaseCleanedName = cleanedSkillName;
    std::transform(lowercaseCleanedName.begin(), lowercaseCleanedName.end(), lowercaseCleanedName.begin(), ::tolower);

    for (const auto& mappingEntry : skillNameToIconFileNameMap)
    {
        std::string lowercaseCatalogName = mappingEntry.first;
        std::transform(lowercaseCatalogName.begin(), lowercaseCatalogName.end(), lowercaseCatalogName.begin(), ::tolower);

        if (lowercaseCleanedName == lowercaseCatalogName)
        {
            return mappingEntry.second;
        }
    }

    for (const auto& mappingEntry : skillNameToIconFileNameMap)
    {
        std::string lowercaseCatalogName = mappingEntry.first;
        std::transform(lowercaseCatalogName.begin(), lowercaseCatalogName.end(), lowercaseCatalogName.begin(), ::tolower);

        if (lowercaseCatalogName.length() >= 4 && lowercaseCleanedName.find(lowercaseCatalogName) != std::string::npos)
        {
            return mappingEntry.second;
        }
    }

    return "";
}

void RotationCatalog::SetStandaloneAddonDirectory(const std::string& directoryPath)
{
    standaloneAddonDirectoryPath = directoryPath;
}

const std::string& RotationCatalog::GetStandaloneAddonDirectory() const
{
    return standaloneAddonDirectoryPath;
}

const SkillDefinitionMetadata* RotationCatalog::FindSkillMetadata(uint32_t skillIdentifier) const
{
    auto iterator = masterSkillDatabase.find(skillIdentifier);

    if (iterator != masterSkillDatabase.end())
    {
        return &iterator->second;
    }

    return nullptr;
}

bool RotationCatalog::LoadMasterSkillDatabase(const std::string& skillsJsonFilePath)
{
    std::ifstream fileStream(skillsJsonFilePath);
    if (!fileStream.is_open())
    {
        return false;
    }

    std::stringstream bufferStream;
    bufferStream << fileStream.rdbuf();
    std::string fileContent = bufferStream.str();

    size_t scanPosition = 0;

    while (scanPosition < fileContent.size())
    {
        size_t idPosition = fileContent.find("\"id\":", scanPosition);
        if (idPosition == std::string::npos)
        {
            break;
        }

        size_t commaAfterId = fileContent.find(',', idPosition);
        if (commaAfterId == std::string::npos)
        {
            break;
        }

        std::string idText = fileContent.substr(idPosition + 5, commaAfterId - (idPosition + 5));
        uint32_t skillIdentifier = 0;
        try
        {
            skillIdentifier = static_cast<uint32_t>(std::stoul(idText));
        }
        catch (...)
        {
            scanPosition = commaAfterId + 1;
            continue;
        }

        if (skillIdentifier == 0)
        {
            scanPosition = commaAfterId + 1;
            continue;
        }

        size_t blockStart = fileContent.rfind('{', idPosition);
        size_t blockEnd = fileContent.find('}', idPosition);
        if (blockStart == std::string::npos || blockEnd == std::string::npos || blockEnd <= blockStart)
        {
            scanPosition = commaAfterId + 1;
            continue;
        }

        std::string skillBlock = fileContent.substr(blockStart, blockEnd - blockStart + 1);

        SkillDefinitionMetadata metadata;
        metadata.skillIdentifier = skillIdentifier;
        metadata.rechargeSeconds = 0;
        metadata.isAutoAttack = false;
        metadata.slotCategory = SkillSlotCategory::WeaponSkillOne;

        size_t namePos = skillBlock.find("\"name\":");
        if (namePos != std::string::npos)
        {
            size_t firstQuote = skillBlock.find('\"', namePos + 7);
            if (firstQuote != std::string::npos)
            {
                size_t secondQuote = skillBlock.find('\"', firstQuote + 1);
                if (secondQuote != std::string::npos)
                {
                    metadata.skillName = skillBlock.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
                }
            }
        }

        uint32_t renderIdentifier = 0;

        size_t iconPos = skillBlock.find("\"icon\":");
        if (iconPos != std::string::npos)
        {
            size_t firstQuote = skillBlock.find('\"', iconPos + 7);
            if (firstQuote != std::string::npos)
            {
                size_t secondQuote = skillBlock.find('\"', firstQuote + 1);
                if (secondQuote != std::string::npos)
                {
                    std::string iconUrl = skillBlock.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
                    size_t lastSlash = iconUrl.find_last_of('/');
                    if (lastSlash != std::string::npos)
                    {
                        metadata.iconFileName = iconUrl.substr(lastSlash + 1);
                        skillIdentifierToIconFileNameMap[skillIdentifier] = metadata.iconFileName;

                        if (!metadata.skillName.empty())
                        {
                            skillNameToIconFileNameMap[metadata.skillName] = metadata.iconFileName;
                        }

                        size_t dotPos = metadata.iconFileName.find(".png");
                        if (dotPos != std::string::npos)
                        {
                            try
                            {
                                renderIdentifier = static_cast<uint32_t>(std::stoul(metadata.iconFileName.substr(0, dotPos)));
                                skillIdentifierToIconFileNameMap[renderIdentifier] = metadata.iconFileName;
                            }
                            catch (...)
                            {
                            }
                        }
                    }
                }
            }
        }

        size_t skillTypePosition = skillBlock.find("\"skill_type\":");
        if (skillTypePosition != std::string::npos)
        {
            size_t firstQuote = skillBlock.find('\"', skillTypePosition + 13);
            if (firstQuote != std::string::npos)
            {
                size_t secondQuote = skillBlock.find('\"', firstQuote + 1);
                if (secondQuote != std::string::npos)
                {
                    std::string skillTypeString = skillBlock.substr(firstQuote + 1, secondQuote - (firstQuote + 1));

                    if (skillTypeString == "1")
                    {
                        metadata.slotCategory = SkillSlotCategory::WeaponSkillOne;
                    }
                    else if (skillTypeString == "2")
                    {
                        metadata.slotCategory = SkillSlotCategory::WeaponSkillTwo;
                    }
                    else if (skillTypeString == "3")
                    {
                        metadata.slotCategory = SkillSlotCategory::WeaponSkillThree;
                    }
                    else if (skillTypeString == "4")
                    {
                        metadata.slotCategory = SkillSlotCategory::WeaponSkillFour;
                    }
                    else if (skillTypeString == "5")
                    {
                        metadata.slotCategory = SkillSlotCategory::WeaponSkillFive;
                    }
                    else if (skillTypeString == "6")
                    {
                        metadata.slotCategory = SkillSlotCategory::HealingSkill;
                    }
                    else if (skillTypeString == "7")
                    {
                        metadata.slotCategory = SkillSlotCategory::UtilitySkillOne;
                    }
                    else if (skillTypeString == "10")
                    {
                        metadata.slotCategory = SkillSlotCategory::EliteSkill;
                    }
                    else if (skillTypeString == "11")
                    {
                        metadata.slotCategory = SkillSlotCategory::ProfessionSkillOne;
                    }
                    else if (skillTypeString == "12")
                    {
                        metadata.slotCategory = SkillSlotCategory::ProfessionSkillTwo;
                    }
                    else if (skillTypeString == "13")
                    {
                        metadata.slotCategory = SkillSlotCategory::ProfessionSkillThree;
                    }
                    else if (skillTypeString == "14")
                    {
                        metadata.slotCategory = SkillSlotCategory::ProfessionSkillFour;
                    }
                    else if (skillTypeString == "15")
                    {
                        metadata.slotCategory = SkillSlotCategory::ProfessionSkillFive;
                    }
                }
            }
        }
        else if (skillBlock.find("\"is_heal_skill\": true") != std::string::npos)
        {
            metadata.slotCategory = SkillSlotCategory::HealingSkill;
        }
        else if (skillBlock.find("\"is_elite_skill\": true") != std::string::npos)
        {
            metadata.slotCategory = SkillSlotCategory::EliteSkill;
        }
        else if (skillBlock.find("\"is_utility_skill\": true") != std::string::npos)
        {
            metadata.slotCategory = SkillSlotCategory::UtilitySkillOne;
        }
        else if (skillBlock.find("\"is_profession_skill\": true") != std::string::npos)
        {
            metadata.slotCategory = SkillSlotCategory::ProfessionSkillOne;
        }
        else if (skillBlock.find("\"is_weapon_skill\": true") != std::string::npos)
        {
            metadata.slotCategory = SkillSlotCategory::WeaponSkillOne;
        }

        size_t rechargePos = skillBlock.find("\"recharge\":");
        if (rechargePos != std::string::npos)
        {
            size_t rechargeComma = skillBlock.find_first_of(",}", rechargePos);
            if (rechargeComma != std::string::npos)
            {
                std::string rechargeText = skillBlock.substr(rechargePos + 11, rechargeComma - (rechargePos + 11));
                try
                {
                    metadata.rechargeSeconds = static_cast<uint32_t>(std::stoul(rechargeText));
                }
                catch (...) {}
            }
        }

        if (skillBlock.find("\"is_auto_attack\": true") != std::string::npos)
        {
            metadata.isAutoAttack = true;
        }

        masterSkillDatabase[skillIdentifier] = metadata;

        if (!metadata.skillName.empty())
        {
            if (metadata.skillName != "Bladeturn Requiem" && metadata.skillName != "Bladesong Distortion" && metadata.skillName != "Distortion" &&
                metadata.skillName.find("Attunement") == std::string::npos && metadata.skillName.find("Jade Sphere") == std::string::npos)
            {
                skillNameToSlotCategoryMap[metadata.skillName] = metadata.slotCategory;
            }
        }

        if (renderIdentifier > 0)
        {
            auto existingIterator = masterSkillDatabase.find(renderIdentifier);
            if (existingIterator == masterSkillDatabase.end() || metadata.rechargeSeconds > existingIterator->second.rechargeSeconds)
            {
                masterSkillDatabase[renderIdentifier] = metadata;
            }
        }

        scanPosition = blockEnd + 1;
    }

    return !masterSkillDatabase.empty();
}

void RotationCatalog::SyncRotationsFromExternalSource(const std::string& sourceDirectoryPath, const std::string& targetDirectoryPath)
{
    std::error_code errorCode;

    if (!std::filesystem::exists(sourceDirectoryPath, errorCode))
    {
        return;
    }

    if (!std::filesystem::exists(targetDirectoryPath, errorCode))
    {
        std::filesystem::create_directories(targetDirectoryPath, errorCode);
    }

    std::filesystem::copy(sourceDirectoryPath, targetDirectoryPath, std::filesystem::copy_options::recursive | std::filesystem::copy_options::update_existing, errorCode);

    LoadBenchmarkDirectory(targetDirectoryPath);
}

uint32_t RotationCatalog::FindMatchingProfileIndex(uint32_t professionIdentifier, uint32_t specializationIdentifier) const
{
    if (professionIdentifier == 7 && (specializationIdentifier == 66 || specializationIdentifier == 0))
    {
        for (uint32_t profileIndex = 0; profileIndex < availableBuildProfiles.size(); profileIndex = profileIndex + 1)
        {
            const auto& profile = availableBuildProfiles[profileIndex];
            if (profile.professionIdentifierNumber == 7 && profile.specializationIdentifierNumber == 66)
            {
                if (profile.damageCategory == DamageTypeCategory::ConditionDamage || profile.buildDisplayName.find("Condition") != std::string::npos)
                {
                    return profileIndex;
                }
            }
        }
    }

    for (uint32_t profileIndex = 0; profileIndex < availableBuildProfiles.size(); profileIndex = profileIndex + 1)
    {
        const auto& profile = availableBuildProfiles[profileIndex];

        if (profile.professionIdentifierNumber == professionIdentifier && profile.specializationIdentifierNumber == specializationIdentifier)
        {
            return profileIndex;
        }
    }

    for (uint32_t profileIndex = 0; profileIndex < availableBuildProfiles.size(); profileIndex = profileIndex + 1)
    {
        if (availableBuildProfiles[profileIndex].professionIdentifierNumber == professionIdentifier)
        {
            return profileIndex;
        }
    }

    return 0;
}

ProfessionColorRgba RotationCatalog::GetProfessionThemeColor(const std::string& professionName) const
{
    if (professionName == "Guardian")
    {
        return {0.20f, 0.65f, 0.85f, 1.0f};
    }

    if (professionName == "Warrior")
    {
        return {1.00f, 0.60f, 0.15f, 1.0f};
    }

    if (professionName == "Engineer")
    {
        return {0.65f, 0.42f, 0.20f, 1.0f};
    }

    if (professionName == "Ranger")
    {
        return {0.40f, 0.80f, 0.25f, 1.0f};
    }

    if (professionName == "Thief")
    {
        return {0.80f, 0.38f, 0.38f, 1.0f};
    }

    if (professionName == "Elementalist")
    {
        return {0.95f, 0.25f, 0.25f, 1.0f};
    }

    if (professionName == "Mesmer")
    {
        return {0.68f, 0.25f, 0.72f, 1.0f};
    }

    if (professionName == "Necromancer")
    {
        return {0.20f, 0.72f, 0.42f, 1.0f};
    }

    if (professionName == "Revenant")
    {
        return {0.75f, 0.22f, 0.22f, 1.0f};
    }

    return {0.8f, 0.8f, 0.8f, 1.0f};
}

std::string RotationCatalog::FormatBuildRoleName(BuildRoleCategory roleCategory) const
{
    switch (roleCategory)
    {
        case BuildRoleCategory::PureDamagePerSecond:
            return "Pure Damage Per Second";
        case BuildRoleCategory::BoonAlacrity:
            return "Boon Support (Alacrity)";
        case BuildRoleCategory::BoonQuickness:
            return "Boon Support (Quickness)";
        case BuildRoleCategory::HealSupport:
            return "Heal Support";
        case BuildRoleCategory::HybridSupportDamage:
            return "Hybrid Support & Damage";
        default:
            break;
    }

    return "Standard Role";
}

std::string RotationCatalog::FormatDamageTypeName(DamageTypeCategory damageCategory) const
{
    switch (damageCategory)
    {
        case DamageTypeCategory::PowerDamage:
            return "Power Damage";
        case DamageTypeCategory::ConditionDamage:
            return "Condition Damage";
        case DamageTypeCategory::CelestialHybrid:
            return "Celestial / Hybrid Damage";
        default:
            break;
    }

    return "Standard Damage";
}

const std::vector<BuildProfile>& RotationCatalog::GetAllProfiles() const
{
    return availableBuildProfiles;
}

bool RotationCatalog::LoadBenchmarkDirectory(const std::string& directoryPath)
{
    std::error_code errorCode;

    if (!std::filesystem::exists(directoryPath, errorCode) || !std::filesystem::is_directory(directoryPath, errorCode))
    {
        return false;
    }

    for (const auto& directoryEntry : std::filesystem::recursive_directory_iterator(directoryPath, errorCode))
    {
        if (directoryEntry.is_regular_file() && directoryEntry.path().extension() == ".json")
        {
            std::string filePathString = directoryEntry.path().string();
            std::string relativePath = directoryEntry.path().parent_path().filename().string();

            ParseBenchmarkJsonFile(filePathString, relativePath);
        }
    }

    return true;
}

bool RotationCatalog::ParseBenchmarkJsonFile(const std::string& filePath, const std::string& relativeCategory)
{
    std::filesystem::path benchmarkPath(filePath);
    std::string stemName = benchmarkPath.stem().string();

    std::string lowerStem = stemName;
    for (auto& character : lowerStem)
    {
        character = static_cast<char>(std::tolower(character));
    }

    std::string profession = "";
    std::string specialization = "";
    uint32_t professionIdentifier = 0;
    uint32_t specializationIdentifier = 0;

    if (lowerStem.find("dragonhunter") != std::string::npos)
    {
        profession = "Guardian";
        specialization = "Dragonhunter";
        professionIdentifier = 1;
        specializationIdentifier = 27;
    }
    else if (lowerStem.find("firebrand") != std::string::npos)
    {
        profession = "Guardian";
        specialization = "Firebrand";
        professionIdentifier = 1;
        specializationIdentifier = 62;
    }
    else if (lowerStem.find("willbender") != std::string::npos)
    {
        profession = "Guardian";
        specialization = "Willbender";
        professionIdentifier = 1;
        specializationIdentifier = 65;
    }
    else if (lowerStem.find("luminary") != std::string::npos)
    {
        profession = "Guardian";
        specialization = "Luminary";
        professionIdentifier = 1;
        specializationIdentifier = 81;
    }
    else if (lowerStem.find("bladesworn") != std::string::npos)
    {
        profession = "Warrior";
        specialization = "Bladesworn";
        professionIdentifier = 2;
        specializationIdentifier = 68;
    }
    else if (lowerStem.find("berserker") != std::string::npos)
    {
        profession = "Warrior";
        specialization = "Berserker";
        professionIdentifier = 2;
        specializationIdentifier = 18;
    }
    else if (lowerStem.find("spellbreaker") != std::string::npos)
    {
        profession = "Warrior";
        specialization = "Spellbreaker";
        professionIdentifier = 2;
        specializationIdentifier = 61;
    }
    else if (lowerStem.find("paragon") != std::string::npos)
    {
        profession = "Warrior";
        specialization = "Paragon";
        professionIdentifier = 2;
        specializationIdentifier = 82;
    }
    else if (lowerStem.find("virtuoso") != std::string::npos)
    {
        profession = "Mesmer";
        specialization = "Virtuoso";
        professionIdentifier = 7;
        specializationIdentifier = 66;
    }
    else if (lowerStem.find("chronomancer") != std::string::npos)
    {
        profession = "Mesmer";
        specialization = "Chronomancer";
        professionIdentifier = 7;
        specializationIdentifier = 40;
    }
    else if (lowerStem.find("mirage") != std::string::npos)
    {
        profession = "Mesmer";
        specialization = "Mirage";
        professionIdentifier = 7;
        specializationIdentifier = 59;
    }
    else if (lowerStem.find("troubadour") != std::string::npos)
    {
        profession = "Mesmer";
        specialization = "Troubadour";
        professionIdentifier = 7;
        specializationIdentifier = 87;
    }
    else if (lowerStem.find("herald") != std::string::npos)
    {
        profession = "Revenant";
        specialization = "Herald";
        professionIdentifier = 9;
        specializationIdentifier = 52;
    }
    else if (lowerStem.find("renegade") != std::string::npos)
    {
        profession = "Revenant";
        specialization = "Renegade";
        professionIdentifier = 9;
        specializationIdentifier = 63;
    }
    else if (lowerStem.find("vindicator") != std::string::npos)
    {
        profession = "Revenant";
        specialization = "Vindicator";
        professionIdentifier = 9;
        specializationIdentifier = 69;
    }
    else if (lowerStem.find("conduit") != std::string::npos)
    {
        profession = "Revenant";
        specialization = "Conduit";
        professionIdentifier = 9;
        specializationIdentifier = 83;
    }
    else if (lowerStem.find("scourge") != std::string::npos)
    {
        profession = "Necromancer";
        specialization = "Scourge";
        professionIdentifier = 8;
        specializationIdentifier = 60;
    }
    else if (lowerStem.find("reaper") != std::string::npos)
    {
        profession = "Necromancer";
        specialization = "Reaper";
        professionIdentifier = 8;
        specializationIdentifier = 34;
    }
    else if (lowerStem.find("harbinger") != std::string::npos)
    {
        profession = "Necromancer";
        specialization = "Harbinger";
        professionIdentifier = 8;
        specializationIdentifier = 64;
    }
    else if (lowerStem.find("ritualist") != std::string::npos)
    {
        profession = "Necromancer";
        specialization = "Ritualist";
        professionIdentifier = 8;
        specializationIdentifier = 88;
    }
    else if (lowerStem.find("mechanist") != std::string::npos)
    {
        profession = "Engineer";
        specialization = "Mechanist";
        professionIdentifier = 3;
        specializationIdentifier = 70;
    }
    else if (lowerStem.find("holosmith") != std::string::npos)
    {
        profession = "Engineer";
        specialization = "Holosmith";
        professionIdentifier = 3;
        specializationIdentifier = 57;
    }
    else if (lowerStem.find("scrapper") != std::string::npos)
    {
        profession = "Engineer";
        specialization = "Scrapper";
        professionIdentifier = 3;
        specializationIdentifier = 43;
    }
    else if (lowerStem.find("amalgam") != std::string::npos)
    {
        profession = "Engineer";
        specialization = "Amalgam";
        professionIdentifier = 3;
        specializationIdentifier = 84;
    }
    else if (lowerStem.find("soulbeast") != std::string::npos)
    {
        profession = "Ranger";
        specialization = "Soulbeast";
        professionIdentifier = 4;
        specializationIdentifier = 55;
    }
    else if (lowerStem.find("druid") != std::string::npos)
    {
        profession = "Ranger";
        specialization = "Druid";
        professionIdentifier = 4;
        specializationIdentifier = 5;
    }
    else if (lowerStem.find("untamed") != std::string::npos)
    {
        profession = "Ranger";
        specialization = "Untamed";
        professionIdentifier = 4;
        specializationIdentifier = 72;
    }
    else if (lowerStem.find("galeshot") != std::string::npos)
    {
        profession = "Ranger";
        specialization = "Galeshot";
        professionIdentifier = 4;
        specializationIdentifier = 86;
    }
    else if (lowerStem.find("specter") != std::string::npos)
    {
        profession = "Thief";
        specialization = "Specter";
        professionIdentifier = 5;
        specializationIdentifier = 71;
    }
    else if (lowerStem.find("deadeye") != std::string::npos)
    {
        profession = "Thief";
        specialization = "Deadeye";
        professionIdentifier = 5;
        specializationIdentifier = 63;
    }
    else if (lowerStem.find("daredevil") != std::string::npos)
    {
        profession = "Thief";
        specialization = "Daredevil";
        professionIdentifier = 5;
        specializationIdentifier = 58;
    }
    else if (lowerStem.find("antiquary") != std::string::npos)
    {
        profession = "Thief";
        specialization = "Antiquary";
        professionIdentifier = 5;
        specializationIdentifier = 85;
    }
    else if (lowerStem.find("weaver") != std::string::npos)
    {
        profession = "Elementalist";
        specialization = "Weaver";
        professionIdentifier = 6;
        specializationIdentifier = 56;
    }
    else if (lowerStem.find("catalyst") != std::string::npos)
    {
        profession = "Elementalist";
        specialization = "Catalyst";
        professionIdentifier = 6;
        specializationIdentifier = 67;
    }
    else if (lowerStem.find("tempest") != std::string::npos)
    {
        profession = "Elementalist";
        specialization = "Tempest";
        professionIdentifier = 6;
        specializationIdentifier = 48;
    }
    else if (lowerStem.find("evoker") != std::string::npos)
    {
        profession = "Elementalist";
        specialization = "Evoker";
        professionIdentifier = 6;
        specializationIdentifier = 89;
    }
    else if (lowerStem.find("guardian") != std::string::npos)
    {
        profession = "Guardian";
        specialization = "Core Guardian";
        professionIdentifier = 1;
        specializationIdentifier = 0;
    }
    else if (lowerStem.find("warrior") != std::string::npos)
    {
        profession = "Warrior";
        specialization = "Core Warrior";
        professionIdentifier = 2;
        specializationIdentifier = 0;
    }
    else if (lowerStem.find("thief") != std::string::npos)
    {
        profession = "Thief";
        specialization = "Core Thief";
        professionIdentifier = 5;
        specializationIdentifier = 0;
    }

    if (profession.empty())
    {
        return false;
    }

    BuildRoleCategory roleCategory = BuildRoleCategory::PureDamagePerSecond;

    if (filePath.find("Heal") != std::string::npos || lowerStem.find("heal") != std::string::npos)
    {
        roleCategory = BuildRoleCategory::HealSupport;
    }
    else if (filePath.find("Hybrid") != std::string::npos || lowerStem.find("celestial") != std::string::npos || lowerStem.find("hybrid") != std::string::npos)
    {
        roleCategory = BuildRoleCategory::HybridSupportDamage;
    }
    else if (filePath.find("Alacrity") != std::string::npos || filePath.find("alac") != std::string::npos || lowerStem.find("alacrity") != std::string::npos)
    {
        roleCategory = BuildRoleCategory::BoonAlacrity;
    }
    else if (filePath.find("Quickness") != std::string::npos || filePath.find("quick") != std::string::npos || lowerStem.find("quickness") != std::string::npos)
    {
        roleCategory = BuildRoleCategory::BoonQuickness;
    }

    DamageTypeCategory damageCategory = DamageTypeCategory::ConditionDamage;

    if (filePath.find("Power") != std::string::npos || lowerStem.find("power") != std::string::npos)
    {
        damageCategory = DamageTypeCategory::PowerDamage;
    }
    else if (filePath.find("Heal") != std::string::npos || lowerStem.find("heal") != std::string::npos || filePath.find("Hybrid") != std::string::npos || lowerStem.find("celestial") != std::string::npos)
    {
        damageCategory = DamageTypeCategory::CelestialHybrid;
    }

    for (const auto& existingProfile : availableBuildProfiles)
    {
        if (existingProfile.profileIdentifier == stemName)
        {
            return false;
        }
    }

    std::ifstream jsonStream(filePath);

    if (!jsonStream.is_open())
    {
        return false;
    }

    std::stringstream bufferStream;
    bufferStream << jsonStream.rdbuf();
    std::string fileContent = bufferStream.str();

    std::string displayName = stemName;
    std::string guideUrl = "";
    std::string chatCode = "[&DQEqOSA6TB5/HhQA/hIAABcXAABTAQAAiRIAAAAAAAAAAAAAAAAAAAAAAA==]";
    std::string primaryWeaponsString = "First Weapon Set";
    std::string secondaryWeaponsString = "Second Weapon Set";

    size_t metadataPosition = fileContent.find("\"buildMetadata\":");
    if (metadataPosition != std::string::npos)
    {
        size_t namePosition = fileContent.find("\"name\":", metadataPosition);
        if (namePosition != std::string::npos)
        {
            size_t firstQuote = fileContent.find('\"', namePosition + 7);
            if (firstQuote != std::string::npos)
            {
                size_t secondQuote = fileContent.find('\"', firstQuote + 1);
                if (secondQuote != std::string::npos)
                {
                    displayName = fileContent.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
                }
            }
        }

        size_t urlPosition = fileContent.find("\"url\":", metadataPosition);
        if (urlPosition != std::string::npos)
        {
            size_t firstQuote = fileContent.find('\"', urlPosition + 6);
            if (firstQuote != std::string::npos)
            {
                size_t secondQuote = fileContent.find('\"', firstQuote + 1);
                if (secondQuote != std::string::npos)
                {
                    guideUrl = fileContent.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
                }
            }
        }

        size_t chatPosition = fileContent.find("\"chat_code\":", metadataPosition);
        if (chatPosition != std::string::npos)
        {
            size_t firstQuote = fileContent.find('\"', chatPosition + 12);
            if (firstQuote != std::string::npos)
            {
                size_t secondQuote = fileContent.find('\"', firstQuote + 1);
                if (secondQuote != std::string::npos)
                {
                    chatCode = fileContent.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
                }
            }
        }

        size_t weaponsPosition = fileContent.find("\"weapons\":", metadataPosition);
        if (weaponsPosition != std::string::npos)
        {
            size_t firstQuote = fileContent.find('\"', weaponsPosition + 10);
            if (firstQuote != std::string::npos)
            {
                size_t secondQuote = fileContent.find('\"', firstQuote + 1);
                if (secondQuote != std::string::npos)
                {
                    std::string fullWeaponsString = fileContent.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
                    size_t slashPosition = fullWeaponsString.find('/');
                    if (slashPosition != std::string::npos)
                    {
                        primaryWeaponsString = fullWeaponsString.substr(0, slashPosition);
                        secondaryWeaponsString = fullWeaponsString.substr(slashPosition + 1);

                        while (!primaryWeaponsString.empty() && primaryWeaponsString.back() == ' ')
                        {
                            primaryWeaponsString.pop_back();
                        }
                        while (!secondaryWeaponsString.empty() && secondaryWeaponsString.front() == ' ')
                        {
                            secondaryWeaponsString.erase(0, 1);
                        }
                    }
                    else
                    {
                        primaryWeaponsString = fullWeaponsString;
                        secondaryWeaponsString = "None (Attunement / Kit Switching)";
                    }
                }
            }
        }
    }

    if (primaryWeaponsString == "First Weapon Set")
    {
        if (lowerStem.find("dagger_sword_focus") != std::string::npos)
        {
            primaryWeaponsString = "Dagger / Sword";
            secondaryWeaponsString = "Dagger / Focus";
        }
        else if (lowerStem.find("greatsword_dagger_sword") != std::string::npos)
        {
            primaryWeaponsString = "Greatsword";
            secondaryWeaponsString = "Dagger / Sword";
        }
    }

    std::unordered_map<uint32_t, std::string> localSkillNames;
    std::unordered_map<uint32_t, bool> procSkillIdentifiers;

    size_t skillsPosition = fileContent.find("\"skills\":");
    if (skillsPosition == std::string::npos)
    {
        skillsPosition = fileContent.find("\"skillMap\":");
    }

    if (skillsPosition != std::string::npos)
    {
        size_t scanIndex = skillsPosition;
        size_t skillsEndPosition = fileContent.find("\"SkillKeyMapping\"", skillsPosition);

        if (skillsEndPosition == std::string::npos)
        {
            skillsEndPosition = fileContent.find("\"rotation\"", skillsPosition);
        }

        if (skillsEndPosition == std::string::npos)
        {
            skillsEndPosition = fileContent.size();
        }

        while (scanIndex < skillsEndPosition)
        {
            size_t skillKeyPosition = fileContent.find("\"s", scanIndex);
            if (skillKeyPosition == std::string::npos || skillKeyPosition >= skillsEndPosition)
            {
                break;
            }

            size_t colonPosition = fileContent.find(':', skillKeyPosition);
            if (colonPosition == std::string::npos)
            {
                break;
            }

            size_t quoteEnd = fileContent.find('\"', skillKeyPosition + 2);
            if (quoteEnd == std::string::npos || quoteEnd > colonPosition)
            {
                scanIndex = colonPosition + 1;
                continue;
            }

            std::string identifierText = fileContent.substr(skillKeyPosition + 2, quoteEnd - (skillKeyPosition + 2));
            uint32_t currentSkillIdentifier = 0;
            try
            {
                currentSkillIdentifier = static_cast<uint32_t>(std::stoul(identifierText));
            }
            catch (...)
            {
                scanIndex = colonPosition + 1;
                continue;
            }

            size_t namePosition = fileContent.find("\"name\":", colonPosition);
            size_t iconPosition = fileContent.find("\"icon\":", colonPosition);

            if (namePosition != std::string::npos && namePosition < colonPosition + 250)
            {
                size_t firstQuote = fileContent.find('\"', namePosition + 7);
                if (firstQuote != std::string::npos)
                {
                    size_t secondQuote = fileContent.find('\"', firstQuote + 1);
                    if (secondQuote != std::string::npos)
                    {
                        std::string parsedName = fileContent.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
                        localSkillNames[currentSkillIdentifier] = parsedName;
                    }
                }
            }

            if (iconPosition != std::string::npos && iconPosition < colonPosition + 450)
            {
                size_t firstQuote = fileContent.find('\"', iconPosition + 7);
                if (firstQuote != std::string::npos)
                {
                    size_t secondQuote = fileContent.find('\"', firstQuote + 1);
                    if (secondQuote != std::string::npos)
                    {
                        std::string iconUrl = fileContent.substr(firstQuote + 1, secondQuote - (firstQuote + 1));

                        if (iconUrl.find("placeholder") == std::string::npos)
                        {
                            size_t lastSlash = iconUrl.find_last_of('/');
                            if (lastSlash != std::string::npos)
                            {
                                std::string iconFileName = iconUrl.substr(lastSlash + 1);
                                skillIdentifierToIconFileNameMap[currentSkillIdentifier] = iconFileName;

                                if (localSkillNames.find(currentSkillIdentifier) != localSkillNames.end())
                                {
                                    skillNameToIconFileNameMap[localSkillNames[currentSkillIdentifier]] = iconFileName;
                                }

                                size_t dotPosition = iconFileName.find(".png");
                                if (dotPosition != std::string::npos)
                                {
                                    try
                                    {
                                        uint32_t renderIdentifier = static_cast<uint32_t>(std::stoul(iconFileName.substr(0, dotPosition)));
                                        skillIdentifierToIconFileNameMap[renderIdentifier] = iconFileName;
                                    }
                                    catch (...)
                                    {
                                    }
                                }
                            }
                        }
                    }
                }
            }

            size_t nextSkillKey = fileContent.find("\"s", colonPosition);
            size_t blockLimit = (nextSkillKey != std::string::npos && nextSkillKey < skillsEndPosition) ? nextSkillKey : skillsEndPosition;

            std::string skillSnippet = fileContent.substr(colonPosition, blockLimit - colonPosition);
            if (skillSnippet.find("\"gearProc\": true") != std::string::npos || skillSnippet.find("\"traitProc\": true") != std::string::npos)
            {
                procSkillIdentifiers[currentSkillIdentifier] = true;
            }

            scanIndex = colonPosition + 1;
        }
    }

    uint32_t slotSixSkillIdentifier = 0;
    uint32_t slotSevenSkillIdentifier = 0;
    uint32_t slotEightSkillIdentifier = 0;
    uint32_t slotNineSkillIdentifier = 0;
    uint32_t slotZeroSkillIdentifier = 0;

    size_t keyMappingPosition = fileContent.find("\"SkillKeyMapping\":");
    if (keyMappingPosition != std::string::npos)
    {
        auto parseSlotIdentifier = [&](const std::string& slotKey) -> uint32_t {
            size_t slotPosition = fileContent.find(slotKey, keyMappingPosition);
            if (slotPosition != std::string::npos && slotPosition < keyMappingPosition + 350)
            {
                size_t colon = fileContent.find(':', slotPosition);
                if (colon != std::string::npos)
                {
                    size_t comma = fileContent.find_first_of(",}", colon);
                    if (comma != std::string::npos)
                    {
                        std::string valueText = fileContent.substr(colon + 1, comma - (colon + 1));
                        try { return static_cast<uint32_t>(std::stoul(valueText)); } catch (...) {}
                    }
                }
            }
            return 0;
        };

        slotSixSkillIdentifier = parseSlotIdentifier("\"slot_6\":");
        slotSevenSkillIdentifier = parseSlotIdentifier("\"slot_7\":");
        slotEightSkillIdentifier = parseSlotIdentifier("\"slot_8\":");
        slotNineSkillIdentifier = parseSlotIdentifier("\"slot_9\":");
        slotZeroSkillIdentifier = parseSlotIdentifier("\"slot_0\":");
    }

    std::vector<RotationStep> steps;

    size_t rotationKeyPos = fileContent.find("\"rotation\":");

    if (rotationKeyPos != std::string::npos)
    {
        size_t currentScanPos = rotationKeyPos;
        size_t rotationEndPosition = fileContent.find("]]", rotationKeyPos);

        if (rotationEndPosition == std::string::npos)
        {
            rotationEndPosition = fileContent.size();
        }

        while (currentScanPos < rotationEndPosition)
        {
            size_t openBracket = fileContent.find('[', currentScanPos);

            if (openBracket == std::string::npos || openBracket >= rotationEndPosition)
            {
                break;
            }

            size_t closeBracket = fileContent.find(']', openBracket);

            if (closeBracket == std::string::npos || closeBracket >= rotationEndPosition)
            {
                break;
            }

            size_t nextInnerOpenBracket = fileContent.find('[', openBracket + 1);

            if (nextInnerOpenBracket != std::string::npos && nextInnerOpenBracket < closeBracket)
            {
                currentScanPos = nextInnerOpenBracket;
                continue;
            }

            size_t commaPos = fileContent.find(',', openBracket);

            if (commaPos != std::string::npos && commaPos < closeBracket)
            {
                size_t secondCommaPos = fileContent.find(',', commaPos + 1);

                if (secondCommaPos != std::string::npos && secondCommaPos < closeBracket)
                {
                    std::string skillIdString = fileContent.substr(commaPos + 1, secondCommaPos - (commaPos + 1));

                    try
                    {
                        uint32_t parsedSkillId = static_cast<uint32_t>(std::stoul(skillIdString));

                        if (parsedSkillId != 0 && parsedSkillId != 999999)
                        {
                            if (procSkillIdentifiers.find(parsedSkillId) != procSkillIdentifiers.end())
                            {
                                currentScanPos = closeBracket + 1;
                                continue;
                            }

                            RotationStep step;
                            step.stepIndex = static_cast<uint32_t>(steps.size());
                            step.skillIdentifier = parsedSkillId;
                            step.expectedDurationMilliseconds = 500;
                            step.queueWindowBufferMilliseconds = 250;

                            size_t thirdCommaPosition = fileContent.find(',', secondCommaPos + 1);

                            if (thirdCommaPosition != std::string::npos && thirdCommaPosition > secondCommaPos)
                            {
                                std::string durationString = fileContent.substr(secondCommaPos + 1, thirdCommaPosition - (secondCommaPos + 1));

                                try
                                {
                                    double parsedDurationValue = std::stod(durationString);

                                    if (parsedDurationValue > 0.0)
                                    {
                                        step.expectedDurationMilliseconds = static_cast<uint32_t>(parsedDurationValue);
                                    }
                                }
                                catch (...)
                                {
                                }
                            }

                            auto nameIterator = localSkillNames.find(parsedSkillId);
                            if (nameIterator != localSkillNames.end())
                            {
                                step.skillName = nameIterator->second;

                                auto slotCategoryIterator = skillNameToSlotCategoryMap.find(step.skillName);
                                if (slotCategoryIterator != skillNameToSlotCategoryMap.end())
                                {
                                    step.slotCategory = slotCategoryIterator->second;
                                }
                                else
                                {
                                    const SkillDefinitionMetadata* metadata = FindSkillMetadata(parsedSkillId);
                                    if (metadata != nullptr && !metadata->skillName.empty())
                                    {
                                        step.slotCategory = metadata->slotCategory;
                                    }
                                }
                            }
                            else
                            {
                                const SkillDefinitionMetadata* metadata = FindSkillMetadata(parsedSkillId);
                                if (metadata != nullptr && !metadata->skillName.empty())
                                {
                                    step.skillName = metadata->skillName;
                                    step.slotCategory = metadata->slotCategory;
                                }
                                else if (parsedSkillId == 9999)
                                {
                                    step.skillName = "Weapon Swap";
                                }
                                else if (parsedSkillId == 60975)
                                {
                                    step.skillName = "Stow Tome (F2)";
                                }
                                else
                                {
                                    step.skillName = "Skill " + std::to_string(parsedSkillId);
                                }
                            }

                            if (step.skillName.find("Relic of") != std::string::npos || step.skillName.find("Relic") != std::string::npos)
                            {
                                currentScanPos = closeBracket + 1;
                                continue;
                            }

                            auto categoryFromMap = skillNameToSlotCategoryMap.find(step.skillName);
                            if (categoryFromMap != skillNameToSlotCategoryMap.end())
                            {
                                step.slotCategory = categoryFromMap->second;
                            }

                            if (parsedSkillId == 9999 || step.skillName == "Weapon Swap" || step.skillName.find("Swap") != std::string::npos)
                            {
                                step.defaultKeybindName = "`";
                                step.slotCategory = SkillSlotCategory::WeaponSwap;
                            }
                            else if (parsedSkillId == 60975 || step.skillName.find("Stow Tome") != std::string::npos)
                            {
                                step.defaultKeybindName = "F2";
                                step.slotCategory = SkillSlotCategory::ProfessionSkillTwo;
                            }
                            else if (step.skillName.find("Exit Shroud") != std::string::npos || step.skillName.find("Exit Shadow Shroud") != std::string::npos || step.skillName.find("Exit Radiant Forge") != std::string::npos)
                            {
                                step.defaultKeybindName = "F1";
                                step.slotCategory = SkillSlotCategory::ProfessionSkillOne;
                            }
                            else if (parsedSkillId == slotSixSkillIdentifier && slotSixSkillIdentifier > 0)
                            {
                                step.defaultKeybindName = "6";
                                step.slotCategory = SkillSlotCategory::HealingSkill;
                            }
                            else if (parsedSkillId == slotSevenSkillIdentifier && slotSevenSkillIdentifier > 0)
                            {
                                step.defaultKeybindName = "7";
                                step.slotCategory = SkillSlotCategory::UtilitySkillOne;
                            }
                            else if (parsedSkillId == slotEightSkillIdentifier && slotEightSkillIdentifier > 0)
                            {
                                step.defaultKeybindName = "8";
                                step.slotCategory = SkillSlotCategory::UtilitySkillTwo;
                            }
                            else if (parsedSkillId == slotNineSkillIdentifier && slotNineSkillIdentifier > 0)
                            {
                                step.defaultKeybindName = "9";
                                step.slotCategory = SkillSlotCategory::UtilitySkillThree;
                            }
                            else if (parsedSkillId == slotZeroSkillIdentifier && slotZeroSkillIdentifier > 0)
                            {
                                step.defaultKeybindName = "0";
                                step.slotCategory = SkillSlotCategory::EliteSkill;
                            }
                            else if (parsedSkillId == 5492 || step.skillName.find("Fire Attunement") != std::string::npos)
                            {
                                step.defaultKeybindName = "F1";
                                step.slotCategory = SkillSlotCategory::ProfessionSkillOne;
                            }
                            else if (parsedSkillId == 5494 || step.skillName.find("Water Attunement") != std::string::npos)
                            {
                                step.defaultKeybindName = "F2";
                                step.slotCategory = SkillSlotCategory::ProfessionSkillTwo;
                            }
                            else if (parsedSkillId == 5493 || step.skillName.find("Air Attunement") != std::string::npos)
                            {
                                step.defaultKeybindName = "F3";
                                step.slotCategory = SkillSlotCategory::ProfessionSkillThree;
                            }
                            else if (parsedSkillId == 5495 || step.skillName.find("Earth Attunement") != std::string::npos)
                            {
                                step.defaultKeybindName = "F4";
                                step.slotCategory = SkillSlotCategory::ProfessionSkillFour;
                            }
                            else if (step.skillName.find("Deploy Jade Sphere") != std::string::npos || step.skillName.find("Jade Sphere") != std::string::npos)
                            {
                                step.defaultKeybindName = "B";
                                step.slotCategory = SkillSlotCategory::ProfessionSkillFive;
                            }
                            else if (step.slotCategory == SkillSlotCategory::ProfessionSkillOne || step.skillName.find("F1") != std::string::npos)
                            {
                                step.defaultKeybindName = "F1";
                                step.slotCategory = SkillSlotCategory::ProfessionSkillOne;
                            }
                            else if (step.slotCategory == SkillSlotCategory::ProfessionSkillTwo || step.skillName.find("F2") != std::string::npos)
                            {
                                step.defaultKeybindName = "F2";
                                step.slotCategory = SkillSlotCategory::ProfessionSkillTwo;
                            }
                            else if (step.slotCategory == SkillSlotCategory::ProfessionSkillThree || step.skillName.find("F3") != std::string::npos)
                            {
                                step.defaultKeybindName = "F3";
                                step.slotCategory = SkillSlotCategory::ProfessionSkillThree;
                            }
                            else if (step.slotCategory == SkillSlotCategory::ProfessionSkillFour || step.skillName.find("F4") != std::string::npos)
                            {
                                step.defaultKeybindName = "F4";
                                step.slotCategory = SkillSlotCategory::ProfessionSkillFour;
                            }
                            else if (step.slotCategory == SkillSlotCategory::ProfessionSkillFive || step.skillName.find("F5") != std::string::npos)
                            {
                                step.defaultKeybindName = "B";
                                step.slotCategory = SkillSlotCategory::ProfessionSkillFive;
                            }
                            else if (step.slotCategory == SkillSlotCategory::HealingSkill)
                            {
                                step.defaultKeybindName = "6";
                            }
                            else if (step.slotCategory == SkillSlotCategory::UtilitySkillOne)
                            {
                                step.defaultKeybindName = "7";
                            }
                            else if (step.slotCategory == SkillSlotCategory::UtilitySkillTwo)
                            {
                                step.defaultKeybindName = "8";
                            }
                            else if (step.slotCategory == SkillSlotCategory::UtilitySkillThree)
                            {
                                step.defaultKeybindName = "9";
                            }
                            else if (step.slotCategory == SkillSlotCategory::EliteSkill)
                            {
                                step.defaultKeybindName = "0";
                            }
                            else if (step.slotCategory == SkillSlotCategory::WeaponSkillFive || step.skillName.find(" 5)") != std::string::npos || step.skillName.find(" 5") != std::string::npos)
                            {
                                step.defaultKeybindName = "5";
                                step.slotCategory = SkillSlotCategory::WeaponSkillFive;
                            }
                            else if (step.slotCategory == SkillSlotCategory::WeaponSkillFour || step.skillName.find(" 4)") != std::string::npos || step.skillName.find(" 4") != std::string::npos)
                            {
                                step.defaultKeybindName = "4";
                                step.slotCategory = SkillSlotCategory::WeaponSkillFour;
                            }
                            else if (step.slotCategory == SkillSlotCategory::WeaponSkillThree || step.skillName.find(" 3)") != std::string::npos || step.skillName.find(" 3") != std::string::npos)
                            {
                                step.defaultKeybindName = "3";
                                step.slotCategory = SkillSlotCategory::WeaponSkillThree;
                            }
                            else if (step.slotCategory == SkillSlotCategory::WeaponSkillTwo || step.skillName.find(" 2)") != std::string::npos || step.skillName.find(" 2") != std::string::npos)
                            {
                                step.defaultKeybindName = "2";
                                step.slotCategory = SkillSlotCategory::WeaponSkillTwo;
                            }
                            else
                            {
                                step.defaultKeybindName = "1";
                                step.slotCategory = SkillSlotCategory::WeaponSkillOne;
                            }

                            bool isDuplicateConsecutiveSwap = (step.slotCategory == SkillSlotCategory::WeaponSwap && !steps.empty() && steps.back().slotCategory == SkillSlotCategory::WeaponSwap);

                            if (!isDuplicateConsecutiveSwap)
                            {
                                steps.push_back(step);
                            }
                        }
                    }
                    catch (...)
                    {
                    }
                }
            }

            currentScanPos = closeBracket + 1;
        }
    }

    if (steps.empty())
    {
        return false;
    }

    BuildProfile newProfile;
    newProfile.profileIdentifier = stemName;
    newProfile.buildDisplayName = displayName;
    newProfile.professionName = profession;
    newProfile.specializationName = specialization;
    newProfile.professionIdentifierNumber = professionIdentifier;
    newProfile.specializationIdentifierNumber = specializationIdentifier;
    newProfile.roleCategory = roleCategory;
    newProfile.damageCategory = damageCategory;
    newProfile.chatTemplateCode = chatCode;
    newProfile.snowCrowsGuideUrl = guideUrl;
    std::string armorAttributes = (roleCategory == BuildRoleCategory::HealSupport ? "Harrier's / Minstrel's" : (damageCategory == DamageTypeCategory::PowerDamage ? "Berserker's" : "Viper's / Ritualist's"));
    std::string upgradeRunes = (roleCategory == BuildRoleCategory::HealSupport ? "Superior Rune of the Monk" : "Superior Rune of the Scholar");
    std::string sigils = (roleCategory == BuildRoleCategory::HealSupport ? "Superior Sigil of Transference & Water" : "Superior Sigil of Force & Impact");
    std::string relic = (roleCategory == BuildRoleCategory::HealSupport ? "Relic of Karakosa" : "Relic of Fireworks");
    std::string food = (roleCategory == BuildRoleCategory::HealSupport ? "Delicious Rice Ball" : "Bowl of Sweet and Spicy Butternut Squash Soup");
    std::string utility = (roleCategory == BuildRoleCategory::HealSupport ? "Bountiful Maintenance Oil" : "Superior Sharpening Stone");

    newProfile.equipment = {
        primaryWeaponsString,
        secondaryWeaponsString,
        armorAttributes,
        upgradeRunes,
        sigils,
        relic,
        food,
        utility
    };
    newProfile.sequenceSteps = steps;

    availableBuildProfiles.push_back(newProfile);

    return true;
}

void RotationCatalog::PopulateBuiltInProfiles()
{
    availableBuildProfiles.clear();

    BuildProfile dragonhunterPowerProfile;
    dragonhunterPowerProfile.profileIdentifier = "power_dragonhunter_dps";
    dragonhunterPowerProfile.buildDisplayName = "Power DPS Dragonhunter (Greatsword / Longbow)";
    dragonhunterPowerProfile.professionName = "Guardian";
    dragonhunterPowerProfile.specializationName = "Dragonhunter";
    dragonhunterPowerProfile.professionIdentifierNumber = 1;
    dragonhunterPowerProfile.specializationIdentifierNumber = 27;
    dragonhunterPowerProfile.roleCategory = BuildRoleCategory::PureDamagePerSecond;
    dragonhunterPowerProfile.damageCategory = DamageTypeCategory::PowerDamage;
    dragonhunterPowerProfile.equipment = {
        "Greatsword",
        "Longbow",
        "Berserker's (Full)",
        "Superior Rune of the Scholar",
        "Superior Sigil of Force & Impact",
        "Relic of Fireworks",
        "Bowl of Sweet and Spicy Butternut Squash Soup",
        "Superior Sharpening Stone"
    };
    dragonhunterPowerProfile.sequenceSteps = {
        {0, 29927, "Spear of Justice (F1)", "F1", SkillSlotCategory::ProfessionSkillOne, 200, 200},
        {1, 9124, "Symbol of Wrath (GS 4)", "4", SkillSlotCategory::WeaponSkillFour, 500, 250},
        {2, 30273, "Procession of Blades", "8", SkillSlotCategory::UtilitySkillTwo, 250, 250},
        {3, 30364, "Dragon's Maw", "0", SkillSlotCategory::EliteSkill, 400, 250},
        {4, 9125, "Whirling Wrath (GS 2)", "2", SkillSlotCategory::WeaponSkillTwo, 1000, 250},
        {5, 9126, "Leap of Faith (GS 3)", "3", SkillSlotCategory::WeaponSkillThree, 500, 250},
        {6, 9127, "Binding Blade (GS 5)", "5", SkillSlotCategory::WeaponSkillFive, 600, 250},
        {7, 9999, "Weapon Swap (Longbow)", "`", SkillSlotCategory::WeaponSwap, 100, 100},
        {8, 9080, "True Shot (LB 2)", "2", SkillSlotCategory::WeaponSkillTwo, 600, 250},
        {9, 9082, "Symbol of Energy (LB 4)", "4", SkillSlotCategory::WeaponSkillFour, 500, 250},
        {10, 9083, "Hunter's Ward (LB 5)", "5", SkillSlotCategory::WeaponSkillFive, 800, 250},
        {11, 9999, "Weapon Swap (Greatsword)", "`", SkillSlotCategory::WeaponSwap, 100, 100}
    };
    dragonhunterPowerProfile.chatTemplateCode = "[&DQEqOSA6GzkmDw8BxAAAAM4AAACcAAAARgAAAAAAAAAAAAAAAAAAAAAAAAA=]";
    dragonhunterPowerProfile.snowCrowsGuideUrl = "https://snowcrows.com/builds/raids/guardian/power-dragonhunter";
    availableBuildProfiles.push_back(dragonhunterPowerProfile);

    BuildProfile firebrandHealProfile;
    firebrandHealProfile.profileIdentifier = "heal_quickness_firebrand";
    firebrandHealProfile.buildDisplayName = "Heal Quickness Firebrand";
    firebrandHealProfile.professionName = "Guardian";
    firebrandHealProfile.specializationName = "Firebrand";
    firebrandHealProfile.professionIdentifierNumber = 1;
    firebrandHealProfile.specializationIdentifierNumber = 62;
    firebrandHealProfile.roleCategory = BuildRoleCategory::HealSupport;
    firebrandHealProfile.damageCategory = DamageTypeCategory::CelestialHybrid;
    firebrandHealProfile.equipment = {
        "Axe / Shield",
        "Staff",
        "Harrier's & Minstrel's",
        "Superior Rune of the Monk",
        "Superior Sigil of Transference & Water",
        "Relic of Karakosa",
        "Delicious Rice Ball",
        "Bountiful Maintenance Oil"
    };
    firebrandHealProfile.sequenceSteps = {
        {0, 40915, "Mantra of Potence", "7", SkillSlotCategory::UtilitySkillOne, 150, 150},
        {1, 40963, "Mantra of Solace", "6", SkillSlotCategory::HealingSkill, 150, 150},
        {2, 9093, "Symbol of Swiftness", "3", SkillSlotCategory::WeaponSkillThree, 500, 250},
        {3, 9094, "Empower", "4", SkillSlotCategory::WeaponSkillFour, 1200, 250},
        {4, 68648, "Tome of Courage (F3)", "F3", SkillSlotCategory::ProfessionSkillThree, 200, 200},
        {5, 1770485, "Chapter 3: Valiant Bulwark", "3", SkillSlotCategory::WeaponSkillThree, 400, 200},
        {6, 1770486, "Chapter 4: Stalwart Stand", "4", SkillSlotCategory::WeaponSkillFour, 400, 200},
        {7, 60975, "Stow Tome (F3)", "F3", SkillSlotCategory::ProfessionSkillThree, 150, 150},
        {8, 9999, "Weapon Swap (Axe / Shield)", "`", SkillSlotCategory::WeaponSwap, 100, 100},
        {9, 9110, "Symbol of Resolution", "2", SkillSlotCategory::WeaponSkillTwo, 500, 250},
        {10, 9115, "Shield of Judgment", "4", SkillSlotCategory::WeaponSkillFour, 400, 250},
        {11, 9152, "Feel My Wrath!", "0", SkillSlotCategory::EliteSkill, 200, 200},
        {12, 9999, "Weapon Swap (Staff)", "`", SkillSlotCategory::WeaponSwap, 100, 100}
    };
    firebrandHealProfile.chatTemplateCode = "[&DQEqOSA6TB5/HhQA/hIAABcXAABTAQAAiRIAAAAAAAAAAAAAAAAAAAAAAA==]";
    firebrandHealProfile.snowCrowsGuideUrl = "https://snowcrows.com/builds/raids/guardian/heal-quickness-firebrand";
    availableBuildProfiles.push_back(firebrandHealProfile);

    skillIdentifierToIconFileNameMap[40915] = "1770490.png";
    skillIdentifierToIconFileNameMap[41258] = "1770490.png";
    skillIdentifierToIconFileNameMap[40963] = "1770463.png";
    skillIdentifierToIconFileNameMap[41259] = "1770463.png";
    skillIdentifierToIconFileNameMap[9093] = "103268.png";
    skillIdentifierToIconFileNameMap[9094] = "103636.png";
    skillIdentifierToIconFileNameMap[68648] = "2779164.png";
    skillIdentifierToIconFileNameMap[45210] = "2779164.png";
    skillIdentifierToIconFileNameMap[1770485] = "1770485.png";
    skillIdentifierToIconFileNameMap[45211] = "1770485.png";
    skillIdentifierToIconFileNameMap[1770486] = "1770486.png";
    skillIdentifierToIconFileNameMap[45212] = "1770486.png";
    skillIdentifierToIconFileNameMap[9152] = "103702.png";
    skillIdentifierToIconFileNameMap[60975] = "60975.png";
    skillIdentifierToIconFileNameMap[9999] = "9999.png";
    skillIdentifierToIconFileNameMap[9110] = "103655.png";
    skillIdentifierToIconFileNameMap[9115] = "103671.png";
    skillIdentifierToIconFileNameMap[9118] = "103638.png";
    skillIdentifierToIconFileNameMap[9122] = "103662.png";
    skillIdentifierToIconFileNameMap[9119] = "103643.png";

    skillNameToIconFileNameMap["Mantra of Potence"] = "1770490.png";
    skillNameToIconFileNameMap["Mantra of Solace"] = "1770463.png";
    skillNameToIconFileNameMap["Symbol of Swiftness"] = "103268.png";
    skillNameToIconFileNameMap["Symbol of Swiftness (Staff 3)"] = "103268.png";
    skillNameToIconFileNameMap["Empower"] = "103636.png";
    skillNameToIconFileNameMap["Empower (Staff 4)"] = "103636.png";
    skillNameToIconFileNameMap["Tome of Resolve"] = "2779164.png";
    skillNameToIconFileNameMap["Tome of Resolve (F2)"] = "2779164.png";
    skillNameToIconFileNameMap["Tome of Courage (F3)"] = "2779164.png";
    skillNameToIconFileNameMap["Chapter 3: Valiant Bulwark"] = "1770485.png";
    skillNameToIconFileNameMap["Chapter 4: Stalwart Stand"] = "1770486.png";
    skillNameToIconFileNameMap["Feel My Wrath!"] = "103702.png";
    skillNameToIconFileNameMap["\"Feel My Wrath!\""] = "103702.png";
    skillNameToIconFileNameMap["Stow Tome (F2)"] = "60975.png";
    skillNameToIconFileNameMap["Stow Tome (F3)"] = "60975.png";
    skillNameToIconFileNameMap["Stow Tome"] = "60975.png";
    skillNameToIconFileNameMap["Weapon Swap (Axe / Shield)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Staff)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Greatsword)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Longbow)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Sword / Focus)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap"] = "9999.png";
    skillNameToIconFileNameMap["Symbol of Resolution"] = "103655.png";
    skillNameToIconFileNameMap["Shield of Judgment"] = "103671.png";
    skillNameToIconFileNameMap["Symbol of Blades (Sword 2)"] = "103638.png";
    skillNameToIconFileNameMap["Ray of Judgment (Focus 4)"] = "103662.png";
    skillNameToIconFileNameMap["Zealot's Defense (Sword 3)"] = "103643.png";
    skillNameToIconFileNameMap["Drop Med Kit"] = "9999.png";
    skillNameToIconFileNameMap["Exit Shadow Shroud (F1)"] = "60975.png";
    skillNameToIconFileNameMap["Exit Radiant Forge (F1)"] = "60975.png";
    skillNameToIconFileNameMap["Weapon Swap (Sword / Pistol)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Gunsaber)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Axe / Axe)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Dagger / Focus)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Dagger / Sword)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Sword / Sword)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Mace / Shield)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Pistol / Dagger)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Scepter / Torch)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Pistol / Warhorn)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Dagger / Torch)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Hammer)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Spear)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Scepter / Dagger)"] = "9999.png";

    BuildProfile willbenderPowerProfile;
    willbenderPowerProfile.profileIdentifier = "power_willbender_dps";
    willbenderPowerProfile.buildDisplayName = "Power DPS Willbender";
    willbenderPowerProfile.professionName = "Guardian";
    willbenderPowerProfile.specializationName = "Willbender";
    willbenderPowerProfile.professionIdentifierNumber = 1;
    willbenderPowerProfile.specializationIdentifierNumber = 65;
    willbenderPowerProfile.roleCategory = BuildRoleCategory::PureDamagePerSecond;
    willbenderPowerProfile.damageCategory = DamageTypeCategory::PowerDamage;
    willbenderPowerProfile.equipment = {
        "Greatsword",
        "Sword / Focus",
        "Berserker's (Full)",
        "Superior Rune of the Scholar",
        "Superior Sigil of Force & Impact",
        "Relic of Fireworks",
        "Bowl of Sweet and Spicy Butternut Squash Soup",
        "Superior Sharpening Stone"
    };
    willbenderPowerProfile.sequenceSteps = {
        {0, 62603, "Rushing Justice (F1)", "F1", SkillSlotCategory::ProfessionSkillOne, 200, 200},
        {1, 62604, "Whirling Light", "7", SkillSlotCategory::UtilitySkillOne, 400, 250},
        {2, 9124, "Symbol of Wrath (GS 4)", "4", SkillSlotCategory::WeaponSkillFour, 500, 250},
        {3, 9125, "Whirling Wrath (GS 2)", "2", SkillSlotCategory::WeaponSkillTwo, 1000, 250},
        {4, 62605, "Crashing Courage (F3)", "F3", SkillSlotCategory::ProfessionSkillThree, 300, 250},
        {5, 9126, "Leap of Faith (GS 3)", "3", SkillSlotCategory::WeaponSkillThree, 500, 250},
        {6, 9999, "Weapon Swap (Sword / Focus)", "`", SkillSlotCategory::WeaponSwap, 100, 100},
        {7, 9118, "Symbol of Blades (Sword 2)", "2", SkillSlotCategory::WeaponSkillTwo, 400, 250},
        {8, 9122, "Ray of Judgment (Focus 4)", "4", SkillSlotCategory::WeaponSkillFour, 500, 250},
        {9, 9119, "Zealot's Defense (Sword 3)", "3", SkillSlotCategory::WeaponSkillThree, 1000, 250},
        {10, 9999, "Weapon Swap (Greatsword)", "`", SkillSlotCategory::WeaponSwap, 100, 100}
    };
    willbenderPowerProfile.chatTemplateCode = "[&DQEqOSA6QCkmDw8BBhIAAM4AAACcAAAARgAAAAAAAAAAAAAAAAAAAAAAAAA=]";
    willbenderPowerProfile.snowCrowsGuideUrl = "https://snowcrows.com/builds/raids/guardian/power-willbender";
    availableBuildProfiles.push_back(willbenderPowerProfile);

    BuildProfile luminaryAlacrityProfile;
    luminaryAlacrityProfile.profileIdentifier = "power_alacrity_luminary";
    luminaryAlacrityProfile.buildDisplayName = "Power Alacrity Luminary (Longbow / Greatsword)";
    luminaryAlacrityProfile.professionName = "Guardian";
    luminaryAlacrityProfile.specializationName = "Luminary";
    luminaryAlacrityProfile.professionIdentifierNumber = 1;
    luminaryAlacrityProfile.specializationIdentifierNumber = 81;
    luminaryAlacrityProfile.roleCategory = BuildRoleCategory::BoonAlacrity;
    luminaryAlacrityProfile.damageCategory = DamageTypeCategory::PowerDamage;
    luminaryAlacrityProfile.chatTemplateCode = "[&DQEqOSA6TB5/HhQA/hIAABcXAABTAQAAiRIAAAAAAAAAAAAAAAAAAAAAAA==]";
    luminaryAlacrityProfile.snowCrowsGuideUrl = "https://snowcrows.com/builds/raids/guardian/power-alacrity-luminary-longbow-greatsword";
    luminaryAlacrityProfile.equipment = {
        "Longbow",
        "Greatsword",
        "Berserker's & Dragon's",
        "Superior Rune of the Scholar",
        "Superior Sigil of Force & Impact",
        "Relic of Fireworks",
        "Bowl of Sweet and Spicy Butternut Squash Soup",
        "Superior Sharpening Stone"
    };
    luminaryAlacrityProfile.sequenceSteps = {
        {0, 3680154, "Piercing Stance", "7", SkillSlotCategory::UtilitySkillOne, 200, 200},
        {1, 3713157, "Symbol of Energy (LB 2)", "2", SkillSlotCategory::WeaponSkillTwo, 500, 250},
        {2, 3713158, "True Shot (LB 3)", "3", SkillSlotCategory::WeaponSkillThree, 750, 250},
        {3, 3713156, "Hunter's Ward (LB 5)", "5", SkillSlotCategory::WeaponSkillFive, 1200, 250},
        {4, 9999, "Weapon Swap (Greatsword)", "`", SkillSlotCategory::WeaponSwap, 100, 100},
        {5, 3680148, "Radiant Forge (F1)", "F1", SkillSlotCategory::ProfessionSkillOne, 250, 200},
        {6, 3680150, "Solar Burst (Forge 2)", "2", SkillSlotCategory::WeaponSkillTwo, 400, 200},
        {7, 3680152, "Corona Flare (Forge 5)", "5", SkillSlotCategory::WeaponSkillFive, 600, 250},
        {8, 3680148, "Exit Radiant Forge (F1)", "F1", SkillSlotCategory::ProfessionSkillOne, 150, 150},
        {9, 9125, "Whirling Wrath (GS 2)", "2", SkillSlotCategory::WeaponSkillTwo, 1000, 250},
        {10, 9999, "Weapon Swap (Longbow)", "`", SkillSlotCategory::WeaponSwap, 100, 100}
    };
    availableBuildProfiles.push_back(luminaryAlacrityProfile);

    BuildProfile bladeswornPowerDamageProfile;
    bladeswornPowerDamageProfile.profileIdentifier = "power_bladesworn_dps";
    bladeswornPowerDamageProfile.buildDisplayName = "Power DPS Bladesworn (Benchmark)";
    bladeswornPowerDamageProfile.professionName = "Warrior";
    bladeswornPowerDamageProfile.specializationName = "Bladesworn";
    bladeswornPowerDamageProfile.professionIdentifierNumber = 2;
    bladeswornPowerDamageProfile.specializationIdentifierNumber = 68;
    bladeswornPowerDamageProfile.roleCategory = BuildRoleCategory::PureDamagePerSecond;
    bladeswornPowerDamageProfile.damageCategory = DamageTypeCategory::PowerDamage;
    bladeswornPowerDamageProfile.equipment = {
        "Sword / Pistol",
        "Gunsaber",
        "Berserker's & Dragon's",
        "Superior Rune of the Scholar",
        "Superior Sigil of Force & Impact",
        "Relic of Fireworks",
        "Bowl of Sweet and Spicy Butternut Squash Soup",
        "Superior Sharpening Stone"
    };
    bladeswornPowerDamageProfile.sequenceSteps = {
        {0, 62803, "Dragon Trigger", "F2", SkillSlotCategory::ProfessionSkillTwo, 250, 250},
        {1, 62804, "Dragon Slash Force", "1", SkillSlotCategory::WeaponSkillOne, 600, 250},
        {2, 62791, "Break Step", "4", SkillSlotCategory::WeaponSkillFour, 500, 250},
        {3, 62817, "Overcharged Cartridges", "7", SkillSlotCategory::UtilitySkillOne, 150, 150},
        {4, 62822, "Flow Stabilizer", "8", SkillSlotCategory::UtilitySkillTwo, 200, 200},
        {5, 9999, "Weapon Swap (Sword / Pistol)", "`", SkillSlotCategory::WeaponSwap, 100, 100},
        {6, 14373, "Dragon's Roar", "5", SkillSlotCategory::WeaponSkillFive, 500, 250},
        {7, 14371, "Gunslinger's Guts", "4", SkillSlotCategory::WeaponSkillFour, 450, 250},
        {8, 14365, "Gash (Sword)", "2", SkillSlotCategory::WeaponSkillTwo, 400, 200},
        {9, 62829, "Tactical Reload", "0", SkillSlotCategory::EliteSkill, 300, 250},
        {10, 9999, "Weapon Swap (Gunsaber)", "`", SkillSlotCategory::WeaponSwap, 100, 100}
    };
    availableBuildProfiles.push_back(bladeswornPowerDamageProfile);

    BuildProfile bladeswornAlacrityProfile;
    bladeswornAlacrityProfile.profileIdentifier = "power_alacrity_bladesworn";
    bladeswornAlacrityProfile.buildDisplayName = "Power Alacrity Bladesworn";
    bladeswornAlacrityProfile.professionName = "Warrior";
    bladeswornAlacrityProfile.specializationName = "Bladesworn";
    bladeswornAlacrityProfile.professionIdentifierNumber = 2;
    bladeswornAlacrityProfile.specializationIdentifierNumber = 68;
    bladeswornAlacrityProfile.roleCategory = BuildRoleCategory::BoonAlacrity;
    bladeswornAlacrityProfile.damageCategory = DamageTypeCategory::PowerDamage;
    bladeswornAlacrityProfile.equipment = {
        "Sword / Pistol",
        "Gunsaber",
        "Diviner's & Berserker's",
        "Superior Rune of the Scholar",
        "Superior Sigil of Concentration & Force",
        "Relic of Fireworks",
        "Cilantro Lime Sous-Vide Steak",
        "Superior Sharpening Stone"
    };
    bladeswornAlacrityProfile.sequenceSteps = {
        {0, 62803, "Dragon Trigger (Daring)", "F2", SkillSlotCategory::ProfessionSkillTwo, 250, 250},
        {1, 62804, "Dragon Slash Burst", "1", SkillSlotCategory::WeaponSkillOne, 450, 250},
        {2, 62822, "Flow Stabilizer", "8", SkillSlotCategory::UtilitySkillTwo, 200, 200},
        {3, 62817, "Overcharged Cartridges", "7", SkillSlotCategory::UtilitySkillOne, 150, 150},
        {4, 9999, "Weapon Swap (Sword / Pistol)", "`", SkillSlotCategory::WeaponSwap, 100, 100},
        {5, 14373, "Dragon's Roar", "5", SkillSlotCategory::WeaponSkillFive, 500, 250},
        {6, 62829, "Tactical Reload", "0", SkillSlotCategory::EliteSkill, 300, 250},
        {7, 9999, "Weapon Swap (Gunsaber)", "`", SkillSlotCategory::WeaponSwap, 100, 100}
    };
    availableBuildProfiles.push_back(bladeswornAlacrityProfile);

    BuildProfile berserkerQuicknessProfile;
    berserkerQuicknessProfile.profileIdentifier = "power_quickness_berserker";
    berserkerQuicknessProfile.buildDisplayName = "Power Quickness Berserker";
    berserkerQuicknessProfile.professionName = "Warrior";
    berserkerQuicknessProfile.specializationName = "Berserker";
    berserkerQuicknessProfile.professionIdentifierNumber = 2;
    berserkerQuicknessProfile.specializationIdentifierNumber = 18;
    berserkerQuicknessProfile.roleCategory = BuildRoleCategory::BoonQuickness;
    berserkerQuicknessProfile.damageCategory = DamageTypeCategory::PowerDamage;
    berserkerQuicknessProfile.equipment = {
        "Greatsword",
        "Axe / Axe",
        "Berserker's & Diviner's",
        "Superior Rune of the Scholar",
        "Superior Sigil of Force & Impact",
        "Relic of Fireworks",
        "Bowl of Sweet and Spicy Butternut Squash Soup",
        "Superior Sharpening Stone"
    };
    berserkerQuicknessProfile.sequenceSteps = {
        {0, 14444, "Berserk Mode (F2)", "F2", SkillSlotCategory::ProfessionSkillTwo, 200, 200},
        {1, 14445, "Arc Divider (GS F1)", "F1", SkillSlotCategory::ProfessionSkillOne, 800, 250},
        {2, 14387, "Hundred Blades (GS 2)", "2", SkillSlotCategory::WeaponSkillTwo, 1200, 250},
        {3, 14404, "Blood Reckoning (Heal)", "6", SkillSlotCategory::HealingSkill, 300, 250},
        {4, 14445, "Arc Divider (GS F1)", "F1", SkillSlotCategory::ProfessionSkillOne, 800, 250},
        {5, 9999, "Weapon Swap (Axe / Axe)", "`", SkillSlotCategory::WeaponSwap, 100, 100},
        {6, 14418, "Whirling Axe (Axe 5)", "5", SkillSlotCategory::WeaponSkillFive, 1200, 250},
        {7, 14417, "Dual Strike (Axe 3)", "3", SkillSlotCategory::WeaponSkillThree, 500, 250},
        {8, 9999, "Weapon Swap (Greatsword)", "`", SkillSlotCategory::WeaponSwap, 100, 100}
    };
    availableBuildProfiles.push_back(berserkerQuicknessProfile);

    BuildProfile virtuosoConditionProfile;
    virtuosoConditionProfile.profileIdentifier = "condition_dps_virtuoso";
    virtuosoConditionProfile.buildDisplayName = "Condition DPS Virtuoso";
    virtuosoConditionProfile.professionName = "Mesmer";
    virtuosoConditionProfile.specializationName = "Virtuoso";
    virtuosoConditionProfile.professionIdentifierNumber = 7;
    virtuosoConditionProfile.specializationIdentifierNumber = 66;
    virtuosoConditionProfile.roleCategory = BuildRoleCategory::PureDamagePerSecond;
    virtuosoConditionProfile.damageCategory = DamageTypeCategory::ConditionDamage;
    virtuosoConditionProfile.equipment = {
        "Dagger / Sword",
        "Dagger / Focus",
        "Viper's & Sinister",
        "Superior Rune of the Krait",
        "Superior Sigil of Bursting & Earth",
        "Relic of the Fractal",
        "Plate of Beef Rendang",
        "Toxic Tuning Crystal"
    };
    virtuosoConditionProfile.sequenceSteps = {
        {0, 103723, "Phantasmal Swordsman", "5", SkillSlotCategory::WeaponSkillFive, 600, 250},
        {1, 9999, "Weapon Swap (Dagger / Focus)", "`", SkillSlotCategory::WeaponSwap, 100, 100},
        {2, 2479379, "Bladecall (Dagger 2)", "2", SkillSlotCategory::WeaponSkillTwo, 400, 200},
        {3, 103768, "Phantasmal Warden (Focus 4)", "4", SkillSlotCategory::WeaponSkillFour, 500, 250},
        {4, 699528, "Signet of the Ether", "6", SkillSlotCategory::HealingSkill, 500, 250},
        {5, 2479382, "Bladesong Harmony (F1)", "F1", SkillSlotCategory::ProfessionSkillOne, 250, 250},
        {6, 2479389, "Thousand Cuts", "0", SkillSlotCategory::EliteSkill, 750, 250},
        {7, 103768, "Phantasmal Warden (Focus 4)", "4", SkillSlotCategory::WeaponSkillFour, 500, 250},
        {8, 2479385, "Flying Cutter (Dagger 1)", "1", SkillSlotCategory::WeaponSkillOne, 250, 250},
        {9, 2479383, "Bladesong Sorrow (F2)", "F2", SkillSlotCategory::ProfessionSkillTwo, 250, 250},
        {10, 2479391, "Unstable Bladestorm (Dagger 3)", "3", SkillSlotCategory::WeaponSkillThree, 500, 250}
    };
    availableBuildProfiles.push_back(virtuosoConditionProfile);

    BuildProfile heraldQuicknessProfile;
    heraldQuicknessProfile.profileIdentifier = "power_quickness_herald";
    heraldQuicknessProfile.buildDisplayName = "Power Quickness Herald";
    heraldQuicknessProfile.professionName = "Revenant";
    heraldQuicknessProfile.specializationName = "Herald";
    heraldQuicknessProfile.professionIdentifierNumber = 9;
    heraldQuicknessProfile.specializationIdentifierNumber = 52;
    heraldQuicknessProfile.roleCategory = BuildRoleCategory::BoonQuickness;
    heraldQuicknessProfile.damageCategory = DamageTypeCategory::PowerDamage;
    heraldQuicknessProfile.equipment = {
        "Greatsword",
        "Sword / Sword",
        "Berserker's & Diviner's",
        "Superior Rune of the Scholar",
        "Superior Sigil of Force & Impact",
        "Relic of the Claw",
        "Bowl of Sweet and Spicy Butternut Squash Soup",
        "Superior Sharpening Stone"
    };
    heraldQuicknessProfile.sequenceSteps = {
        {0, 28287, "Facet of Nature", "F2", SkillSlotCategory::ProfessionSkillTwo, 100, 100},
        {1, 28288, "Facet of Darkness", "7", SkillSlotCategory::UtilitySkillOne, 100, 100},
        {2, 28289, "Facet of Strength", "8", SkillSlotCategory::UtilitySkillTwo, 100, 100},
        {3, 28290, "Facet of Elements", "9", SkillSlotCategory::UtilitySkillThree, 100, 100},
        {4, 62650, "Mist Unleashed (GS 2)", "2", SkillSlotCategory::WeaponSkillTwo, 500, 250},
        {5, 62651, "Phantom's Onslaught (GS 3)", "3", SkillSlotCategory::WeaponSkillThree, 600, 250},
        {6, 62652, "True Strike (GS 5)", "5", SkillSlotCategory::WeaponSkillFive, 800, 250},
        {7, 9999, "Legend Swap (Shiro)", "F1", SkillSlotCategory::ProfessionSkillOne, 100, 100},
        {8, 28414, "Impossible Odds", "9", SkillSlotCategory::UtilitySkillThree, 100, 100},
        {9, 9999, "Weapon Swap (Sword / Sword)", "`", SkillSlotCategory::WeaponSwap, 100, 100},
        {10, 28312, "Unrelenting Assault (Sword 3)", "3", SkillSlotCategory::WeaponSkillThree, 800, 250},
        {11, 9999, "Weapon Swap (Greatsword)", "`", SkillSlotCategory::WeaponSwap, 100, 100}
    };
    availableBuildProfiles.push_back(heraldQuicknessProfile);

    BuildProfile mechanistHealProfile;
    mechanistHealProfile.profileIdentifier = "heal_alacrity_mechanist";
    mechanistHealProfile.buildDisplayName = "Heal Alacrity Mechanist";
    mechanistHealProfile.professionName = "Engineer";
    mechanistHealProfile.specializationName = "Mechanist";
    mechanistHealProfile.professionIdentifierNumber = 3;
    mechanistHealProfile.specializationIdentifierNumber = 70;
    mechanistHealProfile.roleCategory = BuildRoleCategory::HealSupport;
    mechanistHealProfile.damageCategory = DamageTypeCategory::CelestialHybrid;
    mechanistHealProfile.equipment = {
        "Mace / Shield",
        "Mechanical Mace",
        "Harrier's & Giver's",
        "Superior Rune of the Monk",
        "Superior Sigil of Transference & Concentration",
        "Relic of Karakosa",
        "Delicious Rice Ball",
        "Bountiful Maintenance Oil"
    };
    mechanistHealProfile.sequenceSteps = {
        {0, 63101, "Crisis Zone (F2)", "F2", SkillSlotCategory::ProfessionSkillTwo, 200, 200},
        {1, 63102, "Barrier Signet", "8", SkillSlotCategory::UtilitySkillTwo, 200, 200},
        {2, 63103, "Roiling Skies (F3)", "F3", SkillSlotCategory::ProfessionSkillThree, 400, 250},
        {3, 58459, "Energizing Slam (Mace 2)", "2", SkillSlotCategory::WeaponSkillTwo, 500, 250},
        {4, 58460, "Shield Protocol (Shield 4)", "4", SkillSlotCategory::WeaponSkillFour, 500, 250},
        {5, 63104, "Overclock Signet", "0", SkillSlotCategory::EliteSkill, 300, 250},
        {6, 5824, "Equip Med Kit", "7", SkillSlotCategory::UtilitySkillOne, 150, 150},
        {7, 5825, "Med Blaster (Kit 1)", "1", SkillSlotCategory::WeaponSkillOne, 400, 200},
        {8, 5827, "Cleansing Field (Kit 3)", "3", SkillSlotCategory::WeaponSkillThree, 500, 250},
        {9, 5824, "Drop Med Kit", "`", SkillSlotCategory::WeaponSwap, 150, 150}
    };
    availableBuildProfiles.push_back(mechanistHealProfile);

    BuildProfile scourgeAlacrityProfile;
    scourgeAlacrityProfile.profileIdentifier = "condition_alacrity_scourge";
    scourgeAlacrityProfile.buildDisplayName = "Condition Alacrity Scourge";
    scourgeAlacrityProfile.professionName = "Necromancer";
    scourgeAlacrityProfile.specializationName = "Scourge";
    scourgeAlacrityProfile.professionIdentifierNumber = 8;
    scourgeAlacrityProfile.specializationIdentifierNumber = 60;
    scourgeAlacrityProfile.roleCategory = BuildRoleCategory::BoonAlacrity;
    scourgeAlacrityProfile.damageCategory = DamageTypeCategory::ConditionDamage;
    scourgeAlacrityProfile.equipment = {
        "Scepter / Torch",
        "Pistol / Dagger",
        "Viper's & Plaguedoctor's",
        "Superior Rune of the Tormenting",
        "Superior Sigil of Bursting & Earth",
        "Relic of the Scourge",
        "Plate of Beef Rendang",
        "Toxic Focusing Crystal"
    };
    scourgeAlacrityProfile.sequenceSteps = {
        {0, 44455, "Manifest Sand Shade (F1)", "F1", SkillSlotCategory::ProfessionSkillOne, 150, 150},
        {1, 44456, "Desert Shroud (F5)", "F5", SkillSlotCategory::ProfessionSkillFive, 250, 250},
        {2, 44457, "Blood Is Power", "8", SkillSlotCategory::UtilitySkillTwo, 250, 250},
        {3, 10582, "Oppressive Collapse (Torch 5)", "5", SkillSlotCategory::WeaponSkillFive, 500, 250},
        {4, 10581, "Harrowing Wave (Torch 4)", "4", SkillSlotCategory::WeaponSkillFour, 450, 200},
        {5, 10578, "Grasping Dead (Scepter 2)", "2", SkillSlotCategory::WeaponSkillTwo, 400, 200},
        {6, 9999, "Weapon Swap (Pistol / Dagger)", "`", SkillSlotCategory::WeaponSwap, 100, 100},
        {7, 62900, "Vile Volley (Pistol 2)", "2", SkillSlotCategory::WeaponSkillTwo, 600, 250},
        {8, 10595, "Plaguelands", "0", SkillSlotCategory::EliteSkill, 600, 250},
        {9, 9999, "Weapon Swap (Scepter / Torch)", "`", SkillSlotCategory::WeaponSwap, 100, 100}
    };
    availableBuildProfiles.push_back(scourgeAlacrityProfile);

    BuildProfile scourgeHybridCelestialProfile;
    scourgeHybridCelestialProfile.profileIdentifier = "celestial_hybrid_scourge";
    scourgeHybridCelestialProfile.buildDisplayName = "Celestial Hybrid Scourge (Heal/Boon/DPS)";
    scourgeHybridCelestialProfile.professionName = "Necromancer";
    scourgeHybridCelestialProfile.specializationName = "Scourge";
    scourgeHybridCelestialProfile.professionIdentifierNumber = 8;
    scourgeHybridCelestialProfile.specializationIdentifierNumber = 60;
    scourgeHybridCelestialProfile.roleCategory = BuildRoleCategory::HybridSupportDamage;
    scourgeHybridCelestialProfile.damageCategory = DamageTypeCategory::CelestialHybrid;
    scourgeHybridCelestialProfile.equipment = {
        "Dagger / Torch",
        "Pistol / Warhorn",
        "Celestial (Full)",
        "Superior Rune of the Traveler",
        "Superior Sigil of Frenzy & Transference",
        "Relic of the Monk",
        "Bowl of Fruit Salad with Mint Garnish",
        "Tuning Icicle"
    };
    scourgeHybridCelestialProfile.sequenceSteps = {
        {0, 44455, "Manifest Sand Shade (F1)", "F1", SkillSlotCategory::ProfessionSkillOne, 150, 150},
        {1, 44458, "Sand Cascade (F2 Barrier)", "F2", SkillSlotCategory::ProfessionSkillTwo, 200, 200},
        {2, 44456, "Desert Shroud (F5)", "F5", SkillSlotCategory::ProfessionSkillFive, 250, 250},
        {3, 10582, "Oppressive Collapse (Torch 5)", "5", SkillSlotCategory::WeaponSkillFive, 500, 250},
        {4, 44459, "Trail of Anguish", "7", SkillSlotCategory::UtilitySkillOne, 200, 200},
        {5, 9999, "Weapon Swap (Pistol / Warhorn)", "`", SkillSlotCategory::WeaponSwap, 100, 100},
        {6, 10586, "Wail of Doom (Warhorn 4)", "4", SkillSlotCategory::WeaponSkillFour, 450, 200},
        {7, 10587, "Locust Swarm (Warhorn 5)", "5", SkillSlotCategory::WeaponSkillFive, 500, 250},
        {8, 9999, "Weapon Swap (Dagger / Torch)", "`", SkillSlotCategory::WeaponSwap, 100, 100}
    };
    availableBuildProfiles.push_back(scourgeHybridCelestialProfile);

    BuildProfile soulbeastPowerProfile;
    soulbeastPowerProfile.profileIdentifier = "power_soulbeast_dps";
    soulbeastPowerProfile.buildDisplayName = "Power DPS Soulbeast (Hammer / Axe)";
    soulbeastPowerProfile.professionName = "Ranger";
    soulbeastPowerProfile.specializationName = "Soulbeast";
    soulbeastPowerProfile.professionIdentifierNumber = 4;
    soulbeastPowerProfile.specializationIdentifierNumber = 55;
    soulbeastPowerProfile.roleCategory = BuildRoleCategory::PureDamagePerSecond;
    soulbeastPowerProfile.damageCategory = DamageTypeCategory::PowerDamage;
    soulbeastPowerProfile.equipment = {
        "Hammer",
        "Axe / Axe",
        "Berserker's (Full)",
        "Superior Rune of the Scholar",
        "Superior Sigil of Force & Impact",
        "Relic of Fireworks",
        "Bowl of Sweet and Spicy Butternut Squash Soup",
        "Superior Sharpening Stone"
    };
    soulbeastPowerProfile.sequenceSteps = {
        {0, 41918, "Beastmode (F5)", "F5", SkillSlotCategory::ProfessionSkillFive, 100, 100},
        {1, 43636, "One Wolf Pack", "0", SkillSlotCategory::EliteSkill, 200, 200},
        {2, 45717, "Worldly Impact (F2)", "F2", SkillSlotCategory::ProfessionSkillTwo, 400, 250},
        {3, 62688, "Overbearing Smash (Hammer 2)", "2", SkillSlotCategory::WeaponSkillTwo, 500, 250},
        {4, 62689, "Unleashed Savage Shock Wave (Hammer 3)", "3", SkillSlotCategory::WeaponSkillThree, 600, 250},
        {5, 9999, "Weapon Swap (Axe / Axe)", "`", SkillSlotCategory::WeaponSwap, 100, 100},
        {6, 12511, "Whirling Defense (Axe 5)", "5", SkillSlotCategory::WeaponSkillFive, 1200, 250},
        {7, 12510, "Path of Scars (Axe 4)", "4", SkillSlotCategory::WeaponSkillFour, 500, 250},
        {8, 9999, "Weapon Swap (Hammer)", "`", SkillSlotCategory::WeaponSwap, 100, 100}
    };
    availableBuildProfiles.push_back(soulbeastPowerProfile);

    BuildProfile specterConditionProfile;
    specterConditionProfile.profileIdentifier = "condition_alacrity_specter";
    specterConditionProfile.buildDisplayName = "Condition Alacrity Specter";
    specterConditionProfile.professionName = "Thief";
    specterConditionProfile.specializationName = "Specter";
    specterConditionProfile.professionIdentifierNumber = 5;
    specterConditionProfile.specializationIdentifierNumber = 71;
    specterConditionProfile.roleCategory = BuildRoleCategory::BoonAlacrity;
    specterConditionProfile.damageCategory = DamageTypeCategory::ConditionDamage;
    specterConditionProfile.equipment = {
        "Scepter / Dagger",
        "Spear",
        "Viper's & Ritualist's",
        "Superior Rune of the Tormenting",
        "Superior Sigil of Bursting & Earth",
        "Relic of the Fractal",
        "Plate of Beef Rendang",
        "Toxic Focusing Crystal"
    };
    specterConditionProfile.sequenceSteps = {
        {0, 63319, "Shadow Shroud (F1)", "F1", SkillSlotCategory::ProfessionSkillOne, 150, 150},
        {1, 63320, "Grasp of Shadows (Shroud 2)", "2", SkillSlotCategory::WeaponSkillTwo, 400, 200},
        {2, 63321, "Dawn's Repose (Shroud 3)", "3", SkillSlotCategory::WeaponSkillThree, 400, 200},
        {3, 63322, "Mind Shock (Shroud 5)", "5", SkillSlotCategory::WeaponSkillFive, 600, 250},
        {4, 63319, "Exit Shadow Shroud (F1)", "F1", SkillSlotCategory::ProfessionSkillOne, 150, 150},
        {5, 63325, "Twilight Combo (Scepter 3)", "3", SkillSlotCategory::WeaponSkillThree, 500, 250},
        {6, 9999, "Weapon Swap (Spear)", "`", SkillSlotCategory::WeaponSwap, 100, 100},
        {7, 63330, "Shadow Veil (Spear 2)", "2", SkillSlotCategory::WeaponSkillTwo, 400, 200},
        {8, 9999, "Weapon Swap (Scepter / Dagger)", "`", SkillSlotCategory::WeaponSwap, 100, 100}
    };
    availableBuildProfiles.push_back(specterConditionProfile);

    BuildProfile weaverPowerProfile;
    weaverPowerProfile.profileIdentifier = "power_weaver_dps";
    weaverPowerProfile.buildDisplayName = "Power DPS Weaver (Sword / Dagger)";
    weaverPowerProfile.professionName = "Elementalist";
    weaverPowerProfile.specializationName = "Weaver";
    weaverPowerProfile.professionIdentifierNumber = 6;
    weaverPowerProfile.specializationIdentifierNumber = 56;
    weaverPowerProfile.roleCategory = BuildRoleCategory::PureDamagePerSecond;
    weaverPowerProfile.damageCategory = DamageTypeCategory::PowerDamage;
    weaverPowerProfile.equipment = {
        "Sword / Dagger",
        "Scepter / Focus",
        "Berserker's (Full)",
        "Superior Rune of the Scholar",
        "Superior Sigil of Force & Impact",
        "Relic of Fireworks",
        "Bowl of Sweet and Spicy Butternut Squash Soup",
        "Superior Sharpening Stone"
    };
    weaverPowerProfile.sequenceSteps = {
        {0, 5492, "Fire Attunement (F1)", "F1", SkillSlotCategory::ProfessionSkillOne, 100, 100},
        {1, 40183, "Flame Uprising (Sword 1)", "1", SkillSlotCategory::WeaponSkillOne, 450, 200},
        {2, 43743, "Fire Strike (Sword 2)", "2", SkillSlotCategory::WeaponSkillTwo, 500, 250},
        {3, 5493, "Air Attunement (F3)", "F3", SkillSlotCategory::ProfessionSkillThree, 100, 100},
        {4, 44633, "Quantum Strike (Air/Fire 3)", "3", SkillSlotCategory::WeaponSkillThree, 600, 250},
        {5, 41634, "Ride the Lightning (Dagger 4)", "4", SkillSlotCategory::WeaponSkillFour, 500, 250},
        {6, 5495, "Earth Attunement (F4)", "F4", SkillSlotCategory::ProfessionSkillFour, 100, 100},
        {7, 42355, "Gale Strike (Air/Earth 3)", "3", SkillSlotCategory::WeaponSkillThree, 500, 250},
        {8, 5492, "Fire Attunement (F1)", "F1", SkillSlotCategory::ProfessionSkillOne, 100, 100}
    };
    availableBuildProfiles.push_back(weaverPowerProfile);
}
