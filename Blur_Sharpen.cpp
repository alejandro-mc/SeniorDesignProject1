// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Threshold.cpp - Threshold class
//
// Written by: George Wolberg, 2016
// Modified by: Alejandro Morejon Cortina, 2016
// ======================================================================

#include "MainWindow.h"
#include "Blur_Sharpen.h"


extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur_Sharpen::Blur_Sharpen:
//
// Constructor.
//
Blur_Sharpen::Blur_Sharpen(QWidget *parent) : ImageFilter(parent)
{
    m_maxFilterDim = 11;
    m_minFilterDim = 1;
    //m_blurbuffer = new int[2];//initialize to junk
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur_Sharpen::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Blur_Sharpen::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // error checking
    if(I1.isNull()) return 0;

    // get threshold value
    int xsz = m_sliderFilterX->value();
    int ysz = m_sliderFilterY->value();

    // error checking
    if(xsz < m_minFilterDim || xsz > m_maxFilterDim || ysz < m_minFilterDim || ysz > m_maxFilterDim) return 0;

    // apply filter
    blur(I1,xsz,ysz,I2);

    return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur_Sharpen::createGroupBox:
//
// Create group box for control panel.
//
QGroupBox*
Blur_Sharpen::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Blur");

    // init widgets
    // create label x
    QLabel *xlabel = new QLabel;
    xlabel->setText(QString("X:"));

    // create x slider
    m_sliderFilterX = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderFilterX ->setTickPosition(QSlider::TicksBelow);
    m_sliderFilterX ->setTickInterval(2);
    m_sliderFilterX ->setSingleStep(2);
    m_sliderFilterX ->setMinimum(m_minFilterDim);
    m_sliderFilterX ->setMaximum(m_maxFilterDim);
    m_sliderFilterX ->setValue  (5);

    // create x spinbox
    m_spinBoxFilterX = new QSpinBox(m_ctrlGrp);
    m_spinBoxFilterX ->setMinimum(m_minFilterDim);
    m_spinBoxFilterX ->setSingleStep(2);
    m_spinBoxFilterX ->setMaximum(m_maxFilterDim);
    m_spinBoxFilterX ->setValue  (5);

    // create label y
    QLabel *ylabel = new QLabel;
    ylabel->setText(QString("Y:"));

    // create y slider
    m_sliderFilterY = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderFilterY ->setTickPosition(QSlider::TicksBelow);
    m_sliderFilterY ->setTickInterval(2);
    m_sliderFilterY ->setSingleStep(2);
    m_sliderFilterY ->setMinimum(m_minFilterDim);
    m_sliderFilterY ->setMaximum(m_maxFilterDim);
    m_sliderFilterY ->setValue  (m_minFilterDim);

    // create y spinbox
    m_spinBoxFilterY = new QSpinBox(m_ctrlGrp);
    m_spinBoxFilterY ->setMinimum(m_minFilterDim);
    m_spinBoxFilterY ->setSingleStep(2);
    m_spinBoxFilterY ->setMaximum(m_maxFilterDim);
    m_spinBoxFilterY ->setValue  (m_minFilterDim);



    //create square label
    QLabel *labelSquare = new QLabel;
    labelSquare->setText(QString("Square"));

    //create square checkbox
    m_checkBoxSqr = new QCheckBox(m_ctrlGrp);
    m_checkBoxSqr->setChecked(true);

    // init signal/slot connections for filter size controls
    connect(m_sliderFilterX , SIGNAL(valueChanged(int)), this, SLOT(changeFilterX (int)));
    connect(m_spinBoxFilterX, SIGNAL(valueChanged(int)), this, SLOT(changeFilterX (int)));

    connect(m_sliderFilterY , SIGNAL(valueChanged(int)), this, SLOT(changeFilterY (int)));
    connect(m_spinBoxFilterY , SIGNAL(valueChanged(int)), this, SLOT(changeFilterY (int)));

    connect(m_checkBoxSqr, SIGNAL(stateChanged(int)), this, SLOT(toggleSquare (int)));

    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(  xlabel          , 0, 0);
    layout->addWidget(m_sliderFilterX   , 0, 1);
    layout->addWidget(m_spinBoxFilterX  , 0, 2);

    layout->addWidget(  ylabel          , 1, 0);
    layout->addWidget(m_sliderFilterY   , 1, 1);
    layout->addWidget(m_spinBoxFilterY  , 1, 2);

    layout->addWidget(labelSquare       ,2,0);
    layout->addWidget(m_checkBoxSqr     ,2,1);

    // assign layout to group box
    m_ctrlGrp->setLayout(layout);

    return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur_Sharpen::changeFilterX:
//
// Slot to process change in filter width.
//
void
Blur_Sharpen::changeFilterX(int xsz)
{
    m_sliderFilterX     ->  blockSignals(true);
    m_sliderFilterX     ->  setValue    (xsz );
    m_sliderFilterX     ->  blockSignals(false);
    m_spinBoxFilterX    ->  blockSignals(true);
    m_spinBoxFilterX    ->  setValue    (xsz );
    m_spinBoxFilterX    ->  blockSignals(false);

    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur_Sharpen::changeFilterY:
//
// Slot to process change in filter height.
//
void
Blur_Sharpen::changeFilterY(int xsz)
{
    m_sliderFilterY     ->  blockSignals(true);
    m_sliderFilterY     ->  setValue    (xsz );
    m_sliderFilterY     ->  blockSignals(false);
    m_spinBoxFilterY    ->  blockSignals(true);
    m_spinBoxFilterY    ->  setValue    (xsz );
    m_spinBoxFilterY    ->  blockSignals(false);

    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur_Sharpen::toggleSquare:
//
//
void Blur_Sharpen::toggleSquare(int e)
{
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur_Sharpen::toggleSharpen:
//
//
//void Blur_Sharpen::toggleSharpen(int e)
//{
    // apply filter to source image; save result in destination image
//    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
//    g_mainWindowP->displayOut();
//}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur_Sharpen::blur:
//
// Performs a linear blur
//
//! \brief	Threshold I1 using the 3-level mapping shown below.
//! \details	Output is in I2. val<t1: g1; t1<=val<t2: g2; t2<=val: g3
//! \param[in]	in  - Input image.
//! \param[in]	xsz - Filter width.
//! \param[in]	ysz - Filter height.
//! \param[out]	out  - Output image.
//
void Blur_Sharpen::blur(ImagePtr in, int xsz, int ysz, ImagePtr out){


    IP_copyImageHeader(in,out);
    int width = in->width();
    int height = in->height();
    int total = width * height;

    int type;
    ChannelPtr<uchar> p1, p3,tmpPtr,rowend, endd;
    int* p2 = new int[total];//temp image ptr
    int * p2reset = p2;
    for(int ch = 0; IP_getChannel(in, ch, p1, type); ch++) {
        IP_getChannel(out,ch,p3,type);
        p2 = p2reset;
        endd = p1+total;


        //BLUR ROWS///////////////////////////////////////////////////////////////////////////

        ///create padded buffer
        m_blurbuffer = new int[width + xsz -1];
        int* leftside = m_blurbuffer + ((xsz - 1) >>1);
        int* bufferend = m_blurbuffer + width + xsz -1;

        int * sadd;
        int * ssub;
        int* b;

        ////for each row in input image
        tmpPtr = p1;

        while(tmpPtr < endd)
        {
            //b is the buffer pointer
            /////create left pad
            b = m_blurbuffer;

            while(b < leftside)
            {
                *b = *tmpPtr;
                ++b;
            }


            /////copy image row to buffer
            rowend = tmpPtr + width;
            while(tmpPtr < rowend)
            {
                *b = *tmpPtr;
                b++;
                tmpPtr++;
            }

            /////create right pad
            tmpPtr--;//set pointer back to right end of row
            while(b < bufferend)
            {
                *b = *tmpPtr;
                b++;
            }
            //tmpPtr++;


            /////copy blured pixels to tmpImg
            int sum=0;



            //add fisrt neighborhood
            ssub = m_blurbuffer;
            sadd = m_blurbuffer;

            for(;sadd < xsz + m_blurbuffer; sadd++){

                sum += *sadd;//
            }

            *p2 = (int) (sum / (float) xsz*ysz);//blur first fixel of the row
            p2++;

            //copy averaged pixel values to tmpImg
            for(;sadd < bufferend; ++sadd)
            {
                sum = sum - *ssub + *sadd;
                *p2 = (int) (sum / (float) xsz*ysz);
                ssub++;//next element in the buffer
                p2++;//next pixel
            }

            tmpPtr++;//go to the next row
        }

        delete [] m_blurbuffer;
        //END BLUR ROWS/////////////////////////////////////////////////////////////////////////////


        //BLUR COLUMNS////////////////////////////////////////////////////////////////////////


        //END BLUR COLUMNS ////////////////////////////////////////////////////////////////////////




        //for now just copy the pixel to the image
        p2 = p2reset;//reset p2



        while(p2<p2reset+total)
        {
            *p3 = CLIP(*p2,0,255);
            p2++;
            p3++;
        }




    }

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur_Sharpen::sharpen:
//
//
//void Blur_Sharpen::sharpen(ImagePtr in, int sz, ImagePtr out){


//}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur_Sharpen::reset:
//
// Reset parameters.
//
void
Blur_Sharpen::reset() {}
