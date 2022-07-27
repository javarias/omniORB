#include "TypeCode_s.hh"
USE_STD_NS


class Server : public _sk_TypeCodeTest::_sk_Echo {
public:
    Server(const char* name = 0) : _sk_TypeCodeTest::_sk_Echo(name) {}
    virtual ~Server() {	cout <<	"Server	dtor called" <<	endl <<	flush;}

    CORBA::TypeCode_ptr	echo_tc(CORBA_TypeCode_ptr	t) {
      cout <<	"Echoing [" << t->name() << "]" << endl;
      return CORBA::TypeCode::_duplicate(t);
    }

    CORBA::Any_ptr echo_any(const CORBA::Any& a) {
      CORBA::TypeCode_var t(a.type());
      cout <<	"Echoing [" << t->name() << "]" << endl;
      CORBA::Any_var dup = new CORBA::Any(a);
      return CORBA::Any::_duplicate(dup);
    }
};

int main(int argc, char	**argv)
{
    try	{
	CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
	CORBA::BOA_var boa = orb->BOA_init(argc, argv);

	TypeCodeTest::Echo_var my_obj =	new Server("TypeCode Test");
	boa->obj_is_ready(my_obj);

	cout <<	"Server	is ready..." <<	endl;
	boa->impl_is_ready();

    } catch (CORBA::UserException& ue) {
	cerr <<	ue << endl;
	return 1;
    } catch (CORBA::SystemException& se) {
	cerr <<	se << endl;
	return 1;
    } catch (CORBA::Exception& ee) {
	cerr <<	ee << endl;
	return 1;
    }

    return 0;
}
