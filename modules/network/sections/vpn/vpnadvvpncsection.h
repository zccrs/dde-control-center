/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     listenerri <listenerri@gmail.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VPNADVVPNCSECTION_H
#define VPNADVVPNCSECTION_H

#include "../abstractsection.h"
#include "contentwidget.h"
#include "lineeditwidget.h"
#include "comboboxwidget.h"
#include "spinboxwidget.h"
#include "switchwidget.h"

#include <networkmanagerqt/vpnsetting.h>

namespace dcc {
namespace network {

class VpnAdvVPNCSection : public AbstractSection
{
    Q_OBJECT

public:
    explicit VpnAdvVPNCSection(NetworkManager::VpnSetting::Ptr vpnSetting, QFrame *parent = 0);
    virtual ~VpnAdvVPNCSection();

    bool allInputValid() Q_DECL_OVERRIDE;
    void saveSettings() Q_DECL_OVERRIDE;

Q_SIGNALS:
    void requestPage(ContentWidget * const page) const;

private:
    void initUI();
    void initConnection();

private:
    static const QMap<QString, QString> VendorStrMap;
    static const QMap<QString, QString> EncryptionStrMap;
    static const QMap<QString, QString> NATTravModeStrMap;
    static const QMap<QString, QString> IKEDHGroupStrMap;
    static const QMap<QString, QString> ForwardSecrecyStrMap;

    NetworkManager::VpnSetting::Ptr m_vpnSetting;
    NMStringMap m_dataMap;

    dcc::widgets::LineEditWidget *m_domain;
    dcc::widgets::ComboBoxWidget *m_vendorChooser;
    dcc::widgets::LineEditWidget *m_version;
    dcc::widgets::ComboBoxWidget *m_encryptionChooser;
    dcc::widgets::ComboBoxWidget *m_natTravModeChooser;
    dcc::widgets::ComboBoxWidget *m_ikeDHGroupChooser;
    dcc::widgets::ComboBoxWidget *m_forwordSecrecyChooser;
    dcc::widgets::SpinBoxWidget *m_localPort;
    dcc::widgets::SwitchWidget *m_disableDPD;

    QString m_currentVendor;
    QString m_currentEncryption;
    QString m_currentNatTravMod;
    QString m_currentIkeDHGroup;
    QString m_currentForwordSecrecy;
};

} /* network */
} /* dcc */

#endif /* VPNADVVPNCSECTION_H */