#pragma once
#include <QColor>
#include <QPoint>

enum class DisplayStyle { Grid, Circle };
enum class DateDisplayMode { OnHover, Always, OnClick };
enum class YearDisplayMode { Full4Digit, Last2Digit, Hidden };

struct ClockSettings {
    DisplayStyle style = DisplayStyle::Grid;
    DateDisplayMode dateMode = DateDisplayMode::OnHover;
    YearDisplayMode yearMode = YearDisplayMode::Last2Digit;

    QColor ledOnColor  = QColor(0, 220, 120);
    QColor ledOffColor = QColor(40, 40, 40);
    QColor bgColor     = QColor(20, 20, 20, 200);

    int ledSize     = 14;
    int ledSpacing  = 4;
    double opacity  = 0.9;
    bool alwaysOnTop = true;
    bool showDecimalHint = false;

    QPoint windowPos = QPoint(-1, -1);
};
