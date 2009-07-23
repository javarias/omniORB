import org.omg.PortableServer.*;
import org.omg.PortableServer.ServantLocatorPackage.CookieHolder;

public class AccountManagerLocator extends ServantLocatorPOA {

  public Servant preinvoke (byte[] oid,POA adapter,
                            java.lang.String operation,
                            CookieHolder the_cookie) throws ForwardRequest {
    String accountType = new String(oid);
    System.out.println("\nAccountManagerLocator.preinvoke called with ID = " + accountType + "\n");
    if ( accountType.equalsIgnoreCase("SavingsAccountManager"))
        return new SavingsAccountManagerImpl();

    return new CheckingAccountManagerImpl();
   }

  public void postinvoke (byte[] oid,
                          POA adapter,
                          java.lang.String operation,
                          java.lang.Object the_cookie,
                          Servant the_servant) {
    System.out.println("\nAccountManagerLocator.postinvoke called with ID = " + new String(oid) + "\n");
    }

}
