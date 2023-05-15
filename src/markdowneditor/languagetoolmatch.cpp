#include "languagetoolmatch.h"
#include <QJsonArray>
#include <QDebug>

const char* LanguageToolMatch::sTag = "[LanguageToolMatch]";

LanguageToolMatch::LanguageToolMatch() :
    mOffset(-1),
    mLength(-1)
{
}


LanguageToolMatch::~LanguageToolMatch()
{
}


void LanguageToolMatch::fromJsonObject(const QJsonObject& jsonObject)
{
    if (jsonObject.contains("offset"))
    {
        mOffset = jsonObject.value("offset").toInt();
    }

    if (jsonObject.contains("length"))
    {
        mLength = jsonObject.value("length").toInt();
    }

    if (jsonObject.contains("shortMessage"))
    {
        mShortMessage = jsonObject.value("shortMessage").toString();
    }

    if (jsonObject.contains("message"))
    {
        mMessage = jsonObject.value("message").toString();
    }

    if (jsonObject.contains("replacements"))
    {
        mReplacements.clear();

        QJsonArray replacements = jsonObject.value("replacements").toArray();

        for (int i = 0; i < replacements.size(); i++)
        {
            QJsonObject obj = replacements.at(i).toObject();

            if (obj.contains("value"))
            {
                QString val = obj.value("value").toString();
                mReplacements.append(val);
            }
        }
    }
}
