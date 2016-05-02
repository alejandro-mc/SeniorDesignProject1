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
    void changeKernelDim(int);          // Slot to handle kernel dimension slider and spinbox
    void changeAvg_Nbrs(int);           // Slot to handle avg_nbr slider and spinbox
    void changeIter(int);              // Slot to handle iteration slider and spinbox changes

private:
    //median filter controls
    QSlider		*m_sliderKernelDim ;    // Kernel dimension slider
    QSpinBox	*m_spinBoxKernelDim;    // Kernel dimension spinbox
    QSlider     *m_sliderAvg_Nbrs;      // Slider to control number of neighbors to average
    QSpinBox    *m_spinBoxAvg_Nbrs;     // Spinbox to control nuber of neighbors to average
    QSlider     *m_sliderIter;          // Slider to control iterations to give an oil painting effect
    QSpinBox    *m_spinBoxIter;         // Spinbox to control iterations

    // widgets and groupbox
    QGroupBox	*m_ctrlGrp;	// Groupbox for panel

    int          m_maxKernel; //maximum kernel size
    int          m_minKernel;//minimum kernel size

    uchar      **m_scanlinebuffer;//buffer contaning the image rows needed to process the current scanline
                                  //it's a circular buffer each entry is dynamically allocated array
                                  //of (img_width + KernelDim - 1) uchars
    unsigned int * m_histogram;   //histogram to sort neighborhood pixels
    void          genSortedNbrs(uchar*);//generates a list of sorted elements based on m_histogram

    //once the circular buffer is initialized call this function to process the rows
    //top-down or bottom-up
    void          processRowsTopDown(int width,int sz,int avg_nbrs,
                              ChannelPtr<uchar> p1,ChannelPtr<uchar> p3,ChannelPtr<uchar> endd);

    void          processRowsBottomUp(int width,int sz,int avg_nbrs,
                              ChannelPtr<uchar> p1,ChannelPtr<uchar> p3,ChannelPtr<uchar> endd);
};

#endif // MEDIAN_H
