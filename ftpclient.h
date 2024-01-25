#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QObject>

#include "ftpclient.struct.h"

typedef void (*progressCallback)(qint64 dltotal, qint64 dlnow, qint64 ultotal, qint64 ulnow);

class FtpClientPrivate;
class FtpClient
{
public:
    FtpClient();
    ~FtpClient();

public:
    bool fileList(QList<st_fileInfo> &fileList, const QString &url);

    bool download(const QString &url, const QString &localPath, progressCallback progressCallback = NULL);

    void setTimeOut(int timeout);

    void setUser(const QString &username, const QString &password);

private:
    FtpClientPrivate *m_pd;
};

#endif // FTPCLIENT_H
