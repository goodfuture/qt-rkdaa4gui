#include "rktype.h"
#include "rkxmloper.h"
#include <qmessagebox.h>

extern struct context ctx;
rkXmlOper::rkXmlOper(const char *filename, const char *type)
{
	/* Set Global Variable */
	fileName = ctx.cliarg[ARG_CFG_DIR_OFFSET];
	if (fileName.length() == 0) {
		fileName = filename;
	} else if (fileName.endsWith("/")) {
		fileName += filename;
	} else {
		fileName += "/";
		fileName += filename;
	}
	rootName = type;

	/* Initialize Working */
	rkXmlNode *node = rkXmlLoad(fileName);
	if (!node) {
		QMessageBox::critical(0, "错误", QString("读取配置文件\"%1\"失败!").arg(fileName));
		node = rkXmlCreateNew(fileName.data(), type);
	}
}

QString rkXmlOper::getParamVal(const char *name)
{
	if (NULL == name) {
		return QString::null;
	}

	return rkXmlGetParamVal(name);
}

QString rkXmlOper::getGroupParamVal(const char *groupId, const char *param)
{
	if (NULL == groupId || NULL == param) {
		return QString::null;
	}

	return rkXmlGetGroupParamVal(groupId, param);
}

void rkXmlOper::setParamVal(const char *name, const char *value)
{
	if (NULL == name) {
		return;
	}

	if (rkXmlSetParamVal(name, value) == -1) {
		QMessageBox::critical(0, "错误", QString("参数\"%1\"设置失败!").arg(name));
	}
}

void rkXmlOper::setGroupParamVal(const char *groupId, const char *param, const char *value)
{
	if (NULL == groupId || NULL == param) {
		return;
	}

	if (rkXmlSetGroupParamVal(groupId, param, value) == -1) {
		QMessageBox::critical(0, "错误", QString("参数\"%1.%2\"设置失败!").arg(groupId, param));
	}
}

int rkXmlOper::save()
{
	return rkXmlSaveAndFree();
}

void rkXmlOper::free()
{
	rkXmlFree();
}
