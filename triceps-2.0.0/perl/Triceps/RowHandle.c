/*
 * This file was generated automatically by ExtUtils::ParseXS version 3.18 from the
 * contents of RowHandle.xs. Do not edit this file, edit RowHandle.xs instead.
 *
 *    ANY CHANGES MADE HERE WILL BE LOST!
 *
 */

#line 1 "RowHandle.xs"
//
// (C) Copyright 2011-2014 Sergey A. Babkin.
// This file is a part of Triceps.
// See the file COPYRIGHT for the copyright notice and license information
//
// The wrapper for RowHandle.

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include "TricepsPerl.h"

#line 26 "RowHandle.c"
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

#line 168 "RowHandle.c"

XS_EUPXS(XS_Triceps__RowHandle_CLONE_SKIP); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__RowHandle_CLONE_SKIP)
{
    dVAR; dXSARGS;
    PERL_UNUSED_VAR(cv); /* -W */
    {
	int	RETVAL;
	dXSTARG;
#line 22 "RowHandle.xs"
		RETVAL = 1;
#line 180 "RowHandle.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__RowHandle_DESTROY); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__RowHandle_DESTROY)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	WrapRowHandle *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRowHandle *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::RowHandle::DESTROY(): self has an incorrect magic for WrapRowHandlePtr" );
		}
	} else{
		croakWithMsg( "Triceps::RowHandle::DESTROY(): self is not a blessed SV reference to WrapRowHandlePtr" );
	}
;
#line 29 "RowHandle.xs"
		// warn("RowHandle destroyed!");
		delete self;
#line 208 "RowHandle.c"
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Triceps__RowHandle_same); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__RowHandle_same)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "self, other");
    {
	int	RETVAL;
	dXSTARG;
	WrapRowHandle *	self;
	WrapRowHandle *	other;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRowHandle *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::RowHandle::same(): self has an incorrect magic for WrapRowHandlePtr" );
		}
	} else{
		croakWithMsg( "Triceps::RowHandle::same(): self is not a blessed SV reference to WrapRowHandlePtr" );
	}
;

	if( sv_isobject(ST(1)) && (SvTYPE(SvRV(ST(1))) == SVt_PVMG) ) {
		other = (WrapRowHandle *)SvIV((SV*)SvRV( ST(1) ));
		if (other == 0 || other->badMagic()) {
			croakWithMsg( "Triceps::RowHandle::same(): other has an incorrect magic for WrapRowHandlePtr" );
		}
	} else{
		croakWithMsg( "Triceps::RowHandle::same(): other is not a blessed SV reference to WrapRowHandlePtr" );
	}
;
#line 36 "RowHandle.xs"
		clearErrMsg();
		RowHandle *r1 = self->get();
		RowHandle *r2 = other->get();
		RETVAL = (r1 == r2);
#line 250 "RowHandle.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__RowHandle_getRow); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__RowHandle_getRow)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	WrapRow *	RETVAL;
	WrapRowHandle *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRowHandle *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::RowHandle::getRow(): self has an incorrect magic for WrapRowHandlePtr" );
		}
	} else{
		croakWithMsg( "Triceps::RowHandle::getRow(): self is not a blessed SV reference to WrapRowHandlePtr" );
	}
;
#line 50 "RowHandle.xs"
		// for casting of return value
		static char CLASS[] = "Triceps::Row";
		clearErrMsg();
		RowHandle *rh = self->get();

		try { do {
			if (rh == NULL) {
				throw Exception::f("Triceps::RowHandle::getRow: RowHandle is NULL");
			}
		} while(0); } TRICEPS_CATCH_CROAK;

		// XXX Should it check for row being NULL? C++ code can create that...
		RETVAL = new WrapRow(const_cast<RowType *>(self->ref_.getTable()->getRowType()), const_cast<Row *>(rh->getRow()));
#line 290 "RowHandle.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__RowHandle_getRowSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__RowHandle_getRowSafe)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	WrapRow *	RETVAL;
	WrapRowHandle *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRowHandle *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::RowHandle::getRowSafe(): self has an incorrect magic for WrapRowHandlePtr" );
		}
	} else{
		croakWithMsg( "Triceps::RowHandle::getRowSafe(): self is not a blessed SV reference to WrapRowHandlePtr" );
	}
;
#line 69 "RowHandle.xs"
		// for casting of return value
		static char CLASS[] = "Triceps::Row";
		clearErrMsg();
		RowHandle *rh = self->get();

		if (rh == NULL) {
			XSRETURN_UNDEF; // not a croak!
		}

		// XXX Should it check for row being NULL? C++ code can create that...
		RETVAL = new WrapRow(const_cast<RowType *>(self->ref_.getTable()->getRowType()), const_cast<Row *>(rh->getRow()));
