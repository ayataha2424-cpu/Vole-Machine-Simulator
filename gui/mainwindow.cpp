// GUI layer for the Vole machine simulator (Bonus Task 3).
// Written with AI assistance (allowed for the GUI). The core files are NOT modified.

#include "mainwindow.h"
#include "themes.h"

#include <QAbstractItemView>
#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QBrush>
#include <QColor>
#include <QComboBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QFontDatabase>
#include <QGridLayout>
#include <QGuiApplication>
#include <QScreen>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSettings>
#include <QStringList>
#include <QStyleFactory>
#include <QTableWidget>
#include <QVBoxLayout>

namespace {
// Two uppercase hex digits for a byte value.
QString hex2(int v)
{
    return QString("%1").arg(v & 0xFF, 2, 16, QChar('0')).toUpper();
}
} // namespace

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Vole Machine Simulator");
    QApplication::setStyle(QStyleFactory::create("Fusion")); // consistent look for all themes
    // Size the window to fit the screen (so nothing is cut off on small / scaled displays).
    if (QScreen *scr = QGuiApplication::primaryScreen()) {
        const QRect avail = scr->availableGeometry();
        resize(qMin(1180, int(avail.width() * 0.95)), qMin(780, int(avail.height() * 0.92)));
    } else {
        resize(1180, 700);
    }

    buildUi();
    snapshot();
    buildThemeMenu(); // applies the saved theme and redraws the views
    updateButtons();
    setStatus("Ready. Enter a start address, then press 'Load Program...'.");
}

