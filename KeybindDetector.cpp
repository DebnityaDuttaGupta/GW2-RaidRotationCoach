#include "KeybindDetector.h"
#include <fstream>
#include <sstream>
#include <algorithm>

KeybindDetector::KeybindDetector()
{
    autoLearningEnabled = true;

    ResetToDefaultKeybindings();
}

void KeybindDetector::ResetToDefaultKeybindings()
{
    slotToBoundKeysCollection.clear();

    AddBindingToSlot("WeaponSkill1", "1");
    AddBindingToSlot("WeaponSkill2", "2");
    AddBindingToSlot("WeaponSkill3", "3");
    AddBindingToSlot("WeaponSkill4", "4");
    AddBindingToSlot("WeaponSkill5", "5");

    AddBindingToSlot("HealingSkill", "6");
    AddBindingToSlot("HealingSkill", "Q");

    AddBindingToSlot("UtilitySkill1", "7");
    AddBindingToSlot("UtilitySkill1", "Mouse5");

    AddBindingToSlot("UtilitySkill2", "8");
    AddBindingToSlot("UtilitySkill2", "Mouse4");

    AddBindingToSlot("UtilitySkill3", "9");
    AddBindingToSlot("UtilitySkill3", "Z");

    AddBindingToSlot("EliteSkill", "0");
    AddBindingToSlot("EliteSkill", "R");

    AddBindingToSlot("ProfessionSkill1", "F1");
    AddBindingToSlot("ProfessionSkill2", "F2");
    AddBindingToSlot("ProfessionSkill3", "F3");
    AddBindingToSlot("ProfessionSkill4", "F4");
    AddBindingToSlot("ProfessionSkill4", "E");
    AddBindingToSlot("ProfessionSkill5", "B");
    AddBindingToSlot("ProfessionSkill5", "F5");

    AddBindingToSlot("WeaponSwap", "`");
    AddBindingToSlot("Dodge", "V");
    AddBindingToSlot("SpecialAction", "N");
    AddBindingToSlot("SpecialAction", "MiddleMouse");
}

void KeybindDetector::AddBindingToSlot(const std::string& slotName, const std::string& keyName)
{
    if (slotName.empty() || keyName.empty())
    {
        return;
    }

    auto& keyList = slotToBoundKeysCollection[slotName];

    for (const auto& existingKey : keyList)
    {
        if (existingKey == keyName)
        {
            return;
        }
    }

    keyList.push_back(keyName);
}

std::string KeybindDetector::TranslateGameBindButtonCodeToString(uint32_t buttonCode)
{
    if (buttonCode >= 48 && buttonCode <= 57)
    {
        return std::string(1, static_cast<char>(buttonCode));
    }

    if (buttonCode >= 65 && buttonCode <= 90)
    {
        return std::string(1, static_cast<char>(buttonCode));
    }

    if (buttonCode >= 32 && buttonCode <= 43)
    {
        uint32_t functionNumber = buttonCode - 32 + 1;
        return "F" + std::to_string(functionNumber);
    }

    if (buttonCode == 17)
    {
        return "`";
    }

    if (buttonCode == 21)
    {
        return "Space";
    }

    if (buttonCode == 22)
    {
        return "Tab";
    }

    if (buttonCode == 97)
    {
        return "Mouse4";
    }

    if (buttonCode == 98)
    {
        return "Mouse5";
    }

    if (buttonCode == 96)
    {
        return "MiddleMouse";
    }

    return TranslateVirtualKeyToString(buttonCode);
}

