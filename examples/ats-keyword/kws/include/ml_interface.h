/*
 * Copyright (c) 2021 Arm Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ML_INTERFACE_H
#define ML_INTERFACE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ml_processing_state_t {
    ML_SILENCE,
    ML_UNKNOWN,
    ML_HEARD_YES,
    ML_HEARD_NO,
    ML_HEARD_UP,
    ML_HEARD_DOWN,
    ML_HEARD_LEFT,
    ML_HEARD_RIGHT,
    ML_HEARD_ON,
    ML_HEARD_OFF,
    ML_HEARD_GO,
    ML_HEARD_STOP
} ml_processing_state_t;

/* return pointer to string version of ml_processing_state_t */
const char *get_inference_result_string(ml_processing_state_t ref_state);

void ml_task_inference_start();
void ml_task_inference_stop();

/* Initialises the interface to audio processing.
 */
int ml_interface_init(void);

/* Gets the current state of the ML model.
 */
ml_processing_state_t get_ml_processing_state(void);

/* Type of the handler called when the processing state changes.
 */
typedef void (*ml_processing_change_handler_t)(void *self, ml_processing_state_t new_state);

/* Register an handler called when the processing state change.
 */
void on_ml_processing_change(ml_processing_change_handler_t handler, void *self);

void ml_process_audio(const int16_t *buffer, size_t size);

/* this task does actual ml processing and is gated by the net task which let's it run if no ota job is present */
void ml_task(void *);

/* task used to communicate ml results via mqtt */
void ml_mqtt_task(void *);

#ifdef __cplusplus
}
#endif

#endif /* ! ML_INTERFACE_H */
