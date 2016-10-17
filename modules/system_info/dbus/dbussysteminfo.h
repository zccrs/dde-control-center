/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -c DBusSystemInfo -p dbussysteminfo dbus_systeminfo
 *
 * qdbusxml2cpp is Copyright (C) 2015 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef DBUSSYSTEMINFO_H_1439185781
#define DBUSSYSTEMINFO_H_1439185781

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.deepin.daemon.SystemInfo
 */
class DBusSystemInfo: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.deepin.daemon.SystemInfo"; }

public:
    DBusSystemInfo(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~DBusSystemInfo();

    Q_PROPERTY(qulonglong DiskCap READ diskCap)
    inline qulonglong diskCap() const
    { return qvariant_cast< qulonglong >(property("DiskCap")); }

    Q_PROPERTY(qulonglong MemoryCap READ memoryCap)
    inline qulonglong memoryCap() const
    { return qvariant_cast< qulonglong >(property("MemoryCap")); }

    Q_PROPERTY(QString Processor READ processor)
    inline QString processor() const
    { return qvariant_cast< QString >(property("Processor")); }

    Q_PROPERTY(qlonglong SystemType READ systemType)
    inline qlonglong systemType() const
    { return qvariant_cast< qlonglong >(property("SystemType")); }

    Q_PROPERTY(QString Version READ version)
    inline QString version() const
    { return qvariant_cast< QString >(property("Version")); }

    Q_PROPERTY(QString DistroID READ distroId)
    inline QString distroId() const
    { return qvariant_cast< QString >(property("DistroID")); }

    Q_PROPERTY(QString DistroDesc READ distroDesc)
    inline QString distroDesc() const
    { return qvariant_cast< QString >(property("DistroDesc")); }

    Q_PROPERTY(QString DistroVer READ distroVer)
    inline QString distroVer() const
    { return qvariant_cast< QString >(property("DistroVer")); }

public Q_SLOTS: // METHODS
Q_SIGNALS: // SIGNALS
};

namespace com
{
namespace deepin
{
namespace daemon
{
typedef ::DBusSystemInfo SystemInfo;
}
}
}
#endif