std::string KeybindDetector::TranslateVirtualKeyToString(uint32_t virtualKeyCode)
{
    if (virtualKeyCode >= '0' && virtualKeyCode <= '9')
    {
        return std::string(1, static_cast<char>(virtualKeyCode));
    }

    if (virtualKeyCode >= 'A' && virtualKeyCode <= 'Z')
    {
        return std::string(1, static_cast<char>(virtualKeyCode));
    }

    if (virtualKeyCode >= VK_F1 && virtualKeyCode <= VK_F12)
    {
        uint32_t functionNumber = virtualKeyCode - VK_F1 + 1;
        return "F" + std::to_string(functionNumber);
    }

    if (virtualKeyCode >= VK_NUMPAD0 && virtualKeyCode <= VK_NUMPAD9)
    {
        uint32_t numpadDigit = virtualKeyCode - VK_NUMPAD0;
        return "Num" + std::to_string(numpadDigit);
    }

    switch (virtualKeyCode)
    {
        case VK_OEM_3:
            return "`";
        case VK_SPACE:
            return "Space";
        case VK_TAB:
            return "Tab";
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
            return "Shift";
        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL:
            return "Ctrl";
        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU:
            return "Alt";
        case VK_OEM_MINUS:
            return "-";
        case VK_OEM_PLUS:
            return "=";
        case VK_OEM_4:
            return "[";
        case VK_OEM_6:
            return "]";
        case VK_OEM_1:
            return ";";
        case VK_OEM_7:
            return "'";
        case VK_OEM_COMMA:
            return ",";
        case VK_OEM_PERIOD:
            return ".";
        case VK_OEM_2:
            return "/";
        case VK_XBUTTON1:
            return "Mouse4";
        case VK_XBUTTON2:
            return "Mouse5";
        case VK_MBUTTON:
            return "MiddleMouse";
        default:
            break;
    }

    return "Key" + std::to_string(virtualKeyCode);
}

