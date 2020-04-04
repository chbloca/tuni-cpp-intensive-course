#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Normally this member should be in private part of the
    // class, but due to the automated tests it must be more
    // accessible.
    QTimer* timer;

private slots:
    // Add your slots here

    void on_startButton_clicked();
    void second_passed();
    void reset_button_clicked();
    void stop_button_clicked();

private:
    Ui::MainWindow *ui;
    // Add other necessary members here
    int what_time_ = 0;
    int seconds_;
    int minutes_;
};

#endif // MAINWINDOW_HH
