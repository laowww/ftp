#ifndef RROGRESS_H
#define RROGRESS_H

#include <QWidget>

namespace Ui {
class Progress;
}

class Progress : public QWidget
{
    Q_OBJECT

public:
    explicit Progress(QWidget *parent = nullptr);
    ~Progress();

public:
    void setName(const QString &name);

    void setDownLoad(double dltotal, double dlnow);

    void updateProgress();

private:
    Ui::Progress *ui;

    QString m_name;
    double m_dltotal;
    double m_dlnow;
    double m_speed;
};

#endif // RROGRESS_H
