///-------------------------------------------------------------------------------------------------
// file:	Conversions.cpp
//
// summary:  Initializes the const char arrays that define the allowable conversions within a
//           AtomicUnit subclass.  Also defines the conversion functions.
//              IdentityConversion - (scalar conversion)
//              FreqConversion - conversion engine for units of frequency
//              AmplConversion - conversion engine for units of relative logarithmic power
//              TimeConversion - conversion between units of time
//              DistanceConversion - conversion engine for units of distance
//              TempConversion - conversion engine for units of temperature
//              CurrentConversion - conversion engine for units of electrical current
//              VolumeConversion - conversion engine for units of volume
//              MassConversion - conversion engine for units of mass
//              ForceConversion - conversion engine for units of force
//              PressureConversion - conversion engine for units of pressure
//              EnergyConversion - conversion engine for units of energy
//              PowerConversion - conversion engine for units of power
//              foundInUnits - utility function for finding the units
//
//
//  Copyright (c) 2014 by Michael Ryan
///-------------------------------------------------------------------------------------------------

#include "Conversions.h"

#ifdef WIN32

#define ISNAN(a)            _isnan(a)

#else

#define ISNAN(a)            isnan(a)

#endif

const char *scalar[] = {""};

// Does nothing except test for isnan
bool IdentityConversion(double& value_in,
                        int in,
                        int out,
                        const double * params_list,
                        size_t params_list_len)
{
	return (!ISNAN(value_in));
}


/* Frequency Units */
const char *freqs[] = {"Hz", "kHz", "MHz", "GHz"};
const int freqs_len = sizeof(freqs)/sizeof(char *);

/*
 Frequency Conversion engine
 */
bool FreqConversion(double& value_in,
                           int in,
                           int out,
                           const double * params_list,
                           size_t params_list_len)
{
	if ( in != out ) {
		value_in = value_in*pow(10, 3*((double)in-(double)out));
	}
    
	return (!ISNAN(value_in) );
};

/* Power Units */
// for future reference of use of dBmV/Hz values and dBmV/m values
/* per x values e.g. /Hz and /m^2 are undefined.  In the future these will use the parameter list to define the extra parameter to calculate these values */

const char *ampls[] = {"dBm", "dBmV", "dBuV", "dBmA", "dBuA", "dB", "V", "W", "A", "mV", "mW", "mA", "dBµV", "dBµA"};
const int ampls_len = sizeof(ampls)/sizeof(char *);

/*
 Power Ratio Conversion engine
 */

