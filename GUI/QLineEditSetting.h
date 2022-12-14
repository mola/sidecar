#ifndef SIDECAR_GUI_QLINEEDITSETTING_H // -*- C++ -*-
#define SIDECAR_GUI_QLINEEDITSETTING_H

#include "GUI/StringSetting.h"

class QLineEdit;

namespace SideCar {
namespace GUI {

class QLineEditSetting : public StringSetting {
    Q_OBJECT
public:
    QLineEditSetting(PresetManager* mgr, QLineEdit* widget, bool global = false);

private slots:
    void widgetChanged();
};

} // end namespace GUI
} // end namespace SideCar

#endif
