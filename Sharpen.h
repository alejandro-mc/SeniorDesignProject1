#ifndef SHARPEN_H
#define SHARPEN_H

#include "ImageFilter.h"

class Sharpen : public ImageFilter {
    Q_OBJECT

public:
    Sharpen	(QWidget *parent = 0);          // constructor
    QGroupBox*	controlPanel	();                 // create control panel
    bool		applyFilter(ImagePtr, ImagePtr);    // apply filter to input to init output
    void		reset		();                     // reset parameters

protected:
    void blur(ImagePtr in, int xsz, int ysz, ImagePtr out);
    void sharpen(ImagePtr in, int sz, double fctr, ImagePtr out);

protected slots:
    void changeKernelSize(int);        //Slot to handle Filter width slider and spinbox
    void changeFactor(int);        //Slot to handle Filter height slider and spinbox

private:
    //sharpen controls
    QSlider		*m_sliderKernelSize ;  // Filter dimension slider
    QSpinBox	*m_spinBoxKernelSize;  // Filter dimension spinbox
    QSlider     *m_sliderFactor;   //Factor slider
    QSpinBox    *m_spinBoxFactor;  //Factor height spinbox


    // widgets and groupbox
    QGroupBox	*m_ctrlGrp;	     // Groupbox for panel

    int          m_maxKernelSize; //maximum filter dimmesion
    int          m_minKernelSize; //minimum filter dimmesion
    int          m_maxFactor;     //
    int          m_minFactor;

    int         *m_blurbuffer;//blur buffer
};

#endif // SHARPEN_H
