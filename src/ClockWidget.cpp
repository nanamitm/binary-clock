#include "ClockWidget.h"
#include "GridStyleWidget.h"
#include "CircleStyleWidget.h"
#include "SettingsDialog.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QTimer>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QApplication>
#include <QSettings>
#include <QPainter>
#include <QPainterPath>
#include <QCloseEvent>

ClockWidget::ClockWidget(QWidget* parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    m_grid   = new GridStyleWidget(this);
    m_circle = new CircleStyleWidget(this);

    m_stack = new QStackedWidget(this);
    m_stack->addWidget(m_grid);
    m_stack->addWidget(m_circle);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_stack);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &ClockWidget::onTick);
    m_timer->start(200);

    loadPosition();
    applySettings(m_settings);
    onTick();
}

void ClockWidget::applySettings(const ClockSettings& s)
{
    m_settings = s;

    Qt::WindowFlags flags = Qt::FramelessWindowHint | Qt::Tool;
    if (s.alwaysOnTop) flags |= Qt::WindowStaysOnTopHint;
    setWindowFlags(flags);
    setWindowOpacity(s.opacity);

    m_grid->applySettings(s);
    m_circle->applySettings(s);

    m_stack->setCurrentIndex(s.style == DisplayStyle::Grid ? 0 : 1);
    updateChildVisibility();
    show();
}

void ClockWidget::updateChildVisibility()
{
    bool showDate = m_showDate;
    if (m_settings.dateMode == DateDisplayMode::Always) showDate = true;

    m_grid->setShowDate(showDate);
    m_circle->setShowDate(showDate);

    adjustSize();
    update();
}

void ClockWidget::onTick()
{
    m_now = QDateTime::currentDateTime();
    m_grid->setDateTime(m_now);
    m_circle->setDateTime(m_now);
}

void ClockWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);
    p.fillPath(path, m_settings.bgColor);
}

void ClockWidget::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton) {
        if (m_settings.dateMode == DateDisplayMode::OnClick) {
            m_showDate = !m_showDate;
            updateChildVisibility();
        }
        m_dragging   = true;
        m_dragOffset = e->globalPosition().toPoint() - frameGeometry().topLeft();
    }
}

void ClockWidget::mouseMoveEvent(QMouseEvent* e)
{
    if (m_dragging)
        move(e->globalPosition().toPoint() - m_dragOffset);
}

void ClockWidget::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton) {
        m_dragging = false;
        savePosition();
    }
}

void ClockWidget::enterEvent(QEnterEvent*)
{
    if (m_settings.dateMode == DateDisplayMode::OnHover) {
        m_showDate = true;
        updateChildVisibility();
    }
}

void ClockWidget::leaveEvent(QEvent*)
{
    if (m_settings.dateMode == DateDisplayMode::OnHover) {
        m_showDate = false;
        updateChildVisibility();
    }
}

