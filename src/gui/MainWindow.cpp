#include "gui/MainWindow.hpp"
#include "gui/FilterDialog.hpp"
#include "utils/Logger.hpp"
#include <QMessageBox>

void MainWindow::showFilterDialog()
{
    // Create FilterDialog with the NetworkMonitor instance
    FilterDialog dialog(monitor_, this);
    
    if (dialog.exec() == QDialog::Accepted) {
        QString filter = dialog.getFilter();
        try {
            // Set the filter using NetworkMonitor
            monitor_->setFilter(filter.toStdString());
            
            // Update status bar with the new filter
            statusBar()->showMessage(tr("Filter set: %1").arg(filter));
            
            // Log the filter application
            Logger::info("Applied BPF filter: " + filter.toStdString());
        } 
        catch (const std::exception& e) {
            // Fallback error handling in case of unexpected issues
            QMessageBox::critical(this, tr("Filter Error"), 
                tr("Failed to apply filter: %1").arg(e.what()));
            
            // Log the error
            Logger::error("Failed to apply BPF filter: " + std::string(e.what()));
        }
    }
}

void MainWindow::setupFilterActions()
{
    // Create filter action if not already existing
    QAction* filterAction = new QAction(tr("Set BPF Filter"), this);
    filterAction->setShortcut(QKeySequence(tr("Ctrl+F")));
    
    // Connect the filter action to the showFilterDialog method
    connect(filterAction, &QAction::triggered, this, &MainWindow::showFilterDialog);
    
    // Add the action to the appropriate menu (assuming a View or Tools menu)
    if (viewMenu) {
        viewMenu->addAction(filterAction);
    }
}

void MainWindow::updateFilterStatus()
{
    // Optional: Method to update UI to reflect current filter status
    QString currentFilter = QString::fromStdString(monitor_->getCurrentFilter());
    if (!currentFilter.isEmpty()) {
        statusBar()->showMessage(tr("Current Filter: %1").arg(currentFilter));
    } else {
        statusBar()->showMessage(tr("No Filter Applied"));
    }
}