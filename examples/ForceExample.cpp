
//
//  ForceExample.cpp
//  ForceExample
//
//  Created by Michael Ryan on 5/09/14.
//  Copyright (c) 2014 Michael Ryan. All rights reserved.
//

#include <stdlib.h>
#include <iostream>
#include "UValue.hpp"

int main(int argc, const char * argv[])
{
    const char * entries[] = {
        "Please enter the measured mass (in kg).", "Please enter the measured acceleration (in m/s^2)." };
    
    const int entries_len = sizeof(entries)/sizeof(char *);
    
    UValue units[entries_len];
    
    std::string user_in;
    
    for(int i = 0; i < entries_len; i++) {
        std::cout << entries[i] << std::endl;
        
        std::cin >> user_in;
        
        if ( std::cin.bad() || std::cin.fail() || !user_in.length() ) {
            std::cout << "Error in entry.  Please re-enter." << std::endl;
            i--;
        }
        else {
            char * pEndNumber = nullptr;
            double_uv val = strtod(user_in.c_str(), &pEndNumber);
            
            if ( !pEndNumber || pEndNumber == user_in.c_str()
                || val == HUGE_VAL || val == -HUGE_VAL ) {
                    std::cout << "Error: '" << user_in << "' is not a valid number.  Please re-enter." << std::endl;
                    i--;
            }
            else {
                if ( i == 0 ) {
                    units[i] = UValue(val, new MassUnit("kg"));
                }
                else if ( i == 1 ) {
                    UValue tUnit(1, new TimeUnit("s"));
                    units[i] = UValue(val, new DistUnit("m"));
                    units[i] /= (tUnit*tUnit);
                }
            }
        }
            
        std::cin.clear();
    }
    
    UValue uout = units[0]*units[1];
                            
    std::cout << "Your calculated force is " << uout << "." << std::endl;
                    
    return 0;
}


