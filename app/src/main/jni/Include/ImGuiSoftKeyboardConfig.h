#pragma once

#include <string>

/**
 * Configuration constants for ImGui Soft Keyboard
 * This file contains Android input type constants and other configuration options
 */

namespace ImGuiSoftKeyboardConfig {
    
    // Android Input Types (from android.view.inputmethod.EditorInfo)
    namespace InputTypes {
        // Class types
        constexpr int TYPE_CLASS_TEXT = 0x00000001;
        constexpr int TYPE_CLASS_NUMBER = 0x00000002;
        constexpr int TYPE_CLASS_PHONE = 0x00000003;
        constexpr int TYPE_CLASS_DATETIME = 0x00000004;
        
        // Text variations
        constexpr int TYPE_TEXT_VARIATION_NORMAL = 0x00000000;
        constexpr int TYPE_TEXT_VARIATION_URI = 0x00000010;
        constexpr int TYPE_TEXT_VARIATION_EMAIL_ADDRESS = 0x00000020;
        constexpr int TYPE_TEXT_VARIATION_EMAIL_SUBJECT = 0x00000030;
        constexpr int TYPE_TEXT_VARIATION_SHORT_MESSAGE = 0x00000040;
        constexpr int TYPE_TEXT_VARIATION_LONG_MESSAGE = 0x00000050;
        constexpr int TYPE_TEXT_VARIATION_PERSON_NAME = 0x00000060;
        constexpr int TYPE_TEXT_VARIATION_POSTAL_ADDRESS = 0x00000070;
        constexpr int TYPE_TEXT_VARIATION_PASSWORD = 0x00000080;
        constexpr int TYPE_TEXT_VARIATION_VISIBLE_PASSWORD = 0x00000090;
        constexpr int TYPE_TEXT_VARIATION_WEB_EDIT_TEXT = 0x000000a0;
        constexpr int TYPE_TEXT_VARIATION_FILTER = 0x000000b0;
        constexpr int TYPE_TEXT_VARIATION_PHONETIC = 0x000000c0;
        constexpr int TYPE_TEXT_VARIATION_WEB_EMAIL_ADDRESS = 0x000000d0;
        constexpr int TYPE_TEXT_VARIATION_WEB_PASSWORD = 0x000000e0;
        
        // Number variations
        constexpr int TYPE_NUMBER_VARIATION_NORMAL = 0x00000000;
        constexpr int TYPE_NUMBER_VARIATION_PASSWORD = 0x00000010;
        
        // Phone variations
        constexpr int TYPE_PHONE_VARIATION_NORMAL = 0x00000000;
        
        // DateTime variations
        constexpr int TYPE_DATETIME_VARIATION_NORMAL = 0x00000000;
        constexpr int TYPE_DATETIME_VARIATION_DATE = 0x00000010;
        constexpr int TYPE_DATETIME_VARIATION_TIME = 0x00000020;
    }
    
    // Keyboard flags
    namespace KeyboardFlags {
        constexpr int FLAG_AUTO_COMPLETE = 0x00000001;
        constexpr int FLAG_AUTO_CORRECT = 0x00000002;
        constexpr int FLAG_CAP_CHARACTERS = 0x00000004;
        constexpr int FLAG_CAP_SENTENCES = 0x00000008;
        constexpr int FLAG_CAP_WORDS = 0x00000010;
        constexpr int FLAG_MULTI_LINE = 0x00000020;
        constexpr int FLAG_NO_SUGGESTIONS = 0x00000040;
        constexpr int FLAG_PREDICTIVE_TEXT = 0x00000080;
        constexpr int FLAG_SPELL_CHECK = 0x00000100;
    }
    
    // Input validation patterns
    namespace ValidationPatterns {
        // Common regex patterns for validation
        const std::string EMAIL_PATTERN = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";
        const std::string PHONE_PATTERN = "^[+]?[0-9\\s\\-\\(\\)]{10,}$";
        const std::string URL_PATTERN = "^(https?:\\/\\/)?([\\da-z\\.-]+)\\.([a-z\\.]{2,6})([\\/\\w \\.-]*)*\\/?$";
        const std::string DATE_PATTERN = "^(0[1-9]|[12][0-9]|3[01])[-\\/](0[1-9]|1[012])[-\\/](19|20)\\d\\d$";
        const std::string TIME_PATTERN = "^([01]?[0-9]|2[0-3]):[0-5][0-9](:[0-5][0-9])?$";
    }
    
