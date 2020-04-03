#include <stdio.h>
#include <string.h>

#include "uio_helper.h"

static void DumpUioFields(struct uio_info_t* pDev, const int verbose) {
    printf("uio%i: name=%s, version=%s, events=%li\n", pDev->uio_num, pDev->name, pDev->version,
           pDev->event_count);
    if (pDev->map_count == 0) {
        printf("WARNING: no memory maps found\n");
    }
    for (int i = 0; i < pDev->map_count; i++) {
        printf("\tmap[%i]: addr=0x%08lX, size=%i (0x%04X)\n", i, pDev->maps[i].addr,
               pDev->maps[i].size, pDev->maps[i].size);
    }
    if (verbose) {
        if (!pDev->dev_attrs) {
            printf("WARNING: no device attributes found\n");
        } else {
            printf("\tDevice attributes:\n");
            struct uio_dev_attr_t* pAttrs = pDev->dev_attrs;
            while (pAttrs) {
                printf("\t%s=%s\n", pAttrs->name, pAttrs->value);
                pAttrs = pAttrs->next;
            }
        }
    }
}

int main(int argc, char** argv) {
    struct uio_info_t* UioInfoList = uio_find_devices(-1);
    if (!UioInfoList) {
        printf("Did not find any UIO devices.\n");
        return 0;
    }
    int Verbose = 0;
    if ((argc > 1) && (strcmp(argv[1], "-v") == 0)) {
        Verbose = 1;
    }
    struct uio_info_t* dev = UioInfoList;
    while (dev) {
        uio_get_all_info(dev);
        DumpUioFields(dev, Verbose);
        dev = dev->next;
    }
    uio_free_info(UioInfoList);
    return 0;
}
