#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <functional>
#include <iostream>
#include <queue>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief The MainWindow class simulates the actions of an Automated External Defibrillator (AED)
 * device used in emergency medical situations. It includes methods for checking a patient's
 * responsiveness, calling an ambulance, placing the electrodes, delivering a shock, and
 * performing chest compressions.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

   private slots:
    /**
     * @brief updateTimer
     * Updates the timer on the GUI
     */
    void updateTimer();

    /**
     * @brief goodCase
     * Runs the good case scenario
     */
    void goodCase();

    /**
     * @brief badCase
     * Runs the bad case scenario
     */
    void badCase();

    /**
     * @brief processNextAction
     * Processes the next action in the action queue
     */
    void processNextAction();

    /**
     * @brief electrodesPlaced
     * Checks if the electrodes have been placed
     * @return true if the electrodes have been placed, false otherwise
     */
    bool electrodesPlaced();
    /**
     * @brief checkPatientResponsiveness
     * Displays text on the display asking the emergency responder
     * to check forpatient responsiveness
     */
    void checkPatientResponsiveness();

    /**
     * @brief callAmbulance
     * Displays text on the display asking the emergency responder
     * to call an ambulance
     */
    void callAmbulance();

    /**
     * @brief placeElectrodes
     * Displays text on the display asking the emergency responder
     * to place the electrodes
     */
    void placeElectrodes();

    /**
     * @brief readyToShock
     * AED signals that it is ready to shock
     */
    void readyToShock();

    /**
     * @brief vFibDetected
     * AED signals that it has detected ventricular fibrillation
     */
    void vFibDetected();

    /**
     * @brief vTachDetected
     * AED signals that it has detected ventricular tachycardia
     */
    void vTachDetected();

    /**
     * @brief noShockAdvised
     * AED signals that it has detected a normal heart rhythm and no shock is advised
     */
    void noShockAdvised();

    /**
     * @brief administerShock
     * AED signals that it has detected a shockable heart rhythm
     */
    void administerShock();

    /**
     * @brief normalRhythmDetected
     * AED signals that it has detected a normal heart rhythm
     */
    void normalRhythmDetected();

    /**
     * @brief flatlineDetected
     * AED signals that it has detected a flatline
     */
    void flatlineDetected();

    /**
     * @brief performChestCompressions
     * Displays text on the display asking the emergency responder
     * to perform chest compressions
     */
    void performChestCompressions();

    /**
     * @brief rythymAnalysis
     * Displays text on the display rythym analysis
     */
    void rythymAnalysis();

    /**
     * @brief clearQueue
     * Clears the action queue
     */
    void clearQueue();

    /**
     * @brief chargeBattery
     * Displays text on the display asking the emergency responder
     * to charge the battery
     */
    void chargeBattery();

    /**
     * @brief powerChange
     * Updates the power level on the GUI
     */
    void powerChange();

    /**
     * @brief cprFeedback_1
     * Informs the emergency responder that they are performing bad chest compressions
     */
    void cprFeedback_1();

    /**
     * @brief cprFeedback_2
     * Informs the emergency responder that they are performing bad chest compressions
     */
    void cprFeedback_2();

    /**
     * @brief cprFeedback_3
     * Informs the emergency responder that they are performing good chest compressions
     */
    void cprFeedback_3();

    /**
     * @brief restartSimulation
     * Restarts the simulation
     */
    void restartSimulation();

    /**
     * @brief changePowerStatus
     * Changes the power status on the GUI
     */
    void changePowerStatus();

    /**
     * @brief updateBatteryLevel
     * Updates the battery level on the GUI
     */
    void updateBatteryLevel();

    /**
     * @brief rechargeBattery
     * Recharges the battery of the AED
     */
    void rechargeBattery();

   private:
    Ui::MainWindow* ui;                   // Pointer to the UI
    queue<function<void()>> actionQueue;  // Queue of actions to be performed

    QTimer* actionTimer;   // Timer that controls the actions of the AED
    QTimer* batteryTimer;  // Timer that controls the battery level on the GUI
    QTimer* timer;         // Timer that controls the timer on the GUI
    QTime timeElapsed;     // Time elapsed since the start of the simulation
    QLabel* statusLabel;   // Label that displays the status of the AED

    bool powerStatus;         // Status of the power
    int numShocks;            // Number of shocks delivered
    int currentBatteryLevel;  // Current battery level percentage
    int caseNumber;           // Case number
};

#endif  // MAINWINDOW_H
