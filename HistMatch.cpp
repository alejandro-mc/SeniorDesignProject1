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
    int power = m_sliderPower->value();
    int begining = m_sliderBegining->value();
    int end = m_sliderEnd->value();
    float m = (end - begining) / pow(255,power);

    // error checking, maybe?


    /*cregate historgam*/
    unsigned int tmpHist[MXGRAY];

    //clear histogram
    for(int i=0;i<MXGRAY;++i) tmpHist[i] = 0;
    for(int i=0;i<MXGRAY;++i) tmpHist[i] = (unsigned int) (m * pow(i,power) + begining);


	// apply filter
    match(I1,tmpHist, I2);

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
    m_ctrlGrp = new QGroupBox("Histogram Matching");

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
    m_sliderPower->setValue  (1);

    // create power spinbox
    m_spinBoxPower = new QSpinBox(m_ctrlGrp);
    m_spinBoxPower->setMinimum(0);
    m_spinBoxPower->setMaximum(5);
    m_spinBoxPower->setValue  (0);

    // create Begining slider
    m_sliderBegining = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderBegining->setTickPosition(QSlider::TicksBelow);
    m_sliderBegining->setTickInterval(10);
    m_sliderBegining->setMinimum(0);
    m_sliderBegining->setMaximum(200);
    m_sliderBegining->setValue  (200);

    // create begining spinbox
    m_spinBoxBegining = new QSpinBox(m_ctrlGrp);
    m_spinBoxBegining->setMinimum(0);
    m_spinBoxBegining->setMaximum(200);
    m_spinBoxBegining->setValue  (200);

    // create End slider
    m_sliderEnd = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderEnd->setTickPosition(QSlider::TicksBelow);
    m_sliderEnd->setTickInterval(10);
    m_sliderEnd->setMinimum(0);
    m_sliderEnd->setMaximum(200);
    m_sliderEnd->setValue  (200);

    // create begining spinbox
    m_spinBoxEnd = new QSpinBox(m_ctrlGrp);
    m_spinBoxEnd->setMinimum(0);
    m_spinBoxEnd->setMaximum(200);
    m_spinBoxEnd->setValue  (200);


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
HistMatch::changePower(int power)
{

    m_sliderPower ->blockSignals(true);
    m_sliderPower ->setValue    (power );
    m_sliderPower ->blockSignals(false);

    m_spinBoxPower->blockSignals(true);
    m_spinBoxPower->setValue    (power );
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
// HistMatch::match:
//
// HistMatch I1 using the 2-level mapping shown below.  Output is in I2.
//
void
HistMatch::match(ImagePtr I1,unsigned int *histtarget,ImagePtr I2)
{
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    float scale;
    unsigned int sumsrc,sumtarget,histsrc[MXGRAY],normHist[MXGRAY];;
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
            histsrc[i] = 0;
            cumhistsrc[i] = cumhisttarget[i] = 0;
        }

        while(in < endd){
            histsrc[*in] ++;
            ++in;
        }             //init source histogram


        /*Create cummulative histogram for source and target*/
        sumsrc = 0;
        sumtarget =0;
        for(int i = 0; i < MXGRAY; ++i){
            sumsrc += histsrc[i];
            sumtarget += histtarget[i];

            cumhistsrc[i] = sumsrc;
            cumhisttarget[i] = sumtarget;
        }


        /*scale target cummulative histogram*/
        scale = total / (double) sumtarget;
        cumhisttarget[0] = round(cumhisttarget[0] * scale);//normalize cummulative histogram
        normHist[0] = cumhisttarget[0];
        for(int i=1; i<MXGRAY; ++i){
            cumhisttarget[i] = round(cumhisttarget[i] * scale);//normalize cummulative histogram
            normHist[i] = cumhisttarget[i] - cumhisttarget[i-1];//init normHist; will be needed to generate out pixels
        }

        /*initialize lookup table*/
        for(int i=0,j = 1; i<MXGRAY; ++i){// i itereates throug the src and j through the target

            /*find and set target value for i*/
            while(cumhisttarget[j] < cumhistsrc[i]) ++j;//find interval to which cumhistsrc[i] belongs
            if(abs(cumhisttarget[j] - cumhistsrc[i]) > abs(cumhistsrc[i] - cumhisttarget[j-1])){
                lut[i] = j-1;
            }else{
                lut[i] = j;
            }


        }

        /*generate output image*/
        int i;
        for(in = start;in < endd;++in){
            /*look for the next spot to the left of lut to put the pixel*/
            i=lut[*in];
            while(normHist[i] < 1 && i > -1) i--;//normHist holds the number of pixels available for each bucket
                                                 //so normHist[i] < 1 means there is
                                                 //no room left for a new pixel in that bucket


            /*if all buckets to the left are full put the pixel in some bucket to the right*/
            if(i == -1){//if there were no buckets available to the left then i == -1
                i = lut[*in] + 1;

                while(normHist[i] < 1 && i < MXGRAY) i++;
            }

            /*set output pixel*/
            //at this point we have 0 <= i <= 256
            *out = MIN(i,MaxGray);//MaxGray is 255, this clips the values so if no suitable bucket is found
                                  //it will finally be put in the 255 bucket
            normHist[*out] --;//decrement historgram bucket
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
