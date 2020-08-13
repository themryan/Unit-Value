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
//                              VolumeUnit: Volume units class
//                              MassUnit: Mass units class
//                              ForceUnit: Force units class
//                              PressureUnit: Pressure units classs
//                              EnergyUnit: Energy units class
//                              PowerUnit: Power units class
//
//  Copyright (c) 2014 by Michael Ryan
///-------------------------------------------------------------------------------------------------
#include <string.h>

#ifndef __CONVERSIONS_H__
#define __CONVERSIONS_H__

#ifdef UNITS_CASE_INSENSITIVE

#define STRCMP(a, b)        stricmp(a, b)
#define STRNCMP(a, b, n)    strnicmp(a, b, n)

#else

#define STRCMP(a, b)        strcmp(a, b)
#define STRNCMP(a, b, n)    strncmp(a, b, n)

#endif

#ifdef UV_USE_LONG_DOUBLE
typedef long double double_uv;
#else
typedef double double_uv;
#endif

bool foundUnitInUnits(const char * pUnit, const char *const* units, const int units_len);

/*
 // This function pointer defines the format of the conversion class.
 // For each class a function matching this profile must be created.
 */

typedef bool (*UnitConversion)(
        double_uv& value_in,
        int unit_index_in,
        int unit_index_out,
        const double_uv * params_list,
        size_t params_list_len);

extern const char *const scalar[];

// Does nothing except test for isnan
bool IdentityConversion(double_uv& value_in,
                            int in,
                            int out,
                            const double_uv * params_list,
                            size_t params_list_len);

extern const char *const freqs[];
extern const int freqs_len;

/*
    Frequency Conversion engine
 */
bool FreqConversion(double_uv& value_in,
                           int in,
                           int out,
                           const double_uv * params_list,
                           size_t params_list_len);

extern const char *const times[];
extern const int times_len;

/*
 Time Conversion engine
 */
bool TimeConversion(double_uv& value_in,
                       int in,
                       int out,
                       const double_uv * params_list,
                       size_t params_list_len);


extern const char *const dists[];
extern const int dists_len;

/*
 Distance Conversion engine
 */
bool DistanceConversion(double_uv& value_in,
                        int in,
                        int out,
                        const double_uv * params_list,
                        size_t params_list_len);

extern const char *const ampls[] ;
extern const int ampls_len;

/*
 Power Conversion engine
 */
//{"dBm", "dBmV", "dBuV", "dBmA", "dBuA", "dB", "V", "W", "A", "mV", "mW", "mA"};
bool AmplConversion(double_uv& value_in,
                    int in,
                    int out,
                    const double_uv * params_list,
                    size_t params_list_len);

extern const char *const temps_units[];
extern const int temps_units_len;

/*
 Temp Conversion engine
 */
bool TempConversion(double_uv& value_in,
                    int in,
                    int out,
                    const double_uv * params_list,
                    size_t params_list_len);

extern const char *const currents[];
extern const int currents_len;

/*
 Current Conversion engine
 */

bool CurrentConversion(double_uv& value_in,
                       int in,
                       int out,
                       const double_uv * params_list,
                       size_t params_list_len);

extern const char *const volumes[];
extern const int volumes_len;

/*
 Volume Conversion engine
 */

bool VolumeConversion(double_uv& value_in,
                      int in,
                      int out,
                      const double_uv * params_list,
                      size_t params_list_len);

extern const char *const masses[];
extern const int masses_len;

/*
 Mass Conversion engine
 */

bool MassConversion(double_uv& value_in,
                    int in,
                    int out,
                    const double_uv * params_list,
                    size_t params_list_len);

extern const char *const forces[];
extern const int forces_len;

/*
 Force Conversion engine
 */

bool ForceConversion(double_uv& value_in,
                     int in,
                     int out,
                     const double_uv * params_list,
                     size_t params_list_len);

extern const char *const pressures[];
extern const int pressures_len;

/*
 Pressure Conversion engine
 */

bool PressureConversion(double_uv& value_in,
                        int in,
                        int out,
                        const double_uv * params_list,
                        size_t params_list_len);

extern const char *const energies[];
extern const int energies_len;

/*
 Energy Conversion engine
 */

bool EnergyConversion(double_uv& value_in,
                      int in,
                      int out,
                      const double_uv * params_list,
                      size_t params_list_len);

extern const char *const powers[];
extern const int powers_len;

/*
 Power Conversion engine
 */

bool PowerConversion(double_uv& value_in,
                     int in,
                     int out,
                     const double_uv * params_list,
                     size_t params_list_len);

extern const char *const angles[];
extern const int angles_len;

/*
 Angle Conversion engine
 */

bool AngleConversion(double_uv& value_in,
                     int in,
                     int out,
                     const double_uv * params_list,
                     size_t params_list_len);


#endif // __CONVERSIONS_H__