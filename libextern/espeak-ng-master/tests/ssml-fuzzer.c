/*
 * Copyright (C) 2018 Sascha Brawer
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write see:
 *             <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <stdint.h>
#include <stdlib.h>

#include <espeak-ng/espeak_ng.h>

static int initialized = 0;

static int SynthCallback(short *wav, int numsamples, espeak_EVENT *events) {
	/* prevent warning for unused arguments */
	(void) wav;
	(void) numsamples;
	(void) events;

	return 0;
}


/* See http://llvm.org/docs/LibFuzzer.html */
extern int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);
extern int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
	if (!initialized) {
		espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS, 0, NULL, 0);
		espeak_SetSynthCallback(SynthCallback);
		initialized = 1;
	}

	int synth_flags = espeakCHARS_UTF8 | espeakPHONEMES | espeakSSML;
	espeak_Synth((char*) data, size + 1, 0, POS_CHARACTER, 0,
	             synth_flags, NULL, NULL);

	return 0;
}
