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

#include "qarithmeticexpression_p.h"
#include "qcommonvalues_p.h"
#include "qliteral_p.h"
#include "qschemanumeric_p.h"

#include "qunaryexpression_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

UnaryExpression::UnaryExpression(const AtomicMathematician::Operator op,
                                 const Expression::Ptr &operand,
                                 const StaticContext::Ptr &context) : ArithmeticExpression(wrapLiteral(CommonValues::IntegerZero, context,
                                          operand.data()),
                                          op,
                                          operand)
{
   Q_ASSERT(op == AtomicMathematician::Substract ||
            op == AtomicMathematician::Add);
   Q_ASSERT(context);
}

Item UnaryExpression::evaluateSingleton(const DynamicContext::Ptr &context) const
{
   if (operatorID() == AtomicMathematician::Substract) {
      const Item item(m_operand2->evaluateSingleton(context));

      if (item) {
         return item.as<Numeric>()->toNegated();
      } else {
         return Item();
      }
   } else {
      return m_operand2->evaluateSingleton(context);
   }
}

QT_END_NAMESPACE
