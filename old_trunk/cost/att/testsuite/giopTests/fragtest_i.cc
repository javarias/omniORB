#include <iostream.h>
#include <common/omnitest.h>
#include <fragtest.hh>
#include <fragtest_i.h>

FragTest::Payloads*
FragTest_i::bounce(const FragTest::Payloads& arg) {

  FragTest_var to;
  {
    omni_mutex_lock sync(lock_);
    to = FragTest::_duplicate(to_);
  }

  if (CORBA::is_nil(to)) {
    FragTest::Payloads_var result;
    result = new FragTest::Payloads(arg);
    return result._retn();
  }
  else {
    return to->bounce(arg);
  }
}

char*
FragTest_i::bounce_string(const char* arg) {

  FragTest_var to;
  {
    omni_mutex_lock sync(lock_);
    to = FragTest::_duplicate(to_);
  }

  if (CORBA::is_nil(to)) {
    CORBA::String_var result;
    result = arg;
    return result._retn();
  }
  else {
    return to->bounce_string(arg);
  }
}

void
FragTest_i::redirect(FragTest_ptr to) {
  omni_mutex_lock sync(lock_);
  to_ = FragTest::_duplicate(to);
}

