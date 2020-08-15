///-------------------------------------------------------------------------------------------------
// file:	UValue.hpp
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
#include "Conversions.h"

#ifndef __UVALUE_H__
#define __UVALUE_H__


#define MAP_ATOMIC_UNITS	std::map<std::string, AtomicUnit*>
#define LIST_UNIT_GROUP		std::list<UnitGroup>


// ***********************  DEFINES for UValues  ***********************

#define UVALUE_COMPARISON_NOT_DEFINED               0x00001
#define UVALUE_OPER_NOT_POSSIBLE                    0x00002

class UnitGroup;
class AtomicUnit;

class Reducer
{
protected:
	UnitGroup &parent;
public:
	Reducer(UnitGroup &ug) : parent(ug) {};
	virtual ~Reducer();

	UnitGroup &reduce();
};

class AtomicFormatter
{
protected:
	AtomicUnit &parent;
public:
	AtomicFormatter(AtomicUnit &au) : parent(au) {};
	virtual ~AtomicFormatter();

	virtual std::string print(void) = 0;
};
/*
class DegMinSec : public AtomicFormatter
{
private:
	const char reduce_chain[3][4] = {"deg", "min", "sec"};
	UnitGroup &parentGroup;
public:
	DegMinSec(UnitGroup &ug): parentGroup(ug) {this->parent(ug.getUnits()[AngleUnit().getName()]);}
	~DegMinSec() {}

	std::ostream &print(std::ostream &out) {

		double_uv val = 0.;
		int ival = (int)val;

		double_uv remainder = val - ival;

		out << ival;

		val = remainder * 60.0;
		ival = (int)val;

		remainder = val - ival;

		val = remainder * 60.0;
		ival = (int)val;

		return out;
	}
};
*/
///-------------------------------------------------------------------------------------------------
/// <summary>	Atomic Unit Class. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------

class AtomicUnit
{
private:
    const char *const* units;
	size_t len_units;
	int cur_index;
    
	double_uv * params_list;
	size_t params_list_len;
    
    bool reduce;

	UnitConversion conversion;

    size_t typeid_hash;
protected:
    

public:
    static const char exponentChar = '^';
    
    double exponent;

    const char * def_unit;
	const char * cur_unit;
    
    friend class UnitGroup;
    friend class UValue;
    
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit Constructor. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    /// <parameters>
    ///         units_in - *const* char to allowable unit abbreviations
    ///         len - number of abbreviations in units_in
    ///         UnitConversion - function pointer to conversion handler
    ///         current_unit - starting unit (abbrev. from units_in)
    ///         default_unit - default unit abbrev.
    ///         params_list - list of doubles for parameters not handled
    ///                         in UnitConversion (i.e. impedance for power conversions)
    ///         params_list_len - number of parameters in params_list
    /// </parameters>
    ///-------------------------------------------------------------------------------------------------
	explicit AtomicUnit(const char *const* units_in, int len, UnitConversion unit_conv);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit Constructor. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    /// <parameters>
    ///         units_in - ** char to allowable unit abbreviations
    ///         len - number of abbreviations in units_in
    ///         UnitConversion - function pointer to conversion handler
    ///         current_unit - starting unit (abbrev. from units_in)
    ///         default_unit - default unit abbrev.
    /// </parameters>
    ///-------------------------------------------------------------------------------------------------
	explicit AtomicUnit(const char *const* units_in,
                        int len, UnitConversion unit_conv,
                        const char * current_unit,
                        const char * default_unit = nullptr,
                        double_uv * params = nullptr, int params_len = 0);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit Copy Constructor. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	AtomicUnit(const AtomicUnit& au);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit Destructor. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	virtual ~AtomicUnit(void);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit Create. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	virtual AtomicUnit * create(void) = 0;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit Clone. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	virtual AtomicUnit * clone(void) const = 0;

    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit Convert. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	bool convert (double_uv& value_in, int unit_index_out);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit Test for Identical Units. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	bool sameUnits(const AtomicUnit * au) const ;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit Set Parameters List. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	void setParams(double_uv * params, size_t params_len);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit Get Parameters List. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
    const double_uv * getParams(void);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit Set Parameter in Parameters List. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	void setParam(double_uv param, size_t index);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit Get Parameter in Parameters List. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
    double_uv getParam(size_t index);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	[] Operator by index. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	const char * operator[] (const int index) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	[] Operator by Abbrev. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	int operator[] (const char * pszUnits) const;

    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit Comparision. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	bool operator==(const AtomicUnit * unit) const;

    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit Print Units to std::string. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	std::string printUnits(void);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit are same unit type. </summary>
    ///
    /// <remarks>	Michael Ryan, 6/11/20123 </remarks>
    ///-------------------------------------------------------------------------------------------------
    bool sameUnitType(const AtomicUnit * unitToCompare) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Atomic Unit get name. </summary>
    ///
    /// <remarks>	Michael Ryan, 6/11/20123 </remarks>
    ///-------------------------------------------------------------------------------------------------
    virtual const char * getName(void) const ;

private:
    AtomicUnit& operator=(const AtomicUnit &unit_in);
} ; // end class AtomicUnit