bool AmplConversion(double& value_in, int in, int out, const double * params_list, size_t params_list_len)
{
	double impedance = 50; // Assuming 50 Ohms
    
	if ( params_list && params_list_len > 0 ) {
		impedance = *params_list;
	}
    /* "dBm", "dBmV", "dBuV", "dBmA", "dBuA", "dB", "V", "W", "A", "mV", "mW", "mA", "dBµV", "dBµA"*/
    typedef enum {
        eUdBm = 0,
        eUdBmV,
        eUdBuV,
        eUdBmA,
        eUdBuA,
        eUdB,
        eUV,
        eUW,
        eUA,
        eUmV,
        eUmW,
        eUmA,
        eUdBmuV,
        eUdBmuA
    } AmplUnits;
    
	if ( in != out ) {
		switch(in) {
            case eUdBm: // dBm
                break;
            case eUdBmV: // dBmV
                value_in = value_in - 10.0*log10(impedance) - 30.0;
                break;
            case eUdBmuV:
            case eUdBuV: // dBuV
                value_in = value_in-10.0*log10(impedance) - 90.0;
                break;
            case eUdBmA: // dBmA
                value_in = value_in+10.0*log10(impedance) - 30.0;
                break;
            case eUdBmuA:
            case eUdBuA: // dBuA
                value_in = value_in+10.0*log10(impedance) - 90.0;
                break;
            case eUdB: // dB
                break;
            case eUmV: // mV
            case eUV: // V
                value_in = (20.0*log10(value_in))-(10*log10(impedance));
                if ( in == eUV ) {
                    value_in = value_in + 30.0;
                }    
                break;
            case eUmW: // mW
            case eUW: // W
                value_in = 10.0*log10(value_in);
                if ( in == eUW ) {
                    value_in = value_in + 30;
                }
                break;
            case eUmA: // mA
            case eUA: // A
                value_in = (20.0*log10(value_in)) + (10.0*log10(impedance));
                if ( in == eUA ) {
                    value_in = value_in + 30.0;
                }
                break;
		}
        
		switch(out) {
            case eUdBm: // dBm
                break;
            case eUdBmV: // dBmV
                value_in = value_in + 10.0*log10(impedance) + 30.0;
                break;
            case eUdBmuV:
            case eUdBuV: // dBuV
                value_in = value_in + 10.0*log10(impedance) + 90.0;
                break;
            case eUdBmA: // dBmA
                value_in = value_in - 10.0*log10(impedance) + 30.0;
                break;
            case eUdBmuA:
            case eUdBuA: // dBuA
                value_in = value_in - 10.0*log10(impedance) + 90.0;
                break;
            case eUdB: // dB
                break;
            case eUmV: // mV
            case eUV: // V
			{
                double conv = value_in + 10*log10(impedance);
                if ( out == eUV ) {
                    conv -= 30.0;
                }
                value_in = pow(10, conv/20);
                break;
			}
            case eUmW: // mW
            case eUW: // W
			{
                double conv = (value_in/10);
                if ( out == eUW ) {
                    conv -= 3;
                }
                value_in = pow(10, conv);
                break;
			}
            case eUmA: // mA
            case eUA: // A
			{
                double conv = (value_in - (10*log10(impedance)));
                if ( out == eUA ) {
                    conv -= 30;
                }
                value_in = pow(10, conv/20);
                break;
			}
		}
	}
    
	return (!ISNAN(value_in));
}


/* Time Units */
const char *times[] = {"fs", "ps", "ns", "us", "ms", "s", "min", "h", "day", "week", "wk", "µs"};
const int times_len = sizeof(times)/sizeof(char *);


/*
 Time Conversion engine
 */
bool TimeConversion(double& value_in,
                           int in,
                           int out,
                           const double * params_list,
                           size_t params_list_len)
{
	if ( in < 0 || out < 0 ) return false;
    
	if ( in != out ) {
		if ( in < 6 && out < 6 ) {
			value_in = value_in*pow(10, 3*(((double)in-(double)out)));
		}
		else {
			double factors[] = {1.0, 1.0};
			int indexes[] = { in, out };
            
			for(int i = 0; i < 2; i++ ) {
				int index = (i==0) ? in : out;
				switch(indexes[i]) {
                    case 0: //
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                        factors[i] = pow(10, 3*((double)index-5.0));
                        break;
                    case 6:
                    case 7:
                        factors[i] = pow(60, (double)index-5.0);
                        break;
                    case 8:
                        factors[i] = 86400.0;
                        break;
                    case 9:
                    case 10:
                        factors[i] = (7*86400.0);
                        break;
                    case 11:
                        factors[i] = pow(10, 3*(-2.0));
                        break;
				}
			}
            
			double ratio = factors[0]/factors[1];
			value_in = ratio*value_in;
		}
	}
    
	return (!ISNAN(value_in) );
};


/* Distance Units */
const char *dists[] = {"fm", "Å", "nm", "um", "µm", "mm", "cm", "m", "km", "nmi", "mil", "in", "ft", "yd", "fathom", "rod", "chain", "furlong", "mi", "stat mi", "AU", "ly", "parsec"};
const int dists_len = sizeof(dists)/sizeof(char *);

/*
 Distance Conversion engine
 */