void MainWindow::buildUi()
{
    const QFont mono = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(12, 8, 12, 10);
    mainLayout->setSpacing(6);

    // ================= Program control (top) =================
    QGroupBox *ctrlBox = new QGroupBox("Program Control", central);
    QHBoxLayout *ctrl = new QHBoxLayout(ctrlBox);
    ctrl->setSpacing(8);

    startEdit = new QLineEdit("00", ctrlBox);
    startEdit->setMaxLength(2);
    startEdit->setFixedWidth(54);
    startEdit->setFont(mono);
    startEdit->setToolTip("Start address in hex. Must be even (00, 02, ..., FE).");

    loadBtn = new QPushButton("Load Program...", ctrlBox);
    stepBtn = new QPushButton("Step", ctrlBox);
    runBtn = new QPushButton("Run", ctrlBox);
    resetBtn = new QPushButton("Reset", ctrlBox);
    helpBtn = new QPushButton("Help", ctrlBox);
    fileLabel = new QLabel("No program loaded", ctrlBox);
    loadBtn->setObjectName("loadBtn");
    stepBtn->setObjectName("stepBtn");
    runBtn->setObjectName("runBtn");
    resetBtn->setObjectName("resetBtn");
    helpBtn->setObjectName("helpBtn");

    ctrl->addWidget(new QLabel("Start address (hex, even):", ctrlBox));
    ctrl->addWidget(startEdit);
    ctrl->addWidget(loadBtn);
    ctrl->addSpacing(20);
    ctrl->addWidget(stepBtn);
    ctrl->addWidget(runBtn);
    ctrl->addWidget(resetBtn);
    ctrl->addStretch();
    ctrl->addWidget(fileLabel);
    ctrl->addWidget(helpBtn);

    // ================= CPU box (left) =================
    QGroupBox *cpuBox = new QGroupBox("CPU", central);
    cpuBox->setMinimumWidth(220);
    QVBoxLayout *cpuLayout = new QVBoxLayout(cpuBox);

    pcLabel = new QLabel(cpuBox);
    irLabel = new QLabel(cpuBox);
    stateLabel = new QLabel(cpuBox);
    pcLabel->setObjectName("pcLabel");
    irLabel->setObjectName("irLabel");
    stateLabel->setObjectName("stateLabel");
    pcLabel->setFont(mono);
    irLabel->setFont(mono);
    stateLabel->setFont(mono);

    regTable = new QTableWidget(16, 1, cpuBox);
    QStringList regNames;
    for (int i = 0; i < 16; ++i)
        regNames << QString("R%1").arg(i, 1, 16).toUpper();
    regTable->setVerticalHeaderLabels(regNames);
    regTable->setHorizontalHeaderLabels(QStringList() << "Value");
    regTable->horizontalHeader()->setStretchLastSection(true);
    // Rows share the available height, so all 16 registers (R0..RF) always fit.
    regTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    regTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    regTable->setSelectionMode(QAbstractItemView::NoSelection);
    regTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    regTable->verticalHeader()->setMinimumSectionSize(14);
    regTable->setMinimumHeight(16 * 16 + 28);
    regTable->setFont(mono);
    for (int i = 0; i < 16; ++i) {
        QTableWidgetItem *item = new QTableWidgetItem("00");
        item->setTextAlignment(Qt::AlignCenter);
        regTable->setItem(i, 0, item);
    }

    cpuLayout->addWidget(pcLabel);
    cpuLayout->addWidget(irLabel);
    cpuLayout->addWidget(stateLabel);
    cpuLayout->addWidget(regTable, 1);

    // ================= Memory box (center) =================
    QGroupBox *memBox = new QGroupBox("Main Memory (256 cells) - row + column = address", central);
    QVBoxLayout *memLayout = new QVBoxLayout(memBox);

    memTable = new QTableWidget(16, 16, memBox);
    QStringList cols, rows;
    for (int i = 0; i < 16; ++i) {
        cols << QString("%1").arg(i, 1, 16).toUpper();
        rows << hex2(i * 16);
    }
    memTable->setHorizontalHeaderLabels(cols);
    memTable->setVerticalHeaderLabels(rows);
    // Cells stretch to fill the whole box: no empty space on the right.
    memTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    memTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    memTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    memTable->setSelectionMode(QAbstractItemView::NoSelection);
    memTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    memTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    memTable->verticalHeader()->setMinimumSectionSize(14);
    memTable->horizontalHeader()->setMinimumSectionSize(24);
    memTable->setMinimumSize(16 * 30 + 50, 16 * 16 + 28);
    memTable->setFont(mono);
    for (int r = 0; r < 16; ++r) {
        for (int c = 0; c < 16; ++c) {
            QTableWidgetItem *item = new QTableWidgetItem("00");
            item->setTextAlignment(Qt::AlignCenter);
            memTable->setItem(r, c, item);
        }
    }
    memLayout->addWidget(memTable);

    QHBoxLayout *middle = new QHBoxLayout();
    middle->setSpacing(10);
    middle->addWidget(cpuBox);
    middle->addWidget(memBox, 1);

    // ================= Screen (bottom left) =================
    QGroupBox *screenBox = new QGroupBox("Screen (output written to address FF)", central);
    QVBoxLayout *screenLayout = new QVBoxLayout(screenBox);
    screenView = new QPlainTextEdit(screenBox);
    screenView->setObjectName("screenView");
    screenView->setReadOnly(true);
    screenView->setFont(mono);
    screenView->setMinimumHeight(70);
    screenLayout->addWidget(screenView);

    // ================= Edit values (bottom right) =================
    QGroupBox *editBox = new QGroupBox("Overwrite a Register or a Memory Cell", central);
    QGridLayout *grid = new QGridLayout(editBox);
    grid->setHorizontalSpacing(10);
    grid->setVerticalSpacing(8);

    regCombo = new QComboBox(editBox);
    for (int i = 0; i < 16; ++i)
        regCombo->addItem(regNames.at(i));
    regValEdit = new QLineEdit(editBox);
    regValEdit->setMaxLength(2);
    regValEdit->setFont(mono);
    regValEdit->setPlaceholderText("e.g. A5");
    setRegBtn = new QPushButton("Set Register", editBox);

    memAddrEdit = new QLineEdit(editBox);
    memAddrEdit->setMaxLength(2);
    memAddrEdit->setFont(mono);
    memAddrEdit->setPlaceholderText("00-FF");
    memValEdit = new QLineEdit(editBox);
    memValEdit->setMaxLength(2);
    memValEdit->setFont(mono);
    memValEdit->setPlaceholderText("e.g. 3C");
    setMemBtn = new QPushButton("Set Memory Cell", editBox);

    grid->addWidget(new QLabel("Register:", editBox), 0, 0);
    grid->addWidget(regCombo, 0, 1);
    grid->addWidget(new QLabel("New value (hex):", editBox), 0, 2);
    grid->addWidget(regValEdit, 0, 3);
    grid->addWidget(setRegBtn, 0, 4);

    grid->addWidget(new QLabel("Memory address (hex):", editBox), 1, 0);
    grid->addWidget(memAddrEdit, 1, 1);
    grid->addWidget(new QLabel("New value (hex):", editBox), 1, 2);
    grid->addWidget(memValEdit, 1, 3);
    grid->addWidget(setMemBtn, 1, 4);

    QHBoxLayout *bottom = new QHBoxLayout();
    bottom->setSpacing(10);
    bottom->addWidget(screenBox, 1);
    bottom->addWidget(editBox, 2);

    // ================= Status line =================
    statusLabel = new QLabel(central);
    statusLabel->setObjectName("statusLabel");
    statusLabel->setWordWrap(true);

    mainLayout->addWidget(ctrlBox);
    mainLayout->addLayout(middle, 1);
    mainLayout->addLayout(bottom);
    mainLayout->addWidget(statusLabel);

    // ================= Signals =================
    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::onLoad);
    connect(stepBtn, &QPushButton::clicked, this, &MainWindow::onStep);
    connect(runBtn, &QPushButton::clicked, this, &MainWindow::onRun);
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::onReset);
    connect(helpBtn, &QPushButton::clicked, this, &MainWindow::onHelp);
    connect(setRegBtn, &QPushButton::clicked, this, &MainWindow::onSetRegister);
    connect(setMemBtn, &QPushButton::clicked, this, &MainWindow::onSetMemory);
}

