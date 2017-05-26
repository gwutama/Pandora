#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QSharedPointer<AppConfig> appConfig,
                                     QWidget* parent) :
    QDialog(parent),
    mUi(new Ui::PreferencesDialog),
    mAppConfig(appConfig)
{
    mUi->setupUi(this);
    setWindowTitle("Preferences");

    connect(mUi->buttonBox, &QDialogButtonBox::accepted, this, &PreferencesDialog::saveSettings);

    // Fill in values from saved settings
    mUi->bibtexFileBrowse->setFileName(appConfig->bibtexFile());
}


PreferencesDialog::~PreferencesDialog()
{
    delete mUi;
}


void PreferencesDialog::saveSettings()
{
    QString bibtexFile = mUi->bibtexFileBrowse->fileName();
    mAppConfig->setBibtexFile(bibtexFile);
    mAppConfig->save();
    emit saved();
}
