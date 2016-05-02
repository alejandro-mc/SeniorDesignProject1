// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Median.cpp - Median class
//
// Written by: George Wolberg, 2016
// Modified by: Alejandro Morejon Cortina, 2016
// ======================================================================

#include "MainWindow.h"
#include "Median.h"


extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Median::Median:
//
// Constructor.
//
Median::Median(QWidget *parent) : ImageFilter(parent)
{
    m_maxKernel = 99;
    m_minKernel = 1;
    m_histogram = new unsigned int[MXGRAY];

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Median::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Median::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // error checking
    if(I1.isNull()) return 0;

    // get threshold value
    int sz = m_sliderKernelDim->value();
    int avg_nbrs = m_sliderAvg_Nbrs->value();

    // error checking
    if(sz < m_minKernel || sz > m_maxKernel || avg_nbrs < 0
                            || avg_nbrs > (((sz * sz) -1 ) >> 1)) return 0;

    // apply filter
    median(I1,sz,avg_nbrs,I2);

    return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur_Sharpen::createGroupBox:
//
// Create group box for control panel.
//
QGroupBox*
Median::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Median");

    // init widgets
    // create label Filter Size
    QLabel *fsizelabel = new QLabel;
    fsizelabel->setText(QString("Filter Size: "));

    // create Filter Size slider
    m_sliderKernelDim = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderKernelDim ->setTickPosition(QSlider::TicksBelow);
    m_sliderKernelDim ->setTickInterval(2);
    m_sliderKernelDim ->setSingleStep(2);
    m_sliderKernelDim ->setMinimum(m_minKernel);
    m_sliderKernelDim ->setMaximum(m_maxKernel);
    m_sliderKernelDim ->setValue  (m_minKernel);

    // create x spinbox
    m_spinBoxKernelDim = new QSpinBox(m_ctrlGrp);
    m_spinBoxKernelDim ->setMinimum(m_minKernel);
    m_spinBoxKernelDim ->setSingleStep(2);
    m_spinBoxKernelDim ->setMaximum(m_maxKernel);
    m_spinBoxKernelDim ->setValue  (m_minKernel);

    // create label y
    QLabel *avgnbrslabel = new QLabel;
    avgnbrslabel->setText(QString("AVG Neihbors:"));

    // create AVG Neighbor Slider
    m_sliderAvg_Nbrs = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderAvg_Nbrs ->setTickPosition(QSlider::TicksBelow);
    m_sliderAvg_Nbrs ->setTickInterval(2);
    m_sliderAvg_Nbrs ->setSingleStep(2);
    m_sliderAvg_Nbrs ->setMinimum(0);
    m_sliderAvg_Nbrs ->setMaximum(m_maxKernel);
    m_sliderAvg_Nbrs ->setValue  (0);

    // create AVG Neighbor spinbox
    m_spinBoxAvg_Nbrs = new QSpinBox(m_ctrlGrp);
    m_spinBoxAvg_Nbrs ->setMinimum(0);
    m_spinBoxAvg_Nbrs ->setSingleStep(2);
    m_spinBoxAvg_Nbrs ->setMaximum(m_maxKernel);
    m_spinBoxAvg_Nbrs ->setValue  (0);


    // init signal/slot connections for filter size controls
    connect(m_sliderKernelDim , SIGNAL(valueChanged(int)), this, SLOT(changeKernelDim (int)));
    connect(m_spinBoxKernelDim, SIGNAL(valueChanged(int)), this, SLOT(changeKernelDim(int)));

    connect(m_sliderAvg_Nbrs , SIGNAL(valueChanged(int)), this, SLOT(changeAvg_Nbrs(int)));
    connect(m_spinBoxAvg_Nbrs , SIGNAL(valueChanged(int)), this, SLOT(changeAvg_Nbrs(int)));


    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(  fsizelabel        , 0, 0);
    layout->addWidget(m_sliderKernelDim   , 0, 1);
    layout->addWidget(m_spinBoxKernelDim  , 0, 2);

    layout->addWidget(  avgnbrslabel      , 1, 0);
    layout->addWidget(m_sliderAvg_Nbrs    , 1, 1);
    layout->addWidget(m_spinBoxAvg_Nbrs   , 1, 2);

    // assign layout to group box
    m_ctrlGrp->setLayout(layout);

    return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Median::changeKernelDim:
//
// Slot to process change the kernel dimension.
//
void
Median::changeKernelDim(int sz)
{
    if(!(sz & 1)){//if ysz is even
        --sz;
    }

    int tmp = m_sliderAvg_Nbrs->value();

    while( ((tmp << 1) + 1) > (sz * sz)){
        --tmp;
    }

    m_sliderKernelDim     ->  blockSignals(true);
    m_sliderKernelDim     ->  setValue    (sz );
    m_sliderKernelDim     ->  blockSignals(false);
    m_spinBoxKernelDim    ->  blockSignals(true);
    m_spinBoxKernelDim    ->  setValue    (sz );
    m_spinBoxKernelDim    ->  blockSignals(false);


    m_sliderAvg_Nbrs     ->  blockSignals(true);
    m_sliderAvg_Nbrs     ->  setValue    (tmp );
    m_sliderAvg_Nbrs     ->  blockSignals(false);
    m_spinBoxAvg_Nbrs    ->  blockSignals(true);
    m_spinBoxAvg_Nbrs    ->  setValue    (tmp );
    m_spinBoxAvg_Nbrs    ->  blockSignals(false);


    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur_Sharpen::changeKernelDim:
//
// Slot to process change in filter width.
//
void
Median::changeAvg_Nbrs(int avg_nbrs)
{
    int tmp = m_sliderKernelDim->value();

    while( ((avg_nbrs << 1) + 1) > (tmp * tmp))
    {
        tmp += 2;
    }

    m_sliderKernelDim     ->  blockSignals(true);
    m_sliderKernelDim     ->  setValue    (tmp );
    m_sliderKernelDim     ->  blockSignals(false);
    m_spinBoxKernelDim    ->  blockSignals(true);
    m_spinBoxKernelDim    ->  setValue    (tmp );
    m_spinBoxKernelDim    ->  blockSignals(false);

    m_sliderAvg_Nbrs     ->  blockSignals(true);
    m_sliderAvg_Nbrs     ->  setValue    (avg_nbrs );
    m_sliderAvg_Nbrs     ->  blockSignals(false);
    m_spinBoxAvg_Nbrs    ->  blockSignals(true);
    m_spinBoxAvg_Nbrs    ->  setValue    (avg_nbrs );
    m_spinBoxAvg_Nbrs    ->  blockSignals(false);

    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Median::median:
//
// Performs a median filter opertion
//
//! \brief
//! \details	Output is in I2. val<t1: g1; t1<=val<t2: g2; t2<=val: g3
//! \param[in]	in  - Input image.
//! \param[in]	sz -  Kernel Dimension.
//! \param[out]	out  - Output image.
//
void Median::median(ImagePtr in, int sz,int avg_nbrs, ImagePtr out){


    IP_copyImageHeader(in,out);
    int width = in->width();
    int height = in->height();
    int total = width * height;
    int neighborhood_size = sz*sz;
    int padsize = (sz-1)>>1;
    uchar * sortedneighbors = new uchar[neighborhood_size];//will hold the sorted list of neighboring pixels
    int sum,mid,avgstart,avgend,add,sub;
    mid = (neighborhood_size-1) >> 1;
    avgstart = mid-avg_nbrs;
    avgend = mid+avg_nbrs;


    //ALLOCATE PADDED CIRCULAR BUFFER
    m_scanlinebuffer = new uchar*[sz];

    for(int i=0;i<sz;++i){//allocates rows
        m_scanlinebuffer[i] = new uchar[width + sz -1];
    }


    //FOR EACH IMAGE CHANNEL
    int type;
    ChannelPtr<uchar> p1,p1start, p3,p3end, endd;
    for(int ch = 0; IP_getChannel(in, ch, p1,type); ch++) {
        IP_getChannel(out,ch,p3,type);
        endd = p1+total;
        p3end = p3+total;
        p1start =p1;


        //TOP DOWN PASS (PROCESS CHANNEL ROWS FROM ROW ZERO TO ROW (height- 1 - (sz -1)/2) )
        //INITIALIZE BUFFER (COPY ROWS NEEDED TO PROCESS THE FIRST ROW)

        //COPY ROWS FROM 0 TO (sz-1 / 2)
        uchar ** buffrow = m_scanlinebuffer + padsize;
        for(int i=0;!(i>padsize);++i)
        {
            copypadded(*buffrow,p1,padsize,width);
            p1+=width;//move channel pointer to next row
            ++buffrow;//move circular buffer pointer to the next row
        }

        //REPLICATE TOP IMAGE ROW IN THE CIRCULAR BUFFER
        buffrow = m_scanlinebuffer;
        for(;buffrow < m_scanlinebuffer+padsize;++buffrow)
        {
            std::memcpy(*buffrow,*(m_scanlinebuffer+padsize),
                        sizeof(unsigned char) * (width + sz-1));
        }

        buffrow = m_scanlinebuffer;

        //PROCESS CHANNEL UNTIL NEW ROW IS PASSED THE CURRENT CHANNEL
        //p1 = p1start;//reset the input channel pointer
        processRowsTopDown(width,sz,avg_nbrs,p1,p3,endd);



        //BOTTOM UP PASS (PROCESS CHANNEL ROWS FRPM ROW height-1 TO ROW (height - (sz -1)/2))

        //COPY ROWS FROM height-1 TO
        buffrow = m_scanlinebuffer + padsize;
        p1 = endd - width;//set source pointer to bigining of bottom row
        for(int i=0;i<=padsize;++i)
        {
            copypadded(*buffrow,p1,padsize,width);
            p1-=width;//move channel pointer to next row
            ++buffrow;//move circular buffer pointer to the next row
        }

        //REPLICATE BOTTOM IMAGE ROW IN THE CIRCULAR BUFFER
        buffrow = m_scanlinebuffer;
        for(;buffrow < m_scanlinebuffer+padsize;++buffrow)
        {
            std::memcpy(*buffrow,*(m_scanlinebuffer+padsize),
                        sizeof(unsigned char) * (width + sz-1));
        }

        buffrow = m_scanlinebuffer;

        //PROCESS CHANNEL UNTIL NEW ROW IS PASSED THE CURRENT CHANNEL
        //p1 = p1start;//reset the input channel pointer
        processRowsBottomUp(width,sz,avg_nbrs,
                            p1,p3end - width,endd - (width * (sz + 1)) -1);


    }
    //END MEDIAN OPS ON IMAGE CHANNELS

    //DEALLOCATE PADDED CIRCULAR BUFFER
    for(int i=0;i<sz;++i){//deallocates rows
        delete [] m_scanlinebuffer[i];
    }
    delete [] m_scanlinebuffer;


    //delete sortedneighbors
    delete [] sortedneighbors;

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Median::processRowsTopDown:
//
void Median::processRowsTopDown(int width,int sz,int avg_nbrs,
                         ChannelPtr<uchar> p1,ChannelPtr<uchar> p3,ChannelPtr<uchar> endd)
{
    int neighborhood_size = sz*sz;
    int padsize = (sz-1)>>1;
    uchar * sortedneighbors = new uchar[neighborhood_size];//will hold the sorted list
                                                           //of neighboring pixels
    int sum,mid,avgstart,avgend,add,sub;
    mid = (neighborhood_size-1) >> 1;
    avgstart = mid-avg_nbrs;
    avgend = mid+avg_nbrs;


    int row = 0;
    do{

        //GET ROW PIXEL VALUES AND PUT THEM IN THE OUTPUT IMAGE


        //INITIALIZE HISTOGRAM for the first neighborhood of the row
        //INIT HISTOGRAM TO 0
        for(int i=0;i<MXGRAY;++i)
        {
            m_histogram[i] = 0;
        }

        for(int i=0;i<sz;++i)
        {
            for(int j=0;j<sz;++j)
            {
                m_histogram[m_scanlinebuffer[i][j]]++;
            }
        }

        //COMPUTE VALUE OF FIRST PIXEL BASED ON THE INITIAL HISTOGRAM
        genSortedNbrs(sortedneighbors);

        sum=0;
        for(int i=avgstart;i<=avgend;++i){//compute average of neighbors
            sum += sortedneighbors[i];
        }
        *p3 = sum / ((avg_nbrs<<1)+1);
        ++p3;

        //SLIDE NEIGHBORHOOD AND COMPUTE THE VALUES FOR THE REST OF THE PIXELS IN THE ROW
        for (int i=padsize+1;i < padsize+width; ++i){
            add = i + padsize;
            sub = i - padsize -1;
            //update histogram for new neighborhood
            for (int j=0;j<sz;++j)
            {
                ++m_histogram[m_scanlinebuffer[j][add]];
                --m_histogram[m_scanlinebuffer[j][sub]];
            }

            genSortedNbrs(sortedneighbors);

            sum=0;
            for(int i=avgstart;i<=avgend;++i){//compute average of neighbors
                sum += sortedneighbors[i];
            }
            *p3 = sum / ((avg_nbrs<<1)+1);
            ++p3;
        }

        //
        copypadded(m_scanlinebuffer[row%sz],p1,padsize,width);



        p1+=width;//move pointer to next row that will be added to the circular buffer
        row++;//advance to mark row of circular buffer where the new input row will be copied

    }while(p1<endd);

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Median::processRowsBottomUp:
//
void Median::processRowsBottomUp(int width,int sz,int avg_nbrs,
                         ChannelPtr<uchar> p1,ChannelPtr<uchar> p3,ChannelPtr<uchar> endd)
{
    int neighborhood_size = sz*sz;
    int padsize = (sz-1)>>1;
    uchar * sortedneighbors = new uchar[neighborhood_size];//will hold the sorted list
                                                           //of neighboring pixels
    int sum,mid,avgstart,avgend,add,sub;
    mid = (neighborhood_size-1) >> 1;
    avgstart = mid-avg_nbrs;
    avgend = mid+avg_nbrs;


    int row = 0;
    do{

        //GET ROW PIXEL VALUES AND PUT THEM IN THE OUTPUT IMAGE


        //INITIALIZE HISTOGRAM for the first neighborhood of the row
        //INIT HISTOGRAM TO 0
        for(int i=0;i<MXGRAY;++i)
        {
            m_histogram[i] = 0;
        }

        for(int i=0;i<sz;++i)
        {
            for(int j=0;j<sz;++j)
            {
                m_histogram[m_scanlinebuffer[i][j]]++;
            }
        }

        //COMPUTE VALUE OF FIRST PIXEL BASED ON THE INITIAL HISTOGRAM
        genSortedNbrs(sortedneighbors);

        sum=0;
        for(int i=avgstart;i<=avgend;++i){//compute average of neighbors
            sum += sortedneighbors[i];
        }
        *p3 = sum / ((avg_nbrs<<1)+1);
        ++p3;

        //SLIDE NEIGHBORHOOD AND COMPUTE THE VALUES FOR THE REST OF THE PIXELS IN THE ROW
        for (int i=padsize+1;i < padsize+width; ++i){
            add = i + padsize;
            sub = i - padsize -1;
            //update histogram for new neighborhood
            for (int j=0;j<sz;++j)
            {
                ++m_histogram[m_scanlinebuffer[j][add]];
                --m_histogram[m_scanlinebuffer[j][sub]];
            }

            genSortedNbrs(sortedneighbors);

            sum=0;
            for(int i=avgstart;i<=avgend;++i){//compute average of neighbors
                sum += sortedneighbors[i];
            }
            *p3 = sum / ((avg_nbrs<<1)+1);
            ++p3;
        }
        p3 -= (width << 1);//move p3 to begining of row above

        //copy a new padded row to the circular buffer
        copypadded(m_scanlinebuffer[row%sz],p1,padsize,width);



        p1-=width;//move pointer to next row that will be added to the circular buffer
        row++;//advance to mark row of circular buffer where the new input row will be copied

    }while(!(p1<endd));

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Median::genSortedNbrs:
//
void Median::genSortedNbrs(uchar * lst)
{
    for(int i=0;i<MXGRAY;++i)//create list of sorted neighbors
    {
        for(int j=0;j< m_histogram[i];++j)
        {
            *lst = i;
            ++lst;

        }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Median::copypadded:
// copy a row from an image to a padded buffer
void
Median::copypadded(unsigned char * dest, ChannelPtr<uchar> src, int padsize, int imgwidth)
{
    ChannelPtr<uchar> rowend;

    rowend = src+imgwidth;

    unsigned char* leftside,*destend;
    leftside= dest + padsize;
    destend = dest + (imgwidth + (padsize<<1));

    //create left pad
    for(;dest<leftside;++dest)
    {
        *dest = *src;
    }

    //copy image row
    for(;src<rowend;++src)
    {
        *dest = *src;
        ++dest;
    }

    --src;//move source back to right side of the image

    //create right pad
    for(;dest < destend;++dest)
    {
        *dest=*src;
    }

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Median::reset:
//
// Reset parameters.
//
void
Median::reset() {}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Median::~Median()
//
Median::~Median()
{
    delete [] m_histogram;
}
