#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QFileInfo>
#include <QMainWindow>
#include <QMap>
#include <QMessageBox>

class QAction;
class QMenu;
class QTextEdit;
class QProcess;
class PreviewWidget;
class PreviewWidgetSvg;
class PreviewWidgetWeb;
class QTimer;
class QLabel;
class QSignalMapper;
class QToolBox;
class QListWidget;
class QListWidgetItem;
class FileCache;
class RecentDocuments;
class QSignalMapper;
class QScrollArea;
class TextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow ( bool oldPreview = false, QWidget* parent = 0 );
    ~MainWindow();

    void newDocument(const bool addDefaultCode);
    void openDocument ( const QString& path );
    bool useLastDocument();
    void openLastDocument();
    bool generateImage( const QString& filename, const QString& format, const QString& src = NULL, const bool keepPlantUmlFile = false);
    bool switchPreviewModeIfNeeded();
public slots:
    void onNewDocument();

private slots:
    void about();
    void onReferenceGuide();
    void onPlantUmlHome();
    void onPlantUmlNews();
    void onPlantUmlChanges();
    void onPlantUmlQA();
    void refresh ( bool forced = false );
    void refreshFinished();
    void changeImageFormat();
    void undo();
    void redo();
    void copyImage();

    void onAutoRefreshActionToggled ( bool state );
    void onEditorChanged();
    void onRefreshActionTriggered();
    void onPreferencesActionTriggered();
    void onOpenDocumentActionTriggered();
    void onReloadDocumentActionTriggered();
    void onSaveActionTriggered();
    void onSaveAsActionTriggered();
    void onExportImageActionTriggered();
    void onExportAsImageActionTriggered();
    void onRecentDocumentsActionTriggered ( const QString& path );
    void onAssistanItemDoubleClicked ( QListWidgetItem* item );
//s    void onSingleApplicationReceivedMessage ( const QString& message );
    void onAssistantFocus();
    void onAssistantItemInsert ( QWidget* widget );
    void onNextAssistant();
    void onPrevAssistant();
    void onAssistantItemSelectionChanged();
    void onCurrentAssistantChanged ( int index );
    void onZoomAutoFitOff();
    void onZoomAutoFitOn(bool state);

private:
    /*static*/ QIcon iconFromFile(QSize size, /*const*/ QString path, const QString data);
    /*static*/ QListWidget* newAssistantListWidget(const QSize& icon_size, QWidget* parent);
    /*static*/ QString prepareCode(const QString& codeBefore, const bool addTags = true);
    /*static*/ QString prepareCodeFinal(const QString& codeBefore);

    enum ImageFormat { SvgFormat, PngFormat };

    void closeEvent ( QCloseEvent* event );

    bool maybeSave();
    void readSettings ( bool reload = false );
    void writeSettings();
    bool saveDocument ( const QString& name );
    void exportImage ( const QString& name );
    QString makeKeyForDocument ( QByteArray current_document );

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    QIcon getIcon(const QString& name, const QString& type = "png");
    void enableUndoRedoActions();
    void addZoomActions ( QWidget* widget );

    void checkPaths();
    void reloadAssistantXml ( const QString& path );
    void insertAssistantCode ( const QString& code );    

    bool refreshFromCache();
    void updateCacheSizeInfo();
    void focusAssistant();   

    QMessageBox* m_generatingPopup = NULL;

    QLabel* m_currentImageFormatLabel;
    QLabel* m_autoRefreshLabel;
    QLabel* m_exportPathLabel;
    QLabel* m_cacheSizeLabel;

    QString m_documentPath;
    QString m_exportPath;
    QString m_lastKey;
    QByteArray m_cachedImage;

    QString m_assistantXmlPath;
    QList<QListWidget*> m_assistantWidgets;

    bool m_oldPreview;
    bool m_useLastDocument;
    bool m_useCustomJava;
    bool m_useCustomPlantUml;
    bool m_useCustomGraphviz;
    bool m_useCache;
    bool m_useCustomCache;
    bool m_refreshOnSave;
    int m_cacheMaxSize;

    QFileInfo m_javaPath;
    QFileInfo m_plantUmlPath;
    QFileInfo m_graphvizPath;
    QFileInfo m_cachePath;

    QString m_plantUmlVersion;
    QString m_javaVersion;
    QString m_graphvizVersion;

    QString m_customJavaPath;
    QString m_customPlantUmlPath;
    QString m_customGraphvizPath;
    QString m_customCachePath;

    bool m_hasValidPaths;

    QProcess* m_process;
    QMap<ImageFormat, QString> m_imageFormatNames;
    ImageFormat m_currentImageFormat;
    QTimer* m_autoRefreshTimer;
    bool m_needsRefresh;    
    bool m_refreshing; // skip refresh during refreshing

    TextEdit* m_editor;
    // the main image widget, which renders to svg or png
    // and the scroll area container used to add scroll bars
    PreviewWidget* m_previewWidget;
    QScrollArea* m_previewWidgetScrollArea;
    QToolBox* m_assistantToolBox;
    QLabel* m_assistantPreviewNotes;
    QTextEdit* m_assistantCodePreview;

    QToolBar* m_mainToolBar;
    QToolBar* m_zoomToolBar;

    QMenu* m_fileMenu;
    QAction* m_newDocumentAction;
    QAction* m_openDocumentAction;
    QAction* m_reloadDocumentAction;
    QAction* m_saveDocumentAction;
    QAction* m_saveAsDocumentAction;
    QAction* m_exportImageAction;
    QAction* m_exportAsImageAction;
    QAction* m_quitAction;

    QMenu* m_editMenu;
    QAction* m_undoAction;
    QAction* m_copyImageAction;
    QAction* m_redoAction;
    QAction* m_refreshAction;

    QMenu* m_settingsMenu;
    QAction* m_showAssistantDockAction;
    QAction* m_showAssistantInfoDockAction;
    QAction* m_showEditorDockAction;
    QAction* m_showToolbarsAction;
    QAction* m_showStatusBarAction;
    QAction* m_pngPreviewAction;
    QAction* m_svgPreviewAction;
    QAction* m_autoRefreshAction;
    QAction* m_autoSaveImageAction;
    QAction* m_preferencesAction;

    QDockWidget* m_assistantDock;
    QDockWidget* m_assistantInfoDock;

    QMenu* m_zoomMenu;
    QAction* m_zoomInAction;
    QAction* m_zoomOutAction;
    QAction* m_zoomOriginalAction;
    QAction* m_zoomFitBestAction;
    QAction* m_zoomFitWidthAction;
    QAction* m_zoomFitHeightAction;
    QAction* m_zoomAutoFitAction;

    QMenu* m_helpMenu;
    QAction* m_aboutAction;
    QAction* m_referenceGuideAction;
    QAction* m_plantUmlHome;
    QAction* m_plantUmlNews;
    QAction* m_plantUmlChanges;
    QAction* m_plantUmlQA;
    QAction* m_aboutQtAction;

    QSignalMapper* m_assistantInsertSignalMapper;

    FileCache* m_cache;
    RecentDocuments* m_recentDocuments;

    QString m_lastDir;
};

#endif // MAINWINDOW_H
