#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QSharedPointer>
#include "appconfig.h"

namespace Ui
{
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QSharedPointer<AppConfig> appConfig,
                               QWidget* parent = 0);
    ~PreferencesDialog();

private slots:
    void saveSettings();

signals:
    void saved();

private:
    Ui::PreferencesDialog* mUi;
    QSharedPointer<AppConfig> mAppConfig;
};

#endif // PREFERENCESDIALOG_H
