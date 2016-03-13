#include "src/mainwindow.h"

#include "src/previewwidgetsvg.h"
#include "src/previewwidgetweb.h"
#include "src/preferencesdialog.h"
#include "src/assistantxmlreader.h"
#include "src/settingsconstants.h"
#include "src/filecache.h"
#include "src/recentdocuments.h"
#include "src/utils.h"
#include "src/textedit.h"
#include "src/highlighter.cpp"

#include <QFile>
#include <QFileInfo>
#include <QtGui>
#include <QtSvg>
#include <QApplication>
#include <QScrollArea>
#include <QStringList>
#include <QRegExp>
#include <QDesktopServices>

namespace {
    const char* TEXT_REFRESHING = "Refreshing...";

    const int ASSISTANT_ITEM_DATA_ROLE = Qt::UserRole;
    const int ASSISTANT_ITEM_NOTES_ROLE = Qt::UserRole + 1;
    const int ASSISTANT_ITEM_FORMAT_ROLE = Qt::UserRole + 2;

    const int MAX_RECENT_DOCUMENT_SIZE = 10;
    const int STATUSBAR_TIMEOUT = 3000; // in miliseconds
    const QString TITLE_FORMAT_STRING = "%1[*] - %2";
    const QString EXPORT_TO_MENU_FORMAT_STRING = QObject::tr("Export to %1");
    const QString EXPORT_TO_LABEL_FORMAT_STRING = QObject::tr("Export to: %1");
    const QString AUTOREFRESH_STATUS_LABEL = QObject::tr("Auto-refresh");
    const QString CACHE_SIZE_FORMAT_STRING = QObject::tr("Cache: %1");
    const QSize ASSISTANT_ICON_SIZE(128, 128);
    const QRegExp IMAGE_PNG_ONLY("ditaa|jcckit");
    const QRegExp IMAGE_SVG_ONLY("graph\\s+\\w+\\s*[{]");
} // namespace {}

MainWindow::MainWindow(bool oldPreview, QWidget* parent)
    : QMainWindow(parent)
    , m_oldPreview(oldPreview)
    , m_hasValidPaths(false)
    , m_process(0)
    , m_currentImageFormat(SvgFormat)
    , m_needsRefresh(false) {

    setWindowTitle(TITLE_FORMAT_STRING
                   .arg("")
                   .arg(qApp->applicationName())
                  );

    m_cache = new FileCache(0, this);

    m_recentDocuments = new RecentDocuments(MAX_RECENT_DOCUMENT_SIZE, this);
    connect(m_recentDocuments, SIGNAL(recentDocument(QString)), this, SLOT(onRecentDocumentsActionTriggered(QString)));

    m_autoRefreshTimer = new QTimer(this);
    connect(m_autoRefreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));

    m_imageFormatNames[SvgFormat] = "svg";
    m_imageFormatNames[PngFormat] = "png";

    if(m_oldPreview) {
        // original preview widget

        m_previewWidget = new PreviewWidgetSvg(this);

        m_previewWidgetScrollArea = new QScrollArea;
        m_previewWidgetScrollArea->setWidget(m_previewWidget);
        m_previewWidgetScrollArea->setAlignment(Qt::AlignCenter);
        m_previewWidgetScrollArea->setWidgetResizable(true);
        setCentralWidget(m_previewWidgetScrollArea);

        m_previewWidget->setScrollArea(m_previewWidgetScrollArea);
    }
    else {
        // with QWebView as preview widget

        m_previewWidget = new PreviewWidgetWeb(this);

        setCentralWidget(m_previewWidget);
    }

    createDockWindows();
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    setUnifiedTitleAndToolBarOnMac(true);

    m_assistantInsertSignalMapper = new QSignalMapper(this);
    connect(m_assistantInsertSignalMapper,
            SIGNAL(mapped(QWidget*)),
            this, SLOT(onAssistantItemInsert(QWidget*)));

    readSettings();
}

MainWindow::~MainWindow() {
}

/**
 * Renders a small icon from a SVG file.
 */
QIcon MainWindow::iconFromFile(QSize size, /*const*/ QString path, const QString code) {

    QPixmap pixmap(size);
    QPainter painter(&pixmap);
    const QRect bounding_rect(QPoint(0, 0), size);
    bool asPng = false;

    if (!path.isEmpty()) {
        painter.setRenderHint(QPainter::Antialiasing, true);

        painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter.setPen(Qt::NoPen);
        painter.drawRect(bounding_rect);

        // SVG or PNG?
        if(code.contains("ditaa", Qt::CaseInsensitive) || code.contains("jcckit", Qt::CaseInsensitive)) {
            // exception => PNG if SVG is not supported
            path = path.replace(".svg", ".png", Qt::CaseInsensitive);
            asPng = true;
        } else {
            // default SVG
            asPng = false;
        }

        if (!QFile::exists(path)) {
            // create assistant icon from assistant data

            QString src = prepareCode(code);
            QTextStream(stdout) << "render missing assistant icon " << path << endl;

            if(m_generatingPopup == NULL) {
                // first call
                m_generatingPopup = new QMessageBox(QMessageBox::Information,
                                                  qApp->applicationName(),
                                                  tr("Generating missing assistant icons.")
                                                  + "\n"
                                                  + tr("This takes a while..."));
                QTimer::singleShot(5000, m_generatingPopup, SLOT(hide()));
//                generatingPopup->show();  // shows no text :(
                m_generatingPopup->exec();
//            } else {
//                if(generatingPopup->isVisible())
//                    generatingPopup->setText(tr("Generating missing assistant icons.") + path);
            }

            if(asPng) {
                generateImage(path, "png", src);
            } else {
                generateImage(path, "svg", src);
            }
        }

        if(asPng) {
            // PNG
            QImage image(path);

//            image = image.scaled(size,Qt::KeepAspectRatio);
            if(image.width() > image.height()) {
            image = image.scaledToWidth(size.width());
            }
            else {
                image = image.scaledToHeight(size.height());
            }
            painter.drawImage(0, 0, image);
        }
        else {
            // SVG
            QSvgRenderer svg(path);
            QSize target_size = svg.defaultSize();
            target_size.scale(size, Qt::KeepAspectRatio);
            QRect target_rect = QRect(QPoint(0, 0), target_size);
            target_rect.translate(bounding_rect.center() - target_rect.center());
            svg.render(&painter, target_rect);
        }
    } else {
        // dummy icon?
        painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter.setPen(Qt::NoPen);
        painter.drawRect(bounding_rect);

        const int margin = 5;
        QRect target_rect = bounding_rect.adjusted(margin, margin, -margin, -margin);
        painter.setPen(Qt::SolidLine);
        painter.setPen(Qt::red);
        painter.drawRect(target_rect);
        painter.drawLine(target_rect.topLeft(), target_rect.bottomRight() + QPoint(1, 1));
        painter.drawLine(target_rect.bottomLeft() + QPoint(0, 1), target_rect.topRight() + QPoint(1, 0));
    }

    QIcon icon;
    icon.addPixmap(pixmap);
    return icon;
}

QListWidget* MainWindow::newAssistantListWidget(const QSize& icon_size, QWidget* parent) {
    QListWidget* view = new QListWidget(parent);
    view->setUniformItemSizes(true);
    view->setMovement(QListView::Static);
    view->setResizeMode(QListView::Adjust);
    view->setIconSize(icon_size);
    view->setViewMode(QListView::IconMode);
    return view;
}

void MainWindow::newDocument() {

    if (!maybeSave()) {
        return;
    }

    m_documentPath.clear();
    m_exportPath.clear();
    m_cachedImage.clear();
    m_exportImageAction->setText(EXPORT_TO_MENU_FORMAT_STRING.arg(""));
    m_exportPathLabel->setText(EXPORT_TO_LABEL_FORMAT_STRING.arg(""));
    m_exportPathLabel->setEnabled(false);

    QString text = prepareCode("class Foo\nclass Bar\n\nFoo -> Bar");
    m_editor->setPlainText(text);  // resets undo buffer => ok
    setWindowTitle(TITLE_FORMAT_STRING
                   .arg(tr("Untitled"))
                   .arg(qApp->applicationName())
                  );
    setWindowModified(false);
    refresh();

    enableUndoRedoActions();
}

void MainWindow::copyImage() {
    QPixmap pixmap;
    pixmap.loadFromData(m_cachedImage);
    QApplication::clipboard()->setPixmap(pixmap);
    qDebug() << "image copied into clipboard";
}

void MainWindow::undo() {
    QTextDocument* document = m_editor->document();
    document->undo();
    enableUndoRedoActions();
}

void MainWindow::redo() {
    QTextDocument* document = m_editor->document();
    document->redo();
    enableUndoRedoActions();
}

