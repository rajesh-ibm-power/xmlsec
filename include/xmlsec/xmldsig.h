/** 
 * XMLSec library
 *
 * "XML Digital Signature" implementation
 *  http://www.w3.org/TR/xmldsig-core/
 *  http://www.w3.org/Signature/Overview.html
 * 
 * See Copyright for the status of this software.
 * 
 * Author: Aleksey Sanin <aleksey@aleksey.com>
 */
#ifndef __XMLSEC_XMLDSIG_H__
#define __XMLSEC_XMLDSIG_H__    

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */ 
#include <libxml/tree.h>
#include <libxml/parser.h> 

#include <xmlsec/xmlsec.h>
#include <xmlsec/list.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/keyinfo.h>

typedef struct _xmlSecDSigReferenceCtx		xmlSecDSigReferenceCtx,
						*xmlSecDSigReferenceCtxPtr;

typedef enum {
    xmlDSigStatusUnknown = 0,
    xmlDSigStatusSucceeded,
    xmlDSigStatusInvalid,
    xmlDSigStatusFailed
} xmlDSigStatus;

/**************************************************************************
 *
 * xmlSecDSigCtx
 *
 *************************************************************************/

/**
 * xmlSecDSigCtx:
 * @processManifests: if 0 then <dsig:Manifests> nodes are not processed.
 * @storeSignatures: store the signed content just (<dsig:SignedInfo> element)
 *	before applying signature.
 * @storeReferences: store the result of processing <dsig:Reference> nodes in 
 *      <dsig:SignedInfo> nodes just before digesting.
 * @storeManifests: store the result of processing <dsig:Reference> nodes in 
 *	<dsig:Manifest> nodes just before digesting (ignored if @processManifest is 0).
 * @fakeSignatures: for performance testing only.
 *
 * XML DSig context. 
 */
struct _xmlSecDSigCtx {
    /* these data user can set before performing the operation */
    void*			userData;
    xmlSecKeyInfoCtx		keyInfoReadCtx;
    xmlSecKeyInfoCtx		keyInfoWriteCtx;
    xmlSecTransformCtx		signTransformCtx;
    xmlSecTransformPtr		signMethod;
    xmlSecTransformPtr		c14nMethod;
    xmlSecKeyPtr		signKey;
    int				processManifests;
    int				storeSignatures;
    int				storeReferences;
    int				storeManifests;	
    int				fakeSignatures;
    xmlSecTransformId		defaultDigestTransformId;
    xmlSecTransformId		defaultC14NTransformId;
    
    /* these data are returned */
    int				sign;
    xmlSecBufferPtr		result;
    xmlDSigStatus		status;
    xmlSecTransformPtr		preSignMemBufMethod;
    xmlChar*			id;    
    xmlSecPtrList    		references;
    xmlSecPtrList		manifests;
        
    /* these are internal data, nobody should change that except us */
    int				dontDestroySignMethod;
    int				dontDestroyC14NMethod;

    /* reserved for future */
    void*			reserved0;
    void*			reserved1;    
};						


XMLSEC_EXPORT xmlSecDSigCtxPtr	xmlSecDSigCtxCreate		(xmlSecKeysMngrPtr keysMngr);
XMLSEC_EXPORT void 		xmlSecDSigCtxDestroy		(xmlSecDSigCtxPtr ctx);
XMLSEC_EXPORT int		xmlSecDSigCtxInitialize		(xmlSecDSigCtxPtr ctx,
								 xmlSecKeysMngrPtr keysMngr);
XMLSEC_EXPORT void		xmlSecDSigCtxFinalize		(xmlSecDSigCtxPtr ctx);
XMLSEC_EXPORT int		xmlSecDSigCtxSign		(xmlSecDSigCtxPtr ctx,
								 xmlNodePtr tmpl);
XMLSEC_EXPORT int		xmlSecDSigCtxVerify		(xmlSecDSigCtxPtr ctx,
								 xmlNodePtr node);
XMLSEC_EXPORT void		xmlSecDSigCtxDebugDump		(xmlSecDSigCtxPtr ctx,
								 FILE* output);
XMLSEC_EXPORT void		xmlSecDSigCtxDebugXmlDump	(xmlSecDSigCtxPtr ctx,
								 FILE* output);


/**************************************************************************
 *
 * xmlSecDSigReferenceCtx
 *
 *************************************************************************/
/**
 * xmlSecDSigReferenceOrigin:
 * @xmlSecDSigReferenceOriginSignedInfo: reference in <dsig:SignedInfo> node.
 * @xmlSecDSigReferenceOriginManifest: reference <dsig:Manifest> node.
 * 
 * The possible <dsig:Reference> node locations: 
 * in the <dsig:SignedInfo> node or in the <dsig:Manifest> node.
 */
typedef enum  {
    xmlSecDSigReferenceOriginSignedInfo,
    xmlSecDSigReferenceOriginManifest
} xmlSecDSigReferenceOrigin;

struct _xmlSecDSigReferenceCtx {
    xmlSecDSigCtxPtr		signCtx;
    xmlSecDSigReferenceOrigin	origin;
    xmlSecTransformPtr		digestMethod;
    xmlSecTransformPtr		c14nMethod;

    int				sign;
    xmlSecBufferPtr		result;
    xmlDSigStatus		status;
    xmlSecTransformPtr		preDigestMemBufMethod;
    xmlChar*			id;
    xmlChar*			uri;
    xmlChar*			type;
    
    /* these are internal data, nobody should change that except us */
    int				dontDestroyDigestMethod;
    int				dontDestroyC14NMethod;
    
    /* reserved for future */
    void*			reserved0;
    void*			reserved1;    
};

/**************************************************************************
 *
 * xmlSecDSigReferenceCtxListKlass
 *
 *************************************************************************/
#define xmlSecDSigReferenceCtxListId \
	xmlSecDSigReferenceCtxListGetKlass()
XMLSEC_EXPORT xmlSecPtrListId	xmlSecDSigReferenceCtxListGetKlass(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_XMLDSIG_H__ */

