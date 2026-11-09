// Visual themes for the Vole GUI: Dark (modern), Light (clean) and Retro (green terminal).
// Each theme is just a palette; buildQss() turns a palette into a full Qt stylesheet.

#ifndef THEMES_H
#define THEMES_H

#include <QString>
#include <QStringList>
#include <utility>
#include <vector>

struct Theme {
    QString name;

    // base colors
    QString bg, panel, inputBg, border, grid;
    QString text, textDim, accent, accentText;

    // colored buttons
    QString stepBg, runBg, resetBg, btnText;

    // table cell highlighting
    QString changedBg, changedFg;  // changed by the last action
    QString pcBg, pcFg;            // cells at the PC
    QString zeroFg, nonzeroFg;     // "00" cells are dimmed, others stand out

    // status line and screen
    QString ok, err;
    QString screenBg, screenFg;

    // fonts and shape
    QString font, mono;
    int radius = 6;
};

inline Theme makeDarkTheme()
{
    Theme t;
    t.name = "Dark";
    t.bg = "#12151b";      t.panel = "#1a1f27";   t.inputBg = "#0e1116";
    t.border = "#2b3340";  t.grid = "#232a35";
    t.text = "#e6e9ef";    t.textDim = "#5d6878";
    t.accent = "#4c9aff";  t.accentText = "#0b1220";
    t.stepBg = "#2563eb";  t.runBg = "#16a34a";   t.resetBg = "#dc2626";
    t.btnText = "#ffffff";
    t.changedBg = "#f5c542"; t.changedFg = "#000000";
    t.pcBg = "#3b82f6";      t.pcFg = "#ffffff";
    t.zeroFg = "#4a5363";    t.nonzeroFg = "#9cc4ff";
    t.ok = "#4ade80";        t.err = "#f87171";
    t.screenBg = "#05080c";  t.screenFg = "#4ade80";
    t.font = "\"Segoe UI\"";
    t.mono = "Consolas";
    t.radius = 6;
    return t;
}

inline Theme makeLightTheme()
{
    Theme t;
    t.name = "Light";
    t.bg = "#f4f6fa";      t.panel = "#ffffff";   t.inputBg = "#ffffff";
    t.border = "#cfd6e2";  t.grid = "#e3e8f0";
    t.text = "#1f2937";    t.textDim = "#9aa5b4";
    t.accent = "#2563eb";  t.accentText = "#ffffff";
    t.stepBg = "#2563eb";  t.runBg = "#16a34a";   t.resetBg = "#dc2626";
    t.btnText = "#ffffff";
    t.changedBg = "#ffe27a"; t.changedFg = "#000000";
    t.pcBg = "#bcd7ff";      t.pcFg = "#000000";
    t.zeroFg = "#b5bdc9";    t.nonzeroFg = "#111827";
    t.ok = "#15803d";        t.err = "#b91c1c";
    t.screenBg = "#101820";  t.screenFg = "#4ade80";
    t.font = "\"Segoe UI\"";
    t.mono = "Consolas";
    t.radius = 6;
    return t;
}

inline Theme makeRetroTheme()
{
    Theme t;
    t.name = "Retro";
    t.bg = "#000000";      t.panel = "#040904";   t.inputBg = "#000000";
    t.border = "#1a7f1a";  t.grid = "#0b3b0b";
    t.text = "#33ff33";    t.textDim = "#0f6b0f";
    t.accent = "#33ff33";  t.accentText = "#000000";
    t.stepBg = "#1fbf1f";  t.runBg = "#33ff33";   t.resetBg = "#ffb000";
    t.btnText = "#000000";
    t.changedBg = "#ffb000"; t.changedFg = "#000000";
    t.pcBg = "#33ff33";      t.pcFg = "#000000";
    t.zeroFg = "#0f5f0f";    t.nonzeroFg = "#7dff7d";
    t.ok = "#33ff33";        t.err = "#ff5050";
    t.screenBg = "#000000";  t.screenFg = "#33ff33";
    t.font = "Consolas";
    t.mono = "Consolas";
    t.radius = 0;
    return t;
}

inline const std::vector<Theme> &allThemes()
{
    static const std::vector<Theme> themes = {makeDarkTheme(), makeLightTheme(), makeRetroTheme()};
    return themes;
}

// Returns the theme with this name (the first theme if the name is unknown).
inline const Theme &themeByName(const QString &name)
{
    for (const Theme &t : allThemes())
        if (t.name == name)
            return t;
    return allThemes().front();
}

