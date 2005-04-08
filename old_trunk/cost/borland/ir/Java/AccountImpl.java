// AccountImpl.java

import java.util.Date;

// Class that implements the Bank.IDL Account interface.
// Note the commented "describe" method in that interface.
// It is implemented as well in two places:
//  1.the describe method: void String describe () {...}
//  2.the _invoke method: if (opName.equals("describe")) {...}
public class AccountImpl extends Bank.AccountPOA {
	String _name;
	float _balance;
  java.util.Date _created;
  java.util.Date _modified;

  AccountImpl (String name, float balance) {
    _name = name;
    _balance = balance;
    _modified = _created = new Date();
  }

  public float balance () {
    return _balance;
  }

  public synchronized void balance (float balance) {
    _balance = balance;
    _modified = new Date();
  }

  // This method is specific for this particular implementation. But it would
  // be nice if clients could access to it
  public String describe () {
    return "Account name:" + _name + "\n" +
           "Balance     :" + _balance + "\n" +
           "Created     :" + _created + "\n" +
           "Modified    :" + _modified + "\n";
  }

  public org.omg.CORBA.portable.OutputStream _invoke (java.lang.String opName,
                                                      org.omg.CORBA.portable.InputStream _input,
                                                      org.omg.CORBA.portable.ResponseHandler handler) {
    // Check the opName. If it is "describe" invoke our describe method
    // and return its result to user via org.omg.CORBA.portable.OutputStream
    if (opName.equals("describe")) {
      String desc = describe();
      org.omg.CORBA.portable.OutputStream _output = handler.createReply();
      _output.write_string(desc);
      return _output;
    }

    // In all other cases let the Bank.AccountPOA _invoke handle them
    return super._invoke(opName, _input, handler);
  }
}
