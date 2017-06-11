#ifndef LANGUAGETOOL_H
#define LANGUAGETOOL_H

#include <QNetworkAccessManager>
#include <QUrl>
#include <QSharedPointer>
#include <QProcess>
#include "languagetoolmatch.h"

class LanguageTool : public QObject
{
    Q_OBJECT
public:
    explicit LanguageTool(const QUrl& url,
                          const QString& language = "en-US",
                          bool useBuiltin = true,
                          QObject* parent = nullptr);
    virtual ~LanguageTool();

    inline void setLanguage(const QString& language)
    { mLanguage = language; }

    inline QList<LanguageToolMatch> suggestions()
    { return mSuggestions; }

public slots:
    void check(const QString& text);

private slots:
    void replyFinished(QNetworkReply* reply);

private:
    void run();

signals:
    void suggestionsReady();
    void toolReady();

private:
    QUrl mUrl;
    QString mLanguage;
    bool mUseBuiltin;
    QSharedPointer<QNetworkAccessManager> mNetwork;
    QList<LanguageToolMatch> mSuggestions;
    QProcess mExecProc;

    static const char* sTag;
};

#endif // LANGUAGETOOL_H
