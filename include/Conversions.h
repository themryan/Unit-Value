///-------------------------------------------------------------------------------------------------
// file:	Conversions.h
//
// summary:	Declares the UValue units class and the unit classes
//				FrequencyUnit: Frequency units class
//				AmplUnit: Power Ratio units class
//				TimeUnit: Time units class
//				DistUnit: Distance units class
//				TempUnit: Temperature units class
//				CurrentUnit: Electrical current units class
//              VolumeUnit: Volume units class
//              MassUnit: Mass units class
//              ForceUnit: Force units class
//              PressureUnit: Pressure units classs
//              EnergyUnit: Energy units class
//              PowerUnit: Power units class
//
//  Copyright (c) 2014 by Michael Ryan
///-------------------------------------------------------------------------------------------------

#pragma once
#include <iostream>
#include <string.h>
#include <sstream>
#include <map>
#include <list>
#include <math.h>

#ifndef _CONVERSIONS_H_
#define _CONVERSIONS_H_
#endif

#ifdef UNITS_CASE_INSENSITIVE

#define STRCMP(a, b)        stricmp(a, b)
#define STRNCMP(a, b, n)    strnicmp(a, b, n)

#else

#define STRCMP(a, b)        strcmp(a, b)
#define STRNCMP(a, b, n)    strncmp(a, b, n)

#endif

bool foundUnitInUnits(const char * pUnit, const char ** units, const int units_len);

/*
 // This function pointer defines the format of the conversion class.
 // For each class a function matching this profile must be created.
 */

typedef bool (*UnitConversion)(
        double& value_in,
        int unit_index_in,
        int unit_index_out,
        const double * params_list,
        size_t params_list_len);

extern const char *scalar[];

// Does nothing except test for isnan
bool IdentityConversion(double& value_in,
                            int in,
                            int out,
                            const double * params_list,
                            size_t params_list_len);

extern const char *freqs[];
extern const int freqs_len;

/*
    Frequency Conversion engine
 */
bool FreqConversion(double& value_in,
                           int in,
                           int out,
                           const double * params_list,
                           size_t params_list_len);

extern const char *times[];
extern const int times_len;

/*
 Time Conversion engine
 */
bool TimeConversion(double& value_in,
                       int in,
                       int out,
                       const double * params_list,
                       size_t params_list_len);


extern const char *dists[];
extern const int dists_len;

/*
 Distance Conversion engine
 */
bool DistanceConversion(double& value_in,
                        int in,
                        int out,
                        const double * params_list,
                        size_t params_list_len);

extern const char *ampls[] ;
extern const int ampls_len;

/*
 Power Conversion engine
 */
//{"dBm", "dBmV", "dBuV", "dBmA", "dBuA", "dB", "V", "W", "A", "mV", "mW", "mA"};
bool AmplConversion(double& value_in,
                    int in,
                    int out,
                    const double * params_list,
                    size_t params_list_len);

extern const char *temps_units[];
extern const int temps_units_len;

/*
 Temp Conversion engine
 */
bool TempConversion(double& value_in,
                    int in,
                    int out,
                    const double * params_list,
                    size_t params_list_len);

extern const char * currents[];
extern const int currents_len;

/*
 Current Conversion engine
 */

bool CurrentConversion(double& value_in,
                       int in,
                       int out,
                       const double * params_list,
                       size_t params_list_len);

extern const char * volumes[];
extern const int volumes_len;

/*
 Volume Conversion engine
 */

bool VolumeConversion(double& value_in,
                      int in,
                      int out,
                      const double * params_list,
                      size_t params_list_len);

extern const char * masses[];
extern const int masses_len;

/*
 Mass Conversion engine
 */

bool MassConversion(double& value_in,
                    int in,
                    int out,
                    const double * params_list,
                    size_t params_list_len);

extern const char * forces[];
extern const int forces_len;

/*
 Force Conversion engine
 */

bool ForceConversion(double& value_in,
                     int in,
                     int out,
                     const double * params_list,
                     size_t params_list_len);

extern const char * pressures[];
extern const int pressures_len;

/*
 Pressure Conversion engine
 */

bool PressureConversion(double& value_in,
                        int in,
                        int out,
                        const double * params_list,
                        size_t params_list_len);

extern const char * energies[];
extern const int energies_len;

/*
 Energy Conversion engine
 */

bool EnergyConversion(double& value_in,
                      int in,
                      int out,
                      const double * params_list,
                      size_t params_list_len);

extern const char * powers[];
extern const int powers_len;

/*
 Power Conversion engine
 */

bool PowerConversion(double& value_in,
                     int in,
                     int out,
                     const double * params_list,
                     size_t params_list_len);