bool KeybindDetector::LoadFromGameBindsXmlFile(const std::string& xmlFilePath)
{
    std::ifstream fileStream(xmlFilePath);

    if (!fileStream.is_open())
    {
        return false;
    }

    std::string currentLine;

    while (std::getline(fileStream, currentLine))
    {
        if (currentLine.find("<action") == std::string::npos)
        {
            continue;
        }

        std::string actionName = "";
        size_t namePosition = currentLine.find("name=\"");
        if (namePosition != std::string::npos)
        {
            size_t nameStart = namePosition + 6;
            size_t nameEnd = currentLine.find("\"", nameStart);
            if (nameEnd != std::string::npos)
            {
                actionName = currentLine.substr(nameStart, nameEnd - nameStart);
            }
        }

        std::string actionIdentifierString = "";
        size_t identifierPosition = currentLine.find("id=\"");
        if (identifierPosition != std::string::npos)
        {
            size_t identifierStart = identifierPosition + 4;
            size_t identifierEnd = currentLine.find("\"", identifierStart);
            if (identifierEnd != std::string::npos)
            {
                actionIdentifierString = currentLine.substr(identifierStart, identifierEnd - identifierStart);
            }
        }

        std::string targetSlotName = "";

        if (actionName == "Profession Skill 5" || actionIdentifierString == "79" || actionName.find("Profession5") != std::string::npos || actionName.find("ProfessionSkill5") != std::string::npos)
        {
            targetSlotName = "ProfessionSkill5";
        }
        else if (actionName == "Profession Skill 4" || actionIdentifierString == "31" || actionName.find("Profession4") != std::string::npos || actionName.find("ProfessionSkill4") != std::string::npos)
        {
            targetSlotName = "ProfessionSkill4";
        }
        else if (actionName == "Profession Skill 3" || actionIdentifierString == "30" || actionName.find("Profession3") != std::string::npos || actionName.find("ProfessionSkill3") != std::string::npos)
        {
            targetSlotName = "ProfessionSkill3";
        }
        else if (actionName == "Profession Skill 2" || actionIdentifierString == "29" || actionName.find("Profession2") != std::string::npos || actionName.find("ProfessionSkill2") != std::string::npos)
        {
            targetSlotName = "ProfessionSkill2";
        }
        else if (actionName == "Profession Skill 1" || actionIdentifierString == "28" || actionName.find("Profession1") != std::string::npos || actionName.find("ProfessionSkill1") != std::string::npos)
        {
            targetSlotName = "ProfessionSkill1";
        }
        else if (actionName == "Healing Skill" || actionIdentifierString == "23" || actionName.find("Heal") != std::string::npos)
        {
            targetSlotName = "HealingSkill";
        }
        else if (actionName == "Utility Skill 1" || actionIdentifierString == "24" || actionName.find("Utility1") != std::string::npos)
        {
            targetSlotName = "UtilitySkill1";
        }
        else if (actionName == "Utility Skill 2" || actionIdentifierString == "25" || actionName.find("Utility2") != std::string::npos)
        {
            targetSlotName = "UtilitySkill2";
        }
        else if (actionName == "Utility Skill 3" || actionIdentifierString == "26" || actionName.find("Utility3") != std::string::npos)
        {
            targetSlotName = "UtilitySkill3";
        }
        else if (actionName == "Elite Skill" || actionIdentifierString == "27" || actionName.find("Elite") != std::string::npos)
        {
            targetSlotName = "EliteSkill";
        }
        else if (actionName == "Weapon Skill 1" || actionIdentifierString == "17" || actionName.find("Weapon1") != std::string::npos)
        {
            targetSlotName = "WeaponSkill1";
        }
        else if (actionName == "Weapon Skill 2" || actionIdentifierString == "18" || actionName.find("Weapon2") != std::string::npos)
        {
            targetSlotName = "WeaponSkill2";
        }
        else if (actionName == "Weapon Skill 3" || actionIdentifierString == "19" || actionName.find("Weapon3") != std::string::npos)
        {
            targetSlotName = "WeaponSkill3";
        }
        else if (actionName == "Weapon Skill 4" || actionIdentifierString == "20" || actionName.find("Weapon4") != std::string::npos)
        {
            targetSlotName = "WeaponSkill4";
        }
        else if (actionName == "Weapon Skill 5" || actionIdentifierString == "21" || actionName.find("Weapon5") != std::string::npos)
        {
            targetSlotName = "WeaponSkill5";
        }
        else if (actionName == "Swap Weapons" || actionName == "Weapon Swap" || actionIdentifierString == "11" || actionName.find("WeaponSwap") != std::string::npos)
        {
            targetSlotName = "WeaponSwap";
        }
        else if (actionName == "Dodge" || actionIdentifierString == "35" || actionName.find("Dodge") != std::string::npos)
        {
            targetSlotName = "Dodge";
        }
        else if (actionName == "Special Action" || actionIdentifierString == "82" || actionName.find("SpecialAction") != std::string::npos)
        {
            targetSlotName = "SpecialAction";
        }

        if (targetSlotName.empty())
        {
            continue;
        }

        size_t devicePosition = currentLine.find("device=\"");
        std::string deviceType = "";
        if (devicePosition != std::string::npos)
        {
            size_t deviceStart = devicePosition + 8;
            size_t deviceEnd = currentLine.find("\"", deviceStart);
            if (deviceEnd != std::string::npos)
            {
                deviceType = currentLine.substr(deviceStart, deviceEnd - deviceStart);
            }
        }

        size_t buttonPosition = currentLine.find("button=\"");
        if (buttonPosition != std::string::npos && deviceType != "None")
        {
            size_t buttonStart = buttonPosition + 8;
            size_t buttonEnd = currentLine.find("\"", buttonStart);
            if (buttonEnd != std::string::npos)
            {
                std::string buttonCodeString = currentLine.substr(buttonStart, buttonEnd - buttonStart);
                if (!buttonCodeString.empty())
                {
                    uint32_t buttonCode = static_cast<uint32_t>(std::stoul(buttonCodeString));
                    std::string translatedKeyString = "";

                    if (deviceType == "Mouse")
                    {
                        if (buttonCode == 1)
                        {
                            translatedKeyString = "MiddleMouse";
                        }
                        else if (buttonCode == 3)
                        {
                            translatedKeyString = "Mouse4";
                        }
                        else if (buttonCode == 4)
                        {
                            translatedKeyString = "Mouse5";
                        }
                    }
                    else
                    {
                        translatedKeyString = TranslateGameBindButtonCodeToString(buttonCode);
                    }

                    if (!translatedKeyString.empty())
                    {
                        SetPrimaryBindingForSlot(targetSlotName, translatedKeyString);
                    }
                }
            }
        }

        size_t secondDevicePosition = currentLine.find("device2=\"");
        std::string secondDeviceType = "";
        if (secondDevicePosition != std::string::npos)
        {
            size_t secondDeviceStart = secondDevicePosition + 9;
            size_t secondDeviceEnd = currentLine.find("\"", secondDeviceStart);
            if (secondDeviceEnd != std::string::npos)
            {
                secondDeviceType = currentLine.substr(secondDeviceStart, secondDeviceEnd - secondDeviceStart);
            }
        }

        size_t secondButtonPosition = currentLine.find("button2=\"");
        if (secondButtonPosition != std::string::npos && secondDeviceType != "None")
        {
            size_t secondButtonStart = secondButtonPosition + 9;
            size_t secondButtonEnd = currentLine.find("\"", secondButtonStart);
            if (secondButtonEnd != std::string::npos)
            {
                std::string secondButtonCodeString = currentLine.substr(secondButtonStart, secondButtonEnd - secondButtonStart);
                if (!secondButtonCodeString.empty())
                {
                    uint32_t secondButtonCode = static_cast<uint32_t>(std::stoul(secondButtonCodeString));
                    std::string secondTranslatedKeyString = "";

                    if (secondDeviceType == "Mouse")
                    {
                        if (secondButtonCode == 1)
                        {
                            secondTranslatedKeyString = "MiddleMouse";
                        }
                        else if (secondButtonCode == 3)
                        {
                            secondTranslatedKeyString = "Mouse4";
                        }
                        else if (secondButtonCode == 4)
                        {
                            secondTranslatedKeyString = "Mouse5";
                        }
                    }
                    else
                    {
                        secondTranslatedKeyString = TranslateGameBindButtonCodeToString(secondButtonCode);
                    }

                    if (!secondTranslatedKeyString.empty())
                    {
                        SetSecondaryBindingForSlot(targetSlotName, secondTranslatedKeyString);
                    }
                }
            }
        }
    }

    return true;
}