#line 329 "RowHandle.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__RowHandle_isInTable); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__RowHandle_isInTable)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	int	RETVAL;
	dXSTARG;
	WrapRowHandle *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRowHandle *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::RowHandle::isInTable(): self has an incorrect magic for WrapRowHandlePtr" );
		}
	} else{
		croakWithMsg( "Triceps::RowHandle::isInTable(): self is not a blessed SV reference to WrapRowHandlePtr" );
	}
;
#line 86 "RowHandle.xs"
		clearErrMsg();
		RowHandle *rh = self->get();

		if (rh == NULL)
			RETVAL = 0;
		else
			RETVAL = rh->isInTable();
#line 365 "RowHandle.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__RowHandle_isNull); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__RowHandle_isNull)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	int	RETVAL;
	dXSTARG;
	WrapRowHandle *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRowHandle *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::RowHandle::isNull(): self has an incorrect magic for WrapRowHandlePtr" );
		}
	} else{
		croakWithMsg( "Triceps::RowHandle::isNull(): self is not a blessed SV reference to WrapRowHandlePtr" );
	}
;
#line 100 "RowHandle.xs"
		clearErrMsg();
		RowHandle *rh = self->get();
		RETVAL = (rh == NULL);
#line 396 "RowHandle.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__RowHandle_next); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__RowHandle_next)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	WrapRowHandle *	RETVAL;
	WrapRowHandle *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRowHandle *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::RowHandle::next(): self has an incorrect magic for WrapRowHandlePtr" );
		}
	} else{
		croakWithMsg( "Triceps::RowHandle::next(): self is not a blessed SV reference to WrapRowHandlePtr" );
	}
;
#line 112 "RowHandle.xs"
		static char CLASS[] = "Triceps::RowHandle";

		clearErrMsg();
		Table *t = self->ref_.getTable();
		RowHandle *cur = self->get(); // NULL is OK

		RETVAL = new WrapRowHandle(t, t->next(cur));
#line 430 "RowHandle.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__RowHandle_nextIdx); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__RowHandle_nextIdx)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "self, widx");
    {
	WrapRowHandle *	RETVAL;
	WrapRowHandle *	self;
	WrapIndexType *	widx;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRowHandle *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::RowHandle::nextIdx(): self has an incorrect magic for WrapRowHandlePtr" );
		}
	} else{
		croakWithMsg( "Triceps::RowHandle::nextIdx(): self is not a blessed SV reference to WrapRowHandlePtr" );
	}
;

	if( sv_isobject(ST(1)) && (SvTYPE(SvRV(ST(1))) == SVt_PVMG) ) {
		widx = (WrapIndexType *)SvIV((SV*)SvRV( ST(1) ));
		if (widx == 0 || widx->badMagic()) {
			croakWithMsg( "Triceps::RowHandle::nextIdx(): widx has an incorrect magic for WrapIndexTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::RowHandle::nextIdx(): widx is not a blessed SV reference to WrapIndexTypePtr" );
	}
;
#line 125 "RowHandle.xs"
		static char CLASS[] = "Triceps::RowHandle";
		static char funcName[] =  "Triceps::RowHandle::nextIdx";

		RETVAL = NULL; // shut up the warning
		try { do {
			clearErrMsg();
			Table *t = self->ref_.getTable();
			IndexType *idx = widx->get();
			RowHandle *cur = self->get(); // NULL is OK

			if (idx->getTabtype() != t->getType()) {
				throw TRICEPS_NS::Exception(strprintf("%s: indexType argument does not belong to table's type", funcName), false);
			}

			RETVAL = new WrapRowHandle(t, t->nextIdx(idx, cur));
		} while(0); } TRICEPS_CATCH_CROAK;
#line 485 "RowHandle.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__RowHandle_firstOfGroupIdx); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__RowHandle_firstOfGroupIdx)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "self, widx");
    {
	WrapRowHandle *	RETVAL;
	WrapRowHandle *	self;
	WrapIndexType *	widx;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRowHandle *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::RowHandle::firstOfGroupIdx(): self has an incorrect magic for WrapRowHandlePtr" );
		}
	} else{
		croakWithMsg( "Triceps::RowHandle::firstOfGroupIdx(): self is not a blessed SV reference to WrapRowHandlePtr" );
	}
