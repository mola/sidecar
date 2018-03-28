#ifndef UTILS_FORMAT_H // -*- C++ -*-
#define UTILS_FORMAT_H

#include <cstring>
#include <errno.h>

#include <ios>
#include <iostream>
#include <sstream>

namespace Utils {

class Format;

/** Helper class that binds a value to a Format object for use with standard C++ output streams. Instances of
    this class are generated by the Format::operator(), and its attributes are used by the ostream inserter
    template function for Formatter objects.

    The idea for this and most of the framework were shamelessly taken from "C++ Programming Language (3rd
    Edition)" by Bjarne Stroustrup (pp 635-636)
*/
template <class T>
struct Formatter {
    /** Constructor.

        \param format which Format object to apply to the stream

        \param value the value to insert into the stream after the format has been applied.
    */
    Formatter(const Format& format, const T& value) : format_(format), value_(value) {}

    const Format& format_; ///< Format specification to use
    const T& value_;       ///< Value to format
};

/** Formatting settings for a C++ input or output stream. Mimics the format control capabilities of
    std::ios_base + std::ios (for the fill character). Allows one to define and save a specific formatting, and
    then apply them to values within an output stream.

    @code

    int main()
    {
      double d = 1234.56789;
      Utils::Format gen4(4);
      Utils::Format sci8(gen4);
      sci8.scientific().precision(8);

      // Should print out the following: 1234.57 1235 1.23456789e+03 1234.57
      //
      std::cout << d << ' ' << gen4(d) << ' ' << sci8(d) << ' ' << d << '\n';
    }
    @endcode
*/
class Format {
public:
    using fmtflags = std::ios_base::fmtflags;

    /** Constructor. Use default values for everything.
     */
    Format() : flags_(), precision_(6), width_(0), fill_(' ') {}

    /** Constructor. Set number of digits shown after decimal point in floating-point formats.

        \param precision number of digits after the decimal point to display
    */
    explicit Format(int precision) : flags_(), precision_(precision), width_(0), fill_(' ') {}

    /** Constructor. Set field width for all formats, and number of digits shown after decimal point in
        floating-point formats.

        \param width number of characters to use for formatting.

        \param precision number of digits after the decimal point to display
    */
    Format(int width, int precision) : flags_(), precision_(precision), width_(width), fill_(' ') {}

    /** Constructor. Obtain format settings from an input or output stream.

        \param s stream to query for format settings
    */
    Format(const std::ios& s) : flags_(s.flags()), precision_(s.precision()), width_(s.width()), fill_(s.fill()) {}

    /** Set all formatting bits at once.

        \param flags value to use for the formatting bits

        \return self
    */
    Format& flags(std::ios_base::fmtflags flags)
    {
        flags_ = flags;
        return *this;
    }

    /** \return all formatting flag bits.
     */
    std::ios_base::fmtflags flags() const { return flags_; }

    /** Turn on one or more formatting flags

        \param flags bits to turn on

        \return self
    */
    Format& setf(fmtflags flags)
    {
        flags_ |= flags;
        return *this;
    }

    /** Turn on one or more formatting flags, after first masking out the existing flags with a mask.

        \param flags bits to turn on

        \param mask bits to turn off before applying the new flags

        \return self
    */
    Format& setf(fmtflags flags, fmtflags mask)
    {
        flags_ = (~mask & flags_) | flags;
        return *this;
    }

    /** Turn off one or more formatting flags.

        \param flags bits to turn off

        \return self
    */
    Format& unsetf(fmtflags flags)
    {
        flags_ &= ~flags;
        return *this;
    }

    /** Set the number of digits to display after the decimal point.

        \param precision number of digits

        \return self
    */
    Format& precision(int precision)
    {
        precision_ = precision;
        return *this;
    }

    /** \return current precision setting
     */
    int precision() const { return precision_; }

    /** Set the minimum number of characters used to format a value. If a value must use more characters, it
        will do so; this is only a minimum field width.

        \param width number of characters

        \return self
    */
    Format& width(int width)
    {
        width_ = width;
        return *this;
    }

    /** \return current width setting
     */
    int width() const { return width_; }

    /** Set the character to use when aligning a formatted value within a field.

        \param fill character to use

        \return self
    */
    Format& fill(char fill)
    {
        fill_ = fill;
        return *this;
    }

    /** \return current fill character.
     */
    char fill() const { return fill_; }