bool DistanceConversion(double& value_in, int in, int out, const double * params_list, size_t params_list_len)
{
    const double mfactors[] = {
        1e-15, // fm
        1e-10, // Å
        1e-9, // nm
        1e-6, //um
        1e-6, //µm
        1e-3, // mm
        1e-2, // cm
        1, // m
        1e3, // km
        1852, // nmi
        2.54e-5, //mi
        .0254, // in
        .3048, // ft
        .9144, // yd
        1.8288, // fathom
        5.0292, // rod
        20.1168, // chain
        201.168, // furlong
        1609.344, // mi
        1609.3472, // stat mi
        149598000000.0, // AU
        9.4605284e+15, // ly
        3.08567758e+16 // parsec
    };
    
    int mfactors_len = sizeof(mfactors)/sizeof(double);
    
	if ( in < 0 || out < 0 || in > mfactors_len || out > mfactors_len ) return false;
    
	if ( in != out ) {
		double factor = mfactors[in]/mfactors[out];
		value_in = factor*value_in;
	}
	
	return (!ISNAN(value_in) );
}

/* Temperature Units */
const char *temps_units[] = {"K", "F", "C"};
const int temps_units_len = sizeof(temps_units)/sizeof(char *);

/*
 Temp Conversion engine
 */
bool TempConversion(double& value_in, int in, int out, const double * params_list, size_t params_list_len)
{
	if ( in != out ) {
		switch(in) {
            case 0: // Kelvin
                value_in -= 273.15;
                break;
            case 1: // Fahrenheit
                value_in = (value_in - 32)*(5.0/9.0);
                break;
            case 2: // Celsius
                break;
		}
        
		switch(out) {
            case 0: // Kelvin
                value_in += 273.15;
                break;
            case 1: // Fahrenheit
                value_in = ((value_in)*9.0/5.0) + 32;
                break;
            case 2: // Celsius
                break;
		}
	}
    
	return !(ISNAN(value_in));
}

/* Current Units */
const char *currents[] = {"nA", "uA", "mA", "A", "C/s", "kA"};
const int currents_len = sizeof(currents)/sizeof(char *);

/*
 Current Conversion engine
 */

bool CurrentConversion(double& value_in, int in, int out, const double * params_list, size_t params_list_len)
{
	double in_factor = 0;
	double out_factor = 0;
    // nA, uA, mA, A, C/s, kA
	const double factors[] = {-9, -6, -3, 0, 0, 3};
    int factors_len = sizeof(factors)/sizeof(double);
    
	if ( in != out ) {
		if ( in >= 0 && in < factors_len ) {
			in_factor = factors[in];
		}
        
		if ( out >= 0 && out < factors_len ) {
			out_factor = factors[out];
		}
        
		value_in = value_in/pow(10.0, out_factor-in_factor);
	}
    
	return !(ISNAN(value_in));
}

/* Volume Units */
const char *volumes[] = {"ml", "cm^3", "l", "m^3", "stere", "ft^3", "in^3", "board ft", "acre-ft", "dram", "drop", "tsp", "tbsp", "oz", "jigger", "gill", "cup", "pt", "fifth", "qt", "gal", "wbbl" /* wine barrel*/, "bbl" /* barrel  US 42*/, "UK oz", "UK gill", "UK gal", "dry pt", "dry qt", "dry gal", "peck", "bushel"};
const int volumes_len = sizeof(volumes)/sizeof(char *);

/*
 Volume Conversion engine
 */

bool VolumeConversion(double& value_in, int in, int out, const double * params_list, size_t params_list_len)
{
	double in_factor = 0;
	double out_factor = 0;
    
	const double factors[] = {
        .000001, // ml
        .000001, //cm^3
        .001, // l
        1, // m^3
        1, // stere
        .0283168, //ft^3
        1.6387e-5, // in^3
        2.3597e-3, // board ft
        1233.48, // acre-ft
        3.6967e-6, // dram
        5e-8, // drop
        4.9288e-6, // tsp
        1.4787e-5, // tbsp
        2.9574e-5, // oz
        4.4361e-5, // jigger
        1.1829e-4, // gill
        2.3659e-4, // cup
        4.7318e-4, // pt
        .00075,    // fifth (metric 750ml)
        9.4635e-4, // qt
        3.7854e-3, // gal
        .119240, //wbbl - Wine barrel
        0.1589868, //bbl
        2.8413e-5, //UK oz
        1.4206e-4, // UK gill
        4.54609e-3, // UK gal
        5.5061e-4, // dry pt
        1.1012e-3, // dry qt
        4.4049e-3, // dry gal
        8.8098e-3, // peck
        3.5239e-2 // bushel
    };
    int factors_len = sizeof(factors)/sizeof(double);
    
	if ( in != out ) {
		if ( in >= 0 && in < factors_len ) {
			in_factor = factors[in];
		}
        
		if ( out >= 0 && out < factors_len ) {
			out_factor = factors[out];
		}
        
        value_in = (in_factor*value_in)/out_factor;
	}
    
	return !(ISNAN(value_in));
}

