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

#ifndef QSHAREDPOINTER_H
#define QSHAREDPOINTER_H

#include <QtCore/qglobal.h>
#include <QtCore/qatomic.h>
#include <QtCore/qshareddata.h>

#ifndef Q_QDOC
# if !defined(QT_NO_MEMBER_TEMPLATES)
// QSharedPointer requires member template support
# include <QtCore/qsharedpointer_impl.h>
# endif
#else

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Core)

// These classes are here to fool qdoc into generating a better documentation

template <class T>
class QSharedPointer
{
public:
    // basic accessor functions
    T *data() const;
    bool isNull() const;
    operator bool() const;
    bool operator!() const;
    T &operator*() const;
    T *operator ->() const;

    // constructors
    QSharedPointer();
    explicit QSharedPointer(T *ptr);
    QSharedPointer(T *ptr, Deleter d);
    QSharedPointer(const QSharedPointer<T> &other);
    QSharedPointer(const QWeakPointer<T> &other);

    ~QSharedPointer() { }

    QSharedPointer<T> &operator=(const QSharedPointer<T> &other);
    QSharedPointer<T> &operator=(const QWeakPointer<T> &other);

    QWeakPointer<T> toWeakRef() const;

    void clear();

    // casts:
    template <class X> QSharedPointer<X> staticCast() const;
    template <class X> QSharedPointer<X> dynamicCast() const;
    template <class X> QSharedPointer<X> constCast() const;
};

template <class T>
class QWeakPointer
{
public:
    // basic accessor functions
    bool isNull() const;
    operator bool() const;
    bool operator!() const;

    // constructors:
    QWeakPointer();
    QWeakPointer(const QWeakPointer<T> &other);
    QWeakPointer(const QSharedPointer<T> &other);

    ~QWeakPointer();

    QWeakPointer<T> operator=(const QWeakPointer<T> &other);
    QWeakPointer<T> operator=(const QSharedPointer<T> &other);

    void clear();

    QSharedPointer<T> toStrongRef() const;
};

template<class T, class X> bool operator==(const QSharedPointer<T> &ptr1, const QSharedPointer<X> &ptr2);
template<class T, class X> bool operator!=(const QSharedPointer<T> &ptr1, const QSharedPointer<X> &ptr2);
template<class T, class X> bool operator==(const QSharedPointer<T> &ptr1, const X *ptr2);
template<class T, class X> bool operator!=(const QSharedPointer<T> &ptr1, const X *ptr2);
template<class T, class X> bool operator==(const T *ptr1, const QSharedPointer<X> &ptr2);
template<class T, class X> bool operator!=(const T *ptr1, const QSharedPointer<X> &ptr2);
template<class T, class X> bool operator==(const QWeakPointer<T> &ptr1, const QSharedPointer<X> &ptr2);
template<class T, class X> bool operator!=(const QWeakPointer<T> &ptr1, const QSharedPointer<X> &ptr2);
template<class T, class X> bool operator==(const QSharedPointer<T> &ptr1, const QWeakPointer<X> &ptr2);
template<class T, class X> bool operator!=(const QSharedPointer<T> &ptr1, const QWeakPointer<X> &ptr2);

template <class X, class T> QSharedPointer<X> qSharedPointerCast(const QSharedPointer<T> &other);
template <class X, class T> QSharedPointer<X> qSharedPointerCast(const QWeakPointer<T> &other);
template <class X, class T> QSharedPointer<X> qSharedPointerDynamicCast(const QSharedPointer<T> &src);
template <class X, class T> QSharedPointer<X> qSharedPointerDynamicCast(const QWeakPointer<T> &src);
template <class X, class T> QSharedPointer<X> qSharedPointerConstCast(const QSharedPointer<T> &src);
template <class X, class T> QSharedPointer<X> qSharedPointerConstCast(const QWeakPointer<T> &src);

template <class X, class T> QWeakPointer<X> qWeakPointerCast(const QWeakPointer<T> &src);

QT_END_NAMESPACE

QT_END_HEADER

#endif // Q_QDOC

#endif // QSHAREDPOINTER_H
