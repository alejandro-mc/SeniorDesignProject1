#ifndef BLUR_H
#define BLUR_H

#include "ImageFilter.h"

class Blur : public ImageFilter {
    Q_OBJECT

public:
    Blur    	(QWidget *parent = 0);          // constructor
    QGroupBox*	controlPanel	();                 // create control panel
    bool		applyFilter(ImagePtr, ImagePtr);    // apply filter to input to init output
    void		reset		();                     // reset parameters

protected:
    void blur(ImagePtr in, int xsz, int ysz, ImagePtr out);
    //void sharpen(ImagePtr in, int sz, ImagePtr out);

protected slots:
    void changeFilterX(int);        //Slot to handle Filter width slider and spinbox
    void changeFilterY(int);        //Slot to handle Filter height slider and spinbox
    //void changeEdgIntensity(int);   //Slot to handle egde intensity slider with image sharpening

    void toggleSquare(int);         //Slot to handle Square checkbox
    //void toggleSharpen(int);        //Slot to handle sharpen checkbox


private:
    //blur controls
    QSlider		*m_sliderFilterWidth ;  // Filter width slider
    QSpinBox	*m_spinBoxFilterWidth;  // Filter width spinbox
    QSlider     *m_sliderFilterHeight;   //Filter height slider
    QSpinBox    *m_spinBoxFilterHeight;  //Filter height spinbox

    QCheckBox   *m_checkBoxSqr;     // Check box to lock to keep FilterX = FilterY

    //Sharpen Controls
    //QCheckBox   *m_checkBoxSharpen;         //Toggle image sharpenning
    //QSlider     *m_sliderEdgIntensity;      //Slider to control edge intensity
    //QSlider     *m_spinBoxEdgntensity;      //Spinbox to control edge intensity

    // widgets and groupbox
    QGroupBox	*m_ctrlGrp;	// Groupbox for panel

    int          m_maxFilterDim; //maximum filter dimmesion
    int         m_minFilterDim;//minimum filter dimmesion

    int         *m_blurbuffer;//blur buffer
};

#endif // BLUR_H
