
//
//  UnitsExample.cpp
//  UnitsExample
//
//  Created by Michael Ryan on 3/14/13.
//  Copyright (c) 2014 Michael Ryan. All rights reserved.
//

#include <stdlib.h>
#include <iostream>
#include "UValue.h"

int main(int argc, const char * argv[])
{

    while(1) {
        int index = 1;
        int c_index = 0;
        const char * choices[] = {"Amplitude (Rel. Log) Unit", "Distance Unit", "Frequency Unit",
            "Time Unit", "Temperature Unit", "Electrical Current Unit", "Volume Unit",
            "Mass Unit", "Force Unit", "Pressure Unit", "Energy Unit", "Power Unit"};
        const char * def_units[] = { "dBm '50 Ohm Impedance'", "m", "MHz", "s", "C", "A", "m^3", "g", "N", "Pa", "J", "W" };
        std::cout << "Choose a unit:" << std::endl;
        for(int i = 0; i < sizeof(choices)/sizeof(char *)-1; i++) {
            std::cout << index++ << ". " << choices[c_index++] << std::endl;
        }
        
        std::cout << index++ << ". " << choices[c_index] << std::endl;
        std::cout << index << ". Exit" << std::endl;
        
        int exit_index = index;
        
        std::string choice;
        std::cin.sync();
        std::cin >> choice ;
        
        std::cin.clear();
        
        if ( choice.length() > 0 ) {
            if ( isdigit(choice[0]) ) {
                int c = atoi(choice.c_str());
                if ( c > 0 && c < exit_index ) {
                    std::string def_unit;
                    
                    std::cout << "'" << choices[c-1] << "' chosen" << std::endl << std::endl;
                    
                    bool playingwithunits = true;
                    while(playingwithunits) {
                        std::string value_str;
                        std::string units_str;
                        std::string temp_unit;
                        std::cout << "Enter a value (default unit is " << def_units[c-1] << ")." << std::endl;
                        std::cin >> value_str;
                        int next_c = std::cin.peek();
                        int max_iters = 0;
                        while ( next_c == ' ' ) {
                            std::cin.get();
                            next_c = std::cin.peek();
                        }
                        while (next_c != '\n'
                               && next_c != std::istream::eofbit
                               && next_c != std::istream::badbit
                               && next_c != std::istream::failbit
                               && max_iters++ < 10) {
                                std::cin >> temp_unit;
                                if ( temp_unit.compare(units_str) == 0 ) {
                                    break;
                                }
                                if ( units_str.length() ) {
                                    units_str += " ";
                                }
                                units_str += temp_unit;
                            
                                next_c = std::cin.peek();
                                while ( next_c == ' ') {
                                    std::cin.get();
                                    next_c = std::cin.peek();
                                }
                        }
                        
                        std::cin.clear();
                        
                        char * endOfNumber = nullptr;
                        double val_dbl = std::strtod(value_str.c_str(), &endOfNumber);
                        if ( endOfNumber == value_str.c_str()
                            || endOfNumber == nullptr
                            || val_dbl == HUGE_VAL
                            || val_dbl == -HUGE_VAL ) {
                            std::cout << "Error: '" << value_str << "' is not a number.  Please re-enter." << std::endl;
                        }
                        else {
                            long eonIndex = endOfNumber - value_str.c_str();
                            if ( eonIndex != value_str.length() ) {
                                temp_unit = endOfNumber;
                                if ( units_str.length() ) {
                                    units_str +=  " " ;
                                }
                                units_str += temp_unit;
                            }
                            
                            UValue val;
                            int num_conv;
                            const char **unit;
                            
                            switch (c) {
                                default:
                                case 1:
                                    if ( units_str.length() && !AmplUnit::isAllowableUnit(units_str.c_str()) )
                                    {
                                        std::cout << units_str << " is not an allowable unit of " << choices[0];
                                        std::cout << std::endl;
                                        units_str = "dBm";
                                    }
                                    else if ( !units_str.length() )
                                    {
                                        units_str = "dBm";
                                    }
                                
                                    val = UValue(val_dbl, new AmplUnit(units_str.c_str()));
                                    num_conv = ampls_len;
                                    unit = ampls;
                                    break;
                                case 2:
                                    if ( units_str.length() && !DistUnit::isAllowableUnit(units_str.c_str()) )
                                    {
                                        std::cout << units_str << " is not an allowable unit of " << choices[1];
                                        std::cout << std::endl;
                                        units_str = "m";
                                    }
                                    else if ( !units_str.length() )
                                    {
                                        units_str = "m";
                                    }
                                    
                                    val = UValue(val_dbl, new DistUnit(units_str.c_str()));
                                    num_conv = dists_len;
                                    unit = dists;
                                    break;
                                case 3:
                                    if ( units_str.length() && !FrequencyUnit::isAllowableUnit(units_str.c_str()) )
                                    {
                                        std::cout << units_str << " is not an allowable unit of " << choices[2];
                                        std::cout << std::endl;
                                        units_str = "MHz";
                                    }
                                    else if ( !units_str.length() )
                                    {
                                        units_str = "MHz";
                                    }
                                    
                                    val = UValue(val_dbl, new FrequencyUnit(units_str.c_str()));
                                    num_conv = freqs_len;
                                    unit = freqs;
                                    break;
                                case 4:
                                    if ( units_str.length() && !TimeUnit::isAllowableUnit(units_str.c_str()) )
                                    {
                                        std::cout << units_str << " is not an allowable unit of " << choices[3];
                                        std::cout << std::endl;
                                        units_str = "s";
                                    }
                                    else if ( !units_str.length() )
                                    {
                                        units_str = "s";
                                    }
                                    
                                    val = UValue(val_dbl, new TimeUnit(units_str.c_str()));
                                    num_conv = times_len;
                                    unit = times;
                                    break;
                                case 5:
                                    if ( units_str.length() && !TempUnit::isAllowableUnit(units_str.c_str()) )
                                    {
                                        std::cout << units_str << " is not an allowable unit of " << choices[4];
                                        std::cout << std::endl;
                                        units_str = "C";
                                    }
                                    else if ( !units_str.length() )
                                    {
                                        units_str = "C";
                                    }
                                    
                                    val = UValue(val_dbl, new TempUnit(units_str.c_str()));
                                    num_conv = temps_units_len;
                                    unit = temps_units;
                                    break;
                                case 6:
                                    if ( units_str.length() && !CurrentUnit::isAllowableUnit(units_str.c_str()) )
                                    {
                                        std::cout << units_str << " is not an allowable unit of " << choices[5];
                                        std::cout << std::endl;
                                        units_str = "A";
                                    }
                                    else if ( !units_str.length() )
                                    {
                                        units_str = "A";
                                    }
                                    
                                    val = UValue(val_dbl, new CurrentUnit(units_str.c_str()));
                                    num_conv = currents_len;
                                    unit = currents;
                                    break;
                                case 7:
                                    if ( units_str.length() && !VolumeUnit::isAllowableUnit(units_str.c_str()) )
                                    {
                                        std::cout << units_str << " is not an allowable unit of " << choices[6];
                                        std::cout << std::endl;
                                        units_str = "m^3";
                                    }
                                    else if ( !units_str.length() )
                                    {
                                        units_str = "m^3";
                                    }
                                    
                                    val = UValue(val_dbl, new VolumeUnit(units_str.c_str()));
                                    num_conv = volumes_len;
                                    unit = volumes;
                                    break;
                                case 8:
                                    if ( units_str.length() && !MassUnit::isAllowableUnit(units_str.c_str()) )
                                    {
                                        std::cout << units_str << " is not an allowable unit of " << choices[7];
                                        std::cout << std::endl;
                                        units_str = "g";
                                    }
                                    else if ( !units_str.length() )
                                    {
                                        units_str = "g";
                                    }
                                    
                                    val = UValue(val_dbl, new MassUnit(units_str.c_str()));
                                    num_conv = masses_len;
                                    unit = masses;
                                    break;
                                case 9:
                                    if ( units_str.length() && !MassUnit::isAllowableUnit(units_str.c_str()) )
                                    {
                                        std::cout << units_str << " is not an allowable unit of " << choices[8];
                                        std::cout << std::endl;
                                        units_str = "N";
                                    }
                                    else if ( !units_str.length() )
                                    {
                                        units_str = "N";
                                    }
                                    
                                    val = UValue(val_dbl, new ForceUnit(units_str.c_str()));
                                    num_conv = forces_len;
                                    unit = forces;
                                    break;
                                case 10:
                                    if ( units_str.length() && !PressureUnit::isAllowableUnit(units_str.c_str()) )
                                    {
                                        std::cout << units_str << " is not an allowable unit of " << choices[9];
                                        std::cout << std::endl;
                                        units_str = "Pa";
                                    }
                                    else if ( !units_str.length() )
                                    {
                                        units_str = "Pa";
                                    }
                                    
                                    val = UValue(val_dbl, new PressureUnit(units_str.c_str()));
                                    num_conv = pressures_len;
                                    unit = pressures;
                                    break;
                                case 11:
                                    if ( units_str.length() && !EnergyUnit::isAllowableUnit(units_str.c_str()) )
                                    {
                                        std::cout << units_str << " is not an allowable unit of " << choices[10];
                                        std::cout << std::endl;
                                        units_str = "J";
                                    }
                                    else if ( !units_str.length() )
                                    {
                                        units_str = "J";
                                    }
                                    
                                    val = UValue(val_dbl, new EnergyUnit(units_str.c_str()));
                                    num_conv = energies_len;
                                    unit = energies;
                                    break;
                                case 12:
                                    if ( units_str.length() && !PowerUnit::isAllowableUnit(units_str.c_str()) )
                                    {
                                        std::cout << units_str << " is not an allowable unit of " << choices[10];
                                        std::cout << std::endl;
                                        units_str = "W";
                                    }
                                    else if ( !units_str.length() )
                                    {
                                        units_str = "W";
                                    }
                                    
                                    val = UValue(val_dbl, new PowerUnit(units_str.c_str()));
                                    num_conv = powers_len;
                                    unit = powers;
                                    break;
                                    
                            }
                            
                            std::cout << std::endl;
                            
                            std::cout << "Current allowed conversions are: " << std::endl << std::endl;
                            UValue writ = val[units_str.c_str()];
                            for(int unit_index = 0; unit_index < num_conv; unit_index++) {
                                std::cout << writ[*(unit+unit_index)] << std::endl;
                            }
                            std::cout << std::endl << "Conversions squared are: " << std::endl << std::endl;
                            writ = writ*writ;
                            for(int unit_index = 0; unit_index < num_conv; unit_index++) {
                                std::cout << writ[*(unit+unit_index)] << std::endl;
                            }
                            std::cout << std::endl << "Conversions summed (3*unit + unit) are: " << std::endl << std::endl;
                            writ = 3*val + val; // So this equals 4*unit
                            for(int unit_index = 0; unit_index < num_conv; unit_index++) {
                                std::cout << writ[*(unit+unit_index)] << std::endl;
                            }
                            
                            std::cout << std::endl;
                            
                            playingwithunits = false;
                        }
                    }
                }
                else {
                    if ( c == exit_index ) {
                        std::cout << "Bye!" << std::endl;
                        break;
                    }
                    else
                        std::cout << "Please enter again" << std::endl;
                }
            }
            else {
                std::cout << "Not a digit" << std::endl << std::endl;
            }
        }
        else {
            std::cout << "Nothing entered" << std::endl << std::endl;
        }
    }
    return 0;
}