// "Theme" menu with Dark / Light / Retro. The choice is saved and restored on the next launch.
void MainWindow::buildThemeMenu()
{
    QMenu *menu = menuBar()->addMenu("&Theme");
    QActionGroup *group = new QActionGroup(this);
    group->setExclusive(true);

    const QString saved = QSettings("FCAI", "VoleGUI").value("theme", "Dark").toString();
    for (const Theme &t : allThemes()) {
        QAction *action = menu->addAction(t.name);
        action->setCheckable(true);
        group->addAction(action);
        const QString name = t.name;
        connect(action, &QAction::triggered, this, [this, name]() { applyTheme(name); });
        if (name == saved)
            action->setChecked(true);
    }
    if (!group->checkedAction() && !group->actions().isEmpty())
        group->actions().first()->setChecked(true);

    applyTheme(saved);
}

void MainWindow::applyTheme(const QString &name)
{
    const Theme &t = themeByName(name);
    theme = &t;
    qApp->setStyleSheet(buildQss(t));
    // Fixed row height (from the font) so all 16 registers always fit, in every theme.
    regTable->ensurePolished();
    regTable->horizontalHeader()->ensurePolished();
    const int rowH = regTable->fontMetrics().height() + 5;
    regTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    regTable->verticalHeader()->setMinimumSectionSize(10);
    regTable->verticalHeader()->setDefaultSectionSize(rowH);
    regTable->setMinimumHeight(regTable->horizontalHeader()->sizeHint().height()
                               + 16 * rowH + 2 * regTable->frameWidth() + 6);
    QSettings("FCAI", "VoleGUI").setValue("theme", t.name);

    refreshViews(); // re-color table cells for the new theme
    if (!lastStatus.isEmpty())
        setStatus(lastStatus, lastStatusError);
}

// Remember the current machine state so refreshViews() can highlight changes.
void MainWindow::snapshot()
{
    for (int i = 0; i < 16; ++i)
        prevRegs[i] = cpu.getRegister(i);
    for (int a = 0; a < 256; ++a)
        prevMem[a] = mem.read(static_cast<uint8_t>(a));
}

