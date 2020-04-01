#include <stdio.h>
#include <string.h>

#include "uio_helper.h"

static void DumpUioFields(struct uio_info_t* pDev) {
    printf("------------------------------------------\n");
    printf("uio[%i]  name:'%s'  ver:'%s'\n", pDev->uio_num, pDev->name, pDev->version);
    if (pDev->map_count == 0) {
        printf("WARNING: no memory maps found\n");
    }
    for (int i = 0; i < pDev->map_count; i++) {
        printf("  map[%i]: address:0x%08lX  size:0x%04X\n", i, pDev->maps[i].addr,
               pDev->maps[i].size);
    }
    printf("  EventCount: %li\n", pDev->event_count);
    if (!pDev->dev_attrs) {
        printf("WARNING: no device attributes found\n");
    } else {
        printf("  Attributes:\n");
        struct uio_dev_attr_t* pAttrs = pDev->dev_attrs;
        while (pAttrs) {
            printf("    %s : %s\n", pAttrs->name, pAttrs->value);
            pAttrs = pAttrs->next;
        }
    }
}

int main(void) {
    struct uio_info_t* UioInfoList = uio_find_devices(-1);
    if (!UioInfoList) {
        printf("Did not find any UIO devices.\n");
        return 0;
    }
    struct uio_info_t* dev = UioInfoList;
    while (dev) {
        uio_get_all_info(dev);
        DumpUioFields(dev);
        dev = dev->next;
    }
    uio_free_info(UioInfoList);
    return 0;
}
