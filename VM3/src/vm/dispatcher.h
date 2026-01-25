#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "vm.h"
#include "opcodes.h"
#include "handler.h"

void vm_run(vm_state_t *vm);

#endif