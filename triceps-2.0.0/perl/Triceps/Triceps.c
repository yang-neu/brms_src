/*
 * This file was generated automatically by ExtUtils::ParseXS version 3.18 from the
 * contents of Triceps.xs. Do not edit this file, edit Triceps.xs instead.
 *
 *    ANY CHANGES MADE HERE WILL BE LOST!
 *
 */

#line 1 "Triceps.xs"
//
// (C) Copyright 2011-2014 Sergey A. Babkin.
// This file is a part of Triceps.
// See the file COPYRIGHT for the copyright notice and license information
//
// The main file including all the parts of Triceps XS interface.

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include "TricepsPerl.h"
#include <app/Sigusr2.h>

#include "const-c.inc"

#ifdef __cplusplus
extern "C" {
#endif
XS(boot_Triceps__Label); 
XS(boot_Triceps__Row); 
XS(boot_Triceps__Rowop); 
XS(boot_Triceps__RowHandle); 
XS(boot_Triceps__RowType); 
XS(boot_Triceps__IndexType); 
XS(boot_Triceps__TableType); 
XS(boot_Triceps__Tray); 
XS(boot_Triceps__Unit); 
XS(boot_Triceps__UnitTracer); 
XS(boot_Triceps__Table); 
XS(boot_Triceps__AggregatorType); 
XS(boot_Triceps__AggregatorContext); 
XS(boot_Triceps__FrameMark); 
XS(boot_Triceps__FnReturn); 
XS(boot_Triceps__FnBinding); 
XS(boot_Triceps__AutoFnBind); 
XS(boot_Triceps__App); 
XS(boot_Triceps__Triead); 
XS(boot_Triceps__TrieadOwner); 
XS(boot_Triceps__Facet); 
XS(boot_Triceps__Nexus); 
XS(boot_Triceps__AutoDrain); 
XS(boot_Triceps__PerlValue); 
XS(boot_Triceps__TrackedFile); 
#ifdef __cplusplus
};
#endif

#line 61 "Triceps.c"
#ifndef PERL_UNUSED_VAR
#  define PERL_UNUSED_VAR(var) if (0) var = var
#endif

#ifndef dVAR
#  define dVAR		dNOOP
#endif


/* This stuff is not part of the API! You have been warned. */
#ifndef PERL_VERSION_DECIMAL
#  define PERL_VERSION_DECIMAL(r,v,s) (r*1000000 + v*1000 + s)
#endif
#ifndef PERL_DECIMAL_VERSION
#  define PERL_DECIMAL_VERSION \
	  PERL_VERSION_DECIMAL(PERL_REVISION,PERL_VERSION,PERL_SUBVERSION)
#endif
#ifndef PERL_VERSION_GE
#  define PERL_VERSION_GE(r,v,s) \
	  (PERL_DECIMAL_VERSION >= PERL_VERSION_DECIMAL(r,v,s))
#endif
#ifndef PERL_VERSION_LE
#  define PERL_VERSION_LE(r,v,s) \
	  (PERL_DECIMAL_VERSION <= PERL_VERSION_DECIMAL(r,v,s))
#endif

/* XS_INTERNAL is the explicit static-linkage variant of the default
 * XS macro.
 *
 * XS_EXTERNAL is the same as XS_INTERNAL except it does not include
 * "STATIC", ie. it exports XSUB symbols. You probably don't want that
 * for anything but the BOOT XSUB.
 *
 * See XSUB.h in core!
 */


/* TODO: This might be compatible further back than 5.10.0. */
#if PERL_VERSION_GE(5, 10, 0) && PERL_VERSION_LE(5, 15, 1)
#  undef XS_EXTERNAL
#  undef XS_INTERNAL
#  if defined(__CYGWIN__) && defined(USE_DYNAMIC_LOADING)
#    define XS_EXTERNAL(name) __declspec(dllexport) XSPROTO(name)
#    define XS_INTERNAL(name) STATIC XSPROTO(name)
#  endif
#  if defined(__SYMBIAN32__)
#    define XS_EXTERNAL(name) EXPORT_C XSPROTO(name)
#    define XS_INTERNAL(name) EXPORT_C STATIC XSPROTO(name)
#  endif
#  ifndef XS_EXTERNAL
#    if defined(HASATTRIBUTE_UNUSED) && !defined(__cplusplus)
#      define XS_EXTERNAL(name) void name(pTHX_ CV* cv __attribute__unused__)
#      define XS_INTERNAL(name) STATIC void name(pTHX_ CV* cv __attribute__unused__)
#    else
#      ifdef __cplusplus
#        define XS_EXTERNAL(name) extern "C" XSPROTO(name)
#        define XS_INTERNAL(name) static XSPROTO(name)
#      else
#        define XS_EXTERNAL(name) XSPROTO(name)
#        define XS_INTERNAL(name) STATIC XSPROTO(name)
#      endif
#    endif
#  endif
#endif

