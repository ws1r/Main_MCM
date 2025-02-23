/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#ifndef QDIRITERATOR_H
#define QDIRITERATOR_H

#include <QtCore/qdir.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Core)

class QDirIteratorPrivate;
class Q_CORE_EXPORT QDirIterator {
public:
    enum IteratorFlag {
        NoIteratorFlags = 0x0,
        FollowSymlinks = 0x1,
        Subdirectories = 0x2
    };
    Q_DECLARE_FLAGS(IteratorFlags, IteratorFlag)

    QDirIterator(const QDir &dir, IteratorFlags flags = NoIteratorFlags);
    QDirIterator(const QString &path,
                 IteratorFlags flags = NoIteratorFlags);
    QDirIterator(const QString &path,
                 QDir::Filters filter,
                 IteratorFlags flags = NoIteratorFlags);
    QDirIterator(const QString &path,
                 const QStringList &nameFilters,
                 QDir::Filters filters = QDir::NoFilter,
                 IteratorFlags flags = NoIteratorFlags);

    virtual ~QDirIterator();

    QString next();
    bool hasNext() const;

    QString fileName() const;
    QString filePath() const;
    QFileInfo fileInfo() const;
    QString path() const;

private:
    Q_DISABLE_COPY(QDirIterator)

    QDirIteratorPrivate *d;
    friend class QDir;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QDirIterator::IteratorFlags)

QT_END_NAMESPACE

QT_END_HEADER

#endif