    // Default settings
    namespace Defaults {
        constexpr int DEFAULT_INPUT_TYPE = InputTypes::TYPE_CLASS_TEXT | InputTypes::TYPE_TEXT_VARIATION_NORMAL;
        constexpr int DEFAULT_KEYBOARD_FLAGS = KeyboardFlags::FLAG_AUTO_COMPLETE | KeyboardFlags::FLAG_AUTO_CORRECT;
        constexpr size_t DEFAULT_MAX_LENGTH = 1024;
        constexpr const char* DEFAULT_HINT = "Enter text...";
        constexpr int DEFAULT_CURSOR_POSITION = 0;
    }
    
    // Performance settings
    namespace Performance {
        constexpr int UPDATE_FREQUENCY_MS = 16; // 60 FPS
        constexpr int SYNC_DELAY_MS = 100; // Delay before syncing with Java
        constexpr size_t MAX_BUFFER_SIZE = 4096; // Maximum input buffer size
        constexpr bool ENABLE_LAZY_SYNC = true; // Only sync when needed
    }
    
    // UI settings
    namespace UI {
        constexpr float INPUT_HEIGHT = 25.0f;
        constexpr float INPUT_PADDING = 5.0f;
        constexpr float KEYBOARD_ANIMATION_DURATION = 0.3f;
        constexpr bool SHOW_KEYBOARD_STATUS = true;
        constexpr bool SHOW_INPUT_HINTS = true;
        constexpr bool ENABLE_INPUT_VALIDATION = true;
    }
    
    // Localization
    namespace Localization {
        const std::string DEFAULT_LANGUAGE = "en";
        const std::string DEFAULT_COUNTRY = "US";
        
        // Common hint texts in different languages
        const std::map<std::string, std::map<std::string, std::string>> HINT_TEXTS = {
            {"en", {
                {"text", "Enter text..."},
                {"number", "Enter number..."},
                {"email", "Enter email address..."},
                {"password", "Enter password..."},
                {"phone", "Enter phone number..."},
                {"url", "Enter URL..."},
                {"date", "Enter date..."},
                {"time", "Enter time..."}
            }},
            {"id", {
                {"text", "Masukkan teks..."},
                {"number", "Masukkan angka..."},
                {"email", "Masukkan alamat email..."},
                {"password", "Masukkan kata sandi..."},
                {"phone", "Masukkan nomor telepon..."},
                {"url", "Masukkan URL..."},
                {"date", "Masukkan tanggal..."},
                {"time", "Masukkan waktu..."}
            }}
        };
    }
    
    // Error messages
    namespace ErrorMessages {
        const std::string INITIALIZATION_FAILED = "Failed to initialize soft keyboard";
        const std::string JNI_REGISTRATION_FAILED = "Failed to register JNI methods";
        const std::string KEYBOARD_SHOW_FAILED = "Failed to show keyboard";
        const std::string KEYBOARD_HIDE_FAILED = "Failed to hide keyboard";
        const std::string TEXT_SYNC_FAILED = "Failed to sync text with Java";
        const std::string CURSOR_SYNC_FAILED = "Failed to sync cursor position";
    }
    
    // Success messages
    namespace SuccessMessages {
        const std::string INITIALIZATION_SUCCESS = "Soft keyboard initialized successfully";
        const std::string JNI_REGISTRATION_SUCCESS = "JNI methods registered successfully";
        const std::string KEYBOARD_SHOWN = "Keyboard shown successfully";
        const std::string KEYBOARD_HIDDEN = "Keyboard hidden successfully";
        const std::string TEXT_SYNCED = "Text synchronized successfully";
        const std::string CURSOR_SYNCED = "Cursor position synchronized";
    }
    
} // namespace ImGuiSoftKeyboardConfig