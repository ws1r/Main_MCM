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

#ifndef QTEXTFORMAT_H
#define QTEXTFORMAT_H

#include <QtGui/qcolor.h>
#include <QtGui/qfont.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qvector.h>
#include <QtCore/qvariant.h>
#include <QtGui/qpen.h>
#include <QtGui/qbrush.h>
#include <QtGui/qtextoption.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

class QString;
class QVariant;
class QFont;

class QTextFormatCollection;
class QTextFormatPrivate;
class QTextBlockFormat;
class QTextCharFormat;
class QTextListFormat;
class QTextTableFormat;
class QTextFrameFormat;
class QTextImageFormat;
class QTextTableCellFormat;
class QTextFormat;
class QTextObject;
class QTextCursor;
class QTextDocument;
class QTextLength;

Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QTextLength &);
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QTextLength &);

class Q_GUI_EXPORT QTextLength
{
public:
    enum Type { VariableLength = 0, FixedLength, PercentageLength };

    inline QTextLength() : lengthType(VariableLength), fixedValueOrPercentage(0) {}

    inline explicit QTextLength(Type type, qreal value);

    inline Type type() const { return lengthType; }
    inline qreal value(qreal maximumLength) const
    {
        switch (lengthType) {
            case FixedLength: return fixedValueOrPercentage;
            case VariableLength: return maximumLength;
            case PercentageLength: return fixedValueOrPercentage * maximumLength / qreal(100);
        }
        return -1;
    }

    inline qreal rawValue() const { return fixedValueOrPercentage; }

    inline bool operator==(const QTextLength &other) const
    { return lengthType == other.lengthType
             && qFuzzyCompare(fixedValueOrPercentage, other.fixedValueOrPercentage); }
    inline bool operator!=(const QTextLength &other) const
    { return lengthType != other.lengthType
             || !qFuzzyCompare(fixedValueOrPercentage, other.fixedValueOrPercentage); }
    operator QVariant() const;

private:
    Type lengthType;
    qreal fixedValueOrPercentage;
    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QTextLength &);
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QTextLength &);
};

inline QTextLength::QTextLength(Type atype, qreal avalue)
    : lengthType(atype), fixedValueOrPercentage(avalue) {}

Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QTextFormat &);
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QTextFormat &);

class Q_GUI_EXPORT QTextFormat
{
    Q_GADGET
    Q_ENUMS(FormatType Property ObjectTypes)
public:
    enum FormatType {
        InvalidFormat = -1,
        BlockFormat = 1,
        CharFormat = 2,
        ListFormat = 3,
        TableFormat = 4,
        FrameFormat = 5,

        UserFormat = 100
    };

    enum Property {
        ObjectIndex = 0x0,

        // paragraph and char
        CssFloat = 0x0800,
        LayoutDirection = 0x0801,

        OutlinePen = 0x810,
        BackgroundBrush = 0x820,
        ForegroundBrush = 0x821,
        // Internal to qtextlayout.cpp: ObjectSelectionBrush = 0x822
        BackgroundImageUrl = 0x823,

        // paragraph
        BlockAlignment = 0x1010,
        BlockTopMargin = 0x1030,
        BlockBottomMargin = 0x1031,
        BlockLeftMargin = 0x1032,
        BlockRightMargin = 0x1033,
        TextIndent = 0x1034,
        TabPositions = 0x1035,
        BlockIndent = 0x1040,
        BlockNonBreakableLines = 0x1050,
        BlockTrailingHorizontalRulerWidth = 0x1060,

        // character properties
        FirstFontProperty = 0x1FE0,
        FontCapitalization = FirstFontProperty,
        FontLetterSpacing = 0x1FE1,
        FontWordSpacing = 0x1FE2,
        FontStyleHint = 0x1FE3,
        FontStyleStrategy = 0x1FE4,
        FontKerning = 0x1FE5,
        FontFamily = 0x2000,
        FontPointSize = 0x2001,
        FontSizeAdjustment = 0x2002,
        FontSizeIncrement = FontSizeAdjustment, // old name, compat
        FontWeight = 0x2003,
        FontItalic = 0x2004,
        FontUnderline = 0x2005, // deprecated, use TextUnderlineStyle instead
        FontOverline = 0x2006,
        FontStrikeOut = 0x2007,
        FontFixedPitch = 0x2008,
        FontPixelSize = 0x2009,
        LastFontProperty = FontPixelSize,

