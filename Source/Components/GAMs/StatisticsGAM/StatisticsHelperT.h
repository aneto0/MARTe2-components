/**
 * @file StatisticsHelperT.h
 * @brief Header file for class StatisticsHelperT
 * @date 10/03/2017
 * @author Bertrand Bauvir
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class StatisticsHelperT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef StatisticsHelperT_H_
#define StatisticsHelperT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <math.h>
#include <limits>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "CircularBufferT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The class provides templated statistics computation.
 * @detail The class allocates circular buffers to store values of samples and
 * computes average, standard deviation, minimum and maximum over a moving time
 * window.
 * The implementation does not perform division, rather uses bit shift operation
 * for integer types and pre-computes 1.0 / size for floating point types. As such,
 * the computation of average and standard deviation is only exact after the window
 * has been fully populated once with samples.
 * Consequent to that above, the window size for integer types is limited to power
 * of 2, e.g. 64, 1024, etc. In case a different window size is specified, the 
 * highest power of 2 smaller than the specified size is being considered. No
 * limitation for floating point types.
 */

template <typename Type> class StatisticsHelperT {
  public:

    /**
     * @brief Constructor. 
     * @detail Allocates memory buffers to store samples in a moving time
     * window. In case of integer types, the actual window size will be the highest
     * power of 2 smaller or equal to the specified size.
     */
    StatisticsHelperT(uint32 windowSize);

    /**
     * @brief Destructor. Frees allocated memory buffers.
     */
    virtual ~StatisticsHelperT(); 

    /**
     * @brief Initialiser. Clears out sample buffer and initialises accumulators.
     */
    void Reset();

    /**
     * @brief Accessor. Inserts new sample and re-compute statistics.
     * @return if buffer properly allocated.
     */
    bool PushSample(Type sample);

    /**
     * @brief Accessor. Retrieves the sample at index in the sample buffer.
     * @return sample at index.
     */
    Type GetSample(uint32 index);

    /**
     * @brief Accessor. Retrieves the last inserted sample.
     * @return Last inserted sample.
     */
    Type GetSample(void);

    /**
     * @brief Accessor. Returns the window size.
     * @return actual window size.
     */
    uint32 GetSize(void);

    /**
     * @brief Accessor. Returns the number of samples inserted in the buffer.
     * @return number of samples.
     */
    uint32 GetCounter(void);

    /**
     * @brief Average over the moving window of samples.
     * @return average.
     */
    Type GetAvg(void);

    /**
     * @brief Maximum over the moving window of samples.
     * @return maximum.
     */
    Type GetMax(void);

    /**
     * @brief Minimum over the moving window of samples.
     * @return minimum.
     */
    Type GetMin(void);

    /**
     * @brief Root mean squares over the moving window of samples.
     * @return rms.
     */
    Type GetRms(void);

    /**
     * @brief Standard deviation over the moving window of samples.
     * @return standard deviation.
     */
    Type GetStd(void);

    /**
     * @brief Sum of all samples over the moving window of samples.
     * @return sum.
     */
    Type GetSum(void);

  private:

    uint32 size;    /* Window size */
    uint32 counter; /* Number of samples inserted */

    Type Xavg;
    Type Xdiv; 
    Type Xmax; 
    Type Xmin;
    Type Xrms;
    Type Xspl;
    Type Xstd; 

    CircularBufferT<Type> * Xwin; /* Sample buffer */
    CircularBufferT<Type> * Xsq;  /* Sample buffer (squared) */

    /**
     * @brief Average of squares over the moving window.
     * @return average of sample squares.
     */
    Type GetRmsSq (void);

    /**
     * @brief Finds the maximum of samples in the sample buffer.
     * @return maximum.
     */
    Type FindMax(void);

    /**
     * @brief Finds the minimum of samples in the sample buffer.
     * @return maximum.
     */
    Type FindMin(void);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <typename Type> inline Type SquareRoot (Type x) { 

    if (x < (Type) 0) {
        REPORT_ERROR(ErrorManagement::Warning, "SquareRoot<Type> of negative number");
    }

    return sqrt(x); 
}

template <> inline uint8 SquareRoot<uint8>(uint8 x) { /* From http://en.wikipedia.org/wiki/Methods_of_computing_square_roots */

    uint8 res = 0;
    uint8 bit = ((uint8) 1u) << 6; // The second-to-top bit is set
 
    // "bit" starts at the highest power of four <= the argument.
    while (bit > x) bit >>= 2;
 
    while (bit != 0) 
      {
        if (x >= res + bit) { x -= res + bit; res = (res >> 1) + bit; }
        else res >>= 1;

        bit >>= 2;
      }

    return res;

}

template <> inline uint16 SquareRoot<uint16>(uint16 x) { /* From http://en.wikipedia.org/wiki/Methods_of_computing_square_roots */

    uint16 res = 0;
    uint16 bit = ((uint16) 1u) << 14; // The second-to-top bit is set
 
    // "bit" starts at the highest power of four <= the argument.
    while (bit > x) bit >>= 2;
 
    while (bit != 0) 
      {
        if (x >= res + bit) { x -= res + bit; res = (res >> 1) + bit; }
        else res >>= 1;

        bit >>= 2;
      }

    return res;

}

template <> inline uint32 SquareRoot<uint32>(uint32 x) { /* From http://en.wikipedia.org/wiki/Methods_of_computing_square_roots */

    uint32 res = 0;
    uint32 bit = ((uint32) 1u) << 30; // The second-to-top bit is set
 
    // "bit" starts at the highest power of four <= the argument.
    while (bit > x) bit >>= 2;
 
    while (bit != 0) 
      {
        if (x >= res + bit) { x -= res + bit; res = (res >> 1) + bit; }
        else res >>= 1;

        bit >>= 2;
      }

    return res;

}

template <> inline uint64 SquareRoot<uint64>(uint64 x) { /* From http://en.wikipedia.org/wiki/Methods_of_computing_square_roots */

    uint64 res = 0;
    uint64 bit = ((uint64) 1u) << 62; // The second-to-top bit is set
 
    // "bit" starts at the highest power of four <= the argument.
    while (bit > x) bit >>= 2;
 
    while (bit != 0) 
      {
        if (x >= res + bit) { x -= res + bit; res = (res >> 1) + bit; }
        else res >>= 1;

        bit >>= 2;
      }

    return res;

}

template <> inline int8 SquareRoot<int8>(int8 x) {

    if (x < (int8) 0) {
        REPORT_ERROR(ErrorManagement::Warning, "SquareRoot<int8> of negative number");
    }

    uint8 tmp = (uint8) x;
    int8 res = (int8) SquareRoot<uint8>(tmp);

    return res; 
};

template <> inline int16 SquareRoot<int16>(int16 x) {

    if (x < (int16) 0) {
        REPORT_ERROR(ErrorManagement::Warning, "SquareRoot<int16> of negative number");
    }

    uint16 tmp = (uint16) x;
    int16 res = (int16) SquareRoot<uint16>(tmp);

    return res; 
};

template <> inline int32 SquareRoot<int32>(int32 x) {

    if (x < (int32) 0) {
        REPORT_ERROR(ErrorManagement::Warning, "SquareRoot<int32> of negative number");
    }

    uint32 tmp = (uint32) x;
    int32 res = (int32) SquareRoot<uint32>(tmp);

    return res; 
};

template <> inline int64 SquareRoot<int64>(int64 x) {

    if (x < (int64) 0) {
        REPORT_ERROR(ErrorManagement::Warning, "SquareRoot<int64> of negative number");
    }

    uint64 tmp = (uint64) x;
    int64 res = (int64) SquareRoot<uint64>(tmp);

    return res; 
};

template <typename Type> StatisticsHelperT<Type>::StatisticsHelperT(uint32 windowSize)
{

    size = 1u; 
    Xdiv = 0u;
  
    while (windowSize > 1u) { 
        windowSize >>= 1u; 
	size <<= 1u;
	Xdiv += 1u;
    };

    /* Instantiate sample buffers */
    Xwin = new CircularBufferT<Type> (size); 
    Xsq  = new CircularBufferT<Type> (size); 

    Reset();

}

template <> inline StatisticsHelperT<float32>::StatisticsHelperT(uint32 windowSize)
{

    size = windowSize;
    Xdiv = 1.0 / (float32) size;

    /* Instantiate sample buffers */
    Xwin = new CircularBufferT<float32> (size); 
    Xsq  = new CircularBufferT<float32> (size); 

    Reset();

}

template <> inline StatisticsHelperT<float64>::StatisticsHelperT(uint32 windowSize)
{

    size = windowSize;
    Xdiv = 1.0 / (float64) size;

    /* Instantiate sample buffers */
    Xwin = new CircularBufferT<float64> (size); 
    Xsq  = new CircularBufferT<float64> (size); 

    Reset();

}

template <typename Type> StatisticsHelperT<Type>::~StatisticsHelperT ()
{

    Reset();

    if (Xwin != NULL_PTR(CircularBufferT<Type> *)) {
        delete Xwin;
    }

    if (Xsq != NULL_PTR(CircularBufferT<Type> *)) {
        delete Xsq;
    }

}

template <typename Type> void StatisticsHelperT<Type>::Reset() {

  counter = 0;

  Xspl = (Type) 0;
  Xavg = (Type) 0;
  Xmax = (Type) std::numeric_limits<Type>::min();
  Xmin = (Type) std::numeric_limits<Type>::max();
  Xrms = (Type) 0;
  Xstd = (Type) 0;

  /* Reset sample buffers */
  Xwin->Initialise(Xspl); 
  Xsq->Initialise(Xspl);

}

template <typename Type> bool StatisticsHelperT<Type>::PushSample(Type sample) {

    bool ok = true;

    Xspl = sample;

    if (counter < size) {
        counter++;
    }

    Type oldest;

    /* Update sample buffer */
    if (ok) {
        ok = Xwin->PushData(Xspl, oldest);
    }

    if (ok) {
        /* Compute average */
        Xavg -= oldest; /* Remove oldest sample from the accumulator */
	Xavg += Xspl; /* Sum of all sample in time window */

	/* Update max/min, if necessary */
	if (Xspl > Xmax) {
	    Xmax = Xspl; 
	} else if (oldest == Xmax) { /* The removed sample was the max over the time window */
	    Xmax = FindMax();
	}
	
	if (Xspl < Xmin) {
	    Xmin = Xspl; 
	} else if (oldest == Xmin) { /* The removed sample was the min over the time window */
	    Xmin = FindMin();
	}
    }

    Type Xspl_sq  = Xspl * Xspl; /* Square of the sample */

    /* Update sample buffer */
    if (ok) {
        ok = Xsq->PushData(Xspl_sq, oldest);
    }

    if (ok) {
        /* Compute root mean square */
        Xrms -= oldest; /* Remove oldest sample from the accumulator */
	Xrms += Xspl_sq;    /* Sum of squares of all samples in time window */
    }

    return ok;
}

 template <typename Type> Type StatisticsHelperT<Type>::GetAvg() {

    Type avg = Xavg >> Xdiv;

    return avg;
}

template <> inline float32 StatisticsHelperT<float32>::GetAvg() {

    float32 avg = Xavg * Xdiv;

    return avg;
}

template <> inline float64 StatisticsHelperT<float64>::GetAvg() {

    float64 avg = Xavg * Xdiv;

    return avg;
}

template <typename Type> Type StatisticsHelperT<Type>::GetRmsSq() {

    Type rms_sq = Xrms >> Xdiv;

    return rms_sq;
}

template <> inline float32 StatisticsHelperT<float32>::GetRmsSq() {

    float32 rms_sq = Xrms * Xdiv;

    return rms_sq;
}

template <> inline float64 StatisticsHelperT<float64>::GetRmsSq() {

    float64 rms_sq = Xrms * Xdiv;

    return rms_sq;
}

template <typename Type> Type StatisticsHelperT<Type>::GetRms() {

    Type rms_sq = GetRmsSq();
    Type rms = SquareRoot<Type>(rms_sq);

    return rms;

}

template <typename Type> Type StatisticsHelperT<Type>::GetStd() {

    Type avg = GetAvg();
    Type avg_sq = avg * avg;
    Type rms_sq = GetRmsSq();
    Type std = SquareRoot<Type>(rms_sq - avg_sq);
    
    return std;

}

template <typename Type> Type StatisticsHelperT<Type>::GetSum() { 
    return Xavg; 
}

template <typename Type> Type StatisticsHelperT<Type>::GetMax() { 
    return Xmax; 
}

template <typename Type> Type StatisticsHelperT<Type>::GetMin() { 
    return Xmin; 
}

template <typename Type> Type StatisticsHelperT<Type>::FindMax() {

    Type max = (Type) std::numeric_limits<Type>::min();
  
    uint32 index;

    for (index = 0u; index < size; index++) {

        Type sample;

        Xwin->GetData(sample, index);
      
	if (sample > max) {
	    max = sample;
	}
    }

  return max;
}

template <typename Type> Type StatisticsHelperT<Type>::FindMin() {

    Type min = (Type) std::numeric_limits<Type>::max();
  
    uint32 index;

    for (index = 0u; index < size; index++) {

        Type sample;

        Xwin->GetData(sample, index);
      
	if (sample < min) {
	    min = sample;
	}
    }

  return min;
}

} /* namespace MARTe */

#endif /* StatisticsHelperT_H_ */


