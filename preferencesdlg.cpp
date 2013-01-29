#include "preferencesdlg.h"
#include "ui_preferencesdlg.h"

PreferencesDlg::PreferencesDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDlg)
{
    ui->setupUi(this);

    QSettings settings;

    settings.beginGroup("General");
    ui->user->setText(settings.value("User").toString());
    ui->credential->setText(settings.value("Credential").toString());

    int cIdx = settings.value("Index").toInt();
    if (cIdx < ui->tabWidget->count())
        ui->tabWidget->setCurrentIndex(cIdx);

    settings.beginGroup("Video");
    ui->mplayerBuffer->setText(settings.value("mplayerBufferSize").toString());
    ui->mplayerPath->setText(settings.value("mplayerPath").toString());
}

PreferencesDlg::~PreferencesDlg()
{
    delete ui;
}

void PreferencesDlg::on_buttonBox_accepted()
{
    QSettings settings;

    settings.beginGroup("General");
    settings.setValue("Index", ui->tabWidget->currentIndex());
    settings.setValue("User", ui->user->text());
    settings.setValue("Credential", Util::getMD5Hex(ui->credential->text()));
    settings.endGroup();

    settings.beginGroup("Video");
    settings.setValue("mplayerPath", ui->mplayerPath->text());
    settings.setValue("mplayerBufferSize", ui->mplayerBuffer->text());
    settings.endGroup();
}
