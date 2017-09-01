#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QGraphicsVideoItem>


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
    virtual bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_pushButtonPlay_clicked();
    void stateChanged(QMediaPlayer::State state);
    void on_pushButtonPause_clicked();
    void on_pushButtonStop_clicked();
    void positionChanged(qint64 position);
    void on_verticalSliderVolume_actionTriggered(int action);
    void on_horizontalSliderPlaybackRate_actionTriggered(int action);
    void on_pushButtonResetPlaybackRate_clicked();
    void on_horizontalSliderPosition_actionTriggered(int action);
    void on_pushButtonFullScreen_clicked();

private:
    Ui::MainWindow          *ui;
    QMediaPlayer            *m_pcPlayer;            //!< メディアプレイヤーオブジェクト
    QGraphicsVideoItem      *m_pcGVItem;
    static const QString    VIDEO_FILE_NAME;        //!< 再生動画ファイル名

    void resizeVideoWidget();
};

#endif // MAINWINDOW_H
