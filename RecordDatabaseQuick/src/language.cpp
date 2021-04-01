#include "language.h"

#include <QGuiApplication>
#include <QTranslator>
#include <QDir>
#include <QQmlEngine>

Language::Language(QQmlEngine* engine, QObject *parent)
    : QObject(parent)
    , m_engine(engine)
{
    QString defaultLocale = QLocale::system().name();
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));

    setLanguage(defaultLocale);
}

QStringList Language::model()
{
    QDir dir(":/translations/qm");
    QStringList languages = dir.entryList(QStringList("*.qm"));

    for (auto& language : languages) {
        language.truncate(language.lastIndexOf('.')); // "en.qm" -> "en"
    }
    return languages;
}

void Language::setLanguage(const QString& language)
{
    QGuiApplication::removeTranslator(&m_translator);

    QLocale locale = QLocale(language);
    QLocale::setDefault(locale);
    m_translator.load(QString(":/translations/qm/%1.qm").arg(language));
    QGuiApplication::installTranslator(&m_translator);
    m_engine->retranslate();
}
