#ifndef ROTATION_CATALOG_HEADER
#define ROTATION_CATALOG_HEADER

#include <vector>
#include <string>
#include <unordered_map>
#include "RotationTypes.h"

class RotationCatalog
{
public:
    RotationCatalog();

    uint32_t GetProfileCount() const;

    const BuildProfile* GetProfilePointerByIndex(uint32_t requestedProfileIndex) const;

    const BuildProfile* FindProfileByIdentifier(const std::string& requestedProfileIdentifier) const;

    uint32_t FindMatchingProfileIndex(uint32_t professionIdentifier, uint32_t specializationIdentifier) const;

    ProfessionColorRgba GetProfessionThemeColor(const std::string& professionName) const;

    std::string FormatBuildRoleName(BuildRoleCategory roleCategory) const;

    std::string FormatDamageTypeName(DamageTypeCategory damageCategory) const;

    const std::vector<BuildProfile>& GetAllProfiles() const;

    std::string GetSkillIconFileName(uint32_t skillIdentifier) const;

    std::string FindSkillIconByName(const std::string& skillName) const;

    bool LoadBenchmarkDirectory(const std::string& directoryPath);

    bool LoadMasterSkillDatabase(const std::string& skillsJsonFilePath);

    const SkillDefinitionMetadata* FindSkillMetadata(uint32_t skillIdentifier) const;

    const SkillDefinitionMetadata* FindSkillMetadataByName(const std::string& skillName) const;

    uint32_t GetSkillCooldownSeconds(uint32_t skillIdentifier, const std::string& skillName) const;

    uint32_t GetSkillCastTimeMilliseconds(uint32_t skillIdentifier, const std::string& skillName) const;

    uint32_t GetSkillEnergyCost(uint32_t skillIdentifier, const std::string& skillName) const;

    void SetStandaloneAddonDirectory(const std::string& directoryPath);

    const std::string& GetStandaloneAddonDirectory() const;

    void SyncRotationsFromExternalSource(const std::string& sourceDirectoryPath, const std::string& targetDirectoryPath);

private:
    std::string standaloneAddonDirectoryPath;
    std::vector<BuildProfile> availableBuildProfiles;
    std::unordered_map<uint32_t, std::string> skillIdentifierToIconFileNameMap;
    std::unordered_map<std::string, std::string> skillNameToIconFileNameMap;
    std::unordered_map<std::string, SkillSlotCategory> skillNameToSlotCategoryMap;
    std::unordered_map<uint32_t, SkillDefinitionMetadata> masterSkillDatabase;
    std::unordered_map<std::string, uint32_t> skillNameToIdentifierMap;

    void PopulateDefaultSkillMappings();

    bool ParseBenchmarkJsonFile(const std::string& filePath, const std::string& relativeCategory);
};

#endif
