#include "RawSensorData.h"

namespace srg {
    namespace wm {
        RawSensorData::RawSensorData(srg::SRGWorlModel *wm) {
            this->wm = wm;
            auto sc = this->wm->getSystemConfig();
        }

        RawSensorData::~RawSensorData() {

        }

        void RawSensorData::processTelegramMessage(Message message) {

        }
    }
}