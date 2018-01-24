#ifndef RFUNCTIONCOMMAND_H
#define RFUNCTIONCOMMAND_H

#include "RICommand.h"

namespace RPTR {
class FunctionCommand : public ICommand {
public:
  FunctionCommand(void (*fun)(void *), void *param);
  void execute();

private:
  void (*m_fun)(void *);
  void *m_param;
};
}

#endif // RFUNCTIONCOMMAND_H
