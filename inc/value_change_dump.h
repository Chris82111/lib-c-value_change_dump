//! @file
//! @brief The value change dump header file.
//!
//! @details The module can be used in C and C++.

#ifndef INC_VALUE_CHANGE_DUMP_H_
#define INC_VALUE_CHANGE_DUMP_H_

#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------*
 *  public: include files                                              *
 *---------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


/*---------------------------------------------------------------------*
 *  public: define                                                     *
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  public: typedefs                                                   *
 *---------------------------------------------------------------------*/

//! @brief Bit state/values for a single bit or an array/vector, in VHDL `std_logic` or `std_logic_vector`
//!
//! @details The logical values can be used as described:
//!
//! Character | Short           | Description
//! --------- | --------------- | ------------
//!     U     | uninitialized   | The signal has not been assigned a value yet
//!     X     | Forcing Unknown | The logic value is indeterminate
//!     0     | Forcing 0       | The logic value is 0
//!     1     | Forcing 1       | The logic value is 1
//!     Z     | high impedance  | The signal is effectively disconnected
//!     W     | weak unknown    | A weak signal that is not clearly defined as 0 or 1
//!     L     | weak 0          | A weak signal that is pulled low
//!     H     | weak 1          | A weak signal that is pulled high
//!     -     | don't care      | The value is irrelevant for the current operation
//!
typedef enum std_logic_e
{
	STD_LOGIC_UNINITIALIZED = 0,  //!< Uninitialized `'U'`, the signal has not been assigned a value yet
	STD_LOGIC_FORCING_UNKNOWN = 1,//!< Forcing unknown `'X'`, the logic value is indeterminate
	STD_LOGIC_FORCING_0 = 2,      //!< Forcing 0 `'0'`, the logic value is 0
	STD_LOGIC_FORCING_1 = 3,      //!< Forcing 1 `'1'`, the logic value is 1
	STD_LOGIC_HIGH_IMPEDANCE = 4, //!< High Impedance `'Z'`, the signal is effectively disconnected
	STD_LOGIC_WEAK_UNKNOWN = 5,   //!< Unknown `'W'`, a weak signal that is not clearly defined as 0 or 1
	STD_LOGIC_WEAK_0 = 6,         //!< Weak 0 `'L'`, a weak signal that is pulled low
	STD_LOGIC_WEAK_1 = 7,         //!< Weak 1 `'H'`, a weak signal that is pulled high
	STD_LOGIC_DO_NOT_CARE = 8,    //!< Don't care `'-'`, the value is irrelevant for the current operation
}std_logic_t;

//! @brief Represents a simplified form of a class
//! @details The global variable ::value_change_dump can be used to easily access all matching
//! functions with auto-completion.
struct value_change_dump_sc
{
	FILE * (*Open) (const char * filename); //!< @see ::value_change_dump_open()

	struct {
		void (*Print01_Date) (FILE *f, const char * data); //!< @see ::value_change_dump_print_header_01_date()
		void (*Print02_Version) (FILE *f, const char * version); //!< @see ::value_change_dump_print_header_02_version()
		void (*Print03_Timescale) (FILE *f, const char * timescale); //!< @see ::value_change_dump_print_header_03_timescale()
		void (*Print04_ScopeStart) (FILE *f, const char * scope); //!< @see ::value_change_dump_print_header_04_scope_start()
		void (*Print05_Bit) (FILE *f, char id, const char * name); //!< @see ::value_change_dump_print_header_05_bit()
		void (*Print05_Vector) (FILE *f, char id, int8_t msb, int8_t lsb, const char * name); //!< @see ::value_change_dump_print_header_05_vector()
		void (*Print05_Real) (FILE *f, char id, const char * name); //!< @see ::value_change_dump_print_header_05_real()
		void (*Print05_String) (FILE *f, char id, const char * name); //!< @see ::value_change_dump_print_header_05_string()
		void (*Print06_ScopeEnd) (FILE *f); //!< @see ::value_change_dump_print_header_06_scope_end()
		void (*Print07_Enddef) (FILE *f); //!< @see ::value_change_dump_print_header_07_enddef()
		void (*Print08_Dumpvars) (FILE *f); //!< @see ::value_change_dump_print_header_08_dumpvars()
	}Header; //!< Groups all header functions

