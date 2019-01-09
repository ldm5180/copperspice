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

#ifndef QPAGELAYOUT_H
#define QPAGELAYOUT_H

#include <qsharedpointer.h>
#include <qstring.h>
#include <qmargins.h>
#include <qpagesize.h>

class QPageLayoutPrivate;

class Q_GUI_EXPORT QPageLayout
{
 public:

   // NOTE: Must keep in sync with QPageSize::Unit and QPrinter::Unit
   enum Unit {
      Millimeter,
      Point,
      Inch,
      Pica,
      Didot,
      Cicero
   };

   // NOTE: Must keep in sync with QPrinter::Orientation
   enum Orientation {
      Portrait,
      Landscape
   };

   enum Mode {
      StandardMode,  // Paint Rect includes margins
      FullPageMode   // Paint Rect excludes margins
   };

   QPageLayout();
   QPageLayout(const QPageSize &pageSize, Orientation orientation,
      const QMarginsF &margins, Unit units = Point,
      const QMarginsF &minMargins = QMarginsF(0, 0, 0, 0));

   QPageLayout(const QPageLayout &other);

   QPageLayout &operator=(QPageLayout &&other) {
      swap(other);
      return *this;
   }

   QPageLayout &operator=(const QPageLayout &other);
   ~QPageLayout();

   friend Q_GUI_EXPORT bool operator==(const QPageLayout &lhs, const QPageLayout &rhs);

   bool isEquivalentTo(const QPageLayout &other) const;

   bool isValid() const;

   void swap(QPageLayout &other) {
      qSwap(d, other.d);
   }

   void setMode(Mode mode);
   Mode mode() const;

   void setPageSize(const QPageSize &pageSize, const QMarginsF &minMargins = QMarginsF(0, 0, 0, 0));
   QPageSize pageSize() const;

   void setOrientation(Orientation orientation);
   Orientation orientation() const;

   void setUnits(Unit units);
   Unit units() const;

   bool setMargins(const QMarginsF &margins);
   bool setLeftMargin(qreal leftMargin);
   bool setRightMargin(qreal rightMargin);
   bool setTopMargin(qreal topMargin);
   bool setBottomMargin(qreal bottomMargin);

   QMarginsF margins() const;
   QMarginsF margins(Unit units) const;
   QMargins marginsPoints() const;
   QMargins marginsPixels(int resolution) const;

   void setMinimumMargins(const QMarginsF &minMargins);
   QMarginsF minimumMargins() const;
   QMarginsF maximumMargins() const;

   QRectF fullRect() const;
   QRectF fullRect(Unit units) const;
   QRect fullRectPoints() const;
   QRect fullRectPixels(int resolution) const;

   QRectF paintRect() const;
   QRectF paintRect(Unit units) const;
   QRect paintRectPoints() const;
   QRect paintRectPixels(int resolution) const;

 private:
   QExplicitlySharedDataPointer<QPageLayoutPrivate> d;

   friend class QPageLayoutPrivate;
};

Q_GUI_EXPORT bool operator==(const QPageLayout &lhs, const QPageLayout &rhs);

inline bool operator!=(const QPageLayout &lhs, const QPageLayout &rhs)
{
   return !operator==(lhs, rhs);
}

Q_GUI_EXPORT QDebug operator<<(QDebug dbg, const QPageLayout &pageLayout);

Q_DECLARE_METATYPE(QPageLayout)
Q_DECLARE_METATYPE(QPageLayout::Orientation)
Q_DECLARE_METATYPE(QPageLayout::Unit)

using pair_margin_unit = std::pair<QMarginsF, QPageLayout::Unit>;
Q_DECLARE_METATYPE(pair_margin_unit)

#endif