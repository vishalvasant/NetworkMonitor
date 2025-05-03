#pragma once

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QTabWidget>
#include <QTimer>
#include "core/NetworkMonitor.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(NetworkMonitor* monitor, QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void showFilterDialog();  // New method for filter dialog
    void setupFilterActions();  // New method to set up filter-related actions
    void updateFilterStatus();  // New method to update filter status
    void toggleMonitoring(bool start);
    void loadSettings();
    void changeTheme(const QString& theme);
    void setupConnections();
    void updateDisplay();

private:
    NetworkMonitor* monitor_;
    QSettings settings_;
    QTabWidget* tab_widget_;
    QTimer* update_timer_;
    QMenu* viewMenu;  // Menu for view-related actions
    QAction* start_action_;
    QAction* stop_action_;
    bool is_monitoring_{false};
};