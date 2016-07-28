#include "src/utils.h"

#include <QDebug>
#include <QDir>
#include <QCoreApplication>

QString cacheSizeToString(int size)
{
    return QString("%1 Mb").arg(size / CACHE_SCALE, 0, 'f', 2);
}

/**
 * @brief ExpandEnvironmentVariables
 * @param withVariables
 * @return withoutVariables
 */
QString ExpandEnvironmentVariables(const QString& pWithVariables, bool pIsPath)
{
    if(pWithVariables.isEmpty()) return pWithVariables;

    qDebug() << "\nENV <" << pWithVariables;

    QString withVariables = pWithVariables;

    if(pIsPath) {
        // . => ${PQE_HOME}
        withVariables.replace(s_reCurentDir, "${PQE_HOME}/");

        // ~ => ${HOME} (not ~user = /home/user !)
        withVariables.replace(s_reHome, "${HOME}/");

        qDebug() << "ENV -" << withVariables;
    }

    QString withoutVariables = withVariables;

    // *nix and Windows style
    {
        int pos = 0;
        while ((pos = s_reMatches.indexIn(withVariables, pos)) != -1) {

            QString match   = s_reMatches.cap(1);
            QString varName = s_reMatches.cap(2).isEmpty() ? s_reMatches.cap(3) : s_reMatches.cap(2);

            // same behavior on *nix and Windows
#ifdef Q_OS_WIN
            /**/ if(varName == "USER") varName = "USERNAME";
            else if(varName == "HOME") varName = "HOMEPATH";
#else
            // Windoof is not case sensitive => try upper case if not exist
            if(!env.contains(varName)) varName = varName.toUpper();

            /**/ if(varName == "USERNAME")  varName = "USER";
            else if(varName == "CUSER")     varName = "USER"; // CUSER = current user (FUSER = file user)
            else if(varName == "CUSERNAME") varName = "USER"; // CUSERNAME like CUSER
            else if(varName == "HOMEPATH")  varName = "HOME";
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
            else if(varName == "PQE_HOME") {
                env.insert(varName, QCoreApplication::applicationDirPath());
            }

            withoutVariables  = withoutVariables.replace(match, env.value(varName));
            pos += s_reMatches.matchedLength();

            qDebug() << "ENV =" << match << "->" << varName << "=>" << env.value(varName);
        }
    }

    if(pIsPath) {
        withoutVariables.replace("\\", "/"); // Qt handles the separators but not \n (newline)
    }

    qDebug() << "ENV >" << withoutVariables << "\n";
    return withoutVariables;
}