///-------------------------------------------------------------------------------------------------
/// <summary>	Atomic Unit << Operator. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, AtomicUnit * unit);


class ScalarUnit
	: public AtomicUnit
{
public:
	ScalarUnit(void);
    ScalarUnit * create(void);
	ScalarUnit * clone(void) const;
};


///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------

class UnitGroup {
private:
    // parameters of UnitGroup
    double_uv _val;
    char _oper;
	MAP_ATOMIC_UNITS _units;

	friend class UnitGroup;
	friend class Reducer;
	friend class AtomicFormatter;
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Unit Group Set Units. </summary>
	///
	/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
	///-------------------------------------------------------------------------------------------------
	void setUnits(const MAP_ATOMIC_UNITS &units);
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Compare Unit Groups. </summary>
	///
	/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
	///-------------------------------------------------------------------------------------------------
	int compareUnits(const MAP_ATOMIC_UNITS& units_in) const;
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Unit Groups Equal. </summary>
	///
	/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
	///-------------------------------------------------------------------------------------------------
	bool areUnitsEqual(const MAP_ATOMIC_UNITS& units_in) const;
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Fold in units from operating Unit Group according to supplied operator. </summary>
	///
	/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
	///-------------------------------------------------------------------------------------------------
	bool foldInUnit(char oper, const UnitGroup &unit) ;
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Sum Units. </summary>
	///
	/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
	///-------------------------------------------------------------------------------------------------
	bool sumUnits(char oper, const UnitGroup& unit);
public:
    UnitGroup(void);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Constructor. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    /// <parameters>
    ///         val - value to be saved to unit group
    ///         unit - unit type
    ///         op - operator relation to other unit groups
    /// </parameters>
    ///-------------------------------------------------------------------------------------------------
	UnitGroup(double_uv val, AtomicUnit * unit = nullptr, char op = 0);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Copy Constructor. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UnitGroup(double_uv val, char oper, const MAP_ATOMIC_UNITS &units_in);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Copy Constructor. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UnitGroup(const UnitGroup& unit_in);
    
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Destructor. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	~UnitGroup(void);
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Sum Units. </summary>
	///
	/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
	///-------------------------------------------------------------------------------------------------
	UnitGroup pow(double exponent) const;
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Round to number of digits. </summary>
	///
	/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
	///-------------------------------------------------------------------------------------------------
	UnitGroup roundTo(unsigned int num_of_digits) const;
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Reduce to more legible units series </summary>
	///
	/// <remarks>	Michael Ryan, 4/25/2016. </remarks>
	///-------------------------------------------------------------------------------------------------
	UnitGroup& reduce(const char * units, const Reducer& rule);
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Reduce to more legible units series </summary>
	///
	/// <remarks>	Michael Ryan, 4/25/2016. </remarks>
	///-------------------------------------------------------------------------------------------------
	UnitGroup& format(const char * units, const AtomicFormatter& format);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Comparison. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	bool operator==(const UnitGroup& unit) const;
	///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Multiplication. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UnitGroup operator*(const UnitGroup& unit) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Multiplication double_uv. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UnitGroup operator*(const double_uv& val) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group *=. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UnitGroup& operator*=(const UnitGroup& unit);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Division. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UnitGroup operator/(const UnitGroup& unit) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Division double_uv. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UnitGroup operator/(const double_uv& val) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group /=. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UnitGroup& operator/=(const UnitGroup& unit);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group /=. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UnitGroup& operator%=(const UnitGroup& unit);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Addition. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UnitGroup operator+(const UnitGroup& unit) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group +=. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UnitGroup& operator+=(const UnitGroup& unit);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Subtraction. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UnitGroup operator-(const UnitGroup& unit) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group -=. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UnitGroup& operator-=(const UnitGroup& unit);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Not Equals. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	bool operator!=(const UnitGroup& unit) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group [] of unit abbrev. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UnitGroup operator[](const char * pszNewUnits) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Get Units List. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
    const MAP_ATOMIC_UNITS& getUnits(void) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Get Value. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
    double_uv getValue(void) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Set Value. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
    void setValue(double_uv value);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Get Oper. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
    char getOper(void) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Set Oper. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
    void setOper(char oper);
    
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Delete Unit. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	void deleteUnit(std::string id);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Group Delete All Contained Units. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	void deleteAllUnits(void);
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group << operator. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
std::ostream& operator<< (std::ostream& out, const UnitGroup& units);

///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Container. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
class UValue
{
private:
    LIST_UNIT_GROUP _units;
    double _exponent;
public:
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Default Constructor. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/09/2014. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue(void);
	///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Constructor. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    /// <parameters>
    ///         init_val - initial value
    ///         unit - Unit pointer (Unit Value will manage this object from now on.  Do not delete.)
    /// </parameters>
    ///-------------------------------------------------------------------------------------------------
	UValue(double_uv init_val, AtomicUnit * unit = nullptr);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Constructor. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    /// <parameters>
    ///         init_val - initial value
    ///         unit - Unit pointer (Unit Value will manage this object from now on.  Do not delete.)
    /// </parameters>
    ///-------------------------------------------------------------------------------------------------
	UValue(double_uv init_val, const LIST_UNIT_GROUP &group_in);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Copy Constructor. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue(const UValue& val_in);
    
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Copy Constructor. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue(const LIST_UNIT_GROUP units_in, double exponent);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Destructor. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	~UValue(void);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Get number of terms. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	size_t numberOfTerms(void) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Get Polynomial Term (default is zero). </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	double_uv getValue(size_t termIndex = 0) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Get List of Polynomial Parameters. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	std::list<double_uv> getValues(void) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Set First Polynomial Value. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	void setValue(double_uv val);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Get First Unit. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	const char *const getUnit(void);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Get All Unit Groups. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	LIST_UNIT_GROUP getUnits(void) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Set Unit Groups. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
    void setUnits(const LIST_UNIT_GROUP &units);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Round To Number of Digits. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
    UValue roundTo(unsigned int num_of_digits) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value = Operator. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue& operator=(const UValue& val_in);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Comparison Operator. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	bool operator==(const UValue& val_in) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Not Equal Operator. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	bool operator!=(const UValue& val_in) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Multiplication. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue operator*(const UValue& val) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Multiplication double_uv. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue operator*(const double_uv& val) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Division. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue operator/(const UValue& val) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Division double_uv. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue operator/(const double_uv& val) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Addition. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue operator+(const UValue& val) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Subtraction. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue operator-(const UValue& val) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value +=. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue& operator*=(const UValue& val);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value /=. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue& operator/=(const UValue& val);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value +=. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue& operator+=(const UValue& val);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value -=. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue& operator-=(const UValue& val);
    /*
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value <. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	bool operator<(const UValue& val) const {
		bool is_less_than = false;
		const LIST_UNIT_GROUP& val_units = val.getUnits();

		for(LIST_UNIT_GROUP::const_iterator val_iter = val_units.begin();
			val_iter != val_units.end();
			val_iter++) {
			LIST_UNIT_GROUP::const_iterator iter = _units.begin();
			bool are_equal = false;
			for(;
				iter != _units.end() && !are_equal;
				iter++) {
					are_equal = (*iter).areUnitsEqual((*val_iter).getUnits());
			}
			
			if ( are_equal ) {
				if ( getValue() < val.getValue() ) {
					is_less_than = true;
				}
			}
			else {
				// comparing apples to oranges
				is_less_than = false;
				//write_error("False comparision.  Two different UValue objects.");
				break;
			}
		}

		return is_less_than;
	}
    */
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value [] with unit abbrev. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue operator[](const char * pszNewUnits) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value print. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	std::string print(int precision = -1,
		std::ios::fmtflags notation = std::ios::scientific, 
                      std::ios::fmtflags mask = std::ios::floatfield ) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value printUnits. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
    std::string printUnits(void) const;
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value get unit to power of exponent </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	UValue pow(double exponent) const;
private:
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value fold units. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	bool foldIntoUnits(char oper, const UValue& val_in, bool simplify = false);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value sum in units. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	bool sumInUnits(char oper, const UValue& units_in, bool simplify = false);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Delete Unit at Index. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	void deleteUnit(int index);
    ///-------------------------------------------------------------------------------------------------
    /// <summary>	Unit Value Delete All Units. </summary>
    ///
    /// <remarks>	Michael Ryan, 5/11/2012. </remarks>
    ///-------------------------------------------------------------------------------------------------
	void deleteAllUnits(void);
}; // class UValue

///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Multiplication. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue operator*(double_uv val1, const UValue& val2);
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Addition. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue operator+(double_uv val1, const UValue& val2);
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Subtraction. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue operator-(double_uv val1, const UValue& val2);
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Division. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue operator/(double_uv val1, const UValue& val2);
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value <<. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const UValue& val);

