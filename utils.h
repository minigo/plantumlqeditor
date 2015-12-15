#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QDir>
#include <QRegExp>
#include <QProcessEnvironment>

const double CACHE_SCALE = 1024*1024;

static QDir dir;
static QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
//                          ${variable}           %variable%
static QRegExp envVars("([$][{]([a-zA-Z0-9_]+)[}]|%([a-zA-Z0-9_()]+)%)");
//                       11111111111111111111111111111111111111111111
//                              2222222222222       333333333333333
static QRegExp s_reHome("^~/");
static QRegExp s_reQEditor("(%QEDITOR%|[$][{]QEDITOR[}])");
QString ExpandEnvironmentVariables(QString withVariables);

QString cacheSizeToString(int size);

#endif // UTILS_H