;

	if( sv_isobject(ST(1)) && (SvTYPE(SvRV(ST(1))) == SVt_PVMG) ) {
		widx = (WrapIndexType *)SvIV((SV*)SvRV( ST(1) ));
		if (widx == 0 || widx->badMagic()) {
			croakWithMsg( "Triceps::RowHandle::firstOfGroupIdx(): widx has an incorrect magic for WrapIndexTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::RowHandle::firstOfGroupIdx(): widx is not a blessed SV reference to WrapIndexTypePtr" );
	}
;
#line 147 "RowHandle.xs"
		static char CLASS[] = "Triceps::RowHandle";
		static char funcName[] =  "Triceps::RowHandle::firstOfGroupIdx";

		RETVAL = NULL; // shut up the warning
		try { do {
			clearErrMsg();
			Table *t = self->ref_.getTable();
			IndexType *idx = widx->get();
			RowHandle *cur = self->get(); // NULL is OK

			if (idx->getTabtype() != t->getType()) {
				throw TRICEPS_NS::Exception(strprintf("%s: indexType argument does not belong to table's type", funcName), false);
			}

			RETVAL = new WrapRowHandle(t, t->firstOfGroupIdx(idx, cur));
		} while(0); } TRICEPS_CATCH_CROAK;
#line 540 "RowHandle.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__RowHandle_nextGroupIdx); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__RowHandle_nextGroupIdx)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "self, widx");
    {
	WrapRowHandle *	RETVAL;
	WrapRowHandle *	self;
	WrapIndexType *	widx;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRowHandle *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::RowHandle::nextGroupIdx(): self has an incorrect magic for WrapRowHandlePtr" );
		}
	} else{
		croakWithMsg( "Triceps::RowHandle::nextGroupIdx(): self is not a blessed SV reference to WrapRowHandlePtr" );
	}
;

	if( sv_isobject(ST(1)) && (SvTYPE(SvRV(ST(1))) == SVt_PVMG) ) {
		widx = (WrapIndexType *)SvIV((SV*)SvRV( ST(1) ));
		if (widx == 0 || widx->badMagic()) {
			croakWithMsg( "Triceps::RowHandle::nextGroupIdx(): widx has an incorrect magic for WrapIndexTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::RowHandle::nextGroupIdx(): widx is not a blessed SV reference to WrapIndexTypePtr" );
	}
;
#line 169 "RowHandle.xs"
		static char CLASS[] = "Triceps::RowHandle";
		static char funcName[] =  "Triceps::RowHandle::nextGroupIdx";

		RETVAL = NULL; // shut up the warning
		try { do {
			clearErrMsg();
			Table *t = self->ref_.getTable();
			IndexType *idx = widx->get();
			RowHandle *cur = self->get(); // NULL is OK

			if (idx->getTabtype() != t->getType()) {
				throw TRICEPS_NS::Exception(strprintf("%s: indexType argument does not belong to table's type", funcName), false);
			}

			RETVAL = new WrapRowHandle(t, t->nextGroupIdx(idx, cur));
		} while(0); } TRICEPS_CATCH_CROAK;
#line 595 "RowHandle.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}

#ifdef __cplusplus
extern "C"
#endif
XS_EXTERNAL(boot_Triceps__RowHandle); /* prototype to pass -Wmissing-prototypes */
XS_EXTERNAL(boot_Triceps__RowHandle)
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

        newXS("Triceps::RowHandle::CLONE_SKIP", XS_Triceps__RowHandle_CLONE_SKIP, file);
        newXS("Triceps::RowHandle::DESTROY", XS_Triceps__RowHandle_DESTROY, file);
        newXS("Triceps::RowHandle::same", XS_Triceps__RowHandle_same, file);
        newXS("Triceps::RowHandle::getRow", XS_Triceps__RowHandle_getRow, file);
        newXS("Triceps::RowHandle::getRowSafe", XS_Triceps__RowHandle_getRowSafe, file);
        newXS("Triceps::RowHandle::isInTable", XS_Triceps__RowHandle_isInTable, file);
        newXS("Triceps::RowHandle::isNull", XS_Triceps__RowHandle_isNull, file);
        newXS("Triceps::RowHandle::next", XS_Triceps__RowHandle_next, file);
        newXS("Triceps::RowHandle::nextIdx", XS_Triceps__RowHandle_nextIdx, file);
        newXS("Triceps::RowHandle::firstOfGroupIdx", XS_Triceps__RowHandle_firstOfGroupIdx, file);
        newXS("Triceps::RowHandle::nextGroupIdx", XS_Triceps__RowHandle_nextGroupIdx, file);
#if (PERL_REVISION == 5 && PERL_VERSION >= 9)
  if (PL_unitcheckav)
       call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
}

