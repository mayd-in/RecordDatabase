#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>
#include <QTranslator>

class QQmlEngine;

class Language : public QObject
{
    Q_OBJECT
public:
    explicit Language(QQmlEngine* qmlContext, QObject *parent = nullptr);

public slots:
    QStringList model();
    void setLanguage(const QString& language);

private:
    QQmlEngine* m_engine;
    QTranslator m_translator;
};

#endif // LANGUAGE_H
