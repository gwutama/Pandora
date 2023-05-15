#include "spellcheck.h"

const char* SpellCheck::sTag = "[SpellCheck]";

SpellCheck::SpellCheck(QSharedPointer<AppConfig> config,
                       QObject* parent) :
    QObject(parent),
    mConfig(config)
{
    QMap< QString, QSharedPointer<QTemporaryFile> > dict = mConfig->dictionaryFiles();
    Hunspell* hsPtr = new Hunspell(dict.value("aff")->fileName().toStdString().c_str(),
                                   dict.value("dic")->fileName().toStdString().c_str());
    mSpellCheck = QSharedPointer<Hunspell>(hsPtr);
}


SpellCheck::~SpellCheck()
{
}


bool SpellCheck::spellcheck(const QString& word)
{
    std::string wordStr = word.toStdString();
    return mSpellCheck->spell(wordStr);
}


QStringList SpellCheck::suggest(const QString& word)
{
    QStringList out;
    std::string wordStr = word.toStdString();
    std::vector<std::string> vec = mSpellCheck->suggest(wordStr);

    for (int i = 0; i < vec.size(); i++)
    {
        out.append(QString::fromStdString(vec.at(i)));
    }

    return out;
}
