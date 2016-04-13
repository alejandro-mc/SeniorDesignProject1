// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Threshold.h - Threshold widget
//
// Written by: George Wolberg, 2016
// ======================================================================

#ifndef HISTSTRETCH_H
#define HISTSTRETCH_H

#include "ImageFilter.h"


class HistStretch : public ImageFilter {
	Q_OBJECT

public:
    HistStretch	(QWidget *parent = 0);	// constructor
    ~HistStretch();                     // destructor
	QGroupBox*	controlPanel	();		// create control panel
	bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
	void		reset		();		// reset parameters

protected:
    void stretch(ImagePtr I1, int min, int max, ImagePtr I2);

protected slots:
    void changeMin(int);
    void changeMax(int);
    void minAuto(int);
    void maxAuto(int);
    void createHist(void);

private:
	//histogram stretching controls
    QSlider		*m_sliderMin ;	// Min slider
    QSpinBox	*m_spinBoxMin;	// Min spin box

    QSlider		*m_sliderMax ;	// Max slider
    QSpinBox	*m_spinBoxMax;	// Max spin box
    QCheckBox   *m_checkBoxMin; //Min checkbox
    QCheckBox   *m_checkBoxMax; //Max checkBox

	// label for Group Histogram Stretching group label
	QLabel		*m_label;	// Label for printing Otsu thresholds

	// widgets and groupbox
	QGroupBox	*m_ctrlGrp;	// Groupbox for panel
    int         *m_Hist; //Histogram


};


#endif	// HISTSTRETCH_H