void KeybindDetector::SetPrimaryBindingForSlot(const std::string& skillSlotName, const std::string& keyName)
{
    if (skillSlotName.empty() || keyName.empty())
    {
        return;
    }

    auto& keyList = slotToBoundKeysCollection[skillSlotName];

    keyList.erase(std::remove(keyList.begin(), keyList.end(), keyName), keyList.end());

    if (keyList.empty())
    {
        keyList.push_back(keyName);
    }
    else
    {
        keyList.insert(keyList.begin(), keyName);
    }
}

void KeybindDetector::SetSecondaryBindingForSlot(const std::string& skillSlotName, const std::string& keyName)
{
    if (skillSlotName.empty() || keyName.empty())
    {
        return;
    }

    auto& keyList = slotToBoundKeysCollection[skillSlotName];

    keyList.erase(std::remove(keyList.begin(), keyList.end(), keyName), keyList.end());

    if (keyList.empty())
    {
        keyList.push_back("-");
        keyList.push_back(keyName);
    }
    else if (keyList.size() == 1)
    {
        keyList.push_back(keyName);
    }
    else
    {
        keyList[1] = keyName;
    }
}

void KeybindDetector::AssociateKeyWithSkillSlot(const std::string& skillSlotName, const std::string& detectedKeyName)
{
    if (detectedKeyName.empty() || skillSlotName.empty())
    {
        return;
    }

    AddBindingToSlot(skillSlotName, detectedKeyName);
}

void KeybindDetector::RemoveBindingFromSlot(const std::string& skillSlotName, const std::string& keyNameToRemove)
{
    auto iterator = slotToBoundKeysCollection.find(skillSlotName);

    if (iterator != slotToBoundKeysCollection.end())
    {
        auto& keyList = iterator->second;
        keyList.erase(std::remove(keyList.begin(), keyList.end(), keyNameToRemove), keyList.end());
    }
}

bool KeybindDetector::HasBindingForSlot(const std::string& skillSlotName, const std::string& keyName) const
{
    auto iterator = slotToBoundKeysCollection.find(skillSlotName);

    if (iterator != slotToBoundKeysCollection.end())
    {
        for (const auto& existingKey : iterator->second)
        {
            if (existingKey == keyName)
            {
                return true;
            }
        }
    }

    return false;
}

