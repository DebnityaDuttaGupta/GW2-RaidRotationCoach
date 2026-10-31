#ifndef ROTATION_TYPES_HEADER
#define ROTATION_TYPES_HEADER

#include <string>
#include <vector>
#include <cstdint>

enum class PromptPresentationStyle
{
    SimpleTextRibbon,
    SkillIconsRibbon,
    ActionBarSlotHighlighter
};

enum class RotationExecutionStatus
{
    WaitingForFirstSkill,
    ActiveInCombat,
    PausedOutOfCombat,
    CompletedRotation
};

enum class BuildRoleCategory
{
    PureDamagePerSecond,
    BoonAlacrity,
    BoonQuickness,
    HealSupport,
    HybridSupportDamage
};

enum class DamageTypeCategory
{
    PowerDamage,
    ConditionDamage,
    CelestialHybrid
};

enum class SkillSlotCategory
{
    WeaponSkillOne,
    WeaponSkillTwo,
    WeaponSkillThree,
    WeaponSkillFour,
    WeaponSkillFive,
    HealingSkill,
    UtilitySkillOne,
    UtilitySkillTwo,
    UtilitySkillThree,
    EliteSkill,
    ProfessionSkillOne,
    ProfessionSkillTwo,
    ProfessionSkillThree,
    ProfessionSkillFour,
    ProfessionSkillFive,
    WeaponSwap,
    DodgeAction,
    SpecialAction
};

struct RotationStep
{
    uint32_t stepIndex;
    uint32_t skillIdentifier;
    std::string skillName;
    std::string defaultKeybindName;
    SkillSlotCategory slotCategory;
    uint32_t expectedDurationMilliseconds;
    uint32_t queueWindowBufferMilliseconds;
};

struct SkillDefinitionMetadata
{
    uint32_t skillIdentifier;
    std::string skillName;
    std::string iconFileName;
    SkillSlotCategory slotCategory;
    uint32_t rechargeSeconds;
    bool isAutoAttack;
};

struct ExecutionMistake
{
    uint64_t timestampMilliseconds;
    uint32_t stepIndex;
    std::string expectedSkillName;
    std::string actualInputName;
};

struct BenchmarkEvaluation
{
    double currentAverageGapMilliseconds;
    double benchmarkEightyFivePercentMilliseconds;
    double benchmarkNinetyFivePercentMilliseconds;
    double pacingDeltaMilliseconds;
    uint32_t successfulCastCount;
    uint32_t mistakeCount;
};

struct EquipmentLoadout
{
    std::string primaryWeaponSetName;
    std::string secondaryWeaponSetName;
    std::string armorAttributeName;
    std::string upgradeRuneName;
    std::string weaponSigilNames;
    std::string equipmentRelicName;
    std::string foodConsumableName;
    std::string utilityConsumableName;
};

struct ProfessionColorRgba
{
    float red;
    float green;
    float blue;
    float alpha;
};

struct BuildProfile
{
    std::string profileIdentifier;
    std::string buildDisplayName;
    std::string professionName;
    std::string specializationName;
    uint32_t professionIdentifierNumber;
    uint32_t specializationIdentifierNumber;
    BuildRoleCategory roleCategory;
    DamageTypeCategory damageCategory;
    EquipmentLoadout equipment;
    std::vector<RotationStep> sequenceSteps;
    std::string chatTemplateCode;
    std::string snowCrowsGuideUrl;
};

#endif
