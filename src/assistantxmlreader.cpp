#include "src/assistantxmlreader.h"

#include "src/settingsconstants.h"
#include "src/utils.h"

#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QCoreApplication>

namespace {
  const QString ROOT_TAG = "assistants";
  const QString ASSISTANT_TAG = "assistant";
  const QString ASSISTANT_ICON_SUFIX = "-icons";
}

AssistantXmlReader::AssistantXmlReader(QObject *parent)
    : QObject(parent)
{
}

void AssistantXmlReader::clear() {
    // clear all
    foreach(Assistant* assistant, m_items) {
        delete assistant;
    }
    m_items.clear();
}

bool AssistantXmlReader::readFile(const QString &path)
{
    clear();

    QFileInfo realPath(ExpandEnvironmentVariables(path));
    QDir assistantDir = realPath.absoluteDir();
    QString iconDir = realPath.baseName() + ASSISTANT_ICON_SUFIX;

    if(!realPath.exists()) {
        qDebug() << "assistant not found";
        return false;
    }

    // set path to assistant icons

    if (assistantDir.cd(iconDir)) {
        // use subdir
        m_iconDir = assistantDir.absolutePath();
    } else {
        qDebug() << "directory with assistant icons not found => create directory";
        assistantDir.mkdir(iconDir);
        if (assistantDir.cd(iconDir)) {
            m_iconDir = assistantDir.absolutePath();
        } else {
            return false;
        }
    }
    qDebug() << "using assistant file      " << realPath.absoluteFilePath();
    qDebug() << "using assistant icon path:" << m_iconDir;

    QFile file(realPath.absoluteFilePath());
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "can't read assistant file:" << realPath.absoluteFilePath();
        qDebug() << "i/o error:" << file.errorString();
        return false;
    }
    m_reader.setDevice(&file);
    m_reader.readNext();
    while (!m_reader.atEnd()) {
        if (m_reader.isStartElement()) {
            if (m_reader.name() == ROOT_TAG) {
                readRootElement();
            } else {
                m_reader.raiseError(QObject::tr("Not an assistants XML file"));
            }
        } else {
            m_reader.readNext();
        }
    }

    file.close();
    if (m_reader.hasError()) {
        qDebug() << "failed to parse assistant file:" << realPath.absoluteFilePath();
        qDebug() << "xml parsing error:" << m_reader.errorString();
        return false;
    } else if (file.error() != QFile::NoError) {
        qDebug() << "can't read assistant file:" << realPath.absoluteFilePath();
        qDebug() << "i/o error:" << file.errorString();
        return false;
    }

    return true;
}

QString AssistantXmlReader::removeWhiteSpace(const QString &data)
{
    if (data.isEmpty()) return data;

    QStringList lines = data.split('\n');

    int left_space = 0;
    int index_begin = 0;
    int index_end = lines.size() - 1;

    while (index_begin < lines.size()) {
        int space = trimLeft(lines[index_begin]);
        if (!lines[index_begin].isEmpty()) {
            trimRight(lines[index_begin]);
            left_space = space;
            break;
        }
        ++index_begin;
    }

    for (int index = index_begin + 1; index < lines.size(); ++index) {
        lines[index].remove(0, left_space);
        trimRight(lines[index]);
    }

    while (lines[index_end].isEmpty() && index_end > index_begin) {
        --index_end;
    }

    if (index_begin != 0 || index_end != lines.size()) {
        lines = lines.mid(index_begin, index_end - index_begin + 1);
    }
    return lines.join(QChar('\n'));
}

int AssistantXmlReader::trimLeft(QString &data)
{
    for (int index = 0; index < data.size(); ++index) {
        if (!data[index].isSpace()) {
            data.remove(0, index);
            return index;
        }
    }
    int ret = data.size();
    data.clear();
    return ret;
}

void AssistantXmlReader::trimRight(QString &data)
{
    for (int index = data.size() - 1; index >= 0; --index) {
        if (!data[index].isSpace()) {
            data.chop(data.size() - 1 - index);
            return;
        }
    }
    data.clear();
}

void AssistantXmlReader::readRootElement()
{
    m_reader.readNext();
    while (!m_reader.atEnd()) {
        if (m_reader.isEndElement()) {
            m_reader.readNext();
            break;
        }

        if (m_reader.isStartElement()) {
            if (m_reader.name() == ASSISTANT_TAG) {
                readAssistantElement();
            } else {
                skipUnknownElement();
            }
        } else {
            m_reader.readNext();
        }
    }
}

void AssistantXmlReader::skipUnknownElement()
{
    m_reader.readNext();
    while (!m_reader.atEnd()) {
        if (m_reader.isEndElement()) {
            m_reader.readNext();
            break;
        }
        if (m_reader.isStartElement()) {
            skipUnknownElement();
        } else {
            m_reader.readNext();
        }
    }
}

void AssistantXmlReader::readAssistantElement()
{
    QString name = m_reader.attributes().value("name").toString();
    Assistant* assistant = new Assistant(name, this);
    m_items.append(assistant);

    m_reader.readNext();
    while (!m_reader.atEnd()) {
        if (m_reader.isEndElement()) {
            m_reader.readNext();
            break;
        }

        if (m_reader.isStartElement()) {
            if (m_reader.name() == "item") {
                readAssistantItemElement(assistant);
            } else {
                skipUnknownElement();
            }
        } else {
            m_reader.readNext();
        }
    }
}

void AssistantXmlReader::readAssistantItemElement(Assistant *assistant)
{
    QString name = m_reader.attributes().value("name").toString();
    QStringList data;
    QStringList notes;

    m_reader.readNext();
    while (!m_reader.atEnd()) {
        if (m_reader.isEndElement()) {
            m_reader.readNext();
            break;
        }

        if (m_reader.isStartElement()) {
            if (m_reader.name() == "notes") {
                QString tmp = removeWhiteSpace(readAssistantItemNotes());
                if (!tmp.isEmpty()) {
                    notes << tmp;
                }
            } else {
                skipUnknownElement();
            }
        } else {
            QString tmp = m_reader.text().toString();
            tmp = removeWhiteSpace(tmp);
            if (!tmp.isEmpty()) {
                data << tmp;
            }
            m_reader.readNext();
        }
    }

    QString icon = assistant->name() + "__" + name;
    icon.replace(QRegExp("[^a-zA-Z0-9-]"), QString("_"));
    icon = m_iconDir + QDir::separator() + icon + ".svg";

    AssistantItem* item = new AssistantItem(name,
                                            data.join(QChar('\n')),
                                            notes.join(QChar('\n')),
                                            icon,
                                            assistant);
    assistant->append(item);
}

QString AssistantXmlReader::readAssistantItemNotes()
{
    QString notes;
    m_reader.readNext();
    while (!m_reader.atEnd()) {
        if (m_reader.isEndElement()) {
            m_reader.readNext();
            break;
        }

        if (m_reader.isStartElement()) {
            skipUnknownElement();
        } else {
            notes += m_reader.text().toString();
            m_reader.readNext();
        }
    }
    return notes;
}

AssistantItem::AssistantItem(const QString &name, const QString &data, const QString& notes, const QString& icon, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_data(data)
    , m_notes(notes)
    , m_icon(icon)
{
}

Assistant::Assistant(const QString &name, QObject *parent)
    : QObject(parent)
    , m_name(name)
{
}

Assistant::~Assistant()
{
    foreach(AssistantItem* item, m_items) {
        delete item;
    }
    m_items.clear();
}

void Assistant::append(AssistantItem *item)
{
    m_items.append(item);
}
