// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Median.cpp - Median class
//
// Written  by: George Wolberg, 2016
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
    int iter = m_sliderIter->value() -1;//iter will determine the number of extra iterations

    // error checking
    if(sz < m_minKernel || sz > m_maxKernel || avg_nbrs < 0
                            || avg_nbrs > (((sz * sz) -1 ) >> 1)) return 0;

    // apply filter
    median(I1,sz,avg_nbrs,I2);//first iteration requires the source image

    for(iter;iter>0;--iter)
    {
        median(I2,sz,avg_nbrs,I2);//the extra iterations are performed on the output image
    }

    return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Median::createGroupBox:
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
    QLabel *labelfsize = new QLabel;
    labelfsize->setText(QString("Filter Size: "));

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
    QLabel *labelavgnbrs = new QLabel;
    labelavgnbrs->setText(QString("AVG Neighbors:"));

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


    // create label for iterations
    QLabel *labelIter = new QLabel;
    labelIter->setText(QString("Iterations:"));

    // create Iterations Slider
    m_sliderIter = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderIter ->setTickPosition(QSlider::TicksBelow);
    m_sliderIter ->setTickInterval(1);
    m_sliderIter ->setSingleStep(1);
    m_sliderIter ->setMinimum(1);
    m_sliderIter ->setMaximum(10);
    m_sliderIter ->setValue  (1);

    // create Iterations spinbox
    m_spinBoxIter = new QSpinBox(m_ctrlGrp);
    m_spinBoxIter ->setMinimum(1);
    m_spinBoxIter ->setSingleStep(1);
    m_spinBoxIter ->setMaximum(10);
    m_spinBoxIter ->setValue  (1);



    // init signal/slot connections for filter size controls
    connect(m_sliderKernelDim , SIGNAL(valueChanged(int)), this, SLOT(changeKernelDim (int)));
    connect(m_spinBoxKernelDim, SIGNAL(valueChanged(int)), this, SLOT(changeKernelDim(int)));

    connect(m_sliderAvg_Nbrs , SIGNAL(valueChanged(int)), this, SLOT(changeAvg_Nbrs(int)));
    connect(m_spinBoxAvg_Nbrs , SIGNAL(valueChanged(int)), this, SLOT(changeAvg_Nbrs(int)));

    connect(m_sliderIter , SIGNAL(valueChanged(int)), this, SLOT(changeIter(int)));
    connect(m_spinBoxIter , SIGNAL(valueChanged(int)), this, SLOT(changeIter(int)));


    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(  labelfsize        , 0, 0);
    layout->addWidget(m_sliderKernelDim   , 0, 1);
    layout->addWidget(m_spinBoxKernelDim  , 0, 2);

    layout->addWidget(  labelavgnbrs      , 1, 0);
    layout->addWidget(m_sliderAvg_Nbrs    , 1, 1);
    layout->addWidget(m_spinBoxAvg_Nbrs   , 1, 2);

    layout->addWidget(  labelIter         , 2, 0);
    layout->addWidget(m_sliderIter        , 2, 1);
    layout->addWidget(m_spinBoxIter       , 2, 2);


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
// Median::changeKernelDim:
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
// Median::changeIter:
//
// Slot to process change number of iterations.
//
void
Median::changeIter(int iter)
{

    m_sliderIter     ->  blockSignals(true);
    m_sliderIter     ->  setValue    (iter );
    m_sliderIter     ->  blockSignals(false);
    m_spinBoxIter    ->  blockSignals(true);
    m_spinBoxIter    ->  setValue    (iter );
    m_spinBoxIter    ->  blockSignals(false);

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
//! \brief Performs a median filter on the entire image
//! \details Maps every neighborhood of I1 to the corresponding pixel in I2
//! \param[in]	in       -  Input image.
//! \param[in]	sz       -  Kernel Dimension.
//! \param[in]  avg_nbrs -  The number of pixels above and below the median in the sorted neighborhood
//! \param[out]	out      -  Output image.
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

        //PROCESS CHANNEL ROWS
        processRows(width,sz,avg_nbrs,p1,p3,endd);


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
//Median::processRows:
//
void Median::processRows(int width,int sz,int avg_nbrs,
                         ChannelPtr<uchar> p1,ChannelPtr<uchar> p3,ChannelPtr<uchar> endd)
{
    int neighborhood_size = sz*sz;
    int padsize = (sz-1)>>1;
    int add,sub,avg;
    //avgstart = mid-avg_nbrs;
    //avgend = mid+avg_nbrs;


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

        getAvg(neighborhood_size,avg_nbrs,&avg);

        *p3 = avg;
        ++p3;

        //SLIDE NEIGHBORHOOD AND COMPUTE THE VALUES FOR THE REST OF THE PIXELS IN THE ROW
        //int bufferwidth = padsize +
        for (int i=padsize+1;i < padsize+width; ++i){
            add = i + padsize;
            sub = i - padsize -1;
            //update histogram for new neighborhood
            for (int j=0;j<sz;++j)
            {
                ++m_histogram[m_scanlinebuffer[j][add]];
                --m_histogram[m_scanlinebuffer[j][sub]];
            }

            getAvg(neighborhood_size,avg_nbrs,&avg);

            *p3 = avg;
            ++p3;
        }

        //
        copypadded(m_scanlinebuffer[row%sz],p1,padsize,width);



        p1+=width;//move pointer to next row that will be added to the circular buffer
        row++;//advance to mark row of circular buffer where the new input row will be copied

    }while(p1<endd);

    p1-=width;

    for(int i=0;i<=padsize;++i){
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

        getAvg(neighborhood_size,avg_nbrs,&avg);

        *p3 = avg;
        ++p3;

        //SLIDE NEIGHBORHOOD AND COMPUTE THE VALUES FOR THE REST OF THE PIXELS IN THE ROW
        int lastcolumn = padsize + width;
        for (int i=padsize+1;i < lastcolumn; ++i){
            add = i + padsize;
            sub = i - padsize -1;
            //update histogram for new neighborhood
            for (int j=0;j<sz;++j)
            {
                ++m_histogram[m_scanlinebuffer[j][add]];
                --m_histogram[m_scanlinebuffer[j][sub]];
            }


            getAvg(neighborhood_size,avg_nbrs,&avg);

            *p3 = avg;
            ++p3;
        }

        //
        copypadded(m_scanlinebuffer[row%sz],p1,padsize,width);

        row++;
    }

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Median::getAvg:
//
void
Median::getAvg(int neighborhood_size, int avgnbrs,int * avg){

    int sum,median,items,i,iLB,iLA;
    sum=0;
    items=0;

    iLB = avgnbrs;//items left before the median
    iLA = avgnbrs;//items left after the median

    //first find the median/////////////////////////
    median=-1;
    int tmp = neighborhood_size +1;
    do{
        ++median;
        items += m_histogram[median];
    }while((items << 1) < tmp);
    //end median

    //sum neighbors above and below median in the the bucket containing the median
    sum = median * MIN(items - ((neighborhood_size + 1) >> 1),avgnbrs) +
          median * MIN((((neighborhood_size - 1) >> 1) - (items - m_histogram[median]))  ,avgnbrs) +
          median;

    //compute avg///////////////

    //get sum of neighbors below
    i = median - 1;
    iLB -= (((neighborhood_size - 1) >> 1) - (items - m_histogram[median]));
    while(iLB > 0 &&  i > -1)
    {
        sum += i * MIN(m_histogram[i],iLB);
        iLB -= m_histogram[i];
        --i;
    }

    //get sum of neighbors above
    i = median + 1;
    iLA -= items - ((neighborhood_size + 1) >> 1);
    while(iLA > 0 &&  i < MXGRAY)
    {
        sum += i * MIN(m_histogram[i],iLA);
        iLA -= m_histogram[i];
        ++i;
    }


    *avg = sum / ((avgnbrs << 1) + 1);
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
Median::reset() {

    m_sliderIter          ->  blockSignals(true);
    m_spinBoxIter         ->  blockSignals(true);
    m_sliderKernelDim     ->  blockSignals(true);
    m_spinBoxKernelDim    ->  blockSignals(true);
    m_sliderAvg_Nbrs      ->  blockSignals(true);
    m_spinBoxAvg_Nbrs     ->  blockSignals(true);

    m_sliderIter          ->  setValue    (1 );
    m_spinBoxIter         ->  setValue    (1 );
    m_sliderKernelDim     ->  setValue    (1 );
    m_spinBoxKernelDim    ->  setValue    (1 );
    m_sliderAvg_Nbrs      ->  setValue    (0 );
    m_spinBoxAvg_Nbrs     ->  setValue    (0 );

    m_sliderIter          ->  blockSignals(false);
    m_spinBoxIter         ->  blockSignals(false);
    m_sliderKernelDim     ->  blockSignals(false);
    m_spinBoxKernelDim    ->  blockSignals(false);
    m_sliderAvg_Nbrs      ->  blockSignals(false);
    m_spinBoxAvg_Nbrs     ->  blockSignals(false);

    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

    // display output
    g_mainWindowP->displayOut();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Median::~Median()
//
Median::~Median()
{
    delete [] m_histogram;
}