        TextUnderlineColor = 0x2010,
        TextVerticalAlignment = 0x2021,
        TextOutline = 0x2022,
        TextUnderlineStyle = 0x2023,
        TextToolTip = 0x2024,

        IsAnchor = 0x2030,
        AnchorHref = 0x2031,
        AnchorName = 0x2032,
        ObjectType = 0x2f00,

        // list properties
        ListStyle = 0x3000,
        ListIndent = 0x3001,

        // table and frame properties
        FrameBorder = 0x4000,
        FrameMargin = 0x4001,
        FramePadding = 0x4002,
        FrameWidth = 0x4003,
        FrameHeight = 0x4004,
        FrameTopMargin    = 0x4005,
        FrameBottomMargin = 0x4006,
        FrameLeftMargin   = 0x4007,
        FrameRightMargin  = 0x4008,
        FrameBorderBrush = 0x4009,
        FrameBorderStyle = 0x4010,

        TableColumns = 0x4100,
        TableColumnWidthConstraints = 0x4101,
        TableCellSpacing = 0x4102,
        TableCellPadding = 0x4103,
        TableHeaderRowCount = 0x4104,

        // table cell properties
        TableCellRowSpan = 0x4810,
        TableCellColumnSpan = 0x4811,

        TableCellTopPadding = 0x4812,
        TableCellBottomPadding = 0x4813,
        TableCellLeftPadding = 0x4814,
        TableCellRightPadding = 0x4815,

        // image properties
        ImageName = 0x5000,
        ImageWidth = 0x5010,
        ImageHeight = 0x5011,

        // internal
        /*
           SuppressText = 0x5012,
           SuppressBackground = 0x513
        */

        // selection properties
        FullWidthSelection = 0x06000,

        // page break properties
        PageBreakPolicy = 0x7000,

        // --
        UserProperty = 0x100000
    };

    enum ObjectTypes {
        NoObject,
        ImageObject,
        TableObject,
        TableCellObject,

        UserObject = 0x1000
    };

    enum PageBreakFlag {
        PageBreak_Auto = 0,
        PageBreak_AlwaysBefore = 0x001,
        PageBreak_AlwaysAfter  = 0x010
        // PageBreak_AlwaysInside = 0x100
    };
    Q_DECLARE_FLAGS(PageBreakFlags, PageBreakFlag)

    QTextFormat();

    explicit QTextFormat(int type);

    QTextFormat(const QTextFormat &rhs);
    QTextFormat &operator=(const QTextFormat &rhs);
    ~QTextFormat();

    void merge(const QTextFormat &other);

    inline bool isValid() const { return type() != InvalidFormat; }

    int type() const;

    int objectIndex() const;
    void setObjectIndex(int object);

    QVariant property(int propertyId) const;
    void setProperty(int propertyId, const QVariant &value);
    void clearProperty(int propertyId);
    bool hasProperty(int propertyId) const;

    bool boolProperty(int propertyId) const;
    int intProperty(int propertyId) const;
    qreal doubleProperty(int propertyId) const;
    QString stringProperty(int propertyId) const;
    QColor colorProperty(int propertyId) const;
    QPen penProperty(int propertyId) const;
    QBrush brushProperty(int propertyId) const;
    QTextLength lengthProperty(int propertyId) const;
    QVector<QTextLength> lengthVectorProperty(int propertyId) const;

    void setProperty(int propertyId, const QVector<QTextLength> &lengths);

    QMap<int, QVariant> properties() const;
    int propertyCount() const;

    inline void setObjectType(int type);
    inline int objectType() const
    { return intProperty(ObjectType); }

