#ifndef _RK_SET_INTERFACE_H_
#define _RK_SET_INTERFACE_H_ 

char *RkIfaceGetParam(const char *iface, const char *param);

int RkIfaceSetParam(const char *iface, const char *param, const char *value);

#endif /* _RK_SET_INTERFACE_H_ */
