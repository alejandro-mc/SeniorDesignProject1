// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Threshold.cpp - Threshold class
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"
#include "Quantize.h"


extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::Threshold:
//
// Constructor.
//
Quantize::Quantize(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Quantize::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // error checking
    if(I1.isNull()) return 0;

    // get threshold value
    int levels = m_slider->value();

    // error checking
    if(levels < 0 || levels > MXGRAY) return 0;

    // apply filter
    quantize(I1, levels, I2);

    return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::createGroupBox:
//
// Create group box for control panel.
//
QGroupBox*
Quantize::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Quantize");

    // init widgets
    // create label[i]
    QLabel *label = new QLabel;
    label->setText(QString("Leves"));

    // create slider
    m_slider = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_slider->setTickPosition(QSlider::TicksBelow);
    m_slider->setTickInterval(25);
    m_slider->setMinimum(1);
    m_slider->setMaximum(MXGRAY);
    m_slider->setValue  (1);

    // create spinbox
    m_spinBox = new QSpinBox(m_ctrlGrp);
    m_spinBox->setMinimum(1);
    m_spinBox->setMaximum(MXGRAY);
    m_spinBox->setValue  (1);

    //create dither label
    QLabel *labelDither = new QLabel;
    labelDither->setText(QString("Dither"));

    //create dither checkbox
    m_checkBox = new QCheckBox(m_ctrlGrp);
    m_checkBox->setChecked(false);

    // init signal/slot connections for Threshold
    connect(m_slider , SIGNAL(valueChanged(int)), this, SLOT(changeLevels (int)));
    connect(m_spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeLevels (int)));
    connect(m_checkBox, SIGNAL(stateChanged(int)), this, SLOT(toggleDither (int)));

    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(  label  , 0, 0);
    layout->addWidget(m_slider , 0, 1);
    layout->addWidget(m_spinBox, 0, 2);
    layout->addWidget(labelDither,1,0);
    layout->addWidget(m_checkBox,1,1);

    // assign layout to group box
    m_ctrlGrp->setLayout(layout);

    return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Quantize::changeLevels:
//
// Slot to process change in levels caused by moving the slider.
//
void
Quantize::changeLevels(int levels)
{
    m_slider ->blockSignals(true);
    m_slider ->setValue    (levels );
    m_slider ->blockSignals(false);
    m_spinBox->blockSignals(true);
    m_spinBox->setValue    (levels );
    m_spinBox->blockSignals(false);

    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Quantize::changeLevels:
//
// Slot to process change in levels caused by moving the slider.
//
void Quantize::toggleDither(int e)
{
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::threshold:
//
// Threshold I1 using the 2-level mapping shown below.  Output is in I2.
// val<thr: 0;	 val >= thr: MaxGray (255)
//! \brief	Threshold I1 using the 3-level mapping shown below.
//! \details	Output is in I2. val<t1: g1; t1<=val<t2: g2; t2<=val: g3
//! \param[in]	I1  - Input image.
//! \param[in]	thr - Threshold.
//! \param[out]	I2  - Output image.
//
void
Quantize::quantize(ImagePtr I1, int levels, ImagePtr I2) {
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w * h;

    // compute lut[]
    int i,j, lut[MXGRAY];
    int scale = MXGRAY / levels;
    for (i=0; i< MXGRAY ; ++i)
    {
        lut[i] = scale * (int) (i/ scale);
    }

    int type, dithered, delta;
    ChannelPtr<uchar> p1, p2, endd;

    if(m_checkBox->isChecked())
    {
        //add dither
        for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
            IP_getChannel(I2, ch, p2, type);

            //first do the even rows
            i=0;
            while(i < total)
            {
                //apply dither to row
                for(j=0;j<w;++j)
                {
                    delta = (qrand() & 0xf);//random int from 0 to 16
                    if(j & 1)
                    {
                        dithered = CLIP(*(p1 + i + j) + delta,0,MaxGray);
                    }else{
                        dithered = CLIP(*(p1 + i + j) - delta,0,MaxGray);
                    }

                    *(p2 + i +j) = lut[dithered];
                }

                //skip next row
                i = i + w + w;
            }

            //then do the odd rows
            i= w;
            while(i < total)
            {
                //apply dither to row
                for(j=0;j<w;++j)
                {
                    delta = (qrand() & 0xf);//random int from 0 to 16
                    if(j & 1)
                    {
                        dithered = CLIP(*(p1 + i + j) - delta,0,MaxGray);
                    }else{
                        dithered = CLIP(*(p1 + i + j) + delta,0,MaxGray);
                    }

                    *(p2 + i +j) = lut[dithered];
                }

                //skip next row
                i = i + w + w;
            }

        }


    }else{

        //no dither
        for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
            IP_getChannel(I2, ch, p2, type);
            for(endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
        }

    }




}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::reset:
//
// Reset parameters.
//
void
Quantize::reset() {}
