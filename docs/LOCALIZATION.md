# RayDial Localization System

The RayDial library includes a comprehensive localization (i18n) system that allows you to easily support multiple languages in your applications. This document explains how to use the localization features.

## Overview

The localization system is based on a key-value approach, where text strings in your application are replaced with keys that are then translated into different languages at runtime. The system supports:

- Multiple languages
- Styled text (color, size, bold, italic) in translated strings
- Loading/saving translations from/to files
- Integration with RayDial UI components

## Basic Usage

### Initialization

```c
// Create a localization manager
RayDialI18N* i18n = CreateI18NManager();

// Add supported languages
AddLanguage(i18n, "en", "English");
AddLanguage(i18n, "es", "Español");
AddLanguage(i18n, "ja", "日本語");

// Set the active language
SetCurrentLanguage(i18n, "en");
```

### Adding Translations

```c
// Add translations for each language
AddTranslation(i18n, "en", "greeting", "Hello, world!");
AddTranslation(i18n, "es", "greeting", "¡Hola, mundo!");
AddTranslation(i18n, "ja", "greeting", "こんにちは、世界！");
```

### Using Translations

```c
// Get a translated string
const char* greeting = GetLocalizedText(i18n, "greeting");
```

### Creating Localized UI Components

```c
// Create a button with localized text
RayDialComponent* button = CreateLocalizedButton(
    (Rectangle){ 100, 100, 200, 50 },
    "button_text", // Translation key
    OnButtonClick,
    userData,
    i18n
);

// Create a label with localized text
RayDialComponent* label = CreateLocalizedLabel(
    (Rectangle){ 100, 200, 300, 30 },
    "label_text", // Translation key
    true,
    i18n
);
```

### Updating UI Components When Language Changes

```c
// After changing the language
SetCurrentLanguage(i18n, "es");

// Update UI components
SetLocalizedButtonText(button, "button_text", i18n);
SetLocalizedLabelText(label, "label_text", i18n);
```

## Styled Text in Translations

You can use styled text in your translations, which will be automatically parsed:

```c
// Add translations with styling
AddTranslation(i18n, "en", "styled_text", "This is [color=red]colored[/color] and [size=large]sized[/size] text.");
AddTranslation(i18n, "es", "styled_text", "Este es texto [color=red]coloreado[/color] y de [size=large]tamaño[/size] personalizado.");

// Use styled text in dialogue
SetLocalizedPortraitDialogueStyledText(dialogueComponent, "styled_text", i18n);
```

## Loading and Saving Translations

You can load and save translations from/to files:

```c
// Load translations from a file
LoadTranslationsFromFile(i18n, "en", "translations/english.txt");

// Save translations to a file
SaveTranslationsToFile(i18n, "es", "translations/spanish.txt");
```

### Translation File Format

Translation files use a simple key=value format:

```
# English translations
greeting=Hello, world!
button_text=Click me
dialog_title=Welcome to our application

# Supports styled text
styled_text=This is [color=red]colored[/color] text
```

## Memory Management

When you're done, free the localization manager:

```c
FreeI18NManager(i18n);
```

## Font Handling for Non-Latin Languages

Raylib's default font (`GetFontDefault()`) typically only includes glyphs for basic Latin characters (like those used in English and Spanish).
If your translations include characters from other scripts (e.g., Greek, Cyrillic, Japanese, Chinese), these characters will likely render as question marks (`?`) unless you load a font that contains the necessary glyphs.

**Current Limitations:**

- RayDial UI components (`CreateLabel`, `CreatePortraitDialogue`, etc.) and the main drawing function (`DrawDialogueManager`) currently rely solely on `GetFontDefault()` for rendering text.
- There is no built-in mechanism within RayDial itself to specify custom fonts per component or for the entire dialogue manager.

**Workaround using Manual Drawing and Font Loading:**

The `examples/7_localization_example.c` demonstrates a workaround for this limitation:

1.  **Identify Required Characters:** Collect all unique characters needed for your target language(s) into a string.
2.  **Load Specific Glyphs:** Use raylib's `LoadCodepoints()` to convert the character string into an array of integer codepoints.
3.  **Load Font:** Use raylib's `LoadFontEx(fontPath, size, codepoints, codepointCount)` to load a font file (e.g., a system font like "Arial Unicode.ttf" or a bundled TTF/OTF file) containing *only* the required glyphs specified by the codepoints array. This is more efficient than loading thousands of glyphs from a large Unicode font.
4.  **Manual Component Drawing:** Instead of calling `DrawDialogueManager(manager)`, manually draw each component in your main loop using basic raylib drawing functions (`DrawTextEx`, `DrawRectangle`, etc.).
5.  **Select Font:** When drawing text elements using `DrawTextEx`, pass the appropriate loaded `Font` variable (either your custom loaded font or the original default font) based on the currently active language.
6.  **Update Styled Text Parsing:** Ensure that `SetLocalizedPortraitDialogueStyledText` (or similar functions that parse text) is called *after* switching languages if you are manually drawing styled text, as the parsing depends on the metrics of the font intended for drawing.

This approach bypasses `DrawDialogueManager` and gives you full control over which font is used for rendering, enabling support for non-Latin languages.

## Complete Example

See the `examples/7_localization_example.c` file for a complete example of using the localization system, including the font loading and manual drawing workaround for Greek text. 