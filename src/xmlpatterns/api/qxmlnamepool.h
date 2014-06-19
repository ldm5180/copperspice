/***********************************************************************
*
* Copyright (c) 2012-2014 Barbara Geller
* Copyright (c) 2012-2014 Ansel Sermersheim
* Copyright (c) 2012-2014 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This file is part of CopperSpice.
*
* CopperSpice is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with CopperSpice.  If not, see
* <http://www.gnu.org/licenses/>.
*
***********************************************************************/

#ifndef QXMLNAMEPOOL_H
#define QXMLNAMEPOOL_H

#include <QtCore/QSharedData>
#include <QtCore/QString>

QT_BEGIN_NAMESPACE

namespace QPatternist {
class NamePool;
class XsdSchemaParser;
class XsdValidatingInstanceReader;
}

namespace QPatternistSDK {
class Global;
}

class QXmlQueryPrivate;
class QXmlName;

class Q_XMLPATTERNS_EXPORT QXmlNamePool
{
 public:
   QXmlNamePool();
   QXmlNamePool(const QXmlNamePool &other);
   ~QXmlNamePool();
   QXmlNamePool &operator=(const QXmlNamePool &other);

 private:
   QXmlNamePool(QPatternist::NamePool *namePool);
   friend class QXmlQueryPrivate;
   friend class QXmlQuery;
   friend class QXmlSchemaPrivate;
   friend class QXmlSchemaValidatorPrivate;
   friend class QXmlSerializerPrivate;
   friend class QXmlName;
   friend class QPatternist::XsdSchemaParser;
   friend class QPatternist::XsdValidatingInstanceReader;
   friend class QPatternistSDK::Global;
   QExplicitlySharedDataPointer<QPatternist::NamePool> d;
};

QT_END_NAMESPACE

#endif
