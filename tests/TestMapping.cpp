///-------------------------------------------------------------------------------------------------
// file:	TestMapping.cpp
//
// summary:  Tests the here-and-back-again mapping through all iterations of units to get original
//           value
//
//
//  Copyright (c) 2014 by Michael Ryan
///-------------------------------------------------------------------------------------------------

#include <iostream>
#include "UValue.h"

// Main test units engine
// template class here assumes UValue here has getUnits method implemented
template< class T>
bool TestUnits()
{
    bool bSucceeded = true;
    int sig_figs = 16;
    UValue out;
    UValue conversion;
    UValue * orig_conversions = new(std::nothrow) UValue[T::getUnitsLen()];
    
    for(int i = 0; i < T::getUnitsLen(); i++)
    {
        double bigORsmall = rand();
        double randNum = ((double)rand()/RAND_MAX) - .5;
        if ( bigORsmall > RAND_MAX/2 )
        {
            randNum = RAND_MAX*randNum;
        }
        UValue original(randNum, new T(T::getUnits()[i]));
        std::cout << "****** Starting value " << original << std::endl;
        
        do {
            bSucceeded = true;
        
            for(int j = 0; j < T::getUnitsLen(); j++)
            {
                
                conversion = original[T::getUnits()[j]];
                
                if ( orig_conversions) {
                    orig_conversions[j] = conversion;
                }
                
                out = conversion[T::getUnits()[i]];
                
                out = out.roundTo(sig_figs);
                original = original.roundTo(sig_figs);
                
                if ( out != original ) {
                    bSucceeded = false;
                    break;
                }
            }
        } while (!bSucceeded && (--sig_figs) > 1);
        
        for( int j = 0; j < T::getUnitsLen(); j++)
        {
            std::cout << "Conversion from " << T::getUnits()[i] << " to " << T::getUnits()[j] << " and back to " << T::getUnits()[i] << ". ";
            if ( bSucceeded )
            {
                std::cout << "Test succeeded. Result:  ";
                if ( orig_conversions ) {
                    std::cout << orig_conversions[j].roundTo(sig_figs);
                }
                else {
                    std::cout << original.roundTo(sig_figs);
                }
            }
            else
            {
                std::cout << "Test FAILED! " << out << " != " << original;
            }
            
            std::cout << std::endl;
        }
        
        if ( bSucceeded )
        {
            std::cout << std::endl << std::endl << "Test succeeded with " << sig_figs << " significant figures" ;
            std::cout << std::endl << std::endl;
        }
    }
    
    if ( orig_conversions ) {
        delete [] orig_conversions;
    }
    
    return bSucceeded;
}

int main(int argc, const char * argv[])
{
    time_t tseed = time(nullptr);
    
    srand((unsigned int)tseed);
    
    UValue out;
    UValue ep_out;
    UValue conversion;
    
    bool bSucceeded = false;
    
    int sig_figs = 14;
    
    //double epsilon = std::numeric_limits<double>::epsilon();
    
    std::cout << std::endl << std::endl << "**** Relative Power Units ****" << std::endl ;
    
    // log amp first
    do {
        bSucceeded = true;
        
        for(int i = 0; i < ampls_len; i++)
        {
            UValue original(1.0*((double)rand()/RAND_MAX), new AmplUnit(ampls[i]));
            
            if ( ampls[i][0] != 'W' && ampls[i][0] != 'V' && ampls[i][0] != 'm' )
            {
                original.setValue(original.getValue());
            }
            
            std::cout << "****** Starting value " << original << std::endl;
            
            for(int j = 0; j < ampls_len; j++)
            {
                
                conversion = original[ampls[j]];
                
                out = conversion[ampls[i]];
                std::cout << "Conversion from " << ampls[i] << " to " << ampls[j] << " and back to " << ampls[i] << ". ";
                
                out = out.roundTo(sig_figs);
                original = original.roundTo(sig_figs);
                
                if ( out == original )
                {
                    std::cout << "Test succeeded. Result:  " << conversion;
                }
                else
                {
                    bSucceeded = false;
                    std::cout << "Test FAILED! " << out << " != " << original;
                }
                
                std::cout << std::endl;
            }
        }
    } while ( !bSucceeded && (--sig_figs) > 1 );
    
    std::cout << std::endl << "********* Relative Power Units ****";
    
    if ( bSucceeded )
    {
        std::cout << "Succeeded";
    }
    else
    {
        std::cout << "Failed";
    }
    
    std::cout << " to " << sig_figs << " significant figures.";
    
    std::cout << std::endl << std::endl;
    
    std::cout << std::endl << std::endl << "**** Time Units ****" << std::endl ;
    
    TestUnits<TimeUnit>();
    
    std::cout << std::endl << std::endl;
    
    std::cout << std::endl << "********* Frequency Units ****";
    
    std::cout << std::endl << std::endl;
    
    TestUnits<FrequencyUnit>();
    
    std::cout << std::endl << std::endl;
    
    std::cout << std::endl << "********* Temperature Units ****";
    
    std::cout << std::endl << std::endl;
    
    TestUnits<TempUnit>();
    
    std::cout << std::endl << std::endl;
    
    std::cout << std::endl << "********* Current Units ****";
    
    std::cout << std::endl << std::endl;
    
    TestUnits<CurrentUnit>();
    
    std::cout << std::endl << std::endl;
    
    std::cout << std::endl << "********* Volume Units ****";
    
    std::cout << std::endl << std::endl;
    
    TestUnits<VolumeUnit>();
    
    std::cout << std::endl << std::endl;
    
    std::cout << std::endl << "********* Mass Units ****";
    
    std::cout << std::endl << std::endl;
    
    TestUnits<MassUnit>();
    
    std::cout << std::endl << std::endl;
    
    std::cout << std::endl << "********* Force Units ****";
    
    std::cout << std::endl << std::endl;
    
    TestUnits<ForceUnit>();
    
    std::cout << std::endl << std::endl;
    
    std::cout << std::endl << "********* Pressure Units ****";
    
    std::cout << std::endl << std::endl;
    
    TestUnits<PressureUnit>();
    
    std::cout << std::endl << std::endl;
    
    std::cout << std::endl << "********* Energy Units ****";
    
    std::cout << std::endl << std::endl;
    
    TestUnits<EnergyUnit>();
    
    std::cout << std::endl << std::endl;
    
    std::cout << std::endl << "********* Power Units ****";
    
    std::cout << std::endl << std::endl;
    
    TestUnits<PowerUnit>();
    
    return 0;
}


