#pragma once
#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QPoint>
#include "ClockSettings.h"

class GridStyleWidget;
class CircleStyleWidget;
class QStackedWidget;

class ClockWidget : public QWidget {
    Q_OBJECT
public:
    explicit ClockWidget(QWidget* parent = nullptr);

    void applySettings(const ClockSettings& s);
    ClockSettings currentSettings() const { return m_settings; }

protected:
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void contextMenuEvent(QContextMenuEvent* e) override;
    void enterEvent(QEnterEvent* e) override;
    void leaveEvent(QEvent* e) override;
    void paintEvent(QPaintEvent* e) override;

private slots:
    void onTick();
    void openSettings();
    void quit();

private:
    void updateChildVisibility();
    void savePosition();
    void loadPosition();

    ClockSettings       m_settings;
    GridStyleWidget*    m_grid   = nullptr;
    CircleStyleWidget*  m_circle = nullptr;
    QStackedWidget*     m_stack  = nullptr;
    QTimer*             m_timer  = nullptr;

    QDateTime m_now;
    bool      m_showDate   = false;
    bool      m_dragging   = false;
    bool      m_dateByClick = false;
    QPoint    m_dragOffset;
};