/* Mass Units */
const char *masses[] = {"mg", "g", "kg", "Mg", "t", "lb", "troy", "gr", "scruple", "pennyweight", "dram", "oz", "troy oz", "carat", "stone", "slug", "hundredweight", "ton", "long ton"};
const int masses_len = sizeof(masses)/sizeof(char *);

/*
 Mass Conversion engine
 */

bool MassConversion(double& value_in, int in, int out, const double * params_list, size_t params_list_len)
{
	double in_factor = 0;
	double out_factor = 0;
    /*                         "mg", "g", "kg", "Mg", "t", "lb", "troy", "gr", "scruple", "pennyweight", "dram", "oz", "troy oz", "carat", "stone", "slug", "hundredweight", "ton", "long ton"*/
	const double factors[] = {
        .001, // mg
        1, //g
        1000, // kg
        1e+9, // Mg
        1e+9, // t
        453.59237, // lb
        373.2417216, // troy
        64.79891, // gr
        1.2960, // scruple
        1.5552, //pennyweight
        3.8879, // dram
        28.3495, // oz
        31.1035, // troy oz
        0.2, // carat
        6.35029, // stone
        14.5939, // slug
        50.8023, // hundredweight
        907185, // ton
        1016047 // long ton
        
    };
    
    int factors_len = sizeof(factors)/sizeof(double);
    
	if ( in != out ) {
		if ( in >= 0 && in < factors_len ) {
			in_factor = factors[in];
		}
        
		if ( out >= 0 && out < factors_len ) {
			out_factor = factors[out];
		}
        
        value_in = (in_factor*value_in)/out_factor;
	}
    
	return !(ISNAN(value_in));
}

/* Force Units */
const char *forces[] = {"dyne", "N", "kg*m/s^2", "ozf", "lbf", "gmf", "kgf", "kip", "ton-force"};
const int forces_len = sizeof(forces)/sizeof(char *);

/*
 Force Conversion engine
 */

bool ForceConversion(double& value_in, int in, int out, const double * params_list, size_t params_list_len)
{
	double in_factor = 0;
	double out_factor = 0;
    /*                     "dyne", "N", "kg*m/s^2", "ozf", "lbf", "gmf", "kgf", "kip", "ton-force"*/
	const double factors[] = {1e-5, 1, 1, .27801, 4.4482, 9806.65, 9.80665, 4448.2, 8896.4};
    int factors_len = sizeof(factors)/sizeof(double);
    
	if ( in != out ) {
		if ( in >= 0 && in < factors_len ) {
			in_factor = factors[in];
		}
        
		if ( out >= 0 && out < factors_len ) {
			out_factor = factors[out];
		}
        
        value_in = (in_factor*value_in)/out_factor;
	}
    
	return !(ISNAN(value_in));
}

/* Pressure Units */
const char *pressures[] = {"dyne/cm^2", "Pa", "torr", "N/cm^2", "mbar", "bar", "atm", "lbf/ft^2", "cm H20", "gmf/cm^2", "mm Hg", "cm Hg", "in H20", "in Hg", "lbf/in^2", "kgf/cm^2"};
const int pressures_len = sizeof(pressures)/sizeof(char *);


/*
 Pressure Conversion engine
 */