    /** Apply our format settings to a stream.

        \param io stream to act on
    */
    void put(std::ios& io) const;

    /** Enable translation of boolean values into strings.

        \return self
    */
    Format& boolalpha() { return setf(std::ios_base::boolalpha); }

    /** Disable translation of boolean values into strings.

        \return self
    */
    Format& noboolalpha() { return unsetf(std::ios_base::boolalpha); }

    /** Generate integer output in decimal (base 10) format.

        \return self
    */
    Format& dec() { return setf(std::ios_base::dec, std::ios_base::basefield); }

    /** Generate integer output in octal (base 8) format.

        \return self
    */
    Format& oct() { return setf(std::ios_base::oct, std::ios_base::basefield); }

    /** Generate integer output in hexadecimal (base 16) format.

        \return self
    */
    Format& hex() { return setf(std::ios_base::hex, std::ios_base::basefield); }

    /** Output a prefix indicating the numeric base for integer output.

        \return self
    */
    Format& showbase() { return setf(std::ios_base::showbase); }

    /** Do not output a prefix indicating the numeric base for integer output.

        \return self
    */
    Format& noshowbase() { return unsetf(std::ios_base::showbase); }

    /** Generate floating-point output in fixed-point notation.

        \return self
    */
    Format& fixed() { return setf(std::ios_base::fixed, std::ios_base::floatfield); }

    /** Generate floating-point output in scientific notation.

        \return self
    */
    Format& scientific() { return setf(std::ios_base::scientific, std::ios_base::floatfield); }

    /** Force output of a decimal-point character in floating-point output.

        \return self
    */
    Format& showpoint() { return setf(std::ios_base::showpoint); }

    /** Do not force output of a decimal-point character in floating-point output.

        \return self
    */
    Format& noshowpoint() { return unsetf(std::ios_base::showpoint); }

    /** Adds fill characters on the right of certain generated output.

        \return self
    */
    Format& left() { return setf(std::ios_base::left, std::ios_base::adjustfield); }

    /** Adds fill characters on the left of certain generated output.

        \return self
    */
    Format& right() { return setf(std::ios_base::right, std::ios_base::adjustfield); }

    /** Adds fill characters at a designated internal point in certain generated output. If no such point is
        designated, its identical to `right'.

        \return self
    */
    Format& internal() { return setf(std::ios_base::internal, std::ios_base::adjustfield); }

    /** Output a `+' sign in non-negative generated numeric output.

        \return self
    */
    Format& showpos() { return setf(std::ios_base::showpos); }

    /** Do not output a `+' sign in non-negative generated numeric output.

        \return self
    */
    Format& noshowpos() { return unsetf(std::ios_base::showpos); }

    /** Skip leading white space before certain input operations.

        \return self
    */
    Format& skipws() { return setf(std::ios_base::skipws); }

    /** Do not skip leading white space before certain input operations.

        \return self
    */
    Format& noskipws() { return unsetf(std::ios_base::skipws); }

    /** Replace certain lowercase letters with their uppercase equivalents in generated output.

        \return self
    */
    Format& uppercase() { return setf(std::ios_base::uppercase); }

    /** Do not replace certain lowercase letters with their uppercase equivalents in generated output.

        \return self
    */
    Format& nouppercase() { return unsetf(std::ios_base::uppercase); }

    /** Template method that returns a new Formatter instance to hold a reference to us and to a particular
        value to format.

        \param v value to format

        \return new Formatter instance
    */
    template <class T>
    Formatter<T> operator()(const T& v) const
    {
        return Formatter<T>(*this, v);
    }

private:
    std::ios_base::fmtflags flags_;
    int precision_;
    int width_;
    char fill_;
};

/** Utility class that records the format settings of a stream, and then applies the settings found in a Format
    object. When the PushFormat instance is destroyed, it restores the stream to its original settings.
*/
class PushFormat {
public:
    /** Constructor. Just remember the current formatting settings of the given stream.

        \param s I/O stream to work with
    */
    explicit PushFormat(std::ios& s) : s_(s), prev_(s) {}

    /** Constructor. Remembers the current formatting settings of an I/O stream, and applies new formatting
        settings contained in a Format object.

        \param s stream to work with

        \param format format settings to apply
    */
    PushFormat(std::ios& s, const Format& format) : s_(s), prev_(s) { format.put(s); }

