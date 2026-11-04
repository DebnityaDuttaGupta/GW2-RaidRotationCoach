#include "RotationCatalog.h"
#include <filesystem>
#include <fstream>
#include <sstream>

RotationCatalog::RotationCatalog()
{
    PopulateDefaultSkillMappings();

    standaloneAddonDirectoryPath = "E:\\SteamLibrary\\steamapps\\common\\Guild Wars 2\\addons\\Gw2RotationCoach";

    std::string primarySkillsFilePath = standaloneAddonDirectoryPath + "\\skills\\gw2_skills_en.json";
    if (std::filesystem::exists(primarySkillsFilePath))
    {
        LoadMasterSkillDatabase(primarySkillsFilePath);
    }

    std::string uncategorizedSkillsFilePath = standaloneAddonDirectoryPath + "\\skills\\gw2_uncategorized_skills.json";
    if (std::filesystem::exists(uncategorizedSkillsFilePath))
    {
        LoadMasterSkillDatabase(uncategorizedSkillsFilePath);
    }

    std::string standaloneBenchPath = standaloneAddonDirectoryPath + "\\bench";
    if (std::filesystem::exists(standaloneBenchPath))
    {
        LoadBenchmarkDirectory(standaloneBenchPath);
    }
}

void RotationCatalog::PopulateDefaultSkillMappings()
{
    skillIdentifierToIconFileNameMap[9999] = "9999.png";
    skillIdentifierToIconFileNameMap[60975] = "60975.png";

    skillNameToIconFileNameMap["Weapon Swap"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Axe / Shield)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Staff)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Greatsword)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Longbow)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Sword / Focus)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Dagger / Torch)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Scepter / Focus)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Pistol / Pistol)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Hammer)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Rifle)"] = "9999.png";
    skillNameToIconFileNameMap["Weapon Swap (Shortbow)"] = "9999.png";

    skillNameToIconFileNameMap["Stow"] = "60975.png";
    skillNameToIconFileNameMap["Stow Weapon"] = "60975.png";
    skillNameToIconFileNameMap["Stow Pet"] = "60975.png";
    skillNameToIconFileNameMap["Stow Tome"] = "60975.png";
    skillNameToIconFileNameMap["Stow Tome (F2)"] = "60975.png";
    skillNameToIconFileNameMap["Stow Tome (F3)"] = "60975.png";

    skillNameToSlotCategoryMap["Weapon Swap"] = SkillSlotCategory::WeaponSwap;
    skillNameToSlotCategoryMap["Dodge"] = SkillSlotCategory::DodgeAction;

    skillNameToSlotCategoryMap["Bladesong Harmony"] = SkillSlotCategory::ProfessionSkillOne;
    skillNameToSlotCategoryMap["Bladesong Sorrow"] = SkillSlotCategory::ProfessionSkillTwo;
    skillNameToSlotCategoryMap["Bladesong Dissonance"] = SkillSlotCategory::ProfessionSkillThree;
    skillNameToSlotCategoryMap["Bladeturn Requiem"] = SkillSlotCategory::ProfessionSkillFour;
    skillNameToSlotCategoryMap["Bladesong Distortion"] = SkillSlotCategory::ProfessionSkillFive;
    skillNameToSlotCategoryMap["Distortion"] = SkillSlotCategory::ProfessionSkillFive;

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



    if (skillName.find("Weapon Swap") != std::string::npos || skillName.find("Drop Med Kit") != std::string::npos)

    {

        return "9999.png";

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

    std::string primarySkillsFilePath = standaloneAddonDirectoryPath + "\\skills\\gw2_skills_en.json";
    if (std::filesystem::exists(primarySkillsFilePath))
    {
        LoadMasterSkillDatabase(primarySkillsFilePath);
    }

    std::string uncategorizedSkillsFilePath = standaloneAddonDirectoryPath + "\\skills\\gw2_uncategorized_skills.json";
    if (std::filesystem::exists(uncategorizedSkillsFilePath))
    {
        LoadMasterSkillDatabase(uncategorizedSkillsFilePath);
    }
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

const SkillDefinitionMetadata* RotationCatalog::FindSkillMetadataByName(const std::string& skillName) const
{
    auto nameIterator = skillNameToIdentifierMap.find(skillName);
    if (nameIterator != skillNameToIdentifierMap.end())
    {
        return FindSkillMetadata(nameIterator->second);
    }

    for (const auto& databaseEntry : masterSkillDatabase)
    {
        if (databaseEntry.second.skillName == skillName)
        {
            return &databaseEntry.second;
        }
    }

    return nullptr;
}

uint32_t RotationCatalog::GetSkillCooldownSeconds(uint32_t skillIdentifier, const std::string& skillName) const
{
    const SkillDefinitionMetadata* metadata = FindSkillMetadata(skillIdentifier);
    if (metadata != nullptr && metadata->cooldownSeconds > 0)
    {
        return metadata->cooldownSeconds;
    }

    if (!skillName.empty())
    {
        metadata = FindSkillMetadataByName(skillName);
        if (metadata != nullptr && metadata->cooldownSeconds > 0)
        {
            return metadata->cooldownSeconds;
        }
    }

    return 0;
}

uint32_t RotationCatalog::GetSkillCastTimeMilliseconds(uint32_t skillIdentifier, const std::string& skillName) const
{
    const SkillDefinitionMetadata* metadata = FindSkillMetadata(skillIdentifier);
    if (metadata != nullptr && metadata->castTimeMilliseconds > 0)
    {
        return metadata->castTimeMilliseconds;
    }

    if (!skillName.empty())
    {
        metadata = FindSkillMetadataByName(skillName);
        if (metadata != nullptr && metadata->castTimeMilliseconds > 0)
        {
            return metadata->castTimeMilliseconds;
        }
    }

    return 0;
}

uint32_t RotationCatalog::GetSkillEnergyCost(uint32_t skillIdentifier, const std::string& skillName) const
{
    const SkillDefinitionMetadata* metadata = FindSkillMetadata(skillIdentifier);
    if (metadata != nullptr && metadata->energyCost > 0)
    {
        return metadata->energyCost;
    }

    if (!skillName.empty())
    {
        metadata = FindSkillMetadataByName(skillName);
        if (metadata != nullptr && metadata->energyCost > 0)
        {
            return metadata->energyCost;
        }
    }

    return 0;
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
        if (idText.find('-') != std::string::npos)
        {
            scanPosition = commaAfterId + 1;
            continue;
        }

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
        metadata.cooldownSeconds = 0;
        metadata.castTimeMilliseconds = 0;
        metadata.energyCost = 0;
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
            skillNameToIdentifierMap[metadata.skillName] = skillIdentifier;

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

        size_t castTimePos = skillBlock.find("\"cast_time\":");
        if (castTimePos != std::string::npos)
        {
            size_t castTimeComma = skillBlock.find_first_of(",}", castTimePos);
            if (castTimeComma != std::string::npos)
            {
                std::string castTimeText = skillBlock.substr(castTimePos + 12, castTimeComma - (castTimePos + 12));
                try
                {
                    float castTimeSeconds = std::stof(castTimeText);
                    metadata.castTimeMilliseconds = static_cast<uint32_t>(castTimeSeconds * 1000.0f);
                }
                catch (...) {}
            }
        }

        size_t energyPos = skillBlock.find("\"energy\":");
        if (energyPos == std::string::npos)
        {
            energyPos = skillBlock.find("\"energy_cost\":");
        }
        if (energyPos != std::string::npos)
        {
            size_t energyColon = skillBlock.find(':', energyPos);
            if (energyColon != std::string::npos)
            {
                size_t energyComma = skillBlock.find_first_of(",}", energyColon);
                if (energyComma != std::string::npos)
                {
                    std::string energyText = skillBlock.substr(energyColon + 1, energyComma - (energyColon + 1));
                    try
                    {
                        metadata.energyCost = static_cast<uint32_t>(std::stoul(energyText));
                    }
                    catch (...) {}
                }
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
                if (profile.buildDisplayName.find("Beginner") == std::string::npos &&
                    (profile.damageCategory == DamageTypeCategory::ConditionDamage || profile.buildDisplayName.find("Condition") != std::string::npos))
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

    else if (lowerStem.find("paragon") != std::string::npos)

    {

        profession = "Warrior";

        specialization = "Paragon";

        professionIdentifier = 2;

        specializationIdentifier = 73;

    }

    else if (lowerStem.find("amalgam") != std::string::npos)

    {

        profession = "Engineer";

        specialization = "Amalgam";

        professionIdentifier = 3;

        specializationIdentifier = 74;

    }

    else if (lowerStem.find("antiquary") != std::string::npos)

    {

        profession = "Thief";

        specialization = "Antiquary";

        professionIdentifier = 5;

        specializationIdentifier = 75;

    }

    else if (lowerStem.find("evoker") != std::string::npos)

    {

        profession = "Elementalist";

        specialization = "Evoker";

        professionIdentifier = 6;

        specializationIdentifier = 76;

    }

    else if (lowerStem.find("conduit") != std::string::npos)

    {

        profession = "Revenant";

        specialization = "Conduit";

        professionIdentifier = 9;

        specializationIdentifier = 77;

    }

    else if (lowerStem.find("luminary") != std::string::npos)

    {

        profession = "Guardian";

        specialization = "Luminary";

        professionIdentifier = 1;

        specializationIdentifier = 78;

    }

    else if (lowerStem.find("galeshot") != std::string::npos)

    {

        profession = "Ranger";

        specialization = "Galeshot";

        professionIdentifier = 4;

        specializationIdentifier = 79;

    }

    else if (lowerStem.find("troubadour") != std::string::npos)

    {

        profession = "Mesmer";

        specialization = "Troubadour";

        professionIdentifier = 7;

        specializationIdentifier = 80;

    }

    else if (lowerStem.find("ritualist") != std::string::npos)

    {

        profession = "Necromancer";

        specialization = "Ritualist";

        professionIdentifier = 8;

        specializationIdentifier = 81;

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

    else if (lowerStem.find("mesmer") != std::string::npos)

    {

        profession = "Mesmer";

        specialization = "Core Mesmer";

        professionIdentifier = 7;

        specializationIdentifier = 0;

    }

    else if (lowerStem.find("revenant") != std::string::npos)

    {

        profession = "Revenant";

        specialization = "Core Revenant";

        professionIdentifier = 9;

        specializationIdentifier = 0;

    }

    else if (lowerStem.find("necromancer") != std::string::npos)

    {

        profession = "Necromancer";

        specialization = "Core Necromancer";

        professionIdentifier = 8;

        specializationIdentifier = 0;

    }

    else if (lowerStem.find("engineer") != std::string::npos)

    {

        profession = "Engineer";

        specialization = "Core Engineer";

        professionIdentifier = 3;

        specializationIdentifier = 0;

    }

    else if (lowerStem.find("ranger") != std::string::npos)

    {

        profession = "Ranger";

        specialization = "Core Ranger";

        professionIdentifier = 4;

        specializationIdentifier = 0;

    }

    else if (lowerStem.find("elementalist") != std::string::npos)

    {

        profession = "Elementalist";

        specialization = "Core Elementalist";

        professionIdentifier = 6;

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



                    size_t bracketOpen = displayName.find('[');

                    if (bracketOpen != std::string::npos)

                    {

                        size_t bracketClose = displayName.find(']', bracketOpen);

                        if (bracketClose != std::string::npos)

                        {

                            displayName.erase(bracketOpen, bracketClose - bracketOpen + 1);

                            while (!displayName.empty() && displayName.back() == ' ')

                            {

                                displayName.pop_back();

                            }

                        }

                    }
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

        size_t professionPos = fileContent.find("\"profession\":", metadataPosition);
        if (professionPos != std::string::npos)
        {
            size_t firstQuote = fileContent.find('"', professionPos + 13);
            if (firstQuote != std::string::npos)
            {
                size_t secondQuote = fileContent.find('"', firstQuote + 1);
                if (secondQuote != std::string::npos)
                {
                    profession = fileContent.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
                }
            }
        }

        size_t specPos = fileContent.find("\"elite_spec\":", metadataPosition);
        if (specPos != std::string::npos)
        {
            size_t firstQuote = fileContent.find('"', specPos + 13);
            if (firstQuote != std::string::npos)
            {
                size_t secondQuote = fileContent.find('"', firstQuote + 1);
                if (secondQuote != std::string::npos)
                {
                    specialization = fileContent.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
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

    size_t equipmentPosition = fileContent.find("\"equipment\":");
    if (equipmentPosition != std::string::npos)
    {
        size_t attributesPosition = fileContent.find("\"armor_attributes\":", equipmentPosition);
        if (attributesPosition != std::string::npos)
        {
            size_t firstQuote = fileContent.find('"', attributesPosition + 19);
            if (firstQuote != std::string::npos)
            {
                size_t secondQuote = fileContent.find('"', firstQuote + 1);
                if (secondQuote != std::string::npos && secondQuote > firstQuote + 1)
                {
                    armorAttributes = fileContent.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
                }
            }
        }

        size_t runesPosition = fileContent.find("\"runes\":", equipmentPosition);
        if (runesPosition != std::string::npos)
        {
            size_t firstQuote = fileContent.find('"', runesPosition + 8);
            if (firstQuote != std::string::npos)
            {
                size_t secondQuote = fileContent.find('"', firstQuote + 1);
                if (secondQuote != std::string::npos && secondQuote > firstQuote + 1)
                {
                    upgradeRunes = fileContent.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
                }
            }
        }

        size_t sigilsPosition = fileContent.find("\"sigils\":", equipmentPosition);
        if (sigilsPosition != std::string::npos)
        {
            size_t firstQuote = fileContent.find('"', sigilsPosition + 9);
            if (firstQuote != std::string::npos)
            {
                size_t secondQuote = fileContent.find('"', firstQuote + 1);
                if (secondQuote != std::string::npos && secondQuote > firstQuote + 1)
                {
                    sigils = fileContent.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
                }
            }
        }

        size_t relicPosition = fileContent.find("\"relic\":", equipmentPosition);
        if (relicPosition != std::string::npos)
        {
            size_t firstQuote = fileContent.find('"', relicPosition + 8);
            if (firstQuote != std::string::npos)
            {
                size_t secondQuote = fileContent.find('"', firstQuote + 1);
                if (secondQuote != std::string::npos && secondQuote > firstQuote + 1)
                {
                    relic = fileContent.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
                }
            }
        }

        size_t foodPosition = fileContent.find("\"food\":", equipmentPosition);
        if (foodPosition != std::string::npos)
        {
            size_t firstQuote = fileContent.find('"', foodPosition + 7);
            if (firstQuote != std::string::npos)
            {
                size_t secondQuote = fileContent.find('"', firstQuote + 1);
                if (secondQuote != std::string::npos && secondQuote > firstQuote + 1)
                {
                    food = fileContent.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
                }
            }
        }

        size_t utilityPosition = fileContent.find("\"utility\":", equipmentPosition);
        if (utilityPosition != std::string::npos)
        {
            size_t firstQuote = fileContent.find('"', utilityPosition + 10);
            if (firstQuote != std::string::npos)
            {
                size_t secondQuote = fileContent.find('"', firstQuote + 1);
                if (secondQuote != std::string::npos && secondQuote > firstQuote + 1)
                {
                    utility = fileContent.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
                }
            }
        }
    }

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