bool PressureConversion(double& value_in, int in, int out, const double * params_list, size_t params_list_len)
{
	double in_factor = 0;
	double out_factor = 0;

	const double factors[] = {
        .1, // dyne/cm^2
        1, // Pa
        133.3224, // torr
        10000, // N/cm^2
        100, // mbar
        100000, // bar
        101325, // atm
        47.880, // lbf/ft^2
        98.0637, // cm H20
        98.0665, // gmf/cm^2
        13332.24, // mm Hg
        1333.224, // cm Hg
        249.08, // in H20
        3386.4, // in Hg
        6894.8, // lbf/in^2
        98066.5 // kgf/cm^2
    };
    int factors_len = sizeof(factors)/sizeof(double);
    
	if ( in != out ) {
		if ( in >= 0 && in < factors_len ) {
			in_factor = factors[in];
		}
        
		if ( out >= 0 && out < factors_len ) {
			out_factor = factors[out];
		}
        
        value_in = (in_factor*value_in)/out_factor;
	}
    
	return !(ISNAN(value_in));
}

/* Energy Units */
const char *energies[] = {"erg", "ton TNT", "J", "kg*m^2/s^2", "ft-lbf", "cal th", "cal 15", "cal st", "W-s", "W-h", "therm",
    "therm US", "Btu th", "Btu 15", "Btu ST", "quad"};
const int energies_len = sizeof(energies)/sizeof(char *);

/*
 Energy Conversion engine
 */

bool EnergyConversion(double& value_in, int in, int out, const double * params_list, size_t params_list_len)
{
	double in_factor = 0;
	double out_factor = 0;
    
	const double factors[] = {
        1e-7, // erg
        4.184e+9, // ton TNT
        1, // J
        1, // kg*m^2/s^2
        1.3558, // ft-lbf
        4.184, // cal th
        4.1855, // cal 15
        4.1868, // cal st
        1, // W-s
        3600, // W-h
        1.05506e+8, // therm (E.C.)
        1.054804e+8, // therm (US)
        1054.350, // Btu th
        1054.728, // Btu 15
        1055.05585262, // Btu ST
        1e+18 // quad
    };
    int factors_len = sizeof(factors)/sizeof(double);
    
	if ( in != out ) {
		if ( in >= 0 && in < factors_len ) {
			in_factor = factors[in];
		}
        
		if ( out >= 0 && out < factors_len ) {
			out_factor = factors[out];
		}
        
        value_in = (in_factor*value_in)/out_factor;
	}
    
	return !(ISNAN(value_in));
}

/* Power Units */
const char *powers[] = {"cal/s", "cal th/s", "erg/s", "ft-lbf/h", "Btu th/h", "Btu/h", "W", "metric hp", "hp", "electric hp"};
const int powers_len = sizeof(powers)/sizeof(char *);

/*
 Power Conversion engine
 */

bool PowerConversion(double& value_in, int in, int out, const double * params_list, size_t params_list_len)
{
	double in_factor = 0;
	double out_factor = 0;
    
	const double factors[] = {
        4.1868, // cal/s
        4.184, // cal th/s
        1e-7, // erg/s
        3.7662e-4, // ft-lbf/h
        .292875, // Btu th/h
        .293071, // Btu/h
        1, // W
        735.50, // metric hp
        745.70, // hp
        746 // electric hp
    };
    int factors_len = sizeof(factors)/sizeof(double);
    
	if ( in != out ) {
		if ( in >= 0 && in < factors_len ) {
			in_factor = factors[in];
		}
        
		if ( out >= 0 && out < factors_len ) {
			out_factor = factors[out];
		}
        
        value_in = (in_factor*value_in)/out_factor;
	}
    
	return !(ISNAN(value_in));
}

///-------------------------------------------------------------------------------------------------
/// <summary>	foundUnitInUnits - Searches units for pUnit </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
/// <parameters>
///         pUnit - unit string to check
///         units - array of allowable units
///         units_len - number of allowable units
/// </parameters>
///-------------------------------------------------------------------------------------------------

bool foundUnitInUnits(const char * pUnit, const char ** units, const int units_len)
{
    bool bFound = false;
    
    for(int i = 0; i < units_len; i++)
    {
        if ( STRCMP(*(units+i), pUnit) == 0 )
        {
            bFound = true;
            break;
        }
    }
    
    return bFound;
}