void MainWindow::about() {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("About %1").arg(qApp->applicationName()));
    msgBox.setTextFormat(Qt::RichText); //this is what makes the links clickable
    msgBox.setText(tr("The <b>%1</b> allows simple edit and preview of UML "
                      "diagrams generated with <b>PlantUML</b>."
                      "<br><br>"
                      "<a href='http://www.oracle.com/technetwork/java/index.html'>Java</a>, "
                      "<a href='http://plantuml.sourceforge.net/'>PlantUML</a> and "
                      "<a href='http://www.graphviz.org/'>Graphviz</a> must be installed before "
                      "using the editor."
                      "<br><br>"
                      "The syntaxes are described on the web pages of "
                      "<a href='http://plantuml.sourceforge.net/'>PlantUML</a>, "
                      "<a href='http://ditaa.sourceforge.net/'>ditaa</a>, "
                      "<a href='http://jcckit.sourceforge.net/index.html'>JCCKit</a> and "
                      "<a href='http://www.graphviz.org/'>Graphviz dot</a>. "
                      "See <a href='http://plantuml.sourceforge.net/PlantUML_Language_Reference_Guide.pdf'>"
                      "PlantUML_Language_Reference_Guide.pdf</a> for more information."
                      "<br><br>"
                      "%1 build is <b>" __DATE__ ", " __TIME__ "</b><br>"
                      "PlantUML version is <b>%2</b><br>"
                      "Java version is <b>%3</b><br>"
                      "Graphviz version is <b>%4</b>"
                      "<br><br>"
                      "Copyright (C) 2012-2016 Ionutz Borcoman, Jens Albers and others. "
                      "See AUTHORS for more information. "
                      "Released under <a href=\"http://www.gnu.org/licenses/gpl-3.0\">GPL version 3</a>." )
                   .arg(qApp->applicationName()) // %1
                   .arg(m_plantUmlVersion)       // %2
                   .arg(m_javaVersion)           // %3
                   .arg(m_graphvizVersion)       // %4
                  );
    msgBox.exec();
}

/**
 * Download and display the PlantUML reference guide.
 *
 * @todo Make PDF configurable to use an already downloaded file!
 *
 * @brief MainWindow::onReferenceGuide
 */
void MainWindow::onReferenceGuide() {

    QFileInfo pdf(ExpandEnvironmentVariables("${PQE_HOME}/PlantUML_Language_Reference_Guide.pdf"));

    if(pdf.exists()) {
        QDesktopServices::openUrl(QUrl(pdf.absoluteFilePath()));
    } else {
        QDesktopServices::openUrl(QUrl("http://plantuml.com/PlantUML_Language_Reference_Guide.pdf"));
    }
}

QString MainWindow::makeKeyForDocument(QByteArray current_document) {
    QString key = QString("%1.%2")
                  .arg(QString::fromUtf8(QCryptographicHash::hash(current_document, QCryptographicHash::Md5).toHex()))
                  .arg(m_imageFormatNames[m_currentImageFormat])
                  ;

    return key;
}

bool MainWindow::refreshFromCache() {

    if (m_useCache) {

        if (m_editor->isEmpty()) {
            qDebug() << "empty document. skipping...";
            return true;
        }

        QByteArray current_document = m_editor->toPlainText().toUtf8().trimmed();

        switch (m_currentImageFormat) {
            case SvgFormat:
                m_previewWidget->setMode(PreviewWidget::SvgMode);
                break;

            case PngFormat:
                m_previewWidget->setMode(PreviewWidget::PngMode);
                break;
        }

        QString key = makeKeyForDocument(current_document);
        // try the cache first
        const FileCacheItem* item = qobject_cast<const FileCacheItem*>(m_cache->item(key));

        if (item) {
            QFile file(item->path());

            if (file.open(QFile::ReadOnly)) {
                QByteArray cache_image = file.readAll();

                if (cache_image.size()) {
                    m_cachedImage = cache_image;
                    m_previewWidget->load(m_cachedImage);
//                    statusBar()->showMessage(tr("Cache hit: %1").arg(key), STATUSBAR_TIMEOUT);
                    qDebug() << "cache hit:" << key;
                    m_needsRefresh = false;
                    return true;
                }
            }
        }
    }

    return false;
}

/**
 * Switch preview mode automatically if PlantUML does not support the current format:
 * PNG only: ditaa, JCCKit
 * SVG only: Graphviz dot
 *
 * @brief MainWindow::switchPreviewModeIfNeeded
 * @return Preview mode changed.
 */
bool MainWindow::switchPreviewModeIfNeeded() {

    if(m_editor->toPlainText().contains(IMAGE_PNG_ONLY)) {
        // PNG needed
        if(m_currentImageFormat == SvgFormat) {
            qDebug() << "switch preview to PNG";
            m_svgPreviewAction->setChecked(false);
            m_pngPreviewAction->setChecked(true);
            changeImageFormat();
            statusBar()->showMessage(tr("Switched preview mode to PNG"), STATUSBAR_TIMEOUT);
            return true;
        }
    }
    else if(m_editor->toPlainText().contains(IMAGE_SVG_ONLY)) {
        // SVG needed
        if(m_currentImageFormat == PngFormat) {
            qDebug() << "switch preview to SVG";
            m_pngPreviewAction->setChecked(false);
            m_svgPreviewAction->setChecked(true);
            changeImageFormat();
            statusBar()->showMessage(tr("Switched preview mode to SVG"), STATUSBAR_TIMEOUT);
            return true;
        }
    }
    return false;
}

void MainWindow::refresh(bool forced) {
    if (m_process) {
        qDebug() << "still processing previous refresh. skipping...";
        return;
    }

    if (!m_needsRefresh && !forced) {
        return;
    }

    if (!m_hasValidPaths) {
        qDebug() << "please configure paths for plantuml and java. aborting...";
        statusBar()->showMessage(tr("PlantUML or Java not found. Please set them correctly in the \"Preferences\" dialog!"));
        return;
    }

    if (!forced && refreshFromCache()) {
        return;
    }

    if (m_editor->isEmpty()) {
        qDebug() << "empty document. skipping...";
        return;
    }

    QString src = prepareCodeFinal(m_editor->toPlainText().toUtf8().trimmed());
    QByteArray current_document(src.toUtf8().trimmed());

    m_needsRefresh = false;

    m_refreshing = true;
    switchPreviewModeIfNeeded();
    m_refreshing = false;

    switch (m_currentImageFormat) {
        case SvgFormat:
            m_previewWidget->setMode(PreviewWidgetSvg::SvgMode);
            break;

        case PngFormat:
            m_previewWidget->setMode(PreviewWidgetSvg::PngMode);
            break;
    }

    QString key = makeKeyForDocument(current_document);

    if(statusBar()->currentMessage().isEmpty()) {
        statusBar()->showMessage(tr(TEXT_REFRESHING));
    }

    QStringList arguments;

    arguments << "-splash:no"
              << "-jar" << m_plantUmlPath.absoluteFilePath()
              << QString("-t%1").arg(m_imageFormatNames[m_currentImageFormat])
              << "-word"                // don't use the optional filename
              << "-fastfail2"
              << "-nbthread" << "auto";

    if (m_useCustomGraphviz) {
        arguments << "-graphvizdot" << m_graphvizPath.absoluteFilePath();
    }

    arguments << "-charset" << "UTF-8"
              << "-pipe";

    m_lastKey = key;
    m_process = new QProcess(this);

    if(!m_documentPath.isEmpty()) {
        QFileInfo fi(m_documentPath);
        m_process->setWorkingDirectory(fi.absolutePath());
    } else if(!m_lastDir.isEmpty()) {
        QFileInfo fi(m_lastDir);
        m_process->setWorkingDirectory(fi.absolutePath());
    } else if(!m_assistantXmlPath.isEmpty()) {
        QFileInfo fi(m_assistantXmlPath);
        m_process->setWorkingDirectory(fi.absolutePath());
    }

    m_process->start(m_javaPath.absoluteFilePath(), arguments);

    if (!m_process->waitForStarted()) {
        qDebug() << "refresh subprocess failed to start";
        return;
    }

    connect(m_process, SIGNAL(finished(int)), this, SLOT(refreshFinished()));

    m_process->write(current_document); // pipe
    m_process->closeWriteChannel();
}

void MainWindow::updateCacheSizeInfo() {
    m_cacheSizeLabel->setText(m_useCache ?
                              CACHE_SIZE_FORMAT_STRING.arg(cacheSizeToString(m_cache->totalCost())) :
                              tr("NO CACHE"));
}

void MainWindow::focusAssistant() {
    QListWidget* widget = qobject_cast<QListWidget*>(m_assistantToolBox->currentWidget());

    if (widget) {
        widget->setFocus();

        if (widget->selectedItems().count() == 0) {
            widget->setCurrentItem(widget->itemAt(0, 0));
        }
    }
}

void MainWindow::refreshFinished() {
    m_cachedImage = m_process->readAll();
    m_previewWidget->load(m_cachedImage);
    m_process->deleteLater();
    m_process = 0;

    if (m_useCache && m_cache) {
        m_cache->addItem(m_cachedImage, m_lastKey,
                         [](const QString & path,
                            const QString & key,
                            int cost,
                            const QDateTime & date_time,
                            QObject * parent
        ) {
            return new FileCacheItem(path, key, cost, date_time, parent);
        });
        updateCacheSizeInfo();
    }

    if(statusBar()->currentMessage().isEmpty() || statusBar()->currentMessage() == tr(TEXT_REFRESHING)) {
        statusBar()->showMessage(tr("Refreshed"), STATUSBAR_TIMEOUT);
    }
}

void MainWindow::changeImageFormat() {

    ImageFormat new_format;

    if (m_pngPreviewAction->isChecked()) {
        new_format = PngFormat;
    } else {
        new_format = SvgFormat;
    }

    if (new_format != m_currentImageFormat) {
        m_currentImageFormat = new_format;
        m_needsRefresh = true;
        m_currentImageFormatLabel->setText(m_imageFormatNames[m_currentImageFormat].toUpper());
        if(m_refreshing == true) {
            qDebug() << "skip refreshing"; // no refresh, because we still refreshing (see MainWindow::switchPreviewModeIfNeeded)
        }
        else {
            refresh();
        }
    }
}

