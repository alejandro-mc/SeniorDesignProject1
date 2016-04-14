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
    m_maxFilterDim = 99;
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
    if(xsz < m_minFilterDim || xsz > m_maxFilterDim || ysz < m_minFilterDim
                            || ysz > m_maxFilterDim) return 0;

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
    m_sliderFilterX ->setValue  (m_minFilterDim);

    // create x spinbox
    m_spinBoxFilterX = new QSpinBox(m_ctrlGrp);
    m_spinBoxFilterX ->setMinimum(m_minFilterDim);
    m_spinBoxFilterX ->setSingleStep(2);
    m_spinBoxFilterX ->setMaximum(m_maxFilterDim);
    m_spinBoxFilterX ->setValue  (m_minFilterDim);

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
    if(!(xsz & 1)){//if ysz is even
        --xsz;
    }
    m_sliderFilterX     ->  blockSignals(true);
    m_sliderFilterX     ->  setValue    (xsz );
    m_sliderFilterX     ->  blockSignals(false);
    m_spinBoxFilterX    ->  blockSignals(true);
    m_spinBoxFilterX    ->  setValue    (xsz );
    m_spinBoxFilterX    ->  blockSignals(false);

    if(m_checkBoxSqr->isChecked())
    {
        m_sliderFilterY ->  setValue(xsz);
        m_sliderFilterY ->  setValue(xsz);
    }

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
Blur_Sharpen::changeFilterY(int ysz)
{

    if(!(ysz & 1)){//if ysz is even
        --ysz;
    }
    m_sliderFilterY     ->  blockSignals(true);
    m_sliderFilterY     ->  setValue    (ysz );
    m_spinBoxFilterY    ->  blockSignals(true);
    m_spinBoxFilterY    ->  setValue    (ysz );
    m_spinBoxFilterY    ->  blockSignals(false);
    m_sliderFilterY     ->  blockSignals(false);

    if(m_checkBoxSqr->isChecked())
    {
        m_sliderFilterX ->  setValue(ysz);
        m_sliderFilterX ->  setValue(ysz);
    }

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
    if(e)
    {
        m_sliderFilterY->setValue(m_sliderFilterX->value());
    }

    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}


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