void MainWindow::refreshViews()
{
    const Theme &t = theme ? *theme : allThemes().front();
    const QBrush changedBg{QColor(t.changedBg)};
    const QBrush changedFg{QColor(t.changedFg)};
    const QBrush pcBg{QColor(t.pcBg)};
    const QBrush pcFg{QColor(t.pcFg)};
    const QBrush zeroFg{QColor(t.zeroFg)};
    const QBrush nonzeroFg{QColor(t.nonzeroFg)};
    const int pc = cpu.getPC();

    pcLabel->setText("PC: " + QString("%1").arg(pc, 2, 16, QChar('0')).toUpper());
    irLabel->setText("IR: " + QString::fromStdString(cpu.getIR()));
    if (!loaded)
        stateLabel->setText("State: no program");
    else
        stateLabel->setText(cpu.isHalted() ? "State: HALTED" : "State: ready");

    // Registers: changed = highlighted, "00" = dimmed, other values stand out
    for (int i = 0; i < 16; ++i) {
        const std::string v = cpu.getRegister(i);
        QTableWidgetItem *item = regTable->item(i, 0);
        item->setText(QString::fromStdString(v));
        const bool changed = (v != prevRegs[i]);
        if (changed) {
            item->setBackground(changedBg);
            item->setForeground(changedFg);
        } else {
            item->setBackground(QBrush());
            item->setForeground(v == "00" ? zeroFg : nonzeroFg);
        }
    }

    // Memory: changed = highlighted, cells at the PC = marked, "00" dimmed, others stand out
    for (int a = 0; a < 256; ++a) {
        const uint8_t v = mem.read(static_cast<uint8_t>(a));
        QTableWidgetItem *item = memTable->item(a / 16, a % 16);
        item->setText(hex2(v));
        const bool changed = (v != prevMem[a]);
        const bool atPc = loaded && (a == pc || a == pc + 1);
        if (changed) {
            item->setBackground(changedBg);
            item->setForeground(changedFg);
        } else if (atPc) {
            item->setBackground(pcBg);
            item->setForeground(pcFg);
        } else {
            item->setBackground(QBrush());
            item->setForeground(v == 0 ? zeroFg : nonzeroFg);
        }
    }

    // Screen
    screenView->setPlainText(QString::fromStdString(cpu.getScreenOutput()));
}

void MainWindow::updateButtons()
{
    const bool canRun = loaded && !cpu.isHalted();
    stepBtn->setEnabled(canRun);
    runBtn->setEnabled(canRun);
    resetBtn->setEnabled(loaded);
}

void MainWindow::setStatus(const QString &text, bool error)
{
    lastStatus = text;
    lastStatusError = error;
    const QString color = theme ? (error ? theme->err : theme->ok)
                                : (error ? QString("#c0392b") : QString("#1e8449"));
    statusLabel->setStyleSheet(QString("color: %1; %2")
                                   .arg(color, error ? "font-weight: bold;" : ""));
    statusLabel->setText(text);
}

QString MainWindow::haltMessage() const
{
    const std::string ir = cpu.getIR();
    if (!ir.empty() && ir[0] == 'C')
        return "Program halted (HALT instruction). Load a program or press Reset to run again.";
    return "Machine stopped: reached the end of memory, an invalid instruction, "
           "or the 10,000-step limit. Press Reset or load another program.";
}

// Parses 1-2 hex digits and checks the range.
bool MainWindow::parseHex(const QString &text, int minValue, int maxValue, int &out) const
{
    const QString t = text.trimmed();
    if (t.isEmpty() || t.size() > 2)
        return false;
    bool ok = false;
    const int v = t.toInt(&ok, 16);
    if (!ok || v < minValue || v > maxValue)
        return false;
    out = v;
    return true;
}

