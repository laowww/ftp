#include <QFile>
#include <qmath.h>
#include <QDebug>
#include <QMutex>
#include <QDateTime>
#include <QDataStream>

#include "ftpclient.h"
#include "FtpClient.const.h"

#include "curl.h"

size_t queryCallback(void* contents, size_t size, size_t nmemb, QString* output)
{
    output->append(QString::fromLocal8Bit((char*)contents, size * nmemb));
    return size * nmemb;
}

size_t writeFileCallback(char* contents, size_t size, size_t nmemb, QFile *file)
{
    file->write(contents, size * nmemb);
    return size * nmemb;
}

class FtpClientPrivate
{
public:
    int timeout;
    QString user;
    QMutex mutex;
    CURLM *pCurlMulti;

    FtpClientPrivate()
        : timeout(3)
        , user("")
    {
        curl_global_init(CURL_GLOBAL_ALL);
        pCurlMulti = curl_multi_init();
        if (!pCurlMulti)
        {
            qDebug()<< "Failed to initialize libcurl";
        }
    }

    ~FtpClientPrivate()
    {
        if (pCurlMulti)
        {
            curl_multi_cleanup(pCurlMulti);
        }
        pCurlMulti = NULL;
        curl_global_cleanup();
    }

    void perform(CURL *pCurl)
    {
        QMutexLocker locker(&mutex);
        if (!user.isEmpty())
        {
            curl_easy_setopt(pCurl, CURLOPT_USERPWD, user.toStdString().c_str());
        }
        curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, time);

        curl_multi_add_handle(pCurlMulti, pCurl);
        int runningHandles = 0;
        do
        {
            curl_multi_perform(pCurlMulti, &runningHandles);
        }
        while(runningHandles > 0);

        curl_multi_remove_handle(pCurlMulti, pCurl);
        curl_easy_cleanup(pCurl);
    }
};

FtpClient::FtpClient()
    : m_pd(new FtpClientPrivate)
{
    setTimeOut(3);
}

FtpClient::~FtpClient()
{
    delete m_pd;
    m_pd = NULL;
}

bool FtpClient::fileList(QList<st_fileInfo> &fileList, const QString &url)
{
    if(url.isEmpty())
    {
        return false;
    }

    CURL *pCurl = curl_easy_init();
    if(pCurl)
    {
        QString fileData;

        curl_easy_setopt(pCurl, CURLOPT_URL, encodeUrl(url).data());
        curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, queryCallback);
        curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &fileData);
        m_pd->perform(pCurl);

        // windows
        QRegExp pattern(QLatin1String("^(\\d\\d-\\d\\d-\\d\\d\\ \\ \\d\\d:\\d\\d[AP]M)\\s+(<DIR>|\\d+)\\s+(\\S.*)$"));
        foreach(QString str, fileData.trimmed().split("\r\n"))
        {
            if(pattern.indexIn(str.trimmed()) == 0)
            {
                QStringList infoList = pattern.capturedTexts();
                st_fileInfo info;
                info.date = QLocale::c().toDateTime(infoList.at(1), DATETIME_CURLFORMAT).addYears(100).toString(DATETIME_FORMAT);
                if(infoList.at(2).compare("<DIR>") == 0)
                {
                    info.bDir = true;
                }
                else
                {
                    info.size = infoList.at(2).toLongLong();
                }
                info.name = infoList.at(3);

                fileList.append(info);
            }
        }

        return true;
    }
    else
    {
        qDebug()<< "Failed to initialize libcurl";
    }

    return false;
}

bool FtpClient::download(const QString &url, const QString &localPath, void *obj, progressCallback progressCallback)
{
    if(url.isEmpty())
    {
        return false;
    }

    CURL *pCurl = curl_easy_init();
    if(pCurl)
    {
        QFile file(localPath);
        if(file.open(QIODevice::WriteOnly))
        {
            curl_easy_setopt(pCurl, CURLOPT_URL, encodeUrl(url).data());
            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, writeFileCallback);
            curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &file);

            curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, progressCallback);
            curl_easy_setopt(pCurl, CURLOPT_PROGRESSDATA, obj);
            curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL,1L);

            curl_multi_add_handle(m_pd->pCurlMulti, pCurl);

            m_pd->perform(pCurl);
            file.close();
        }
    }
    else
    {
        qDebug()<< "Failed to initialize libcurl";
    }

    return false;
}

void FtpClient::setTimeOut(int secs)
{
    // 设置超时时间
    m_pd->timeout = secs;
}

void FtpClient::setUser(const QString &username, const QString &password)
{
    if(!username.isEmpty() && !password.isEmpty())
    {
        m_pd->user = username + ":" + password;
    }
    else
    {
        m_pd->user = "";
    }
}

QByteArray FtpClient::encodeUrl(const QString &url)
{
    return url.toLocal8Bit().toPercentEncoding(":/!@#$&*()=:/;?+\\.");
}
