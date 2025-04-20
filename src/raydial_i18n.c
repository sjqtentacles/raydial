#include "raydial_i18n.h"
#include "raydial.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Create a new localization manager
RayDialI18N* CreateI18NManager(void) {
    RayDialI18N* manager = (RayDialI18N*)malloc(sizeof(RayDialI18N));
    if (manager) {
        manager->languages = NULL;
        manager->currentLanguage = NULL;
        manager->useStyledTextParsing = true;
    }
    return manager;
}

// Free all resources associated with the localization manager
void FreeI18NManager(RayDialI18N* manager) {
    if (!manager) return;
    
    // Free all languages and their translation entries
    RayDialLanguage* lang = manager->languages;
    while (lang) {
        RayDialLanguage* nextLang = lang->next;
        
        // Free all translations in this language
        RayDialTranslationEntry* entry = lang->translations;
        while (entry) {
            RayDialTranslationEntry* nextEntry = entry->next;
            
            // Free the entry itself (but not the strings, as they might be static)
            free(entry);
            
            entry = nextEntry;
        }
        
        // Free the language itself (but not the code and name, as they might be static)
        free(lang);
        
        lang = nextLang;
    }
    
    // Free the manager itself
    free(manager);
}

// Add a new language to the manager
bool AddLanguage(RayDialI18N* manager, const char* languageCode, const char* languageName) {
    if (!manager || !languageCode || !languageName) return false;
    
    // Check if the language already exists
    RayDialLanguage* lang = manager->languages;
    while (lang) {
        if (strcmp(lang->languageCode, languageCode) == 0) {
            return false; // Language already exists
        }
        lang = lang->next;
    }
    
    // Create a new language
    RayDialLanguage* newLang = (RayDialLanguage*)malloc(sizeof(RayDialLanguage));
    if (!newLang) return false;
    
    newLang->languageCode = languageCode;
    newLang->languageName = languageName;
    newLang->translations = NULL;
    newLang->next = NULL;
    
    // Add the language to the list
    if (!manager->languages) {
        manager->languages = newLang;
    } else {
        lang = manager->languages;
        while (lang->next) {
            lang = lang->next;
        }
        lang->next = newLang;
    }
    
    // If this is the first language, set it as the current language
    if (!manager->currentLanguage) {
        manager->currentLanguage = newLang;
    }
    
    return true;
}

// Set the current language by language code
bool SetCurrentLanguage(RayDialI18N* manager, const char* languageCode) {
    if (!manager || !languageCode) return false;
    
    RayDialLanguage* lang = manager->languages;
    while (lang) {
        if (strcmp(lang->languageCode, languageCode) == 0) {
            manager->currentLanguage = lang;
            return true;
        }
        lang = lang->next;
    }
    
    return false; // Language not found
}

// Get the current language
RayDialLanguage* GetCurrentLanguage(RayDialI18N* manager) {
    if (!manager) return NULL;
    return manager->currentLanguage;
}

// Get the current language code
const char* GetCurrentLanguageCode(RayDialI18N* manager) {
    if (!manager || !manager->currentLanguage) return NULL;
    return manager->currentLanguage->languageCode;
}

// Get the current language name
const char* GetCurrentLanguageName(RayDialI18N* manager) {
    if (!manager || !manager->currentLanguage) return NULL;
    return manager->currentLanguage->languageName;
}

// Add a translation to a language
bool AddTranslation(RayDialI18N* manager, const char* languageCode, const char* key, const char* value) {
    if (!manager || !languageCode || !key || !value) return false;
    
    // Find the language
    RayDialLanguage* lang = manager->languages;
    while (lang) {
        if (strcmp(lang->languageCode, languageCode) == 0) {
            break;
        }
        lang = lang->next;
    }
    
    if (!lang) return false; // Language not found
    
    // Check if the key already exists
    RayDialTranslationEntry* entry = lang->translations;
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            // Update the value
            entry->value = value;
            return true;
        }
        entry = entry->next;
    }
    
    // Create a new translation entry
    RayDialTranslationEntry* newEntry = (RayDialTranslationEntry*)malloc(sizeof(RayDialTranslationEntry));
    if (!newEntry) return false;
    
    newEntry->key = key;
    newEntry->value = value;
    newEntry->next = NULL;
    
    // Add the entry to the language
    if (!lang->translations) {
        lang->translations = newEntry;
    } else {
        entry = lang->translations;
        while (entry->next) {
            entry = entry->next;
        }
        entry->next = newEntry;
    }
    
    return true;
}

