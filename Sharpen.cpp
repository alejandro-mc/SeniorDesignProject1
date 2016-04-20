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
#include "Sharpen.h"


extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sharpen::Sharpen:
//
// Constructor.
//
Sharpen::Sharpen(QWidget *parent) : ImageFilter(parent)
{
    m_maxKernelSize = 99;
    m_minKernelSize = 1;

    m_minFactor=1;
    m_maxFactor=5;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sharpen::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Sharpen::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // error checking
    if(I1.isNull()) return 0;

    // get threshold value
    int sz = m_sliderKernelSize->value();
    double fctr = m_sliderFactor->value();

    // error checking
    if(sz < m_minKernelSize || sz > m_maxKernelSize) return 0;

    // apply filter
    sharpen(I1,sz,fctr,I2);

    return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sharpen::createGroupBox:
//
// Create group box for control panel.
//
QGroupBox*
Sharpen::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Blur");

    // init widgets
    // create label x
    QLabel *ksizelabel = new QLabel;
    ksizelabel->setText(QString("Filter Size:"));

    // create filter size slider
    m_sliderKernelSize = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderKernelSize ->setTickPosition(QSlider::TicksBelow);
    m_sliderKernelSize ->setTickInterval(2);
    m_sliderKernelSize ->setSingleStep(2);
    m_sliderKernelSize ->setMinimum(m_minKernelSize);
    m_sliderKernelSize ->setMaximum(m_maxKernelSize);
    m_sliderKernelSize ->setValue  (m_minKernelSize);

    // create filter size spinbox
    m_spinBoxKernelSize = new QSpinBox(m_ctrlGrp);
    m_spinBoxKernelSize ->setMinimum(m_minKernelSize);
    m_spinBoxKernelSize ->setSingleStep(2);
    m_spinBoxKernelSize ->setMaximum(m_maxKernelSize);
    m_spinBoxKernelSize ->setValue  (m_minKernelSize);

    // create factor label
    QLabel *factorlabel = new QLabel;
    factorlabel->setText(QString("Factor:"));

    // create factor slider
    m_sliderFactor = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderFactor ->setTickPosition(QSlider::TicksBelow);
    m_sliderFactor ->setTickInterval(2);
    m_sliderFactor ->setSingleStep(2);
    m_sliderFactor ->setMinimum(m_minFactor);
    m_sliderFactor ->setMaximum(m_maxFactor);
    m_sliderFactor ->setValue  (m_minFactor);

    // create factor spinbox
    m_spinBoxFactor = new QSpinBox(m_ctrlGrp);
    m_spinBoxFactor ->setMinimum(m_minFactor);
    m_spinBoxFactor ->setSingleStep(2);
    m_spinBoxFactor ->setMaximum(m_maxFactor);
    m_spinBoxFactor ->setValue  (m_minFactor);


    // init signal/slot connections for filter size controls
    connect(m_sliderKernelSize , SIGNAL(valueChanged(int)), this, SLOT(changeKernelSize(int)));
    connect(m_spinBoxKernelSize, SIGNAL(valueChanged(int)), this, SLOT(changeKernelSize(int)));

    connect(m_sliderFactor , SIGNAL(valueChanged(int)), this, SLOT(changeFactor(int)));
    connect(m_spinBoxFactor , SIGNAL(valueChanged(int)), this, SLOT(changeFactor(int)));

    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(  ksizelabel          , 0, 0);
    layout->addWidget(m_sliderKernelSize   , 0, 1);
    layout->addWidget(m_spinBoxKernelSize  , 0, 2);

    layout->addWidget(  factorlabel          , 1, 0);
    layout->addWidget(m_sliderFactor   , 1, 1);
    layout->addWidget(m_spinBoxFactor  , 1, 2);


    // assign layout to group box
    m_ctrlGrp->setLayout(layout);

    return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sharpen::changeFilterX:
//
// Slot to process change in filter width.
//
void
Sharpen::changeKernelSize(int sz)
{
    if(!(sz & 1)){//if ysz is even
        --sz;
    }
    m_sliderKernelSize     ->  blockSignals(true);
    m_sliderKernelSize     ->  setValue    (sz );
    m_sliderKernelSize     ->  blockSignals(false);
    m_spinBoxKernelSize    ->  blockSignals(true);
    m_spinBoxKernelSize    ->  setValue    (sz );
    m_spinBoxKernelSize    ->  blockSignals(false);

    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sharpen::changeFilterY:
//
// Slot to process change in filter height.
//
void
Sharpen::changeFactor(int fctr)
{

    m_sliderFactor     ->  blockSignals(true);
    m_sliderFactor     ->  setValue    (fctr );
    m_sliderFactor     ->  blockSignals(false);
    m_spinBoxFactor    ->  blockSignals(true);
    m_spinBoxFactor    ->  setValue    (fctr );
    m_spinBoxFactor    ->  blockSignals(false);


    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sharpen::blur:
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
void Sharpen::blur(ImagePtr in, int xsz, int ysz, ImagePtr out){


    IP_copyImageHeader(in,out);
    int width = in->width();
    int height = in->height();
    int total = width * height;
    int neighborhood_size = xsz*ysz;

    int * tmpImg = new int[total];//temporary image to hold the horizontal pass values
    int * tmpImgStart = tmpImg;//beginig address of temp image
    int * tmpImgrowend;//points to the end of the current tmpImg row
    int * tmpImgColptr;//points to the current column current column of the tmpImg being
                       //being procesed during vertical pass

    int * tmpImgEnd = tmpImgStart + total;//points to 1 int past the end of tmpImg

    int type;

    //p1: is the pointer to the current channel in the input image
    //p3: is the ponter to the current channel in the output image
    //p3colptr: int the current channel points to the columns of the output image
    //          during vertical pass when averaging for pixels in the tmpImg
    //          is finished and the result copied to the output image
    //rowend: for each channel marks the end of the current row in the input image
    //endd: for each channel marks the end of the channel in the input image
    ChannelPtr<uchar> p1, p3,p3colptr,rowend, endd;
    for(int ch = 0; IP_getChannel(in, ch, p1, type); ch++) {
        IP_getChannel(out,ch,p3,type);
        endd = p1+total;


        //BLUR ROWS///////////////////////////////////////////////////////////////////////////

        ///create padded buffer
        m_blurbuffer = new int[width + xsz -1];
        int* leftside = m_blurbuffer + ((xsz - 1) >>1);
        int* bufferend = m_blurbuffer + width + xsz -1;

        int * sadd;//will point to pixel to be added to the sum
        int * ssub;//will point to pixel to be subtracted from the sum
        int* b;//will point to buffer locations
        int sum;//will hold the sum of the neighborhood pixels

        ////for each row in input image
        while(p1 < endd)
        {
            //b is the buffer pointer
            /////create left pad
            b = m_blurbuffer;

            while(b < leftside)
            {
                *b = *p1;
                ++b;
            }


            /////copy image row to buffer
            rowend = p1 + width;
            while(p1 < rowend)
            {
                *b = *p1;
                b++;
                p1++;
            }

            /////create right pad
            p1--;//set pointer back to right end of row
            while(b < bufferend)
            {
                *b = *p1;
                b++;
            }


            /////copy blured pixels to tmpImg
            sum=0;



            //add fisrt neighborhood
            ssub = m_blurbuffer;
            sadd = m_blurbuffer;

            for(;sadd < xsz + m_blurbuffer; sadd++){

                sum += *sadd;//
            }

            *tmpImg = sum;//blur first fixel of the row
            tmpImg++;

            //copy averaged pixel values to tmpImg
            for(;sadd < bufferend; ++sadd)//
            {
                sum = sum - *ssub + *sadd;
                *tmpImg = sum;
                ssub++;//next element in the buffer
                tmpImg++;//next pixel
            }

            p1++;//go to the next row
        }

        delete [] m_blurbuffer;
        //END BLUR ROWS/////////////////////////////////////////////////////////////////////////////

        tmpImg = tmpImgStart;//reset tmpImg pointer

        //BLUR COLUMNS////////////////////////////////////////////////////////////////////////

        ///create padded buffer
        m_blurbuffer = new int[height + ysz -1];
        leftside = m_blurbuffer + ((ysz - 1) >>1);
        bufferend = m_blurbuffer + height + ysz -1;


        ////for each column in output image
        tmpImgrowend = tmpImgStart + width;//rowend marks the end of the first row of the image channel
        tmpImgColptr = tmpImgStart;//tmpPtr points to the top of a column, in this case to the first column
        p3colptr = p3;
        while(tmpImgColptr < tmpImgrowend)
        {
            //b is the buffer pointer
            /////create left pad
            b = m_blurbuffer;
            tmpImg = tmpImgColptr;//put tmpImg at the beggining of the column
            while(b < leftside)
            {
                *b = *tmpImg;
                ++b;
            }


            /////copy image column to buffer
            
            while(tmpImg < tmpImgEnd)//when p3 is more than endd it is past the last element of the column.
            {
                *b = *tmpImg;
                b++;
                tmpImg+= width;//moves new image channel pointer p3 to next pixel in the current column
            }


            /////create right pad
            tmpImg-= width;//set channel pointer back to the bottom of the column
            while(b < bufferend)
            {
                *b = *tmpImg;
                b++;
            }


            /////copy blured pixels back to image
            tmpImg = tmpImgColptr;//reset pointer to begining of column to copy blured pixel from buffer to the column
            p3 = p3colptr;
            sum=0;

            //add fisrt neighborhood
            ssub = m_blurbuffer;
            sadd = m_blurbuffer;

            for(;sadd < ysz + m_blurbuffer; sadd++){

                sum += *sadd;//
            }

            //*tmpImg = sum;//blur first fixel of the row
            *p3= sum / neighborhood_size;
            p3+=width;//advance the
            tmpImg+=width;

            //copy averaged pixel values to image
            for(;sadd < bufferend; ++sadd)
            {
                sum = sum - *ssub + *sadd;
                //*tmpImg = sum;
                *p3 = sum / neighborhood_size;
                ssub++;//next element in the buffer
                tmpImg+=width;//next pixel
                p3+=width;
            }

            tmpImgColptr++;//go to the next column in the temp image
            p3colptr++;//got to the next column in the output image
        }

        delete [] m_blurbuffer;

        //END BLUR COLUMNS ////////////////////////////////////////////////////////////////////////


        tmpImg = tmpImgStart; //reset tmpImg pointer

    }




    delete [] tmpImg;

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sharpen::sharpen:
//
//
void Sharpen::sharpen(ImagePtr in, int sz,double fctr, ImagePtr out){


    ImagePtr tmp;

    IP_copyImageHeader(in,out);
    IP_copyImageHeader(in,tmp);

    int width = in->width();
    int height = in->height();
    int total = width * height;

    blur(in,sz,sz,tmp);

    int type;
    ChannelPtr<uchar> p1,p2,p3,endd;
    for(int ch = 0; IP_getChannel(in, ch, p1, type); ch++) {
        IP_getChannel(out,ch,p3,type);
        IP_getChannel(tmp,ch,p2,type);
        endd = p1+total;


        //subtract and add
        for(;p1<endd;++p1)
        {
            *p3 = CLIP(*p1 + CLIP(*p1 - *p2,0,MaxGray) * fctr,0,MaxGray);
            ++p2;
            ++p3;
        }
    }



}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur_Sharpen::reset:
//
// Reset parameters.
//
void
Sharpen::reset() {}
