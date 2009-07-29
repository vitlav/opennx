#include <stdio.h>
#include <opensc/opensc.h>

#define CTXNAME "openssh"

static int findreader() {
    unsigned int rc, i;
    int r, j;
    int found_id = -1;
    sc_context_t *ctx = NULL;

    if (SC_SUCCESS != sc_establish_context(&ctx, CTXNAME))
        return -1;
    rc = ctx->reader_count;
    if (rc > 0) {
        unsigned int errc = 0;

        for (i = 0; i < rc; i++) {
            sc_reader_t *reader = ctx->reader[i];
            if (!reader)
                continue;
            for (j = 0; j < reader->slot_count; j++) {
                r = sc_detect_card_presence(reader, j);
                if (r > 0) {
                    found_id = i;
                    break;
                }
                if (r < 0) {
                    errc++;
                }
            }
            if (found_id != -1)
                return found_id;
        }
        if (errc >= rc) {
            sc_release_context(ctx);
            return -1;
        }
    }
    sc_release_context(ctx);
    return -1;
}

int main(int argc, char **argv) {
    int rid = findreader();
    return (rid < 0) ? 255 : rid;
}
