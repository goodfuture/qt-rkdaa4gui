#include "rkxmlproc.h"

rkXmlProc context;

rkXmlNode *rkXmlLoad(const char *filename)
{
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		return NULL;
	}
	context.fileName = filename;
	context.declare = mxmlLoadFile(NULL, fp, rkXmlLoadCallBack);
	context.root = mxmlGetFirstChild(context.declare);
	fclose(fp);

	/* Disable Line Wrapping */
	mxmlSetWrapMargin(0);

	return context.root;
}

rkXmlNode *rkXmlCreateNew(const char *filename, const char *type)
{
	context.fileName = filename;
	context.declare = mxmlNewXML("1.0");
	context.root = rkXmlCreateElement(context.declare, type);
	context.rootName = type;
	return context.root;
}

rkXmlNode *rkXmlCreateElement(rkXmlNode *parent, const char *name)
{
	return mxmlNewElement(parent, name);
}

void rkXmlSetAttr(rkXmlNode *node, const char *name, const char *value)
{
	mxmlElementSetAttr(node, name, value);
}

void rkXmlCreateText(rkXmlNode *parent, const char *text)
{
	mxmlNewText(parent, 0, text);
}

int rkXmlSave()
{
	FILE *fp = fopen(context.fileName, "w+");
	if (NULL == fp) {
		return -1;
	}
	if (mxmlSaveFile(context.declare, fp, rkXmlSaveCallBack) == -1) {
		return -1;
	}
	fclose(fp);

	return 0;
}

void rkXmlFree()
{
	mxmlDelete(context.declare);
}

int rkXmlSaveAndFree()
{
	FILE *fp = fopen(context.fileName, "w+");
	if (NULL == fp) {
		return -1;
	}

	if (mxmlSaveFile(context.declare, fp, rkXmlSaveCallBack) == -1) {
		return -1;
	}

	fclose(fp);
	mxmlDelete(context.declare);

	return 0;
}

const char *rkXmlSaveCallBack(rkXmlNode *node, int where)
{
	const char *name = mxmlGetElement(node);
	static char space[16];
	int depth;
	int i;

	if (where == MXML_WS_BEFORE_OPEN) {
		bzero(space, sizeof(space));
		depth = rkXmlGetNodeDepth(node);
		for (i = 1; i < depth - 1; i++) {
			space[i - 1] = '\t';
		}
		return space;
	}

	if (where == MXML_WS_BEFORE_CLOSE && !mxmlGetText(node, 0)) {
		bzero(space, sizeof(space));
		depth = rkXmlGetNodeDepth(node);
		for (i = 1; i < depth - 1; i++) {
			space[i - 1] = '\t';
		}
		return space;
	}

	if (where == MXML_WS_AFTER_OPEN && !mxmlGetText(node, 0)) {
		return "\n";
	}

	if (where == MXML_WS_AFTER_CLOSE) {
		return "\n";
	}

	return NULL;
}

int rkXmlGetNodeDepth(rkXmlNode *node)
{
	int depth = 0;
	for(; NULL != node; node = mxmlGetParent(node)) {
		depth++;
	}

	return depth;
}

int rkXmlSetParamVal(const char *name, const char *value)
{
	rkXmlNode *node = mxmlGetFirstChild(context.root);
	rkXmlNode *text;

	/* We Will Try To Find Exist Node, And If We Success, We Just Need To Modify It */
	for(; NULL != node; node = mxmlGetNextSibling(node)) {
		if (mxmlGetType(node) == MXML_ELEMENT && !strcmp(mxmlGetElement(node), "parameter")
				&& !strcmp(mxmlElementGetAttr(node, "name"), name)) {
			text = mxmlGetFirstChild(node);
			if (text && mxmlGetType(text) == MXML_TEXT) {
				return mxmlSetText(text, 0, value);
			} else {
				rkXmlCreateText(node, value);
				return 0;
			}
		}
	}

	/* Now, We Failed To Find What We Want, We Must Create A New One! */
	node = rkXmlCreateElement(context.root, "parameter");
	rkXmlSetAttr(node, "name", name);
	rkXmlCreateText(node, value);
	return 0;
}

