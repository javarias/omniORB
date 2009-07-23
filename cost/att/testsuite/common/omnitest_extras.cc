#include CORBA_HEADER

CORBA::ULong omniORB::traceLevel = 1;

//////////////////////////////////////////////////////////////////////
/////////////////////////// omniORB::logger //////////////////////////
//////////////////////////////////////////////////////////////////////

#ifdef INIT_BUF_SIZE
#undef INIT_BUF_SIZE
#endif
#define INIT_BUF_SIZE  256

#define PREFIX           ""


omniORB::logger::logger(const char* prefix)
  : pd_prefix(prefix), pd_buf(new char[INIT_BUF_SIZE])
{
  if( !pd_prefix )  pd_prefix = PREFIX;

  strcpy(pd_buf, pd_prefix);
  pd_p = pd_buf + strlen(pd_prefix);
  pd_end = pd_buf + INIT_BUF_SIZE;
}


omniORB::logger::~logger()
{
  if( (size_t)(pd_p - pd_buf) != strlen(pd_prefix) )
    fprintf(stderr, "%s", pd_buf);

  delete[] pd_buf;
}


omniORB::logger& 
omniORB::logger::operator<<(char c)
{
  reserve(1);
  *pd_p++ = c;
  *pd_p = '\0';
  return *this;
}


omniORB::logger&
omniORB::logger::operator<<(const char *s)
{
  size_t len = strlen(s);
  reserve(len);
  strcpy(pd_p, s);
  pd_p += len;
  return *this;
}


omniORB::logger&
omniORB::logger::operator<<(const void *p)
{
  reserve(30); // guess!
  sprintf(pd_p, "%p", p);
  pd_p += strlen(pd_p);
  return *this;
}


omniORB::logger&
omniORB::logger::operator<<(int n)
{
  reserve(20);
  sprintf(pd_p, "%d", n);
  pd_p += strlen(pd_p);
  return *this;
}


omniORB::logger&
omniORB::logger::operator<<(unsigned int n)
{
  reserve(20);
  sprintf(pd_p, "%u", n);
  pd_p += strlen(pd_p);
  return *this;
}


omniORB::logger&
omniORB::logger::operator<<(long n)
{
  reserve(30);
  sprintf(pd_p, "%ld", n);
  pd_p += strlen(pd_p);
  return *this;
}


omniORB::logger&
omniORB::logger::operator<<(unsigned long n)
{
  reserve(30);
  sprintf(pd_p, "%lu", n);
  pd_p += strlen(pd_p);
  return *this;
}


#ifndef NO_FLOAT
omniORB::logger&
omniORB::logger::operator<<(double n)
{
  reserve(30);
  sprintf(pd_p, "%g", n);
  pd_p += strlen(pd_p);
  return *this;
}
#endif


omniORB::logger&
omniORB::logger::operator<<(const omniORB::logger::exceptionStatus& ex)
{
  switch (ex.status) {
  case CORBA::COMPLETED_YES:
    *this << "YES,";
    break;
  case CORBA::COMPLETED_NO:
    *this << "NO,";
    break;
  case CORBA::COMPLETED_MAYBE:
    *this << "MAYBE,";
    break;
  }
  if (ex.minor_string) {
    *this << ex.minor_string;
  }
  else {
    reserve(30);
    sprintf(pd_p, "0x%08x", (int)ex.minor);
    pd_p += strlen(pd_p);
  }
  return *this;
}


omniORB::logger&
omniORB::logger::operator<<(const CORBA::SystemException& ex)
{
  int sz;

#ifdef OB_INTEGER_VERSION
  const char* repoid = ex._rep_id();
  sz = strlen(repoid);
  *this << repoid;
#else
  *this << ex._NP_repoId(&sz);
#endif
  switch (ex.completed()) {
  case CORBA::COMPLETED_YES:
    *this << ",YES,";
    break;
  case CORBA::COMPLETED_NO:
    *this << ",NO,";
    break;
  case CORBA::COMPLETED_MAYBE:
    *this << ",MAYBE,";
    break;
  }
  reserve(30);
  sprintf(pd_p, "0x%08x", (int)ex.minor());
  pd_p += strlen(pd_p);
  return *this;
}

void
omniORB::logger::flush()
{
  if( (size_t)(pd_p - pd_buf) != strlen(pd_prefix) )
    fprintf(stderr, "%s", pd_buf);

  pd_p = pd_buf + strlen(pd_prefix);
  *pd_p = '\0';
}


void
omniORB::logger::more(int n)
{
  int used = pd_p - pd_buf + 1;
  int size = pd_end - pd_buf;

  while( size - used < n )  size *= 2;

  char* newbuf = new char[size];
  strcpy(newbuf, pd_buf);
  char* newp = newbuf + (used - 1);
  delete[] pd_buf;
  pd_buf = newbuf;
  pd_p = newp;
  pd_end = pd_buf + size;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#ifdef INLINE_BUF_SIZE
#undef INLINE_BUF_SIZE
#endif
#define INLINE_BUF_SIZE  256

#ifndef __CIAO__
void
omniORB::logf(const char* fmt ...)
{
  char inlinebuf[INLINE_BUF_SIZE];
  char* buf = inlinebuf;
  size_t fmtlen = strlen(fmt) + sizeof(PREFIX) + 1;

  if( fmtlen > INLINE_BUF_SIZE )  buf = new char[fmtlen];

  strcpy(buf, PREFIX);
  strcpy(buf + sizeof(PREFIX) - 1, fmt);
  strcat(buf, "\n");

  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, buf, args);
  va_end(args);

  if( buf != inlinebuf )  delete[] buf;
}
#endif


void
omniORB::do_logs(const char* fmt)
{
  char inlinebuf[INLINE_BUF_SIZE];
  char* buf = inlinebuf;
  size_t fmtlen = strlen(fmt) + sizeof(PREFIX) + 1;

  if( fmtlen > INLINE_BUF_SIZE )  buf = new char[fmtlen];

  strcpy(buf, PREFIX);
  strcpy(buf + sizeof(PREFIX) - 1, fmt);
  strcat(buf, "\n");

  fprintf(stderr, "%s", buf);

  if( buf != inlinebuf )  delete[] buf;
}

