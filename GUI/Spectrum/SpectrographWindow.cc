#include "QtWidgets/QDoubleSpinBox"
#include "QtWidgets/QGridLayout"
#include "QtWidgets/QLabel"
#include "QtWidgets/QMenu"
#include "QtWidgets/QScrollArea"
#include "QtWidgets/QScrollBar"
#include "QtWidgets/QVBoxLayout"

#include "GUI/ChannelMenu.h"
#include "GUI/LogUtils.h"
#include "GUI/ToolBar.h"
#include "Utils/Utils.h"

#include "App.h"
#include "ColorMapWidget.h"
#include "Configuration.h"
#include "CursorWidget.h"
#include "FFTSettings.h"
#include "FreqScaleWidget.h"
#include "MainWindow.h"
#include "Settings.h"
#include "SpectrographImaging.h"
#include "SpectrographScrollArea.h"
#include "SpectrographWidget.h"
#include "SpectrographWindow.h"

using namespace SideCar::GUI::Spectrum;

Logger::Log&
SpectrographWindow::Log()
{
    static Logger::Log& log_ = Logger::Log::Find("spectrum.SpectrographWindow");
    return log_;
}

SpectrographWindow::SpectrographWindow(int shortcut) : Super(), Ui::SpectrographWindow()
{
    setupUi(this);
    setObjectName("SpectrographWindow");
    setWindowTitle("Spectrograph");
    makeShowAction(shortcut);

    QVBoxLayout* vboxLayout = new QVBoxLayout(centralwidget);
    SpectrographScrollArea* scrollArea = new SpectrographScrollArea(centralwidget);
    vboxLayout->addWidget(scrollArea);

    display_ = new SpectrographWidget(centralwidget);
    scrollArea->setWidget(display_);
    scrollArea->setWidgetResizable(true);

    connect(display_, &SpectrographWidget::currentCursorPosition, scrollArea,
            &SpectrographScrollArea::currentCursorPositionChanged);
    connect(scrollArea, &SpectrographScrollArea::currentCursorPosition, this,
            &SpectrographWindow::showCursorPosition);
    
    App* app = App::GetApp();
    Configuration* configuration = app->getConfiguration();

    actionViewPause_->setData(QStringList() << "Freeze" << "Unfreeze");
    actionViewPause_->setIcon(QIcon(":/unfreeze.png"));
    connect(actionViewPause_, &QAction::triggered, app, &App::updateToggleAction);

    Settings* settings = configuration->getSettings();
    ChannelSetting* channelSetting = settings->getInputChannel();
    connect(channelSetting, &StringSetting::valueChanged, this, &SpectrographWindow::channelChanged);
    if (channelSetting->isConnected()) {
        channelChanged(channelSetting->getValue());
    }

    // Cursor info widget in the status bar
    //
    cursorWidget_ = new CursorWidget(statusBar());
    statusBar()->addPermanentWidget(cursorWidget_);

    // Tool windows toolbar
    //
    ToolBar* toolBar = makeToolBar("Controls", Qt::TopToolBarArea);
    QAction* action = app->getToolsMenuAction(App::kShowConfigurationWindow);
    toolBar->addAction(action);
    toolBar->addAction(actionViewPause_);

    SpectrographImaging* imaging = configuration->getSpectrographImaging();

    // Create a toolbar to select the colormap to use
    //
#if 0
  toolBar = makeToolBar("Color Map Selection", Qt::TopToolBarArea);
  toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
  toolBar->addWidget(new QLabel("Mapping: ", toolBar));
  toolBar->addWidget(imaging->duplicateColorMapType(toolBar));
#endif

    // Create a toolbar to display the colormap
    //
    toolBar = makeToolBar("Color Map", Qt::TopToolBarArea);
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    ColorMapWidget* colorMapWidget = new ColorMapWidget(imaging->getMinCutoff(), imaging->getMaxCutoff(),
                                                        toolBar);
    toolBar->addWidget(colorMapWidget);

    connect(imaging, &SpectrographImaging::minCutoffChanged, colorMapWidget, &ColorMapWidget::setMinCutoff);
    connect(imaging, &SpectrographImaging::maxCutoffChanged, colorMapWidget, &ColorMapWidget::setMaxCutoff);
    connect(imaging, &SpectrographImaging::colorMapChanged, colorMapWidget, &ColorMapWidget::setColorMap);
    colorMapWidget->setColorMap(imaging->getColorMap());

    // Min/max cutoff toolbar
    //
    toolBar = makeToolBar("Min/Max Cutoffs", Qt::TopToolBarArea);
    toolBar->addWidget(new QLabel("Min Cutoff: ", toolBar));
    QDoubleSpinBox* widget = imaging->duplicateMinCutoff(toolBar);
    widget->setSuffix(" db");
    toolBar->addWidget(widget);
    toolBar->addWidget(new QLabel("Max Cutoff: ", toolBar));
    widget = imaging->duplicateMaxCutoff(toolBar);
    widget->setSuffix(" db");
    toolBar->addWidget(widget);
}

void
SpectrographWindow::showCursorPosition(const QString& x, const QString& y)
{
    cursorWidget_->showCursorPosition(QString("X: %1 Y: %2").arg(x).arg(y));
}

void
SpectrographWindow::on_actionShowMainWindow__triggered()
{
    App::GetApp()->getMainWindow()->showAndRaise();
}

void
SpectrographWindow::on_actionViewPause__triggered(bool state)
{
    display_->setFrozen(state);
}

void
SpectrographWindow::channelChanged(const QString& name)
{
    setWindowTitle(QString("Spectrograph: %1").arg(name));
}
