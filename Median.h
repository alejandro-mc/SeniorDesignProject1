#ifndef MEDIAN_H
#define MEDIAN_H

#include "ImageFilter.h"

class Median : public ImageFilter {
    Q_OBJECT

public:
    Median      (QWidget *parent = 0);          // constructor
    ~Median     ();                             //destructor
    QGroupBox*	controlPanel	();                 // create control panel
    bool		applyFilter(ImagePtr, ImagePtr);    // apply filter to input to init output
    void		reset		();                     // reset parameters

protected:
    void median(ImagePtr in, int, int, ImagePtr out);
    void copypadded(unsigned char*,ChannelPtr<uchar>,int,int);

protected slots:
    void changeKernelDim(int);        //Slot to handle kernel dimension slider and spinbox
    void changeAvg_Nbrs(int);        //Slot to handle avg_nbr slider and spinbox

private:
    //blur controls
    QSlider		*m_sliderKernelDim ;  // Kernel dimension slider
    QSpinBox	*m_spinBoxKernelDim;  // Kernel dimension spinbox
    QSlider     *m_sliderAvg_Nbrs;   //Filter height slider
    QSpinBox    *m_spinBoxAvg_Nbrs;  //Filter height spinbox

    // widgets and groupbox
    QGroupBox	*m_ctrlGrp;	// Groupbox for panel

    int          m_maxKernel; //maximum kernel size
    int          m_minKernel;//minimum kernel size

    uchar      **m_scanlinebuffer;//buffer contaning the image rows needed to process the current scanline
                                  //it's a circular buffer each entry is dynamically allocated array
                                  //of (img_width + KernelDim - 1) uchars
    unsigned int * m_histogram;   //histogram to sort neighborhood pixels
    void          genSortedNbrs(uchar*);//generates a list of sorted elements based on m_histogram
};

#endif // MEDIAN_H
