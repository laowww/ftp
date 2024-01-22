#include <QDebug>

#include "ftpclient.h"

#include "curl.h"

size_t writeCallback(void* contents, size_t size, size_t nmemb, QString* output)
{
    output->append(QString::fromLocal8Bit((char*)contents, size * nmemb));
    return size * nmemb;
}

class FtpClientPrivate
{
public:
    CURL *pCurl;

    FtpClientPrivate()
    {
        curl_global_init(CURL_GLOBAL_ALL);
        pCurl = curl_easy_init();
        if (!pCurl)
        {
            qDebug()<< "Failed to initialize libcurl";
        }
    }

    ~FtpClientPrivate()
    {
        if (pCurl)
        {
            curl_easy_cleanup(pCurl);
        }
        pCurl = NULL;
        curl_global_cleanup();
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

void FtpClient::fileList(const QString &url, const QString &username, const QString &password)
{
    if(url.isEmpty())
    {
        return;
    }

    QString fileData;

    curl_easy_setopt(m_pd->pCurl, CURLOPT_URL, url.toStdString().c_str());
    curl_easy_setopt(m_pd->pCurl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(m_pd->pCurl, CURLOPT_WRITEDATA, &fileData);

    if (!username.isEmpty() && !password.isEmpty())
    {
        QString credentials = username + ":" + password;
        curl_easy_setopt(m_pd->pCurl, CURLOPT_USERPWD, credentials.toStdString().c_str());
    }

    // 解决中文文件名称乱码问题
    //curl_easy_setopt(m_pd->pCurl, CURLOPT_TRANSFERTEXT, 1L);
    //curl_easy_setopt(m_pd->pCurl, CURLOPT_ENCODING, "UTF-8");

    CURLcode res = curl_easy_perform(m_pd->pCurl);
    if (res != CURLE_OK)
    {
        qDebug() << "curl_easy_perform() failed: " << curl_easy_strerror(res);
    }

    foreach(const QString &str, fileData.trimmed().split("\r\n"))
    {
        qDebug()<< str.split("\t");
    }
}

void FtpClient::setTimeOut(int time)
{
    // 设置超时时间
    curl_easy_setopt(m_pd->pCurl, CURLOPT_TIMEOUT, time);
}
