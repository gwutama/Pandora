#ifndef DOCUMENTGENERATOR_H
#define DOCUMENTGENERATOR_H

#include <QTemporaryFile>
#include <QProcess>

class DocumentGenerator : public QObject
{
    Q_OBJECT

public:
    explicit DocumentGenerator(QObject* parent = 0);
    virtual ~DocumentGenerator();

    inline void setCssFile(const QString& cssFile)
    { mCssFile = cssFile; }

    inline void setOutputFile(const QString& outputFile)
    { mOutputFile = outputFile; }

    inline void setBibtexFile(const QString& bibtexFile)
    { mBibtexFile = bibtexFile; }

    void setContent(const QString& content);

public slots:
    void generate(bool async = true);

private:
    void executePandoc(const QString& markdownFile,
                       const QString& outputFile,
                       const QString& cssFile = "",
                       const QString& bibtextFile = "",
                       bool async = true);

private slots:
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

signals:
    void generated(const QString& path);

private:
    QProcess mExecProc;
    QTemporaryFile mTmpMarkdownFile;
    QString mCssFile;
    QString mOutputFile;
    QString mBibtexFile;
    static const char* sTag;
};

#endif // DOCUMENTGENERATOR_H