    inline bool isCharFormat() const { return type() == CharFormat; }
    inline bool isBlockFormat() const { return type() == BlockFormat; }
    inline bool isListFormat() const { return type() == ListFormat; }
    inline bool isFrameFormat() const { return type() == FrameFormat; }
    inline bool isImageFormat() const { return type() == CharFormat && objectType() == ImageObject; }
    inline bool isTableFormat() const { return type() == FrameFormat && objectType() == TableObject; }
    inline bool isTableCellFormat() const { return type() == CharFormat && objectType() == TableCellObject; }

    QTextBlockFormat toBlockFormat() const;
    QTextCharFormat toCharFormat() const;
    QTextListFormat toListFormat() const;
    QTextTableFormat toTableFormat() const;
    QTextFrameFormat toFrameFormat() const;
    QTextImageFormat toImageFormat() const;
    QTextTableCellFormat toTableCellFormat() const;

    bool operator==(const QTextFormat &rhs) const;
    inline bool operator!=(const QTextFormat &rhs) const { return !operator==(rhs); }
    operator QVariant() const;

    inline void setLayoutDirection(Qt::LayoutDirection direction)
        { setProperty(QTextFormat::LayoutDirection, direction); }
    inline Qt::LayoutDirection layoutDirection() const
        { return Qt::LayoutDirection(intProperty(QTextFormat::LayoutDirection)); }

    inline void setBackground(const QBrush &brush)
    { setProperty(BackgroundBrush, brush); }
    inline QBrush background() const
    { return brushProperty(BackgroundBrush); }
    inline void clearBackground()
    { clearProperty(BackgroundBrush); }

    inline void setForeground(const QBrush &brush)
    { setProperty(ForegroundBrush, brush); }
    inline QBrush foreground() const
    { return brushProperty(ForegroundBrush); }
    inline void clearForeground()
    { clearProperty(ForegroundBrush); }

private:
    QSharedDataPointer<QTextFormatPrivate> d;
    qint32 format_type;

    friend class QTextFormatCollection;
    friend class QTextCharFormat;
    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QTextFormat &);
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QTextFormat &);
};

inline void QTextFormat::setObjectType(int atype)
{ setProperty(ObjectType, atype); }

Q_DECLARE_OPERATORS_FOR_FLAGS(QTextFormat::PageBreakFlags)

class Q_GUI_EXPORT QTextCharFormat : public QTextFormat
{
public:
    enum VerticalAlignment {
        AlignNormal = 0,
        AlignSuperScript,
        AlignSubScript,
        AlignMiddle,
        AlignTop,
        AlignBottom
    };
    enum UnderlineStyle { // keep in sync with Qt::PenStyle!
        NoUnderline,
        SingleUnderline,
        DashUnderline,
        DotLine,
        DashDotLine,
        DashDotDotLine,
        WaveUnderline,
        SpellCheckUnderline
    };

    QTextCharFormat();

    bool isValid() const { return isCharFormat(); }
    void setFont(const QFont &font);
    QFont font() const;

    inline void setFontFamily(const QString &family)
    { setProperty(FontFamily, family); }
    inline QString fontFamily() const
    { return stringProperty(FontFamily); }

    inline void setFontPointSize(qreal size)
    { setProperty(FontPointSize, size); }
    inline qreal fontPointSize() const
    { return doubleProperty(FontPointSize); }

    inline void setFontWeight(int weight)
    { if (weight == QFont::Normal) weight = 0; setProperty(FontWeight, weight); }
    inline int fontWeight() const
    { int weight = intProperty(FontWeight); if (weight == 0) weight = QFont::Normal; return weight; }
    inline void setFontItalic(bool italic)
    { setProperty(FontItalic, italic); }
    inline bool fontItalic() const
    { return boolProperty(FontItalic); }
    inline void setFontCapitalization(QFont::Capitalization capitalization)
    { setProperty(FontCapitalization, capitalization); }
    inline QFont::Capitalization fontCapitalization() const
    { return static_cast<QFont::Capitalization>(intProperty(FontCapitalization)); }
    inline void setFontLetterSpacing(qreal spacing)
    { setProperty(FontLetterSpacing, spacing); }
    inline qreal fontLetterSpacing() const
    { return doubleProperty(FontLetterSpacing); }
    inline void setFontWordSpacing(qreal spacing)
    { setProperty(FontWordSpacing, spacing); }
    inline qreal fontWordSpacing() const
    { return doubleProperty(FontWordSpacing); }

