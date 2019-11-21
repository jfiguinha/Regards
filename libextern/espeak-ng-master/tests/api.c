/*
 * Copyright (C) 2017 Reece H. Dunn
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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <espeak-ng/espeak_ng.h>
#include <espeak-ng/speak_lib.h>
#include <espeak-ng/encoding.h>

#include "speech.h"
#include "phoneme.h"
#include "voice.h"
#include "synthesize.h"
#include "translate.h"

// region espeak_Initialize

static void
test_espeak_terminate_without_initialize()
{
	printf("testing espeak_Terminate without espeak_Initialize\n");

	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Terminate() == EE_OK);
	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);
}

static void
test_espeak_initialize()
{
	printf("testing espeak_Initialize\n");

	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0) == 22050);
	assert(event_list != NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Terminate() == EE_OK);
	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);
}

// endregion
// region espeak_Synth

static void
test_espeak_synth()
{
	printf("testing espeak_Synth\n");

	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, NULL, 0) == 22050);
	assert(event_list != NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	const char *test = "One two three.";
	assert(espeak_Synth(test, strlen(test)+1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, NULL) == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Synchronize() == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Terminate() == EE_OK);
	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);
}

static void
test_espeak_synth_no_voices(const char *path)
{
	printf("testing espeak_Synth in path with no voices\n");

	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, path, espeakINITIALIZE_DONT_EXIT) == 22050);
	assert(event_list != NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	const char *test = "One two three.";
	assert(espeak_Synth(test, strlen(test)+1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, NULL) == EE_NOT_FOUND);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Synchronize() == EE_OK);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Terminate() == EE_OK);
	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);
}

// endregion
// region espeak_ng_Synthesize

static void
test_espeak_ng_synthesize()
{
	printf("testing espeak_ng_Synthesize\n");

	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, NULL, 0) == 22050);
	assert(event_list != NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	const char *test = "One two three.";
	assert(espeak_ng_Synthesize(test, strlen(test)+1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, NULL) == ENS_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Synchronize() == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Terminate() == EE_OK);
	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);
}

static void
test_espeak_ng_synthesize_no_voices(const char *path)
{
	printf("testing espeak_ng_Synthesize in path with no voices\n");

	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, path, espeakINITIALIZE_DONT_EXIT) == 22050);
	assert(event_list != NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	const char *test = "One two three.";
	assert(espeak_ng_Synthesize(test, strlen(test)+1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, NULL) == ENS_VOICE_NOT_FOUND);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Synchronize() == EE_OK);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Terminate() == EE_OK);
	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);
}

// endregion
// region espeak_SetVoiceByName

static void
test_espeak_set_voice_by_name_null_voice()
{
	printf("testing espeak_SetVoiceByName(NULL)\n");

	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, NULL, 0) == 22050);
	assert(event_list != NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_SetVoiceByName("") == EE_NOT_FOUND);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	const char *test = "One two three.";
	assert(espeak_Synth(test, strlen(test)+1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, NULL) == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Synchronize() == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Terminate() == EE_OK);
	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);
}

static void
test_espeak_set_voice_by_name_blank_voice()
{
	printf("testing espeak_SetVoiceByName(\"\")\n");

	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, NULL, 0) == 22050);
	assert(event_list != NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_SetVoiceByName("") == EE_NOT_FOUND);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	const char *test = "One two three.";
	assert(espeak_Synth(test, strlen(test)+1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, NULL) == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Synchronize() == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Terminate() == EE_OK);
	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);
}

static void
test_espeak_set_voice_by_name_valid_voice()
{
	printf("testing espeak_SetVoiceByName(\"de\")\n");

	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, NULL, 0) == 22050);
	assert(event_list != NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_SetVoiceByName("de") == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "de") == 0);
	assert(p_decoder == NULL);

	const char *test = "One two three.";
	assert(espeak_Synth(test, strlen(test)+1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, NULL) == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "de") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Synchronize() == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "de") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Terminate() == EE_OK);
	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);
}

static void
test_espeak_set_voice_by_name_invalid_voice()
{
	printf("testing espeak_SetVoiceByName(\"zzz\")\n");

	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, NULL, 0) == 22050);
	assert(event_list != NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_SetVoiceByName("zzz") == EE_NOT_FOUND);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	const char *test = "One two three.";
	assert(espeak_Synth(test, strlen(test)+1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, NULL) == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Synchronize() == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Terminate() == EE_OK);
	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);
}

static void
test_espeak_set_voice_by_name_language_variant_intonation_parameter()
{
	printf("testing espeak_SetVoiceByName(\"!v/Annie\") (language variant; intonation)\n");

	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, NULL, 0) == 22050);
	assert(event_list != NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_SetVoiceByName("!v/Annie") == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder == NULL);

	const char *test = "One two three.";
	assert(espeak_Synth(test, strlen(test)+1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, NULL) == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Synchronize() == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Terminate() == EE_OK);
	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);
}

// endregion
// region espeak_SetVoiceByProperties

static void
test_espeak_set_voice_by_properties_empty()
{
	printf("testing espeak_SetVoiceByProperties: (none)\n");

	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, NULL, 0) == 22050);
	assert(event_list != NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	espeak_VOICE properties;
	memset(&properties, 0, sizeof(properties));

	assert(espeak_SetVoiceByProperties(&properties) == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder == NULL);

	const char *test = "One two three.";
	assert(espeak_Synth(test, strlen(test)+1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, NULL) == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Synchronize() == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Terminate() == EE_OK);
	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);
}

static void
test_espeak_set_voice_by_properties_blank_language()
{
	printf("testing espeak_SetVoiceByProperties: languages=\"\"\n");

	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, NULL, 0) == 22050);
	assert(event_list != NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	espeak_VOICE properties;
	memset(&properties, 0, sizeof(properties));
	properties.languages = "";

	assert(espeak_SetVoiceByProperties(&properties) == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder == NULL);

	const char *test = "One two three.";
	assert(espeak_Synth(test, strlen(test)+1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, NULL) == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Synchronize() == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Terminate() == EE_OK);
	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);
}

static void
test_espeak_set_voice_by_properties_with_valid_language()
{
	printf("testing espeak_SetVoiceByProperties: languages=\"mk\" (valid)\n");

	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, NULL, 0) == 22050);
	assert(event_list != NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	espeak_VOICE properties;
	memset(&properties, 0, sizeof(properties));
	properties.languages = "mk";

	assert(espeak_SetVoiceByProperties(&properties) == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "mk") == 0);
	assert(p_decoder == NULL);

	const char *test = "One two three.";
	assert(espeak_Synth(test, strlen(test)+1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, NULL) == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "mk") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Synchronize() == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "mk") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Terminate() == EE_OK);
	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);
}

static void
test_espeak_set_voice_by_properties_with_invalid_language()
{
	printf("testing espeak_SetVoiceByProperties: languages=\"zzz\" (invalid)\n");

	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	assert(espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, NULL, 0) == 22050);
	assert(event_list != NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	espeak_VOICE properties;
	memset(&properties, 0, sizeof(properties));
	properties.languages = "zzz";

	assert(espeak_SetVoiceByProperties(&properties) == EE_NOT_FOUND);
	assert(translator == NULL);
	assert(p_decoder == NULL);

	const char *test = "One two three.";
	assert(espeak_Synth(test, strlen(test)+1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, NULL) == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Synchronize() == EE_OK);
	assert(translator != NULL);
	assert(strcmp(translator->dictionary_name, "en") == 0);
	assert(p_decoder != NULL);

	assert(espeak_Terminate() == EE_OK);
	assert(event_list == NULL);
	assert(translator == NULL);
	assert(p_decoder == NULL);
}

// endregion

int
main(int argc, char **argv)
{
	(void)argc; // unused parameter

	char *progdir = strdup(argv[0]);
	char *dir = strrchr(progdir, '/');
	if (dir != NULL) *dir = 0;

	test_espeak_terminate_without_initialize();
	test_espeak_initialize();

	test_espeak_synth();
	test_espeak_synth(); // Check that this does not crash when run a second time.
	test_espeak_synth_no_voices(progdir);
	test_espeak_synth();

	test_espeak_ng_synthesize();
	test_espeak_ng_synthesize(); // Check that this does not crash when run a second time.
	test_espeak_ng_synthesize_no_voices(progdir);
	test_espeak_ng_synthesize();

	test_espeak_set_voice_by_name_null_voice();
	test_espeak_set_voice_by_name_blank_voice();
	test_espeak_set_voice_by_name_valid_voice();
	test_espeak_set_voice_by_name_invalid_voice();
	test_espeak_set_voice_by_name_language_variant_intonation_parameter();

	test_espeak_set_voice_by_properties_empty();
	test_espeak_set_voice_by_properties_blank_language();
	test_espeak_set_voice_by_properties_with_valid_language();
	test_espeak_set_voice_by_properties_with_invalid_language();

	free(progdir);

	return EXIT_SUCCESS;
}
