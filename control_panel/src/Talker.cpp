#include "control/Talker.h"

#include "ui_ControlPanel.h"
#include "control/ControlPanel.h"

#define DEBUG_TALKER

namespace control
{
Talker::Talker(ControlPanel* controlPanel) :
        controlPanel(controlPanel)
{
    this->voice = new srg::agent::Voice(true);
    QObject::connect(this->controlPanel->uiControlPanel->sendBtn, SIGNAL(clicked(bool)), this, SLOT(run()), Qt::DirectConnection);
}

Talker::~Talker()
{
    // STFU ;-)
    delete voice;
}

void Talker::run()
{
    this->voice->speak(this->controlPanel->uiControlPanel->lineEdit->text().toStdString());
}
} // namespace control
