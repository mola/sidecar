#ifndef SIDECAR_GUI_SPECTRUM_CURSORWIDGET_H // -*- C++ -*-
#define SIDECAR_GUI_SPECTRUM_CURSORWIDGET_H

#include "GUI/CursorWidget.h"

namespace SideCar {
namespace GUI {
namespace Spectrum {

/** QToolBar widget that shows the mouse cursor postion in real-world values such as range and sample value.
    Updates to the widget come from the showCursorPosition() slot.

    This class performs a lot of data caching in order to minimize the time
    spent in showCursorPosition(), and to speed up widget updates. Within
    showCursorPosition(), the class takes in the raw data, and sets various
    boolean flags if the raw data is different than what was seen before. An
    internal timer periodically checks the boolean flags to see if the widget
    needs an update. If so, the makeLabel() routine will rebuild a QString
    value for the widget's label text. The CursorWidget class keesp around the
    individual QString objects associated with raw values so that they only get
    regenerated by mouse position changes.
*/
class CursorWidget : public GUI::CursorWidget {
    Q_OBJECT
    using Super = GUI::CursorWidget;

public:
    /** Constructor. Creates and initializes window widgets.
     */
    CursorWidget(QWidget* parent = 0);

public slots:

    void showCursorPosition(const QString& value);
};

} // end namespace Spectrum
} // end namespace GUI
} // end namespace SideCar

/** \file
 */

#endif
