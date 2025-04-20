#ifndef RAYDIAL_I18N_H
#define RAYDIAL_I18N_H

#include <raylib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration for RayDialTextSegment to avoid circular dependencies
typedef struct RayDialTextSegment RayDialTextSegment;

// Structure to hold translation pairs
typedef struct RayDialTranslationEntry {
    const char* key;
    const char* value;
    struct RayDialTranslationEntry* next;
} RayDialTranslationEntry;

// Structure to hold language data
typedef struct RayDialLanguage {
    const char* languageCode;
    const char* languageName;
    RayDialTranslationEntry* translations;
    struct RayDialLanguage* next;
} RayDialLanguage;

// Localization manager
typedef struct RayDialI18N {
    RayDialLanguage* languages;
    RayDialLanguage* currentLanguage;
    bool useStyledTextParsing;
} RayDialI18N;

// Function declarations
RayDialI18N* CreateI18NManager(void);
void FreeI18NManager(RayDialI18N* manager);

// Language management
bool AddLanguage(RayDialI18N* manager, const char* languageCode, const char* languageName);
bool SetCurrentLanguage(RayDialI18N* manager, const char* languageCode);
RayDialLanguage* GetCurrentLanguage(RayDialI18N* manager);
const char* GetCurrentLanguageCode(RayDialI18N* manager);
const char* GetCurrentLanguageName(RayDialI18N* manager);

// Translation management
bool AddTranslation(RayDialI18N* manager, const char* languageCode, const char* key, const char* value);
bool LoadTranslationsFromFile(RayDialI18N* manager, const char* languageCode, const char* filename);
bool SaveTranslationsToFile(RayDialI18N* manager, const char* languageCode, const char* filename);

// Text retrieval
const char* GetLocalizedText(RayDialI18N* manager, const char* key);
RayDialTextSegment* GetLocalizedStyledText(RayDialI18N* manager, const char* key, Color defaultColor, float defaultFontSize);

// Configuration
void SetUseStyledTextParsing(RayDialI18N* manager, bool useStyledText);

#ifdef __cplusplus
}
#endif

#endif // RAYDIAL_I18N_H 