void MainWindow::onAutoRefreshActionToggled(bool state) {
    if (state) {
        refresh();
        m_autoRefreshTimer->start();
    } else {
        m_autoRefreshTimer->stop();
    }

    m_autoRefreshLabel->setEnabled(state);
}

void MainWindow::onEditorChanged() {

    // prevent refreshing during typing
    m_autoRefreshTimer->start();

    if (!refreshFromCache()) {
        // cache not used or not in cache
        m_needsRefresh = true;
    }

    setWindowModified(true);

    enableUndoRedoActions();
}

void MainWindow::onRefreshActionTriggered() {
    m_needsRefresh = true;
    refresh(true);
}

void MainWindow::onPreferencesActionTriggered() {
    writeSettings();
    PreferencesDialog dialog(m_cache, this);
    dialog.readSettings();
    dialog.exec();

    if (dialog.result() == QDialog::Accepted) {
        dialog.writeSettings();
        readSettings(true);
    }
}

void MainWindow::onOpenDocumentActionTriggered() {
    openDocument("");
}

void MainWindow::onReloadDocumentActionTriggered() {
    if (!m_documentPath.isEmpty()) {
        openDocument(m_documentPath);
        onRefreshActionTriggered();
        statusBar()->showMessage(tr("Reloaded"));
    }
}

void MainWindow::onSaveActionTriggered() {
    saveDocument(m_documentPath);

    if (m_refreshOnSave) {
        onRefreshActionTriggered();
    }
}

void MainWindow::onSaveAsActionTriggered() {
    saveDocument("");
}

void MainWindow::onExportImageActionTriggered() {
    exportImage(m_exportPath);
}

void MainWindow::onExportAsImageActionTriggered() {
    exportImage("");
}

void MainWindow::onRecentDocumentsActionTriggered(const QString& path) {
    openDocument(path);
}

void MainWindow::onAssistanItemDoubleClicked(QListWidgetItem* item) {

    insertAssistantCode(item->data(ASSISTANT_ITEM_DATA_ROLE).toString());

    m_previewWidget->zoomOriginal();

    refresh(true);

    m_editor->setFocus(); // force focus to move to the editor
}

void MainWindow::onAssistantFocus() {
    focusAssistant();
}

void MainWindow::onAssistantItemInsert(QWidget* widget) {
    QListWidget* list_widget = qobject_cast<QListWidget*>(widget);

    if (list_widget) {
        onAssistanItemDoubleClicked(list_widget->currentItem());
    }
}

void MainWindow::onNextAssistant() {
    if(m_assistantToolBox->count() <= 0) return;

    m_assistantToolBox->setCurrentIndex((m_assistantToolBox->currentIndex() + 1) % m_assistantToolBox->count());
}

void MainWindow::onPrevAssistant() {
    if(m_assistantToolBox->count() <= 0) return;

    const int count = m_assistantToolBox->count();
    m_assistantToolBox->setCurrentIndex((count + m_assistantToolBox->currentIndex() - 1) % count);
}

void MainWindow::onAssistantItemSelectionChanged() {
    QListWidget* widget = qobject_cast<QListWidget*>(m_assistantToolBox->currentWidget());

    if (widget) {
        QListWidgetItem* item = widget->currentItem();

        if (item) {
            QString notes = item->data(ASSISTANT_ITEM_NOTES_ROLE).toString();

            if(notes.isEmpty()) {
               m_assistantPreviewNotes->setText(tr("Code:"));
            }
            else {
               if(!notes.contains(QRegExp("^<(div|ol|ul|p|dl|h[1-6]|table)"))) {
                   notes = "<br>" + notes + "<br>";
               }
               m_assistantPreviewNotes->setText(tr("Notes:") + notes + tr("Code:"));
            }
            m_assistantCodePreview->setPlainText(item->data(ASSISTANT_ITEM_DATA_ROLE).toString());
        }
    }
}

