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

#ifndef QXMLRESULTITEMS_P_H
#define QXMLRESULTITEMS_P_H

#include "qcommonvalues_p.h"
#include "qdynamiccontext_p.h"
#include "qitem_p.h"

QT_BEGIN_NAMESPACE

class QXmlResultItemsPrivate
{
 public:
   inline QXmlResultItemsPrivate() : iterator(QPatternist::CommonValues::emptyIterator)
      , hasError(false) {
   }

   void setDynamicContext(const QPatternist::DynamicContext::Ptr &context) {
      m_context = context;
   }

   QPatternist::Item::Iterator::Ptr iterator;
   QXmlItem current;
   bool hasError;

 private:
   /**
    * We never use it. We only keep a ref to it such that it doesn't get
    * de-allocated.
    */
   QPatternist::DynamicContext::Ptr    m_context;
};

QT_END_NAMESPACE
#endif

