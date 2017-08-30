#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>


namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void showEvent(QShowEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

private slots:
    void on_pushButtonPlay_clicked();
    void stateChanged(QMediaPlayer::State state);
    void on_pushButtonPause_clicked();
    void on_pushButtonStop_clicked();
    void positionChanged(qint64 position);
    void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::MainWindow          *ui;
    QMediaPlayer            *m_pcPlayer;
    QVideoWidget            *m_pcVWidget;
    static const QString    VIDEO_FILE_NAME;

    void resizeVideoWidget();
};

#endif // MAINWINDOW_H
