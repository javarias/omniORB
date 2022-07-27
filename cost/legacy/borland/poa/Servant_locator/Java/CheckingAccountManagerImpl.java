// CheckingAccountManagerImpl.java

import java.lang.String;

public class CheckingAccountManagerImpl extends AccountManagerImpl {
  public synchronized Bank.Account open(String name) {
    String newName = name + "_" + "Checking";
    return super.open( newName );
  }
}

