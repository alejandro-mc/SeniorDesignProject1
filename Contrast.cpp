// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Contrast.cpp - Brightness/Contrast widget.
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"
#include "Contrast.h"

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::Contrast:
//
// Constructor.
//
Contrast::Contrast(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::slider_to_contrast_map:
//
// Maps slider value to contrast value
//
double Contrast::map_slider_to_contrast(int slidervalue)
{
    return slidervalue / 50.0 + 2;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Contrast::applyFilter(ImagePtr I1, ImagePtr I2)
{
	// INSERT YOUR CODE HERE
    // error checking
    if(I1.isNull()) return 0;

    // apply filter
    double b, c;	                         // brightness, contrast parameters
        b = m_sliderB->value() << 1;         //get birghtness value
        c = map_slider_to_contrast(m_sliderC->value());   //get contrast value
        contrast(I1, b, c, I2);
	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::createGroupBox:
//
// Create group box for control panel.
//
QGroupBox*
Contrast::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Contrast");

	// INSERT YOUR CODE HERE
    // init widgets

    //BRIGHTNESS CONTROLS
    // create label[i]
    QLabel *labelB = new QLabel;
    labelB->setText(QString("Brightness"));

    // create slider
    m_sliderB = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderB->setTickPosition(QSlider::TicksBelow);
    m_sliderB->setTickInterval(10);
    m_sliderB->setMinimum(-100);
    m_sliderB->setMaximum(100);
    m_sliderB->setValue  (0);

    // create spinbox
    m_spinBoxB = new QSpinBox(m_ctrlGrp);
    m_spinBoxB->setMinimum(-100);
    m_spinBoxB->setMaximum(100);
    m_spinBoxB->setValue  (0);

    // init signal/slot connections for Threshold
    connect(m_sliderB , SIGNAL(valueChanged(int)), this, SLOT(changeBrightness (int)));
    connect(m_spinBoxB, SIGNAL(valueChanged(int)), this, SLOT(changeBrightness (int)));


    //CONTRAST CONTROLS
    // create label[i]
    QLabel *labelC = new QLabel;
    labelC->setText(QString("Contrast"));

    // create slider
    m_sliderC = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderC->setTickPosition(QSlider::TicksBelow);
    m_sliderC->setTickInterval(10);
    m_sliderC->setMinimum(-100);
    m_sliderC->setMaximum(100);
    m_sliderC->setValue  (0);

    // create spinbox
    m_spinBoxC = new QDoubleSpinBox(m_ctrlGrp);
    m_spinBoxC->setMinimum(0);
    m_spinBoxC->setMaximum(4.0);
    m_spinBoxC->setValue  (2.0);

    // init signal/slot connections for Threshold
    connect(m_sliderC , SIGNAL(valueChanged(int)), this, SLOT(changeContrast (int)));
    connect(m_spinBoxC, SIGNAL(valueChanged(int)), this, SLOT(changeContrast (int)));


    //REFERENCE CONTROLS
    // create label[i]
    QLabel *labelR = new QLabel;
    labelR->setText(QString("Reference"));

    // create slider
    m_sliderR = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderR->setTickPosition(QSlider::TicksBelow);
    m_sliderR->setTickInterval(10);
    m_sliderR->setMinimum(0);
    m_sliderR->setMaximum(255);
    m_sliderR->setValue  (128);

    // create spinbox
    m_spinBoxR = new QSpinBox(m_ctrlGrp);
    m_spinBoxR->setMinimum(0);
    m_spinBoxR->setMaximum(255);
    m_spinBoxR->setValue  (128);

    // init signal/slot connections for Threshold
    connect(m_sliderR , SIGNAL(valueChanged(int)), this, SLOT(changeReference (int)));
    connect(m_spinBoxR, SIGNAL(valueChanged(int)), this, SLOT(changeReference (int)));



    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(  labelB  , 0, 0);
    layout->addWidget(m_sliderB , 0, 1);
    layout->addWidget(m_spinBoxB, 0, 2);

    layout->addWidget(  labelC  , 1, 0);
    layout->addWidget(m_sliderC , 1, 1);
    layout->addWidget(m_spinBoxC, 1, 2);

    layout->addWidget(  labelR  , 2, 0);
    layout->addWidget(m_sliderR , 2, 1);
    layout->addWidget(m_spinBoxR, 2, 2);



    // assign layout to group box
    m_ctrlGrp->setLayout(layout);

	return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// contrast:
//
// INSERT YOUR CODE HERE.
//
void
Contrast::contrast(ImagePtr I1, double brightness, double contrast, ImagePtr I2)
{

    IP_copyImageHeader(I1, I2);

    int w = I1->width();
    int h = I1->height();
    int total = w * h;

    // compute lut[]
    int i, value, reference, lut[MXGRAY];
    reference = m_sliderR->value();

    for(i=0; i<MXGRAY; ++i){
        value = (i - reference) * contrast + reference + brightness;
        lut[i] = (int) CLIP(value,0,255);

    }


    int type;
    ChannelPtr<uchar> p1, p2, endd;
    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        for(endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
    }


}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::changeBrightness:
//
// Slot to change image birghtness.
//
void
Contrast::changeBrightness(int brightness)
{
    m_sliderB ->blockSignals(true);
    m_sliderB ->setValue    (brightness);
    m_sliderB ->blockSignals(false);
    m_spinBoxB->blockSignals(true);
    m_spinBoxB->setValue    (brightness);
    m_spinBoxB->blockSignals(false);

    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::changeContrast:
//
// Slot to change image contrast.
//
void
Contrast::changeContrast(int contrast)
{
    m_sliderC ->blockSignals(true);
    m_sliderC ->setValue    (contrast);
    m_sliderC ->blockSignals(false);
    m_spinBoxC->blockSignals(true);
    m_spinBoxC->setValue    (map_slider_to_contrast(contrast));
    m_spinBoxC->blockSignals(false);

    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::changeReference:
//
// Slot to change contrast reference value.
//
void
Contrast::changeReference(int refval)
{
    m_sliderR ->blockSignals(true);
    m_sliderR ->setValue    (refval);
    m_sliderR ->blockSignals(false);
    m_spinBoxR->blockSignals(true);
    m_spinBoxR->setValue    (refval);
    m_spinBoxR->blockSignals(false);

    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::reset:
//
// Reset parameters.
//
void
Contrast::reset() {}