bool KeybindDetector::MatchesSlotKeybind(const std::string& skillSlotName, const std::string& pressedKeyName, const std::string& defaultFallbackKeybind)
{
    auto iterator = slotToBoundKeysCollection.find(skillSlotName);

    if (iterator != slotToBoundKeysCollection.end())
    {
        for (const auto& boundKey : iterator->second)
        {
            if (boundKey == pressedKeyName)
            {
                return true;
            }
        }
    }

    return (pressedKeyName == defaultFallbackKeybind);
}

std::string KeybindDetector::LookupKeybindForSkillSlot(const std::string& skillSlotName, const std::string& defaultFallbackKeybind)
{
    return GetFormattedKeybindText(skillSlotName, defaultFallbackKeybind);
}

std::string KeybindDetector::FormatKeyDisplayName(const std::string& rawKeyName) const
{
    if (rawKeyName == "Mouse5")
    {
        return "M5";
    }

    if (rawKeyName == "Mouse4")
    {
        return "M4";
    }

    if (rawKeyName == "MiddleMouse")
    {
        return "M3";
    }

    return rawKeyName;
}

std::string KeybindDetector::GetFormattedKeybindText(const std::string& skillSlotName, const std::string& defaultFallbackKeybind)
{
    auto iterator = slotToBoundKeysCollection.find(skillSlotName);

    if (iterator != slotToBoundKeysCollection.end() && !iterator->second.empty())
    {
        return FormatKeyDisplayName(iterator->second.front());
    }

    return FormatKeyDisplayName(defaultFallbackKeybind);
}

const std::unordered_map<std::string, std::vector<std::string>>& KeybindDetector::GetAllSlotKeybinds() const
{
    return slotToBoundKeysCollection;
}

bool KeybindDetector::IsKeybindAutoLearningEnabled() const
{
    return autoLearningEnabled;
}

void KeybindDetector::SetKeybindAutoLearningEnabled(bool enabledState)
{
    autoLearningEnabled = enabledState;
}

std::string KeybindDetector::GetSlotNameFromCategory(SkillSlotCategory slotCategory) const
{
    switch (slotCategory)
    {
        case SkillSlotCategory::WeaponSkillOne:
            return "WeaponSkill1";
        case SkillSlotCategory::WeaponSkillTwo:
            return "WeaponSkill2";
        case SkillSlotCategory::WeaponSkillThree:
            return "WeaponSkill3";
        case SkillSlotCategory::WeaponSkillFour:
            return "WeaponSkill4";
        case SkillSlotCategory::WeaponSkillFive:
            return "WeaponSkill5";
        case SkillSlotCategory::HealingSkill:
            return "HealingSkill";
        case SkillSlotCategory::UtilitySkillOne:
            return "UtilitySkill1";
        case SkillSlotCategory::UtilitySkillTwo:
            return "UtilitySkill2";
        case SkillSlotCategory::UtilitySkillThree:
            return "UtilitySkill3";
        case SkillSlotCategory::EliteSkill:
            return "EliteSkill";
        case SkillSlotCategory::ProfessionSkillOne:
            return "ProfessionSkill1";
        case SkillSlotCategory::ProfessionSkillTwo:
            return "ProfessionSkill2";
        case SkillSlotCategory::ProfessionSkillThree:
            return "ProfessionSkill3";
        case SkillSlotCategory::ProfessionSkillFour:
            return "ProfessionSkill4";
        case SkillSlotCategory::ProfessionSkillFive:
            return "ProfessionSkill5";
        case SkillSlotCategory::WeaponSwap:
            return "WeaponSwap";
        case SkillSlotCategory::DodgeAction:
            return "Dodge";
        case SkillSlotCategory::SpecialAction:
            return "SpecialAction";
        default:
            break;
    }

    return "UtilitySkill1";
}

