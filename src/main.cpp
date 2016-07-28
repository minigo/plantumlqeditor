#include "src/mainwindow.h"

#include <QFileInfo>
#include <QDebug>
#include <QCoreApplication>
#include <QApplication>
#include <QStyleFactory>
#include <QTextStream>

namespace {
  const char* APPLICATION_NAME    = "PlantUML QEditor";
  const char* APPLICATION_VERSION = "2.0";  
  const char* ORGANIZATION_NAME   = "jalbersol";
  const char* ORGANIZATION_DOMAIN = "github.com";
  // => conf file: ~/.config/jalbersol/PlantUML QEditor.conf

  const char* OPTION_HELP_SHORT        = "-h";
  const char* OPTION_HELP_LONG         = "--help";
  const char* OPTION_OLD_PREVIEW_SHORT = "-o";
  const char* OPTION_OLD_PREVIEW_LONG  = "--old-preview";

  void displayHelp(const char* app_name) {
    QTextStream(stdout) << qPrintable(QString("Usage: %1 [options] [FILE]").arg(QFileInfo(app_name).fileName()));
    QTextStream(stdout) << qPrintable(QString("\n"
                                              "    %1  %2        Display this help\n"
                                              "    %3  %4 Use old preview\n"
                                              "\n")
                                              .arg(OPTION_HELP_SHORT)
                                              .arg(OPTION_HELP_LONG)
                                              .arg(OPTION_OLD_PREVIEW_SHORT)
                                              .arg(OPTION_OLD_PREVIEW_LONG) );
  }
} // Namespace

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(APPLICATION_VERSION);

    QApplication application(argc, argv);

    //application.setStyle(QStyleFactory::create("Fusion"));

    QStringList options;
    for (int i = 1; i < argc; ++i) {
        options << argv[i];
    }

    if (options.contains(OPTION_HELP_SHORT) || options.contains(OPTION_HELP_LONG)) {
        displayHelp(argv[0]);
        return 0;
    }

    bool oldPreview = false;
    if (options.contains(OPTION_OLD_PREVIEW_SHORT) || options.contains(OPTION_OLD_PREVIEW_LONG)) {
        oldPreview = true;
        options.pop_front();
    }

    QString open_document_path;
    if (options.size() > 0) {
        open_document_path = options.last();
    }

    MainWindow window(oldPreview);
    if (open_document_path.isEmpty()) {
      if(window.useLastDocument()) {
        window.openLastDocument();
      } else {
        window.newDocument(true);
      }
    } else {        
        window.openDocument(open_document_path);
    }
    window.show();

    return application.exec();
}
