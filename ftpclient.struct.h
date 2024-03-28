#ifndef FTPCLIENT_STRUCT_H
#define FTPCLIENT_STRUCT_H

#include <QString>
#include <QMetaType>

struct st_fileInfo
{
    QString name;
    QString date;
    qint64 size;
    bool bDir;

    st_fileInfo()
    {
        name = "";
        date = "";
        size = 0;
        bDir = false;
    }
};
Q_DECLARE_METATYPE(st_fileInfo)

#endif // FTPCLIENT_STRUCT_H
