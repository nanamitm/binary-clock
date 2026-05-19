#include "CircleStyleWidget.h"
#include <QPainter>
#include <QDate>
#include <QTime>

static int bitsNeeded(int maxVal)
{
    int b = 0;
    while ((1 << b) <= maxVal) b++;
    return b;
}

CircleStyleWidget::CircleStyleWidget(QWidget* parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void CircleStyleWidget::applySettings(const ClockSettings& s)
{
    m_settings = s;
    setFixedSize(calcSize());
    update();
}

void CircleStyleWidget::setDateTime(const QDateTime& dt)
{
    m_dt = dt;
    update();
}

void CircleStyleWidget::setShowDate(bool show)
{
    m_showDate = show;
    setFixedSize(calcSize());
    update();
}

QList<CircleStyleWidget::Row> CircleStyleWidget::buildRows() const
{
    QList<Row> rows;
    QDate d = m_dt.date();
    QTime t = m_dt.time();

    if (m_showDate) {
        if (m_settings.yearMode == YearDisplayMode::Full4Digit) {
            rows << Row{d.year(), bitsNeeded(9999), "YEAR"};
        } else if (m_settings.yearMode == YearDisplayMode::Last2Digit) {
            rows << Row{d.year() % 100, bitsNeeded(99), "YY"};
        }
        rows << Row{d.month(), bitsNeeded(12), "MON"};
        rows << Row{d.day(),   bitsNeeded(31), "DAY"};
    }

    rows << Row{t.hour(),   bitsNeeded(23), "HR"};
    rows << Row{t.minute(), bitsNeeded(59), "MIN"};
    rows << Row{t.second(), bitsNeeded(59), "SEC"};

    return rows;
}

QSize CircleStyleWidget::calcSize() const
{
    QList<Row> rows = buildRows();
    if (rows.isEmpty()) return QSize(100, 60);

    const int s   = m_settings.ledSize;
    const int sp  = m_settings.ledSpacing;
    const int labelW = 36;
    const int rowH   = s + sp;

    int maxBits = 0;
    for (const auto& r : rows) maxBits = qMax(maxBits, r.bits);

    int hintW = m_settings.showDecimalHint ? (sp + 28) : 0;
    int w = sp + labelW + sp + maxBits * (s + sp) + hintW;
    int h = sp + rows.size() * rowH + sp;

    // Extra space for date/time separator (any date row present)
    if (m_showDate) h += sp * 2 + 1;

    return QSize(w, h);
}

void CircleStyleWidget::drawRow(QPainter& p, int y, const Row& row, int maxBits) const
{
    const int s   = m_settings.ledSize;
    const int sp  = m_settings.ledSpacing;
    const int labelW = 36;

    // Label
    p.setPen(QColor(160, 160, 160));
    p.setFont(QFont("monospace", 8));
    p.drawText(sp, y, labelW, s, Qt::AlignVCenter | Qt::AlignRight, row.label);

    int x = sp + labelW + sp;
    // Draw MSB first (left side)
    for (int bit = row.bits - 1; bit >= 0; bit--) {
        bool active = (row.value >> bit) & 1;
        p.setBrush(active ? m_settings.ledOnColor : m_settings.ledOffColor);
        p.setPen(Qt::NoPen);
        p.drawEllipse(x, y, s, s);
        x += s + sp;
    }
    // Fill remaining columns with dim placeholder
    int extra = maxBits - row.bits;
    for (int i = 0; i < extra; i++) {
        p.setBrush(QColor(30, 30, 30));
        p.setPen(Qt::NoPen);
        p.drawEllipse(x, y, s, s);
        x += s + sp;
    }

    // Decimal hint at end of row
    if (m_settings.showDecimalHint) {
        p.setPen(m_settings.ledOnColor);
        p.setFont(QFont("monospace", 8, QFont::Bold));
        p.drawText(x, y, 28, s, Qt::AlignVCenter | Qt::AlignLeft,
                   QString::number(row.value));
    }
}

void CircleStyleWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QList<Row> rows = buildRows();
    const int s  = m_settings.ledSize;
    const int sp = m_settings.ledSpacing;

    int maxBits = 0;
    for (const auto& r : rows) maxBits = qMax(maxBits, r.bits);

    // How many date rows
    int dateRowCount = 0;
    if (m_showDate) {
        if (m_settings.yearMode != YearDisplayMode::Hidden) dateRowCount++;
        dateRowCount += 2; // month + day
    }

    int y = sp;
    for (int i = 0; i < rows.size(); i++) {
        // Separator between date and time
        if (m_showDate && i == dateRowCount) {
            int sepY = y - sp/2;
            p.setPen(QPen(QColor(80, 80, 80), 1));
            p.drawLine(4, sepY, width() - 4, sepY);
            y += sp * 2;
        }
        drawRow(p, y, rows[i], maxBits);
        y += s + sp;
    }
}
