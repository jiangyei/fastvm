﻿
#ifndef __SPACE_H__
#define __SPACE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mcore/mcore.h"
#include "types.h"

/* copy from Ghidra space.hh */

typedef enum spacetype {
    IPTR_CONSTANT = 0,
    IPTR_PROCESSOR = 1,
    IPTR_SPACEBASE = 2,
    IPTR_INTERNAL = 3,
    IPTR_FSPEC = 4,
    IPTR_IOP = 5,
    IPTR_JOIN = 6,
} spacetype;

typedef struct AddrSpaceManager AddrSpaceManager;
typedef struct AddrSpaceManager Translate;


typedef struct AddrSpace {
    struct {
        unsigned big_endian : 1;
        unsigned heritaged : 1;
        unsigned does_deadcode : 1;
        unsigned programspecific : 1;
        unsigned reverse_justification : 1;
        unsigned overlay : 1;
        unsigned truncated : 1;
        unsigned hashpysical : 1;
        unsigned is_otherspace : 1;
        unsigned has_nearpointers : 1;
    } flags;
    /* privated */
    spacetype type;
    AddrSpaceManager *manage;
    Translate *trans;
    int refcount;
    uintb highest;
    unsigned pointerLowerBound;
    int shortcut;

    /* protected */
    char *name;
    int addrsize;
    int wordsize;
    int minimumPointerSize;
    int index;
    int delay;   // delay in heritage for the space
    int deadcodedelay;      // delay before deadcode removal is allowed on this space
} AddrSpace;

#define AddrSpace_getTrans(a)               (a)->trans
#define AddrSpace_getType(a)                (a)->type
#define AddrSpace_getDelay(a)               (a)->delay
#define AddrSpace_getDeadCodeDelay(a)       (a)->deadcodedelay
#define AddrSpace_getIndex(a)               (a)->index
#define AddrSpace_getWordSize(a)            (a)->wordsize
#define AddrSpace_getAddrSize(a)            (a)->addrsize
#define AddrSpace_getHighest(a)             (a)->highest
#define AddrSpace_getPointerLowerBound(a)   (a)->pointerLowerBound
#define AddrSpace_getMinimumPtrSize(a)      (a)->minimumPointerSize
#define AddrSpace_getShortcut(a)            (a)->shortcut
#define AddrSpace_isHeritaged(a)            (a)->flags.heritaged
#define AddrSpace_doesDeadCode(a)           (a)->flags.does_deadcode
#define AddrSpace_hasPhysical(a)            (a)->flags.hashpysical
#define AddrSpace_isBigEndian(a)            (a)->flags.isBigEndian
#define AddrSpace_isReverseJustified(a)     (a)->flags.reverse_justification
#define AddrSpace_isOverlay(a)              (a)->flags.overlay

inline uintb  AddrSpace_wrapOffset(AddrSpace *s, uintb off)
{
    if (off <= s->highest)
        return off;

    intb mod = (intb)(s->highest + 1);
    intb res = (intb)(off % mod);
    if (res < 0)
        res += mod;
    return (uintb)res;
}


typedef struct AddrSpaceManager {
    struct dynarray baselist;
    AddrSpace *constantspace;
    AddrSpace *defaultcodespace;
    AddrSpace *defaultdataspace;
    AddrSpace *iospace;
    AddrSpace *fspecspace;
    AddrSpace *joinspace;
    AddrSpace *stackspace;
} AddrSpaceManager, Translate;

AddrSpace*          AddrSpace_new(AddrSpaceManager *m);

AddrSpaceManager*   AddrSpaceManager_new();
void                AddrSpaceManager_delete(AddrSpaceManager *mgr);

int                 AddrSpaceManager_getDefaultSize(AddrSpaceManager *mgr);
AddrSpace*          AddrSpaceManager_getSpaceByName(AddrSpaceManager *mgr, const char *name);
AddrSpace*          AddrSpaceManager_getSpaceByShortcunt(AddrSpaceManager *m, char sc);

typedef struct VarnodeData {
    AddrSpace *space;
    uint8_t offset;
    uint32_t size;
} VarnodeData;

static inline bool VarnodeData_less(const VarnodeData *op1, const VarnodeData *op2) {
    return false;
}

#ifdef __cplusplus
}
#endif

#endif