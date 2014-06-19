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

#include "qdeduplicateiterator_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

DeduplicateIterator::DeduplicateIterator(const Item::List &source) : ListIterator<Item>(source)
   , m_listPos(0)
{
   Q_ASSERT(!Item());
   Q_ASSERT(!Item().isNode());
   Q_ASSERT(!Item().isAtomicValue());
}

Item DeduplicateIterator::next()
{
   if (m_listPos == m_list.count()) {
      m_current.reset();
      m_position = -1;
      return Item();
   }

   Item next(m_list.at(m_listPos));

   while (next.asNode().is(m_current.asNode())) {
      ++m_listPos;
      if (m_listPos == m_list.count()) {
         m_current.reset();
         m_position = -1;
         return Item();
      } else {
         next = m_list.at(m_listPos);
      }
   }

   ++m_position;
   m_current = next;
   return next;
}

xsInteger DeduplicateIterator::count()
{
   return QAbstractXmlForwardIterator<Item>::count();
}

Item::Iterator::Ptr DeduplicateIterator::copy() const
{
   return Item::Iterator::Ptr(new DeduplicateIterator(m_list));
}

QT_END_NAMESPACE
