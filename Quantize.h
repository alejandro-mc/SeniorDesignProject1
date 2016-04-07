#ifndef QUANTIZE_H
#define QUANTIZE_H

#include "ImageFilter.h"

class Quantize : public ImageFilter {
    Q_OBJECT

public:
    Quantize	(QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
    void		reset		();		// reset parameters

protected:
    void quantize(ImagePtr I1, int levels, ImagePtr I2);

protected slots:
    void changeLevels(int);
    void toggleDither(int);

private:
    // scale controls
    QSlider		*m_slider ;	   // Level sliders
    QSpinBox	*m_spinBox;	   // Level spinbox boxes
    QCheckBox   *m_checkBox;   // Dither checkbox

    // label for Otsu thresholds
    QLabel		*m_label;	   // Label for printing Otsu thresholds
    QLabel      *m_labelDither; //Label for dither checkbox

    // widgets and groupbox
    QGroupBox	*m_ctrlGrp;	// Groupbox for panel
};

#endif // QUANTIZE_H
