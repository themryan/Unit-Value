///-------------------------------------------------------------------------------------------------
// file:	UValue.cpp
//
// summary:  Initializes the const char arrays that define the allowable conversions within a
//           AtomicUnit subclass.
//
//
//  Copyright (c) 2014 by Michael Ryan
///-------------------------------------------------------------------------------------------------

#include <typeinfo>
#include "UValue.hpp"


// ***********************  DEFINES for UValues  ***********************

#define UVALUE_COMPARISON_NOT_DEFINED               0x00001
#define UVALUE_OPER_NOT_POSSIBLE                    0x00002


///-------------------------------------------------------------------------------------------------
/// <summary>	Atomic Unit Class. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------

    
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
///         params_list - list of doubles for parameters not handled
///                         in UnitConversion (i.e. impedance for power conversions)
///         params_list_len - number of parameters in params_list
/// </parameters>
///-------------------------------------------------------------------------------------------------
AtomicUnit::AtomicUnit(const char *const* units_in, int len, UnitConversion unit_conv)
            : exponent(1), units(units_in), len_units(len)
            , params_list(nullptr), params_list_len(0)
            , conversion(unit_conv), cur_unit(nullptr) {
    if ( units_in ) {
        cur_index = 0;
        def_unit = units_in[cur_index];
        cur_unit = units_in[cur_index];
    }
}
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
AtomicUnit::AtomicUnit(const char *const* units_in,
                    int len, UnitConversion unit_conv,
                    const char * current_unit,
                    const char * default_unit,
                    double * params, int params_len)
            : params_list(nullptr), cur_unit(nullptr)
            , exponent(1), units(units_in), len_units(len), params_list_len(0)
            , conversion(unit_conv) {
    if ( len_units ) {
        cur_index = (*this)[current_unit];
        cur_unit = units[cur_index];
        
        def_unit = units[(*this)[default_unit]];
        if ( !default_unit ) {
            def_unit = units[0];
        }
        
        if ( params != nullptr && params_len > 0 ) {
            params_list = new double[params_len];
            params_list_len = params_len;
            for(int i = 0; i < params_len; ++i) {
                params_list[i] = params[i];
            }
        }
    }
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Atomic Unit Copy Constructor. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
AtomicUnit::AtomicUnit(const AtomicUnit& au)
    : params_list(nullptr), params_list_len(0) {
    this->exponent = au.exponent;
    this->units = au.units;
    this->len_units = au.len_units;
    this->def_unit = au.def_unit;
    this->conversion = au.conversion;
    this->cur_unit = au.cur_unit;
    this->cur_index = au.cur_index;
    

    this->params_list = new double[au.params_list_len];
    for(size_t i = 0; i < au.params_list_len; ++i) {
        *(this->params_list+i) = *(au.params_list+i);
    }
    this->params_list_len = au.params_list_len;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Atomic Unit Destructor. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
AtomicUnit::~AtomicUnit(void) {    
    if ( params_list ) {
        delete [] params_list;
    }
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Atomic Unit Convert. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
bool AtomicUnit::convert (double& value_in, int unit_index_out)
{
    bool ret = false;
    double sign = 1.0;
    
    if ( (size_t)unit_index_out < len_units ) {
        if ( this->exponent != 0 ) {
            if ( this->exponent != 1 ) {
                sign = ( value_in < 0 ) ? -1.0 : sign;
                // ignore sign for now - imaginary numbers may come in the future
                value_in = pow(fabs(value_in), 1.0/exponent);
            }
            ret = conversion(value_in, cur_index, unit_index_out, params_list, params_list_len);
            if ( this->exponent != 1 ) {
                value_in = pow(value_in, exponent);
                value_in = sign*value_in;
            }
            if ( ret ) {
                cur_index = unit_index_out;
                cur_unit = units[cur_index];
            }
        }
    }
    
    return ret;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Atomic Unit Test for Identical Units. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
bool AtomicUnit::sameUnits(const AtomicUnit * au) const {
    bool ret = false;
    if ( au->len_units == len_units ) {
        ret = true;
        for(size_t i = 0; i < len_units; ++i ) {
            ret &= (strlen(units[i]) == strlen(au->units[i]) && (STRCMP(au->units[i], units[i]) == 0));
        }
    }
    
    return ret;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Atomic Unit Set Parameters List. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
void AtomicUnit::setParams(double * params, size_t params_len)
{
    if ( *params_list ) {
        delete [] params_list;
    }
    
    params_list = new double[params_len];
    
    memcpy(params_list, params, params_len);
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Atomic Unit Get Parameters List. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
const double * AtomicUnit::getParams(void)
{
    return params_list;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Atomic Unit Set Parameter in Parameters List. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
void AtomicUnit::setParam(double param, size_t index)
{
    double * params = params_list;
    size_t new_len = index+1;
    if ( new_len > params_list_len ) {
        params = new double[new_len];
        
        memcpy(params, params_list, new_len);
        
        delete [] params_list;
        params_list = params;
    }
    
    *(params + index) = param;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Atomic Unit Get Parameter in Parameters List. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
double AtomicUnit::getParam(size_t index)
{
    if ( index < params_list_len ) {
        return params_list[index];
    }
    return NAN;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	[] Operator by index. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
const char * AtomicUnit::operator[] (const int index) const {
    const char * ret = nullptr;
    if ( index > 0 && index < (int)len_units ) {
        ret = units[index];
    }
    
    return ret;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	[] Operator by Abbrev. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
int AtomicUnit::operator[] (const char * pszUnits) const {
    int ret = 0;
    if ( pszUnits && strlen(pszUnits) ) {
        for(size_t i = 0; i < len_units; ++i) {
            const char * unit = *(units+i);
            size_t len = strlen(pszUnits) ;
            if ( strlen(unit) == len
                && STRCMP(unit , pszUnits) == 0 ) {
                ret = (int)i;
                break;
            }
        }
    }
    return ret;
}
    
///-------------------------------------------------------------------------------------------------
/// <summary>	Atomic Unit Comparision. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
bool AtomicUnit::operator==(const AtomicUnit * unit) const {
    bool ret = true;
    
    ret &= ( exponent == unit->exponent ) ;
    
    if ( ret ) {
        ret = sameUnits(unit);
    }
    
    return ret;
}
    
///-------------------------------------------------------------------------------------------------
/// <summary>	Atomic Unit Print Units to std::string. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
std::string AtomicUnit::printUnits(void) {
    std::string out;
    
    if ( this->exponent != 0 ) {
        if ( strchr(this->cur_unit, ' ') != nullptr ) {
            out = "(";
            out += this->cur_unit;
            out += ")";
        }
        else {
            out = this->cur_unit;
        }
        
        if ( abs((int)this->exponent) > 1 ) {
            out += this->exponentChar + fabs(this->exponent);
        }
    }
    
    return out;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Atomic Unit are same unit type. </summary>
///
/// <remarks>	Michael Ryan, 6/11/20123 </remarks>
///-------------------------------------------------------------------------------------------------
bool AtomicUnit::sameUnitType(const AtomicUnit * unitToCompare) const {
    return (STRCMP(this->getName(), unitToCompare->getName()) == 0);
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Atomic Unit get name. </summary>
///
/// <remarks>	Michael Ryan, 6/11/20123 </remarks>
///-------------------------------------------------------------------------------------------------
const char * AtomicUnit::getName(void) const {
    return typeid(*this).name();
}
    

AtomicUnit& AtomicUnit::operator=(const AtomicUnit &unit_in) {
    return *this;
}

// end class AtomicUnit

std::ostream& operator<<(std::ostream& out, AtomicUnit * unit)
{
	if ( unit->exponent != 0 ) {
        if ( strchr(unit->cur_unit, ' ') != nullptr ) {
            out << "(" << unit->cur_unit << ")";
        }
        else {
            out << unit->cur_unit;
        }
        
		if ( abs((int)unit->exponent) > 1 ) {
			out << unit->exponentChar << fabs(unit->exponent);
		}
	}
    
	return out;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Scalar Unit - unitless class. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------

ScalarUnit::ScalarUnit(void)
    : AtomicUnit(scalar, 1, IdentityConversion, "", "")
{
}

ScalarUnit * ScalarUnit::create(void)
{
    return new ScalarUnit();
}

ScalarUnit * ScalarUnit::clone(void) const
{
    return new ScalarUnit(*this);
}

// end class ScalarUnit


///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group - group of folded units. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------


UnitGroup::UnitGroup(void)
    : _oper(0), _val(0)
{}
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
UnitGroup::UnitGroup(double val, AtomicUnit * unit, char op)
    : _oper(op), _val(val) {
    if ( unit != nullptr ) {
        std::string name = unit->getName();
        this->_units[name.c_str()] = unit->clone();
    }
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Copy Constructor. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup::UnitGroup(double val, char oper, const MAP_ATOMIC_UNITS &units_in)
    : _val(val), _oper(oper) {
    // remember that one can not simply copy MAP_ATOMIC_UNITS yet
    // one must clone each unit key-value pair
    for (MAP_ATOMIC_UNITS::const_iterator iter = units_in.begin() ; iter != units_in.end(); ++iter) {
        this->_units[iter->second->getName()] = iter->second->clone();
    }
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Copy Constructor. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup::UnitGroup(const UnitGroup& unit_in)
    : _val(unit_in.getValue()), _oper(unit_in.getOper()) {
    
    for(MAP_ATOMIC_UNITS::const_iterator iter = unit_in.getUnits().begin();
        iter != unit_in.getUnits().end();
        ++iter
        )
    {
        if ( (*iter).second != nullptr ) {
            std::string name = (*iter).first;
            this->_units[name] = ((*iter).second)->clone();
        }
    }
}
    
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Destructor. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup::~UnitGroup(void) {
    deleteAllUnits();
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Compare Unit Groups. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
int UnitGroup::compareUnits(const MAP_ATOMIC_UNITS& units_in) const {
    int ret = 0;
    
    if ( this->areUnitsEqual(units_in) ) {
        ret = 1;
    }
    
    return ret;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Groups Equal. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
bool UnitGroup::areUnitsEqual(const MAP_ATOMIC_UNITS& units_in) const {
    bool ret = false;
    
    if ( units_in.size() == this->_units.size() ) {
        ret = true;
        for(MAP_ATOMIC_UNITS::const_iterator iter = _units.begin();
            iter != _units.end();
            ++iter)
        {
            if ( units_in.find((*iter).first) != units_in.end() ) {
                ret &= (*(*iter).second == units_in.at((*iter).first));
            }
            else {
                ret = false;
            }
        }
    }
    
    return ret;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Fold in units from operating Unit Group according to supplied operator. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
bool UnitGroup::foldInUnit(char oper, const UnitGroup &unit) {
    bool ret = true;
    double conv_value = unit.getValue();
    if ( unit.getUnits().size() && _units.size() ) {
        for(MAP_ATOMIC_UNITS::const_iterator iter = unit.getUnits().begin();
            iter != unit.getUnits().end();
            ++iter) {
            AtomicUnit * au_in = iter->second;
            const std::string name = au_in->getName();
            if ( _units.count(name) == 1 ) {
                AtomicUnit * au = _units[name];
                if ( au->conversion(conv_value, (*au)[au_in->cur_unit], (*au)[au->cur_unit], au->params_list, au->params_list_len ) )
                {
                    switch(oper) {
                        case '*':
							au->exponent += au_in->exponent;
							break;
                        case '/':
							au->exponent -= au_in->exponent;
							break;

                    }
                    
                    if ( au->exponent == 0 ) {
                        deleteUnit(name);
                    }
                }
            } // if ( _units.count(name) == 1 )
            else {
                // copy
                AtomicUnit * clone = au_in->clone();
                
                switch(oper) {
                    case '*':
						clone->exponent = au_in->exponent;
						break;
                    case '/':
						clone->exponent = -au_in->exponent;
						break;
                }
                
                _units[au_in->getName()] = clone;
            }
        } // for iter != unit.getUnits().end()
        switch (oper) {
            case '*':
				_val *= conv_value;
				break;

            case '/':
				_val /= conv_value;
				break;
        }
    } // if ( unit.getUnits().size() && _units.size() )
    else {
        switch(oper) {
            case '*':
				_val *= unit.getValue();
				break;
            case '/':
				_val /= unit.getValue();
				break;
        }
    }
    
    return ret;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Sum Units. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
bool UnitGroup::sumUnits(char oper, const UnitGroup& unit) {
    bool ret = true;
    double conv_value = unit.getValue();
    
    if ( *this == unit ) {
        for(MAP_ATOMIC_UNITS::const_iterator iter = unit.getUnits().begin();
            iter != unit.getUnits().end();
            ++iter)
        {
            const AtomicUnit * au = unit.getUnits().at((*iter).first);
            if ( ((*iter).second)->conversion(conv_value,  (*au)[au->cur_unit]
                                              , (*au)[_units[(*iter).first]->cur_unit]
                                              , au->params_list, au->params_list_len ) ) {
                switch(oper) {
                    case '+':
						_val += conv_value;
						break;
                    case '-':
						_val -= conv_value;
						break;
                }
            }
        }
    } // if ( *this == unit )
    else {
        _val = double();
        ret = false;
    }
    
    return ret;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Sum Units. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup UnitGroup::pow(double exponent) const {
    double new_val = ::pow(_val, exponent);
    MAP_ATOMIC_UNITS new_units = this->_units;
    
    for(MAP_ATOMIC_UNITS::iterator iter = new_units.begin();
        iter != new_units.end();
        ++iter)
    {
        iter->second->exponent = iter->second->exponent * exponent;
    }
    
    return UnitGroup(new_val, this->_oper, new_units);
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Round to number of digits. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup UnitGroup::roundTo(unsigned int num_of_digits) const {
    double top = log10(fabs(this->_val));
    double cutoff = ::pow(10, int(num_of_digits-top-1));
    
    double val = ::round(this->_val*cutoff)/cutoff;
    
    return UnitGroup(val, this->_oper, this->_units);
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Comparison. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
bool UnitGroup::operator==(const UnitGroup& unit) const
{
    return unit.areUnitsEqual(this->_units);
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Multiplication. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup UnitGroup::operator*(const UnitGroup& unit) const {
    UnitGroup new_group = *this;
    
    new_group.foldInUnit('*', unit);
    
    return new_group;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Multiplication Double. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup UnitGroup::operator*(const double& val) const {
    UnitGroup new_group(*this);
    
    new_group.foldInUnit('*', UnitGroup(val, nullptr));
    
    return new_group;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group *=. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup& UnitGroup::operator*=(const UnitGroup& unit)
{
    this->foldInUnit('*', unit);
    
    return *this;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Division. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup UnitGroup::operator/(const UnitGroup& unit) const {
    UnitGroup new_group = *this;
    
    new_group.foldInUnit('/', unit);
    
    return new_group;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Division double. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup UnitGroup::operator/(const double& val) const {
    UnitGroup new_group(*this);
    
    new_group.foldInUnit('*', UnitGroup(1/val, nullptr));
    
    return new_group;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group /=. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup& UnitGroup::operator/=(const UnitGroup& unit) {
    this->foldInUnit('/', unit);
    
    return *this;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Addition. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup UnitGroup::operator+(const UnitGroup& unit) const {
    UnitGroup new_group = *this;
    new_group.sumUnits('+', unit);
    
    return new_group;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group +=. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup& UnitGroup::operator+=(const UnitGroup& unit) {
    this->sumUnits('+', unit);
    
    return *this;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Subtraction. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup UnitGroup::operator-(const UnitGroup& unit) const {
    UnitGroup new_group = *this;
    
    new_group.sumUnits('-', unit);
    
    return new_group;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group -=. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup& UnitGroup::operator-=(const UnitGroup& unit) {
    this->sumUnits('-', unit);
    
    return *this;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Not Equals. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
bool UnitGroup::operator!=(const UnitGroup& unit) const {
    return !(*this==unit);
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group [] of unit abbrev. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UnitGroup UnitGroup::operator[](const char * pszNewUnits) const {
    UnitGroup ng(*this);
    double val_in = _val;
    for(MAP_ATOMIC_UNITS::const_iterator iter = ng.getUnits().begin();
        iter != ng.getUnits().end();
        ++iter)
    {
        AtomicUnit *au = (*iter).second;
        int index = (*au)[pszNewUnits];
        if ( index != -1 ) {
            if ( au->convert(val_in, index) ) {
                ng.setValue(val_in);
            }
        }
    }
    
    return ng;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Get Units List. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
const MAP_ATOMIC_UNITS& UnitGroup::getUnits(void) const {
    return _units;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Set Units. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
void UnitGroup::setUnits(const MAP_ATOMIC_UNITS &units) {
    deleteAllUnits();
    _units = units;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Get Value. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
double UnitGroup::getValue(void) const {
    return _val;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Set Value. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
void UnitGroup::setValue(double value) {
    _val = value;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Get Oper. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
char UnitGroup::getOper(void) const {
    return _oper;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Set Oper. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
void UnitGroup::setOper(char oper) {
    _oper = oper;
}
    
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Delete Unit. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
void UnitGroup::deleteUnit(std::string id) {
    if ( _units.count(id) == 1 ) {
        AtomicUnit * unit = _units[id];
        
        if ( unit ) {
            delete unit;
        }
        
        _units.erase(id);
    }
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group Delete All Contained Units. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
void UnitGroup::deleteAllUnits(void) {
    for(MAP_ATOMIC_UNITS::iterator iter = _units.begin();
        _units.size() && iter != _units.end();
        ++iter) {
        
        if ( (*iter).second != nullptr ) {
            delete (*iter).second;
        }
    }
    
    _units.clear();
}

// end UnitGroup

///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Group << operator. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const UnitGroup& units)
{
	const MAP_ATOMIC_UNITS& aunits = units.getUnits();
	MAP_ATOMIC_UNITS numerator, denominator;
    
	for(MAP_ATOMIC_UNITS::const_iterator iter = aunits.begin();
		iter != aunits.end();
		++iter)
	{
		if ( (*iter).second->exponent > 0 ) {
			numerator[(*iter).second->getName()] = ((*iter).second)->clone();
		}
		else {
			denominator[(*iter).second->getName()] = ((*iter).second)->clone();
		}
	}
    
	bool fIsDenom = (denominator.size() > 0);
    
	if ( numerator.size() ) {
		if ( fIsDenom ) {
			out << "(";
		}
        
		for(MAP_ATOMIC_UNITS::const_iterator iter = numerator.begin();
			iter != numerator.end();
			++iter)
		{
			out << (AtomicUnit *)((*iter).second);
			delete (*iter).second;
		}
        
		numerator.clear();
        
		if ( fIsDenom ) {
			out << ")";
		}
	} // if ( numerator.size() )
	else {
		if ( denominator.size() ) {
			out << "1" ;
		}
	}
    
	if ( denominator.size() ) {
		out << "/(";
        
		for(MAP_ATOMIC_UNITS::const_iterator iter = denominator.begin();
			iter != denominator.end();
			++iter)
		{
			out << (AtomicUnit *)((*iter).second);
			delete (*iter).second;
		}
        
		denominator.clear();
        
		out << ")";
	}
    
	return out;
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Container - Contains polynomial of unit values contained
//              in unit groups. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------

///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Default Constructor. </summary>
///
/// <remarks>	Michael Ryan, 5/09/2014. </remarks>
///-------------------------------------------------------------------------------------------------
UValue::UValue(void) : _exponent(1) {};
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Constructor. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
/// <parameters>
///         init_val - initial value
///         unit - Unit pointer (Unit Value will manage this object from now on.  Do not delete.)
/// </parameters>
///-------------------------------------------------------------------------------------------------
UValue::UValue(double init_val, AtomicUnit * unit)
    : _exponent(1)
{
    _units.push_back(UnitGroup(init_val, unit));
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Constructor. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
/// <parameters>
///         init_val - initial value
///         unit - Unit pointer (Unit Value will manage this object from now on.  Do not delete.)
/// </parameters>
///-------------------------------------------------------------------------------------------------
UValue::UValue(double init_val, const LIST_UNIT_GROUP &group_in)
    : _exponent(1) {
    _units = group_in;
    (*_units.begin()).setValue(init_val);
}
    
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Copy Constructor. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue::UValue(const UValue& val_in)
    :_exponent(1) {
    const LIST_UNIT_GROUP& units_in = val_in.getUnits();
    
    this->_units.clear();
    this->_units = units_in;
}
    
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Copy Constructor. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue::UValue(const LIST_UNIT_GROUP units_in, double exponent)
    :_exponent(exponent), _units(units_in)
{
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Destructor. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue::~UValue(void)
{
    deleteAllUnits();
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Get number of terms. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
size_t UValue::numberOfTerms(void) const
{
    return _units.size();
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Get Polynomial Term (default is zero). </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
double UValue::getValue(size_t termIndex) const {
    double value = 0;
    if ( termIndex < _units.size() ) {
        LIST_UNIT_GROUP::const_iterator iter = _units.begin();
        std::advance(iter, termIndex);
        value = (*iter).getValue();
    }
    
    return value;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Get List of Polynomial Parameters. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
std::list<double> UValue::getValues(void) const
{
	std::list<double> values;
    for( LIST_UNIT_GROUP::const_iterator iter = _units.begin();
        iter != _units.end();
        ++iter)
    {
        values.push_back((*iter).getValue());
    }

    return values;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Set First Polynomial Value. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
void UValue::setValue(double val)
{
    (*_units.begin()).setValue(val);
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Get First Unit. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
const char *const UValue::getUnit(void)
{
    return (*(*_units.begin()).getUnits().begin()).second->cur_unit;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Get All Unit Groups. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
LIST_UNIT_GROUP UValue::getUnits(void) const
{
    //return units;
    return this->_units;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Set Unit Groups. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
void UValue::setUnits(const LIST_UNIT_GROUP &units) {
    this->_units.clear();
    this->_units = units;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Round To Number of Digits. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue UValue::roundTo(unsigned int num_of_digits) const {
    UValue new_value = *this;
    
    if ( num_of_digits > 0 )
    {
        LIST_UNIT_GROUP new_units;
        for(LIST_UNIT_GROUP::const_iterator iter = this->_units.begin();
            iter != this->_units.end()
            ; ++iter)
        {
            new_units.push_back(iter->roundTo(num_of_digits));
        }

        new_value.setUnits(new_units);
    }
    
    return new_value;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value = Operator. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue& UValue::operator=(const UValue& val_in) {
    const LIST_UNIT_GROUP& units_in = val_in.getUnits();
    
    this->_units.clear();
    this->_units = units_in;
    return *this;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Comparison Operator. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
bool UValue::operator==(const UValue& val_in) const {
    bool ret = false;
    const LIST_UNIT_GROUP& units_in = val_in.getUnits();
    
    if ( _units.size() == units_in.size() ) {
        ret = true;
        
        for(LIST_UNIT_GROUP::const_iterator self = _units.begin();
            self != _units.end();
            self++ ) {
            bool found = false;
            for(LIST_UNIT_GROUP::const_iterator iter = units_in.begin();
                iter !=  units_in.end();
                ++iter)
            {
                if ( (*iter) == (*self) ) {
                    if ( (*iter).getValue() == (*self).getValue() ) {
                        found = true;
                    }
                }
            }
            ret &= found;
        }
    }
    
    return ret;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Not Equal Operator. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
bool UValue::operator!=(const UValue& val_in) const {
    return !(*this==val_in);
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Multiplication. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue UValue::operator*(const UValue& val) const {
    UValue new_val = *this;
    new_val.foldIntoUnits('*', val);
    return new_val;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Multiplication Double. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue UValue::operator*(const double& val) const {
    UValue new_value(*this);
    UValue factor(val);
    new_value.foldIntoUnits('*', factor);
    return new_value;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Division. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue UValue::operator/(const UValue& val) const {
    UValue new_val = *this;
    new_val.foldIntoUnits('/', val);
    return new_val;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Division Double. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue UValue::operator/(const double& val) const {
    UValue new_value(*this);
    UValue factor(val);
    new_value.foldIntoUnits('/', factor);
    return new_value;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Addition. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue UValue::operator+(const UValue& val) const {
    UValue new_val = *this;
    new_val.sumInUnits('+', val);
    return new_val;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Subtraction. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue UValue::operator-(const UValue& val) const {
    UValue new_val = *this;
    new_val.sumInUnits('-', val);
    return new_val;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value +=. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue& UValue::operator*=(const UValue& val) {
    UValue& new_val = *this;
    new_val.foldIntoUnits('*', val);
    return new_val;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value /=. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue& UValue::operator/=(const UValue& val) {
    UValue& new_val = *this;
    new_val.foldIntoUnits('/', val);
    return new_val;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value +=. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue& UValue::operator+=(const UValue& val) {
    UValue& new_val = *this;
    new_val.sumInUnits('+', val);
    return new_val;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value -=. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue& UValue::operator-=(const UValue& val) {
    UValue& new_val = *this;
    new_val.sumInUnits('-', val);
    return new_val;
}
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
 ++val_iter) {
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
UValue UValue::operator[](const char * pszNewUnits) const {
    UValue new_value = *this;
    LIST_UNIT_GROUP ngroups;

    for(LIST_UNIT_GROUP::const_iterator iter = _units.begin();
        iter != _units.end();
        ++iter)
    {
        const UnitGroup& ug = (*iter);
        ngroups.push_back(ug[pszNewUnits]);
    }

    new_value.setUnits(ngroups);
    
    return new_value;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value print. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
std::string UValue::print(int precision,
                  std::ios::fmtflags notation,
                  std::ios::fmtflags mask ) const {
    std::stringstream out;
    
    if ( precision >= 0 ) {
        out.precision(precision);
        out.setf(notation, mask);
    }
    
    out << this->getValue() << " ";
    const LIST_UNIT_GROUP& uni = this->getUnits();
    bool fFirst = true;
    
    if ( uni.size() ) {
        for(LIST_UNIT_GROUP::const_iterator iter = uni.begin();
            iter != uni.end();
            ++iter)
        {
            if ( !fFirst ) {
                out << (*iter).getOper() << " ";
            }
            
            out << (*iter);
            
            fFirst = false;
        }
    } //if ( uni.size() )
    else {
        out << 0;
    }
    
    return out.str();
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value printUnits. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
std::string UValue::printUnits(void) const {
    std::stringstream ss;
    bool fFirst = true;
    
    if ( this->_units.size() ) {
        for(LIST_UNIT_GROUP::const_iterator iter = this->_units.begin();
            iter != this->_units.end();
            ++iter)
        {
            const UnitGroup &ug = *iter;
            if ( !fFirst ) {
                ss << ug.getOper() << " ";
            }
            
            ss << ug;
            
            fFirst = false;
        }
    }
    return ss.str();
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value get unit to power of exponent </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue UValue::pow(double exponent) const {
    LIST_UNIT_GROUP new_units = this->_units;
    int pow_to = (int)exponent;
    double frac = exponent - pow_to;
    double new_exponent = 0.0;
    
    if ( this->_units.size() > 1 )
    {
        pow_to *= int(1/this->_exponent);
        
        
        for(int i = 0; i < pow_to; i++)
        {
            for(LIST_UNIT_GROUP::iterator iter1 = new_units.begin();
                iter1 != new_units.end();
                ++iter1
                )
            {
                for(LIST_UNIT_GROUP::const_iterator iter2 = this->_units.begin();
                    iter2 != this->_units.end();
                    ++iter2
                    )
                {
                    *iter1 = (*iter1)*(*iter2);
                }
            }
        }
        
        if ( frac != 0 )
        {
            new_exponent = frac;
        }
    } // if ( this->_units.size() > 1 )
    else if (this->_units.size() == 1)
    {
        new_units.push_back((this->_units.front()).pow(exponent));
    }
    return UValue(new_units, new_exponent);
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value fold units. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
bool UValue::foldIntoUnits(char oper, const UValue& val_in, bool simplify)
{
    const LIST_UNIT_GROUP& unit_in = val_in.getUnits();
    bool ret = true;
    int i = 0;
    
    if ( !_units.empty() && !unit_in.empty() ) {
        LIST_UNIT_GROUP::iterator * deleteList = new LIST_UNIT_GROUP::iterator[_units.size()];
        for(LIST_UNIT_GROUP::iterator iter1 = _units.begin();
            iter1 != _units.end();
            ++iter1)
        {
            for(LIST_UNIT_GROUP::const_iterator iter2 = unit_in.begin();
                iter2 != unit_in.end();
                ++iter2)
            {
                switch(oper) {
                    case '*':
						*iter1 *= (*iter2);
						break;
                    case '/':
						*iter1 /= (*iter2);
						break;
                }
                
                if ( simplify && (*iter1).getValue() == 0 ) {
                    deleteList[i++] = iter1;
                }
            }
        }
        
        for(;i;i--) {
            _units.erase(deleteList[i-1]);
        }
        
        delete [] deleteList;
    }
    
    return ret;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value sum in units. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
bool UValue::sumInUnits(char oper, const UValue& units_in, bool simplify) {
    LIST_UNIT_GROUP units_copy = units_in.getUnits();
    bool ret = true;
    //bool added = false;
    double value = 0;
    
    int i = 0;
    LIST_UNIT_GROUP::iterator * deleteList = new LIST_UNIT_GROUP::iterator[units_copy.size()];
    for(LIST_UNIT_GROUP::iterator iter1 = _units.begin();
        iter1 != _units.end();
        ++iter1)
    {
        for(LIST_UNIT_GROUP::iterator iter2 = units_copy.begin();
            iter2 != units_copy.end();
            ++iter2)
        {
            if ( (*iter1) == (*iter2) ) {
                switch(oper) {
                    case '+':
						*iter1 += (*iter2);
						break;
                    case '-':
						*iter1 -= (*iter2);
						break;
                }
                
                value = (*iter1).getValue();
                
                deleteList[i++] = iter2;
            }
        }
    }
    
    for(;i;i--) {
        units_copy.erase(deleteList[i-1]);
    }
    
    delete [] deleteList;
    
    if ( (simplify && value != 0)
        || !simplify ) {
        if ( _units.size() ) {
            for(LIST_UNIT_GROUP::iterator iter2 = units_copy.begin();
                iter2 != units_copy.end();
                ++iter2) {
                _units.push_back(*iter2);
                (*iter2).setOper(oper);
            }
        }
    }
    else {
        deleteAllUnits();
    }
    
    return ret;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Delete Unit at Index. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
void UValue::deleteUnit(int i) {
    int j = 0;
    for(LIST_UNIT_GROUP::iterator iter = _units.begin();
        iter != _units.end() &&  j != i;
        ++iter, ++j)
    {
        if ( j == i ) {
            _units.erase(iter);
        }
    }
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Delete All Units. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
void UValue::deleteAllUnits(void)
{
    _units.clear();
}
// end class UValue

///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Multiplication. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue operator*(double val1, const UValue& val2)
{
	return val2*val1;
};
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Addition. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue operator+(double val1, const UValue& val2)
{
	return val2+val1;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Subtraction. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue operator-(double val1, const UValue& val2)
{
	return (val2*(-1))+val1;
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value Division. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
UValue operator/(double val1, const UValue& val2) {
	UValue num(1.0);
    
	return num/(val2/val1);
}
///-------------------------------------------------------------------------------------------------
/// <summary>	Unit Value <<. </summary>
///
/// <remarks>	Michael Ryan, 5/11/2012. </remarks>
///-------------------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const UValue& val) {
    std::string s = val.printUnits();
	out << val.getValue() << " " << s;
    
	return out;
}

