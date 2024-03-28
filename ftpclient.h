#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QObject>

#include "ftpclient.struct.h"

typedef int (*progressCallback)(void *obj, double dltotal, double dlnow, double ultotal, double ulnow);

class FtpClientPrivate;
class FtpClient
{
public:
    FtpClient();
    ~FtpClient();

public:
    bool fileList(QList<st_fileInfo> &fileList, const QString &url);

    bool download(const QString &url, const QString &localPath, void *obj = NULL, progressCallback progressCallback = NULL);

    void setTimeOut(int timeout);

    void setUser(const QString &username, const QString &password);

private:
    QByteArray encodeUrl(const QString &url);

private:
    FtpClientPrivate *m_pd;
};

#endif // FTPCLIENT_H
