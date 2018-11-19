#include "ImageViewer.h"

#include "Viewer.h"
#include "Tools.h"

#include <QKeySequence>
#include <QFileDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QImage>
#include <QImageReader>
#include <QDebug>
#include <QToolButton>
#include <QAction>
#include <QSettings>
#include <QMenuBar>

namespace {

QToolButton* createToolButton(QWidget* parent, QString tooltip, QString iconPath) {
    QToolButton* button = new QToolButton(parent);
    button->setToolTip(tooltip);
    button->setIcon(QIcon(iconPath));
    return button;
}

QToolButton* createToolButton(QWidget* parent, QAction* action, QString iconPath) {
    QToolButton* button = new QToolButton(parent);
    button->setDefaultAction(action);
    button->setIcon(QIcon(iconPath));
    return button;
}

QAction* action(QString text, QKeySequence shortcut) {
    QAction* action = new QAction(text);
    if (!shortcut.isEmpty()) {
        action->setShortcut(shortcut);
    }
    action->setToolTip(text);
    return action;
}

}

ImageViewer::ImageViewer(QWidget* parent) : QWidget(parent)
  , mViewer(nullptr)
  , mToolsWidget(nullptr)
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    ViewerHelper::disablePlatformSpecificMenuItems();

    mViewer = new Viewer(this);

    mInfo = new QLabel(this);
    mInfo->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Fixed);
    mInfo->setMaximumHeight(16);
    mInfo->setMinimumWidth(10);
    QFont infoFont = mInfo->font();
    infoFont.setPointSize(10);
    mInfo->setFont(infoFont);
    QVBoxLayout* viewerLayout = new QVBoxLayout;
    viewerLayout->addWidget(mViewer);
    viewerLayout->addWidget(mInfo);

    mToolsWidget = new QWidget(this);
    mToolsWidget->setMinimumWidth(34);
    mToolsWidget->setMaximumWidth(34);
    mToolsWidget->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding);
    QVBoxLayout* toolsLayout = new QVBoxLayout;
    toolsLayout->setSpacing(5);
    toolsLayout->setContentsMargins(2, 5, 2, 5);
    toolsLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    mToolsWidget->setLayout(toolsLayout);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->setMargin(2);
    layout->setSpacing(2);
    layout->addWidget(mToolsWidget);
    layout->addLayout(viewerLayout);
    setLayout(layout);

    createActions();
    createMenus();
}

void ImageViewer::openFileDialog()
{
    QString path = QFileDialog::getOpenFileName(this, "Open Image");
    if (!path.isEmpty()) {
        openFile(path);
    }
}

void ImageViewer::openFile(QString filename)
{
    QImageReader reader(filename);
    if (reader.canRead()) {
        QFileInfo fi(filename);
        setWindowTitle(fi.fileName());
        showImage(reader.read(), filename);
    }
}

void ImageViewer::showImage(QImage image, QString info)
{
    mViewer->setImage(image);
    mInfo->setText(QString("Image: %1 w:%2 h:%3 c:%4")
                   .arg(info)
                   .arg(image.width())
                   .arg(image.height())
                   .arg(image.bytesPerLine() / image.width()));
    mViewer->zoomToFit();
}

void ImageViewer::setMode(DrawMode m)
{
    mViewer->setDrawMode(m);
}

QSize ImageViewer::sizeHint() const
{
    return QSize(640, 480);
}

void ImageViewer::restoreWindowState()
{
    QSettings settings;

    QByteArray geometry = settings.value("ImageViewer/geometry_" + windowTitle(), QByteArray()).toByteArray();
    restoreGeometry(geometry);

    mToolsWidget->setVisible(settings.value("ImageViewer/tools_visible_" + windowTitle(), true).toBool());
    mInfo->setVisible(settings.value("ImageViewer/info_visible_" + windowTitle(), true).toBool());
}

void ImageViewer::saveWindowState() const
{
    QSettings settings;
    settings.setValue("ImageViewer/geometry_" + windowTitle(), saveGeometry());
    settings.setValue("ImageViewer/tools_visible_" + windowTitle(), mToolsWidget->isVisible());
    settings.setValue("ImageViewer/info_visible_" + windowTitle(), mInfo->isVisible());
}

void ImageViewer::showEvent(QShowEvent * event)
{
    QWidget::showEvent(event);
    restoreWindowState();
}

void ImageViewer::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
    saveWindowState();
}

void ImageViewer::createActions()
{
    mOpenAction = action("Open", QKeySequence::Open);
    connect(mOpenAction, &QAction::triggered, this, &ImageViewer::openFileDialog);
    mZoomInAction = action("Zoom in", QKeySequence::ZoomIn);
    connect(mZoomInAction, &QAction::triggered, mViewer, &Viewer::zoomIn);
    mZoomOutAction = action("Zoom out", QKeySequence::ZoomOut);
    connect(mZoomOutAction, &QAction::triggered, mViewer, &Viewer::zoomOut);
    mResetZoomAction = action("Reset zoom", QKeySequence("ctrl+0"));
    connect(mResetZoomAction, &QAction::triggered, mViewer, &Viewer::resetZoom);
    mZoomtoFitAction = action("Zoom to fit", QKeySequence());
    connect(mZoomtoFitAction, &QAction::triggered, mViewer, &Viewer::zoomToFit);

    mToogleToolsAction = action("Toggle Tools", QKeySequence("ctrl+t"));
    connect(mToogleToolsAction, &QAction::triggered, this, &ImageViewer::toogleTools);
    mToogleInfoAction = action("Toogle Info", QKeySequence("ctrl+i"));
    connect(mToogleInfoAction, &QAction::triggered, this, &ImageViewer::toogleInfo);
}

void ImageViewer::createMenus()
{
    QMenuBar* mb = new QMenuBar(this);

    auto file = mb->addMenu("File");
    file->addAction(mOpenAction);

    mb->addMenu("Edit");

    auto view = mb->addMenu("View");
    view->addAction(mToogleToolsAction);
    view->addAction(mToogleInfoAction);
    view->addSeparator();
    view->addAction(mZoomInAction);
    view->addAction(mZoomOutAction);
    view->addAction(mResetZoomAction);
    view->addAction(mZoomtoFitAction);

    QLayout* toolLayout = mToolsWidget->layout();
    QToolButton* moveButtom = createToolButton(mToolsWidget, "Move", ":/images/move.png");
    connect(moveButtom, &QAbstractButton::clicked, [=](){ mViewer->setCursor(Tools::move()->cursor()); });
    toolLayout->addWidget(moveButtom);
    QToolButton* rulerButtom = createToolButton(mToolsWidget, "Ruler", ":/images/ruler.png");
    rulerButtom->setVisible(false);
    toolLayout->addWidget(rulerButtom);

    toolLayout->addWidget(createToolButton(mToolsWidget, mZoomInAction, ":/images/zoom-in.png"));
    toolLayout->addWidget(createToolButton(mToolsWidget, mZoomOutAction, ":/images/zoom-out.png"));
    toolLayout->addWidget(createToolButton(mToolsWidget, mResetZoomAction, ":/images/reset-zoom.png"));
    toolLayout->addWidget(createToolButton(mToolsWidget, mZoomtoFitAction, ":/images/zoom-to-fit.png"));
}

void ImageViewer::toogleTools()
{
    mToolsWidget->setVisible(!mToolsWidget->isVisible());
}

void ImageViewer::toogleInfo()
{
    mInfo->setVisible(!mInfo->isVisible());
}


