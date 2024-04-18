#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/smf.h>

/* List of events */
#define EVENT_ANY          (uint32_t)(0xFFFFFFFF)
#define EVENT_BUTTON_PRESS (uint32_t)(BIT(0))

/* Used to facilitate indexing the states[] list */
typedef enum {
  STATE_0 = 0,
  STATE_1,
  STATE_MAX,
} state_t;

/* State machine global context (contains SMF contaxt + user data) */
typedef struct {
  /* This must be first */
  struct smf_ctx ctx;
  /* In our case, user data is the event kernel object and a 32 bits variable to store events */
  struct k_event kEvent;
  uint32_t events;
  /* Other state specific data add here */
} state_machine_t;

static void appThreadHandler();
static void state0Run(void *machine);
static void state1Run(void *machine);
static void setupButton(const struct gpio_dt_spec *buttonGpio, struct gpio_callback *callbackData);
static void onButtonPress(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/* Create thread */
K_THREAD_DEFINE(appThread, 1024, appThreadHandler, NULL, NULL, NULL, 7, 0, 0);

/* Get button device from DTS */
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw0), gpios, {0});
static struct gpio_callback buttonCallbackData = {0};

/* List of states */
static const struct smf_state states[] = {
  [STATE_0] = SMF_CREATE_STATE(NULL, state0Run, NULL),
  [STATE_1] = SMF_CREATE_STATE(NULL, state1Run, NULL),
};

static void appThreadHandler() {
  /* Global state machine */
  state_machine_t stateMachine = {0};

  /* Configure button GPIO and interrupt */
  setupButton(&button, &callbackData);

  /* Initialize the event kernel object inside the state machine */
  k_event_init(&stateMachine.kEvent);

  /* Set initial state for the state machine */
  smf_set_initial(SMF_CTX(&stateMachine), &states[STATE_0]);

  /* Run the state machine */
  while(1) {
    /* Block forever until any event is detected */
    stateMachine.events = k_event_wait(&stateMachine.kEvent, EVENT_ANY, true, K_FOREVER);
    /* Runs one iteration of a state machine (including any parent states) */
    ret = smf_run_state(SMF_CTX(&stateMachine));
    /* State machine terminates if a non-zero value is returned */
    if (ret) {
      /* handle return code and terminate state machine */
      break;
    }
  }
}

static void state0Run(void *machine) {
  state_machine_t *s = (state_machine_t *)machine;

  /* Fiter on events that we need in STATE 0 */

  if (stateMachine.events & EVENT_BUTTON_PRESS) {
    /* Change state on button press event */
    smf_set_state(SMF_CTX(&stateMachine), &states[STATE_1]);
  }
}

static void state1Run(void *machine) {
  state_machine_t *s = (state_machine_t *)machine;

  /* Fiter on events that we need in STATE 1 */

  if (stateMachine.events & EVENT_BUTTON_PRESS) {
    /* Change state on button press event */
    smf_set_state(SMF_CTX(&stateMachine), &states[STATE_0]);
  }
}

static void setupButton(const struct gpio_dt_spec *buttonGpio, struct gpio_callback *callbackData) {
  int ret;

  /* Setup button GPIO and interrupt */
  if (!gpio_is_ready_dt(buttonGpio)) {
    printk("Error: button device %s is not ready\r\n", button.port->name);
    return;
  }
  ret = gpio_pin_configure_dt(buttonGpio, GPIO_INPUT);
  if (ret != 0) {
    printk("Error %d: failed to configure %s pin %d\r\n", ret, button.port->name, button.pin);
    return;
  }
  ret = gpio_pin_interrupt_configure_dt(buttonGpio, GPIO_INT_EDGE_TO_ACTIVE);
  if (ret != 0) {
    printk("Error %d: failed to configure interrupt on %s pin %d\r\n", ret, button.port->name, button.pin);
    return;
  }
  gpio_init_callback(*callbackData, onButtonPress, BIT(button.pin));
  gpio_add_callback(button.port, *callbackData);
}

static void onButtonPress(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
  /* Generate button press event */
  k_event_post(&stateMachine.kEvent, EVENT_BUTTON_PRESS);
}