	void (*PrintTimestamp) (FILE *f, uint64_t timestamp); //!< @see ::value_change_dump_print_timestamp()
	void (*PrintBit) (FILE *f, char id, bool value); //!< @see ::value_change_dump_print_bit()
	void (*PrintStdLogic) (FILE *f, char id, char state); //!< @see ::value_change_dump_print_std_logic()
	void (*PrintVector) (FILE *f, char id, uint8_t length, uint64_t value); //!< @see ::value_change_dump_print_vector()
	void (*PrintReal) (FILE *f, char id, double value); //!< @see ::value_change_dump_print_real()
	void (*PrintString) (FILE *f, char id, const char * str); //!< @see ::value_change_dump_print_string()

	void (*Close) (FILE * f); //!< @see ::value_change_dump_close()
};


/*---------------------------------------------------------------------*
 *  public: function prototypes                                        *
 *---------------------------------------------------------------------*/

//! @brief Creates a file or overwrites it for writing
//!
//! @details This file pointer must be closed, use ::fclose() or ::value_change_dump_close()
//!
//! @param[in] filename Path and file name as '\\0' terminated c string
//! @return Returns a file pointer for writing to the file
FILE * value_change_dump_open(const char * filename);

//! @brief Writes part of the header
//!
//! @details Date, normally use `__DATE__`
//!
//! @param[in,out] f File pointer to which is written
//! @param[in] data Date as '\\0' terminated c string
void value_change_dump_print_header_01_date(FILE *f, const char * data);

//! @brief Writes part of the header
//!
//! @details Information about the version
//!
//! @param[in,out] f File pointer to which is written
//! @param[in] version Version as '\\0' terminated c string
void value_change_dump_print_header_02_version(FILE *f, const char * version);

//! @brief Writes part of the header
//!
//! @details Time specification, consisting of an integer 1, 10 or 100 and unit.
//! This is appended without spaces, consists of a unit prefix and the time unit character 's'.
//! The following are valid: "s", "ms", "us", "ns", "ps", and "fs".
//! An example is "10ns"
//!
//! @param[in,out] f File pointer to which is written
//! @param[in] timescale Time scale as '\\0' terminated c string
void value_change_dump_print_header_03_timescale(FILE *f, const char * timescale);

//! @brief Writes part of the header
//!
//! @details Scope section as opening tag, can be nested and parallel,
//! requires a matching closing tag ::value_change_dump_print_header_06_scope_end()
//!
//! @param[in,out] f File pointer to which is written
//! @param[in] scope Name of the scope as '\\0' terminated c string
void value_change_dump_print_header_04_scope_start(FILE *f, const char * scope);

//! @brief Writes part of the header.
//! Optional, depending on the signal form here one bit (VHDL: `std_logic`)
//!
//! @details The same logical values can be used as described in enum ::std_logic_e
//!
//! @param[in,out] f File pointer to which is written
//! @param id An ID as a single character
//! @param[in] name Name of the signal as '\\0' terminated c string
void value_change_dump_print_header_05_bit(FILE *f, char id, const char * name);

//! @brief Writes part of the header.
//! Optional, depending on the signal form here multiple bits (VHDL: `std_logic_vector`)
//!
//! @details It is an array of bits with the same logical values can be
//! used as described in enum ::std_logic_e
//!
//! @param[in,out] f File pointer to which is written
//! @param id An ID as a single character
//! @param msb Most Significant Bit (MSB) index of the vector
//! @param lsb Least Significant Bit (LSB) index of the vector
//! @param[in] name Name of the signal as '\\0' terminated c string
void value_change_dump_print_header_05_vector(FILE *f, char id, int8_t msb, int8_t lsb, const char * name);

//! @brief Writes part of the header.
//! Optional, depending on the signal form here real a floating point value (VHDL: `real`)
//!
//! @details A floating point number, float, double, long double
//!
//! @param[in,out] f File pointer to which is written
//! @param id An ID as a single character
//! @param[in] name Name of the signal as '\\0' terminated c string
void value_change_dump_print_header_05_real(FILE *f, char id, const char * name);