void MainWindow::onCurrentAssistantChanged(int /*index*/) {
    focusAssistant();
    onAssistantItemSelectionChanged(); // make sure we don't show stale info
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

bool MainWindow::maybeSave() {
    if (m_editor->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, qApp->applicationName(),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            return saveDocument(m_documentPath);
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Read settings from conf file.
 * @param reload
 */
void MainWindow::readSettings(bool reload) {

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    const QString DEFAULT_CACHE_PATH = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
    const QString DEFAULT_CACHE_PATH = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif

    QSettings settings;

    settings.beginGroup(SETTINGS_PREF_PROGRAMS);
    {
        // java.exe
        {
            m_useCustomJava = settings.value(SETTINGS_USE_CUSTOM_JAVA, SETTINGS_USE_CUSTOM_JAVA_DEFAULT).toBool();
            m_customJavaPath = settings.value(SETTINGS_CUSTOM_JAVA_PATH, SETTINGS_CUSTOM_JAVA_PATH_DEFAULT).toString();

            m_javaPath = QFileInfo(ExpandEnvironmentVariables(m_useCustomJava ? m_customJavaPath : SETTINGS_CUSTOM_JAVA_PATH_DEFAULT));

            { // Determine java version
                m_javaVersion = tr("Unknown");
                m_process = new QProcess(this);
                m_process->setWorkingDirectory(m_javaPath.absolutePath());
                m_process->setProcessChannelMode(QProcess::MergedChannels);
                QStringList arguments;
                arguments << "-version";
                m_process->start(m_javaPath.absoluteFilePath(), arguments);

                if (!m_process->waitForStarted()) {
                    qDebug() << "refresh subprocess failed to start";
                } else {
                    QByteArray data;

                    while (m_process->waitForReadyRead()) {
                        data.append(m_process->readAll());
                    }

                    QRegExp regex("version (\\S+)");
                    int pos = 0;
                    pos = regex.indexIn(data.data(), pos);

                    if (pos > -1) {
                        m_javaVersion = regex.cap(1);
                        m_javaVersion.replace("\"", "");
                    }
                }

                //delete m_process;
                m_process = 0;
            }
        }

        // plantuml.jar
        {
            m_useCustomPlantUml = settings.value(SETTINGS_USE_CUSTOM_PLANTUML, SETTINGS_USE_CUSTOM_PLANTUML_DEFAULT).toBool();
            m_customPlantUmlPath = settings.value(SETTINGS_CUSTOM_PLANTUML_PATH, SETTINGS_CUSTOM_PLANTUML_PATH_DEFAULT).toString();

            m_plantUmlPath = QFileInfo(ExpandEnvironmentVariables(m_useCustomPlantUml ? m_customPlantUmlPath : SETTINGS_CUSTOM_PLANTUML_PATH_DEFAULT));

            { // Determine plantuml version
                m_plantUmlVersion = tr("Unknown");
                m_process = new QProcess(this);
                m_process->setWorkingDirectory(m_plantUmlPath.absolutePath());
                m_process->setProcessChannelMode(QProcess::MergedChannels);
                QStringList arguments;
                arguments << "-splash:no" << "-jar" << m_plantUmlPath.absoluteFilePath() << "-version";
                m_process->start(m_javaPath.absoluteFilePath(), arguments);

                if (!m_process->waitForStarted()) {
                    qDebug() << "refresh subprocess failed to start";
                } else {
                    QByteArray data;

                    while (m_process->waitForReadyRead()) {
                        data.append(m_process->readAll());
                    }

                    QRegExp regex("version (\\d+)");
                    int pos = 0;
                    pos = regex.indexIn(data.data(), pos);

                    if (pos > -1) {
                        m_plantUmlVersion = regex.cap(1);
                    }
                }

                //delete m_process;
                m_process = 0;
            }
        }

        // dot.exe
        {
            m_useCustomGraphviz = settings.value(SETTINGS_USE_CUSTOM_GRAPHVIZ, SETTINGS_USE_CUSTOM_GRAPHVIZ_DEFAULT).toBool();
            m_customGraphvizPath = settings.value(SETTINGS_CUSTOM_GRAPHVIZ_PATH, SETTINGS_CUSTOM_GRAPHVIZ_PATH_DEFAULT).toString();

            m_graphvizPath = QFileInfo(ExpandEnvironmentVariables(m_useCustomGraphviz ? m_customGraphvizPath : SETTINGS_CUSTOM_GRAPHVIZ_PATH_DEFAULT));

            { // Determine graphviz version
                m_graphvizVersion = tr("Unknown");
                m_process = new QProcess(this);
                m_process->setWorkingDirectory(m_graphvizPath.absolutePath());
                m_process->setProcessChannelMode(QProcess::MergedChannels);
                QStringList arguments;
                arguments << "-V";
                m_process->start(m_graphvizPath.absoluteFilePath(), arguments);

                if (!m_process->waitForStarted()) {
                    qDebug() << "refresh subprocess failed to start";
                } else {
                    QByteArray data;

                    while (m_process->waitForReadyRead()) {
                        data.append(m_process->readAll());
                    }

                    QRegExp regex("version (\\S+)");
                    int pos = 0;
                    pos = regex.indexIn(data.data(), pos);

                    if (pos > -1) {
                        m_graphvizVersion = regex.cap(1);
                    }
                }

                //delete m_process;
                m_process = 0;
            }
        }

        checkPaths();
    }
    settings.endGroup();

    settings.beginGroup(SETTINGS_PREF_ASSISTANT);
    {
        // assistant.xml
        reloadAssistantXml(settings.value(SETTINGS_ASSISTANT_XML_PATH).toString());

        // assistant font
        QFont assistantFont;
        QFont defaultFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        defaultFont.setPointSize(defaultFont.pointSize()-2);
        assistantFont.fromString(settings.value(SETTINGS_ASSISTANT_FONT, defaultFont.toString()).toString());
        m_assistantCodePreview->setFont(assistantFont);
    }
    settings.endGroup();

    settings.beginGroup(SETTINGS_PREF_PREVIEW);
    {
        // cache
        m_useCache = settings.value(SETTINGS_USE_CACHE, SETTINGS_USE_CACHE_DEFAULT).toBool();
        m_useCustomCache = settings.value(SETTINGS_USE_CUSTOM_CACHE, SETTINGS_USE_CUSTOM_CACHE_DEFAULT).toBool();
        m_customCachePath = settings.value(SETTINGS_CUSTOM_CACHE_PATH, DEFAULT_CACHE_PATH).toString();
        m_cacheMaxSize = settings.value(SETTINGS_CACHE_MAX_SIZE, SETTINGS_CACHE_MAX_SIZE_DEFAULT).toInt();
        m_cachePath = QFileInfo(ExpandEnvironmentVariables(m_useCustomCache ? m_customCachePath : DEFAULT_CACHE_PATH));

        m_cache->setMaxCost(m_cacheMaxSize);
        m_cache->setPath(m_cachePath.absolutePath(),
                         [](const QString & path,
                            const QString & key,
                            int cost,
                            const QDateTime & date_time,
                            QObject * parent
                         ) {
            return new FileCacheItem(path, key, cost, date_time, parent);
        });
        // autorefresh
        m_autoRefreshTimer->stop();
        m_autoRefreshTimer->setInterval(settings.value(SETTINGS_AUTOREFRESH_TIMEOUT, SETTINGS_AUTOREFRESH_TIMEOUT_DEFAULT).toInt());
        m_autoRefreshTimer->start();
    }
    settings.endGroup();

    settings.beginGroup(SETTINGS_PREF_EDITOR);
    {
        QFont editorFont;
        QFont defaultFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        editorFont.fromString(settings.value(SETTINGS_EDITOR_FONT, defaultFont.toString()).toString());
        m_editor->setFont(editorFont);

        m_editor->setAutoIndent(settings.value(SETTINGS_EDITOR_INDENT, SETTINGS_EDITOR_INDENT_DEFAULT).toBool());
        m_editor->setIndentWithSpace(settings.value(SETTINGS_EDITOR_INDENT_WITH_SPACE,
                                                    SETTINGS_EDITOR_INDENT_WITH_SPACE_DEFAULT).toBool());
        m_editor->setIndentSize(settings.value(SETTINGS_EDITOR_INDENT_SIZE,
                                               SETTINGS_EDITOR_INDENT_SIZE_DEFAULT).toInt());
        m_useLastDocument = settings.value(SETTINGS_USE_LAST_DOCUMENT, SETTINGS_USE_LAST_DOCUMENT_DEFAULT).toBool();
        m_refreshOnSave = settings.value(SETTINGS_PREVIEW_REFRESH_ON_SAVE, SETTINGS_EDITOR_REFRESH_ON_SAVE_DEFAULT).toBool();

    }
    settings.endGroup();

    settings.beginGroup(SETTINGS_WIN);
    {
        m_currentImageFormat = m_imageFormatNames.key(settings.value(SETTINGS_IMAGE_FORMAT, m_imageFormatNames[SvgFormat]).toString());

        if (m_currentImageFormat == SvgFormat) {
            m_svgPreviewAction->setChecked(true);
        } else if (m_currentImageFormat == PngFormat) {
            m_pngPreviewAction->setChecked(true);
        }

        m_currentImageFormatLabel->setText(m_imageFormatNames[m_currentImageFormat].toUpper());

        const bool autorefresh_enabled = settings.value(SETTINGS_AUTOREFRESH_ENABLED, false).toBool();
        m_autoRefreshAction->setChecked(autorefresh_enabled);

        if (autorefresh_enabled) {
            m_autoRefreshTimer->start();
        } else {
            m_autoRefreshTimer->stop();
        }

        m_autoRefreshLabel->setEnabled(autorefresh_enabled);

        m_autoSaveImageAction->setChecked(settings.value(SETTINGS_AUTOSAVE_IMAGE_ENABLED, SETTINGS_AUTOSAVE_IMAGE_ENABLED_DEFAULT).toBool());

        if (!reload) {
            restoreGeometry(settings.value(SETTINGS_WIN_GEOMETRY).toByteArray());
            restoreState(settings.value(SETTINGS_WINDOW_STATE).toByteArray());
        }

        m_lastDir = settings.value(SETTINGS_EDITOR_LAST_DIR, SETTINGS_EDITOR_LAST_DIR_DEFAULT).toString();

        m_showToolbarsAction->setChecked(m_mainToolBar->isVisibleTo(this)); // NOTE: works even if the current window is not yet displayed
        connect(m_showToolbarsAction, SIGNAL(toggled(bool)), m_mainToolBar, SLOT(setVisible(bool)));
        connect(m_showToolbarsAction, SIGNAL(toggled(bool)), m_zoomToolBar, SLOT(setVisible(bool)));

        const bool show_statusbar = settings.value(SETTINGS_SHOW_STATUSBAR, true).toBool();
        m_showStatusBarAction->setChecked(show_statusbar);
        statusBar()->setVisible(show_statusbar);
        connect(m_showStatusBarAction, SIGNAL(toggled(bool)), statusBar(), SLOT(setVisible(bool)));
    }
    settings.endGroup();

    m_recentDocuments->readFromSettings(settings, SETTINGS_WIN_RECENT_DOCUMENTS);
    updateCacheSizeInfo();
}

/**
 * @brief Write settings to conf file.
 */
void MainWindow::writeSettings() {

    QSettings settings;

    settings.beginGroup(SETTINGS_PREF_PROGRAMS);
    {
        // java.exe
        settings.setValue(SETTINGS_USE_CUSTOM_JAVA, m_useCustomJava);
        settings.setValue(SETTINGS_CUSTOM_JAVA_PATH, m_customJavaPath);
        // plantuml.jar
        settings.setValue(SETTINGS_USE_CUSTOM_PLANTUML, m_useCustomPlantUml);
        settings.setValue(SETTINGS_CUSTOM_PLANTUML_PATH, m_customPlantUmlPath);
        // dot.exe
        settings.setValue(SETTINGS_USE_CUSTOM_GRAPHVIZ, m_useCustomGraphviz);
        settings.setValue(SETTINGS_CUSTOM_GRAPHVIZ_PATH, m_customGraphvizPath);
    }
    settings.endGroup();

    settings.beginGroup(SETTINGS_PREF_ASSISTANT);
    {
        // assistant.xml
        settings.setValue(SETTINGS_ASSISTANT_XML_PATH, m_assistantXmlPath);
        // assistant font
        settings.setValue(SETTINGS_ASSISTANT_FONT, m_assistantCodePreview->font().toString());
    }
    settings.endGroup();

    settings.beginGroup(SETTINGS_PREF_PREVIEW);
    {
        // cache
        settings.setValue(SETTINGS_USE_CACHE, m_useCache);
        settings.setValue(SETTINGS_USE_CUSTOM_CACHE, m_useCustomCache);
        settings.setValue(SETTINGS_CUSTOM_CACHE_PATH, m_customCachePath);
        settings.setValue(SETTINGS_CACHE_MAX_SIZE, m_cacheMaxSize);        
        // refresh
        settings.setValue(SETTINGS_AUTOREFRESH_TIMEOUT, m_autoRefreshTimer->interval());
        settings.setValue(SETTINGS_PREVIEW_REFRESH_ON_SAVE, m_refreshOnSave);
    }
    settings.endGroup();

    settings.beginGroup(SETTINGS_PREF_EDITOR);
    {
        // font (Oxygen Mono,11,-1,5,50,0,0,0,0,0)
        settings.setValue(SETTINGS_EDITOR_FONT, m_editor->font().toString());
        // autoindent
        settings.setValue(SETTINGS_EDITOR_INDENT, m_editor->autoIndent());
        // use spaces instead tabs
        settings.setValue(SETTINGS_EDITOR_INDENT_WITH_SPACE, m_editor->indentWithSpace());
        settings.setValue(SETTINGS_EDITOR_INDENT_SIZE, m_editor->indentSize());

        // load last file
        settings.setValue(SETTINGS_USE_LAST_DOCUMENT, m_useLastDocument);
    }
    settings.endGroup();

    settings.beginGroup(SETTINGS_WIN);
    {
        // from menu
        settings.setValue(SETTINGS_IMAGE_FORMAT, m_imageFormatNames[m_currentImageFormat]);
        settings.setValue(SETTINGS_AUTOREFRESH_ENABLED, m_autoRefreshAction->isChecked());
        settings.setValue(SETTINGS_AUTOSAVE_IMAGE_ENABLED, m_autoSaveImageAction->isChecked());
        // window state
        settings.setValue(SETTINGS_EDITOR_LAST_DIR, m_lastDir);
        settings.setValue(SETTINGS_WIN_GEOMETRY, saveGeometry());
        settings.setValue(SETTINGS_WINDOW_STATE, saveState());
        settings.setValue(SETTINGS_SHOW_STATUSBAR, m_showStatusBarAction->isChecked());
    }
    settings.endGroup();

    m_recentDocuments->writeToSettings(settings, SETTINGS_WIN_RECENT_DOCUMENTS);
}

void MainWindow::openDocument(const QString& name) {
    if (!maybeSave()) {
        return;
    }

    QString tmp_name = name;

    if (tmp_name.isEmpty() || QFileInfo(tmp_name).exists() == false) {
        tmp_name = QFileDialog::getOpenFileName(this,
                   tr("Select a file to open"),
                   m_lastDir,
                   "PlantUML (*.plantuml);; All Files (*.*)"
                                               );

        if (tmp_name.isEmpty()) {
            return;
        } else {
            QFileInfo fi(tmp_name);
            m_lastDir = fi.absolutePath();
        }
    }

    QFile file(tmp_name);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    m_editor->setPlainText(QString::fromUtf8(file.readAll())); // resets undo buffer => ok
    setWindowModified(false);
    m_documentPath = tmp_name;
    setWindowTitle(TITLE_FORMAT_STRING
                   .arg(QFileInfo(tmp_name).fileName())
                   .arg(qApp->applicationName())
                  );
    m_needsRefresh = true;
    refresh();
    m_recentDocuments->accessing(tmp_name);
}

bool MainWindow::saveDocument(const QString& name) {
    QString file_path = name;

    if (file_path.isEmpty()) {
        file_path = QFileDialog::getSaveFileName(this,
                    tr("Select where to store the document"),
                    m_lastDir,
                    "PlantUML (*.plantuml);; All Files (*.*)"
                                                );

        if (file_path.isEmpty()) {
            return false;
        } else {
            QFileInfo fi(file_path);
            m_lastDir = fi.absolutePath();
        }
    }

    qDebug() << "saving document in:" << file_path;
    QFile file(file_path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    file.write(m_editor->toPlainText().toUtf8().trimmed());
    file.close();
    m_documentPath = file_path;
    setWindowTitle(TITLE_FORMAT_STRING
                   .arg(QFileInfo(file_path).fileName())
                   .arg(qApp->applicationName())
                  );
    statusBar()->showMessage(tr("Document save in %1").arg(file_path), STATUSBAR_TIMEOUT);
    m_recentDocuments->accessing(file_path);

    if (m_autoSaveImageAction->isChecked()) {
        QFileInfo info(file_path);
        QString image_path = QString("%1/%2.%3")
                             .arg(info.absolutePath())
                             .arg(info.baseName())
                             .arg(m_imageFormatNames[m_currentImageFormat])
                             ;
        qDebug() << "saving image in:   " << image_path;
        QFile image(image_path);

        if (!image.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return false;
        }

        image.write(m_cachedImage);
        image.close();
    }

    m_editor->document()->setModified(false);
    setWindowModified(false);
    return true;
}

void MainWindow::exportImage(const QString& filename) {

    if (m_cachedImage.isEmpty()) {
        qDebug() << "no image to export. aborting...";
        return;
    }

    QString tmpFilename = filename;

    if (tmpFilename.isEmpty()) {

        QStringList filter;
        filter << "PNG Image (*.png)"
               << "SVG Image (*.svg)"
               << "EPS Image (*.eps)"
               << "VDX Image (*.vdx)"
               << "PDF Image !!! (*.pdf)"
               << "XMI Image !!! (*.xmi)"
               << "HTML File !!! (*.html)"
               << "ATXT File (*.atxt)"
               << "UTXT File (*.utxt)"
               << "All Files (*.*)";

        QString dir;
        if(m_exportPath != NULL) {
            dir = m_exportPath;
        }
        else {
            dir = QString(m_documentPath);
        }

        dir = dir.replace(QRegExp("\\.\\w{1,10}$"), ".png");

        QString selectedFilter;
        tmpFilename = QFileDialog::getSaveFileName(this,
                                                   tr("Select where to export the image"),
                                                   dir,
                                                   filter.join(";;"),
                                                   &selectedFilter
                                                   );
        if (tmpFilename.isEmpty()) {
            return;
        }

        // Example: "PNG Image (*.png)"
        QRegExp reExt("\\*(\\.\\w{3,4})");
        if(reExt.indexIn(selectedFilter) > -1){
            QString fileExtension = reExt.cap(1);

            qDebug() << "add extemsion:" << fileExtension;

            if(!tmpFilename.contains(fileExtension, Qt::CaseInsensitive)) {
                tmpFilename = tmpFilename + fileExtension;
            }
        }
    }

    qDebug() << "exporting image in:" << tmpFilename;

    QFile file(tmpFilename);

    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QRegExp ext("\\.(\\w{3,4})$");
    if (ext.indexIn(tmpFilename) > -1) {
        QString fileExtension = ext.cap(1);

        fileExtension = fileExtension.toLower();

        if(fileExtension == "png") {
            if(m_currentImageFormat == PngFormat) {
                file.write(m_cachedImage);
            }
            else {
                generateImage(tmpFilename, fileExtension, NULL, true);
            }
        } else if(fileExtension == "svg") {
            if(m_currentImageFormat == SvgFormat) {
                file.write(m_cachedImage);
            }
            else {
                generateImage(tmpFilename, fileExtension, NULL, true);
            }
        } else if(fileExtension == "eps"  ||
                  fileExtension == "pdf"  ||
                  fileExtension == "vdx"  ||
                  fileExtension == "xmi"  ||
                  fileExtension == "html" ||
                  fileExtension == "atxt" ||
                  fileExtension == "utxt" ) {
            generateImage(tmpFilename, fileExtension, NULL, true);
        }
        else {
            // unknown file extension
            file.write(m_cachedImage);
        }
    }
    else {
        // no file extension
        file.write(m_cachedImage);
    }

//    file.write(m_cachedImage);
    m_exportImageAction->setText(EXPORT_TO_MENU_FORMAT_STRING.arg(tmpFilename));
    m_exportPath = tmpFilename;
    QString short_tmp_name = QFileInfo(tmpFilename).fileName();
    statusBar()->showMessage(tr("Image exported to %1").arg(/*short_*/tmpFilename), STATUSBAR_TIMEOUT);
    m_exportPathLabel->setText(EXPORT_TO_LABEL_FORMAT_STRING.arg(short_tmp_name));
    m_exportPathLabel->setEnabled(true);
}

/**
 * Generates an image from the given code or from the current editor code.
 *
 * @brief MainWindow::generateImage
 * @param filename Filename of the image.
 * @param format   Format of the image.
 * @param src      PlantUML code or NULL = current editor code.
 * @return Success.
 */
bool MainWindow::generateImage(const QString& filename, const QString& format, const QString& code, const bool keepPlantUmlFile){

    if (!m_hasValidPaths) {
        qDebug() << "please configure paths for plantuml and java. aborting...";
        statusBar()->showMessage(tr("PlantUML or Java not found. Please set them correctly in the \"Preferences\" dialog!"));
        return false;
    }

    QByteArray src;
    if(code == NULL) {
        // use editor code
        if(!m_editor->isEmpty()) {
            src = m_editor->toPlainText().toUtf8().trimmed();
        }
    }
    else {
        src = code.toUtf8().trimmed();
        // see TextEdit::toPlainText
        if(src.contains("ditaa") && src.contains("\\\n")) {
            qDebug() << "fix ditaa problem";
            src.replace("\\\n", "\\ \n");
        }
    }

    if (src.isEmpty()) {
        qDebug() << "empty document. skipping...";
        return false;
    }

//    QString key = makeKeyForDocument(src);

    qDebug() << "generate" << format << "file" << filename << "...";

    QString plantUmlFile = filename;
    plantUmlFile.replace(format, "plantuml");

    QFile file(plantUmlFile);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << src;
    file.close();

    QString formatOpt = "-t" + ( format == "atxt" ? "txt" : format ); // -ttxt generates *.atxt files

    QStringList arguments;
    arguments << "-splash:no"
              << "-jar" << m_plantUmlPath.absoluteFilePath()
              << formatOpt
              << "-word"           // don't use the optional filename
              << "-fastfail2"
              << "-nbthread" << "auto";

    if (m_useCustomGraphviz) {
        arguments << "-graphvizdot" << m_graphvizPath.absoluteFilePath();
    }

    arguments << "-charset" << "UTF-8"
              << plantUmlFile;

    QProcess* process = new QProcess(this);
    QFileInfo fi(plantUmlFile);
    process->setWorkingDirectory(fi.absolutePath());

    qDebug() << "call" << m_javaPath.absoluteFilePath() << arguments;
    process->start(m_javaPath.absoluteFilePath(), arguments);

    if (!process->waitForStarted()) {
        qDebug() << "render subprocess failed to start";
        return false;
    }

    process->waitForFinished();
    process->close();

    if(!keepPlantUmlFile) {
        QFile::remove(plantUmlFile);
    }

    qDebug() << "file" << filename << "generated";

    return true;
}

void MainWindow::createActions() {
    { // File menu
        m_newDocumentAction = new QAction(getIcon("document-new"), tr("&New"), this);
        m_newDocumentAction->setShortcut(QKeySequence::New);
        connect(m_newDocumentAction, SIGNAL(triggered()), this, SLOT(newDocument()));

        m_openDocumentAction = new QAction(getIcon("document-open"), tr("&Open"), this);
        m_openDocumentAction->setShortcuts(QKeySequence::Open);
        connect(m_openDocumentAction, SIGNAL(triggered()), this, SLOT(onOpenDocumentActionTriggered()));

        m_reloadDocumentAction = new QAction(getIcon("view-refresh"), tr("&Reload"), this);
        m_reloadDocumentAction->setShortcut(Qt::Key_F6);
        connect(m_reloadDocumentAction, SIGNAL(triggered()), this, SLOT(onReloadDocumentActionTriggered()));

        m_saveDocumentAction = new QAction(getIcon("document-save"), tr("&Save"), this);
        m_saveDocumentAction->setShortcuts(QKeySequence::Save);
        connect(m_saveDocumentAction, SIGNAL(triggered()), this, SLOT(onSaveActionTriggered()));

        m_saveAsDocumentAction = new QAction(getIcon("document-save-as"), tr("Save As..."), this);
        m_saveAsDocumentAction->setShortcuts(QKeySequence::SaveAs);
        connect(m_saveAsDocumentAction, SIGNAL(triggered()), this, SLOT(onSaveAsActionTriggered()));

        m_exportImageAction = new QAction(getIcon("document-export"),EXPORT_TO_MENU_FORMAT_STRING.arg(""), this);
        m_exportImageAction->setShortcut(Qt::CTRL + Qt::Key_E);
        connect(m_exportImageAction, SIGNAL(triggered()), this, SLOT(onExportImageActionTriggered()));

        m_exportAsImageAction = new QAction(getIcon("document-export"), tr("Export as ..."), this);
        m_exportAsImageAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_E);
        connect(m_exportAsImageAction, SIGNAL(triggered()), this, SLOT(onExportAsImageActionTriggered()));

        m_quitAction = new QAction(getIcon("application-exit"), tr("&Quit"), this);
        m_quitAction->setShortcuts(QKeySequence::Quit);
        m_quitAction->setStatusTip(tr("Quit the application"));
        connect(m_quitAction, SIGNAL(triggered()), this, SLOT(close()));
    }

    { // Edit menu
        m_undoAction = new QAction(getIcon("edit-undo"), tr("&Undo"), this);
        m_undoAction->setShortcuts(QKeySequence::Undo);
        connect(m_undoAction, SIGNAL(triggered()), this, SLOT(undo()));

        m_redoAction = new QAction(getIcon("edit-redo"), tr("&Redo"), this);
        m_redoAction->setShortcuts(QKeySequence::Redo);
        connect(m_redoAction, SIGNAL(triggered()), this, SLOT(redo()));

        m_copyImageAction = new QAction(getIcon("edit-copy"), tr("&Copy Image"), this);
        m_copyImageAction->setShortcuts(QList<QKeySequence>()
                                        << QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C)
                                        //                                    << QKeySequence::Copy
                                        );
        connect(m_copyImageAction, SIGNAL(triggered()), this, SLOT(copyImage()));
    }

    { // Tools menu
        m_pngPreviewAction = new QAction(tr("PNG"), this);
        m_pngPreviewAction->setCheckable(true);
        m_pngPreviewAction->setStatusTip(tr("Tell PlantUML to produce PNG output"));
        m_pngPreviewAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_P));
        connect(m_pngPreviewAction, SIGNAL(toggled(bool)), this, SLOT(changeImageFormat()));

        m_svgPreviewAction = new QAction(tr("SVG"), this);
        m_svgPreviewAction->setCheckable(true);
        m_svgPreviewAction->setStatusTip(tr("Tell PlantUML to produce SVG output"));
        m_svgPreviewAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
        connect(m_svgPreviewAction, SIGNAL(toggled(bool)), this, SLOT(changeImageFormat()));

        QActionGroup* output_action_group = new QActionGroup(this);
        output_action_group->setExclusive(true);
        output_action_group->addAction(m_pngPreviewAction);
        output_action_group->addAction(m_svgPreviewAction);
        m_svgPreviewAction->setChecked(true);

        m_refreshAction = new QAction(getIcon("view-refresh"), tr("Refresh"), this);
        m_refreshAction->setShortcuts(QKeySequence::Refresh);
        m_refreshAction->setStatusTip(tr("Call PlantUML to regenerate the UML image"));
        connect(m_refreshAction, SIGNAL(triggered()), this, SLOT(onRefreshActionTriggered()));

        m_autoRefreshAction = new QAction(tr("Auto-Refresh"), this);
        m_autoRefreshAction->setCheckable(true);

        m_autoSaveImageAction = new QAction(tr("Auto-Save image"), this);
        m_autoSaveImageAction->setCheckable(true);
        connect(m_autoRefreshAction, SIGNAL(toggled(bool)), this, SLOT(onAutoRefreshActionToggled(bool)));
    }

    { // Settings menu
        m_showToolbarsAction = new QAction(tr("Show toolbars"), this);
        m_showToolbarsAction->setCheckable(true);
        m_showToolbarsAction->setShortcut(QKeySequence(Qt::SHIFT + Qt::ALT + Qt::Key_T));

        m_showStatusBarAction = new QAction(tr("Show statusbar"), this);
        m_showStatusBarAction->setCheckable(true);
        m_showStatusBarAction->setShortcut(QKeySequence(Qt::SHIFT + Qt::ALT + Qt::Key_S));

        m_preferencesAction = new QAction(getIcon("preferences-other"), tr("Preferences"), this);
        connect(m_preferencesAction, SIGNAL(triggered()), this, SLOT(onPreferencesActionTriggered()));
    }

    { // Help menu
        m_aboutAction = new QAction(getIcon("help-about"), tr("&About"), this);
        m_aboutAction->setStatusTip(tr("Show the application's About box"));
        connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(about()));

        m_referenceGuideAction = new QAction(getIcon("help-hint"), tr("&Reference Guide"), this);
        m_referenceGuideAction->setStatusTip(tr("Show PlantUML Reference Guide"));
        connect(m_referenceGuideAction, SIGNAL(triggered()), this, SLOT(onReferenceGuide()));

        m_aboutQtAction = new QAction(getIcon("help-about-qt"), tr("About &Qt"), this);
        m_aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
        connect(m_aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    }

    //    // focus actions
    //    QAction* focus_action = new QAction(this);
    //    focus_action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_0));
    //    connect(focus_action, SIGNAL(triggered()), m_editor, SLOT(setFocus()));
    //    this->addAction(focus_action);

    //    focus_action = new QAction(this);
    //    focus_action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1));
    //    connect(focus_action, SIGNAL(triggered()), this, SLOT(onAssistantFocus()));
    //    this->addAction(focus_action);

    { // assistant actions
        QAction* navigation_action = new QAction(this);
        navigation_action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));
        connect(navigation_action, SIGNAL(triggered()), this, SLOT(onNextAssistant()));
        addAction(navigation_action);

        navigation_action = new QAction(this);
        navigation_action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));
        connect(navigation_action, SIGNAL(triggered()), this, SLOT(onPrevAssistant()));
        addAction(navigation_action);
    }

    { // zoom actions
        m_zoomInAction = new QAction(getIcon("zoom-in"), tr("Zoom In"), this);
        m_zoomInAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus));
        connect(m_zoomInAction, SIGNAL(triggered()), m_previewWidget, SLOT(zoomIn()));

        m_zoomOutAction = new QAction(getIcon("zoom-out"), tr("Zoom Out"), this);
        m_zoomOutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus));
        connect(m_zoomOutAction, SIGNAL(triggered()), m_previewWidget, SLOT(zoomOut()));

        m_zoomOriginalAction = new QAction(getIcon("zoom-original"), tr("1:1"), this);
        m_zoomOriginalAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_0));
        connect(m_zoomOriginalAction, SIGNAL(triggered()), m_previewWidget, SLOT(zoomOriginal()));

        m_zoomFitBestAction = new QAction(getIcon("zoom-fit-best"), tr("Fit Best"), this);
        m_zoomFitBestAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));
        connect(m_zoomFitBestAction, SIGNAL(triggered()), m_previewWidget, SLOT(zoomFitBest()));

        m_zoomFitWidthAction = new QAction(getIcon("zoom-fit-width"), tr("Fit Width"), this);
        m_zoomFitWidthAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
        connect(m_zoomFitWidthAction, SIGNAL(triggered()), m_previewWidget, SLOT(zoomFitWidth()));

        m_zoomFitHeightAction = new QAction(getIcon("zoom-fit-height"), tr("Fit Height"), this);
        m_zoomFitHeightAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
        connect(m_zoomFitHeightAction, SIGNAL(triggered()), m_previewWidget, SLOT(zoomFitHeight()));
    }
}

