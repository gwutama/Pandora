#include "appconfig.h"
#include <QDebug>
#include <QFileInfo>

const char* AppConfig::sTag = "[AppConfig]";

AppConfig::AppConfig()
{
    setTheme("default");

    // Prepare tmp output file
    mOutputFile = QSharedPointer<QTemporaryFile>(new QTemporaryFile);
    mOutputFile->open();

    // Finally load the values from permanent config storage
    load();
}


AppConfig::~AppConfig()
{
}


void AppConfig::load()
{
    // Actual options
    setMarkdownFile(mSettings.value("App/markdownFile").toString());
    setBibtexFile(mSettings.value("App/bibtexFile").toString());
}


void AppConfig::save()
{
    // Actual options
    mSettings.setValue("App/markdownFile", mMarkdownFile);
    mSettings.setValue("App/bibtexFile", mBibtexFile);

    mSettings.sync();
}


void AppConfig::setTheme(const QString &theme)
{
    mTheme = theme;
    QTemporaryFile* tmpFile = extractThemeFromResources(theme);

    if (tmpFile != nullptr)
    {
        mCssFile = QSharedPointer<QTemporaryFile>(tmpFile);
    }
    else
    {
        mCssFile = QSharedPointer<QTemporaryFile>();
    }
}


QTemporaryFile* AppConfig::extractThemeFromResources(const QString& cssTheme)
{
    QFile resourceFile(":/themes/" + cssTheme + ".css");

    if (!resourceFile.exists())
    {
        qWarning() << sTag << "Theme does not exist:" << cssTheme;
        return nullptr;
    }

    QTemporaryFile* tmpFile = QTemporaryFile::createNativeFile(resourceFile);
    return tmpFile;
}