    inline void setFontUnderline(bool underline)
    { setProperty(TextUnderlineStyle, underline ? SingleUnderline : NoUnderline); }
    bool fontUnderline() const;

    inline void setFontOverline(bool overline)
    { setProperty(FontOverline, overline); }
    inline bool fontOverline() const
    { return boolProperty(FontOverline); }

    inline void setFontStrikeOut(bool strikeOut)
    { setProperty(FontStrikeOut, strikeOut); }
    inline bool fontStrikeOut() const
    { return boolProperty(FontStrikeOut); }

    inline void setUnderlineColor(const QColor &color)
    { setProperty(TextUnderlineColor, color); }
    inline QColor underlineColor() const
    { return colorProperty(TextUnderlineColor); }

    inline void setFontFixedPitch(bool fixedPitch)
    { setProperty(FontFixedPitch, fixedPitch); }
    inline bool fontFixedPitch() const
    { return boolProperty(FontFixedPitch); }

    inline void setFontStyleHint(QFont::StyleHint hint, QFont::StyleStrategy strategy = QFont::PreferDefault)
    { setProperty(FontStyleHint, hint); setProperty(FontStyleStrategy, strategy); }
    inline void setFontStyleStrategy(QFont::StyleStrategy strategy)
    { setProperty(FontStyleStrategy, strategy); }
    QFont::StyleHint fontStyleHint() const
    { return static_cast<QFont::StyleHint>(intProperty(FontStyleHint)); }
    QFont::StyleStrategy fontStyleStrategy() const
    { return static_cast<QFont::StyleStrategy>(intProperty(FontStyleStrategy)); }

    inline void setFontKerning(bool enable)
    { setProperty(FontKerning, enable); }
    inline bool fontKerning() const
    { return boolProperty(FontKerning); }

    void setUnderlineStyle(UnderlineStyle style);
    inline UnderlineStyle underlineStyle() const
    { return static_cast<UnderlineStyle>(intProperty(TextUnderlineStyle)); }

    inline void setVerticalAlignment(VerticalAlignment alignment)
    { setProperty(TextVerticalAlignment, alignment); }
    inline VerticalAlignment verticalAlignment() const
    { return static_cast<VerticalAlignment>(intProperty(TextVerticalAlignment)); }

    inline void setTextOutline(const QPen &pen)
    { setProperty(TextOutline, pen); }
    inline QPen textOutline() const
    { return penProperty(TextOutline); }

    inline void setToolTip(const QString &tip)
    { setProperty(TextToolTip, tip); }
    inline QString toolTip() const
    { return stringProperty(TextToolTip); }

    inline void setAnchor(bool anchor)
    { setProperty(IsAnchor, anchor); }
    inline bool isAnchor() const
    { return boolProperty(IsAnchor); }

    inline void setAnchorHref(const QString &value)
    { setProperty(AnchorHref, value); }
    inline QString anchorHref() const
    { return stringProperty(AnchorHref); }

    inline void setAnchorName(const QString &name)
    { setAnchorNames(QStringList(name)); }
    QString anchorName() const;

    inline void setAnchorNames(const QStringList &names)
    { setProperty(AnchorName, names); }
    QStringList anchorNames() const;

    inline void setTableCellRowSpan(int tableCellRowSpan);
    inline int tableCellRowSpan() const
    { int s = intProperty(TableCellRowSpan); if (s == 0) s = 1; return s; }
    inline void setTableCellColumnSpan(int tableCellColumnSpan);
    inline int tableCellColumnSpan() const
    { int s = intProperty(TableCellColumnSpan); if (s == 0) s = 1; return s; }

protected:
    explicit QTextCharFormat(const QTextFormat &fmt);
    friend class QTextFormat;
};

inline void QTextCharFormat::setTableCellRowSpan(int _tableCellRowSpan)
{
    if (_tableCellRowSpan <= 1)
        clearProperty(TableCellRowSpan); // the getter will return 1 here.
    else
        setProperty(TableCellRowSpan, _tableCellRowSpan);
}

