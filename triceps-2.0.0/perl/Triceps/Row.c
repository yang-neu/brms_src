/*
 * This file was generated automatically by ExtUtils::ParseXS version 3.18 from the
 * contents of Row.xs. Do not edit this file, edit Row.xs instead.
 *
 *    ANY CHANGES MADE HERE WILL BE LOST!
 *
 */

#line 1 "Row.xs"
//
// (C) Copyright 2011-2014 Sergey A. Babkin.
// This file is a part of Triceps.
// See the file COPYRIGHT for the copyright notice and license information
//
// The wrapper for Row.

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include "TricepsPerl.h"

#line 26 "Row.c"
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

#line 168 "Row.c"

XS_EUPXS(XS_Triceps__Row_CLONE_SKIP); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__Row_CLONE_SKIP)
{
    dVAR; dXSARGS;
    PERL_UNUSED_VAR(cv); /* -W */
    {
	int	RETVAL;
	dXSTARG;
#line 25 "Row.xs"
		RETVAL = 1;
#line 180 "Row.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__Row_DESTROY); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__Row_DESTROY)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	WrapRow *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRow *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::Row::DESTROY(): self has an incorrect magic for WrapRowPtr" );
		}
	} else{
		croakWithMsg( "Triceps::Row::DESTROY(): self is not a blessed SV reference to WrapRowPtr" );
	}
;
#line 32 "Row.xs"
		// warn("Row destroyed!");
		delete self;
#line 208 "Row.c"
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Triceps__Row_hexdump); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__Row_hexdump)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	char *	RETVAL;
	dXSTARG;
	WrapRow *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRow *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::Row::hexdump(): self has an incorrect magic for WrapRowPtr" );
		}
	} else{
		croakWithMsg( "Triceps::Row::hexdump(): self is not a blessed SV reference to WrapRowPtr" );
	}
;
#line 39 "Row.xs"
		clearErrMsg();
		string dump;
		const RowType *t = self->ref_.getType();
		Row *r = self->ref_.get();
		t->hexdumpRow(dump, r);
		RETVAL = (char *)dump.c_str();
#line 241 "Row.c"
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__Row_toHash); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__Row_toHash)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	SV *	RETVAL;
	WrapRow *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRow *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::Row::toHash(): self has an incorrect magic for WrapRowPtr" );
		}
	} else{
		croakWithMsg( "Triceps::Row::toHash(): self is not a blessed SV reference to WrapRowPtr" );
	}
;
#line 52 "Row.xs"
		clearErrMsg();
		const RowType *t = self->ref_.getType();
		Row *r = self->ref_.get();
		const RowType::FieldVec &fld = t->fields();
		int nf = fld.size();

		for (int i = 0; i < nf; i++) {
			XPUSHs(sv_2mortal(newSVpvn(fld[i].name_.c_str(), fld[i].name_.size())));

			const char *data;
			intptr_t dlen;
			bool notNull = t->getField(r, i, data, dlen);
			XPUSHs(sv_2mortal(bytesToVal(fld[i].type_->getTypeId(), fld[i].arsz_, notNull, data, dlen, fld[i].name_.c_str())));
		}
#line 284 "Row.c"
	PUTBACK;
	return;
    }
}


XS_EUPXS(XS_Triceps__Row_toArray); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__Row_toArray)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	SV *	RETVAL;
	WrapRow *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRow *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::Row::toArray(): self has an incorrect magic for WrapRowPtr" );
		}
	} else{
		croakWithMsg( "Triceps::Row::toArray(): self is not a blessed SV reference to WrapRowPtr" );
	}
;
#line 71 "Row.xs"
		clearErrMsg();
		const RowType *t = self->ref_.getType();
		Row *r = self->ref_.get();
		const RowType::FieldVec &fld = t->fields();
		int nf = fld.size();

		for (int i = 0; i < nf; i++) {
			const char *data;
			intptr_t dlen;
			bool notNull = t->getField(r, i, data, dlen);
			XPUSHs(sv_2mortal(bytesToVal(fld[i].type_->getTypeId(), fld[i].arsz_, notNull, data, dlen, fld[i].name_.c_str())));
		}
