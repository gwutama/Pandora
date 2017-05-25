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

    connect(mUi->buttonBox, &QDialogButtonBox::accepted,
            this, &PreferencesDialog::saveSettings);

    // Fill in values from saved settings
    mUi->markdownFileBrowse->setFileName(appConfig->markdownFile());
    mUi->bibtexFileBrowse->setFileName(appConfig->bibtexFile());
}


PreferencesDialog::~PreferencesDialog()
{
    delete mUi;
}


void PreferencesDialog::saveSettings()
{
    QString markdownFile = mUi->markdownFileBrowse->fileName();
    QString bibtexFile = mUi->bibtexFileBrowse->fileName();
    mAppConfig->setMarkdownFile(markdownFile);
    mAppConfig->setBibtexFile(bibtexFile);
    mAppConfig->save();
    emit saved();
}
