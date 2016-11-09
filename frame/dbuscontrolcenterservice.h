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
 * Command line was: qdbusxml2cpp dde-center -a dbuscontrolcenter -c DBusControlCenter -l Frame
 *
 * qdbusxml2cpp is Copyright (C) 2015 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef DBUSCONTROLCENTER_H_1439447183
#define DBUSCONTROLCENTER_H_1439447183

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

#include "frame.h"

/*
 * Adaptor class for interface com.deepin.dde.ControlCenter
 */
class Frame;
class DBusControlCenterService: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.ControlCenter")
    Q_CLASSINFO("D-Bus Introspection", ""
                "  <interface name=\"com.deepin.dde.ControlCenter\">\n"
                "    <property access=\"read\" type=\"i\" name=\"X\"/>\n"
                "    <property access=\"read\" type=\"b\" name=\"ShowInRight\"/>\n"
                "    <method name=\"Toggle\"/>\n"
                "    <method name=\"ToggleInLeft\"/>\n"
                "    <method name=\"Show\"/>\n"
                "    <method name=\"ShowModule\">\n"
                "      <arg direction=\"in\" type=\"s\" name=\"name\"/>\n"
                "    </method>\n"
                "    <method name=\"ShowImmediately\"/>\n"
                "    <method name=\"Hide\"/>\n"
                "    <method name=\"HideImmediately\"/>\n"
                "    <method name=\"isNetworkCanShowPassword\">\n"
                "      <arg direction=\"out\" type=\"b\"/>\n"
                "    </method>\n"
                "  </interface>\n"
                "")
public:
    explicit DBusControlCenterService(Frame *parent);
    virtual ~DBusControlCenterService();

    inline Frame *parent() const;

public: // PROPERTIES
    Q_PROPERTY(bool ShowInRight READ showInRight)
    bool showInRight() const;

    Q_PROPERTY(int X READ x)
    int x() const;

public Q_SLOTS: // METHODS
    void Hide();
    void HideImmediately();
    void Show();
    void ShowImmediately();
    void ShowModule(const QString &name);
    void Toggle();
    void ToggleInLeft();
    bool isNetworkCanShowPassword();
Q_SIGNALS: // SIGNALS
};

#endif