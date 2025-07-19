//! @file
//! @brief The value change dump source file.

/*---------------------------------------------------------------------*
 *  private: include files                                             *
 *---------------------------------------------------------------------*/

#include "value_change_dump.h"

#include <stddef.h>
#include <inttypes.h>


/*---------------------------------------------------------------------*
 *  private: definitions                                               *
 *---------------------------------------------------------------------*/

//! @brief Alias for the number 64
//!
#define BITS_U64 64


/*---------------------------------------------------------------------*
 *  private: macros like functions                                     *
 *---------------------------------------------------------------------*/

//! @brief Returns the lesser of two values
//! @param i1 First number
//! @param i2 Second number
//! @returns Smaller number
#define MIN(i1, i2) ( ((i1) < (i2)) ? (i1) : (i2) )

//! @brief Returns the greater of two values
//! @param i1 First number
//! @param i2 Second number
//! @returns Bigger number
#define MAX(i1, i2) ( ((i1) > (i2)) ? (i1) : (i2) )


/*---------------------------------------------------------------------*
 *  private: typedefs                                                  *
 *---------------------------------------------------------------------*/

//! @brief Structure of the buffer for writing a single bit
//!
typedef enum std_logic_string_e {
	STD_LOGIC_STRING_POSITION_0_VALUE = 0,   //!< Position of the value, '1' or '0'
	STD_LOGIC_STRING_POSITION_1_ID,          //!< Position of the id, any ASCII character
	STD_LOGIC_STRING_POSITION_2_NEWLINE,     //!< Position of the newline character '\n'
	STD_LOGIC_STRING_POSITION_3_END_OF_LINE, //!< Position of the end of line character '\0'
	STD_LOGIC_STRING_LENGTH_OF_BUFFER,       //!< Length of the buffer
}std_logic_string_t;


/*---------------------------------------------------------------------*
 *  private: variables                                                 *
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  public:  variables                                                 *
 *---------------------------------------------------------------------*/


const struct value_change_dump_sc value_change_dump =
{
	value_change_dump_open,
	{
		value_change_dump_print_header_01_date,
		value_change_dump_print_header_02_version,
		value_change_dump_print_header_03_timescale,
		value_change_dump_print_header_04_scope_start,
		value_change_dump_print_header_05_bit,
		value_change_dump_print_header_05_vector,
		value_change_dump_print_header_05_real,
		value_change_dump_print_header_05_string,
		value_change_dump_print_header_06_scope_end,
		value_change_dump_print_header_07_enddef,
		value_change_dump_print_header_08_dumpvars,
	},
	value_change_dump_print_timestamp,
	value_change_dump_print_bit,
	value_change_dump_print_std_logic,
	value_change_dump_print_vector,
	value_change_dump_print_real,
	value_change_dump_print_string,

	value_change_dump_close,
};


/*---------------------------------------------------------------------*
 *  private: function prototypes                                       *
 *---------------------------------------------------------------------*/

//! @brief Converts a value into a vector
//!
//! @details It is important to know that the least significant bit
//! is stored in the highest array index.
//!
//! @param value Value that is converted
//! @param bits Number of bits
//! @param[out] out Array into which is written
static void uint64_to_vector(uint64_t value, int bits, char *out);


/*---------------------------------------------------------------------*
 *  private: functions                                                 *
 *---------------------------------------------------------------------*/

static void uint64_to_vector(uint64_t value, int bits, char *out)
{
	if(NULL == out ){ return; }

	char * start = out;

	if(0 > bits)
	{
		bits =  MIN(-bits, 64);
		out += bits - 1;
		while(out >= start)
		{
			*out = 'x';
			out--;
		}
	}
	else
	{
		bits =  MIN(bits, 64);
		out += bits - 1;
		while(out >= start)
		{
			*out = (0x01 & value) ? '1' : '0';
			out--;
			value >>= 1;
		}
	}
    *(start + bits) = '\0';
}


/*---------------------------------------------------------------------*
 *  public:  functions                                                 *
 *---------------------------------------------------------------------*/

FILE * value_change_dump_open(const char * filename)
{
	return fopen(filename, "w");
}

void value_change_dump_print_header_01_date(FILE *f, const char * data)
{
	if(NULL == f || NULL == data){ return; }
	fprintf(f, "$date\n\t%s\n$end\n", data);
}

void value_change_dump_print_header_02_version(FILE *f, const char * version)
{
	if(NULL == f || NULL == version){ return; }
    fprintf(f, "$version\n\t%s\n$end\n", version);
}

void value_change_dump_print_header_03_timescale(FILE *f, const char * timescale)
{
	if(NULL == f || NULL == timescale){ return; }
    fprintf(f, "$timescale %s $end\n", timescale);
}

void value_change_dump_print_header_04_scope_start(FILE *f, const char * scope)
{
	if(NULL == f || NULL == scope){ return; }
    fprintf(f, "$scope module %s $end\n", scope);
}

void value_change_dump_print_header_05_bit(FILE *f, char id, const char * name)
{
	if(NULL == f || NULL == name){ return; }
	fprintf(f, "$var wire 1 %c %s $end\n", id, name);
}

