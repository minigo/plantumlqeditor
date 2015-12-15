#include "utils.h"
#include <QProcessEnvironment>
#include <QDir>
#include <QRegExp>
#include <QDebug>

QString cacheSizeToString(int size)
{
    return QString("%1 Mb").arg(size / CACHE_SCALE, 0, 'f', 2);
}

#define ENV_VAR  variables.cap(1)
#define UNIX_VAR variables.cap(2)
#define WIN_VAR  variables.cap(3)

/**
 * @brief ExpandEnvironmentVariables
 * @param withVariables
 * @return withoutVariables
 */
QString ExpandEnvironmentVariables(QString withVariables)
{
    if(withVariables.isEmpty()) return withVariables;

//    withVariables.replace(QRegExp("^[.]/"), "");
//    withVariables.replace(QRegExp("^[.]/"), dir.absolutePath());
    withVariables.replace(s_reHome, "${HOME}/"); // ~ => QDir::homePath() => Qt isn't very smart :(
    withVariables.replace(s_reQEditor, dir.absolutePath());

    QString withoutVariables = withVariables;

    qDebug() << "ENV  IN" << withoutVariables;

    // *nix and Windows style
    {
        int pos = 0;
        while ((pos = envVars.indexIn(withVariables, pos)) != -1) {

            QString envVar  = envVars.cap(1);
            QString varName = envVars.cap(1).startsWith('$') ? envVars.cap(2) : envVars.cap(3);

            // same behavior on *nix and Windows
#ifdef Q_OS_WIN
            /**/ if(varName == "USER") varName = "USERNAME";
            else if(varName == "HOME") varName = "HOMEPATH";
#else
            // Windoof is not case sensitive => try upper case if not exist
            if(!env.contains(varName)) varName = varName.toUpper();

            /**/ if(varName == "USERNAME") varName = "USER";
            else if(varName == "HOMEPATH") varName = "HOME";
            else if((varName == "TEMP" || varName == "TMP" ) && !env.contains(varName)) {
                env.insert(varName, "/tmp");
            }
            else if(varName == "OS" && !env.contains(varName)) {
                QProcess process;
                process.start("uname -s");
                process.waitForFinished(1000); // max 1 second!
                QString stdout = process.readAllStandardOutput();
                if(stdout.isEmpty())
                    stdout = "Unix";
                else
                    stdout.replace("/", "_");

                env.insert(varName, stdout);
            }
#endif

            withoutVariables  = withoutVariables.replace(envVar, env.value(varName));
            pos += envVars.matchedLength();

            qDebug() << "ENV" << envVar << "->" << varName << "=>" << env.value(varName);
        }
    }
    qDebug() << "ENV OUT" << withoutVariables;
    return withoutVariables;
}