const char *rkXmlGetParamVal(const char *name)
{
	rkXmlNode *node = mxmlGetFirstChild(context.root);
	rkXmlNode *text;

	/* Try To Find It, And Return */
	for(; NULL != node; node = mxmlGetNextSibling(node)) {
		if (mxmlGetType(node) == MXML_ELEMENT && !strcmp(mxmlGetElement(node), "parameter")
				&& !strcmp(mxmlElementGetAttr(node, "name"), name)) {
			text = mxmlGetFirstChild(node);
			if (text && mxmlGetType(text) == MXML_TEXT) {
				return mxmlGetText(text, 0);
			}

			return NULL;
		}
	}
}

int rkXmlSetGroupParamVal(const char *groupId, const char *name, const char *value)
{
	rkXmlNode *group = mxmlGetFirstChild(context.root);
	rkXmlNode *node;
	rkXmlNode *text;
	unsigned int flag = 0; /* If We Find What We Want, We Set "flag" To NonZero */

	/* First, We Need To Find The Group Node */
	for (; NULL != group; group = mxmlGetNextSibling(group)) {
		if (mxmlGetType(group) == MXML_ELEMENT && !strcmp(mxmlGetElement(group), "group")
				&& !strcmp(mxmlElementGetAttr(group, "id"), groupId)) {
			flag = 1;
			break;
		}
	}

	/* When We Failed To Find The Group Node, We Have To Create A New One */
	if (!flag) {
		group = rkXmlCreateElement(context.root, "group");
		rkXmlSetAttr(group, "id", groupId);
	}
	flag = 0;

	/* Now We Will Try To Find The Parameter Node */
	node = mxmlGetFirstChild(group);
	for(; NULL != node; node = mxmlGetNextSibling(node)) {
		if (mxmlGetType(node) == MXML_ELEMENT && !strcmp(mxmlGetElement(node), "parameter")
				&& !strcmp(mxmlElementGetAttr(node, "name"), name)) {
			flag = 1;
			break;
		}
	}

	/* The Same To Above */
	if (!flag) {
		node = rkXmlCreateElement(group, "parameter");
		rkXmlSetAttr(node, "name", name);
	}

	/* Get Or Create Text */
	text = mxmlGetFirstChild(node);
	if (text && mxmlGetType(text) == MXML_TEXT) {
		return mxmlSetText(text, 0, value);
	} else {
		rkXmlCreateText(node, value);
		return 0;
	}
}

const char *rkXmlGetGroupParamVal(const char *groupId, const char *name)
{
	rkXmlNode *group = mxmlGetFirstChild(context.root);
	rkXmlNode *node;
	rkXmlNode *text;
	unsigned int flag = 0; 

	/* First, We Need To Find The Group Node */
	for (; NULL != group; group = mxmlGetNextSibling(group)) {
		if (mxmlGetType(group) == MXML_ELEMENT && !strcmp(mxmlGetElement(group), "group")
				&& !strcmp(mxmlElementGetAttr(group, "id"), groupId)) {
			flag = 1;
			break;
		}
	}

	/* When We Failed To Find The Group Node, We Just Return NULL */
	if (!flag) {
		return NULL;
	}
	flag = 0;

	/* Now We Will Try To Find The Parameter Node */
	node = mxmlGetFirstChild(group);
	for(; NULL != node; node = mxmlGetNextSibling(node)) {
		if (mxmlGetType(node) == MXML_ELEMENT && !strcmp(mxmlGetElement(node), "parameter")
				&& !strcmp(mxmlElementGetAttr(node, "name"), name)) {
			flag = 1;
			break;
		}
	}

	/* The Same To Above */
	if (!flag) {
		return NULL;
	}

	/* Get Text */
	text = mxmlGetFirstChild(node);
	if (text && mxmlGetType(text) == MXML_TEXT) {
		return mxmlGetText(text, 0);
	}

	return NULL;
}

rkXmlType rkXmlLoadCallBack(rkXmlNode *node)
{
	const char *name = mxmlGetElement(node);
	int depth = rkXmlGetNodeDepth(node);

	if (depth == 1 || depth == 2) {
		return MXML_ELEMENT;
	}

	if (!strcmp(name, "group")) {
		return MXML_ELEMENT;
	}

	if (!strcmp(name, "parameter")) {
		return MXML_TEXT;
	}

	return MXML_ELEMENT;
}
