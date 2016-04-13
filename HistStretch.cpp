// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Threshold.cpp - Threshold class
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"
#include "HistStretch.h"
#include <iostream>

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::Threshold:
//
// Constructor.
//
HistStretch::HistStretch(QWidget *parent) : ImageFilter(parent)
{
    m_Hist = NULL;

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistStretch::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
HistStretch::applyFilter(ImagePtr I1, ImagePtr I2)
{
	// error checking
	if(I1.isNull()) return 0;

	// get threshold value
    int min = m_sliderMin->value();
    int max = m_sliderMax->value();

	// error checking
    if(min < 0 || min > MaxGray) return 0;
    if(max < 0 || max > MaxGray) return 0;

	// apply filter
    stretch(I1, min, max, I2);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::controlPanel:
//
// Create group box for control panel.
//
QGroupBox*
HistStretch::controlPanel()
{
	// init group box
    m_ctrlGrp = new QGroupBox("Histogram Stretching");

	// init widgets
	// create label[i]
    QLabel *labelMin = new QLabel;
    labelMin->setText(QString("Min"));

    QLabel *labelAutoMin = new QLabel;
    labelAutoMin->setText(QString("Auto"));

    QLabel *labelMax = new QLabel;
    labelMax->setText("Max");

    QLabel *labelAutoMax = new QLabel;
    labelAutoMax->setText(QString("Auto"));

    // create min slider
    m_sliderMin = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderMin->setTickPosition(QSlider::TicksBelow);
    m_sliderMin->setTickInterval(25);
    m_sliderMin->setMinimum(0);
    m_sliderMin->setMaximum(MaxGray);
    m_sliderMin->setValue  (0);

    // create min spinbox
    m_spinBoxMin = new QSpinBox(m_ctrlGrp);
    m_spinBoxMin->setMinimum(0);
    m_spinBoxMin->setMaximum(MaxGray);
    m_spinBoxMin->setValue  (0);

    //create min Auto check box
    m_checkBoxMin = new QCheckBox(m_ctrlGrp);
    m_checkBoxMin->setChecked(false);

    // create max slider
    m_sliderMax = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderMax->setTickPosition(QSlider::TicksBelow);
    m_sliderMax->setTickInterval(25);
    m_sliderMax->setMinimum(0);
    m_sliderMax->setMaximum(MaxGray);
    m_sliderMax->setValue  (MaxGray);

    // create max spinbox
    m_spinBoxMax = new QSpinBox(m_ctrlGrp);
    m_spinBoxMax->setMinimum(0);
    m_spinBoxMax->setMaximum(MaxGray);
    m_spinBoxMax->setValue  (MaxGray);

    //create max Auto check box
    m_checkBoxMax = new QCheckBox(m_ctrlGrp);
    m_checkBoxMax->setChecked(false);


    // init signal/slot connections
    //MIN CONNECTIONS
    connect(m_sliderMin , SIGNAL(valueChanged(int)), this, SLOT(changeMin (int)));
    connect(m_spinBoxMin, SIGNAL(valueChanged(int)), this, SLOT(changeMin (int)));
    connect(m_checkBoxMin,SIGNAL(stateChanged(int)),this, SLOT(minAuto(int)));

    //MAX CONNECTIONS
    connect(m_sliderMax , SIGNAL(valueChanged(int)), this, SLOT(changeMax (int)));
    connect(m_spinBoxMax, SIGNAL(valueChanged(int)), this, SLOT(changeMax (int)));
    connect(m_checkBoxMax,SIGNAL(stateChanged(int)),this, SLOT(maxAuto(int)));


	// assemble dialog
	QGridLayout *layout = new QGridLayout;
    layout->addWidget(  labelMin  , 0, 0);
    layout->addWidget(m_sliderMin , 0, 1);
    layout->addWidget(m_spinBoxMin, 0, 2);
    layout->addWidget(labelAutoMin, 0, 3);
    layout->addWidget(m_checkBoxMin,0, 4);

    layout->addWidget(  labelMax  , 1, 0);
    layout->addWidget(m_sliderMax , 1, 1);
    layout->addWidget(m_spinBoxMax, 1, 2);
    layout->addWidget(labelAutoMax, 1, 3);
    layout->addWidget(m_checkBoxMax,1, 4);

	// assign layout to group box
	m_ctrlGrp->setLayout(layout);

	return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::changeThr:
//
// Slot to process change in thr caused by moving the slider.
//
void
HistStretch::changeMin(int min)
{
    m_checkBoxMin->blockSignals(true);
    m_checkBoxMin->setChecked(false);
    m_checkBoxMin->blockSignals(false);

    m_sliderMin ->blockSignals(true);
    m_sliderMin ->setValue    (min );
    m_sliderMin ->blockSignals(false);

    m_spinBoxMin->blockSignals(true);
    m_spinBoxMin->setValue    (min );
    m_spinBoxMin->blockSignals(false);

	// apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());


	// display output
	g_mainWindowP->displayOut();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::changeThr:
//
// Slot to process change in thr caused by moving the slider.
//
void
HistStretch::changeMax(int max)
{
    m_checkBoxMax->blockSignals(true);
    m_checkBoxMax->setChecked(false);
    m_checkBoxMax->blockSignals(false);

    m_sliderMax ->blockSignals(true);
    m_sliderMax ->setValue    (max );
    m_sliderMax ->blockSignals(false);

    m_spinBoxMax->blockSignals(true);
    m_spinBoxMax->setValue    (max );
    m_spinBoxMax->blockSignals(false);

    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());


    // display output
    g_mainWindowP->displayOut();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistStretch::minAuto:
//
// Slot to process change in thr caused by moving the slider.
//
void HistStretch::minAuto(int a)
{

    if(m_checkBoxMin->isChecked()){
        //build histogram
        createHist();

        int i;

        //find min non zero bucket
        for(i=0;i<MXGRAY;++i)
        {
            if(m_Hist[i] != 0)
            {
                m_sliderMin->setValue(i);
                m_spinBoxMin->setValue(i);
                break;

            }
        }

        // apply filter to source image; save result in destination image
        applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());


        // display output
        g_mainWindowP->displayOut();
    }
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistStretch::maxAuto:
//
// Slot to process change in thr caused by moving the slider.
//
void HistStretch::maxAuto(int a)
{

    if(m_checkBoxMax->isChecked()){
        //build histo
        createHist();

        int i;


        //find min non zero bucket
        for(i=MaxGray;i>=0;--i)
        {
            if(m_Hist[i] != 0)
            {
                m_sliderMax->blockSignals(true);
                m_spinBoxMax->blockSignals(true);
                m_sliderMax->setValue(i);
                m_spinBoxMax->setValue(i);
                m_spinBoxMax->blockSignals(false);
                m_sliderMax->blockSignals(false);
                break;

            }
        }


        // apply filter to source image; save result in destination image
        applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());


        // display output
        g_mainWindowP->displayOut();
    }

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistStretch::createHist:
//
void HistStretch::createHist(){

    if(m_Hist == NULL){
        ImagePtr I1 = g_mainWindowP->imageSrc();

        m_Hist =  new int [MXGRAY];

        int w = I1->width();
        int h = I1->height();
        int total = w * h;


        int i;

        //init histogram
        for(i=0;i<MXGRAY;++i) m_Hist[i] =0;

        int type;
        ChannelPtr<uchar> p1, p2, endd;
        for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
            //IP_getChannel(I2, ch, p2, type);

            //std::cout << "This Works" << std::endl;
            for(endd = p1 + total; p1<endd;){
                m_Hist[*p1] += 1;
                ++p1;
            }


        }
    }
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistStretch::stretch:
//
// Threshold I1 using the 2-level mapping shown below.  Output is in I2.
// val<thr: 0;	 val >= thr: MaxGray (255)
//! \brief	Threshold I1 using the 3-level mapping shown below.
//! \details	Output is in I2. val<t1: g1; t1<=val<t2: g2; t2<=val: g3
//! \param[in]	I1  - Input image.
//! \param[in]	min - minimum.
//! \param[in]  max - maximum.
//! \param[out]	I2  - Output image.
//
void
HistStretch::stretch(ImagePtr I1, int min, int max, ImagePtr I2) {
    IP_copyImageHeader(I1, I2);
	int w = I1->width();
	int h = I1->height();
	int total = w * h;



	// compute lut[]
    int i,delta, lut[MXGRAY];
    //map values lower than min
    for(i=0;i<=min;++i) lut[i] = i; //function is the identity

    delta = max - min;

    //map values from min to max
    for(i=min;i<=max;++i)
        lut[i] = CLIP((int) (MaxGray * ( (double) (i - min) / (double) delta)),0,MaxGray);

    //map values greater than max
    for(i=max+1;i<MXGRAY;++i) lut[i] = MaxGray; //function is the identity

	int type;
	ChannelPtr<uchar> p1, p2, endd;
	for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);

        //std::cout << "This Works" << std::endl;
        for(endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];


	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistStretch::~HistStretch:
HistStretch::~HistStretch()
{
    if(m_Hist != NULL) delete [] m_Hist;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistStretch::reset:
//
// Reset parameters.
//
void
HistStretch::reset() {}
