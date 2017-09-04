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
#if defined(WIN32)
    void videoFullScreenChanged(bool fullScreen);
#endif
    void playbackRateChanged(qreal rate);
    void on_horizontalSliderBrightness_actionTriggered(int action);
    void on_pushButtonResetBrightness_clicked();

private:
    Ui::MainWindow          *ui;
    QMediaPlayer            *m_pcPlayer;            //!< メディアプレイヤーオブジェクト
    QVideoWidget            *m_pcVWidget;           //!< 動画表示用ウィジェット
    static const QString    VIDEO_FILE_NAME;        //!< 再生動画ファイル名
    QString                 m_strFilename;          //!< 動画ファイルパス

    void resizeVideoWidget();
#if !defined(WIN32)
    void createPlayer();
#endif
};

#endif // MAINWINDOW_H
