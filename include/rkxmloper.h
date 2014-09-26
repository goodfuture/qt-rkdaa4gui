#ifndef _RK_XML_OPER_H_
#define _RK_XML_OPER_H_ 

#include "rkxmlproc.h"
#include <qstring.h>

class rkXmlOper 
{
	public:
		rkXmlOper(const char *filename, const char *type);
		~rkXmlOper(){};
		QString getParamVal(const char *);
		QString getGroupParamVal(const char *groupId, const char *param);
		void setParamVal(const char *name, const char *value);
		void setGroupParamVal(const char *groupId, const char *param, const char *value);
		int save();
		void free();
	private:
		QString fileName; /* XML filename */
		QString rootName; /* XML Root Node's Name */
};

#endif /* _RK_XML_OPER_H_ */
