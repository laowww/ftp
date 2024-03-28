#include "progress.h"
#include "ui_progress.h"
#include "FtpClient.const.h"

#include <QDebug>

Progress::Progress(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Progress)
    , m_name("")
    , m_dltotal(0)
    , m_dlnow(0)
    , m_speed(0)
{
    ui->setupUi(this);
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    ui->lb_progress->setText("0B/0B");
}

Progress::~Progress()
{
    delete ui;
}

void Progress::setName(const QString &name)
{
    m_name = name;
    ui->lb_name->setText(name);
}

void Progress::setDownLoad(double dltotal, double dlnow)
{
    m_dltotal = dltotal;
    m_dlnow = dlnow;
}

void Progress::updateProgress()
{
    int progress = 100 * m_dlnow / m_dltotal;
    int speed = m_dlnow - m_speed;

    ui->progressBar->setValue(progress);
    ui->lb_progress->setText(byteConversion(m_dlnow) + "/" + byteConversion(m_dltotal));
    ui->lb_speed->setText(byteConversion(speed) + "/s");

    m_speed = m_dlnow;
}