// Load translations from a file
bool LoadTranslationsFromFile(RayDialI18N* manager, const char* languageCode, const char* filename) {
    if (!manager || !languageCode || !filename) return false;
    
    FILE* file = fopen(filename, "r");
    if (!file) return false;
    
    char line[1024];
    char key[512];
    char value[512];
    
    // Find the language
    RayDialLanguage* lang = NULL;
    RayDialLanguage* current = manager->languages;
    while (current) {
        if (strcmp(current->languageCode, languageCode) == 0) {
            lang = current;
            break;
        }
        current = current->next;
    }
    
    if (!lang) {
        fclose(file);
        return false; // Language not found
    }
    
    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;
        
        // Parse key=value format
        char* separator = strchr(line, '=');
        if (!separator) continue;
        
        // Split the line into key and value
        int keyLength = separator - line;
        if (keyLength >= sizeof(key)) keyLength = sizeof(key) - 1;
        
        strncpy(key, line, keyLength);
        key[keyLength] = '\0';
        
        // Trim whitespace from key
        char* end = key + keyLength - 1;
        while (end > key && (*end == ' ' || *end == '\t')) {
            *end = '\0';
            end--;
        }
        
        // Get value part
        separator++; // Move past '='
        // Trim leading whitespace from value
        while (*separator == ' ' || *separator == '\t') {
            separator++;
        }
        
        // Copy value and handle line endings
        size_t valueLength = strlen(separator);
        if (valueLength >= sizeof(value)) valueLength = sizeof(value) - 1;
        
        strncpy(value, separator, valueLength);
        value[valueLength] = '\0';
        
        // Remove newline character if present
        end = value + valueLength - 1;
        while (end >= value && (*end == '\n' || *end == '\r')) {
            *end = '\0';
            end--;
        }
        
        // Allocate permanent storage for the key and value
        char* keyCopy = strdup(key);
        char* valueCopy = strdup(value);
        
        if (keyCopy && valueCopy) {
            // Add the translation
            AddTranslation(manager, languageCode, keyCopy, valueCopy);
        } else {
            // Memory allocation failed
            if (keyCopy) free(keyCopy);
            if (valueCopy) free(valueCopy);
        }
    }
    
    fclose(file);
    return true;
}

// Save translations to a file
bool SaveTranslationsToFile(RayDialI18N* manager, const char* languageCode, const char* filename) {
    if (!manager || !languageCode || !filename) return false;
    
    // Find the language
    RayDialLanguage* lang = NULL;
    RayDialLanguage* current = manager->languages;
    while (current) {
        if (strcmp(current->languageCode, languageCode) == 0) {
            lang = current;
            break;
        }
        current = current->next;
    }
    
    if (!lang) return false; // Language not found
    
    FILE* file = fopen(filename, "w");
    if (!file) return false;
    
    // Write a header
    fprintf(file, "# Translations for %s (%s)\n", lang->languageName, lang->languageCode);
    fprintf(file, "# Format: key=value\n\n");
    
    // Write all translations
    RayDialTranslationEntry* entry = lang->translations;
    while (entry) {
        fprintf(file, "%s=%s\n", entry->key, entry->value);
        entry = entry->next;
    }
    
    fclose(file);
    return true;
}

// Get localized text for a key
const char* GetLocalizedText(RayDialI18N* manager, const char* key) {
    if (!manager || !key || !manager->currentLanguage) return key;
    
    // Search for the key in the current language
    RayDialTranslationEntry* entry = manager->currentLanguage->translations;
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    
    return key; // Return the key if no translation is found
}

// Get localized styled text for a key
RayDialTextSegment* GetLocalizedStyledText(RayDialI18N* manager, const char* key, Color defaultColor, float defaultFontSize) {
    if (!manager || !key || !manager->currentLanguage) return NULL;
    
    // First get the localized text
    const char* localizedText = GetLocalizedText(manager, key);
    
    // If we don't want to parse styled text or the text is the same as the key (not found),
    // return a simple segment with the text
    if (!manager->useStyledTextParsing || localizedText == key) {
        RayDialTextSegment* segment = (RayDialTextSegment*)malloc(sizeof(RayDialTextSegment));
        if (!segment) return NULL;
        
        segment->text = strdup(localizedText);
        segment->styles = NULL;
        segment->next = NULL;
        
        return segment;
    }
    
    // Parse the text for styles
    return ParseStyledText(localizedText, defaultColor, defaultFontSize);
}

// Set whether to use styled text parsing
void SetUseStyledTextParsing(RayDialI18N* manager, bool useStyledText) {
    if (manager) {
        manager->useStyledTextParsing = useStyledText;
    }
} 