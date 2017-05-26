#include "documentgenerator.h"
#include <QDebug>
#include <QStringList>
#include <QFileInfo>
#include "processrunner.h"

const char* DocumentGenerator::sTag = "[DocumentGenerator]";

DocumentGenerator::DocumentGenerator(const QString& markdownFile,
                                     QObject* parent) :
    QObject(parent),
    mMarkdownFile(markdownFile)
{
    // Setup file watcher
    connect(&mFsWatcher, &QFileSystemWatcher::fileChanged,
            this, &DocumentGenerator::generate);

    watchFile(markdownFile);
}


DocumentGenerator::~DocumentGenerator()
{
}


bool DocumentGenerator::generate(const QString& path)
{
    // Extra check, for ease of mind
    if (path != mMarkdownFile)
    {
        return false;
    }

    qDebug() << sTag << "Executing pandoc because file has been changed:" << path;
    QByteArray output;

    if (!executePandoc(output))
    {
        qWarning() << sTag << "Error generating html file:" << path;
        return false;
    }

    emit generated(mOutputFile);
    return true;
}


void DocumentGenerator::watchFile(const QString& path)
{
    if (path.isEmpty())
    {
        qWarning() << sTag << "Cannot add path because it is empty";
        return;
    }

    QFileInfo finfo(path);

    if (!finfo.exists() || !finfo.isReadable())
    {
        qWarning() << sTag << "Cannot add path because it does not exist or not readable";
        return;
    }

    // Unwatch old files first
    QStringList oldPaths = mFsWatcher.files();

    if (oldPaths.size())
    {
        mFsWatcher.removePaths(oldPaths);
    }

    // Then watch requested file
    qDebug() << sTag << "Watching path" << path;
    mFsWatcher.addPath(path);

    mMarkdownFile = path;
}


bool DocumentGenerator::executePandoc(QByteArray& output)
{
    // Build command line.
    // Execute pandoc -Ss draft.md --css=pandoc.css --bibliography=bibliography.bib -o out.html
    // --bibliography is optional

    // Basic
    QString cmd("pandoc -Ss '%1' -o '%2'");
    cmd = cmd.arg(mMarkdownFile).arg(mOutputFile);

    // CSS theme
    if (!mCssFile.isNull())
    {
        cmd.append(" --css='%1'");
        cmd = cmd.arg(mCssFile);
    }

    // Bibtex
    if (!mBibtexFile.isEmpty())
    {
        cmd.append(" --bibliography='%1'");
        cmd = cmd.arg(mBibtexFile);
    }

    qDebug() << sTag << "Executing pandoc" << cmd;
    return ProcessRunner::run(cmd, output);
}