/* perl >= 5.10.0 && perl <= 5.15.1 */


/* The XS_EXTERNAL macro is used for functions that must not be static
 * like the boot XSUB of a module. If perl didn't have an XS_EXTERNAL
 * macro defined, the best we can do is assume XS is the same.
 * Dito for XS_INTERNAL.
 */
#ifndef XS_EXTERNAL
#  define XS_EXTERNAL(name) XS(name)
#endif
#ifndef XS_INTERNAL
#  define XS_INTERNAL(name) XS(name)
#endif

/* Now, finally, after all this mess, we want an ExtUtils::ParseXS
 * internal macro that we're free to redefine for varying linkage due
 * to the EXPORT_XSUB_SYMBOLS XS keyword. This is internal, use
 * XS_EXTERNAL(name) or XS_INTERNAL(name) in your code if you need to!
 */

#undef XS_EUPXS
#if defined(PERL_EUPXS_ALWAYS_EXPORT)
#  define XS_EUPXS(name) XS_EXTERNAL(name)
#else
   /* default to internal */
#  define XS_EUPXS(name) XS_INTERNAL(name)
#endif

#ifndef PERL_ARGS_ASSERT_CROAK_XS_USAGE
#define PERL_ARGS_ASSERT_CROAK_XS_USAGE assert(cv); assert(params)

/* prototype to pass -Wmissing-prototypes */
STATIC void
S_croak_xs_usage(pTHX_ const CV *const cv, const char *const params);

STATIC void
S_croak_xs_usage(pTHX_ const CV *const cv, const char *const params)
{
    const GV *const gv = CvGV(cv);

    PERL_ARGS_ASSERT_CROAK_XS_USAGE;

    if (gv) {
        const char *const gvname = GvNAME(gv);
        const HV *const stash = GvSTASH(gv);
        const char *const hvname = stash ? HvNAME(stash) : NULL;

        if (hvname)
            Perl_croak(aTHX_ "Usage: %s::%s(%s)", hvname, gvname, params);
        else
            Perl_croak(aTHX_ "Usage: %s(%s)", gvname, params);
    } else {
        /* Pants. I don't think that it should be possible to get here. */
        Perl_croak(aTHX_ "Usage: CODE(0x%"UVxf")(%s)", PTR2UV(cv), params);
    }
}
#undef  PERL_ARGS_ASSERT_CROAK_XS_USAGE

#ifdef PERL_IMPLICIT_CONTEXT
#define croak_xs_usage(a,b)    S_croak_xs_usage(aTHX_ a,b)
#else
#define croak_xs_usage        S_croak_xs_usage
#endif

#endif

/* NOTE: the prototype of newXSproto() is different in versions of perls,
 * so we define a portable version of newXSproto()
 */
#ifdef newXS_flags
#define newXSproto_portable(name, c_impl, file, proto) newXS_flags(name, c_impl, file, proto, 0)
#else
#define newXSproto_portable(name, c_impl, file, proto) (PL_Sv=(SV*)newXS(name, c_impl, file), sv_setpv(PL_Sv, proto), (CV*)PL_Sv)
#endif /* !defined(newXS_flags) */

#line 203 "Triceps.c"

/* INCLUDE:  Including 'const-xs.inc' from 'Triceps.xs' */


XS_EUPXS(XS_Triceps_constant); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_constant)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "sv");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
#line 4 "./const-xs.inc"
#ifdef dXSTARG
	dXSTARG; /* Faster if we have it.  */
#else
	dTARGET;
#endif
	STRLEN		len;
        int		type;
	IV		iv;
	/* NV		nv;	Uncomment this if you need to return NVs */
	/* const char	*pv;	Uncomment this if you need to return PVs */
#line 228 "Triceps.c"
	SV *	sv = ST(0)