///-------------------------------------------------------------------------------------------------
/// <summary>	Macro for common form of static bool isAllowableUnit method </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------

#define STATIC_UNIT_MTHDS(pUnit, pUnitsArr, iUnitsArrLen)               \
static bool isAllowableUnit(const char * pUnit) {                       \
    return foundUnitInUnits(pUnit, pUnitsArr, iUnitsArrLen);            \
}                                                                       \
static const char *const* getUnits(void) {                              \
    return pUnitsArr;                                                   \
}                                                                       \
static int getUnitsLen(void) {                                          \
    return iUnitsArrLen;                                                \
}	

///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Frequency Unit. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------

class FrequencyUnit
	: public AtomicUnit
{
public:
	explicit FrequencyUnit(const char * cur_units = "MHz")
		: AtomicUnit(freqs, freqs_len, FreqConversion, cur_units, "MHz") 
	{
	}

	FrequencyUnit * create() 
	{
		return new FrequencyUnit();
	}

	FrequencyUnit * clone() const
	{
		return new FrequencyUnit(*this);
	}
    STATIC_UNIT_MTHDS(pUNit, freqs, freqs_len);
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Power Ratio Unit. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
class AmplUnit
	: public AtomicUnit
{
public:
	explicit AmplUnit(const char * cur_units = "dBm")
		: AtomicUnit(ampls, ampls_len, AmplConversion, cur_units, "dBm")
	{
	}

	AmplUnit * create(void)
	{
		return new AmplUnit();
	}

	AmplUnit * clone(void) const
	{
		return new AmplUnit(*this);
	}
    void setImpedance(double_uv impedance)
    {
        this->setParam(impedance, 0);
    }
    double_uv getImpedance(void)
    {
        return this->getParam(0);
    }
    STATIC_UNIT_MTHDS(pUNit, ampls, ampls_len);
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Time Interval Unit. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
class TimeUnit
	: public AtomicUnit
{
public:
	TimeUnit(const char * cur_units = "s")
		: AtomicUnit(times, times_len, TimeConversion, cur_units, "s")
	{
	}

	TimeUnit * create(void)
	{
		return new TimeUnit();
	}
	TimeUnit * clone(void) const
	{
		return new TimeUnit(*this);
	}
    STATIC_UNIT_MTHDS(pUNit, times, times_len);
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Distance Unit. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
class DistUnit
	: public AtomicUnit
{
public:
	DistUnit(const char * cur_units = "m")
		: AtomicUnit(dists, dists_len, DistanceConversion, cur_units, "m")
	{
	}

	DistUnit * create(void)
	{
		return new DistUnit();
	}
	DistUnit * clone(void) const
	{
		return new DistUnit(*this);
	}
    STATIC_UNIT_MTHDS(pUNit, dists, dists_len);
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Temperature Unit. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
class TempUnit
	: public AtomicUnit
{
public:
	TempUnit(const char * cur_units="C")
		: AtomicUnit(temps_units, temps_units_len, TempConversion, cur_units, "C")
	{
	}

	TempUnit * create(void)
	{
		return new TempUnit();
	}
	TempUnit * clone(void) const
	{
		return new TempUnit(*this);
	}
    STATIC_UNIT_MTHDS(pUNit, temps_units, temps_units_len);
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Current Unit. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
class CurrentUnit
	: public AtomicUnit
{
public:
	CurrentUnit(const char * cur_units="A")
		: AtomicUnit(currents, currents_len, CurrentConversion, cur_units, "A")
	{
	}

	CurrentUnit * create(void)
	{
		return new CurrentUnit();
	}
	CurrentUnit * clone(void) const
	{
		return new CurrentUnit(*this);
	}
    STATIC_UNIT_MTHDS(pUNit, currents, currents_len);
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Volume Unit. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
class VolumeUnit
: public AtomicUnit
{
public:
	VolumeUnit(const char * cur_units="l")
        : AtomicUnit(volumes, volumes_len, VolumeConversion, cur_units, "l")
	{
	}
    
	VolumeUnit * create(void)
	{
		return new VolumeUnit();
	}
	VolumeUnit * clone(void) const
	{
		return new VolumeUnit(*this);
	}
    STATIC_UNIT_MTHDS(pUNit, volumes, volumes_len);
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Mass Unit. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
class MassUnit
: public AtomicUnit
{
public:
	MassUnit(const char * cur_units="g")
        : AtomicUnit(masses, masses_len, MassConversion, cur_units, "g")
	{
	}
    
	MassUnit * create(void)
	{
		return new MassUnit();
	}
	MassUnit * clone(void) const
	{
		return new MassUnit(*this);
	}
    STATIC_UNIT_MTHDS(pUNit, masses, masses_len);
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Force Unit. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
class ForceUnit
: public AtomicUnit
{
public:
	ForceUnit(const char * cur_units="N")
        : AtomicUnit(forces, forces_len, ForceConversion, cur_units, "N")
	{
	}
    
	ForceUnit * create(void)
	{
		return new ForceUnit();
	}
	ForceUnit * clone(void) const
	{
		return new ForceUnit(*this);
	}
    STATIC_UNIT_MTHDS(pUNit, forces, forces_len);
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Pressure Unit. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
class PressureUnit
: public AtomicUnit
{
public:
	PressureUnit(const char * cur_units="Pa")
        : AtomicUnit(pressures, pressures_len, PressureConversion, cur_units, "Pa")
	{
	}
    
	PressureUnit * create(void)
	{
		return new PressureUnit();
	}
	PressureUnit * clone(void) const
	{
		return new PressureUnit(*this);
	}
    STATIC_UNIT_MTHDS(pUNit, pressures, pressures_len);
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Energy Unit. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
class EnergyUnit
: public AtomicUnit
{
public:
	EnergyUnit(const char * cur_units="J")
        : AtomicUnit(energies, energies_len, EnergyConversion, cur_units, "J")
	{
	}
    
	EnergyUnit * create(void)
	{
		return new EnergyUnit();
	}
	EnergyUnit * clone(void) const
	{
		return new EnergyUnit(*this);
	}
    STATIC_UNIT_MTHDS(pUNit, energies, energies_len);
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Power Unit. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
class PowerUnit
: public AtomicUnit
{
public:
	PowerUnit(const char * cur_units="W")
        : AtomicUnit(powers, powers_len, PowerConversion, cur_units, "W")
	{
	}
    
	PowerUnit * create(void)
	{
		return new PowerUnit();
	}
	PowerUnit * clone(void) const
	{
		return new PowerUnit(*this);
	}
    STATIC_UNIT_MTHDS(pUNit, powers, powers_len);
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Angle Unit. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
class AngleUnit
: public AtomicUnit
{
public:
    AngleUnit(const char * cur_units="deg")
    : AtomicUnit(angles, angles_len, AngleConversion, cur_units, "deg")
    {
    }
    
    AngleUnit * create(void)
    {
        return new AngleUnit();
    }
    AngleUnit * clone(void) const
    {
        return new AngleUnit(*this);
    }
    STATIC_UNIT_MTHDS(pUNit, angles, angles_len);
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Area Unit. </summary>
///
/// <remarks>	Michael Ryan, 8/14/2020. </remarks>
///-------------------------------------------------------------------------------------------------
class AreaUnit
: public AtomicUnit
{
public:
    AreaUnit(const char * cur_units="sq m")
    : AtomicUnit(areas, areas_len, AreaConversion, cur_units, "sq m")
    {
    }
    
    AreaUnit * create(void)
    {
        return new AreaUnit();
    }
    AreaUnit * clone(void) const
    {
        return new AreaUnit(*this);
    }
    STATIC_UNIT_MTHDS(pUNit, areas, areas_len);
};


#endif //__UVALUE_H__