#ifndef SPELLCHECK_H
#define SPELLCHECK_H

#include <QObject>
#include <QVector>
#include <hunspell/hunspell.hxx>
#include "common/appconfig.h"

class SpellCheck : public QObject
{
    Q_OBJECT

public:
    explicit SpellCheck(QSharedPointer<AppConfig> config,
                        QObject* parent = 0);
    virtual ~SpellCheck();

public slots:
    bool spellcheck(const QString& word);
    QStringList suggest(const QString& word);

private:
    QSharedPointer<AppConfig> mConfig;
    QSharedPointer<Hunspell> mSpellCheck;
    static const char* sTag;
};

#endif // SPELLCHECK_H
