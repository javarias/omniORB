// AccountImpl.java

public class AccountImpl extends Bank.AccountPOA {
  public AccountImpl(float balance) {
    _balance = balance;
  }
  public float balance() {
    return _balance;
  }
  private float _balance;
}

