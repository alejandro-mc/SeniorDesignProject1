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
    m_isPixelBucketsInit = false;

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//HistMatch::initPixelBuckets:
void
HistMatch::initPixelBuckets()
{
    ImagePtr I1 = g_mainWindowP->imageSrc();
    int w = I1->width();
    int h = I1->height();
    unsigned long int total = w*h;

    pixel * tmptop;//will be used to hold top pixel temporarily wile new pixel is put in a bucket


    //initialize pixel buckets
    for(int i=0;i<MXGRAY;++i)
    {
        m_pixelBucketsR[i].toppixel = NULL;
        m_pixelBucketsG[i].toppixel = NULL;
        m_pixelBucketsB[i].toppixel = NULL;
    }

    int type;
    ChannelPtr<uchar> in, end;
    pixelbucket * pixelBuckets;
    for(int ch = 0; IP_getChannel(I1, ch, in, type); ch++) {
        end = in + total;

        if(ch < 2){

            if(ch < 1){
                pixelBuckets = m_pixelBucketsR;
            }else{
                pixelBuckets = m_pixelBucketsG;
            }

        }else{
            pixelBuckets = m_pixelBucketsB;
        }

        unsigned long int pixindex =0;
        while(in < end){//init source histogram

            //let's add the pixel to one of our buckets
            tmptop = pixelBuckets[*in].toppixel;
            pixelBuckets[*in].toppixel = new pixel;
            pixelBuckets[*in].toppixel->index = pixindex;
            pixelBuckets[*in].toppixel->nextpixel = tmptop;
            ++in;
            ++pixindex;

        }
    }



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
    int begining,end;
    if(m_checkBoxDecreasing->isChecked())
    {
        begining = 200;
        if(power == 0){
            end  = 200;
        }else{
            end  = 0;
        }

    }else{

        begining = 0;
        end      = 200;
    }

    float m = (end - begining) / pow(255,power);

    // error checking, maybe?


    /*create target histogram from parameters*/
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


    //create decreasing checkbox
    QLabel *labelDecreasing = new QLabel;
    labelDecreasing->setText(QString("Decreasing"));

    //create dither checkbox
    m_checkBoxDecreasing = new QCheckBox(m_ctrlGrp);
    m_checkBoxDecreasing->setChecked(false);


    // init signal/slot connections
    //Power
    connect(m_sliderPower , SIGNAL(valueChanged(int)), this, SLOT(changePower (int)));
    connect(m_spinBoxPower, SIGNAL(valueChanged(int)), this, SLOT(changePower (int)));

    //decreasing checkbox
    connect(m_checkBoxDecreasing,SIGNAL(stateChanged(int)),this,SLOT(handleDecreasing(int)));


	// assemble dialog
	QGridLayout *layout = new QGridLayout;
    layout->addWidget(  labelPower  ,       0,  0);
    layout->addWidget(m_sliderPower ,       0,  1);
    layout->addWidget(m_spinBoxPower,       0,  2);
    layout->addWidget(labelDecreasing,      1,  0);
    layout->addWidget(m_checkBoxDecreasing, 1,  1);


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
    m_internal = true;

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
// HistMatch::handleDecreasing:
//
// Slot to process handle checking and unchecking of decreasing checkbox
//
void
HistMatch::handleDecreasing(int checked)
{
    m_internal = true;


    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());


    // display output
    g_mainWindowP->displayOut();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistMatch::match:
//
//
//
void
HistMatch::match(ImagePtr I1,unsigned int *histtarget,ImagePtr I2)
{
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    float scale;
    unsigned int sumtarget,normHist[MXGRAY];
    unsigned long cumhisttarget[MXGRAY];


    if(! m_isPixelBucketsInit)
    {
        initPixelBuckets();
        m_isPixelBucketsInit = true;
    }

    if(!m_internal)
    {
          clearPixelBuckets();
          initPixelBuckets();
    }

    m_internal= false;




    int type;
    pixelbucket * pixelBuckets;
    ChannelPtr<uchar> in,out,start, endd;
    for(int ch = 0; IP_getChannel(I1, ch, in, type); ch++) {
        IP_getChannel(I2, ch, out, type);

        start = in;
        endd = in + total;

        if(ch < 2){

            if(ch < 1){
                pixelBuckets = m_pixelBucketsR;
            }else{
                pixelBuckets = m_pixelBucketsG;
            }

        }else{
            pixelBuckets = m_pixelBucketsB;
        }

        /*Create cummulative histogram for target*/
        //sumsrc = 0;
        sumtarget =0;
        for(int i = 0; i < MXGRAY; ++i){
            //sumsrc += histsrc[i];
            sumtarget += histtarget[i];

            //cumhistsrc[i] = sumsrc;
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


        /*generate output image*/
        int j=0;//j is the current normHist bucket
        pixel * currentpixel;
        //for each bucket in the pixel bucket
        for(int i=0;i<MXGRAY;++i)
        {
            //for each pixel in the bucket assign a value
            //according to the current normHist bucket
            if(pixelBuckets[i].toppixel != NULL)
            {
                currentpixel = pixelBuckets[i].toppixel;
                while(currentpixel->nextpixel != NULL)
                {
                    //look for next non empty bucket in normHist
                    while(normHist[j] < 1 && j < MXGRAY) j++;

                    out[currentpixel->index] = j;
                    normHist[j]--;
                    currentpixel = currentpixel->nextpixel;
                }

                //set the value of the last pixel in the bucket
                while(normHist[j] < 1 & j < MXGRAY) j++;
                out[currentpixel->index] = j;
                normHist[j]--;
            }

        }



    }

}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//HistMatch::clearPixelBuckets:
//
void
HistMatch::clearPixelBuckets()
{
    pixel * nextpixeltobecleared;
    pixel * pixeltobecleared;

    pixelbucket * pixelBuckets;
    //for every channel
    for(int ch=0;ch<3;ch++)
    {
        if(ch < 2){

            if(ch < 1){
                pixelBuckets = m_pixelBucketsR;
            }else{
                pixelBuckets = m_pixelBucketsG;
            }

        }else{
            pixelBuckets = m_pixelBucketsB;
        }

        //for each bucket
        for(int i=0;i<MXGRAY;i++)
        {
            //delete each pixel
            if(pixelBuckets[i].toppixel !=NULL)
            {
                pixeltobecleared = pixelBuckets[i].toppixel;
                nextpixeltobecleared = pixeltobecleared->nextpixel;
                while(nextpixeltobecleared !=NULL)
                {
                    delete pixeltobecleared;
                    pixeltobecleared = nextpixeltobecleared;
                    nextpixeltobecleared = pixeltobecleared->nextpixel;
                }
                delete pixeltobecleared;
            }
        }



    }

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistMatch::~HistMatch:
HistMatch::~HistMatch()
{
    clearPixelBuckets();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistStretch::reset:
//
// Reset parameters.
//
void
HistMatch::reset() {

    m_internal = true;

    m_sliderPower        ->  blockSignals(true);
    m_spinBoxPower       ->  blockSignals(true);
    m_checkBoxDecreasing ->  blockSignals(true);

    m_sliderPower        ->  setValue    (0 );
    m_spinBoxPower       ->  setValue    (0 );
    m_checkBoxDecreasing ->  setChecked(false);

    m_sliderPower        ->  blockSignals(false);
    m_spinBoxPower       ->  blockSignals(false);
    m_checkBoxDecreasing ->  blockSignals(false);


    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());


    // display output
    g_mainWindowP->displayOut();

}
