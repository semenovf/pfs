#pragma once
#define MAX_NTIMERS 120

enum timer_type_enum {
      Timer_Invalid
    , Timer_OneShot
    , Timer_Periodic
};

typedef int timer_id_t;

#ifndef __cplusplus
typedef enum timer_type_enum timer_type_enum;
#endif

#ifdef __cplusplus
extern "C" {
#endif

int timer_init   (void);

/**
 * @brief Set timer.
 * @param seconds Specified timeout for timer.
 * @return Timer identifier.
 */
timer_id_t timer_set    (timer_type_enum type, const double seconds);
int timer_passed (timer_id_t const id);
int timer_unset  (timer_id_t const id);
void timer_unset_all  (void);
int timer_done   (void);
int timer_count  (void);


/**
 * @brief Returns tier type.
 * @param id Timer identifier.
 * @return Timer type (@see timer_type_enum).
 */
timer_type_enum timer_type (timer_id_t const id);

/**
 * @brief Resets the timer: periodic will restarted, one-shot will unset.
 * @param id Timer identifier.
 */
void timer_reset (timer_id_t const id);

/**
 * @brief Checks if timer is periodic.
 * @param id Timer identifier.
 * @return @c true if timer is periodic, @c false otherwise.
 */
int timer_is_periodic (timer_id_t const id);

/**
 * @brief Checks if timer is one-shot.
 * @param id Timer identifier.
 * @return @c true if timer is one-shot, @c false otherwise.
 */
int timer_is_oneshot (timer_id_t const id);

#ifdef __cplusplus
}
#endif
