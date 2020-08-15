///-------------------------------------------------------------------------------------------------
// file:	ConverionsUnitTest.cpp
//
// summary:  Tests the conversions functions
//
//
//  Copyright (c) 2020 by Michael Ryan
///-------------------------------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <limits>
#include "Conversions.h"

#define SUCCESS_COLOR           "\033[1;32m"
#define FAILED_COLOR            "\033[1;31m"
#define NOMINAL_COLOR           "\033[0m"
#define YELLOW_COLOR            "\033[1;33m"

int failed_test(std::string &test_name, int index, double_uv val)
{
    std::ostringstream err_string;

    err_string << "Failed " ;
    err_string << test_name;
    err_string << " at index " << index ;
    err_string << " at val = " << val;

    std::cout << FAILED_COLOR << err_string.str() << NOMINAL_COLOR << std::endl;

    return 1;
}

bool compare_double_uv(const double_uv &val1, 
                        const double_uv &val2, 
                        double_uv &diff)
{
    bool is_comparable = true;

    // obtain a magnitude of accuracy for a chosen range
    if( val1 != val2 && abs((val1 - val2)/val1) >= 1e-5 )
    {
        std::cout << abs(val1 - val2)/val1 << std::endl;
        std::cout << val2 << std::endl;
        is_comparable = false;
    }

    diff = abs(val2 - val1)/val1;

    return is_comparable;
}

bool standard_test(double_uv &val_in, 
                    UnitConversion func, 
                    const double_uv params_to_check[], 
                    int params_to_check_len, 
                    int def_unit, int &index, double_uv &max_diff)
{
    double_uv val = val_in;
    double_uv diff = 0;
    max_diff = 0;

    for( index = 0; index < params_to_check_len; index++)
    {   
        bool successful = func(val, def_unit, index, NULL, 0);

        if( !successful || !compare_double_uv(val, params_to_check[index], diff) )
        {
            val_in = val;
            return false;
        }

        if( diff > max_diff )
        {
            max_diff = diff;
        }

        val = val_in;
    }

    return true;
}

