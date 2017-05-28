#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QString>
#include <QTemporaryFile>
#include <QSharedPointer>
#include <QSettings>
#include <QSize>
#include <QPoint>

class AppConfig
{
    friend class PreferencesDialog;
    friend class MainWindow;
    friend class MarkdownEditor;

public:
    explicit AppConfig();
    virtual ~AppConfig();

    inline QString markdownFile()
    { return mMarkdownFile; }

    inline QString theme()
    { return mTheme; }

    inline QSharedPointer<QTemporaryFile> cssFile()
    { return mCssFile; }

    inline QString bibtexFile()
    { return mBibtexFile; }

    inline QSharedPointer<QTemporaryFile> outputFile()
    { return mOutputFile; }

    inline int editorFontPointSize()
    { return mEditorFontPointSize; }

    QSettings& settings()
    { return mSettings; }

private:
    void load();
    void save();

    inline void setMarkdownFile(const QString& markdownFile)
    { mMarkdownFile = markdownFile; }

    void setTheme(const QString &theme);

    inline void setBibtexFile(const QString& bibtexFile)
    { mBibtexFile = bibtexFile; }

    inline void setEditorFontPointSize(int size)
    { mEditorFontPointSize = size; }

    QTemporaryFile* extractThemeFromResources(const QString& cssTheme);

private:
    QSettings mSettings;

    // Actual options
    QString mMarkdownFile;
    QString mTheme;
    QString mBibtexFile; // optional
    int mEditorFontPointSize;

    // Auto generated based on other properties
    QSharedPointer<QTemporaryFile> mCssFile;
    QSharedPointer<QTemporaryFile> mOutputFile;

    static const char* sTag;
};

#endif // APPCONFIG_H