void MainWindow::onLoad()
{
    int addr = 0;
    if (!parseHex(startEdit->text(), 0x00, 0xFE, addr) || addr % 2 != 0) {
        setStatus("Invalid start address: it must be an EVEN hex number between 00 and FE.", true);
        return;
    }

    const QString path = QFileDialog::getOpenFileName(
        this, "Open a Vole program", QString(), "Text files (*.txt);;All files (*)");
    if (path.isEmpty())
        return;

    // The core loader validates every instruction and returns an empty vector on any error.
    const std::vector<uint8_t> program =
        loader.loadProgram(path.toLocal8Bit().toStdString(), addr);
    if (program.empty()) {
        setStatus("Could not load the program: the file is unreadable or empty, has a "
                  "malformed line, contains an invalid instruction, or does not fit in memory.",
                  true);
        return;
    }

    mem.clear();
    if (!loader.loadIntoMemory(mem, program, addr)) {
        setStatus("Failed to place the program in memory (invalid address or size).", true);
        return;
    }

    cpu.reset();
    cpu.setPC(addr);
    startAddr = addr;
    loaded = true;

    fileLabel->setText(QFileInfo(path).fileName());
    snapshot();
    refreshViews();
    updateButtons();
    setStatus(QString("Program loaded at address %1 (%2 instructions). Use Step or Run.")
                  .arg(hex2(addr))
                  .arg(program.size() / 2));
}

void MainWindow::onStep()
{
    if (!loaded || cpu.isHalted())
        return;

    snapshot();
    cpu.step(mem);
    refreshViews();
    updateButtons();

    if (cpu.isHalted())
        setStatus(haltMessage(), cpu.getIR().empty() || cpu.getIR()[0] != 'C');
    else
        setStatus("Executed instruction " + QString::fromStdString(cpu.getIR()) +
                  ". Press Step to continue.");
}

void MainWindow::onRun()
{
    if (!loaded || cpu.isHalted())
        return;

    snapshot();
    cpu.run(mem); // the core stops at HALT or after 10,000 steps
    refreshViews();
    updateButtons();
    setStatus(haltMessage(), cpu.getIR().empty() || cpu.getIR()[0] != 'C');
}

void MainWindow::onReset()
{
    if (!loaded)
        return;

    cpu.reset();
    cpu.setPC(startAddr);
    snapshot();
    refreshViews();
    updateButtons();
    setStatus("CPU reset: registers, IR and screen cleared, PC back to the start address. "
              "Memory was kept.");
}

void MainWindow::onSetRegister()
{
    int value = 0;
    if (!parseHex(regValEdit->text(), 0x00, 0xFF, value)) {
        setStatus("Invalid register value: enter 1-2 hex digits (00-FF).", true);
        return;
    }

    const int index = regCombo->currentIndex();
    snapshot();
    cpu.overwriteRegister(index, hex2(value).toStdString());
    refreshViews();
    setStatus(QString("Register R%1 set to %2.")
                  .arg(index, 1, 16)
                  .arg(hex2(value))
                  .toUpper());
}

void MainWindow::onSetMemory()
{
    int addr = 0;
    int value = 0;
    if (!parseHex(memAddrEdit->text(), 0x00, 0xFF, addr)) {
        setStatus("Invalid memory address: enter 1-2 hex digits (00-FF).", true);
        return;
    }
    if (!parseHex(memValEdit->text(), 0x00, 0xFF, value)) {
        setStatus("Invalid memory value: enter 1-2 hex digits (00-FF).", true);
        return;
    }

    snapshot();
    mem.write(static_cast<uint8_t>(addr), static_cast<uint8_t>(value));
    refreshViews();
    setStatus(QString("Memory cell %1 set to %2.").arg(hex2(addr)).arg(hex2(value)));
}

void MainWindow::onHelp()
{
    QMessageBox::information(
        this, "How to use the simulator",
        "1. Type a start address in hex (it must be even, e.g. 00 or 10).\n"
        "2. Press 'Load Program...' and choose a text file of Vole instructions.\n"
        "   Invalid programs are rejected with a message.\n"
        "3. Press 'Step' to run one instruction, or 'Run' to run until HALT.\n"
        "4. Yellow/amber cells changed in the last action; blue (bright green in Retro) "
        "cells are at the PC. Zero cells are dimmed.\n"
        "5. Use the bottom-right panel to overwrite a register or a memory cell.\n"
        "6. Anything stored to address FF appears in the Screen box.\n"
        "7. 'Reset' restarts the program from its start address (memory is kept).\n"
        "8. Use the 'Theme' menu to switch between Dark, Light and Retro.");
}