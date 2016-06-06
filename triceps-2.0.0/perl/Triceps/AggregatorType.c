/*
 * This file was generated automatically by ExtUtils::ParseXS version 3.18 from the
 * contents of AggregatorType.xs. Do not edit this file, edit AggregatorType.xs instead.
 *
 *    ANY CHANGES MADE HERE WILL BE LOST!
 *
 */

#line 1 "AggregatorType.xs"
//
// (C) Copyright 2011-2014 Sergey A. Babkin.
// This file is a part of Triceps.
// See the file COPYRIGHT for the copyright notice and license information
//
// The wrapper for AggregatorType.

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include "TricepsPerl.h"
#include "PerlCallback.h"
#include "PerlAggregator.h"

#line 28 "AggregatorType.c"
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

#line 170 "AggregatorType.c"

XS_EUPXS(XS_Triceps__AggregatorType_CLONE_SKIP); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__AggregatorType_CLONE_SKIP)
{
    dVAR; dXSARGS;
    PERL_UNUSED_VAR(cv); /* -W */
    {
	int	RETVAL;
	dXSTARG;
#line 24 "AggregatorType.xs"
		RETVAL = 1;
#line 182 "AggregatorType.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__AggregatorType_DESTROY); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__AggregatorType_DESTROY)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	WrapAggregatorType *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapAggregatorType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::AggregatorType::DESTROY(): self has an incorrect magic for WrapAggregatorTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::AggregatorType::DESTROY(): self is not a blessed SV reference to WrapAggregatorTypePtr" );
	}
;
#line 31 "AggregatorType.xs"
		// warn("AggregatorType destroyed!");
		delete self;
#line 210 "AggregatorType.c"
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Triceps__AggregatorType_new); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__AggregatorType_new)
{
    dVAR; dXSARGS;
    if (items < 5)
       croak_xs_usage(cv,  "CLASS, wrt, name, constructor, handler, ...");
    {
	WrapAggregatorType *	RETVAL;
	char *	CLASS = (char *)SvPV_nolen(ST(0))
;
	WrapRowType *	wrt;
	char *	name = (char *)SvPV_nolen(ST(2))
;
	SV *	constructor = ST(3)
;
	SV *	handler = ST(4)
;

	if( sv_isobject(ST(1)) && (SvTYPE(SvRV(ST(1))) == SVt_PVMG) ) {
		wrt = (WrapRowType *)SvIV((SV*)SvRV( ST(1) ));
		if (wrt == 0 || wrt->badMagic()) {
			croakWithMsg( "Triceps::AggregatorType::new(): wrt has an incorrect magic for WrapRowTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::AggregatorType::new(): wrt is not a blessed SV reference to WrapRowTypePtr" );
	}
;
#line 46 "AggregatorType.xs"
		static char funcName[] =  "Triceps::AggregatorType::new";
		RETVAL = NULL; // shut up the warning

		try { do {
			clearErrMsg();

			RowType *rt = wrt->get();

			Onceref<PerlCallback> cbconst; // defaults to NULL
			if (SvOK(constructor)) {
				cbconst = new PerlCallback(true);
				PerlCallbackInitializeSplit(cbconst, "Triceps::AggregatorType::new(constructor)", constructor, 5, items-5); // may throw
			}

			Onceref<PerlCallback> cbhand = new PerlCallback(true);
			PerlCallbackInitialize(cbhand, "Triceps::AggregatorType::new(handler)", 4, items-4); // may throw

			RETVAL = new WrapAggregatorType(new PerlAggregatorType(name, rt, NULL, cbconst, cbhand));
		} while(0); } TRICEPS_CATCH_CROAK;
#line 263 "AggregatorType.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__AggregatorType_copy); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__AggregatorType_copy)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	WrapAggregatorType *	RETVAL;
	WrapAggregatorType *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapAggregatorType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::AggregatorType::copy(): self has an incorrect magic for WrapAggregatorTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::AggregatorType::copy(): self is not a blessed SV reference to WrapAggregatorTypePtr" );
	}
;
#line 126 "AggregatorType.xs"
		// for casting of return value
		static char CLASS[] = "Triceps::AggregatorType";

		clearErrMsg();
		PerlAggregatorType *agt = self->get();
		RETVAL = new WrapAggregatorType(static_cast<PerlAggregatorType *>( agt->copy() ));
#line 297 "AggregatorType.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__AggregatorType_same); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__AggregatorType_same)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "self, other");
    {
	int	RETVAL;
	dXSTARG;
	WrapAggregatorType *	self;
	WrapAggregatorType *	other;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapAggregatorType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::AggregatorType::same(): self has an incorrect magic for WrapAggregatorTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::AggregatorType::same(): self is not a blessed SV reference to WrapAggregatorTypePtr" );
	}
;

	if( sv_isobject(ST(1)) && (SvTYPE(SvRV(ST(1))) == SVt_PVMG) ) {
		other = (WrapAggregatorType *)SvIV((SV*)SvRV( ST(1) ));
		if (other == 0 || other->badMagic()) {
			croakWithMsg( "Triceps::AggregatorType::same(): other has an incorrect magic for WrapAggregatorTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::AggregatorType::same(): other is not a blessed SV reference to WrapAggregatorTypePtr" );
	}
;
#line 138 "AggregatorType.xs"
		clearErrMsg();
		PerlAggregatorType *agself = self->get();
		PerlAggregatorType *agother = other->get();
		RETVAL = (agself == agother);
#line 341 "AggregatorType.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__AggregatorType_getRowType); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__AggregatorType_getRowType)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	WrapRowType *	RETVAL;
	WrapAggregatorType *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapAggregatorType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::AggregatorType::getRowType(): self has an incorrect magic for WrapAggregatorTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::AggregatorType::getRowType(): self is not a blessed SV reference to WrapAggregatorTypePtr" );
	}
;
#line 149 "AggregatorType.xs"
		// for casting of return value
		static char CLASS[] = "Triceps::RowType";

		clearErrMsg();
		PerlAggregatorType *agself = self->get();
		RETVAL = new WrapRowType(const_cast<RowType *>(agself->getRowType()));
#line 374 "AggregatorType.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__AggregatorType_print); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__AggregatorType_print)
{
    dVAR; dXSARGS;
    if (items < 1)
       croak_xs_usage(cv,  "self, ...");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	SV *	RETVAL;
	WrapAggregatorType *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapAggregatorType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::AggregatorType::print(): self has an incorrect magic for WrapAggregatorTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::AggregatorType::print(): self is not a blessed SV reference to WrapAggregatorTypePtr" );
	}
;
#line 164 "AggregatorType.xs"
		GEN_PRINT_METHOD(PerlAggregatorType)
#line 405 "AggregatorType.c"
	PUTBACK;
	return;
    }
}