#line 325 "Row.c"
	PUTBACK;
	return;
    }
}


XS_EUPXS(XS_Triceps__Row_copymod); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__Row_copymod)
{
    dVAR; dXSARGS;
    if (items < 1)
       croak_xs_usage(cv,  "self, ...");
    {
	WrapRow *	RETVAL;
	WrapRow *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRow *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::Row::copymod(): self has an incorrect magic for WrapRowPtr" );
		}
	} else{
		croakWithMsg( "Triceps::Row::copymod(): self is not a blessed SV reference to WrapRowPtr" );
	}
;
#line 88 "Row.xs"
		static char funcName[] =  "Triceps::Row::copymod";
		// for casting of return value
		static char CLASS[] = "Triceps::Row";
		RETVAL = NULL; // shut up the warning
		try { do {
			clearErrMsg();
			const RowType *rt = self->ref_.getType();
			Row *r = self->ref_.get();

			// The arguments come in pairs fieldName => value;
			// the value may be either a simple value that will be
			// cast to the right type, or a reference to a list of values.
			// The uint8 and string are converted from Perl strings
			// (the difference for now is that string is 0-terminated)
			// and can not have lists.

			if (items % 2 != 1) {
				throw Exception::f("Usage: %s(RowType, [fieldName, fieldValue, ...]), names and types must go in pairs", funcName);
			}

			// parse data to create a copy
			FdataVec fields;
			rt->splitInto(r, fields);

			// now override the modified fields
			// this code is copied from RowType::makerow_hs
			vector<Autoref<EasyBuffer> > bufs;
			for (int i = 1; i < items; i += 2) {
				const char *fname = (const char *)SvPV_nolen(ST(i));
				int idx  = rt->findIdx(fname);
				if (idx < 0) {
					throw Exception::f("%s: attempting to set an unknown field '%s'", funcName, fname);
				}
				const RowType::Field &finfo = rt->fields()[idx];

				if (!SvOK(ST(i+1))) { // undef translates to null
					fields[idx].setNull();
				} else {
					if (SvROK(ST(i+1)) && finfo.arsz_ < 0) {
						throw Exception::f("%s: attempting to set an array into scalar field '%s'", funcName, fname);
					}
					EasyBuffer *d = valToBuf(finfo.type_->getTypeId(), ST(i+1), fname); // may throw
					bufs.push_back(d); // remember for cleaning

					fields[idx].setPtr(true, d->data_, d->size_);
				}
			}
			RETVAL = new WrapRow(rt, rt->makeRow(fields));
		} while(0); } TRICEPS_CATCH_CROAK;
#line 401 "Row.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__Row_get); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__Row_get)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "self, fname");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	SV *	RETVAL;
	WrapRow *	self;
	char *	fname = (char *)SvPV_nolen(ST(1))
;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRow *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::Row::get(): self has an incorrect magic for WrapRowPtr" );
		}
	} else{
		croakWithMsg( "Triceps::Row::get(): self is not a blessed SV reference to WrapRowPtr" );
	}
;
#line 144 "Row.xs"
		static char funcName[] =  "Triceps::Row::get";
		try { do {
			clearErrMsg();
			const RowType *t = self->ref_.getType();
			Row *r = self->ref_.get();
			const RowType::FieldVec &fld = t->fields();

			int i = t->findIdx(fname);
			if ( i < 0 )
				throw Exception::f("%s: unknown field '%s'", funcName, fname);

			const char *data;
			intptr_t dlen;
			bool notNull = t->getField(r, i, data, dlen);
			XPUSHs(sv_2mortal(bytesToVal(fld[i].type_->getTypeId(), fld[i].arsz_, notNull, data, dlen, fld[i].name_.c_str())));
		} while(0); } TRICEPS_CATCH_CROAK;
#line 449 "Row.c"
	PUTBACK;
	return;
    }
}


