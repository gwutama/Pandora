#include "languagetool.h"
#include <QDebug>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonArray>

const char* LanguageTool::sTag = "[LanguageTool]";

LanguageTool::LanguageTool(const QUrl& url,
                           const QString& language,
                           bool useBuiltin,
                           QObject* parent) :
    QObject(parent),
    mUrl(url),
    mLanguage(language),
    mUseBuiltin(useBuiltin)
{
    mNetwork = QSharedPointer<QNetworkAccessManager>(new QNetworkAccessManager);
    connect(mNetwork.data(), &QNetworkAccessManager::finished,
            this, &LanguageTool::replyFinished);

    if (mUseBuiltin)
    {
        run();
        connect(&mExecProc, &QProcess::started, this, &LanguageTool::toolReady);
    }
    else
    {
        emit toolReady();
    }
}


LanguageTool::~LanguageTool()
{
    if (mUseBuiltin)
    {
        mExecProc.terminate();
        mExecProc.waitForFinished();
        mExecProc.kill();
    }
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
    if (reply->error())
    {
        qWarning() << sTag << "Network error:" << reply->errorString();
        return;
    }

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


void LanguageTool::run()
{
    if (mExecProc.state() != QProcess::NotRunning)
    {
        return;
    }

    qDebug() << sTag << "Running LanguageTool";
    // run java -cp languagetool-server.jar org.languagetool.server.HTTPServer --port 8080
    mExecProc.setProgram("/usr/bin/java");
    QStringList args;
    args << "-cp"
         << "Pandora.app/Contents/MacOS/LanguageTool/languagetool-server.jar"
         << "org.languagetool.server.HTTPServer"
         << "--port"
         << QString::number(mUrl.port());
    mExecProc.setArguments(args);
    mExecProc.start();
}
