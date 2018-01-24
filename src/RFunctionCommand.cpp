#include "RFunctionCommand.h"

RPTR::FunctionCommand::FunctionCommand(void (*fun)(void *), void *param)
    : m_fun(fun), m_param(param) {}

void RPTR::FunctionCommand::execute() { m_fun(m_param); }