SkillSlotCategory KeybindDetector::GetCategoryFromSlotName(const std::string& slotName) const
{
    if (slotName == "WeaponSkill1")
    {
        return SkillSlotCategory::WeaponSkillOne;
    }
    if (slotName == "WeaponSkill2")
    {
        return SkillSlotCategory::WeaponSkillTwo;
    }
    if (slotName == "WeaponSkill3")
    {
        return SkillSlotCategory::WeaponSkillThree;
    }
    if (slotName == "WeaponSkill4")
    {
        return SkillSlotCategory::WeaponSkillFour;
    }
    if (slotName == "WeaponSkill5")
    {
        return SkillSlotCategory::WeaponSkillFive;
    }
    if (slotName == "HealingSkill")
    {
        return SkillSlotCategory::HealingSkill;
    }
    if (slotName == "UtilitySkill1")
    {
        return SkillSlotCategory::UtilitySkillOne;
    }
    if (slotName == "UtilitySkill2")
    {
        return SkillSlotCategory::UtilitySkillTwo;
    }
    if (slotName == "UtilitySkill3")
    {
        return SkillSlotCategory::UtilitySkillThree;
    }
    if (slotName == "EliteSkill")
    {
        return SkillSlotCategory::EliteSkill;
    }
    if (slotName == "ProfessionSkill1")
    {
        return SkillSlotCategory::ProfessionSkillOne;
    }
    if (slotName == "ProfessionSkill2")
    {
        return SkillSlotCategory::ProfessionSkillTwo;
    }
    if (slotName == "ProfessionSkill3")
    {
        return SkillSlotCategory::ProfessionSkillThree;
    }
    if (slotName == "ProfessionSkill4")
    {
        return SkillSlotCategory::ProfessionSkillFour;
    }
    if (slotName == "ProfessionSkill5")
    {
        return SkillSlotCategory::ProfessionSkillFive;
    }
    if (slotName == "WeaponSwap")
    {
        return SkillSlotCategory::WeaponSwap;
    }
    if (slotName == "Dodge")
    {
        return SkillSlotCategory::DodgeAction;
    }
    if (slotName == "SpecialAction")
    {
        return SkillSlotCategory::SpecialAction;
    }

    return SkillSlotCategory::UtilitySkillOne;
}

std::string KeybindDetector::GetPrimaryBindingForSlot(const std::string& skillSlotName) const
{
    auto iterator = slotToBoundKeysCollection.find(skillSlotName);

    if (iterator != slotToBoundKeysCollection.end() && !iterator->second.empty())
    {
        return iterator->second[0];
    }

    return "-";
}

std::string KeybindDetector::GetSecondaryBindingForSlot(const std::string& skillSlotName) const
{
    auto iterator = slotToBoundKeysCollection.find(skillSlotName);

    if (iterator != slotToBoundKeysCollection.end() && iterator->second.size() > 1)
    {
        return iterator->second[1];
    }

    return "-";
}

void KeybindDetector::ClearBindingsForSlot(const std::string& skillSlotName)
{
    auto iterator = slotToBoundKeysCollection.find(skillSlotName);

    if (iterator != slotToBoundKeysCollection.end())
    {
        iterator->second.clear();
    }
}

bool KeybindDetector::IsKeyBoundToAnyRotationSlot(const std::string& keyName) const
{
    if (keyName.empty())
    {
        return false;
    }

    if (keyName == "`")
    {
        return true;
    }

    const std::vector<std::string> validRotationSlotNames = {
        "WeaponSkill1", "WeaponSkill2", "WeaponSkill3", "WeaponSkill4", "WeaponSkill5",
        "HealingSkill", "UtilitySkill1", "UtilitySkill2", "UtilitySkill3", "EliteSkill",
        "ProfessionSkill1", "ProfessionSkill2", "ProfessionSkill3", "ProfessionSkill4", "ProfessionSkill5",
        "WeaponSwap"
    };

    for (const auto& slotName : validRotationSlotNames)
    {
        auto slotIterator = slotToBoundKeysCollection.find(slotName);

        if (slotIterator != slotToBoundKeysCollection.end())
        {
            for (const auto& boundKey : slotIterator->second)
            {
                if (boundKey == keyName)
                {
                    return true;
                }
            }
        }
    }

    return false;
}
