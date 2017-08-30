#include <QStandardPaths>
#include <QFile>

#include "mainwindow.h"
#include "ui_mainwindow.h"


const QString MainWindow::VIDEO_FILE_NAME = "TestVideo.mp4";  //!< 動画ファイル名

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
  , m_pcVWidget(NULL)
{
    ui->setupUi(this);

    /*
     *  ビデオ出力関係初期設定
     */
    m_pcPlayer = new QMediaPlayer(this);
    m_pcVWidget = new QVideoWidget(ui->widget);
    QPalette vwPal = palette();                         // Windows環境では背景が透過になってしまうためパレットの背景色を黒に設定
    vwPal.setColor(QPalette::Background, Qt::black);
    m_pcVWidget->setAutoFillBackground(true);
    m_pcVWidget->setPalette(vwPal);

    m_pcPlayer->setVideoOutput(m_pcVWidget);
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
        m_pcPlayer->setMedia(QUrl::fromLocalFile(clstDirs.at(0) + "/TestVideo.mp4"));
    }

    /*
     * 再生位置更新シグナル接続
     */
    connect(m_pcPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
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
 * @brief MainWindow::on_pushButtonPlay_clicked
 *        再生ボタンクリックイベントハンドラ
 */
void MainWindow::on_pushButtonPlay_clicked()
{
    m_pcPlayer->play();
    ui->horizontalSlider->setMaximum(m_pcPlayer->duration());
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
    m_pcVWidget->setGeometry(ui->widget->rect());
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
    ui->horizontalSlider->setSliderPosition(position);
    qDebug() << "player position = " + QString::number(position);
}

//**********************************************************************************************************************
/**
 * @brief       MainWindow::on_horizontalSlider_sliderMoved
 *              スライダー位置変更イベントハンドラ
 * @param[in]   position    スライダー位置
 */
void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    m_pcPlayer->setPosition(position);
    qDebug() << "slider position = " + QString::number(position);
}
