// SavingsAccountManagerImpl.java
import java.lang.String;

public class SavingsAccountManagerImpl extends AccountManagerImpl {
  public synchronized Bank.Account open(String name) {
    String newName = name + "_" + "Savings";
    return super.open( newName );
  }
}

