/***********************************************************************
*
* Copyright (c) 2012-2018 Barbara Geller
* Copyright (c) 2012-2018 Ansel Sermersheim
* Copyright (c) 2012-2016 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This file is part of CopperSpice.
*
* CopperSpice is free software. You can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* <http://www.gnu.org/licenses/>.
*
***********************************************************************/

#ifndef QCOCOASYSTEMTRAYICON_P_H
#define QCOCOASYSTEMTRAYICON_P_H

#include <QtCore/qglobal.h>

#ifndef QT_NO_SYSTEMTRAYICON

#include "QtCore/qstring.h"
#include "QtGui/qpa/qplatformsystemtrayicon.h"

QT_BEGIN_NAMESPACE

class QSystemTrayIconSys;

class Q_GUI_EXPORT QCocoaSystemTrayIcon : public QPlatformSystemTrayIcon
{
public:
    QCocoaSystemTrayIcon() : m_sys(0) {}

    void init() override;
    void cleanup() override;
    void updateIcon(const QIcon &icon) override;
    void updateToolTip(const QString &toolTip) override;
    void updateMenu(QPlatformMenu *menu) override;
    QRect geometry() const override;
    void showMessage(const QString &title, const QString &msg,
                     const QIcon& icon, MessageIcon iconType, int secs) override;

    bool isSystemTrayAvailable() const override;
    bool supportsMessages() const override;

private:
    QSystemTrayIconSys *m_sys;
};

QT_END_NAMESPACE

#endif // QT_NO_SYSTEMTRAYICON

#endif // QCOCOASYSTEMTRAYICON_P_H