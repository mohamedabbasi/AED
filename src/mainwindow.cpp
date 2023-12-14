#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      timeElapsed(0, 0),
      numShocks(0),
      currentBatteryLevel(100) {
    ui->setupUi(this);
    resize(940, 568);

    timer = new QTimer(this);
    timer->start(1000);

    statusLabel = new QLabel(this);
    statusLabel->setFixedSize(16, 16);

    batteryTimer = new QTimer(this);
    batteryTimer->start(10000);

    actionTimer = new QTimer(this);

    ui->timeElapsed->setText("00:00");
    ui->batteryLevel->setValue(currentBatteryLevel);

    powerStatus = false;
    ui->cprValue->setVisible(false);
    ui->cprLine->setVisible(false);
    changePowerStatus();

    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    connect(actionTimer, &QTimer::timeout, this, &MainWindow::processNextAction);
    connect(batteryTimer, &QTimer::timeout, this, &MainWindow::updateBatteryLevel);
    connect(ui->powerButton, &QPushButton::released, this, &MainWindow::powerChange);
    connect(ui->goodCaseButton, &QPushButton::released, this, &MainWindow::goodCase);
    connect(ui->badCaseButton, &QPushButton::released, this, &MainWindow::badCase);
    connect(ui->rechargeBattery, &QPushButton::released, this, &MainWindow::rechargeBattery);
    connect(ui->nodePlacement, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::restartSimulation);
}

MainWindow::~MainWindow() {
    delete ui;
    delete timer;
    delete statusLabel;
    delete batteryTimer;
    delete actionTimer;
}

void MainWindow::goodCase() {
    numShocks = 0;
    caseNumber = 0;
    if (powerStatus) {
        actionQueue.push([this]() { checkPatientResponsiveness(); });
        actionQueue.push([this]() { callAmbulance(); });
        actionQueue.push([this]() { placeElectrodes(); });
        if (electrodesPlaced()) {
            actionQueue.push([this]() { readyToShock(); });
            actionQueue.push([this]() { vFibDetected(); });
            actionQueue.push([this]() { readyToShock(); });
            actionQueue.push([this]() { administerShock(); });
            actionQueue.push([this]() { normalRhythmDetected(); });
            actionQueue.push([this]() { performChestCompressions(); });
            actionQueue.push([this]() { cprFeedback_1(); });
            actionQueue.push([this]() { cprFeedback_2(); });
            actionQueue.push([this]() { cprFeedback_3(); });
            actionQueue.push([this]() { rythymAnalysis(); });
        }
        actionTimer->start(3000);
    }
}

void MainWindow::badCase() {
    numShocks = 0;
    caseNumber = 1;
    if (powerStatus) {
        actionQueue.push([this]() { checkPatientResponsiveness(); });
        actionQueue.push([this]() { callAmbulance(); });
        actionQueue.push([this]() { placeElectrodes(); });
        if (electrodesPlaced()) {
            actionQueue.push([this]() { readyToShock(); });
            actionQueue.push([this]() { flatlineDetected(); });
            actionQueue.push([this]() { noShockAdvised(); });
            actionQueue.push([this]() { performChestCompressions(); });
            actionQueue.push([this]() { vFibDetected(); });
            actionQueue.push([this]() { readyToShock(); });
            actionQueue.push([this]() { administerShock(); });
            actionQueue.push([this]() { performChestCompressions(); });
            actionQueue.push([this]() { cprFeedback_1(); });
            actionQueue.push([this]() { cprFeedback_2(); });
            actionQueue.push([this]() { cprFeedback_3(); });
            actionQueue.push([this]() { vFibDetected(); });
            actionQueue.push([this]() { readyToShock(); });
            actionQueue.push([this]() { administerShock(); });
            actionQueue.push([this]() { performChestCompressions(); });
            actionQueue.push([this]() { cprFeedback_1(); });
            actionQueue.push([this]() { cprFeedback_2(); });
            actionQueue.push([this]() { cprFeedback_3(); });
            actionQueue.push([this]() { vFibDetected(); });
            actionQueue.push([this]() { readyToShock(); });
            actionQueue.push([this]() { administerShock(); });
            actionQueue.push([this]() { performChestCompressions(); });
            actionQueue.push([this]() { cprFeedback_1(); });
            actionQueue.push([this]() { cprFeedback_2(); });
            actionQueue.push([this]() { cprFeedback_3(); });
            actionQueue.push([this]() { vFibDetected(); });
            actionQueue.push([this]() { readyToShock(); });
            actionQueue.push([this]() { administerShock(); });
            actionQueue.push([this]() { flatlineDetected(); });
            actionQueue.push([this]() { rythymAnalysis(); });
        }
        actionTimer->start(3000);
    }
}

void MainWindow::processNextAction() {
    if (!actionQueue.empty()) {
        std::function<void()> nextAction = actionQueue.front();
        actionQueue.pop();
        nextAction();
    }
}

bool MainWindow::electrodesPlaced() {
    return ui->nodePlacement->currentText() == "True";
}

void MainWindow::updateTimer() {
    timeElapsed = timeElapsed.addSecs(1);
    ui->timeElapsed->setText(timeElapsed.toString("mm:ss"));
}

void MainWindow::checkPatientResponsiveness() {
    statusLabel->move(248, 240);
    ui->mainScreenDisplay->setText("CHECKING\nRESPONSIVENESS");
    qDebug("Voice Prompt: Checking the patient's responsiveness.");
}

void MainWindow::callAmbulance() {
    statusLabel->move(339, 125);
    ui->mainScreenDisplay->setText("CALL\nAMBULANCE");
    qDebug("Voice Prompt: Call an ambulance.");
}

