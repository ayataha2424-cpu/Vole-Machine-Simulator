// GUI layer for the Vole machine simulator (Bonus Task 3).
// Written with AI assistance (allowed for the GUI). The core files are NOT modified.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <cstdint>
#include <string>

#include "CPU.h"
#include "Memory.h"
#include "ProgramLoader.h"

class QComboBox;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QPushButton;
class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onLoad();
    void onStep();
    void onRun();
    void onReset();
    void onHelp();
    void onSetRegister();
    void onSetMemory();

private:
    void buildUi();
    void snapshot();       // remember the current state (to detect changes later)
    void refreshViews();   // redraw everything, highlighting what changed
    void updateButtons();
    void setStatus(const QString &text, bool error = false);
    QString haltMessage() const;
    bool parseHex(const QString &text, int minValue, int maxValue, int &out) const;

    // ---- Core objects (existing classes, used as-is) ----
    Memory mem;
    CPU cpu;
    ProgramLoader loader;

    // ---- State ----
    bool loaded = false;
    int startAddr = 0;
    std::string prevRegs[16];
    uint8_t prevMem[256] = {0};

    // ---- Widgets ----
    QLineEdit *startEdit = nullptr;
    QPushButton *loadBtn = nullptr;
    QPushButton *stepBtn = nullptr;
    QPushButton *runBtn = nullptr;
    QPushButton *resetBtn = nullptr;
    QPushButton *helpBtn = nullptr;
    QLabel *fileLabel = nullptr;

    QLabel *pcLabel = nullptr;
    QLabel *irLabel = nullptr;
    QLabel *stateLabel = nullptr;
    QTableWidget *regTable = nullptr;
    QTableWidget *memTable = nullptr;
    QPlainTextEdit *screenView = nullptr;

    QComboBox *regCombo = nullptr;
    QLineEdit *regValEdit = nullptr;
    QPushButton *setRegBtn = nullptr;
    QLineEdit *memAddrEdit = nullptr;
    QLineEdit *memValEdit = nullptr;
    QPushButton *setMemBtn = nullptr;

    QLabel *statusLabel = nullptr;
};

#endif // MAINWINDOW_H
