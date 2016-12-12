/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "bluetoothworker.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "pincodedialog.h"

namespace dcc {
namespace bluetooth {

BluetoothWorker::BluetoothWorker(BluetoothModel *model) :
    QObject(),
    m_bluetoothInter(new DBusBluetooth("com.deepin.daemon.Bluetooth", "/com/deepin/daemon/Bluetooth", QDBusConnection::sessionBus(), this)),
    m_model(model)
{
    connect(m_bluetoothInter, &DBusBluetooth::AdapterAdded, this, &BluetoothWorker::addAdapter);
    connect(m_bluetoothInter, &DBusBluetooth::AdapterRemoved, this, &BluetoothWorker::removeAdapter);
    connect(m_bluetoothInter, &DBusBluetooth::AdapterPropertiesChanged, this, &BluetoothWorker::onAdapterPropertiesChanged);
    connect(m_bluetoothInter, &DBusBluetooth::DevicePropertiesChanged, this, &BluetoothWorker::onDevicePropertiesChanged);

    connect(m_bluetoothInter, &DBusBluetooth::RequestAuthorization, [] (const QDBusObjectPath &in0) {
        qDebug() << "request authorization: " << in0.path();
    });

    connect(m_bluetoothInter, &DBusBluetooth::RequestConfirmation, [this] (const QDBusObjectPath &in0, const QString &in1) {
        qDebug() << "request confirmation: " << in0.path() << in1;

        PinCodeDialog dialog(in1);
        int ret = dialog.exec();

        m_bluetoothInter->Confirm(in0, bool(ret));
    });

    connect(m_bluetoothInter, &DBusBluetooth::RequestPasskey, [] (const QDBusObjectPath &in0) {
        qDebug() << "request passkey: " << in0.path();
    });

    connect(m_bluetoothInter, &DBusBluetooth::RequestPinCode, [] (const QDBusObjectPath &in0) {
        qDebug() << "request pincode: " << in0.path();
    });

    connect(m_bluetoothInter, &__Bluetooth::DisplayPasskey, [] (const QDBusObjectPath &in0, uint in1, uint in2) {
        qDebug() << "request display passkey: " << in0.path() << in1 << in2;
    });

    connect(m_bluetoothInter, &__Bluetooth::DisplayPinCode, [] (const QDBusObjectPath &in0, const QString &in1) {
        qDebug() << "request display pincode: " << in0.path() << in1;

        PinCodeDialog dialog(in1, false);
        dialog.exec();
    });

    activate();
}

void BluetoothWorker::activate()
{
    m_model->blockSignals(false);
    m_model->m_adapters.clear();
    m_bluetoothInter->ClearUnpairedDevice();

    QDBusPendingCall call = m_bluetoothInter->GetAdapters();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, [this, call] {
        if (!call.isError()) {
            QDBusReply<QString> reply = call.reply();
            const QString replyStr = reply.value();
            QJsonDocument doc = QJsonDocument::fromJson(replyStr.toUtf8());
            QJsonArray arr = doc.array();
            for (QJsonValue val : arr) {
                Adapter *adapter = new Adapter(m_model);
                inflateAdapter(adapter, val.toObject());

                m_model->addAdapter(adapter);
            }
        } else {
            qWarning() << call.error().message();
        }
    });
}

void BluetoothWorker::deactivate()
{
    m_model->blockSignals(true);
}

void BluetoothWorker::setAdapterPowered(const Adapter *adapter, const bool &powered)
{
    QDBusObjectPath path(adapter->id());
    m_bluetoothInter->SetAdapterPowered(path, powered);

    if (powered) {
        setAdapterDiscoverable(adapter->id());
    }
}

void BluetoothWorker::disconnectDevice(const Device *device)
{
    QDBusObjectPath path(device->id());
    m_bluetoothInter->DisconnectDevice(path);
    qDebug() << "disconnect from device: " << device->name();
}

void BluetoothWorker::ignoreDevice(const Adapter *adapter, const Device *device)
{
    m_bluetoothInter->RemoveDevice(QDBusObjectPath(adapter->id()), QDBusObjectPath(device->id()));
    m_bluetoothInter->ClearUnpairedDevice();
    qDebug() << "ignore device: " << device->name();
}

void BluetoothWorker::connectDevice(const Device *device)
{
    QDBusObjectPath path(device->id());
    m_bluetoothInter->ConnectDevice(path);
    qDebug() << "connect to device: " << device->name();
}

void BluetoothWorker::inflateAdapter(Adapter *adapter, const QJsonObject &adapterObj)
{
    const QString path = adapterObj["Path"].toString();
    const QString alias = adapterObj["Alias"].toString();
    const bool powered = adapterObj["Powered"].toBool();

    adapter->setId(path);
    adapter->setName(alias);
    adapter->setPowered(powered);

    QDBusObjectPath dPath(path);
    QDBusPendingCall call = m_bluetoothInter->GetDevices(dPath);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, [this, adapter, call] {
        if (!call.isError())  {
            QStringList tmpList;

            QDBusReply<QString> reply = call.reply();
            const QString replyStr = reply.value();
            QJsonDocument doc = QJsonDocument::fromJson(replyStr.toUtf8());
            QJsonArray arr = doc.array();
            for (QJsonValue val : arr) {
                const QString id = val.toObject()["Path"].toString();

                const Device *result = adapter->deviceById(id);
                Device *device = const_cast<Device*>(result);
                if (!device) device = new Device(adapter);
                inflateDevice(device, val.toObject());
                adapter->addDevice(device);

                tmpList << id;
            }

            for (const Device *device : adapter->devices()) {
                if (!tmpList.contains(device->id())) {
                    adapter->removeDevice(device->id());

                    Device *target = const_cast<Device*>(device);
                    if (target) target->deleteLater();
                }
            }
        } else {
            qWarning() << call.error().message();
        }
    });
}

