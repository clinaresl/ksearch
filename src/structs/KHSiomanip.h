// -*- coding: utf-8 -*-
// KHSiomanip.h
// -----------------------------------------------------------------------------
//
// Started on <dom 25-05-2025 00:02:23.578809541 (1748124143)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Definition of iomanip-like functions for customizing how solutions are
// printed on streams
//

#ifndef _KHSIOMANIP_H_
#define _KHSIOMANIP_H_

#include <iostream>
#include <iomanip>
#include <string>

namespace khs {

    // index allocates an index for encoding parameters within ::ios
    static int index() {

        // just alllocate a flag in ios_base to signal a mode
        static int idx = std::ios_base::xalloc();
        return idx;
    }

    // Formatters are in charge of internally storing any parameters passed to
    // an iomanip, so that iomanips just return formatters with specific
    // parameters which are then encoded into ::ios
    struct formatter {
        std::string mode;
        formatter(const std::string& m) :
            mode(m)
        {}

        // The following overloading function takes a formatter (produced by any
        // iomanip) and sets accordingly the flags in ::ios so that specific
        // operator<< can handle them appropriately
        friend std::ostream& operator<<(std::ostream& os, const formatter& fmt) {

            // Check whether the formatter specifies csv or console
            if (fmt.mode == "csv") {
                os.iword(index()) = 0;
            } else if (fmt.mode == "console") {
                os.iword(index()) = 1;
            } else if (fmt.mode == "color") {
                os.iword(index ()) = 2;
            } else {
                os.setstate(std::ios_base::failbit);
            }
            return os;
        }
    }; // formatter

    // set_mode is an iomanip that shows solutions either as a csv (i.e., just
    // using a semicolon to separate fields) or with fixed with separators to be
    // printed typically on the console
    static formatter set_mode (const std::string& mode) {
        return formatter(mode);
    }

} // namespace khs

#endif // _KHSIOMANIP_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
