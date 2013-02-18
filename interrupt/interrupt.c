#include <interrupt.h>
#include <klib.h>

void handlerRedirection(void **parameters, KernelGlobal *global, UserTrapframe *user_sp) {
	global->ready_queue->curtask->current_sp = user_sp;

	DEBUG(DB_TASK, "| TASK:\tHANDLER SP: 0x%x SPSR: 0x%x ResumePoint: 0x%x\n",
	      user_sp, user_sp->spsr, user_sp->resume_point);

	// If is an IRQ
	if (parameters == NULL) {
		irqHandler(global);
	}
	else {
		user_sp->r0 = syscallHandler(parameters, global);
	}
}