void MainWindow::cprFeedback_1() {
    ui->cprValue->setText("1");
    ui->cprValue->move(156, 30);
    ui->cprLine->move(178, 40);
    ui->mainScreenDisplay->setText("PUSH\nHARDER");
    qDebug("Voice Prompt: Push Harder.");
}

void MainWindow::cprFeedback_2() {
    ui->cprValue->setText("1.5\"");
    ui->cprValue->move(156, 41);
    ui->cprLine->move(178, 51);
    ui->mainScreenDisplay->setText("PUSH\nHARDER");
    qDebug("Voice Prompt: Push Harder.");
}

void MainWindow::cprFeedback_3() {
    ui->cprValue->setText("2.2\"");
    ui->cprValue->move(156, 73);
    ui->cprLine->move(178, 85);
    ui->mainScreenDisplay->setText("GOOD\nCPR");
    qDebug("Voice Prompt: Good CPR.");
}

void MainWindow::placeElectrodes() {
    statusLabel->move(487, 291);
    ui->mainScreenDisplay->setText("PLACE\nELECTRODES");
    qDebug("Voice Prompt: Place the electrodes.");
}

void MainWindow::readyToShock() {
    statusLabel->move(489, 407);
    ui->mainScreenDisplay->setText("READY\nTO\nSHOCK");
    qDebug("Voice Prompt: Ready to shock.");
}

void MainWindow::vFibDetected() {
    ui->mainScreenDisplay->setPixmap(QPixmap(":/v-fib.png"));
    qDebug("Voice Prompt: VF detected.");
}

void MainWindow::vTachDetected() {
    ui->mainScreenDisplay->setPixmap(QPixmap(":/v-tach.png"));
    qDebug("Voice Prompt: VT detected.");
}

void MainWindow::noShockAdvised() {
    ui->mainScreenDisplay->setText("NO\nSHOCK\nADVISED");
    qDebug("Voice Prompt: No shock advised.");
}

void MainWindow::administerShock() {
    numShocks++;
    int shocklevel = 20;

    if (ui->ageGroupLabel_2->currentIndex() == 0) {
        shocklevel = 20;
    } else if (ui->ageGroupLabel_2->currentIndex() == 1) {
        shocklevel = 10;
    } else {
        shocklevel = 5;
    }

    currentBatteryLevel -= shocklevel;
    statusLabel->move(367, 315);
    ui->mainScreenDisplay->setText("STAND\nCLEAR");
    qDebug("Voice Prompt: Shock delivered.");
    updateBatteryLevel();
    ui->shockCounter->setText(QString::number(numShocks));
}

void MainWindow::normalRhythmDetected() {
    ui->mainScreenDisplay->setPixmap(QPixmap(":/good-case.png"));
    qDebug("Voice Prompt: Normal rhythm detected.");
}

void MainWindow::flatlineDetected() {
    ui->mainScreenDisplay->setPixmap(QPixmap(":/flatline.png"));
    qDebug("Voice Prompt: Flatline detected.");
}

void MainWindow::performChestCompressions() {
    ui->cprValue->setVisible(true);
    ui->cprLine->setVisible(true);
    statusLabel->move(260, 416);
    ui->mainScreenDisplay->setText("PERFORM\nCHEST\nCOMPRESSIONS");
    qDebug("Voice Prompt: Perform chest compressions.");
}

void MainWindow::rythymAnalysis() {
    ui->mainScreenDisplay->setText("SIMULATION\nCOMPLETE");
    statusLabel->setVisible(false);
    qDebug("Voice Prompt: Simulation complete.");
}

void MainWindow::clearQueue() {
    for (int i = 0; i < (int)actionQueue.size(); i++) {
        actionQueue.pop();
    }
}

void MainWindow::chargeBattery() {
    clearQueue();
    ui->mainScreenDisplay->setText("CHARGE\nBATTERY");
    qDebug("Voice Prompt: Charge the battery.");
}

void MainWindow::powerChange() {
    powerStatus = !powerStatus;
    changePowerStatus();
}

void MainWindow::changePowerStatus() {
    timeElapsed = QTime(0, 0);
    numShocks = 0;
    clearQueue();

    statusLabel->setPixmap(QPixmap(":/red-circle.png"));
    statusLabel->move(0, 0);
    ui->mainScreenDisplay->setVisible(powerStatus);
    ui->mainScreenDisplay->setText("PERFORMING\nSELF-TEST\n");
    ui->timeElapsed->setVisible(powerStatus);
    ui->batteryLevel->setVisible(powerStatus);
    ui->shockCounter->setVisible(powerStatus);
    ui->shocksLabel->setVisible(powerStatus);
    ui->compressionDepth->setVisible(powerStatus);
    statusLabel->setVisible(powerStatus);
    ui->cprValue->setVisible(false);
    ui->cprLine->setVisible(false);
    ui->minCPR->setVisible(powerStatus);
    ui->minCPRLine->setVisible(powerStatus);
}

void MainWindow::updateBatteryLevel() {
    currentBatteryLevel -= 1;
    if (currentBatteryLevel < 0) {
        currentBatteryLevel = 0;
        chargeBattery();
    } else if (currentBatteryLevel <= 20) {
        chargeBattery();
    }
    ui->batteryLevel->setValue(currentBatteryLevel);
}

void MainWindow::rechargeBattery() {
    currentBatteryLevel = 100;
    ui->batteryLevel->setValue(currentBatteryLevel);
}

void MainWindow::restartSimulation() {
    clearQueue();

    if (!electrodesPlaced()) {
        actionQueue.push([this]() { ui->mainScreenDisplay->setText("ELECTRODES\nDETACHED\n"); });
    }

    caseNumber == 0 ? goodCase() : badCase();
}
