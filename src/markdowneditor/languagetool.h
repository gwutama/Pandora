#ifndef LANGUAGETOOL_H
#define LANGUAGETOOL_H

#include <QNetworkAccessManager>
#include <QUrl>
#include <QSharedPointer>
#include "languagetoolmatch.h"

class LanguageTool : public QObject
{
    Q_OBJECT
public:
    explicit LanguageTool(const QUrl& url,
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

signals:
    void suggestionsReady();

private:
    QUrl mUrl;
    QString mLanguage;
    QSharedPointer<QNetworkAccessManager> mNetwork;
    QList<LanguageToolMatch> mSuggestions;

    static const char* sTag;
};

#endif // LANGUAGETOOL_H