XS_EUPXS(XS_Triceps__Row_getType); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__Row_getType)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	WrapRowType *	RETVAL;
	WrapRow *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRow *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::Row::getType(): self has an incorrect magic for WrapRowPtr" );
		}
	} else{
		croakWithMsg( "Triceps::Row::getType(): self is not a blessed SV reference to WrapRowPtr" );
	}
;
#line 165 "Row.xs"
		clearErrMsg();
		const RowType *t = self->ref_.getType();

		// for casting of return value
		static char CLASS[] = "Triceps::RowType";
		RETVAL = new WrapRowType(const_cast<RowType *>(t));
#line 482 "Row.c"
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__Row_same); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__Row_same)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "self, other");
    {
	int	RETVAL;
	dXSTARG;
	WrapRow *	self;
	WrapRow *	other;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRow *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::Row::same(): self has an incorrect magic for WrapRowPtr" );
		}
	} else{
		croakWithMsg( "Triceps::Row::same(): self is not a blessed SV reference to WrapRowPtr" );
	}
;

	if( sv_isobject(ST(1)) && (SvTYPE(SvRV(ST(1))) == SVt_PVMG) ) {
		other = (WrapRow *)SvIV((SV*)SvRV( ST(1) ));
		if (other == 0 || other->badMagic()) {
			croakWithMsg( "Triceps::Row::same(): other has an incorrect magic for WrapRowPtr" );
		}
	} else{
		croakWithMsg( "Triceps::Row::same(): other is not a blessed SV reference to WrapRowPtr" );
	}
;
#line 178 "Row.xs"
		clearErrMsg();
		Row *r1 = self->get();
		Row *r2 = other->get();
		RETVAL = (r1 == r2);
#line 526 "Row.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Triceps__Row_isEmpty); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Triceps__Row_isEmpty)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "self");
    {
	int	RETVAL;
	dXSTARG;
	WrapRow *	self;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) ) {
		self = (WrapRow *)SvIV((SV*)SvRV( ST(0) ));
		if (self == 0 || self->badMagic()) {
			croakWithMsg( "Triceps::Row::isEmpty(): self has an incorrect magic for WrapRowPtr" );
		}
	} else{
		croakWithMsg( "Triceps::Row::isEmpty(): self is not a blessed SV reference to WrapRowPtr" );
	}
;
#line 189 "Row.xs"
		clearErrMsg();
		RETVAL = self->ref_.getType()->isRowEmpty(self->ref_.get())? 1 : 0;
#line 556 "Row.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}

#ifdef __cplusplus
extern "C"
#endif
XS_EXTERNAL(boot_Triceps__Row); /* prototype to pass -Wmissing-prototypes */
XS_EXTERNAL(boot_Triceps__Row)
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

        newXS("Triceps::Row::CLONE_SKIP", XS_Triceps__Row_CLONE_SKIP, file);
        newXS("Triceps::Row::DESTROY", XS_Triceps__Row_DESTROY, file);
        newXS("Triceps::Row::hexdump", XS_Triceps__Row_hexdump, file);
        newXS("Triceps::Row::toHash", XS_Triceps__Row_toHash, file);
        newXS("Triceps::Row::toArray", XS_Triceps__Row_toArray, file);
        newXS("Triceps::Row::copymod", XS_Triceps__Row_copymod, file);
        newXS("Triceps::Row::get", XS_Triceps__Row_get, file);
        newXS("Triceps::Row::getType", XS_Triceps__Row_getType, file);
        newXS("Triceps::Row::same", XS_Triceps__Row_same, file);
        newXS("Triceps::Row::isEmpty", XS_Triceps__Row_isEmpty, file);

    /* Initialisation Section */

#line 20 "Row.xs"
// fprintf(stderr, "DEBUG Row items=%d sp=%p mark=%p\n", items, sp, mark);

#line 598 "Row.c"

    /* End of Initialisation Section */

#if (PERL_REVISION == 5 && PERL_VERSION >= 9)
  if (PL_unitcheckav)
       call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
}

