#ifndef __FRAGTEST_I_H__
#define __FRAGTEST_I_H__

class FragTest_i : public POA_FragTest,
		   public PortableServer::RefCountServantBase {
public:
  FragTest_i() {}
  virtual ~FragTest_i() {}
  FragTest::Payloads* bounce(const FragTest::Payloads& arg);
  char* bounce_string(const char* arg);
  void redirect(FragTest_ptr to);

private:
  FragTest_var to_;
  omni_mutex   lock_;
};

#endif // __FRAGTEST_I_H__