void MainWindow::createMenus() {

    { // File
        m_fileMenu = menuBar()->addMenu(tr("&File"));
        m_fileMenu->addAction(m_newDocumentAction);
        m_fileMenu->addAction(m_openDocumentAction);
        m_fileMenu->addAction(m_reloadDocumentAction);
        m_fileMenu->addAction(m_saveDocumentAction);
        m_fileMenu->addAction(m_saveAsDocumentAction);
        m_fileMenu->addSeparator();
        QMenu* recent_documents_submenu = m_fileMenu->addMenu(tr("Recent Documents"));
        recent_documents_submenu->addActions(m_recentDocuments->actions());
        m_fileMenu->addSeparator();
        m_fileMenu->addAction(m_exportImageAction);
        m_fileMenu->addAction(m_exportAsImageAction);
        m_fileMenu->addSeparator();
        m_fileMenu->addAction(m_quitAction);
    }

    { // Edit
        m_editMenu = menuBar()->addMenu(tr("&Edit"));
        m_editMenu->addAction(m_undoAction);
        m_editMenu->addAction(m_redoAction);
        m_editMenu->addAction(m_copyImageAction);
        m_editMenu->addSeparator();
        m_editMenu->addAction(m_refreshAction);
    }

    { // Settings
        m_settingsMenu = menuBar()->addMenu(tr("&Settings"));
        m_settingsMenu->addAction(m_showToolbarsAction);
        m_settingsMenu->addAction(m_showStatusBarAction);
        m_settingsMenu->addSeparator();
        m_settingsMenu->addAction(m_showAssistantDockAction);
        m_settingsMenu->addAction(m_showAssistantInfoDockAction);
        m_settingsMenu->addAction(m_showEditorDockAction);
        m_settingsMenu->addSeparator();
        m_settingsMenu->addAction(m_pngPreviewAction);
        m_settingsMenu->addAction(m_svgPreviewAction);
        m_settingsMenu->addSeparator();
        m_settingsMenu->addAction(m_autoRefreshAction);
        m_settingsMenu->addAction(m_autoSaveImageAction);
        m_settingsMenu->addSeparator();
        m_settingsMenu->addAction(m_preferencesAction);
    }

    { // Zoom
        m_zoomMenu = menuBar()->addMenu(tr("&Zoom"));
        addZoomActions(m_zoomMenu);
    }

    menuBar()->addSeparator();

    { // Help
        m_helpMenu = menuBar()->addMenu(tr("&Help"));
        m_helpMenu->addAction(m_aboutAction);
        m_helpMenu->addAction(m_referenceGuideAction);
        m_helpMenu->addAction(m_aboutQtAction);
    }
}

