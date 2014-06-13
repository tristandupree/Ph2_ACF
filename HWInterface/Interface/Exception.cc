#include "Exception.h"
#include "TROOT.h"

namespace Ph2_HwInterface
{
        const char *Exception::what() const throw()
        {
            return Form( "CBCInterface %s", fStrError.c_str() );
        }
}
