#ifndef SIDECAR_GUI_CURSORWIDGET_H // -*- C++ -*-
#define SIDECAR_GUI_CURSORWIDGET_H

#include "QtWidgets/QWidget"

namespace Ui {
class CursorWidget;
}
namespace SideCar {
namespace GUI {

/** QToolBar widget that shows the mouse cursor postion in real-world values such as range and sample value.
    Updates to the widget come from the showCursorPosition() slot.

    This class performs a lot of data caching in order to minimize the time spent in showCursorPosition(), and
    to speed up widget updates. Within showCursorPosition(), the class takes in the raw data, and sets various
    boolean flags if the raw data is different than what was seen before. An internal timer periodically checks
    the boolean flags to see if the widget needs an update. If so, the makeLabel() routine will rebuild a
    QString value for the widget's label text. The CursorWidget class keesp around the individual QString
    objects associated with raw values so that they only get regenerated by mouse position changes.
*/
class CursorWidget : public QWidget {
    Q_OBJECT
    using Super = QWidget;

public:
    /** Constructor. Creates and initializes window widgets.
     */
    CursorWidget(QWidget* parent = 0);

    QString getValue() const;

protected:
    void setValue(const QString& value);

private:
    Ui::CursorWidget* gui_;
};

} // end namespace GUI
} // end namespace SideCar

/** \file
 */

#endif