void MainWindow::createToolBars() {

    { // Main toolbar
        m_mainToolBar = addToolBar(tr("MainToolbar"));
        m_mainToolBar->setObjectName("main_toolbar");
        m_mainToolBar->addAction(m_newDocumentAction);
        m_mainToolBar->addAction(m_openDocumentAction);
        m_mainToolBar->addAction(m_saveDocumentAction);
        m_mainToolBar->addAction(m_saveAsDocumentAction);
        m_mainToolBar->addSeparator();
        m_mainToolBar->addAction(m_showAssistantDockAction);
        m_mainToolBar->addAction(m_showAssistantInfoDockAction);
        m_mainToolBar->addAction(m_showEditorDockAction);
        m_mainToolBar->addSeparator();
        m_mainToolBar->addAction(m_undoAction);
        m_mainToolBar->addAction(m_redoAction);
        m_mainToolBar->addSeparator();
        m_mainToolBar->addAction(m_refreshAction);
        m_mainToolBar->addSeparator();
        m_mainToolBar->addAction(m_preferencesAction);
    }
    { // Zoom toolbar
        m_zoomToolBar = addToolBar(tr("ZoomToolbar"));
        m_zoomToolBar->setObjectName("zoom_toolbar");
        addZoomActions(m_zoomToolBar);
    }
}

