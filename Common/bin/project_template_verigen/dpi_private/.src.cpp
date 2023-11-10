#include "dpi_common.h"

DPI_FUNCTION void Test_ALU(const svBitVecVal* A, svBitVecVal* O)
{
	const int& iA		= *(const int*)A;
	int& iO				= *(int*)O;
	iO					= iA + 3;
}
