#ifndef _RK_IP_VALIDATOR_H_
#define _RK_IP_VALIDATOR_H_ 

#include <qvalidator.h>

class rkFltIp : public QValidator
{
	public:
		rkFltIp(QObject *parent):QValidator(parent){};
		~rkFltIp(){};
		QValidator::State validate(QString &, int &pos) const;
};

#endif /* _RK_IP_VALIDATOR_H_ */
