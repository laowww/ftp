#ifndef FTPCLIENT_CONST_H
#define FTPCLIENT_CONST_H

#include <QString>

const QString DATETIME_FORMAT = "yyyy/MM/dd hh:mm";
const QString DATETIME_CURLFORMAT = "MM-dd-yy  hh:mmAP";

static QString byteConversion(const qint64 &size)
{
    int integer = 0;  //整数位
    int decimal = 0;  //小数位，保留一位
    QString unit = "B";
    qint64 standardSize = size;
    qint64 curSize = size;

    if(standardSize > 1024) {
        curSize = standardSize * 1000;
        curSize /= 1024;
        integer = curSize / 1000;
        decimal = curSize % 10;
        standardSize /= 1024;
        unit = "KB";
        if(standardSize > 1024) {
            curSize = standardSize * 1000;
            curSize /= 1024;
            integer = curSize / 1000;
            decimal = curSize % 10;
            standardSize /= 1024;
            unit = "MB";
            if(standardSize > 1024) {
                curSize = standardSize * 1000;
                curSize /= 1024;
                integer = curSize / 1000;
                decimal = curSize % 10;
                unit = "GB";
            }
        }
    }

    QString dec = QString::number(decimal);
//    if (0 <= decimal && decimal <= 9) {
//        dec = dec + dec + QString::number(decimal);
//    }

//    if (10 <= decimal && decimal <= 99) {
//        dec = "0" + QString::number(decimal);
//    }

//    if (100 <= decimal && decimal <= 999) {
//        dec = QString::number(decimal);
//    }

    return QString::number(integer) + "." + dec + unit;
}

#endif // FTPCLIENT_CONST_H
