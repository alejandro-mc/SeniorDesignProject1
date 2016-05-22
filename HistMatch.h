// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Histogram Matching.h - Histogram Matching widget
//
// Written by: George Wolberg, 2016
// Modified By: Alejandro Morejon Cortina, 2016
// ======================================================================

#ifndef HISTMATCH_H
#define HISTMATCH_H

#include "ImageFilter.h"


class HistMatch : public ImageFilter {
	Q_OBJECT

public:
    HistMatch	(QWidget *parent = 0);	// constructor
    ~HistMatch();                     // destructor
	QGroupBox*	controlPanel	();		// create control panel
	bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
	void		reset		();		// reset parameters

protected:
    void  match(ImagePtr,unsigned int *,ImagePtr);
    void  matchRandomized(ImagePtr,unsigned int *,ImagePtr);

protected slots:
    void changePower(int);
    void handleDecreasing(int);

private:
    //controls for a target histogram of the form m * X^n + c
    //the controls are the power, and decreasing which determines whether the histogram
    //shape is increasing or decreasing
    QSlider		*m_sliderPower ;	// Slider to change power of the polynomial
    QSpinBox	*m_spinBoxPower;	// Spinbox to change the power of the polynomial

    QCheckBox   *m_checkBoxDecreasing;//determines whether the histogram is increasing or decreasing

    // label for Group Histogram Matching group label
    QLabel		*m_label;

	// widgets and groupbox
	QGroupBox	*m_ctrlGrp;	// Groupbox for panel

    //I'm going to bucket sort the input pixels
    //the pixels will be put in 256 buckets
    struct pixel{
        unsigned long int index;
        pixel * nextpixel;
    };

    struct pixelbucket{
        pixel * toppixel;
    };

    pixelbucket  m_pixelBucketsR[MXGRAY];//array to hold the sorted input pixels for the red channel

    pixelbucket  m_pixelBucketsG[MXGRAY];//array to hold the sorted input pixels for the green channel

    pixelbucket  m_pixelBucketsB[MXGRAY];//array to hold the sorted input pixels for the blue channel


    bool         m_isPixelBucketsInit;//deterines whether the pixelBuckets were initialized

    void         initPixelBuckets();//initializes pixel buckets

    void         clearPixelBuckets();

    bool         m_internal;//This is to know when the event invoking applyfilter
                            //did not originate from the HistMatch widget


};


#endif	// HISTMATCH_H
