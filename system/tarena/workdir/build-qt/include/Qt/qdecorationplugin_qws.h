/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://www.qtsoftware.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECORATIONPLUGIN_QWS_H
#define QDECORATIONPLUGIN_QWS_H

#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

class QDecoration;

struct Q_GUI_EXPORT QDecorationFactoryInterface : public QFactoryInterface
{
    virtual QDecoration *create(const QString &key) = 0;
};

#define QDecorationFactoryInterface_iid "com.trolltech.Qt.QDecorationFactoryInterface"
Q_DECLARE_INTERFACE(QDecorationFactoryInterface, QDecorationFactoryInterface_iid)

class Q_GUI_EXPORT QDecorationPlugin : public QObject, public QDecorationFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(QDecorationFactoryInterface:QFactoryInterface)
        public:
    explicit QDecorationPlugin(QObject *parent = 0);
    ~QDecorationPlugin();

    virtual QStringList keys() const = 0;
    virtual QDecoration *create(const QString &key) = 0;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QDECORATIONPLUGIN_QWS_H
