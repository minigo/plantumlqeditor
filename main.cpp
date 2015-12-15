#include <QFileInfo>
#include <QDebug>
#include <QCoreApplication>
#include <QApplication>
#include "mainwindow.h"

namespace {
  const char* APPLICATION_NAME    = "PlantUML QEditor";
  const char* APPLICATION_VERSION = "2.0";
  const char* ORGANIZATION_NAME   = "Ionutz Borcoman";
  const char* ORGANIZATION_DOMAIN = "borco.net";
  // => conf file: ~/.config/Ionutz Borcoman/PlantUML QEditor.conf

  const char* OPTION_HELP_SHORT = "-h";
  const char* OPTION_HELP_LONG = "--help";

  void displayHelp(const char* app_name) {
    qDebug() << qPrintable(QString("Usage: %1 [options] [FILE]").arg(QFileInfo(app_name).fileName()));
    qDebug() << qPrintable(QString("\n"
                                   "%1  %2    Display this help\n"
                                   "\n"
                                   "If FILE is provided, load it on start. If running in single\n"
                                   "instance mode, the instance is signaled to load FILE.\n")
                           .arg(OPTION_HELP_SHORT)
                           .arg(OPTION_HELP_LONG)
                          );
  }
} // Namespace

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(APPLICATION_VERSION);

    QApplication application(argc, argv);

    QStringList options;
    for (int i = 1; i < argc; ++i) {
        options << argv[i];
    }

    if (options.contains(OPTION_HELP_SHORT) || options.contains(OPTION_HELP_LONG)) {
        displayHelp(argv[0]);
        return 0;
    }

    QString open_document_path;
    if (options.size() > 0) {
        open_document_path = options.last();
    }

    MainWindow window;
    if (open_document_path.isEmpty()) {
      if(window.useLastDocument()) {
        window.openLastDocument();
      }
      else {
        window.newDocument();
      }
    } else {        
        window.openDocument(open_document_path);
    }
    window.show();

    return application.exec();
}
