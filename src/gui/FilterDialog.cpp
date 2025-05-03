#include "gui/FilterDialog.hpp"

FilterDialog::FilterDialog(NetworkMonitor* monitor, QWidget* parent)
    : QDialog(parent)
    , monitor_(monitor)
{
    setWindowTitle(tr("Set BPF Filter"));
    
    // Create widgets
    filter_edit_ = new QLineEdit(this);
    ok_button_ = new QPushButton(tr("OK"), this);
    cancel_button_ = new QPushButton(tr("Cancel"), this);
    help_label_ = new QLabel(tr("Enter a BPF expression (e.g. \"tcp port 80\" or \"host 192.168.1.1\")"), this);
    help_label_->setWordWrap(true);
    
    // Set up layout
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->addWidget(help_label_);
    main_layout->addWidget(filter_edit_);
    
    QHBoxLayout* button_layout = new QHBoxLayout();
    button_layout->addStretch();
    button_layout->addWidget(ok_button_);
    button_layout->addWidget(cancel_button_);
    main_layout->addLayout(button_layout);
    
    // Connect signals
    connect(ok_button_, &QPushButton::clicked, this, &FilterDialog::validateAndAccept);
    connect(cancel_button_, &QPushButton::clicked, this, &QDialog::reject);
    connect(filter_edit_, &QLineEdit::returnPressed, this, &FilterDialog::validateAndAccept);
    
    filter_edit_->setFocus();
    setMinimumWidth(400);
}

void FilterDialog::validateAndAccept()
{
    QString filter = filter_edit_->text().trimmed();
    QString errorMessage;
    
    if (filter.isEmpty()) {
        // Empty filter is valid (clears filter)
        accept();
        return;
    }
    
    if (validateFilter(filter, errorMessage)) {
        accept();
    } else {
        QMessageBox::warning(this, tr("Invalid Filter"), 
            tr("The filter expression is invalid:\n\n%1").arg(errorMessage));
        
        // Log the error
        Logger::error("Invalid BPF filter expression: " + filter.toStdString() + 
                      " - Error: " + errorMessage.toStdString());
    }
}

bool FilterDialog::validateFilter(const QString& filter, QString& errorMessage)
{
    try {
        // Use the monitor to validate the filter
        return monitor_->validateBPFFilter(filter.toStdString(), errorMessage);
    } catch (const std::exception& e) {
        errorMessage = QString::fromStdString(e.what());
        return false;
    } catch (...) {
        errorMessage = tr("Unknown error occurred while validating filter");
        return false;
    }
}