//! @brief Writes part of the header.
//! Optional, depending on the signal form here a state/enum/string (VHDL: `enumerated type`)
//!
//! @details Ideal for writing the states of a state machine.
//!
//! @param[in,out] f File pointer to which is written
//! @param id An ID as a single character
//! @param[in] name Name of the signal as '\\0' terminated c string
void value_change_dump_print_header_05_string(FILE *f, char id, const char * name);

//! @brief Writes part of the header
//!
//! @details End of a scope section, requires a matching opening tag ::value_change_dump_print_header_04_scope_start()
//!
//! @param[in,out] f File pointer to which is written
void value_change_dump_print_header_06_scope_end(FILE *f);

//! @brief Writes part of the header
//!
//! @details Ends the definition
//!
//! @param[in,out] f File pointer to which is written
void value_change_dump_print_header_07_enddef(FILE *f);

//! @brief Writes part of the header
//!
//! @details Start of dump values, initial values of all variables dumped
//!
//! @param[in,out] f File pointer to which is written
void value_change_dump_print_header_08_dumpvars(FILE *f);

//! @brief Writes a timestamp
//!
//! @details Unitless number which is the result of the current run multiplied by the period time
//! - period time: Number without unit prefix and unit sign of the function ::value_change_dump_print_header_03_timescale().
//!   If `"10ns"` is used, the run is multiplied by 10.
//!
//! @param[in,out] f File pointer to which is written
//! @param timestamp Timestamp value
void value_change_dump_print_timestamp(FILE *f, uint64_t timestamp);

//! @brief Writes a signal, here one bit (VHDL: `std_logic`)
//!
//! @details It is a single bit, but only with the Boolean logical values 1 (true) and 0 (false)
//!
//! @param[in,out] f File pointer to which is written
//! @param id An ID as a single character
//! @param value Value of the bit
void value_change_dump_print_bit(FILE *f, char id, bool value);

//! @brief Writes a signal, here one bit of VHDL: `std_logic`
//!
//! @details The same logical values can be used as described in enum ::std_logic_e
//!
//! The value of @p state can be an index from 0 to 8 (enum ::std_logic_e) or
//! direct the ASCII character
//!
//! @param[in,out] f File pointer to which is written
//! @param id An ID as a single character
//! @param state Value of the bit
void value_change_dump_print_std_logic(FILE *f, char id, char state);

//! @brief Writes a signal, here a vector of bit (VHDL: `std_logic_vector`)
//!
//! @details It is an array (a vector) of single bits, but only with the Boolean logical values 1 (true) and 0 (false)
//!
//! The same logical values can be used as described in enum ::std_logic_e
//!
//! @param[in,out] f File pointer to which is written
//! @param id An ID as a single character
//! @param length Number of bits
//! @param value Value of a variable
void value_change_dump_print_vector(FILE *f, char id, uint8_t length, uint64_t value);

//! @brief Writes a signal, here a real a floating point value (VHDL: `real`)
//!
//! @details Can be any number up to a precision of 16 decimal places, with the formatting string `"%.16g"`.
//!
//! @param[in,out] f File pointer to which is written
//! @param id An ID as a single character
//! @param value Value of a variable
void value_change_dump_print_real(FILE *f, char id, double value);

//! @brief Writes the name of a state/enum/string (VHDL: `enumerated type`)
//!
//! @param[in,out] f File pointer to which is written
//! @param id An ID as a single character
//! @param[in] str String/Name of a state as '\\0' terminated c string
void value_change_dump_print_string(FILE *f, char id, const char * str);

//! @brief Closes the file handler
//!
//! @param[in,out] f File pointer to which is written
void value_change_dump_close(FILE * f);


/*---------------------------------------------------------------------*
 *  public: extern variables                                           *
 *---------------------------------------------------------------------*/

//! @brief To access all functions
//! @details Allows a simplified class to easily access all functions.
//! The auto-completion function helps you to select all
//! suitable functions via ::value_change_dump_sc struct.
extern const struct value_change_dump_sc value_change_dump;


/*---------------------------------------------------------------------*
 *  public: static inline functions                                    *
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  public: macros like functions                                      *
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  eof                                                                *
 *---------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif	/*	ifndef INC_VALUE_CHANGE_DUMP_H_	*/
