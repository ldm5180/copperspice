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

// NOTE: This file is AUTO GENERATED by qautomaton2cpp.xsl

#ifndef QPatternist_XsdSchemaToken_h
#define QPatternist_XsdSchemaToken_h

#include <QtCore/QString>

QT_BEGIN_NAMESPACE

class XsdSchemaToken
{
 public:
   enum NodeName

   {
      NoKeyword,
      Abstract,
      All,
      Alternative,
      Annotation,
      Any,
      AnyAttribute,
      Appinfo,
      AppliesToEmpty,
      Assert,
      Assertion,
      Attribute,
      AttributeFormDefault,
      AttributeGroup,
      Base,
      Block,
      BlockDefault,
      Choice,
      Collapse,
      ComplexContent,
      ComplexType,
      Default,
      DefaultAttributes,
      DefaultAttributesApply,
      DefaultOpenContent,
      Documentation,
      Element,
      ElementFormDefault,
      Enumeration,
      Extension,
      Field,
      Final,
      FinalDefault,
      Fixed,
      Form,
      FractionDigits,
      Group,
      Id,
      Import,
      Include,
      ItemType,
      Key,
      Keyref,
      Length,
      List,
      MaxExclusive,
      MaxInclusive,
      MaxLength,
      MaxOccurs,
      MemberTypes,
      MinExclusive,
      MinInclusive,
      MinLength,
      MinOccurs,
      Mixed,
      Mode,
      Name,
      Namespace,
      Nillable,
      NotNamespace,
      NotQName,
      Notation,
      OpenContent,
      Override,
      Pattern,
      Preserve,
      ProcessContents,
      Public,
      Redefine,
      Ref,
      Refer,
      Replace,
      Restriction,
      Schema,
      SchemaLocation,
      Selector,
      Sequence,
      SimpleContent,
      SimpleType,
      Source,
      SubstitutionGroup,
      System,
      TargetNamespace,
      Test,
      TotalDigits,
      Type,
      Union,
      Unique,
      Use,
      Value,
      Version,
      WhiteSpace,
      XML_NS_SCHEMA_URI,
      XPathDefaultNamespace,
      XmlLanguage,
      Xpath
   };

   static inline NodeName toToken(const QString &value);
   static inline NodeName toToken(const QStringRef &value);
   static NodeName toToken(const QChar *data, int length);
   static QString toString(NodeName token);


 private:
   static inline NodeName classifier2(const QChar *data);
   static inline NodeName classifier3(const QChar *data);
   static inline NodeName classifier4(const QChar *data);
   static inline NodeName classifier5(const QChar *data);
   static inline NodeName classifier6(const QChar *data);
   static inline NodeName classifier7(const QChar *data);
   static inline NodeName classifier8(const QChar *data);
   static inline NodeName classifier9(const QChar *data);
   static inline NodeName classifier10(const QChar *data);
   static inline NodeName classifier11(const QChar *data);
   static inline NodeName classifier12(const QChar *data);
   static inline NodeName classifier13(const QChar *data);
   static inline NodeName classifier14(const QChar *data);
   static inline NodeName classifier15(const QChar *data);
   static inline NodeName classifier17(const QChar *data);
   static inline NodeName classifier18(const QChar *data);
   static inline NodeName classifier20(const QChar *data);
   static inline NodeName classifier21(const QChar *data);
   static inline NodeName classifier22(const QChar *data);
   static inline NodeName classifier32(const QChar *data);

};

inline XsdSchemaToken::NodeName XsdSchemaToken::toToken(const QString &value)
{
   return toToken(value.constData(), value.length());
}

inline XsdSchemaToken::NodeName XsdSchemaToken::toToken(const QStringRef &value)
{
   return toToken(value.constData(), value.length());
}

QT_END_NAMESPACE

#endif
