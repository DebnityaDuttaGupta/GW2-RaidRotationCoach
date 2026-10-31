#ifndef KEYBIND_DETECTOR_HEADER
#define KEYBIND_DETECTOR_HEADER

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <windows.h>
#include "RotationTypes.h"

class KeybindDetector
{
public:
    KeybindDetector();

    std::string TranslateVirtualKeyToString(uint32_t virtualKeyCode);

    std::string TranslateGameBindButtonCodeToString(uint32_t buttonCode);

    void AssociateKeyWithSkillSlot(const std::string& skillSlotName, const std::string& detectedKeyName);

    void RemoveBindingFromSlot(const std::string& skillSlotName, const std::string& keyNameToRemove);

    bool HasBindingForSlot(const std::string& skillSlotName, const std::string& keyName) const;

    bool MatchesSlotKeybind(const std::string& skillSlotName, const std::string& pressedKeyName, const std::string& defaultFallbackKeybind);

    std::string LookupKeybindForSkillSlot(const std::string& skillSlotName, const std::string& defaultFallbackKeybind);

    std::string GetFormattedKeybindText(const std::string& skillSlotName, const std::string& defaultFallbackKeybind);

    const std::unordered_map<std::string, std::vector<std::string>>& GetAllSlotKeybinds() const;

    void ResetToDefaultKeybindings();

    bool LoadFromGameBindsXmlFile(const std::string& xmlFilePath);

    bool IsKeybindAutoLearningEnabled() const;

    void SetKeybindAutoLearningEnabled(bool enabledState);

    std::string GetSlotNameFromCategory(SkillSlotCategory slotCategory) const;

    SkillSlotCategory GetCategoryFromSlotName(const std::string& slotName) const;

    void SetPrimaryBindingForSlot(const std::string& skillSlotName, const std::string& keyName);

    void SetSecondaryBindingForSlot(const std::string& skillSlotName, const std::string& keyName);

    std::string GetPrimaryBindingForSlot(const std::string& skillSlotName) const;

    std::string GetSecondaryBindingForSlot(const std::string& skillSlotName) const;

    void ClearBindingsForSlot(const std::string& skillSlotName);

    bool IsKeyBoundToAnyRotationSlot(const std::string& keyName) const;

    std::string FormatKeyDisplayName(const std::string& rawKeyName) const;

private:
    bool autoLearningEnabled;
    std::unordered_map<std::string, std::vector<std::string>> slotToBoundKeysCollection;

    void AddBindingToSlot(const std::string& slotName, const std::string& keyName);
};

#endif
