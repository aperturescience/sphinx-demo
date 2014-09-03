#include <stdio.h>
#include <pocketsphinx.h>

int main(int argc, char *argv[]) {

    // create configuration
    cmd_ln_t *config = cmd_ln_init(NULL, ps_args(), TRUE,
       "-hmm",  MODELDIR "/hmm/en_US/hub4wsj_sc_8k",
       "-lm",   MODELDIR "/lm/en/turtle.DMP",
       "-dict", MODELDIR "/lm/en/turtle.dic",
       "-logfn", "/dev/null", // no logs in stdout
       NULL);

    // instantiate decoder
    ps_decoder_t *decoder = ps_init(config);
    cmd_ln_free_r(config);

    // open file and parse it
    FILE *file = fopen("goforward.raw", "rb");
    // decode raw file
    int samples = ps_decode_raw(decoder, file, NULL, -1);
    fclose(file);

    // create hypothesis
    char const *utterance;
    int32 score;

    char const *hypothesis = ps_get_hyp(decoder, &score, &utterance);

    printf("Recognized: %s\n", hypothesis);

    ps_free(decoder);

    return 0;
}
