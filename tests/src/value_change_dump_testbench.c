/*---------------------------------------------------------------------*
 *  private: include files
 *---------------------------------------------------------------------*/

#include <stdio.h>
#include <inttypes.h>

#include "value_change_dump_testbench.h"
#include "value_change_dump.h"
#include <time.h>


/*---------------------------------------------------------------------*
 *  private: definitions
 *---------------------------------------------------------------------*/

#define SIM_CYCLES 20
#define CLK_PERIOD_NS 10 // 10ns clock period


/*---------------------------------------------------------------------*
 *  private: typedefs
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  private: variables
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  public:  variables
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  private: function prototypes
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  private: functions
 *---------------------------------------------------------------------*/

#define ID_CLK    'a'
#define ID_BIT    'b'
#define ID_VECTOR 'v'
#define ID_REAL   'r'
#define ID_STRING 's'

/*---------------------------------------------------------------------*
 *  public:  functions
 *---------------------------------------------------------------------*/

bool value_change_dump_test(const char * filename)
{
	FILE * f = value_change_dump.Open(filename);

	value_change_dump.Header.Print01_Date(f, __DATE__);
	value_change_dump.Header.Print02_Version(f, "Simple example output");
	value_change_dump.Header.Print03_Timescale(f, "1ns");
	value_change_dump.Header.Print04_ScopeStart(f, "sim");

	value_change_dump.Header.Print05_Vector(f, ID_CLK, 0, 0, "clk");
	value_change_dump.Header.Print05_Bit(f, ID_BIT, "std_bit");
	value_change_dump.Header.Print05_Vector(f, ID_VECTOR, 7, 0, "vector"); // @suppress("Avoid magic numbers")
	value_change_dump.Header.Print05_Real(f, ID_REAL, "real");
	value_change_dump.Header.Print05_String(f, ID_STRING, "string");

	value_change_dump.Header.Print06_ScopeEnd(f);
	value_change_dump.Header.Print07_Enddef(f);
	value_change_dump.Header.Print08_Dumpvars(f);

	value_change_dump.PrintVector(f, ID_CLK, 1, 1);
	value_change_dump.PrintBit(f, ID_BIT, 5);
	value_change_dump.PrintVector(f, ID_VECTOR, 8, 0);
	value_change_dump.PrintReal(f, ID_REAL, 0.0);
	value_change_dump.PrintString(f, ID_STRING, "UNINIT");


	int clk = 1;
	double real = 0.0;

	for (uint64_t cycle = 0; cycle < SIM_CYCLES; ++cycle, real += 0.1, clk = 1 - clk) {

		value_change_dump.PrintTimestamp(f, cycle * CLK_PERIOD_NS);

		value_change_dump.PrintBit(f, ID_CLK, clk);
		value_change_dump.PrintStdLogic(f, ID_BIT, (char)cycle);
		value_change_dump.PrintVector(f, ID_VECTOR, 8, cycle);
		value_change_dump.PrintReal(f, ID_REAL, real);

		if(cycle == 8)
		{
			value_change_dump.PrintString(f, ID_STRING, "INIT");
		}
		if(cycle == 16)
		{
			value_change_dump.PrintString(f, ID_STRING, "START");
		}
	}

	value_change_dump.Close(f);

    return false;
}


/*---------------------------------------------------------------------*
 *  eof
 *---------------------------------------------------------------------*/
