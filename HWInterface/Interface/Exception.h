#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__
#include <exception>
#include <string>

namespace Ph2_HwInterface
{
    class Exception : public std::exception
    {
        private:
            std::string fStrError;

        public:
            Exception( const char *pStrError ){ fStrError = pStrError; }
            ~Exception()throw(){}
            const char *what() const throw();
    };
}
#endif
