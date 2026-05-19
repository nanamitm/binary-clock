#include "SettingsDialog.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QSlider>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QColorDialog>

SettingsDialog::SettingsDialog(const ClockSettings& current, QWidget* parent)
    : QDialog(parent), m_settings(current)
{
    setWindowTitle(tr("設定"));
    setModal(true);

    auto* form = new QFormLayout;

    // Style
    m_styleCombo = new QComboBox;
    m_styleCombo->addItem(tr("A: グリッド"), 0);
    m_styleCombo->addItem(tr("C: 円形"),     1);
    m_styleCombo->setCurrentIndex(m_settings.style == DisplayStyle::Grid ? 0 : 1);
    form->addRow(tr("スタイル:"), m_styleCombo);

    // Date display mode
    m_dateModeCombo = new QComboBox;
    m_dateModeCombo->addItem(tr("ホバー時"),     0);
    m_dateModeCombo->addItem(tr("常時"),         1);
    m_dateModeCombo->addItem(tr("クリック切替"), 2);
    m_dateModeCombo->setCurrentIndex(static_cast<int>(m_settings.dateMode));
    form->addRow(tr("日付表示:"), m_dateModeCombo);

    // Year mode
    m_yearCombo = new QComboBox;
    m_yearCombo->addItem(tr("西暦4桁"), 0);
    m_yearCombo->addItem(tr("下2桁"),   1);
    m_yearCombo->addItem(tr("非表示"),  2);
    m_yearCombo->setCurrentIndex(static_cast<int>(m_settings.yearMode));
    form->addRow(tr("年表示:"), m_yearCombo);

    // LED size
    m_ledSizeSlider = new QSlider(Qt::Horizontal);
    m_ledSizeSlider->setRange(8, 24);
    m_ledSizeSlider->setValue(m_settings.ledSize);
    form->addRow(tr("LEDサイズ:"), m_ledSizeSlider);

    // Opacity
    m_opacitySlider = new QSlider(Qt::Horizontal);
    m_opacitySlider->setRange(30, 100);
    m_opacitySlider->setValue(static_cast<int>(m_settings.opacity * 100));
    form->addRow(tr("不透明度:"), m_opacitySlider);

    // Always on top
    m_alwaysOnTop = new QCheckBox;
    m_alwaysOnTop->setChecked(m_settings.alwaysOnTop);
    form->addRow(tr("常に最前面:"), m_alwaysOnTop);

    // Decimal hint
    m_decimalHint = new QCheckBox;
    m_decimalHint->setChecked(m_settings.showDecimalHint);
    form->addRow(tr("10進数ヒント:"), m_decimalHint);

    // Colors
    auto* colorRow = new QHBoxLayout;
    m_btnLedOn  = new QPushButton(tr("ON色"));
    m_btnLedOff = new QPushButton(tr("OFF色"));
    m_btnBg     = new QPushButton(tr("背景色"));
    updateColorButton(m_btnLedOn,  m_settings.ledOnColor);
    updateColorButton(m_btnLedOff, m_settings.ledOffColor);
    updateColorButton(m_btnBg,     m_settings.bgColor);
    colorRow->addWidget(m_btnLedOn);
    colorRow->addWidget(m_btnLedOff);
    colorRow->addWidget(m_btnBg);
    form->addRow(tr("LED色:"), colorRow);

    connect(m_btnLedOn,  &QPushButton::clicked, this, &SettingsDialog::pickLedOnColor);
    connect(m_btnLedOff, &QPushButton::clicked, this, &SettingsDialog::pickLedOffColor);
    connect(m_btnBg,     &QPushButton::clicked, this, &SettingsDialog::pickBgColor);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &SettingsDialog::onAccepted);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto* main = new QVBoxLayout(this);
    main->addLayout(form);
    main->addWidget(buttons);
}

void SettingsDialog::updateColorButton(QPushButton* btn, const QColor& c)
{
    btn->setStyleSheet(QString("background-color: %1; color: %2;")
        .arg(c.name())
        .arg(c.lightness() > 128 ? "black" : "white"));
}

void SettingsDialog::pickLedOnColor()
{
    QColor c = QColorDialog::getColor(m_settings.ledOnColor, this, tr("ON色を選択"));
    if (c.isValid()) { m_settings.ledOnColor = c; updateColorButton(m_btnLedOn, c); }
}

void SettingsDialog::pickLedOffColor()
{
    QColor c = QColorDialog::getColor(m_settings.ledOffColor, this, tr("OFF色を選択"));
    if (c.isValid()) { m_settings.ledOffColor = c; updateColorButton(m_btnLedOff, c); }
}

void SettingsDialog::pickBgColor()
{
    QColor c = QColorDialog::getColor(m_settings.bgColor, this, tr("背景色を選択"),
                                       QColorDialog::ShowAlphaChannel);
    if (c.isValid()) { m_settings.bgColor = c; updateColorButton(m_btnBg, c); }
}

void SettingsDialog::onAccepted()
{
    m_settings.style    = m_styleCombo->currentIndex() == 0
                          ? DisplayStyle::Grid : DisplayStyle::Circle;
    m_settings.dateMode = static_cast<DateDisplayMode>(m_dateModeCombo->currentIndex());
    m_settings.yearMode = static_cast<YearDisplayMode>(m_yearCombo->currentIndex());
    m_settings.ledSize  = m_ledSizeSlider->value();
    m_settings.opacity  = m_opacitySlider->value() / 100.0;
    m_settings.alwaysOnTop     = m_alwaysOnTop->isChecked();
    m_settings.showDecimalHint = m_decimalHint->isChecked();
    accept();
}