void ClockWidget::contextMenuEvent(QContextMenuEvent* e)
{
    QMenu menu(this);

    auto* styleMenu = menu.addMenu(tr("スタイル"));
    auto* actGrid   = styleMenu->addAction(tr("A: グリッド"));
    auto* actCircle = styleMenu->addAction(tr("B: ライン"));
    actGrid->setCheckable(true);
    actCircle->setCheckable(true);
    actGrid->setChecked(m_settings.style == DisplayStyle::Grid);
    actCircle->setChecked(m_settings.style == DisplayStyle::Circle);

    auto* dateMenu   = menu.addMenu(tr("日付表示"));
    auto* actHover   = dateMenu->addAction(tr("ホバー時"));
    auto* actAlways  = dateMenu->addAction(tr("常時"));
    auto* actOnClick = dateMenu->addAction(tr("クリック切替"));
    actHover->setCheckable(true);
    actAlways->setCheckable(true);
    actOnClick->setCheckable(true);
    actHover->setChecked(m_settings.dateMode == DateDisplayMode::OnHover);
    actAlways->setChecked(m_settings.dateMode == DateDisplayMode::Always);
    actOnClick->setChecked(m_settings.dateMode == DateDisplayMode::OnClick);

    auto* actDecimal = menu.addAction(tr("10進数ヒント表示"));
    actDecimal->setCheckable(true);
    actDecimal->setChecked(m_settings.showDecimalHint);

    menu.addSeparator();
    auto* actSettings = menu.addAction(tr("詳細設定..."));
    menu.addSeparator();
    auto* actQuit = menu.addAction(tr("終了"));

    QAction* chosen = menu.exec(e->globalPos());
    if (!chosen) return;

    if (chosen == actGrid)    { m_settings.style = DisplayStyle::Grid;                       applySettings(m_settings); savePosition(); }
    if (chosen == actCircle)  { m_settings.style = DisplayStyle::Circle;                     applySettings(m_settings); savePosition(); }
    if (chosen == actHover)   { m_settings.dateMode = DateDisplayMode::OnHover;  m_showDate = false; updateChildVisibility(); savePosition(); }
    if (chosen == actAlways)  { m_settings.dateMode = DateDisplayMode::Always;   m_showDate = false; updateChildVisibility(); savePosition(); }
    if (chosen == actOnClick) { m_settings.dateMode = DateDisplayMode::OnClick;  m_showDate = false; updateChildVisibility(); savePosition(); }
    if (chosen == actDecimal) { m_settings.showDecimalHint = !m_settings.showDecimalHint;   applySettings(m_settings); savePosition(); }
    if (chosen == actSettings) openSettings();
    if (chosen == actQuit) quit();
}

void ClockWidget::openSettings()
{
    SettingsDialog dlg(m_settings, this);
    if (dlg.exec() == QDialog::Accepted) {
        applySettings(dlg.settings());
        savePosition();
    }
}

void ClockWidget::quit()
{
    close();
}

void ClockWidget::closeEvent(QCloseEvent* e)
{
    savePosition();
    e->accept();
}

void ClockWidget::savePosition()
{
    QSettings qs("BinaryClock", "BinaryClock");
    qs.setValue("pos", pos());

    qs.setValue("style",           static_cast<int>(m_settings.style));
    qs.setValue("dateMode",        static_cast<int>(m_settings.dateMode));
    qs.setValue("yearMode",        static_cast<int>(m_settings.yearMode));
    qs.setValue("ledOnColor",      m_settings.ledOnColor.name(QColor::HexArgb));
    qs.setValue("ledOffColor",     m_settings.ledOffColor.name(QColor::HexArgb));
    qs.setValue("bgColor",         m_settings.bgColor.name(QColor::HexArgb));
    qs.setValue("ledSize",         m_settings.ledSize);
    qs.setValue("opacity",         m_settings.opacity);
    qs.setValue("alwaysOnTop",     m_settings.alwaysOnTop);
    qs.setValue("showDecimalHint", m_settings.showDecimalHint);
}

void ClockWidget::loadPosition()
{
    QSettings qs("BinaryClock", "BinaryClock");

    QPoint p = qs.value("pos", QPoint(-1, -1)).toPoint();
    if (p != QPoint(-1, -1)) move(p);

    if (!qs.contains("style")) return;

    m_settings.style    = static_cast<DisplayStyle>(qs.value("style").toInt());
    m_settings.dateMode = static_cast<DateDisplayMode>(qs.value("dateMode").toInt());
    m_settings.yearMode = static_cast<YearDisplayMode>(qs.value("yearMode").toInt());
    m_settings.ledOnColor  = QColor(qs.value("ledOnColor").toString());
    m_settings.ledOffColor = QColor(qs.value("ledOffColor").toString());
    m_settings.bgColor     = QColor(qs.value("bgColor").toString());
    m_settings.ledSize     = qs.value("ledSize").toInt();
    m_settings.opacity     = qs.value("opacity").toDouble();
    m_settings.alwaysOnTop     = qs.value("alwaysOnTop").toBool();
    m_settings.showDecimalHint = qs.value("showDecimalHint").toBool();
}
