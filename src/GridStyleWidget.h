#pragma once
#include <QWidget>
#include <QDateTime>
#include "ClockSettings.h"

class GridStyleWidget : public QWidget {
    Q_OBJECT
public:
    explicit GridStyleWidget(QWidget* parent = nullptr);

    void applySettings(const ClockSettings& s);
    void setDateTime(const QDateTime& dt);
    void setShowDate(bool show);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    struct Column {
        int value;
        int maxBits;
        QString label;
    };

    QList<Column> buildColumns() const;
    QSize calcSize() const;

    ClockSettings m_settings;
    QDateTime     m_dt;
    bool          m_showDate = false;
};