void value_change_dump_print_header_05_vector(FILE *f, char id, int8_t msb, int8_t lsb, const char * name)
{
	if(NULL == f || NULL == name){ return; }
	int16_t length = (int16_t)( (int16_t)msb - (int16_t)lsb + 1 );
	if(INT8_MAX < length)
	{
		length = INT8_MAX;
	}
	else if(INT8_MIN > length)
	{
		length = INT8_MIN;
	}
	if(1 == length)
	{
		fprintf(f, "$var wire 1 %c %s $end\n", id, name);
	}
	else
	{
		fprintf(f, "$var wire %" PRIi8 " %c %s [%" PRIi8 ":%" PRIi8 "] $end\n",	length, id, name, msb, lsb);
	}
}

void value_change_dump_print_header_05_real(FILE *f, char id, const char * name)
{
	if(NULL == f || NULL == name){ return; }
	fprintf(f, "$var real 64 %c %s $end\n", id, name);
}

void value_change_dump_print_header_05_string(FILE *f, char id, const char * name)
{
	if(NULL == f || NULL == name){ return; }
	fprintf(f, "$var string 1 %c %s $end\n", id, name);
}

void value_change_dump_print_header_06_scope_end(FILE *f)
{
	if(NULL == f){ return; }
	fputs("$upscope $end\n", f);
}

void value_change_dump_print_header_07_enddef(FILE *f)
{
	if(NULL == f){ return; }
	fputs("$enddefinitions $end\n", f);
}

void value_change_dump_print_header_08_dumpvars(FILE *f)
{
	if(NULL == f){ return; }
	fputs("$dumpvars\n", f);
}

void value_change_dump_print_timestamp(FILE *f, uint64_t timestamp)
{
	if(NULL == f){ return; }
	fprintf(f, "#%" PRIu64 "\n", timestamp);
}

void value_change_dump_print_bit(FILE *f, char id, bool value)
{
	if(NULL == f){ return; }

	char str[STD_LOGIC_STRING_LENGTH_OF_BUFFER];

	str[STD_LOGIC_STRING_POSITION_0_VALUE] = value ? '1' : '0';
	str[STD_LOGIC_STRING_POSITION_1_ID] = id;
	str[STD_LOGIC_STRING_POSITION_2_NEWLINE] = '\n';
	str[STD_LOGIC_STRING_POSITION_3_END_OF_LINE] = '\0';

	fputs(str, f);
}

void value_change_dump_print_std_logic(FILE *f, char id, char state)
{
	if(NULL == f){ return; }

	char str[STD_LOGIC_STRING_LENGTH_OF_BUFFER];

	switch(state)
	{
		case STD_LOGIC_UNINITIALIZED:
		case 'u':
		case 'U':
			str[0] = 'U';
			break;

		case STD_LOGIC_FORCING_UNKNOWN:
		case 'x':
		case 'X':
			str[0] = 'X';
			break;

		case STD_LOGIC_FORCING_0:
		case '0':
			str[0] = '0';
			break;

		case STD_LOGIC_FORCING_1:
		case '1':
			str[0] = '1';
			break;

		case STD_LOGIC_HIGH_IMPEDANCE:
		case 'z':
		case 'Z':
			str[0] = 'Z';
			break;

		case STD_LOGIC_WEAK_UNKNOWN:
		case 'w':
		case 'W':
			str[0] = 'W';
			break;

		case STD_LOGIC_WEAK_0:
		case 'l':
		case 'L':
			str[0] = 'L';
			break;

		case STD_LOGIC_WEAK_1:
		case 'h':
		case 'H':
			str[0] = 'H';
			break;

		case STD_LOGIC_DO_NOT_CARE:
		case '-':
			str[0] = '-';
			break;

		default:
			str[0] = 'X';
			break;
	}

	str[STD_LOGIC_STRING_POSITION_1_ID] = id;
	str[STD_LOGIC_STRING_POSITION_2_NEWLINE] = '\n';
	str[STD_LOGIC_STRING_POSITION_3_END_OF_LINE] = '\0';

	fputs(str, f);
}

void value_change_dump_print_vector(FILE *f, char id, uint8_t length, uint64_t value)
{
	if(NULL == f){ return; }

	if(1 < length)
	{
		char vector[BITS_U64 + 1];
		uint64_to_vector(value, length, vector);
	    fprintf(f, "b" "%s" " %c" "\n", vector, id);
	}
	else if(1 == length)
	{
		value_change_dump_print_bit(f, id, value);
	}
}

void value_change_dump_print_real(FILE *f, char id, double value)
{
	if(NULL == f){ return; }

    fprintf(f, "r" "%.16g" " %c" "\n", value, id);
}

void value_change_dump_print_string(FILE *f, char id, const char * str)
{
	if(NULL == f || NULL == str){ return; }
    fprintf(f, "s" "%s" " %c" "\n", str, id);
}

void value_change_dump_close(FILE * f)
{
	if(NULL == f){ return; }

	fputs("$finish\n\n", f);
    fclose(f);
}


/*---------------------------------------------------------------------*
 *  private: inline functions                                          *
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  eof                                                                *
 *---------------------------------------------------------------------*/