inline void QTextCharFormat::setTableCellColumnSpan(int _tableCellColumnSpan)
{
    if (_tableCellColumnSpan <= 1)
        clearProperty(TableCellColumnSpan); // the getter will return 1 here.
    else
        setProperty(TableCellColumnSpan, _tableCellColumnSpan);
}

class Q_GUI_EXPORT QTextBlockFormat : public QTextFormat
{
public:
    QTextBlockFormat();

    bool isValid() const { return isBlockFormat(); }

    inline void setAlignment(Qt::Alignment alignment);
    inline Qt::Alignment alignment() const
    { int a = intProperty(BlockAlignment); if (a == 0) a = Qt::AlignLeft; return QFlag(a); }

    inline void setTopMargin(qreal margin)
    { setProperty(BlockTopMargin, margin); }
    inline qreal topMargin() const
    { return doubleProperty(BlockTopMargin); }

    inline void setBottomMargin(qreal margin)
    { setProperty(BlockBottomMargin, margin); }
    inline qreal bottomMargin() const
    { return doubleProperty(BlockBottomMargin); }

    inline void setLeftMargin(qreal margin)
    { setProperty(BlockLeftMargin, margin); }
    inline qreal leftMargin() const
    { return doubleProperty(BlockLeftMargin); }

    inline void setRightMargin(qreal margin)
    { setProperty(BlockRightMargin, margin); }
    inline qreal rightMargin() const
    { return doubleProperty(BlockRightMargin); }

    inline void setTextIndent(qreal aindent)
    { setProperty(TextIndent, aindent); }
    inline qreal textIndent() const
    { return doubleProperty(TextIndent); }

    inline void setIndent(int indent);
    inline int indent() const
    { return intProperty(BlockIndent); }

    inline void setNonBreakableLines(bool b)
    { setProperty(BlockNonBreakableLines, b); }
    inline bool nonBreakableLines() const
    { return boolProperty(BlockNonBreakableLines); }

    inline void setPageBreakPolicy(PageBreakFlags flags)
    { setProperty(PageBreakPolicy, int(flags)); }
    inline PageBreakFlags pageBreakPolicy() const
    { return PageBreakFlags(intProperty(PageBreakPolicy)); }

    void setTabPositions(const QList<QTextOption::Tab> &tabs);
    QList<QTextOption::Tab> tabPositions() const;

protected:
    explicit QTextBlockFormat(const QTextFormat &fmt);
    friend class QTextFormat;
};

inline void QTextBlockFormat::setAlignment(Qt::Alignment aalignment)
{ setProperty(BlockAlignment, int(aalignment)); }

inline void QTextBlockFormat::setIndent(int aindent)
{ setProperty(BlockIndent, aindent); }

class Q_GUI_EXPORT QTextListFormat : public QTextFormat
{
public:
    QTextListFormat();

    bool isValid() const { return isListFormat(); }

    enum Style {
        ListDisc = -1,
        ListCircle = -2,
        ListSquare = -3,
        ListDecimal = -4,
        ListLowerAlpha = -5,
        ListUpperAlpha = -6,
        ListStyleUndefined = 0
    };

    inline void setStyle(Style style);
    inline Style style() const
    { return static_cast<Style>(intProperty(ListStyle)); }

    inline void setIndent(int indent);
    inline int indent() const
    { return intProperty(ListIndent); }

protected:
    explicit QTextListFormat(const QTextFormat &fmt);
    friend class QTextFormat;
};

inline void QTextListFormat::setStyle(Style astyle)
{ setProperty(ListStyle, astyle); }

inline void QTextListFormat::setIndent(int aindent)
{ setProperty(ListIndent, aindent); }

class Q_GUI_EXPORT QTextImageFormat : public QTextCharFormat
{
public:
    QTextImageFormat();

    bool isValid() const { return isImageFormat(); }

    inline void setName(const QString &name);
    inline QString name() const
    { return stringProperty(ImageName); }

    inline void setWidth(qreal width);
    inline qreal width() const
    { return doubleProperty(ImageWidth); }

    inline void setHeight(qreal height);
    inline qreal height() const
    { return doubleProperty(ImageHeight); }

protected:
    explicit QTextImageFormat(const QTextFormat &format);
    friend class QTextFormat;
};