;
	const char *	s = SvPV(sv, len);
#line 18 "./const-xs.inc"
	type = constant(aTHX_ s, len, &iv);
      /* Return 1 or 2 items. First is error message, or undef if no error.
           Second, if present, is found value */
        switch (type) {
        case PERL_constant_NOTFOUND:
          sv =
	    sv_2mortal(newSVpvf("%s is not a valid Triceps macro", s));
          PUSHs(sv);
          break;
        case PERL_constant_NOTDEF:
          sv = sv_2mortal(newSVpvf(
	    "Your vendor has not defined Triceps macro %s, used",
				   s));
          PUSHs(sv);
          break;
        case PERL_constant_ISIV:
          EXTEND(SP, 1);
          PUSHs(&PL_sv_undef);
          PUSHi(iv);
          break;
	/* Uncomment this if you need to return NOs
        case PERL_constant_ISNO:
          EXTEND(SP, 1);
          PUSHs(&PL_sv_undef);
          PUSHs(&PL_sv_no);
          break; */
	/* Uncomment this if you need to return NVs
        case PERL_constant_ISNV:
          EXTEND(SP, 1);
          PUSHs(&PL_sv_undef);
          PUSHn(nv);
          break; */
	/* Uncomment this if you need to return PVs
        case PERL_constant_ISPV:
          EXTEND(SP, 1);
          PUSHs(&PL_sv_undef);
          PUSHp(pv, strlen(pv));
          break; */
	/* Uncomment this if you need to return PVNs
        case PERL_constant_ISPVN:
          EXTEND(SP, 1);
          PUSHs(&PL_sv_undef);
          PUSHp(pv, iv);
          break; */
	/* Uncomment this if you need to return SVs
        case PERL_constant_ISSV:
          EXTEND(SP, 1);
          PUSHs(&PL_sv_undef);
          PUSHs(sv);
          break; */
	/* Uncomment this if you need to return UNDEFs
        case PERL_constant_ISUNDEF:
          break; */
	/* Uncomment this if you need to return UVs
        case PERL_constant_ISUV:
          EXTEND(SP, 1);
          PUSHs(&PL_sv_undef);
          PUSHu((UV)iv);
          break; */
	/* Uncomment this if you need to return YESs
        case PERL_constant_ISYES:
          EXTEND(SP, 1);
          PUSHs(&PL_sv_undef);
          PUSHs(&PL_sv_yes);
          break; */
        default:
          sv = sv_2mortal(newSVpvf(
	    "Unexpected return type %d while processing Triceps macro %s, used",
               type, s));
          PUSHs(sv);
        }
#line 304 "Triceps.c"
	PUTBACK;
	return;
    }
}


/* INCLUDE: Returning to 'Triceps.xs' from 'const-xs.inc' */


