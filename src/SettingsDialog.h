#pragma once
#include <QDialog>
#include "ClockSettings.h"

class QComboBox;
class QSlider;
class QCheckBox;
class QPushButton;
class QLabel;

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(const ClockSettings& current, QWidget* parent = nullptr);
    ClockSettings settings() const { return m_settings; }

private slots:
    void pickLedOnColor();
    void pickLedOffColor();
    void pickBgColor();
    void onAccepted();

private:
    void updateColorButton(QPushButton* btn, const QColor& c);

    ClockSettings  m_settings;
    QComboBox*     m_styleCombo    = nullptr;
    QComboBox*     m_dateModeCombo = nullptr;
    QComboBox*     m_yearCombo     = nullptr;
    QSlider*       m_opacitySlider = nullptr;
    QSlider*       m_ledSizeSlider = nullptr;
    QCheckBox*     m_alwaysOnTop     = nullptr;
    QCheckBox*     m_decimalHint     = nullptr;
    QPushButton*   m_btnLedOn      = nullptr;
    QPushButton*   m_btnLedOff     = nullptr;
    QPushButton*   m_btnBg         = nullptr;
};
