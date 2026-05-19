#pragma once
#include <QWidget>
#include <QDateTime>
#include "ClockSettings.h"

class CircleStyleWidget : public QWidget {
    Q_OBJECT
public:
    explicit CircleStyleWidget(QWidget* parent = nullptr);

    void applySettings(const ClockSettings& s);
    void setDateTime(const QDateTime& dt);
    void setShowDate(bool show);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    struct Row {
        int value;
        int bits;
        QString label;
    };

    QList<Row> buildRows() const;
    QSize calcSize() const;
    void drawRow(QPainter& p, int y, const Row& row, int maxBits) const;

    ClockSettings m_settings;
    QDateTime     m_dt;
    bool          m_showDate = false;
};
