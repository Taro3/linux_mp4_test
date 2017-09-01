#include <QStandardPaths>
#include <QFile>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMediaObject>
#include <QGraphicsVideoItem>
#include <QGraphicsScene>
#include <QSizeF>
#include <QDesktopWidget>

#include "mainwindow.h"
#include "ui_mainwindow.h"


const QString MainWindow::VIDEO_FILE_NAME = "TestVideo.mp4";  //!< 動画ファイル名
//const QString MainWindow::VIDEO_FILE_NAME = "TestVideo.wmv";  //!< 動画ファイル名

//**********************************************************************************************************************
/**
 * @brief       MainWindow::MainWindow
 *              コンストラクタ
 * @param[in]   parent  親ウィジェットポインタ
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_pcPlayer(NULL)
  , m_pcGVItem(NULL)
{
    ui->setupUi(this);

    /*
     *  ビデオ出力関係初期設定
     */
    m_pcPlayer = new QMediaPlayer(this);

    m_pcGVItem = new QGraphicsVideoItem;
    QGraphicsScene *pScene = new QGraphicsScene(ui->graphicsView);
    pScene->setBackgroundBrush(Qt::black);
    ui->graphicsView->setScene(pScene);
    ui->graphicsView->scene()->addItem(m_pcGVItem);
    ui->graphicsView->installEventFilter(this);

    m_pcPlayer->setVideoOutput(m_pcGVItem);
    connect(m_pcPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(stateChanged(QMediaPlayer::State)));

    QStringList clstDirs = QStandardPaths::standardLocations(QStandardPaths::MoviesLocation);   // システムの動画ファイルパスを取得
    QString strMoviePath = clstDirs.at(0) + "/" + VIDEO_FILE_NAME;

    if (clstDirs.isEmpty() == false)
    {
        if (QFile::exists(strMoviePath) == false)
        {
            strMoviePath = "./" + VIDEO_FILE_NAME;

            if (QFile::exists(strMoviePath) == false)
            {
                strMoviePath.clear();
            }
        }
    }
    qDebug() << "The movie file path = '" + strMoviePath + "'";

    if (strMoviePath.isEmpty() == false)
    {
        m_pcPlayer->setMedia(QUrl::fromLocalFile(strMoviePath));
    }

    /*
     * 再生位置スライダー初期化
     */
    connect(m_pcPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));

    /*
     * ボリューム位置設定
     */
    ui->verticalSliderVolume->setSliderPosition(m_pcPlayer->volume());

    /*
     * 再生速度初期化
     */
    m_pcPlayer->setPlaybackRate(1.0f);
}

//**********************************************************************************************************************
/**
 * @brief MainWindow::~MainWindow
 *        デストラクタ
 */
MainWindow::~MainWindow()
{
    delete ui;
}

//**********************************************************************************************************************
/**
 * @brief       MainWindow::showEvent
 *              Widget表示イベントハンドラ
 * @param[in]   event   イベントデータ
 */
void MainWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    resizeVideoWidget();
}

//**********************************************************************************************************************
/**
 * @brief       MainWindow::resizeEvent
 *              リサイズイベントハンドラ
 * @param[in]   event   イベントデータ
 */
void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    resizeVideoWidget();
}

//**********************************************************************************************************************
/**
 * @brief       MainWindow::eventFilter
 *              イベントフィルタ処理
 * @param[in]   watched 対象オブジェクト
 * @param[in]   event   イベント情報
 * @return      処理結果
 * @retval      true    処理済み
 * @retval      false   未処理
 */
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    bool isProcessed = false;

    if (watched == ui->graphicsView && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *pKeyEvent = static_cast<QKeyEvent*>(event);
        if (pKeyEvent->key() == Qt::Key_Escape)
        {
            ui->widgetRightSide->show();
            ui->widgetBottomSide->show();
            this->showNormal();
            isProcessed = true;
        }
    }
    else if (watched == ui->graphicsView && event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent *pMouseEvent = static_cast<QMouseEvent*>(event);
        if (pMouseEvent->button() == Qt::LeftButton)
        {
            if (this->isFullScreen() == true)
            {
                ui->widgetRightSide->show();
                ui->widgetBottomSide->show();
                this->showNormal();
            }
            else
            {
                on_pushButtonFullScreen_clicked();
            }
            isProcessed = true;
        }
    }

    return isProcessed;
}

//**********************************************************************************************************************
/**
 * @brief MainWindow::on_pushButtonPlay_clicked
 *        再生ボタンクリックイベントハンドラ
 */
void MainWindow::on_pushButtonPlay_clicked()
{
    m_pcPlayer->play();
    qDebug() << "playback rate =" + QString::number(m_pcPlayer->playbackRate());
    ui->horizontalSliderPlaybackRate->setSliderPosition(m_pcPlayer->playbackRate() * 10);
    ui->horizontalSliderPosition->setMaximum(m_pcPlayer->duration());
    ui->horizontalSliderPosition->setPageStep(m_pcPlayer->duration() / 100);
    qDebug() << "video duration = " + QString::number(m_pcPlayer->duration());
}

