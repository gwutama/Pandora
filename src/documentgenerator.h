#ifndef DOCUMENTGENERATOR_H
#define DOCUMENTGENERATOR_H

#include <QFileSystemWatcher>

class DocumentGenerator : public QObject
{
    Q_OBJECT

public:
    explicit DocumentGenerator(const QString& markdownFile,
                               QObject* parent = 0);
    virtual ~DocumentGenerator();

    inline void setCssFile(const QString& cssFile)
    { mCssFile = cssFile; }

    inline void setOutputFile(const QString& outputFile)
    { mOutputFile = outputFile; }

    inline void setBibtexFile(const QString& bibtexFile)
    { mBibtexFile = bibtexFile; }

public slots:
    void generate(const QString& path);

private:
    void watchFile(const QString& path);
    bool executePandoc(QByteArray& output);

signals:
    void fileChanged(const QString& path);
    void generated(const QString& path);

private:
    QString mMarkdownFile;
    QString mCssFile;
    QString mOutputFile;
    QString mBibtexFile;
    QFileSystemWatcher mFsWatcher;
};

#endif // DOCUMENTGENERATOR_H