inline void QTextImageFormat::setName(const QString &aname)
{ setProperty(ImageName, aname); }

inline void QTextImageFormat::setWidth(qreal awidth)
{ setProperty(ImageWidth, awidth); }

inline void QTextImageFormat::setHeight(qreal aheight)
{ setProperty(ImageHeight, aheight); }

class Q_GUI_EXPORT QTextFrameFormat : public QTextFormat
{
public:
    QTextFrameFormat();

    bool isValid() const { return isFrameFormat(); }

    enum Position {
        InFlow,
        FloatLeft,
        FloatRight
        // ######
//        Absolute
    };

    enum BorderStyle {
        BorderStyle_None,
        BorderStyle_Dotted,
        BorderStyle_Dashed,
        BorderStyle_Solid,
        BorderStyle_Double,
        BorderStyle_DotDash,
        BorderStyle_DotDotDash,
        BorderStyle_Groove,
        BorderStyle_Ridge,
        BorderStyle_Inset,
        BorderStyle_Outset
    };

    inline void setPosition(Position f)
    { setProperty(CssFloat, f); }
    inline Position position() const
    { return static_cast<Position>(intProperty(CssFloat)); }

    inline void setBorder(qreal border);
    inline qreal border() const
    { return doubleProperty(FrameBorder); }

    inline void setBorderBrush(const QBrush &brush)
    { setProperty(FrameBorderBrush, brush); }
    inline QBrush borderBrush() const
    { return brushProperty(FrameBorderBrush); }

    inline void setBorderStyle(BorderStyle style)
    { setProperty(FrameBorderStyle, style); }
    inline BorderStyle borderStyle() const
    { return static_cast<BorderStyle>(intProperty(FrameBorderStyle)); }

    void setMargin(qreal margin);
    inline qreal margin() const
    { return doubleProperty(FrameMargin); }

    inline void setTopMargin(qreal margin);
    qreal topMargin() const;

    inline void setBottomMargin(qreal margin);
    qreal bottomMargin() const;

    inline void setLeftMargin(qreal margin);
    qreal leftMargin() const;

    inline void setRightMargin(qreal margin);
    qreal rightMargin() const;

    inline void setPadding(qreal padding);
    inline qreal padding() const
    { return doubleProperty(FramePadding); }

    inline void setWidth(qreal width);
    inline void setWidth(const QTextLength &length)
    { setProperty(FrameWidth, length); }
    inline QTextLength width() const
    { return lengthProperty(FrameWidth); }

    inline void setHeight(qreal height);
    inline void setHeight(const QTextLength &height);
    inline QTextLength height() const
    { return lengthProperty(FrameHeight); }

    inline void setPageBreakPolicy(PageBreakFlags flags)
    { setProperty(PageBreakPolicy, int(flags)); }
    inline PageBreakFlags pageBreakPolicy() const
    { return PageBreakFlags(intProperty(PageBreakPolicy)); }

protected:
    explicit QTextFrameFormat(const QTextFormat &fmt);
    friend class QTextFormat;
};

inline void QTextFrameFormat::setBorder(qreal aborder)
{ setProperty(FrameBorder, aborder); }

inline void QTextFrameFormat::setPadding(qreal apadding)
{ setProperty(FramePadding, apadding); }

inline void QTextFrameFormat::setWidth(qreal awidth)
{ setProperty(FrameWidth, QTextLength(QTextLength::FixedLength, awidth)); }

inline void QTextFrameFormat::setHeight(qreal aheight)
{ setProperty(FrameHeight, QTextLength(QTextLength::FixedLength, aheight)); }
inline void QTextFrameFormat::setHeight(const QTextLength &aheight)
{ setProperty(FrameHeight, aheight); }

inline void QTextFrameFormat::setTopMargin(qreal amargin)
{ setProperty(FrameTopMargin, amargin); }

inline void QTextFrameFormat::setBottomMargin(qreal amargin)
{ setProperty(FrameBottomMargin, amargin); }

inline void QTextFrameFormat::setLeftMargin(qreal amargin)
{ setProperty(FrameLeftMargin, amargin); }

