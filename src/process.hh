/* process.hh*/
#ifndef PROCESS_HH
#define PROCESS_HH

#define VALUE_INVALID 0x0

namespace process 
{
	void await(void);
	void dispatch(void);
	void setup_threads(void);
	void mark(int, int, int);
	void invalidate(void);
}

#endif /* PROCESS_HH */
