#ifndef FTPCLIENT_STRUCT_H
#define FTPCLIENT_STRUCT_H

#include <QString>
#include <QMetaType>

struct st_fileInfo
{
    QString name;
    QString date;
    QString size;
    bool bDir;

    st_fileInfo()
    {
        name = "";
        date = "";
        size = "";
        bDir = false;
    }
};
Q_DECLARE_METATYPE(st_fileInfo)

#endif // FTPCLIENT_STRUCT_H