//**********************************************************************************************************************
/**
 * @brief       MainWindow::stateChanged
 *              プレイヤー状態変更イベントハンドラ
 * @param[in]   state   プレイヤー状態
 */
void MainWindow::stateChanged(QMediaPlayer::State state)
{
    QString strMessage;
    bool fPauseEnable = false;
    bool fPlayEnable = true;
    bool fStopEnale = false;

    switch (state)
    {
    case QMediaPlayer::StoppedState:
        strMessage = "Stopped.";
        break;

    case QMediaPlayer::PlayingState:
        strMessage = "Playing.";
        fPauseEnable = true;
        fPlayEnable = false;
        fStopEnale = true;
        break;

    case QMediaPlayer::PausedState:
        strMessage = "Paused.";
        fStopEnale = true;
        break;

    default:
        strMessage = "Unknown.";
        break;
    }
    ui->statusBar->showMessage(strMessage);
    ui->pushButtonPause->setEnabled(fPauseEnable);
    ui->pushButtonPlay->setEnabled(fPlayEnable);
    ui->pushButtonStop->setEnabled(fStopEnale);
}

//**********************************************************************************************************************
/**
 * @brief MainWindow::resizeVideoWidget
 *        ビデオ表示用ウィジェットのサイズ設定処理
 */
void MainWindow::resizeVideoWidget()
{
    QRect r = ui->graphicsView->rect();
#ifdef  WIN32
    r.adjust(0, 0, -2, -2);
#endif
    ui->graphicsView->setSceneRect(r);
    m_pcGVItem->setSize(ui->graphicsView->sceneRect().size());
}

//**********************************************************************************************************************
/**
 * @brief MainWindow::on_pushButtonPause_clicked
 *        一時停止ボタンクリックイベントハンドラ
 */
void MainWindow::on_pushButtonPause_clicked()
{
    m_pcPlayer->pause();
}

//**********************************************************************************************************************
/**
 * @brief MainWindow::on_pushButtonStop_clicked
 *        停止ボタンクリックイベントハンドラ
 */
void MainWindow::on_pushButtonStop_clicked()
{
    m_pcPlayer->stop();
}

//**********************************************************************************************************************
/**
 * @brief       MainWindow::positionChanged
 *              再生位置変更イベントハンドラ
 * @param[in]   position    再生位置
 */
void MainWindow::positionChanged(qint64 position)
{
    ui->horizontalSliderPosition->setValue(position);
}

//**********************************************************************************************************************
/**
 * @brief       MainWindow::on_verticalSliderVolume_actionTriggered
 *              ボリュームスライダー操作イベントハンドラ
 * @param[in]   action  操作種別
 */
void MainWindow::on_verticalSliderVolume_actionTriggered(int action)
{
    if (action == QAbstractSlider::SliderToMinimum)         // 先頭へのジャンプ処理を末尾へのジャンプに変更する
    {
        ui->verticalSliderVolume->setSliderPosition(ui->verticalSliderVolume->maximum());
    }
    else if (action == QAbstractSlider::SliderToMaximum)    // 末尾へのジャンプを先頭へのジャンプに変更する
    {
        ui->verticalSliderVolume->setSliderPosition(ui->verticalSliderVolume->minimum());
    }

    m_pcPlayer->setVolume(ui->verticalSliderVolume->sliderPosition());
}

//**********************************************************************************************************************
/**
 * @brief       MainWindow::on_horizontalSliderPlaybackRate_actionTriggered
 *              再生速度スライダー操作イベントハンドラ
 * @param[in]   action  操作種別
 */
void MainWindow::on_horizontalSliderPlaybackRate_actionTriggered(int action)
{
    Q_UNUSED(action);
    m_pcPlayer->setPlaybackRate(ui->horizontalSliderPlaybackRate->sliderPosition() / 10.0f);
}

//**********************************************************************************************************************
/**
 * @brief       MainWindow::on_pushButtonResetPlaybackRate_clicked
 *              再生速度リセットボタンクリックイベントハンドラ
 */
void MainWindow::on_pushButtonResetPlaybackRate_clicked()
{
    m_pcPlayer->setPlaybackRate(1.0f);
    ui->horizontalSliderPlaybackRate->setSliderPosition(10);
}

//**********************************************************************************************************************
/**
 * @brief       MainWindow::on_horizontalSliderPosition_actionTriggered
 *              再生位置スライダー操作イベントハンドラ
 * @param[in]   action  操作種別
 */
void MainWindow::on_horizontalSliderPosition_actionTriggered(int action)
{
    Q_UNUSED(action);
    m_pcPlayer->setPosition(ui->horizontalSliderPosition->sliderPosition());
}

//**********************************************************************************************************************
/**
 * @brief       MainWindow::on_pushButtonFullScreen_clicked
 *              [Full Screen]ボタンクリックイベントハンドラ
 */
void MainWindow::on_pushButtonFullScreen_clicked()
{
    ui->widgetRightSide->hide();
    ui->widgetBottomSide->hide();
    ui->statusBar->hide();
    this->showFullScreen();
}
