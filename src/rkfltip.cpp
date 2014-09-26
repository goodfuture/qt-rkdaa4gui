#include "rkfltip.h"

QValidator::State rkFltIp::validate ( QString & input, int &pos ) const
{
	Q_UNUSED(pos);

	const char *p = input.data();
	for(unsigned int i = 0; i < input.length(); i++) {
		if ((p[i] >= '0' && p[i] <= '9') || p[i] == '.') {
			continue;
		} else {
			return QValidator::Invalid;
		}
	}

	int count = input.contains(".", false);
	if ((count == 0 && input.toInt() > 255) || count > 3 || input.at(0) == '.') {
		return QValidator::Invalid;
	} 
	return QValidator::Acceptable;
}
