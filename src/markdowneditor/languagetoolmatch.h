#ifndef LANGUAGETOOLMATCH_H
#define LANGUAGETOOLMATCH_H

#include <QString>
#include <QStringList>
#include <QJsonObject>

class LanguageToolMatch
{
    friend class LanguageTool;

public:
    explicit LanguageToolMatch();
    virtual ~LanguageToolMatch();

    inline QString message()
    { return mMessage; }

    inline QString shortMessage()
    { return mShortMessage; }

    inline QStringList replacements()
    { return mReplacements; }

    inline int offset()
    { return mOffset; }

    inline int length()
    { return mLength; }

    void fromJsonObject(const QJsonObject& jsonObject);

private:
    QString mMessage;
    QString mShortMessage;
    QStringList mReplacements;
    int mOffset;
    int mLength;
    static const char* sTag;
};

#endif // LANGUAGETOOLMATCH_H