void BluetoothWorker::inflateDevice(Device *device, const QJsonObject &deviceObj)
{
    const QString id = deviceObj["Path"].toString();
    const QString name = deviceObj["Alias"].toString();
    const bool paired = deviceObj["Paired"].toBool();
    const Device::State state = Device::State(deviceObj["State"].toInt());

    device->setId(id);
    device->setName(name);
    device->setPaired(paired);
    device->setState(state);
}

void BluetoothWorker::onAdapterPropertiesChanged(const QString &json)
{
    const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    const QJsonObject obj = doc.object();
    const QString id = obj["Path"].toString();
    for (const Adapter *adapter : m_model->adapters()) {
        if (adapter->id() == id) {
            Adapter *vAdapter = const_cast<Adapter*>(adapter);
            inflateAdapter(vAdapter, obj);
            break;
        }
    }
}

void BluetoothWorker::onDevicePropertiesChanged(const QString &json)
{
    const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    const QJsonObject obj = doc.object();
    const QString id = obj["Path"].toString();
    for (const Adapter *adapter : m_model->adapters()) {
        for (const Device *device : adapter->devices()) {
            if (device->id() == id) {
                Device *vDevice = const_cast<Device*>(device);
                inflateDevice(vDevice, obj);
                return;
            }
        }
    }
}

void BluetoothWorker::addAdapter(const QString &json)
{
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject obj = doc.object();

    Adapter *adapter = new Adapter(m_model);
    inflateAdapter(adapter, obj);
    m_model->addAdapter(adapter);
}

void BluetoothWorker::removeAdapter(const QString &json)
{
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject obj = doc.object();
    const QString id = obj["Path"].toString();
    m_model->removeAdapater(id);

    const Adapter *result = m_model->adapterById(id);
    Adapter *adapter = const_cast<Adapter*>(result);
    if (adapter) adapter->deleteLater();
}

void BluetoothWorker::setAdapterDiscoverable(const QString &path)
{
    QDBusObjectPath dPath(path);
    m_bluetoothInter->SetAdapterDiscoverable(dPath, true);
    m_bluetoothInter->SetAdapterDiscoverableTimeout(dPath, 60 * 5);

    m_bluetoothInter->RequestDiscovery(dPath);
}

} // namespace bluetooth
} // namespace dcc