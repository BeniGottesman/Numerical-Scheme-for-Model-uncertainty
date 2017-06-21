#include "drawwindow.h"
//#include "ui_drawwindow.h"

DrawWindow::DrawWindow(Graph3D *g3d,
                       //PayoffAlgorithm * abstractAlgo,
                       QWidget *parent) :
    QDialog(parent)
{
    Q3DSurface *graph  = g3d->getGraph();
    QWidget *container = QWidget::createWindowContainer(graph);

    if (!graph->hasContext())
    {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return;
    }

    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    vLayout->setAlignment(Qt::AlignTop);

    widget->setWindowTitle(QStringLiteral("Numerical scheme"));

    QGroupBox *selectionGroupBox = new QGroupBox(QStringLiteral("Selection Mode"));

    QRadioButton *modeNoneRB = new QRadioButton(widget);
    modeNoneRB->setText(QStringLiteral("No selection"));
    modeNoneRB->setChecked(false);

    QRadioButton *modeItemRB = new QRadioButton(widget);
    modeItemRB->setText(QStringLiteral("Item"));
    modeItemRB->setChecked(true);

    QRadioButton *modeSliceRowRB = new QRadioButton(widget);
    modeSliceRowRB->setText(QStringLiteral("Row Slice"));
    modeSliceRowRB->setChecked(false);

    QRadioButton *modeSliceColumnRB = new QRadioButton(widget);
    modeSliceColumnRB->setText(QStringLiteral("Column Slice"));
    modeSliceColumnRB->setChecked(false);

    QVBoxLayout *selectionVBox = new QVBoxLayout;
    selectionVBox->addWidget(modeNoneRB);
    selectionVBox->addWidget(modeItemRB);
    selectionVBox->addWidget(modeSliceRowRB);
    selectionVBox->addWidget(modeSliceColumnRB);
    selectionGroupBox->setLayout(selectionVBox);

    QSlider *axisMinSliderX = new QSlider(Qt::Horizontal, widget);
    axisMinSliderX->setMinimum(0);
    axisMinSliderX->setTickInterval(1);
    axisMinSliderX->setEnabled(true);
    QSlider *axisMaxSliderX = new QSlider(Qt::Horizontal, widget);
    axisMaxSliderX->setMinimum(1);
    axisMaxSliderX->setTickInterval(1);
    axisMaxSliderX->setEnabled(true);
    QSlider *axisMinSliderZ = new QSlider(Qt::Horizontal, widget);
    axisMinSliderZ->setMinimum(0);
    axisMinSliderZ->setTickInterval(1);
    axisMinSliderZ->setEnabled(true);
    QSlider *axisMaxSliderZ = new QSlider(Qt::Horizontal, widget);
    axisMaxSliderZ->setMinimum(1);
    axisMaxSliderZ->setTickInterval(1);
    axisMaxSliderZ->setEnabled(true);

    QComboBox *themeList = new QComboBox(widget);
    themeList->addItem(QStringLiteral("HUJI"));
    themeList->addItem(QStringLiteral("Primary Colors"));
    themeList->addItem(QStringLiteral("Digia"));
    themeList->addItem(QStringLiteral("Stone Moss"));
    themeList->addItem(QStringLiteral("Army Blue"));
    themeList->addItem(QStringLiteral("Retro"));
    themeList->addItem(QStringLiteral("Ebony"));
    themeList->addItem(QStringLiteral("Cherry"));

    QGroupBox *colorGroupBox = new QGroupBox(QStringLiteral("Custom gradient"));

    QLinearGradient grBtoY(0, 0, 1, 100);
    grBtoY.setColorAt(1.0, Qt::black);
    grBtoY.setColorAt(0.67, Qt::blue);
    grBtoY.setColorAt(0.33, Qt::red);
    grBtoY.setColorAt(0.0, Qt::yellow);
    QPixmap pm(24, 100);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(grBtoY));
    pmp.setPen(Qt::NoPen);
    pmp.drawRect(0, 0, 24, 100);
    QPushButton *gradientBtoYPB = new QPushButton(widget);
    gradientBtoYPB->setIcon(QIcon(pm));
    gradientBtoYPB->setIconSize(QSize(24, 100));

    QLinearGradient grGtoR(0, 0, 1, 100);
    grGtoR.setColorAt(1.0, Qt::darkGreen);
    grGtoR.setColorAt(0.5, Qt::yellow);
    grGtoR.setColorAt(0.2, Qt::red);
    grGtoR.setColorAt(0.0, Qt::darkRed);
    pmp.setBrush(QBrush(grGtoR));
    pmp.drawRect(0, 0, 24, 100);
    QPushButton *gradientGtoRPB = new QPushButton(widget);
    gradientGtoRPB->setIcon(QIcon(pm));
    gradientGtoRPB->setIconSize(QSize(24, 100));

    QHBoxLayout *colorHBox = new QHBoxLayout;
    colorHBox->addWidget(gradientBtoYPB);
    colorHBox->addWidget(gradientGtoRPB);
    colorGroupBox->setLayout(colorHBox);

    vLayout->addWidget(selectionGroupBox);
    vLayout->addWidget(new QLabel(QStringLiteral("Column range")));
    vLayout->addWidget(axisMinSliderX);
    vLayout->addWidget(axisMaxSliderX);
    vLayout->addWidget(new QLabel(QStringLiteral("Row range")));
    vLayout->addWidget(axisMinSliderZ);
    vLayout->addWidget(axisMaxSliderZ);
    vLayout->addWidget(new QLabel(QStringLiteral("Theme")));
    vLayout->addWidget(themeList);
    vLayout->addWidget(colorGroupBox);

    widget->show();

    //g3d->computeGraph(new Strategy (abstractAlgo));

    QObject::connect(modeNoneRB, &QRadioButton::toggled,
                     g3d, &Graph3D::toggleModeNone);
    QObject::connect(modeItemRB, &QRadioButton::toggled,
                     g3d, &Graph3D::toggleModeItem);
    QObject::connect(modeSliceRowRB, &QRadioButton::toggled,
                     g3d, &Graph3D::toggleModeSliceRow);
    QObject::connect(modeSliceColumnRB, &QRadioButton::toggled,
                     g3d, &Graph3D::toggleModeSliceColumn);
    QObject::connect(axisMinSliderX, &QSlider::valueChanged,
                     g3d, &Graph3D::adjustXMin);
    QObject::connect(axisMaxSliderX, &QSlider::valueChanged,
                     g3d, &Graph3D::adjustXMax);
    QObject::connect(axisMinSliderZ, &QSlider::valueChanged,
                     g3d, &Graph3D::adjustZMin);
    QObject::connect(axisMaxSliderZ, &QSlider::valueChanged,
                     g3d, &Graph3D::adjustZMax);
    QObject::connect(themeList, SIGNAL(currentIndexChanged(int)),
                     g3d, SLOT(changeTheme(int)));
    QObject::connect(gradientBtoYPB, &QPushButton::pressed,
                     g3d, &Graph3D::setBlackToYellowGradient);
    QObject::connect(gradientGtoRPB, &QPushButton::pressed,
                     g3d, &Graph3D::setGreenToRedGradient);


    g3d->setAxisMinSliderX(axisMinSliderX);
    g3d->setAxisMaxSliderX(axisMaxSliderX);
    g3d->setAxisMinSliderZ(axisMinSliderZ);
    g3d->setAxisMaxSliderZ(axisMaxSliderZ);

    g3d->initSliders ();

    themeList->setCurrentIndex(1);
}

DrawWindow::~DrawWindow()
{
    //delete ui;
}