inline void QTextFrameFormat::setRightMargin(qreal amargin)
{ setProperty(FrameRightMargin, amargin); }

class Q_GUI_EXPORT QTextTableFormat : public QTextFrameFormat
{
public:
    QTextTableFormat();

    inline bool isValid() const { return isTableFormat(); }

    inline int columns() const
    { int cols = intProperty(TableColumns); if (cols == 0) cols = 1; return cols; }
    inline void setColumns(int columns);

    inline void setColumnWidthConstraints(const QVector<QTextLength> &constraints)
    { setProperty(TableColumnWidthConstraints, constraints); }

    inline QVector<QTextLength> columnWidthConstraints() const
    { return lengthVectorProperty(TableColumnWidthConstraints); }

    inline void clearColumnWidthConstraints()
    { clearProperty(TableColumnWidthConstraints); }

    inline qreal cellSpacing() const
    { return doubleProperty(TableCellSpacing); }
    inline void setCellSpacing(qreal spacing)
    { setProperty(TableCellSpacing, spacing); }

    inline qreal cellPadding() const
    { return doubleProperty(TableCellPadding); }
    inline void setCellPadding(qreal padding);

    inline void setAlignment(Qt::Alignment alignment);
    inline Qt::Alignment alignment() const
    { return QFlag(intProperty(BlockAlignment)); }

    inline void setHeaderRowCount(int count)
    { setProperty(TableHeaderRowCount, count); }
    inline int headerRowCount() const
    { return intProperty(TableHeaderRowCount); }

protected:
    explicit QTextTableFormat(const QTextFormat &fmt);
    friend class QTextFormat;
};

inline void QTextTableFormat::setColumns(int acolumns)
{
    if (acolumns == 1)
        acolumns = 0;
    setProperty(TableColumns, acolumns);
}

inline void QTextTableFormat::setCellPadding(qreal apadding)
{ setProperty(TableCellPadding, apadding); }

inline void QTextTableFormat::setAlignment(Qt::Alignment aalignment)
{ setProperty(BlockAlignment, int(aalignment)); }

class Q_GUI_EXPORT QTextTableCellFormat : public QTextCharFormat
{
public:
    QTextTableCellFormat();

    inline bool isValid() const { return isTableCellFormat(); }

    inline void setTopPadding(qreal padding);
    inline qreal topPadding() const;

    inline void setBottomPadding(qreal padding);
    inline qreal bottomPadding() const;

    inline void setLeftPadding(qreal padding);
    inline qreal leftPadding() const;

    inline void setRightPadding(qreal padding);
    inline qreal rightPadding() const;

    inline void setPadding(qreal padding);

protected:
    explicit QTextTableCellFormat(const QTextFormat &fmt);
    friend class QTextFormat;
};

inline void QTextTableCellFormat::setTopPadding(qreal padding)
{
    setProperty(TableCellTopPadding, padding);
}

inline qreal QTextTableCellFormat::topPadding() const
{
    return doubleProperty(TableCellTopPadding);
}

inline void QTextTableCellFormat::setBottomPadding(qreal padding)
{
    setProperty(TableCellBottomPadding, padding);
}

inline qreal QTextTableCellFormat::bottomPadding() const
{
    return doubleProperty(TableCellBottomPadding);
}

inline void QTextTableCellFormat::setLeftPadding(qreal padding)
{
    setProperty(TableCellLeftPadding, padding);
}

inline qreal QTextTableCellFormat::leftPadding() const
{
    return doubleProperty(TableCellLeftPadding);
}

inline void QTextTableCellFormat::setRightPadding(qreal padding)
{
    setProperty(TableCellRightPadding, padding);
}

inline qreal QTextTableCellFormat::rightPadding() const
{
    return doubleProperty(TableCellRightPadding);
}

inline void QTextTableCellFormat::setPadding(qreal padding)
{
    setTopPadding(padding);
    setBottomPadding(padding);
    setLeftPadding(padding);
    setRightPadding(padding);
}


QT_END_NAMESPACE

QT_END_HEADER

#endif // QTEXTFORMAT_H
