#include "AdvancedErrorManagement.h"
#include "BasicConsoleLogAdapter.h"

namespace MARTe {

    BasicConsoleLogAdapter::BasicConsoleLogAdapter(ProfinetDataSourceDriver::log_adapter_level_t minimumLevel) {
        this->minimumLevel = minimumLevel;
    }

    void BasicConsoleLogAdapter::Log(ProfinetDataSourceDriver::log_adapter_level_t logLevel, std::string message) {
        if(logLevel >= minimumLevel) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, message.c_str());
        }
    }

}