    /** Destructor. Restore the stream to its original settings.
     */
    ~PushFormat() { prev_.put(s_); }

private:
    std::ios& s_; ///< Stream affected
    Format prev_; ///< Original stream settings
};

/** Declare an abstract base class that defines an interface for all `printable' objects, those that can be
    inserted into an output stream with the `<<' operator.
*/
class Printable {
public:
    virtual ~Printable() {}
    virtual std::ostream& print(std::ostream& os) const = 0;
};

/** Output stream formatter for void* pointers. Shows the pointer as a hexadecimal value.
 */
class hexPtr : public Printable {
public:
    hexPtr(const void* ptr) : ptr_(ptr) {}
    virtual std::ostream& print(std::ostream& os) const;

private:
    const void* ptr_;
};

/** Output stream formatter for heading values. Shows the heading as a value between 0 and 360, with leading
    zeros, in 3 places.
*/
class formatHeading : public Printable {
public:
    formatHeading(double degrees) : degrees_(degrees) {}
    virtual std::ostream& print(std::ostream& os) const;

private:
    double degrees_;
};

class formatTime : public Printable {
public:
    formatTime(double when) : when_(when) {}
    virtual std::ostream& print(std::ostream& os) const;

private:
    double when_;
};

/** Simple output stream manipulator that spits out text centered in a field of a given width.
 */
class center : public Printable {
public:
    /** Constructor for standard C++ std::strings

        \param width field size

        \param s C++ std::string to center
    */
    center(int width, const std::string& s) : width_(width), data_(s) {}

    center(int width, const char* s) : width_(width), data_(s) {}

    template <typename T>
    center(int width, const T& value) : width_(width), data_("")
    {
        std::ostringstream os("");
        os << value;
        data_ = os.str();
    }

    virtual ~center() {}

    /** Implementation of Printable interface.

        \param os stream to write to

        \return stream written to
    */
    std::ostream& print(std::ostream& os) const;

private:
    int width_;        ///< Format used to center text
    std::string data_; ///< Text to center
};

/** Output stream manipulator that spits out a sequence of one character (a `-' by default).
 */
class dashLine : public Printable {
public:
    /** Constructor to output a field of dash characters.

        \param width field size

        \param c character to use to fill the field.
    */
    dashLine(int width, char c = '-') : format_(width, 0) { format_.fill(c); }

    /** Implementation of Printable interface.

        \param os stream to write to

        \return stream written to
    */
    std::ostream& print(std::ostream& os) const;

private:
    Format format_; ///< Format used to fill a field
};

struct dumpHex : public Printable {
    dumpHex(const void* data, int size) : data_(data), size_(size) {}
    char digit(short value) const { return "0123456789ABCDEF"[value & 15]; }
    std::ostream& print(std::ostream& os) const;
    const void* data_;
    int size_;
};

class showErrno : public Printable {
public:
    showErrno() : errno_(errno), text_(strerror(errno)) {}
    std::ostream& print(std::ostream& os) const { return os << text_ << " (" << errno_ << ')'; }

private:
    int errno_;
    std::string text_;
};

} // namespace Utils

namespace std {

/** Insertor method for Format objects. Applies the format settings found in a Format object to a stream.

    \param s stream to work with

    \param format format settings to use

    \return stream acted on
*/
inline std::ostream&
operator<<(std::ostream& s, const Utils::Format& format)
{
    format.put(s);
    return s;
}

inline std::istream&
operator<<(std::istream& s, const Utils::Format& format)
{
    format.put(s);
    return s;
}

/** Template method used to insert Formatter objects into an output stream. Updates the output stream with the
    format settings found in a Format object, and then invokes the std::ios formatter for the value. Upon exit,
    the format settings of the stream are restored to their original values.

    \param os output stream to write to

    \param f Formatter instance containing the format to use and the value to format.

    \return stream written to
*/
template <class T>
std::ostream&
operator<<(std::ostream& os, const Utils::Formatter<T>& f)
{
    Utils::PushFormat prev(os, f.format_);
    return os << f.value_;
}

/** Output stream insertor for all Printable object types. Just invokes the Printable::print method that must be
    defined in Printable subclasses.

    \param os stream to write to

    \param p object to write

    \return stream written to
*/
inline std::ostream&
operator<<(std::ostream& os, const Utils::Printable& p)
{
    return p.print(os);
}

} // namespace std

/** \file
 */

#endif
