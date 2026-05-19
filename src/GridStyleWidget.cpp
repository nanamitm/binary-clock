#include "GridStyleWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QDate>
#include <QTime>

GridStyleWidget::GridStyleWidget(QWidget* parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void GridStyleWidget::applySettings(const ClockSettings& s)
{
    m_settings = s;
    setFixedSize(calcSize());
    update();
}

void GridStyleWidget::setDateTime(const QDateTime& dt)
{
    m_dt = dt;
    update();
}

void GridStyleWidget::setShowDate(bool show)
{
    m_showDate = show;
    setFixedSize(calcSize());
    update();
}

// Returns BCD columns: tens digit then units digit for each field
QList<GridStyleWidget::Column> GridStyleWidget::buildColumns() const
{
    QList<Column> cols;
    QDate d = m_dt.date();
    QTime t = m_dt.time();

    if (m_showDate) {
        int year = m_settings.yearMode == YearDisplayMode::Full4Digit
                   ? d.year() : d.year() % 100;

        if (m_settings.yearMode == YearDisplayMode::Full4Digit) {
            cols << Column{year / 1000,       2, "Y"};
            cols << Column{(year / 100) % 10, 4, "Y"};
        }
        if (m_settings.yearMode != YearDisplayMode::Hidden) {
            cols << Column{(year / 10) % 10, 4, "Y"};
            cols << Column{year % 10,        4, "Y"};
        }
        cols << Column{d.month() / 10, 2, "M"};
        cols << Column{d.month() % 10, 4, "M"};
        cols << Column{d.day() / 10,   2, "D"};
        cols << Column{d.day() % 10,   4, "D"};
    }

    cols << Column{t.hour() / 10,   2, "H"};
    cols << Column{t.hour() % 10,   4, "H"};
    cols << Column{t.minute() / 10, 3, "M"};
    cols << Column{t.minute() % 10, 4, "M"};
    cols << Column{t.second() / 10, 3, "S"};
    cols << Column{t.second() % 10, 4, "S"};

    return cols;
}

QSize GridStyleWidget::calcSize() const
{
    const int s  = m_settings.ledSize;
    const int sp = m_settings.ledSpacing;
    const int rows = 4;
    const int labelH = 16;

    QList<Column> cols = buildColumns();

    // Group separator: add gap between date and time sections
    int dateCols = 0;
    if (m_showDate && m_settings.yearMode != YearDisplayMode::Hidden)
        dateCols = (m_settings.yearMode == YearDisplayMode::Full4Digit ? 4 : 2) + 4;
    else if (m_showDate)
        dateCols = 4;

    int separatorW = (m_showDate && dateCols > 0) ? (sp * 2 + 1) : 0;

    int hintH = m_settings.showDecimalHint ? (sp + 14) : 0;
    int w = sp + cols.size() * (s + sp) + separatorW;
    int h = labelH + sp + rows * (s + sp) + hintH;
    return QSize(w, h);
}

void GridStyleWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const int s      = m_settings.ledSize;
    const int sp     = m_settings.ledSpacing;
    const int rows   = 4;
    const int labelH = 16;

    QList<Column> cols = buildColumns();

    // Figure out how many date columns there are to draw a separator
    int dateColCount = 0;
    if (m_showDate) {
        if (m_settings.yearMode == YearDisplayMode::Full4Digit) dateColCount += 4;
        else if (m_settings.yearMode == YearDisplayMode::Last2Digit) dateColCount += 2;
        dateColCount += 4; // month + day
    }

    int x = sp;
    for (int ci = 0; ci < cols.size(); ci++) {
        // Draw separator between date and time sections
        if (m_showDate && ci == dateColCount) {
            int sepX = x - sp/2;
            p.setPen(QPen(QColor(80, 80, 80), 1));
            p.drawLine(sepX, 2, sepX, height() - 2);
            x += sp;
        }

        const Column& col = cols[ci];

        // Label
        p.setPen(QColor(160, 160, 160));
        p.setFont(QFont("monospace", 8));
        p.drawText(x, 0, s, labelH, Qt::AlignCenter, col.label);

        // Bits (MSB at top)
        for (int bit = 0; bit < rows; bit++) {
            int bitIndex = rows - 1 - bit; // row 0 = MSB (bit 3)
            bool active = (col.value >> bitIndex) & 1;

            // Dim unneeded high bits
            bool needed = bitIndex < col.maxBits;
            QColor color = active
                           ? m_settings.ledOnColor
                           : (needed ? m_settings.ledOffColor : QColor(30, 30, 30));

            int y = labelH + sp + bit * (s + sp);
            p.setBrush(color);
            p.setPen(Qt::NoPen);
            p.drawEllipse(x, y, s, s);
        }

        // Decimal hint below LEDs
        if (m_settings.showDecimalHint) {
            int hy = labelH + sp + rows * (s + sp);
            p.setPen(m_settings.ledOnColor);
            p.setFont(QFont("monospace", 8, QFont::Bold));
            p.drawText(x, hy, s, 14, Qt::AlignCenter, QString::number(col.value));
        }

        x += s + sp;
    }
}