void MainWindow::createStatusBar()
{
    m_exportPathLabel = new QLabel(this);
    m_exportPathLabel->setMinimumWidth(200);
    m_exportPathLabel->setText(EXPORT_TO_LABEL_FORMAT_STRING.arg(""));
    m_exportPathLabel->setEnabled(false);

    m_currentImageFormatLabel = new QLabel(this);

    QFontMetrics font_metrics(m_exportPathLabel->font());
    m_cacheSizeLabel = new QLabel(this);
    m_cacheSizeLabel->setMinimumWidth(font_metrics.width(QString(CACHE_SIZE_FORMAT_STRING.arg("#.## Mb"))));

    m_autoRefreshLabel = new QLabel(this);
    m_autoRefreshLabel->setText(AUTOREFRESH_STATUS_LABEL);

#ifdef Q_OS_X11
    const int label_fram_style = QFrame::Panel | QFrame::Sunken;
    m_exportPathLabel->setFrameStyle(label_fram_style);
    m_currentImageFormatLabel->setFrameStyle(label_fram_style);
    m_cacheSizeLabel->setFrameStyle(label_fram_style);
    m_autoRefreshLabel->setFrameStyle(label_fram_style);
#endif

    { // status bar
        statusBar()->addPermanentWidget(m_exportPathLabel);
        statusBar()->addPermanentWidget(m_cacheSizeLabel);
        statusBar()->addPermanentWidget(m_autoRefreshLabel);
        statusBar()->addPermanentWidget(m_currentImageFormatLabel);
    }

    statusBar()->showMessage(tr("Ready"), STATUSBAR_TIMEOUT);
}

QIcon MainWindow::getIcon(const QString& name, const QString& type) {

    QIcon icon;

#ifdef Q_OS_LINUX
    icon = QIcon::fromTheme(name);
#endif
    if(icon.isNull()) {
        icon = QIcon(":/resources/" + name + "." + type);
    }
    return icon;
}

void MainWindow::createDockWindows() {

    QDockWidget* dock;

    { // Editor dock

        dock = new QDockWidget(tr("Text Editor"), this);
        m_editor = new TextEdit(dock);

        new HighLighter(m_editor->document());

        connect(m_editor->document(), SIGNAL(contentsChanged()), this, SLOT(onEditorChanged()));
        dock->setWidget(m_editor);
        dock->setObjectName("text_editor");
        addDockWidget(Qt::RightDockWidgetArea, dock);

        m_showEditorDockAction = dock->toggleViewAction();
        m_showEditorDockAction->setIconVisibleInMenu(false);
        m_showEditorDockAction->setStatusTip(tr("Show or hide the document editor dock"));
        m_showEditorDockAction->setIcon(getIcon("accessories-text-editor"));
        m_showEditorDockAction->setShortcut(QKeySequence(Qt::SHIFT + Qt::ALT + Qt::Key_E));
    }

    { // Assistant dock
        m_assistantDock = dock = new QDockWidget(tr("Assistant"), this);
        m_assistantToolBox = new QToolBox(dock);
        m_assistantToolBox->layout()->setSpacing(1); //display more content
        dock->setWidget(m_assistantToolBox);
        dock->setObjectName("assistant");
        addDockWidget(Qt::LeftDockWidgetArea, dock);
        connect(m_assistantToolBox, SIGNAL(currentChanged(int)), this, SLOT(onCurrentAssistantChanged(int)));

        m_showAssistantDockAction = dock->toggleViewAction();
        m_showAssistantDockAction->setIconVisibleInMenu(false);
        m_showAssistantDockAction->setStatusTip(tr("Show or hide the assistant dock"));
#ifndef Q_OS_WIN // BUG: icons are not displayed when cross-linking
        m_showAssistantDockAction->setIcon(QIcon(":/resources/assistant.svg"));
#else
        m_showAssistantDockAction->setIcon(QIcon(":/resources/assistant.svg"));
#endif
        m_showAssistantDockAction->setShortcut(QKeySequence(Qt::SHIFT + Qt::ALT + Qt::Key_A));
    }

    { // Assistant Info dock
        m_assistantInfoDock = dock = new QDockWidget(tr("Assistant Info"), this);
        QWidget* widget = new QWidget(dock);
        m_assistantPreviewNotes = new QLabel(widget);
        m_assistantPreviewNotes->setText(tr("Code:"));
        m_assistantCodePreview = new QTextEdit(widget);
        m_assistantCodePreview->setReadOnly(true);
        QBoxLayout* assistant_info_layout = new QBoxLayout(QBoxLayout::TopToBottom, widget);
        assistant_info_layout->addWidget(m_assistantPreviewNotes);
        assistant_info_layout->addWidget(m_assistantCodePreview);
        widget->setLayout(assistant_info_layout);
        dock->setWidget(widget);
        dock->setObjectName("assistant_info");
        addDockWidget(Qt::LeftDockWidgetArea, dock);

        m_showAssistantInfoDockAction = dock->toggleViewAction();
        m_showAssistantInfoDockAction->setIconVisibleInMenu(false);
        m_showAssistantInfoDockAction->setStatusTip(tr("Show or hide the assistant info dock"));
#ifndef Q_OS_WIN // BUG: icons are not displayed when cross-linking
        m_showAssistantInfoDockAction->setIcon(QIcon(":/resources/assistant-info.svg"));
#else
        m_showAssistantInfoDockAction->setIcon(QIcon(":/resources/assistant-info.svg"));
#endif
        m_showAssistantInfoDockAction->setShortcut(QKeySequence(Qt::SHIFT + Qt::ALT + Qt::Key_I));
    }
}

void MainWindow::enableUndoRedoActions() {
    QTextDocument* document = m_editor->document();
    m_undoAction->setEnabled(document->isUndoAvailable());
    m_redoAction->setEnabled(document->isRedoAvailable());
}

void MainWindow::addZoomActions(QWidget* widget) {
    widget->addAction(m_zoomInAction);
    widget->addAction(m_zoomOutAction);
    widget->addAction(m_zoomOriginalAction);
    widget->addAction(m_zoomFitBestAction);
    widget->addAction(m_zoomFitWidthAction);
    widget->addAction(m_zoomFitHeightAction);
}

