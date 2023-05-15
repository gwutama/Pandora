#include "documentgenerator.h"
#include <QDebug>
#include <QStringList>
#include <QFileInfo>
#include <QProcess>

const char* DocumentGenerator::sTag = "[DocumentGenerator]";

DocumentGenerator::DocumentGenerator(QObject* parent) :
    QObject(parent)
{
    mExecProc.setProgram("Pandora.app/Contents/MacOS/pandoc/pandoc");
    connect(&mExecProc, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, &DocumentGenerator::processFinished);
}


DocumentGenerator::~DocumentGenerator()
{
}


void DocumentGenerator::setContent(const QString& content)
{
    if (!content.isEmpty())
    {
        if (!mTmpMarkdownFile.open())
        {
            qDebug() << sTag << "Cannot open temp file:" << mTmpMarkdownFile.fileName();
            return;
        }

        mTmpMarkdownFile.write(content.toUtf8());
        mTmpMarkdownFile.close();
    }

}

void DocumentGenerator::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode != 0 || exitStatus != QProcess::NormalExit)
    {
        qWarning() << sTag << "Error generating html file:" << mTmpMarkdownFile.fileName();
        qWarning() << sTag << "Error generating html file:" << mExecProc.readAllStandardError();
        return;
    }

    emit generated(mOutputFile);
}


void DocumentGenerator::generate(bool async)
{
    if (mExecProc.state() != QProcess::NotRunning)
    {
        qDebug() << sTag << "Not generating content because process is busy";
        return;
    }

    executePandoc(mTmpMarkdownFile.fileName(), mOutputFile, mCssFile, mBibtexFile, async);
}


void DocumentGenerator::executePandoc(const QString& markdownFile,
                                      const QString& outputFile,
                                      const QString& cssFile,
                                      const QString& bibtextFile,
                                      bool async)
{
    // Build command line.
    // Execute pandoc -Ss draft.md --css=pandoc.css --bibliography=bibliography.bib -o out.html
    // --bibliography is optional
    QStringList args;

    // Basic
    args << "-Ss" << markdownFile << "--self-contained" << "-o" << outputFile;

    // CSS theme
    if (!cssFile.isNull())
    {
        args << "-c" << cssFile;
    }

    // Bibtex
    if (!bibtextFile.isEmpty())
    {
        args << "--bibliography=" + bibtextFile;
    }

    qDebug() << sTag << "Executing pandoc" << args;
    mExecProc.setArguments(args);
    mExecProc.start();

    if (!async)
    {
        mExecProc.waitForFinished();
    }
}
