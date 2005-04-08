#ifndef __OMNITEST_EXTRAS_H__
#define __OMNITEST_EXTRAS_H__

namespace omniORB {

  extern CORBA::ULong   traceLevel;

  inline int trace(CORBA::ULong tl) { return traceLevel >= tl; }

  class logger {
  public:
    logger(const char* prefix = 0);  // prefix defaults to "omniORB: "
    ~logger();
    // The destructor flushes the message.

    logger& operator<<(char c);
    logger& operator<<(unsigned char c) { return (*this) << (char)c; }
    logger& operator<<(signed char c) { return (*this) << (char)c; }
    logger& operator<<(const char *s);
    logger& operator<<(const unsigned char *s) {
      return (*this) << (const char*)s;
    }
    logger& operator<<(const signed char *s) {
      return (*this) << (const char*)s;
    }
    logger& operator<<(const void *p);
    logger& operator<<(int n);
    logger& operator<<(unsigned int n);
    logger& operator<<(long n);
    logger& operator<<(unsigned long n);
    logger& operator<<(short n) {return operator<<((int)n);}
    logger& operator<<(unsigned short n) {return operator<<((unsigned int)n);}
#ifdef HAS_Cplusplus_Bool
    logger& operator<<(bool b) { return operator<<((int)b); }
#endif
#ifndef NO_FLOAT
    logger& operator<<(double n);
    logger& operator<<(float n) { return operator<<((double)n); }
#endif

    logger& operator<<(const CORBA::SystemException&);

    class exceptionStatus {
    public:
      exceptionStatus(CORBA::CompletionStatus s, CORBA::ULong m) :
	status(s), minor(m), minor_string(0) {}

      exceptionStatus(CORBA::CompletionStatus s, const char* description) :
	status(s), minor(0), minor_string(description) {}

      CORBA::CompletionStatus status;
      CORBA::ULong            minor;
      const char*             minor_string;
    private:
      exceptionStatus();
    };

    logger& operator<<(const exceptionStatus&);

    void flush();
    // Flushes the logger -- it can then be re-used for another
    // message.

  private:
    logger(const logger&);
    logger& operator=(const logger&);

    inline void reserve(int n) { if( pd_end - pd_p - 1 < n )  more(n); }
    void more(int n);

    const char* pd_prefix;
    char*       pd_buf;
    char*       pd_p;      // assert(*pd_p == '\0')
    char*       pd_end;    // assert(pd_p < pd_end)
  };


  void logf(const char* fmt ...);
  // Writes log message with prefix, and appends '\n'.

  void do_logs(const char* msg);
  // internal

  inline void logs(CORBA::ULong tl, const char* msg) {
    if( traceLevel >= tl ) do_logs(msg);
  }
  // Writes log message with prefix, and appends '\n'.


  ////////////////////////////////////////////////////////////////////////
  // class fatalException                                               //
  //                                                                    //
  // This exception is thrown if a bug inside the omniORB2 runtime is   //
  // detected. The exact location in the source where the exception is  //
  // thrown is indicated by file() and line().                          //
  //                                                                    //
  class fatalException {                                                //
  public:                                                               //
    fatalException(const char *file,int line,const char *errmsg);       //
    inline ~fatalException() {}                                         //
    inline const char *file() const   { return pd_file;   }             //
    inline int line() const           { return pd_line;   }             //
    inline const char *errmsg() const { return pd_errmsg; }             //
  private:                                                              //
    const char *pd_file;                                                //
    int         pd_line;                                                //
    const char *pd_errmsg;                                              //
                                                                        //
    fatalException();                                                   //
  };                                                                    //
  ////////////////////////////////////////////////////////////////////////

};

#endif