void MainWindow::checkPaths() {
    m_hasValidPaths =  QFileInfo(m_javaPath).exists()
                    && QFileInfo(m_plantUmlPath).exists();
}

void MainWindow::reloadAssistantXml(const QString& path) {

    if (m_assistantXmlPath == path) return;

    m_assistantXmlPath = path;

    foreach (QListWidget * widget, m_assistantWidgets) {
        widget->deleteLater();
    }
    m_assistantWidgets.clear();

    if (m_assistantXmlPath.isEmpty()) {
        qDebug() << "no assistant defined.";
    } else {
        AssistantXmlReader reader;
        if(!reader.readFile(m_assistantXmlPath)) {
            qDebug() << "no assistant file found";
            return;
        }

        QAction* action_insert = new QAction(this);
        action_insert->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter));

        for (int i = 0; i < reader.size(); ++i) {
            const Assistant* assistant = reader.assistant(i);

            QListWidget* view = newAssistantListWidget(ASSISTANT_ICON_SIZE, this);

            for (int j = 0; j < assistant->size(); ++j) {
                const AssistantItem* assistantItem = assistant->item(j);

//                qDebug() << "ICON" << assistantItem->icon();

                QListWidgetItem* listWidgetItem =
                        new QListWidgetItem(iconFromFile(ASSISTANT_ICON_SIZE,
                                                         assistantItem->icon(),
                                                         assistantItem->data()),
                                            assistantItem->name(),
                                            view);
                listWidgetItem->setData(ASSISTANT_ITEM_DATA_ROLE, assistantItem->data());
                listWidgetItem->setData(ASSISTANT_ITEM_NOTES_ROLE, assistantItem->notes());

                // preview mode
                if(assistant->name().contains("SVG")) {
                    // SVG only
                    listWidgetItem->setData(ASSISTANT_ITEM_FORMAT_ROLE, SvgFormat);
                }
                else if(assistant->name().contains("PNG")) {
                    // PNG only
                    listWidgetItem->setData(ASSISTANT_ITEM_FORMAT_ROLE, PngFormat);
                }
                listWidgetItem->setToolTip(tr("Double click to add to editor"));
            }

            m_assistantToolBox->addItem(view, assistant->name());
            connect(view, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                    this, SLOT(onAssistanItemDoubleClicked(QListWidgetItem*)));
            connect(view, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onAssistantItemSelectionChanged()));

            QAction* action = new QAction(this);
            action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return));
            m_assistantInsertSignalMapper->setMapping(action, view);
            connect(action, SIGNAL(triggered()),
                    m_assistantInsertSignalMapper, SLOT(map()));
            view->addAction(action);

            m_assistantWidgets << view;
        }
    }
}

void MainWindow::insertAssistantCode(const QString& code) {

    if (code.isEmpty()) return;

    QTextCursor cursor = m_editor->textCursor();
    if(cursor.isNull()) return;

    // change code by "hand", because the undo/redo buffer

    QString src = m_editor->document()->toPlainText(); // w/o ditaa fix!!!
    int start = cursor.selectionStart();
    int end   = cursor.selectionEnd();
    int mode  = 0;

    if(start != end) { // selection
        mode = 1;
        if(start == 0 && end == src.length()) {  // replace all
            src = prepareCode(code);
        } else {  // replace part
            src = src.remove(start, end-start);
            src = src.insert(start, prepareCode(code, false));
        }
    }
    else if(start == 0) { // cursor at start
        mode = 2;
        src =  prepareCode(code);
    }
    else if(end == src.length()) {  // cursor at end
        mode = 3;
        src = prepareCode(code);
    }
    else { // insert
        mode = 4;
        src = src.insert(start, prepareCode(code, false));
    }

    cursor.select(QTextCursor::Document);
    cursor.beginEditBlock();
    cursor.insertText(src);
    switch(mode) {
        case 1:
            cursor.setPosition(start);
            statusBar()->showMessage(tr("Selection replaced with assistant item"), STATUSBAR_TIMEOUT);
            break;
        case 2:
            cursor.setPosition(start);
            statusBar()->showMessage(tr("Cursor at start code replaced with assistant item"), STATUSBAR_TIMEOUT);
            break;
        case 3:
            cursor.setPosition(code.length());
            statusBar()->showMessage(tr("Cursor at end - code replaced with assistant item"), STATUSBAR_TIMEOUT);
            break;
        case 4:
            cursor.setPosition(start);
            statusBar()->showMessage(tr("Inserted assistant item"), STATUSBAR_TIMEOUT);
            break;
    }
    cursor.endEditBlock();
}

QString MainWindow::prepareCode(const QString& codeBefore, const bool addTags)
{
    QString code    = codeBefore;
    QString start;
    QString end;
    QRegExp* regex  = NULL;
    QString options;
    bool emptyLines = true;

    if(codeBefore.contains("salt")) {
        start = "@startsalt";
        end   = "@endsalt";
        regex = new QRegExp("^salt([^\\n\\r]+)?[\\r\\n]?");
    }
    else if(codeBefore.contains("ditaa")) {
        start = "@startditaa";
        end   = "@endditaa";
        regex = new QRegExp("^ditaa([^\\n\\r]+)?[\\r\\n]?");
    }
    else if(codeBefore.contains("jcckit")) {
        start = "@startjcckit";
        end   = "@endjcckit";
        regex = new QRegExp("^jcckit([^\\n\\r]+)?[\\r\\n]?");
    }
    else if(codeBefore.contains(QRegExp("(di)?graph\\s+\\w+"))) {
        start = "@startdot";
        end   = "@enddot";
        // keep digraph/graph
        emptyLines = false;
    }
    else {
        start = "@startuml";
        end   = "@enduml";
        // no first line
        if(codeBefore.trimmed().contains(QRegExp("^\\w+$"))) {
            // one word = testdot, checkversion & Co
            emptyLines = false;
        }
    }

    if(regex != NULL) {
        if(regex->indexIn(codeBefore) > -1) {
            options = regex->cap(1).trimmed();
           if(!options.isEmpty()) {
               options = " " + options;
           }
        }
        code.remove(*regex); // remove salt/ditaa/jcckit/... + options
    }

    //don't remove leading spaces!!! code = code.trimmed();

    if(!code.contains("@start") == true && addTags == true) {
        if(emptyLines == false) {
            code = start + "\n" + code + "\n" + end;
        } else {
            code = start + options + "\n\n" + code + "\n\n" + end;
        }
    }

    return code;
}

QString MainWindow::prepareCodeFinal(const QString& codeBefore) {

    QString code = codeBefore; //prepareCode(codeBefore, addTags);

    QDateTime dateTime = QDateTime::currentDateTime();
    code.replace(QRegExp("(\\$\\{DATE\\}|%DATE%)"),   dateTime.toString("yyyy-mm-dd"));
    code.replace(QRegExp("(\\$\\{TIME\\}|%TIME%)"),   dateTime.toString("hh:mm:ss"));
    code.replace(QRegExp("(\\$\\{NOW\\}|%NOW%)"),     dateTime.toString(Qt::ISODate));
    code.replace(QRegExp("(\\$\\{NOW:L\\}|%NOW:L%)"), dateTime.toString(Qt::SystemLocaleLongDate));
    code.replace(QRegExp("(\\$\\{NOW:S\\}|%NOW:S%)"), dateTime.toString(Qt::SystemLocaleShortDate));

    QRegExp dateTimeFormatUnix("([$][{]NOW:([^}]+)[}])");
    int pos = 0;
    while ((pos = dateTimeFormatUnix.indexIn(code, pos)) != -1) {
        code.replace(dateTimeFormatUnix.cap(1), dateTime.toString(dateTimeFormatUnix.cap(2)));
        pos += dateTimeFormatUnix.matchedLength();
    }
    QRegExp dateTimeFormatWin("([%]NOW:([^%]+)[%])");
    pos = 0;
    while ((pos = dateTimeFormatWin.indexIn(code, pos)) != -1) {
        code.replace(dateTimeFormatWin.cap(1), dateTime.toString(dateTimeFormatWin.cap(2)));
        pos += dateTimeFormatWin.matchedLength();
    }

    if(m_documentPath.isEmpty()){
        code.replace(QRegExp("(\\$\\{PATH\\}|%PATH%)"), "???");
        code.replace(QRegExp("(\\$\\{FILE\\}|%FILE%)"), "???");
        code.replace(QRegExp("(\\$\\{DIR\\}|%DIR%)"),   "???");
    } else {
        QFileInfo fileInfo(m_documentPath);
        code.replace(QRegExp("(\\$\\{PATH\\}|%PATH%)"), fileInfo.absoluteFilePath());
        code.replace(QRegExp("(\\$\\{FILE\\}|%FILE%)"), fileInfo.fileName());
        code.replace(QRegExp("(\\$\\{DIR\\}|%DIR%)"),   fileInfo.absolutePath());
    }

    code = ExpandEnvironmentVariables(code, false);

    return code;

}

bool MainWindow::useLastDocument() {
    if (m_useLastDocument == true) {
        if (m_recentDocuments->hasLastDocument() == true) {
            return true;
        } else {
            qDebug() << "last document is not available";
            return false;
        }
    } else {
        return false;
    }
}

void MainWindow::openLastDocument() {
    m_recentDocuments->loadLastDocument();
}
