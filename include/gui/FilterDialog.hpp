#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include "utils/Logger.hpp"
#include "core/NetworkMonitor.hpp"

class FilterDialog : public QDialog {
    Q_OBJECT

public:
    explicit FilterDialog(NetworkMonitor* monitor, QWidget* parent = nullptr);
    QString getFilter() const { return filter_edit_->text(); }

private slots:
    void validateAndAccept();

private:
    QLineEdit* filter_edit_;
    QPushButton* ok_button_;
    QPushButton* cancel_button_;
    QLabel* help_label_;
    NetworkMonitor* monitor_;
    
    bool validateFilter(const QString& filter, QString& errorMessage);
};