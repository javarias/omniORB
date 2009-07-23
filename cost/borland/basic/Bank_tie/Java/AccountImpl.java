// AccountImpl.java

public class AccountImpl implements Bank.AccountOperations {
  public AccountImpl(float balance) {
    _balance = balance;
  }
  public float balance() {
    return _balance;
  }
  private float _balance;
}

