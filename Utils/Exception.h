#ifndef UTILS_EXCEPTION_H // -*- C++ -*-
#define UTILS_EXCEPTION_H

#include <iosfwd>
#include <memory> // for std::unique_ptr
#include <sstream>
#include <stdexcept>
#include <string>

namespace Utils {

/** Define a stream inserter operator to use for Exception objects that returns the appropriate type.
 */
template <typename T>
struct ExceptionInserter {
    template <typename U>
    friend T& operator<<(T& lhs, const U& rhs)
    {
        lhs.append(rhs);
        return lhs;
    }
};

/** Base exception class. Nothing too special about this over the stock C++ exception class, except that it does
    have the wonderful ability to accumulate error information via the append() method.
*/
class Exception : public std::exception, public ExceptionInserter<Exception> {
public:
    /** Default constructor.
     */
    Exception() : std::exception(), os_(nullptr), err_("") {}

    /** Constructor.

        \param err C character string to use as the start of the error. Throws exception if NULL pointer is
        passed in. text.
    */
    Exception(const char* err);

    /** Constructor.

        \param err C++ string to use as the start of the error text.
    */
    Exception(const std::string& err) : std::exception(), os_(nullptr), err_(err) {}

    /** Copy constructor. Seeds the output formatter with the value from the given instance.

        \param rhs instance to copy from
    */
    Exception(const Exception& rhs);

    /** Destructor. Dispose of any active ostringstream object.
     */
    virtual ~Exception();

    /** Assignment operator.

        \param rhs value to copy from

        \return reference to self
    */
    Exception& operator=(const Exception& rhs);

    /** Accessor for the error text of the exception.

        \return error text
    */
    const std::string& err() const;

    /** Override of std::exception method. Takes error text from formatted string stream.

        \return error text
    */
    virtual const char* what() const noexcept;

    /** Append a value to the exception text.

        \param value data to write to exception data capture stream
    */
    template <typename T>
    void append(const T& value)
    {
        os() << value;
    }

private:
    std::ostream& os();

    mutable std::unique_ptr<std::ostringstream> os_;
    mutable std::string err_;
}; // class Exception

} // namespace Utils

/** \file
 */

#endif
