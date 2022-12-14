#include "QtWidgets/QBoxLayout"
#include "QtWidgets/QComboBox"

#include "ViewChooser.h"
#include "ViewEditor.h"

using namespace SideCar::GUI::ESScope;

ViewChooser::ViewChooser(ViewEditor* viewEditor, QWidget* parent) : Super(parent)
{
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    layout->setMargin(0);
    layout->setSpacing(6);

    chooser_ = new QComboBox(this);
    chooser_->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    chooser_->addItems(viewEditor->getPresetNames());
    chooser_->setCurrentIndex(viewEditor->getActivePresetIndex());
    chooser_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    chooser_->setFocusPolicy(Qt::NoFocus);

    layout->addWidget(chooser_, 1);
    layout->addSpacing(6);
    layout->addStretch(1);

    connect(chooser_, SIGNAL(activated(int)), viewEditor, SLOT(applyPreset(int)));

    connect(viewEditor, SIGNAL(activePreset(int)), chooser_, SLOT(setCurrentIndex(int)));

    connect(viewEditor, SIGNAL(presetNamesChanged(const QStringList&)), SLOT(presetNamesChanged(const QStringList&)));
}

void
ViewChooser::presetNamesChanged(const QStringList& names)
{
    int index = chooser_->currentIndex();
    chooser_->blockSignals(true);
    chooser_->clear();
    chooser_->addItems(names);
    chooser_->setCurrentIndex(index);
    chooser_->blockSignals(false);
}
