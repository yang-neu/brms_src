/*
 * This file was generated automatically by ExtUtils::ParseXS version 3.18 from the
 * contents of TableType.xs. Do not edit this file, edit TableType.xs instead.
 *
 *    ANY CHANGES MADE HERE WILL BE LOST!
 *
 */

#line 1 "TableType.xs"
//
// (C) Copyright 2011-2014 Sergey A. Babkin.
// This file is a part of Triceps.
// See the file COPYRIGHT for the copyright notice and license information
//
// The wrapper for TableType.

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include "TricepsPerl.h"
#include <type/HoldRowTypes.h>

#line 27 "TableType.c"
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

#line 169 "TableType.c"

XS_EUPXS(XS_Triceps__TableType_CLONE_SKIP); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_CLONE_SKIP)
{
    dVAR; dXSARGS;
    PERL_UNUSED_VAR(cv); /* -W */
    {
	int	RETVAL;
	dXSTARG;
#line 23 "TableType.xs"
		RETVAL = 1;
#line 181 "TableType.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__TableType_DESTROY); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_DESTROY)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	WrapTableType *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::DESTROY(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::DESTROY(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 30 "TableType.xs"
		// warn("TableType destroyed!");
		delete self;
#line 209 "TableType.c"
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Triceps__TableType_new); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_new)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "CLASS, wrt");
    {
	char *	CLASS = (char *)SvPV_nolen(ST(0))
;
	WrapTableType *	RETVAL;
	WrapRowType *	wrt;

	if( sv_isobject(ST(1)) && (SvTYPE(SvRV(ST(1))) == SVt_PVMG) ) {
		wrt = (WrapRowType *)SvIV((SV*)SvRV( ST(1) ));
		if (wrt == 0 || wrt->badMagic()) {
			croakWithMsg( "Triceps::TableType::new(): wrt has an incorrect magic for WrapRowTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::new(): wrt is not a blessed SV reference to WrapRowTypePtr" );
	}
;
#line 36 "TableType.xs"
		clearErrMsg();

		RETVAL = new WrapTableType(new TableType(wrt->get()));
#line 240 "TableType.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__TableType_print); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_print)
{
    dVAR; dXSARGS;
    if (items < 1)
       croak_xs_usage(cv,  "self, ...");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	SV *	RETVAL;
	WrapTableType *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::print(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::print(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 48 "TableType.xs"
		GEN_PRINT_METHOD(TableType)
#line 271 "TableType.c"
	PUTBACK;
	return;
    }
}


XS_EUPXS(XS_Triceps__TableType_equals); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_equals)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "self, other");
    {
	int	RETVAL;
	dXSTARG;
	WrapTableType *	self;
	WrapTableType *	other;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::equals(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::equals(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;

	if( sv_isobject(ST(1)) && (SvTYPE(SvRV(ST(1))) == SVt_PVMG) ) {
		other = (WrapTableType *)SvIV((SV*)SvRV( ST(1) ));
		if (other == 0 || other->badMagic()) {
			croakWithMsg( "Triceps::TableType::equals(): other has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::equals(): other is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 54 "TableType.xs"
		clearErrMsg();
		TableType *tbself = self->get();
		TableType *tbother = other->get();
		RETVAL = tbself->equals(tbother);
#line 314 "TableType.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__TableType_match); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_match)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "self, other");
    {
	int	RETVAL;
	dXSTARG;
	WrapTableType *	self;
	WrapTableType *	other;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::match(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::match(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;

	if( sv_isobject(ST(1)) && (SvTYPE(SvRV(ST(1))) == SVt_PVMG) ) {
		other = (WrapTableType *)SvIV((SV*)SvRV( ST(1) ));
		if (other == 0 || other->badMagic()) {
			croakWithMsg( "Triceps::TableType::match(): other has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::match(): other is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 64 "TableType.xs"
		clearErrMsg();
		TableType *tbself = self->get();
		TableType *tbother = other->get();
		RETVAL = tbself->match(tbother);
#line 357 "TableType.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__TableType_same); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_same)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "self, other");
    {
	int	RETVAL;
	dXSTARG;
	WrapTableType *	self;
	WrapTableType *	other;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::same(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::same(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;

	if( sv_isobject(ST(1)) && (SvTYPE(SvRV(ST(1))) == SVt_PVMG) ) {
		other = (WrapTableType *)SvIV((SV*)SvRV( ST(1) ));
		if (other == 0 || other->badMagic()) {
			croakWithMsg( "Triceps::TableType::same(): other has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::same(): other is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 74 "TableType.xs"
		clearErrMsg();
		TableType *tbself = self->get();
		TableType *tbother = other->get();
		RETVAL = (tbself == tbother);
#line 400 "TableType.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__TableType_addSubIndex); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_addSubIndex)
{
    dVAR; dXSARGS;
    if (items != 3)
       croak_xs_usage(cv,  "self, subname, sub");
    {
	WrapTableType *	RETVAL;
	WrapTableType *	self;
	char *	subname = (char *)SvPV_nolen(ST(1))
;
	WrapIndexType *	sub;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::addSubIndex(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::addSubIndex(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;

	if( sv_isobject(ST(2)) && (SvTYPE(SvRV(ST(2))) == SVt_PVMG) ) {
		sub = (WrapIndexType *)SvIV((SV*)SvRV( ST(2) ));
		if (sub == 0 || sub->badMagic()) {
			croakWithMsg( "Triceps::TableType::addSubIndex(): sub has an incorrect magic for WrapIndexTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::addSubIndex(): sub is not a blessed SV reference to WrapIndexTypePtr" );
	}
;
#line 86 "TableType.xs"
		static char funcName[] =  "Triceps::TableType::addSubIndex";
		// for casting of return value
		static char CLASS[] = "Triceps::TableType";

		clearErrMsg();
		TableType *tbt = self->get();

		try { do {
			if (tbt->isInitialized())
				throw Exception::f("%s: table is already initialized, can not add indexes any more", funcName);
		} while(0); } TRICEPS_CATCH_CROAK;

		IndexType *ixsub = sub->get();
		// can't just return self because it will upset the refcount
		RETVAL = new WrapTableType(tbt->addSubIndex(subname, ixsub));
#line 455 "TableType.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__TableType_findSubIndex); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_findSubIndex)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "self, subname");
    {
	WrapIndexType *	RETVAL;
	WrapTableType *	self;
	char *	subname = (char *)SvPV_nolen(ST(1))
;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::findSubIndex(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::findSubIndex(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 108 "TableType.xs"
		static char funcName[] =  "Triceps::TableType::findSubIndex";
		// for casting of return value
		static char CLASS[] = "Triceps::IndexType";

		clearErrMsg();
		TableType *tbt = self->get();
		IndexType *ixsub = tbt->findSubIndex(subname);
		try { do {
			if (ixsub == NULL) 
				throw Exception::f("%s: unknown nested index '%s'", funcName, subname);
		} while(0); } TRICEPS_CATCH_CROAK;
		RETVAL = new WrapIndexType(ixsub);
#line 497 "TableType.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__TableType_findSubIndexSafe); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_findSubIndexSafe)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "self, subname");
    {
	WrapIndexType *	RETVAL;
	WrapTableType *	self;
	char *	subname = (char *)SvPV_nolen(ST(1))
;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::findSubIndexSafe(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::findSubIndexSafe(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 127 "TableType.xs"
		static char funcName[] =  "Triceps::TableType::findSubIndex";
		// for casting of return value
		static char CLASS[] = "Triceps::IndexType";

		clearErrMsg();
		TableType *tbt = self->get();
		IndexType *ixsub = tbt->findSubIndex(subname);
		if (ixsub == NULL) 
			XSRETURN_UNDEF; // not croak!
		RETVAL = new WrapIndexType(ixsub);
#line 537 "TableType.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__TableType_findSubIndexById); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_findSubIndexById)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "self, idarg");
    {
	WrapIndexType *	RETVAL;
	WrapTableType *	self;
	SV *	idarg = ST(1)
;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::findSubIndexById(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::findSubIndexById(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 144 "TableType.xs"
		static char funcName[] =  "Triceps::TableType::findSubIndexById";
		// for casting of return value
		static char CLASS[] = "Triceps::IndexType";
		RETVAL = NULL; // shut up the warning

		try { do {
			clearErrMsg();
			TableType *tbt = self->get();

			IndexType::IndexId id = parseIndexId(funcName, idarg); // may throw

			IndexType *ixsub = tbt->findSubIndexById(id);
			if (ixsub == NULL)
				throw Exception::f("%s: no nested index with type id '%s' (%d)", funcName, IndexType::indexIdString(id), id);
			RETVAL = new WrapIndexType(ixsub);
		} while(0); } TRICEPS_CATCH_CROAK;
#line 583 "TableType.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__TableType_getSubIndexes); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_getSubIndexes)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	SV *	RETVAL;
	WrapTableType *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::getSubIndexes(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::getSubIndexes(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 167 "TableType.xs"
		// for casting of return value
		static char CLASS[] = "Triceps::IndexType";

		clearErrMsg();
		TableType *tbt = self->get();

		const IndexTypeVec &nested = tbt->getSubIndexes();
		for (IndexTypeVec::const_iterator it = nested.begin(); it != nested.end(); ++it) {
			const IndexTypeRef &ref = *it;

			XPUSHs(sv_2mortal(newSVpvn(ref.name_.c_str(), ref.name_.size())));
			SV *sub = newSV(0);
			sv_setref_pv( sub, CLASS, (void*)(new WrapIndexType(ref.index_.get())) );
			XPUSHs(sv_2mortal(sub));
		}
#line 628 "TableType.c"
	PUTBACK;
	return;
    }
}


XS_EUPXS(XS_Triceps__TableType_getFirstLeaf); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_getFirstLeaf)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	WrapIndexType *	RETVAL;
	WrapTableType *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::getFirstLeaf(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::getFirstLeaf(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 187 "TableType.xs"
		static char funcName[] =  "Triceps::TableType::getFirstLeaf";
		// for casting of return value
		static char CLASS[] = "Triceps::IndexType";

		clearErrMsg();
		TableType *tbt = self->get();
		IndexType *leaf = tbt->getFirstLeaf();
		try { do {
			if (leaf == NULL)
				throw Exception::f("%s: table type has no indexes defined", funcName);
		} while(0); } TRICEPS_CATCH_CROAK;
		RETVAL = new WrapIndexType(leaf);
#line 667 "TableType.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__TableType_isInitialized); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_isInitialized)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	int	RETVAL;
	dXSTARG;
	WrapTableType *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::isInitialized(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::isInitialized(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 206 "TableType.xs"
		clearErrMsg();
		TableType *tbt = self->get();
		RETVAL = tbt->isInitialized();
#line 699 "TableType.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__TableType_initialize); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_initialize)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	int	RETVAL;
	dXSTARG;
	WrapTableType *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::initialize(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::initialize(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 216 "TableType.xs"
		clearErrMsg();
		TableType *tbt = self->get();
		tbt->initialize();
		Erref err = tbt->getErrors();
		RETVAL = 1;
		try { do {
			if (err->hasError())
				throw Exception(err, false);
		} while(0); } TRICEPS_CATCH_CROAK;
#line 736 "TableType.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__TableType_rowType); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_rowType)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	WrapRowType *	RETVAL;
	WrapTableType *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::rowType(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::rowType(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 232 "TableType.xs"
		// for casting of return value
		static char CLASS[] = "Triceps::RowType";

		clearErrMsg();
		TableType *tbt = self->get();
		RETVAL = new WrapRowType(const_cast<RowType *>(tbt->rowType()));
#line 769 "TableType.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__TableType_getRowType); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_getRowType)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	WrapRowType *	RETVAL;
	WrapTableType *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::getRowType(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::getRowType(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 245 "TableType.xs"
		// for casting of return value
		static char CLASS[] = "Triceps::RowType";

		clearErrMsg();
		TableType *tbt = self->get();
		RETVAL = new WrapRowType(const_cast<RowType *>(tbt->rowType()));
#line 803 "TableType.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__TableType_copy); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_copy)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	WrapTableType *	RETVAL;
	WrapTableType *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::copy(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::copy(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 258 "TableType.xs"
		// for casting of return value
		static char CLASS[] = "Triceps::TableType";

		clearErrMsg();
		TableType *tbt = self->get();
		RETVAL = new WrapTableType(tbt->copy());
#line 837 "TableType.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__TableType_deepCopy); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__TableType_deepCopy)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	WrapTableType *	RETVAL;
	WrapTableType *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapTableType *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::TableType::deepCopy(): self has an incorrect magic for WrapTableTypePtr" );
		}
	} else{
		croakWithMsg( "Triceps::TableType::deepCopy(): self is not a blessed SV reference to WrapTableTypePtr" );
	}
;
#line 271 "TableType.xs"
		// for casting of return value
		static char CLASS[] = "Triceps::TableType";

		clearErrMsg();
		TableType *tbt = self->get();
		Autoref<HoldRowTypes> holder = new HoldRowTypes;
		RETVAL = new WrapTableType(tbt->deepCopy(holder));
#line 872 "TableType.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}

#ifdef __cplusplus
extern "C"
#endif
XS_EXTERNAL(boot_Triceps__TableType); /* prototype to pass -Wmissing-prototypes */
XS_EXTERNAL(boot_Triceps__TableType)
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

        newXS("Triceps::TableType::CLONE_SKIP", XS_Triceps__TableType_CLONE_SKIP, file);
        newXS("Triceps::TableType::DESTROY", XS_Triceps__TableType_DESTROY, file);
        newXS("Triceps::TableType::new", XS_Triceps__TableType_new, file);
        newXS("Triceps::TableType::print", XS_Triceps__TableType_print, file);
        newXS("Triceps::TableType::equals", XS_Triceps__TableType_equals, file);
        newXS("Triceps::TableType::match", XS_Triceps__TableType_match, file);
        newXS("Triceps::TableType::same", XS_Triceps__TableType_same, file);
        newXS("Triceps::TableType::addSubIndex", XS_Triceps__TableType_addSubIndex, file);
        newXS("Triceps::TableType::findSubIndex", XS_Triceps__TableType_findSubIndex, file);
        newXS("Triceps::TableType::findSubIndexSafe", XS_Triceps__TableType_findSubIndexSafe, file);
        newXS("Triceps::TableType::findSubIndexById", XS_Triceps__TableType_findSubIndexById, file);
        newXS("Triceps::TableType::getSubIndexes", XS_Triceps__TableType_getSubIndexes, file);
        newXS("Triceps::TableType::getFirstLeaf", XS_Triceps__TableType_getFirstLeaf, file);
        newXS("Triceps::TableType::isInitialized", XS_Triceps__TableType_isInitialized, file);
        newXS("Triceps::TableType::initialize", XS_Triceps__TableType_initialize, file);
        newXS("Triceps::TableType::rowType", XS_Triceps__TableType_rowType, file);
        newXS("Triceps::TableType::getRowType", XS_Triceps__TableType_getRowType, file);
        newXS("Triceps::TableType::copy", XS_Triceps__TableType_copy, file);
        newXS("Triceps::TableType::deepCopy", XS_Triceps__TableType_deepCopy, file);
#if (PERL_REVISION == 5 && PERL_VERSION >= 9)
  if (PL_unitcheckav)
       call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
}