XS_EUPXS(XS_Triceps_isInsert); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_isInsert)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "op");
    {
	int	RETVAL;
	dXSTARG;
	int	op = (int)SvIV(ST(0))
;
#line 173 "Triceps.xs"
		clearErrMsg();
		RETVAL = Rowop::isInsert(op);
#line 328 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_isDelete); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_isDelete)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "op");
    {
	int	RETVAL;
	dXSTARG;
	int	op = (int)SvIV(ST(0))
;
#line 181 "Triceps.xs"
		clearErrMsg();
		RETVAL = Rowop::isDelete(op);
#line 349 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_isNop); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_isNop)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "op");
    {
	int	RETVAL;
	dXSTARG;
	int	op = (int)SvIV(ST(0))
;
#line 189 "Triceps.xs"
		clearErrMsg();
		RETVAL = Rowop::isNop(op);
#line 370 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_stringOpcode); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_stringOpcode)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	char *	val = (char *)SvPV_nolen(ST(0))
;
#line 202 "Triceps.xs"
		clearErrMsg();
		int res = Rowop::stringOpcode(val);
		try { do {
			if (res == Rowop::OP_BAD)
				throw Exception::f("Triceps::stringOpcode: bad opcode string '%s'", val);
		} while(0); } TRICEPS_CATCH_CROAK;
		RETVAL = res;
#line 396 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_stringOpcodeSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_stringOpcodeSafe)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	char *	val = (char *)SvPV_nolen(ST(0))
;
#line 215 "Triceps.xs"
		clearErrMsg();
		int res = Rowop::stringOpcode(val);
		if (res == Rowop::OP_BAD)
			XSRETURN_UNDEF; // not a croak
		RETVAL = res;
#line 420 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_stringOcf); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_stringOcf)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	char *	val = (char *)SvPV_nolen(ST(0))
;
#line 226 "Triceps.xs"
		clearErrMsg();
		int res = Rowop::stringOcf(val);
		try { do {
			if (res == -1)
				throw Exception::f("Triceps::stringOcf: bad opcode flag string '%s'", val);
		} while(0); } TRICEPS_CATCH_CROAK;
		RETVAL = res;
#line 446 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_stringOcfSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_stringOcfSafe)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	char *	val = (char *)SvPV_nolen(ST(0))
;
#line 239 "Triceps.xs"
		clearErrMsg();
		int res = Rowop::stringOcf(val);
		if (res == -1)
			XSRETURN_UNDEF; // not a croak
		RETVAL = res;
#line 470 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_stringEm); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_stringEm)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	char *	val = (char *)SvPV_nolen(ST(0))
;
#line 250 "Triceps.xs"
		clearErrMsg();
		int res = Gadget::stringEm(val);
		try { do {
			if (res == -1)
				throw Exception::f("Triceps::stringEm: bad enqueueing mode string '%s'", val);
		} while(0); } TRICEPS_CATCH_CROAK;
		RETVAL = res;
#line 496 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_stringEmSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_stringEmSafe)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	char *	val = (char *)SvPV_nolen(ST(0))
;
#line 263 "Triceps.xs"
		clearErrMsg();
		int res = Gadget::stringEm(val);
		if (res == -1)
			XSRETURN_UNDEF; // not a croak
		RETVAL = res;
#line 520 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_stringTracerWhen); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_stringTracerWhen)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	char *	val = (char *)SvPV_nolen(ST(0))
;
#line 274 "Triceps.xs"
		clearErrMsg();
		int res = Unit::stringTracerWhen(val);
		try { do {
			if (res == -1)
				throw Exception::f("Triceps::stringTracerWhen: bad TracerWhen string '%s'", val);
		} while(0); } TRICEPS_CATCH_CROAK;
		RETVAL = res;
#line 546 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_stringTracerWhenSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_stringTracerWhenSafe)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	char *	val = (char *)SvPV_nolen(ST(0))
;
#line 287 "Triceps.xs"
		clearErrMsg();
		int res = Unit::stringTracerWhen(val);
		if (res == -1)
			XSRETURN_UNDEF; // not a croak
		RETVAL = res;
#line 570 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_humanStringTracerWhen); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_humanStringTracerWhen)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	char *	val = (char *)SvPV_nolen(ST(0))
;
#line 298 "Triceps.xs"
		clearErrMsg();
		int res = Unit::humanStringTracerWhen(val);
		try { do {
			if (res == -1)
				throw Exception::f("Triceps::humanStringTracerWhen: bad human-readable TracerWhen string '%s'", val);
		} while(0); } TRICEPS_CATCH_CROAK;
		RETVAL = res;
#line 596 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_humanStringTracerWhenSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_humanStringTracerWhenSafe)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	char *	val = (char *)SvPV_nolen(ST(0))
;
#line 311 "Triceps.xs"
		clearErrMsg();
		int res = Unit::humanStringTracerWhen(val);
		if (res == -1)
			XSRETURN_UNDEF; // not a croak
		RETVAL = res;
#line 620 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_stringIndexId); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_stringIndexId)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	char *	val = (char *)SvPV_nolen(ST(0))
;
#line 322 "Triceps.xs"
		clearErrMsg();
		int res = IndexType::stringIndexId(val);
		try { do {
			if (res == -1)
				throw Exception::f("Triceps::stringIndexId: bad index id string '%s'", val);
		} while(0); } TRICEPS_CATCH_CROAK;
		RETVAL = res;
#line 646 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_stringIndexIdSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_stringIndexIdSafe)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	char *	val = (char *)SvPV_nolen(ST(0))
;
#line 335 "Triceps.xs"
		clearErrMsg();
		int res = IndexType::stringIndexId(val);
		if (res == -1)
			XSRETURN_UNDEF; // not a croak
		RETVAL = res;
#line 670 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_stringAggOp); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_stringAggOp)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	char *	val = (char *)SvPV_nolen(ST(0))
;
#line 346 "Triceps.xs"
		clearErrMsg();
		int res = Aggregator::stringAggOp(val);
		try { do {
			if (res == -1)
				throw Exception::f("Triceps::stringAggOp: bad aggregation opcode string '%s'", val);
		} while(0); } TRICEPS_CATCH_CROAK;
		RETVAL = res;
#line 696 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_stringAggOpSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_stringAggOpSafe)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	char *	val = (char *)SvPV_nolen(ST(0))
;
#line 359 "Triceps.xs"
		clearErrMsg();
		int res = Aggregator::stringAggOp(val);
		if (res == -1)
			XSRETURN_UNDEF; // not a croak
		RETVAL = res;
#line 720 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_opcodeString); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_opcodeString)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	char *	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 373 "Triceps.xs"
		clearErrMsg();
		const char *res = Rowop::opcodeString(val); // never returns NULL
		RETVAL = (char *)res;
#line 742 "Triceps.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_opcodeStringSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_opcodeStringSafe)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	char *	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 383 "Triceps.xs"
		clearErrMsg();
		const char *res = Rowop::opcodeString(val); // never returns NULL
		RETVAL = (char *)res;
#line 764 "Triceps.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_ocfString); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_ocfString)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	char *	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 392 "Triceps.xs"
		clearErrMsg();
		const char *res = Rowop::ocfString(val, NULL);
		try { do {
			if (res == NULL)
				throw Exception::f("Triceps::ocfString: opcode flag value '%d' not defined in the enum", val);
		} while(0); } TRICEPS_CATCH_CROAK;
		RETVAL = (char *)res;
#line 790 "Triceps.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_ocfStringSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_ocfStringSafe)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	char *	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 405 "Triceps.xs"
		clearErrMsg();
		const char *res = Rowop::ocfString(val, NULL);
		if (res == NULL)
			XSRETURN_UNDEF; // not a croak
		RETVAL = (char *)res;
#line 814 "Triceps.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_emString); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_emString)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	char *	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 416 "Triceps.xs"
		clearErrMsg();
		const char *res = Gadget::emString(val, NULL);
		try { do {
			if (res == NULL)
				throw Exception::f("Triceps::emString: enqueueing mode value '%d' not defined in the enum", val);
		} while(0); } TRICEPS_CATCH_CROAK;
		RETVAL = (char *)res;
#line 840 "Triceps.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_emStringSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_emStringSafe)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	char *	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 429 "Triceps.xs"
		clearErrMsg();
		const char *res = Gadget::emString(val, NULL);
		if (res == NULL)
			XSRETURN_UNDEF; // not a croak
		RETVAL = (char *)res;
#line 864 "Triceps.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_tracerWhenString); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_tracerWhenString)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	char *	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 440 "Triceps.xs"
		clearErrMsg();
		const char *res = Unit::tracerWhenString(val, NULL);
		try { do {
			if (res == NULL)
				throw Exception::f("Triceps::tracerWhenString: TracerWhen value '%d' not defined in the enum", val);
		} while(0); } TRICEPS_CATCH_CROAK;
		RETVAL = (char *)res;
#line 890 "Triceps.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_tracerWhenStringSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_tracerWhenStringSafe)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	char *	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 453 "Triceps.xs"
		clearErrMsg();
		const char *res = Unit::tracerWhenString(val, NULL);
		if (res == NULL)
			XSRETURN_UNDEF; // not a croak
		RETVAL = (char *)res;
#line 914 "Triceps.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_tracerWhenHumanString); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_tracerWhenHumanString)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	char *	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 464 "Triceps.xs"
		clearErrMsg();
		const char *res = Unit::tracerWhenHumanString(val, NULL);
		try { do {
			if (res == NULL)
				throw Exception::f("Triceps::tracerWhenHumanString: TracerWhen value '%d' not defined in the enum", val);
		} while(0); } TRICEPS_CATCH_CROAK;
		RETVAL = (char *)res;
#line 940 "Triceps.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_tracerWhenHumanStringSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_tracerWhenHumanStringSafe)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	char *	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 477 "Triceps.xs"
		clearErrMsg();
		const char *res = Unit::tracerWhenHumanString(val, NULL);
		if (res == NULL)
			XSRETURN_UNDEF; // not a croak
		RETVAL = (char *)res;
#line 964 "Triceps.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_indexIdString); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_indexIdString)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	char *	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 488 "Triceps.xs"
		clearErrMsg();
		const char *res = IndexType::indexIdString(val, NULL);
		try { do {
			if (res == NULL)
				throw Exception::f("Triceps::indexIdString: index id value '%d' not defined in the enum", val);
		} while(0); } TRICEPS_CATCH_CROAK;
		RETVAL = (char *)res;
#line 990 "Triceps.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_indexIdStringSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_indexIdStringSafe)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	char *	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 501 "Triceps.xs"
		clearErrMsg();
		const char *res = IndexType::indexIdString(val, NULL);
		if (res == NULL)
			XSRETURN_UNDEF; // not a croak
		RETVAL = (char *)res;
#line 1014 "Triceps.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_aggOpString); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_aggOpString)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	char *	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 512 "Triceps.xs"
		clearErrMsg();
		const char *res = Aggregator::aggOpString(val, NULL);
		try { do {
			if (res == NULL)
				throw Exception::f("Triceps::aggOpString: aggregation opcode value '%d' not defined in the enum", val);
		} while(0); } TRICEPS_CATCH_CROAK;
		RETVAL = (char *)res;
#line 1040 "Triceps.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_aggOpStringSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_aggOpStringSafe)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	char *	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 525 "Triceps.xs"
		clearErrMsg();
		const char *res = Aggregator::aggOpString(val, NULL);
		if (res == NULL)
			XSRETURN_UNDEF; // not a croak
		RETVAL = (char *)res;
#line 1064 "Triceps.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_tracerWhenIsBefore); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_tracerWhenIsBefore)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 537 "Triceps.xs"
		clearErrMsg();
		RETVAL = Unit::tracerWhenIsBefore(val);
#line 1085 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_tracerWhenIsAfter); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_tracerWhenIsAfter)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "val");
    {
	int	RETVAL;
	dXSTARG;
	int	val = (int)SvIV(ST(0))
;
#line 546 "Triceps.xs"
		clearErrMsg();
		RETVAL = Unit::tracerWhenIsAfter(val);
#line 1106 "Triceps.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_now); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_now)
{
    dVAR; dXSARGS;
    if (items != 0)
       croak_xs_usage(cv,  "");
    {
	double	RETVAL;
	dXSTARG;
#line 557 "Triceps.xs"
		timespec tm;
		clock_gettime(CLOCK_REALTIME, &tm);
		RETVAL = (double)tm.tv_sec + (double)tm.tv_nsec / 1000000000.;
#line 1126 "Triceps.c"
	XSprePUSH; PUSHn((double)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps_sigusr2_setup); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps_sigusr2_setup)
{
    dVAR; dXSARGS;
    if (items != 0)
       croak_xs_usage(cv,  "");
    {
#line 571 "Triceps.xs"
		Sigusr2::setup();
#line 1142 "Triceps.c"
    }
    XSRETURN_EMPTY;
}

#ifdef __cplusplus
extern "C"
#endif
XS_EXTERNAL(boot_Triceps); /* prototype to pass -Wmissing-prototypes */
XS_EXTERNAL(boot_Triceps)
{
    dVAR; dXSARGS;
#if (PERL_REVISION == 5 && PERL_VERSION < 9)
    char* file = __FILE__;
#else
    const char* file = __FILE__;
#endif

    PERL_UNUSED_VAR(cv); /* -W */
    PERL_UNUSED_VAR(items); /* -W */
#ifdef XS_APIVERSION_BOOTCHECK
    XS_APIVERSION_BOOTCHECK;
#endif
    XS_VERSION_BOOTCHECK;

        newXS("Triceps::constant", XS_Triceps_constant, file);
        newXS("Triceps::isInsert", XS_Triceps_isInsert, file);
        newXS("Triceps::isDelete", XS_Triceps_isDelete, file);
        newXS("Triceps::isNop", XS_Triceps_isNop, file);
        newXS("Triceps::stringOpcode", XS_Triceps_stringOpcode, file);
        newXS("Triceps::stringOpcodeSafe", XS_Triceps_stringOpcodeSafe, file);
        newXS("Triceps::stringOcf", XS_Triceps_stringOcf, file);
        newXS("Triceps::stringOcfSafe", XS_Triceps_stringOcfSafe, file);
        newXS("Triceps::stringEm", XS_Triceps_stringEm, file);
        newXS("Triceps::stringEmSafe", XS_Triceps_stringEmSafe, file);
        newXS("Triceps::stringTracerWhen", XS_Triceps_stringTracerWhen, file);
        newXS("Triceps::stringTracerWhenSafe", XS_Triceps_stringTracerWhenSafe, file);
        newXS("Triceps::humanStringTracerWhen", XS_Triceps_humanStringTracerWhen, file);
        newXS("Triceps::humanStringTracerWhenSafe", XS_Triceps_humanStringTracerWhenSafe, file);
        newXS("Triceps::stringIndexId", XS_Triceps_stringIndexId, file);
        newXS("Triceps::stringIndexIdSafe", XS_Triceps_stringIndexIdSafe, file);
        newXS("Triceps::stringAggOp", XS_Triceps_stringAggOp, file);
        newXS("Triceps::stringAggOpSafe", XS_Triceps_stringAggOpSafe, file);
        newXS("Triceps::opcodeString", XS_Triceps_opcodeString, file);
        newXS("Triceps::opcodeStringSafe", XS_Triceps_opcodeStringSafe, file);
        newXS("Triceps::ocfString", XS_Triceps_ocfString, file);
        newXS("Triceps::ocfStringSafe", XS_Triceps_ocfStringSafe, file);
        newXS("Triceps::emString", XS_Triceps_emString, file);
        newXS("Triceps::emStringSafe", XS_Triceps_emStringSafe, file);
        newXS("Triceps::tracerWhenString", XS_Triceps_tracerWhenString, file);
        newXS("Triceps::tracerWhenStringSafe", XS_Triceps_tracerWhenStringSafe, file);
        newXS("Triceps::tracerWhenHumanString", XS_Triceps_tracerWhenHumanString, file);
        newXS("Triceps::tracerWhenHumanStringSafe", XS_Triceps_tracerWhenHumanStringSafe, file);
        newXS("Triceps::indexIdString", XS_Triceps_indexIdString, file);
        newXS("Triceps::indexIdStringSafe", XS_Triceps_indexIdStringSafe, file);
        newXS("Triceps::aggOpString", XS_Triceps_aggOpString, file);
        newXS("Triceps::aggOpStringSafe", XS_Triceps_aggOpStringSafe, file);
        newXS("Triceps::tracerWhenIsBefore", XS_Triceps_tracerWhenIsBefore, file);
        newXS("Triceps::tracerWhenIsAfter", XS_Triceps_tracerWhenIsAfter, file);
        newXS("Triceps::now", XS_Triceps_now, file);
        newXS("Triceps::sigusr2_setup", XS_Triceps_sigusr2_setup, file);

    /* Initialisation Section */

#line 54 "Triceps.xs"
	// the exceptions will be caught and backtraced in Perl
	TRICEPS_NS::Exception::abort_ = false;
	TRICEPS_NS::Exception::enableBacktrace_ = false;
	//
	// boot sub-packages that are compiled separately
	//
	// fprintf(stderr, "DEBUG Triceps items=%d sp=%p mark=%p\n", items, sp, mark);
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__Label(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	// fprintf(stderr, "DEBUG Triceps items=%d sp=%p mark=%p\n", items, sp, mark);
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__Row(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__Rowop(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__RowHandle(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	// fprintf(stderr, "DEBUG Triceps items=%d sp=%p mark=%p\n", items, sp, mark);
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__RowType(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__IndexType(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__TableType(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__Tray(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__Unit(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__UnitTracer(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__Table(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__AggregatorType(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__AggregatorContext(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__FrameMark(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__FnReturn(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__FnBinding(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__AutoFnBind(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__App(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__Triead(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__TrieadOwner(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__Facet(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__Nexus(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__AutoDrain(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__PerlValue(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	PUSHMARK(SP); if (items >= 2) { XPUSHs(ST(0)); XPUSHs(ST(1)); } PUTBACK; 
	boot_Triceps__TrackedFile(aTHX_ cv); 
	SPAGAIN; POPs;
	//
	// fprintf(stderr, "DEBUG Triceps items=%d sp=%p mark=%p\n", items, sp, mark);

#line 1318 "Triceps.c"

    /* End of Initialisation Section */

#if (PERL_REVISION == 5 && PERL_VERSION >= 9)
  if (PL_unitcheckav)
       call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
}

