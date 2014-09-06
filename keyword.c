#include <stdio.h>
#include <pocketsphinx.h>

// TODO: create a header file
void ps_reset(ps_decoder_t *decoder);

int main(int argc, char *argv[]) {

    ps_decoder_t *decoder;

    int rv;

    FILE *file;
    const char *KEYWORD = "forward";

    int buffsize = 512;
    int16 buf[buffsize]; // audio buffer

    int score;
    const char *uttid;

    // create configuration
    cmd_ln_t *config = cmd_ln_init(NULL, ps_args(), TRUE,
        "-hmm",     MODELDIR "/hmm/en_US/hub4wsj_sc_8k",
        "-lm",      MODELDIR "/lm/en/turtle.DMP",
        "-dict",    MODELDIR "/lm/en/turtle.dic",
        "-logfn",   "/dev/null", // no logs in stdout
        NULL);

    // instantiate decoder
    decoder = ps_init(config);

    // set KWS values
    ps_set_keyphrase(decoder, "keywords", KEYWORD);
    ps_set_search(decoder, "keywords");

    // open file
    file = fopen("goforward.raw", "rb");

    // start listening
    ps_start_utt(decoder, "spotting");

    // decode audio buffer
    while (!feof(file)) {
        size_t nsamp = fread(buf, 2, buffsize, file);
        ps_process_raw(decoder, buf, nsamp, FALSE, FALSE);
    }

    const char *hyp = ps_get_hyp(decoder, &score, &uttid);

    if (hyp != NULL)
        printf("%s \n", hyp);

    ps_reset(decoder);

    fclose(file);
    ps_free(decoder);

    return 0;
}

void ps_reset(ps_decoder_t *decoder) {
    ps_end_utt(decoder);
    ps_start_utt(decoder, NULL);
}