inline QString buildQss(const Theme &t)
{
    QString q = QString::fromUtf8(R"QSS(
QWidget { background-color: @bg@; color: @text@; font-family: @font@; font-size: 10pt; }
QLabel { background: transparent; }
QToolTip { background-color: @panel@; color: @text@; border: 1px solid @accent@; }

QMenuBar { background-color: @panel@; color: @text@; border-bottom: 1px solid @border@; }
QMenuBar::item { background: transparent; padding: 5px 12px; }
QMenuBar::item:selected { background-color: @accent@; color: @accentText@; }
QMenu { background-color: @panel@; color: @text@; border: 1px solid @border@; padding: 4px; }
QMenu::item { padding: 5px 26px; }
QMenu::item:selected { background-color: @accent@; color: @accentText@; }

QGroupBox {
    background-color: @panel@;
    border: 1px solid @border@;
    border-radius: @r@;
    margin-top: 12px;
    padding: 6px 6px 4px 6px;
    font-weight: bold;
}
QGroupBox::title {
    subcontrol-origin: margin;
    left: 12px;
    padding: 0 6px;
    color: @accent@;
}
QGroupBox QLabel { background: transparent; font-weight: normal; }

QLineEdit, QComboBox {
    background-color: @inputBg@;
    border: 1px solid @border@;
    border-radius: @r@;
    padding: 4px 8px;
    selection-background-color: @accent@;
    selection-color: @accentText@;
}
QLineEdit:focus, QComboBox:focus { border: 1px solid @accent@; }
QComboBox QAbstractItemView {
    background-color: @panel@;
    color: @text@;
    border: 1px solid @border@;
    selection-background-color: @accent@;
    selection-color: @accentText@;
}
QLineEdit, QPlainTextEdit, QTableWidget,
QLabel#pcLabel, QLabel#irLabel, QLabel#stateLabel { font-family: @mono@; }

QPushButton {
    background-color: @inputBg@;
    color: @text@;
    border: 1px solid @border@;
    border-radius: @r@;
    padding: 6px 16px;
}
QPushButton:hover { border: 1px solid @accent@; }
QPushButton:pressed { background-color: @accent@; color: @accentText@; }
QPushButton:disabled { background-color: @panel@; color: @textDim@; border: 1px solid @border@; }

QPushButton#stepBtn  { background-color: @stepBg@;  color: @btnText@; border: 1px solid @stepBg@;  font-weight: bold; }
QPushButton#runBtn   { background-color: @runBg@;   color: @btnText@; border: 1px solid @runBg@;   font-weight: bold; }
QPushButton#resetBtn { background-color: @resetBg@; color: @btnText@; border: 1px solid @resetBg@; font-weight: bold; }
QPushButton#stepBtn:hover, QPushButton#runBtn:hover, QPushButton#resetBtn:hover { border: 1px solid @text@; }
QPushButton#stepBtn:disabled, QPushButton#runBtn:disabled, QPushButton#resetBtn:disabled {
    background-color: @panel@; color: @textDim@; border: 1px solid @border@; font-weight: normal;
}

QTableWidget {
    background-color: @inputBg@;
    color: @text@;
    gridline-color: @grid@;
    border: 1px solid @border@;
}
QTableWidget::item { padding: 0px; }
QHeaderView { background-color: @inputBg@; }
QHeaderView::section {
    background-color: @panel@;
    color: @accent@;
    border: none;
    border-right: 1px solid @grid@;
    border-bottom: 1px solid @grid@;
    padding: 2px;
    font-weight: bold;
}
QTableCornerButton::section { background-color: @panel@; border: none; }

QPlainTextEdit#screenView {
    background-color: @screenBg@;
    color: @screenFg@;
    font-size: 15pt;
    border: 1px solid @accent@;
    border-radius: @r@;
    padding: 6px;
    selection-background-color: @screenFg@;
    selection-color: @screenBg@;
}
)QSS");

    const std::vector<std::pair<QString, QString>> tokens = {
        {"@bg@", t.bg},           {"@panel@", t.panel},     {"@inputBg@", t.inputBg},
        {"@border@", t.border},   {"@grid@", t.grid},       {"@text@", t.text},
        {"@textDim@", t.textDim}, {"@accent@", t.accent},   {"@accentText@", t.accentText},
        {"@stepBg@", t.stepBg},   {"@runBg@", t.runBg},     {"@resetBg@", t.resetBg},
        {"@btnText@", t.btnText}, {"@screenBg@", t.screenBg}, {"@screenFg@", t.screenFg},
        {"@font@", t.font},       {"@mono@", t.mono},
        {"@r@", QString::number(t.radius) + "px"}};
    for (const auto &kv : tokens)
        q.replace(kv.first, kv.second);
    return q;
}

#endif // THEMES_H