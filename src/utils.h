#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QRegExp>
#include <QProcessEnvironment>

const double CACHE_SCALE = 1024*1024;

static QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
//                            ${variable}             %variable%
static QRegExp s_reMatches("([$][{]([a-zA-Z0-9_]+)[}]|%([a-zA-Z0-9_()]+)%)");
//                           11111111111111111111111111111111111111111111
//                                  2222222222222       333333333333333
static QRegExp s_reCurentDir("^([.])([/\\\\])");
static QRegExp s_reHome("^(~)([/\\\\])");

QString ExpandEnvironmentVariables(const QString& pWithVariables, bool pIsPath = true);

QString cacheSizeToString(int size);

#endif // UTILS_H
