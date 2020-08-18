﻿
#ifndef __SLIGHSYMBOL_H__
#define __SLIGHSYMBOL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "semantics.h"
#include "slghpatexpress.h"

#define CODE_ADDRESS            0x01
#define OFFSET_IRREL            0x02
#define VARIABLE_LEN            0x04
#define MARKED                  0x08

typedef struct SleighSymbol SleighSymbol, SpaceSymbol, TokenSymbol, SectionSymbol, UserOpSymbol, TripleSymbol, FamilySymbol,
PatternlessSymbol, EpsilnSymbol, ValueSymbol, ValueMapSymbol, NameSymbol, VarnodeSymbol, BitRangeSymbol,
ContextSymbol, VarnodeListSymbol, OperandSymbol, StartSymbol, EndSymbol, MacroSymbol, SubtableSymbol, LabelSymbol,
BitrangeSymbol, SpecificSymbol;

typedef struct SymbolTable  SymbolTable;
typedef struct SymbolScope  SymbolScope;
typedef struct Constructor  Constructor;

struct SleighSymbol {
    enum {
        empty,
        space_symbol,
        token_symbol,
        userop_symbol,
        value_symbol,
        valuemap_symbol,
        name_symbol,
        varnode_symbol,
        varnodelist_symbol,
        operand_symbol,
        start_symbol,
        end_symbol,
        subtable_symbol,
        macro_symbol,
        section_symbol,
        bitrange_symbol,
        context_symbol,
        epsilon_symbol,
        label_symbol,
        dummy_symbol,
        flow_dest_symbol,
        flow_ref_symbol,
    } type;

    union {
        AddrSpace *space;
        Token *tok;

        struct {
            int templateid;     // Index into the constructTpl array
            int define_count;   // Number of definitions of this named section
            int ref_count;      // Number of references to this named section
        } section;

        int index;          // A user-defined symbol

        struct {
            PatternValue *patval;
        } value;

        struct {
            uint32_t    reloffset;
            int32_t     offsetbase;     
            int32_t     minimumlength;
            int32_t     hand;
            OperandValue *localexp;
            TripleSymbol *triple;
            PatternExpression *defexp;
            uint32_t    flags;
        } operand;

        struct {
            ConstantValue *patexp;
        } epsilon;

        struct {
            ConstantValue *patexp;
            VarnodeData fix;
            bool context_bits;
        } varnode;

        struct {
            AddrSpace *const_space;
            PatternExpression *patexp;
        } start, end;

        struct {
            AddrSpace *const_space;
        } flow_dest, flow_ref;
    };

    int id;
    int scopeid;
    char name[1];
};

void            SleighSymbol_delete(SleighSymbol *sym);
SleighSymbol*   SpaceSymbol_new(AddrSpace *spc);
SleighSymbol*   SectionSymbol_new(const char *name, int id);

PatternValue*       SleighSymbol_getPatternValue(SleighSymbol *s);
PatternExpression*  SleighSymbol_getPatternExpression(SleighSymbol *s);

VarnodeTpl*     SleighSymbol_getVarnode(SleighSymbol *sym);
VarnodeSymbol*  SleighSymbol_getParentSymbol(SleighSymbol *sym);
uint32_t        SleighSymbol_getBitOffset(SleighSymbol *sym);
uint32_t        SleighSymbol_numBits(SleighSymbol *sym);
#define SleighSymbol_getIndex(sym)          sym->index

static AddrSpace*   SleighSymbol_getSpace(SleighSymbol *sym) {
    assert(sym->type == space_symbol);

    return sym->space;
}


#define SleighSymbol_getName(sym)       sym->name 

typedef int (*SymbolCompare)(const SleighSymbol *a, const SleighSymbol *b);

typedef struct SymbolTree {
    SymbolCompare cmp;
} SymbolTree;


struct SymbolScope {
    SymbolTable *tab;
};

struct SymbolTable {
    struct dynarray symbolist;
    struct dynarray table;
    SymbolScope *curscope;
    SymbolScope *skipScope;
};

SymbolTable*    SymbolTable_new();
void            SymbolTable_delete(SymbolTable *s);

SleighSymbol*   SymbolTable_findSymbolInternal(SymbolTable *s, SymbolScope *scope, const char *name);
#define SymbolTable_getCurrentScope(s)  s->curscope
#define SymbolTable_getGlobalScope(s)   s->table[0]
#define SymbolTable_setCurrentScope(s, sc)  s->curscope = scope

void            SymbolTable_addScope(SymbolTable *s);
void            SymbolTable_popScope(SymbolTable *s);
void            SymbolTable_addGlobalSymbol(SymbolTable *s, SleighSymbol *a);
void            SymoblTable_addSymbol(SymbolTable *s, SleighSymbol *a);
SleighSymbol*   SymbolTable_findSymbol(SymbolTable *s, const char *name);
SleighSymbol*   SymbolTable_findSymbolSkip(SymbolTable *s, const char *name, int skip);
SleighSymbol*   SymbolTable_findGlobalSymbol(SymbolTable *s, const char *name);
SleighSymbol*   SymbolTable_findSymbolById(SymbolTable *s, int id);
void            SymbolTable_replaceSymbol(SleighSymbol *a, SleighSymbol *b);

struct Constructor {
    TokenPattern *pattern;
    SubtableSymbol *parent;
    PatternEquation *pateq;
    struct dynarray operands;
    struct dynarray printpiece;
    struct dynarray context;
    ConstructTpl *templ;
    struct dynarray namedtempl;
    int minimumlength;
    uintm id;
    int firstwhitespace;
    int flowthruindex;
    int fileno;
    bool inerror;
};

Constructor*    Constructor_new();
void            Constructor_delete(Constructor *c);
void            Constructor_addSyntx(Constructor *c, const char *syn);
#define Constructor_getParent(ct)       (ct)->parent



#ifdef __cplusplus
}
#endif

#endif