int main()
{
    std::string test_name;
    double_uv val = 0;
    double_uv ep = std::numeric_limits<double_uv>::epsilon();
    double_uv max_diff = 0;
    int index;

    // Does nothing except test for isnan
    test_name = "Identity Conversion";
    if( !IdentityConversion(val, 0, 0, NULL, 0) )
    {
        return failed_test(test_name, 0, val);
    }

    val = nan("");
    if( IdentityConversion(val, 0, 0, NULL, 0) )
    {
        return failed_test(test_name, 0, val);
    }

    // Frequency Conversion Tests

    val = 1.0 ;
    test_name = "Frequency Conversion";
    //const char *const freqs[] = {"Hz", "kHz", "MHz", "GHz"};
    const double_uv freq_vals_valid[] = { 1, 1e-3, 1e-6, 1e-9 };
    if( !standard_test(val, FreqConversion, freq_vals_valid, freqs_len, 0, index, max_diff) )
    {
        return failed_test(test_name, index, val);
    }

    std::cout << SUCCESS_COLOR << test_name << " passed " << NOMINAL_COLOR << std::endl;
    std::cout << YELLOW_COLOR << test_name << " had max error between expected and measured " << max_diff << NOMINAL_COLOR << std::endl;

    val = 1.0;
    test_name = "Time Conversion";
    // const char *const times[] = {"fs", "ps", "ns", "us", "ms", "s", "min", "h", "day", "week", "wk", "µs"};
    const double_uv times_vals_valid[] 
        = { 1e+15, 1e+12, 1e+9, 1e+6, 1e+3, 1.0, 1./60.0, 1./(3600.0), 1./(3600.0*24.0), 1./(3600.0*24.0*7.0), 1./(3600.0*24.0*7.0), 1e+6 };
    if( !standard_test(val, TimeConversion, times_vals_valid, times_len, 5, index, max_diff) )
    {
        return failed_test(test_name, index, val);
    }

    std::cout << SUCCESS_COLOR << test_name << " passed " << NOMINAL_COLOR << std::endl;
    std::cout << YELLOW_COLOR << test_name << " had max error between expected and measured " << max_diff << NOMINAL_COLOR << std::endl;

    val = 1.0;
    test_name = "Distance Conversion";
    // const char *const dists[] = {"fm", "A", "nm", "um", "µm", "mm", "cm", "m", "km", "nmi", "mil", "1/64 in", "1/32 in", "1/16 in", "1/8 in", "1/4 in", "1/2 in", "in", "ft", "yd", "fathom", "rod", "chain", "furlong", "mi", "geo mi", "league", "AU", "ly", "parsec"};
    const double_uv distance_vals_valid[]
        = { 1.e+15, 1.e+10, 1e+9, 1e+6, 1e+6, 1e+3, 1e+2, 
            1.0, 1e-3, 1/1852., 1/2.54e-5, 64./.0254, 32./.0254, 
            16./.0254, 8./.0254, 4./.0254, 2./.0254, 1/.0254, 
            1/.3048, 1/0.9144, 1/1.8288, 1/5.0292, 1/20.1168, 1/201.168, 
            1/1609.344, 1/1828.8, 1/1609.344, 1/149598000000.0, 
            1/9.4605284e+15, 1/3.08567758e+16 };
    if( !standard_test(val, DistanceConversion, distance_vals_valid, dists_len, 7, index, max_diff) )
    {
        return failed_test(test_name, index, val);
    }

    std::cout << SUCCESS_COLOR << test_name << " passed " << NOMINAL_COLOR << std::endl;
    std::cout << YELLOW_COLOR << test_name << " had max error between expected and measured " << max_diff << NOMINAL_COLOR << std::endl;

    /* Temperature Units */
    val = 1.0;
    test_name = "Temperature Conversion";
    // const char *const temps_units[] = {"K", "F", "C"};
    const double_uv temp_vals_valid[] = { 274.15,  33.8, 1.0 };
    if( !standard_test(val, TempConversion, temp_vals_valid, temps_units_len, 2, index, max_diff) )
    {
        return failed_test(test_name, index, val);
    }

    std::cout << SUCCESS_COLOR << test_name << " passed " << NOMINAL_COLOR << std::endl;
    std::cout << YELLOW_COLOR << test_name << " had max error between expected and measured " << max_diff << NOMINAL_COLOR << std::endl;

    /* Current Units */
    val = 1.0;
    test_name = "Electric Current Conversion";
    // const char *const currents[] = {"nA", "uA", "mA", "A", "C/s", "kA"};
    const double_uv current_vals_valid[] = { 1e+9, 1e+6, 1e+3, 1.0, 1.0, 1e-3 };
    if( !standard_test(val, CurrentConversion, current_vals_valid, currents_len, 3, index, max_diff) )
    {
        return failed_test(test_name, index, val);
    }

    std::cout << SUCCESS_COLOR << test_name << " passed " << NOMINAL_COLOR << std::endl;
    std::cout << YELLOW_COLOR << test_name << " had max error between expected and measured " << max_diff << NOMINAL_COLOR << std::endl;

    /* Volume Units */
    val = 1.0;
    test_name = "Volume Conversion";
    /* const char *const volumes[] = {"ml", "cm^3", "l", "m^3", "stere", "ft^3", "in^3", "board ft", "acre-ft", 
        "drop", "fifth", "dram", "tsp", "tbsp", "jigger", "oz", 
        "gill", "cup", "pt", "qt", "gal", "wbbl", "bbl", "Imp dram",
        "Imp tsp", "Imp tbsp", "Imp jigger", "Imp fl oz", "Imp gill", "Imp cup", "Imp pt", "Imp qt", "Imp gal", 
        "metric dram", "metric tsp", "metric tbsp", "metric jigger", "metric cup", "AU tbsp", "JP cup",
        "UK gill", "dry pt", "dry qt", "dry gal", "peck", "bushel"}; */

        const double_uv volume_vals_valid[] = { 
            1e+06, // ml
            1e+06, // cm^3
            1000, // l
            1, // m^3
            1, // stere
            35.3147, // ft^3
            61024, // in^3
            423.783, // (board ft)
            0.000810714, // acre-ft
            2e+07, // drop
            1333.33, // fifth
            270512, // dram
            202889, // tsp
            67627, // tbsp
            22542.3, // jigger
            33813.5, // oz
            8453.8, // gill
            4226.72, // cup
            2113.36, // pt
            1056.69, // qt
            264.173, // gal
            8.38645, // wbbl
            6.28983, // bbl
            281690, // (Imp dram)
            168936, // (Imp tsp)
            56312, // (Imp tbsp)
            28571.4, // (Imp jigger)
            35195.2, // (Imp fl oz)
            12500, // (Imp gill)
            3519.5, // (Imp cup)
            1759.75, // (Imp pt)
            879.877, // (Imp qt)
            219.969, // (Imp gal)
            333333, // (metric dram)
            200000, // (metric tsp)
            66666.7, // (metric tbsp)
            40000, // (metric jigger)
            4000, // (metric cup)
            50000, // (AU tbsp)
            5000, // (JP cup)
            7039.28, // (UK gill)
            1816.17, // (dry pt)
            908.1, // (dry qt)
            227.02, // (dry gal)
            113.51, // peck
            28.3776 // bushel    
        };

    if( !standard_test(val, VolumeConversion, volume_vals_valid, volumes_len, 3, index, max_diff) )
    {
        return failed_test(test_name, index, val);
    }

    std::cout << SUCCESS_COLOR << test_name << " passed " << NOMINAL_COLOR << std::endl;
    std::cout << YELLOW_COLOR << test_name << " had max error between expected and measured " << max_diff << NOMINAL_COLOR << std::endl;

    /* Mass Units */
    val = 1.0;
    test_name = "Mass Conversion";
    // const char *const masses[] = {"mg", "g", "kg", "Mg", "t", "lb", "troy", "gr", "scruple", "pennyweight", "dram", "oz", "troy oz", "carat", "stone", "slug", "hundredweight", "ton", "long ton"};
    const double_uv mass_vals_valid[] = { 1e+3, 1.0, 1e-3, 1e-09, 1e-09, 0.00220462, 0.00267923, 0.0154324, 0.771605, 0.643004, 0.257208, 0.035274, 0.0321507, 5, 0.157473, 0.0685218, 0.0196841, 1.10231e-06, 9.84206e-07 };
    if( !standard_test(val, MassConversion, mass_vals_valid, masses_len, 1, index, max_diff) )
    {
        return failed_test(test_name, index, val);
    }

    std::cout << SUCCESS_COLOR << test_name << " passed " << NOMINAL_COLOR << std::endl;
    std::cout << YELLOW_COLOR << test_name << " had max error between expected and measured " << max_diff << NOMINAL_COLOR << std::endl;

    /* Force Units */
    val = 1.0;
    test_name = "Force Conversion";
    // const char *const forces[] = {"dyne", "N", "kg*m/s^2", "ozf", "lbf", "gmf", "kgf", "kip", "ton-force"};
    const double_uv forces_vals_valid[] = { 100000, 1, 1, 3.59699, 0.22481, 0.000101972, 0.101972, 0.00022481, 0.000112405 };
    if( !standard_test(val, ForceConversion, forces_vals_valid, forces_len, 1, index, max_diff) )
    {
        return failed_test(test_name, index, val);
    }

    std::cout << SUCCESS_COLOR << test_name << " passed " << NOMINAL_COLOR << std::endl;
    std::cout << YELLOW_COLOR << test_name << " had max error between expected and measured " << max_diff << NOMINAL_COLOR << std::endl;

    /* Pressure Units */
    val = 1.0;
    test_name = "Pressure Conversion";
    // const char *const pressures[] = {"dyne/cm^2", "Pa", "torr", "N/cm^2", "mbar", "bar", "atm", "lbf/ft^2", "cm H20", "gmf/cm^2", "mm Hg", "cm Hg", "in H20", "in Hg", "lbf/in^2", "kgf/cm^2"};
    const double_uv pressure_vals_valid[] = { 1.01325e+06, 101325, 760, 10.1325, 1013.25, 1.01325, 1, 2116.23, 1033.26, 1033.23, 7.6, 76, 406.797, 29.9212, 14.6959, 1.03323 };
    if( !standard_test(val, PressureConversion, pressure_vals_valid, pressures_len, 6, index, max_diff) )
    {
        return failed_test(test_name, index, val);
    }

    std::cout << SUCCESS_COLOR << test_name << " passed " << NOMINAL_COLOR << std::endl;
    std::cout << YELLOW_COLOR << test_name << " had max error between expected and measured " << max_diff << NOMINAL_COLOR << std::endl;

    /* Pressure Units */
    val = 1.0;
    test_name = "Energy Conversion";
    // const char *const energies[] = {"erg", "ton TNT", "mJ", "J", "MJ", "kg*m^2/s^2", "ft-lbf", "cal th", "cal 15", "cal st", "W-s", "W-h", "therm",
    const double_uv energy_vals_valid[] = { 1e+07, 2.39006e-10, 1000, 1, 1e-06, 1, 0.737572, 0.239006, 0.23892, 0.238846, 1, 0.000277778, 9.47813e-9, 9.48043e-09, 0.000948452, 0.000948112, 0.000947817, 1e-18 };
    if( !standard_test(val, EnergyConversion, energy_vals_valid, energies_len, 3, index, max_diff) )
    {
        return failed_test(test_name, index, val);
    }

    std::cout << SUCCESS_COLOR << test_name << " passed" << NOMINAL_COLOR << std::endl;
    std::cout << YELLOW_COLOR << test_name << " had max error between expected and measured " << max_diff << NOMINAL_COLOR << std::endl;

    /* Power Units */
    val = 1.0;
    test_name = "Power Conversion";
    // const char *const powers[] = {"cal/s", "cal th/s", "erg/s", "ft-lbf/h", "Btu th/h", "Btu/h", "mW", "W", "kW", "MW", "metric hp", "hp", "electric hp"};
    const double_uv power_vals_valid[] = { 0.238846, 0.239006, 1e+07, 2655.2, 3.41443, 3.41214, 1000, 1, 0.001, 1e-06, 0.00135962, 0.00134102, 0.00134048 };
    if( !standard_test(val, PowerConversion, power_vals_valid, powers_len, 7, index, max_diff) )
    {
        return failed_test(test_name, index, val);
    }

    std::cout << SUCCESS_COLOR << test_name << " passed " << NOMINAL_COLOR << std::endl;
    std::cout << YELLOW_COLOR << test_name << " had max error between expected and measured " << max_diff << NOMINAL_COLOR << std::endl;

    /* Angle Units */
    val = 1.0;
    test_name = "Angle Conversion";
    // const char *const angles[] = { "mil", "°", "deg", "'", "min", "\"", "sec",  "'''", "rad", "grad"};
    const double_uv angle_vals_valid[] = { 6400, 1, 1, 1, 60, 60, 3600, 3600, 0.0174533, 1.11111 };
    if( !standard_test(val, AngleConversion, angle_vals_valid, angles_len, 2, index, max_diff) )
    {
        return failed_test(test_name, index, val);
    }

    std::cout << SUCCESS_COLOR << test_name << " passed " << NOMINAL_COLOR << std::endl;
    std::cout << YELLOW_COLOR << test_name << " had max error between expected and measured " << max_diff << NOMINAL_COLOR << std::endl;

    /* Area Units */
    val = 1.0;
    test_name = "Area Conversion";
    // const char *const areas[] = { "ab", "fb", "pb", "nb", "µb", "um", "mb", "barn", "kb", "Mb", "sq mm", "sq cm", "sq m", "sq km", "Hectacre", "myriad", "sq mil", "sq in", "sq ft", "square", "sq yard", "acre", "sq mi", "sq survey mi", "section", "survey township" };
    const double_uv area_vals_valid[] = { 
        1e+46, // ab
        1e+43, // fb
        1e+40, // pb
        1e+37, // nb
        1e+34, // µb
        1e+34, // um
        1e+31, // mb
        1e+28, // barn
        1e+25, // kb
        1e+22, // Mb
        1e+06, // (sq mm)
        10000, // (sq cm)
        1, // (sq m)
        1e-06, // (sq km)
        0.0001, // Hectacre
        1e-10, // myriad
        1.55e+09, // (sq mil)
        1550, // (sq in)
        10.7639, // (sq ft)
        10.7639, // square
        1.19599, // (sq yard)
        0.000247105, // acre
        3.86102e-07, // (sq mi)
        3.86101e-07, // (sq survey mi)
        3.86102e-07, // section
        1.0725e-08, // (survey township)
    };
    
    if( !standard_test(val, AreaConversion, area_vals_valid, areas_len, 12, index, max_diff) )
    {
        return failed_test(test_name, index, val);
    }

    std::cout << SUCCESS_COLOR << test_name << " passed " << NOMINAL_COLOR << std::endl;
    std::cout << YELLOW_COLOR << test_name << " had max error between expected and measured " << max_diff << NOMINAL_COLOR << std::endl;

    return 0;
}