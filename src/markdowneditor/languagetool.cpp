#include "languagetool.h"
#include <QDebug>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonArray>

const char* LanguageTool::sTag = "[LanguageTool]";

LanguageTool::LanguageTool(const QUrl& url,
                           QObject* parent) :
    QObject(parent),
    mUrl(url),
    mLanguage("en-US")
{
    mNetwork = QSharedPointer<QNetworkAccessManager>(new QNetworkAccessManager);
    connect(mNetwork.data(), &QNetworkAccessManager::finished,
            this, &LanguageTool::replyFinished);
}


LanguageTool::~LanguageTool()
{
}


void LanguageTool::check(const QString& text)
{
    QNetworkRequest request(mUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QByteArray data;
    data.append("language=" + mLanguage);
    data.append("&text=" + QUrl::toPercentEncoding(text));
    mNetwork->post(request, data);
}


void LanguageTool::replyFinished(QNetworkReply* reply)
{
    // Process json
    QByteArray ba = reply->readAll();
    qDebug() << sTag << "Got reply from languagetool server";

    QJsonDocument doc = QJsonDocument::fromJson(ba);
    QJsonObject obj = doc.object();

    if (obj.contains("matches"))
    {
        mSuggestions.clear();

        QJsonArray matches = obj.value("matches").toArray();
        qDebug() << sTag << "Processing matches:" << matches.size();

        for (int i = 0; i < matches.size(); i++)
        {
            QJsonObject match = matches.at(i).toObject();
            LanguageToolMatch ltMatch;
            ltMatch.fromJsonObject(match);
            mSuggestions.append(ltMatch);
        }

        emit suggestionsReady();
    }
}
