/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtScript module of the Qt Toolkit.
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

#ifndef QSCRIPTEXTENSIONPLUGIN_H
#define QSCRIPTEXTENSIONPLUGIN_H

#include <QtCore/qplugin.h>

#ifndef QT_NO_SCRIPT

#include <QtScript/qscriptextensioninterface.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Script)

class QScriptValue;

class Q_SCRIPT_EXPORT QScriptExtensionPlugin : public QObject,
                                               public QScriptExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(QScriptExtensionInterface:QFactoryInterface)
public:
    explicit QScriptExtensionPlugin(QObject *parent = 0);
    ~QScriptExtensionPlugin();

    virtual QStringList keys() const = 0;
    virtual void initialize(const QString &key, QScriptEngine *engine) = 0;

    QScriptValue setupPackage(const QString &key, QScriptEngine *engine) const;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QT_NO_SCRIPT
#endif // QSCRIPTEXTENSIONPLUGIN_H
