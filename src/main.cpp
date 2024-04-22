// Lib C includes
#include <stdlib.h>
#include <stdbool.h>

// Zephyr includes
#include <zephyr/kernel.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);

#include "tmc5160.h"

const struct device *tmc0 = DEVICE_DT_GET( DT_ALIAS(tmc0));

int main(void) {

  if (!device_is_ready(tmc0)) {
    printk("TMC device is not ready\r\n");
    return EXIT_FAILURE;
  }

  tmc_run(tmc0, 0, 60, 0);

  while (true) {
    k_msleep(100);
  }

  return EXIT_SUCCESS;
}

void k_sys_fatal_error_handler(unsigned int reason, const z_arch_esf_t *context) {
  switch (reason) {
    case K_ERR_CPU_EXCEPTION:
      LOG_ERR("Generic CPU exception, not covered by other codes");
      break;
    case K_ERR_SPURIOUS_IRQ:
      LOG_ERR("Unhandled hardware interrupt");
      break;
    case K_ERR_STACK_CHK_FAIL:
      LOG_ERR("Faulting context overflowed its stack buffer");
      break;
    case K_ERR_KERNEL_OOPS:
      LOG_ERR("High severity software error");
      break;
    case K_ERR_ARCH_START:
      LOG_ERR("Arch specific fatal errors");
      break;
    default:
      LOG_ERR("Unknow reason for fatal error (%d)", reason);
      break;
  }
  sys_reboot(SYS_REBOOT_COLD);
}
