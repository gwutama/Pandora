#include "appconfig.h"
#include <QDebug>

const char* AppConfig::sTag = "[AppConfig]";

AppConfig::AppConfig()
{
    // Prepare tmp output file
    mOutputFile = QSharedPointer<QTemporaryFile>(new QTemporaryFile);
    mOutputFile->open();
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
    setEditorFontPointSize(mSettings.value("Editor/fontPointSize", 18).toInt());
    setTheme(mSettings.value("Editor/theme", "default").toString());
    setDictionaryLang(mSettings.value("Editor/dictionaryLang", "en_US").toString());
}


void AppConfig::save()
{
    // Actual options
    mSettings.setValue("App/markdownFile", mMarkdownFile);
    mSettings.setValue("App/bibtexFile", mBibtexFile);
    mSettings.setValue("Editor/fontPointSize", mEditorFontPointSize);
    mSettings.setValue("Editor/theme", mTheme);
    mSettings.setValue("Editor/dictionaryLang", mDictionaryLang);
    mSettings.sync();
}


void AppConfig::setTheme(const QString& theme)
{
    mTheme = theme;
    QTemporaryFile* tmpFile = extractResourceFile(":/themes/" + theme + ".css");

    if (tmpFile != nullptr)
    {
        mCssFile = QSharedPointer<QTemporaryFile>(tmpFile);
    }
    else
    {
        mCssFile = QSharedPointer<QTemporaryFile>();
    }
}


QTemporaryFile* AppConfig::extractResourceFile(const QString& file)
{
    QFile resourceFile(file);

    if (!resourceFile.exists())
    {
        qWarning() << sTag << "Resource does not exist:" << file;
        return nullptr;
    }

    QTemporaryFile* tmpFile = QTemporaryFile::createNativeFile(resourceFile);
    return tmpFile;
}


void AppConfig::setDictionaryLang(const QString& dictionaryLang)
{
    mDictionaryLang = dictionaryLang;

    QTemporaryFile* affFile = extractResourceFile(":/dictionaries/" + dictionaryLang + ".aff");
    QSharedPointer<QTemporaryFile> affShPtr;

    if (affFile != nullptr)
    {
        affShPtr = QSharedPointer<QTemporaryFile>(affFile);
    }
    else
    {
        affShPtr = QSharedPointer<QTemporaryFile>();
    }

    QTemporaryFile* dicFile = extractResourceFile(":/dictionaries/" + dictionaryLang + ".dic");
    QSharedPointer<QTemporaryFile> dicShPtr;

    if (dicFile != nullptr)
    {
        dicShPtr = QSharedPointer<QTemporaryFile>(dicFile);
    }
    else
    {
        dicShPtr = QSharedPointer<QTemporaryFile>();
    }

    mDictionaryFiles.insert("aff", affShPtr);
    mDictionaryFiles.insert("dic", dicShPtr);
}
