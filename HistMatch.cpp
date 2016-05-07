// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// HistMatch.cpp - HistMatch class
//
// Written by: George Wolberg, 2016
// Modified by: Alejandro Morejon Cortina, 2016
// ======================================================================

#include "MainWindow.h"
#include "HistMatch.h"
#include <iostream>

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistMatch::HistMatch:
//
// Constructor.
//
HistMatch::HistMatch(QWidget *parent) : ImageFilter(parent)
{
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistMatch::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
HistMatch::applyFilter(ImagePtr I1, ImagePtr I2)
{
	// error checking
	if(I1.isNull()) return 0;

	// get threshold value
    int pow = m_sliderPower->value();
    int begining = m_sliderBegining->value();
    int end = m_sliderEnd->value();

	// error checking


    /*cregate historgam*/



	// apply filter
    match(I1, min, max, I2);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistMatch::controlPanel:
//
// Create group box for control panel.
//
QGroupBox*
HistMatch::controlPanel()
{
	// init group box
    m_ctrlGrp = new QGroupBox("Histogram Stretching");

	// init widgets
	// create label[i]
    QLabel *labelPower = new QLabel;
    labelPower->setText(QString("Power"));

    QLabel *labelBegining = new QLabel;
    labelBegining->setText(QString("Begining"));

    QLabel *labelEnd = new QLabel;
    labelEnd->setText("End");

    // create power slider
    m_sliderPower = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderPower->setTickPosition(QSlider::TicksBelow);
    m_sliderPower->setTickInterval(1);
    m_sliderPower->setMinimum(0);
    m_sliderPower->setMaximum(5);
    m_sliderPower->setValue  (0);

    // create power spinbox
    m_spinBoxPower = new QSpinBox(m_ctrlGrp);
    m_spinBoxPower->setMinimum(0);
    m_spinBoxPower->setMaximum(5);
    m_spinBoxPower->setValue  (0);

    // create Begining slider
    m_sliderBegining = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderBegining->setTickPosition(QSlider::TicksBelow);
    m_sliderBegining->setTickInterval(1);
    m_sliderBegining->setMinimum(0);
    m_sliderBegining->setMaximum(200);
    m_sliderBegining->setValue  (200);

    // create End slider
    m_sliderEnd = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderEnd->setTickPosition(QSlider::TicksBelow);
    m_sliderEnd->setTickInterval(1);
    m_sliderEnd->setMinimum(0);
    m_sliderEnd->setMaximum(200);
    m_sliderEnd->setValue  (0);


    // init signal/slot connections
    //Power
    connect(m_sliderPower , SIGNAL(valueChanged(int)), this, SLOT(changePower (int)));
    connect(m_spinBoxPower, SIGNAL(valueChanged(int)), this, SLOT(changePower (int)));

    //Begining
    connect(m_sliderBegining , SIGNAL(valueChanged(int)), this, SLOT(changeBegining (int)));
    connect(m_spinBoxBegining, SIGNAL(valueChanged(int)), this, SLOT(changeBegining (int)));

    //End Slider
    connect(m_sliderEnd , SIGNAL(valueChanged(int)), this, SLOT(changeEnd (int)));
    connect(m_spinBoxEnd, SIGNAL(valueChanged(int)), this, SLOT(changeEnd (int)));



	// assemble dialog
	QGridLayout *layout = new QGridLayout;
    layout->addWidget(  labelPower  , 0, 0);
    layout->addWidget(m_sliderPower , 0, 1);
    layout->addWidget(m_spinBoxPower, 0, 2);

    layout->addWidget(  labelBegining  , 1, 0);
    layout->addWidget(m_sliderBegining , 1, 1);
    layout->addWidget(m_spinBoxBegining, 1, 2);

    layout->addWidget(  labelEnd  , 2, 0);
    layout->addWidget(m_sliderEnd , 2, 1);
    layout->addWidget(m_spinBoxEnd, 2, 2);


	// assign layout to group box
	m_ctrlGrp->setLayout(layout);

	return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistMatch::changePower:
//
// Slot to process change in thr caused by moving the slider.
//
void
HistMatch::changePower(int pow)
{

    m_sliderPower ->blockSignals(true);
    m_sliderPower ->setValue    (power );
    m_sliderPower ->blockSignals(false);

    m_spinBoxPower->blockSignals(true);
    m_spinBoxPower->setValue    (pow );
    m_spinBoxPower->blockSignals(false);

	// apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());


	// display output
	g_mainWindowP->displayOut();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistMatch::changeBegining:
//
// Slot to process change in begining slider and checkbox.
//
void
HistMatch::changeBegining(int begining)
{

    m_sliderBegining ->blockSignals(true);
    m_sliderBegining ->setValue    (begining );
    m_sliderBegining ->blockSignals(false);

    m_spinBoxBegining->blockSignals(true);
    m_spinBoxBegining->setValue    (begining );
    m_spinBoxBegining->blockSignals(false);

    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());


    // display output
    g_mainWindowP->displayOut();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistMatch::changeEnd:
//
// Slot to process change in End slider and checkbox.
//
void
HistMatch::changeEnd(int end)
{

    m_sliderEnd ->blockSignals(true);
    m_sliderEnd ->setValue    (end );
    m_sliderEnd ->blockSignals(false);

    m_spinBoxEnd->blockSignals(true);
    m_spinBoxEnd->setValue    (end );
    m_spinBoxEnd->blockSignals(false);

    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());


    // display output
    g_mainWindowP->displayOut();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistMatch::stretch:
//
// HistMatch I1 using the 2-level mapping shown below.  Output is in I2.
// val<thr: 0;	 val >= thr: MaxGray (255)
//! \brief	Match I1 using the 3-level mapping shown below.
//! \details	Output is in I2. val<t1: g1; t1<=val<t2: g2; t2<=val: g3
//! \param[in]	I1  - Input image.
//! \param[in]	min - minimum.
//! \param[in]  max - maximum.
//! \param[out]	I2  - Output image.
//
void
HistMatch::stretch(ImagePtr I1, int min, int max, ImagePtr I2) {
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


        for(endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];


	}
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistMatch::match:
//
// HistMatch I1 using the 2-level mapping shown below.  Output is in I2.
// val<thr: 0;	 val >= thr: MaxGray (255)
//! \brief	Match I1 using the 3-level mapping shown below.
//! \details	Output is in I2. val<t1: g1; t1<=val<t2: g2; t2<=val: g3
//! \param[in]	I1  - Input image.
//! \param[in]	histo - histogram to match.
//! \param[out]	I2  - Output image.
//
void
match(ImagePtr I1,unsigned int *histo,ImagePtr I2)
{
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    float scale;
    unsigned int sumsrc,sumtarget,histsrc[MXGRAY], histtarget[MXGRAY];
    unsigned long cumhistsrc[MXGRAY],cumhisttarget[MXGRAY];
    unsigned char lut[MXGRAY];

    int type;
    ChannelPtr<uchar> in,out,start, endd;
    for(int ch = 0; IP_getChannel(I1, ch, in, type); ch++) {
        IP_getChannel(I2, ch, out, type);

        start = in;
        endd = in + total;

        /*Build Histogram for source image*/
        for(int i = 0; i<MXGRAY;++i){ //clear historgrams
            histsrc[i] = histtarget[i] = 0;
            cumhistsrc[i] = cumhisttarget[i] = 0;
        }

        for(int i = 0; in < endd; ++i) histsrc[i] += *in++;             //init source histogram


        /*Create cummulative histogram for source and target*/
        sumsrc = 0;
        sumtarget =0;
        for(int i = 0; i < MXGRAY; ++i){
            cumhistsrc[i] = (sumsrc += histsrc[i]);
            cumhisttarget[i] = (sumtarget += histtarget[i]);
        }


        /*scale target cummulative histogram*/
        scale = total / (float) sumtarget;
        for(int i=0; i<MXGRAY; ++i) cumhisttarget[i] *= scale;

        /*initialize lookup table*/
        lut[0] = 0;//0 is always mapped to 0
        for(int i=1,j = 0; i<MXGRAY; ++i){// i itereates throug the src and j thourgh the target

            /*find and set target value for i*/
            while(cumhisttarget[j] < cumhistsrc[i]) ++j;//find interval to which cumhistsrc[i] belongs
            if((cumhisttarget[j] - cumhistsrc[i]) > (cumhistsrc[i] - cumhisttarget[j-1])){
                lut[i] = MIN(j-1,0);
            }else{
                lut[i] = j;
            }


        }

        /*generate output image*/
        for(in = start;in < endd;++in){
            *out = lut[*in];
            ++out;
        }



    }




}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistMatch::~HistMatch:
HistMatch::~HistMatch()
{
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistStretch::reset:
//
// Reset parameters.
//
void
HistMatch::reset() {}
