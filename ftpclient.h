#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QObject>

class FtpClientPrivate;
class FtpClient
{
public:
    FtpClient();
    ~FtpClient();

public:
    void fileList(const QString &url, const QString &username = NULL, const QString &password = NULL);

private:
    FtpClientPrivate *m_pd;
};

#endif // FTPCLIENT_H