XS_EUPXS(XS_Triceps__AggregatorType_equals); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__AggregatorType_equals)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "self, other");
    {
	int	RETVAL;
	dXSTARG;
	WrapAggregatorType *	self;
	WrapAggregatorType *	other;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapAggregatorType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::AggregatorType::equals(): self has an incorrect magic for WrapAggregatorTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::AggregatorType::equals(): self is not a blessed SV reference to WrapAggregatorTypePtr" );
	}
;

	if( sv_isobject(ST(1)) && (SvTYPE(SvRV(ST(1))) == SVt_PVMG) ) {
		other = (WrapAggregatorType *)SvIV((SV*)SvRV( ST(1) ));
		if (other == 0 || other->badMagic()) {
			croakWithMsg( "Triceps::AggregatorType::equals(): other has an incorrect magic for WrapAggregatorTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::AggregatorType::equals(): other is not a blessed SV reference to WrapAggregatorTypePtr" );
	}
;
#line 170 "AggregatorType.xs"
		clearErrMsg();
		PerlAggregatorType *agself = self->get();
		PerlAggregatorType *agother = other->get();
		RETVAL = agself->equals(agother);
#line 448 "AggregatorType.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__AggregatorType_match); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__AggregatorType_match)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "self, other");
    {
	int	RETVAL;
	dXSTARG;
	WrapAggregatorType *	self;
	WrapAggregatorType *	other;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapAggregatorType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::AggregatorType::match(): self has an incorrect magic for WrapAggregatorTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::AggregatorType::match(): self is not a blessed SV reference to WrapAggregatorTypePtr" );
	}
;

	if( sv_isobject(ST(1)) && (SvTYPE(SvRV(ST(1))) == SVt_PVMG) ) {
		other = (WrapAggregatorType *)SvIV((SV*)SvRV( ST(1) ));
		if (other == 0 || other->badMagic()) {
			croakWithMsg( "Triceps::AggregatorType::match(): other has an incorrect magic for WrapAggregatorTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::AggregatorType::match(): other is not a blessed SV reference to WrapAggregatorTypePtr" );
	}
;
#line 180 "AggregatorType.xs"
		clearErrMsg();
		PerlAggregatorType *agself = self->get();
		PerlAggregatorType *agother = other->get();
		RETVAL = agself->match(agother);
#line 491 "AggregatorType.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}

#ifdef __cplusplus
extern "C"
#endif
XS_EXTERNAL(boot_Triceps__AggregatorType); /* prototype to pass -Wmissing-prototypes */
XS_EXTERNAL(boot_Triceps__AggregatorType)
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

        newXS("Triceps::AggregatorType::CLONE_SKIP", XS_Triceps__AggregatorType_CLONE_SKIP, file);
        newXS("Triceps::AggregatorType::DESTROY", XS_Triceps__AggregatorType_DESTROY, file);
        newXS("Triceps::AggregatorType::new", XS_Triceps__AggregatorType_new, file);
        newXS("Triceps::AggregatorType::copy", XS_Triceps__AggregatorType_copy, file);
        newXS("Triceps::AggregatorType::same", XS_Triceps__AggregatorType_same, file);
        newXS("Triceps::AggregatorType::getRowType", XS_Triceps__AggregatorType_getRowType, file);
        newXS("Triceps::AggregatorType::print", XS_Triceps__AggregatorType_print, file);
        newXS("Triceps::AggregatorType::equals", XS_Triceps__AggregatorType_equals, file);
        newXS("Triceps::AggregatorType::match", XS_Triceps__AggregatorType_match, file);
#if (PERL_REVISION == 5 && PERL_VERSION >= 9)
  if (PL_unitcheckav)
       call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
}

