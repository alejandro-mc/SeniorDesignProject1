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
    void stretch(ImagePtr I1, int min, int max, ImagePtr I2);
    void  match(ImagePtr I1,unsigned int * histo,ImagePtr I2);

protected slots:
    void changePower(int);
    void changeBegining(int);
    void changeEnd(int);
    void createHist(void);

private:
    //histogram matching controls for a polynomial of the form m * X^n + c
    //the controls are the power, the value at 0 and the value at 255
    QSlider		*m_sliderPower ;	// Slider to change power of the polynomial
    QSpinBox	*m_spinBoxPower;	// Spinbox to change the power of the polynomial

    QSlider		*m_sliderBegining ;	// Slider to control the vaue of the histogram at 0
    QSpinBox	*m_spinBoxBegining;	// Spinbox to control the value of the histogram at 0
    QCheckBox   *m_sliderEnd;       // Slider to control the value of the histogram at 255
    QCheckBox   *m_spinBoxEnd;      // Spinbox to control the value of the histogram at 255

	// label for Group Histogram Stretching group label
	QLabel		*m_label;	// Label for printing Otsu thresholds

	// widgets and groupbox
	QGroupBox	*m_ctrlGrp;	// Groupbox for panel
    int         *m_Hist; //Histogram


};


#endif	// HISTMATCH_H
