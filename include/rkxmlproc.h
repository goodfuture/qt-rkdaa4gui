#ifndef _RK_XML_PROCESS_H_
#define _RK_XML_PROCESS_H_

#include <mxml.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef mxml_node_t rkXmlNode;
typedef mxml_type_t rkXmlType;

typedef struct rk_xml_process {
	rkXmlNode *declare;
	rkXmlNode *root;
	const char *rootName;
	const char *fileName;
} rkXmlProc;

/* Set An Parameter Value */
int rkXmlSetParamVal(const char *name, const char *value);

/* Get An Parameter Value */
const char *rkXmlGetParamVal(const char *name);

/* Set An Group Parameter Value */
int rkXmlSetGroupParamVal(const char *groupId, const char *name, const char *value);

/* Get An Group Parameter Value */
const char *rkXmlGetGroupParamVal(const char *groupId, const char *name);

/* Create New Xml Document Model */
rkXmlNode *rkXmlCreateNew(const char *filename, const char *type);

/* Create A New Element */
rkXmlNode *rkXmlCreateElement(rkXmlNode *parent, const char *name);

/* Set An Attribute */
void rkXmlSetAttr(rkXmlNode *node, const char *name, const char *value);

/* Create A New Text Node */
void rkXmlCreateText(rkXmlNode *parent, const char *text);

/* Load From File To Memory */
rkXmlNode *rkXmlLoad(const char *filename);

/* Save From Memory To File */
int rkXmlSave();

/* Free From Memory */
void rkXmlFree();

/* Save From Memory To File And Free It From Memory */
int rkXmlSaveAndFree();

/* Get A Node's Depth In The Dom Tree */
int rkXmlGetNodeDepth(rkXmlNode *node);

/* The Save Call Back Function */
const char *rkXmlSaveCallBack(rkXmlNode *node, int where);

/* The Load Call Back Function */
rkXmlType rkXmlLoadCallBack(rkXmlNode *node);

#ifdef __cplusplus
}
#endif

#endif /* _RK_XML_PROCESS_H_ */
