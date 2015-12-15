#ifndef SETTINGSCONSTANTS_H
#define SETTINGSCONSTANTS_H

#include <QObject>
#include <QString>

/*
 * Sections
 */

// - Preferences dialog setting
const QString SETTINGS_PREF                 = "Pref";                 // preferences dialog, tab General
const QString SETTINGS_PREF_PROGRAMS        = "Pref_Programs";        // preferences dialog, tab General
const QString SETTINGS_PREF_EDITOR          = "Pref_Editor";          // preferences dialog, tab Editor
const QString SETTINGS_PREF_PREVIEW         = "Pref_Preview";         // preferences dialog, tab Preview
const QString SETTINGS_PREF_ASSISTANT       = "Pref_Assistant";       // preferences dialog, tab Assistant
// - Runtime setting
const QString SETTINGS_WIN                  = "Win";                  // runtime window position/state
const QString SETTINGS_WIN_RECENT_DOCUMENTS = "Win_RecentDocuments";  // runtime recently used documents

/*
 * Fields
 */

// - preferences dialog
const QString SETTINGS_PREF_GEOMETRY = "geometry";

const QString SETTINGS_USE_CUSTOM_JAVA = "use_custom_java";
const QString SETTINGS_CUSTOM_JAVA_PATH = "custom_java";

const QString SETTINGS_USE_CUSTOM_PLANTUML = "use_custom_plantuml";
const QString SETTINGS_CUSTOM_PLANTUML_PATH = "custom_plantuml";

const QString SETTINGS_USE_CUSTOM_GRAPHVIZ = "use_custom_graphiz"; // keep value graphiz w/o v
const QString SETTINGS_CUSTOM_GRAPHVIZ_PATH = "custom_graphiz";    // keep value graphiz w/o v

const QString SETTINGS_ASSISTANT_XML_PATH = "assistant_xml";
const QString SETTINGS_ASSISTANT_FONT = "font";

const QString SETTINGS_USE_CACHE = "use_cache";
const QString SETTINGS_USE_CUSTOM_CACHE = "use_custom_cache";
const QString SETTINGS_CUSTOM_CACHE_PATH = "custom_cache";
const QString SETTINGS_CACHE_MAX_SIZE = "cache_max_size";

const QString SETTINGS_EDITOR_FONT = "font";
const QString SETTINGS_EDITOR_INDENT = "indent";
const QString SETTINGS_EDITOR_INDENT_SIZE = "indent_size";
const QString SETTINGS_EDITOR_INDENT_WITH_SPACE = "indent_with_space";
const QString SETTINGS_USE_LAST_DOCUMENT = "use_last_document";
const QString SETTINGS_PREVIEW_REFRESH_ON_SAVE = "reafresh_on_save";
const QString SETTINGS_EDITOR_LAST_DIR = "last_work_dir";             // runtime - only text editor?

// - main window
const QString SETTINGS_WIN_GEOMETRY = "geometry";
const QString SETTINGS_WINDOW_STATE = "state";
const QString SETTINGS_SHOW_STATUSBAR = "statusbar";
const QString SETTINGS_AUTOREFRESH_ENABLED = "autorefresh_enabled";
const QString SETTINGS_AUTOREFRESH_TIMEOUT = "autorefresh_timeout";
const QString SETTINGS_AUTOSAVE_IMAGE_ENABLED = "autosave_image_enabled";
const QString SETTINGS_IMAGE_FORMAT = "image_format";

const int TIMEOUT_SCALE = 1000;

/*
 * Default values
 */
const bool    SETTINGS_USE_CUSTOM_JAVA_DEFAULT = false;
const bool    SETTINGS_USE_CUSTOM_PLANTUML_DEFAULT = false;
const bool    SETTINGS_USE_CUSTOM_GRAPHVIZ_DEFAULT = false;
#ifdef Q_OS_WIN
const QString SETTINGS_CUSTOM_JAVA_PATH_DEFAULT = "%JRE_HOME%\\bin\\java.exe";
const QString SETTINGS_CUSTOM_PLANTUML_PATH_DEFAULT = "%HOMEPATH%\\plantuml.jar";
const QString SETTINGS_CUSTOM_GRAPHVIZ_PATH_DEFAULT = "%PROGRAMFILES(X86)%\\Graphviz2.38\\bin\\dot.exe"; // %GRAPHVIZ_DOT%
#else
const QString SETTINGS_CUSTOM_JAVA_PATH_DEFAULT = "/usr/bin/java";
const QString SETTINGS_CUSTOM_PLANTUML_PATH_DEFAULT = "~/plantuml.jar";
const QString SETTINGS_CUSTOM_GRAPHVIZ_PATH_DEFAULT = "/usr/bin/dot";
#endif

const bool    SETTINGS_USE_CACHE_DEFAULT = true;
const bool    SETTINGS_USE_CUSTOM_CACHE_DEFAULT = false;
const int     SETTINGS_CACHE_MAX_SIZE_DEFAULT = 50 * 1024 * 1024; // in bytes
const int     SETTINGS_AUTOREFRESH_TIMEOUT_DEFAULT = 5000; // in miliseconds

const bool    SETTINGS_EDITOR_INDENT_DEFAULT = true;
const bool    SETTINGS_EDITOR_INDENT_WITH_SPACE_DEFAULT = true;
const int     SETTINGS_EDITOR_INDENT_SIZE_DEFAULT = 4;
const bool    SETTINGS_EDITOR_REFRESH_ON_SAVE_DEFAULT = true;
const bool    SETTINGS_USE_LAST_DOCUMENT_DEFAULT = true;
const QString SETTINGS_EDITOR_LAST_DIR_DEFAULT = "";

const bool    SETTINGS_AUTOSAVE_IMAGE_ENABLED_DEFAULT = false;

#endif // SETTINGSCONSTANTS_H
